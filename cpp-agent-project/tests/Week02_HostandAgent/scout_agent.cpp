// Week 6: Own, observation-driven ScoutAgent prototype.
// Build on Windows (MSYS2 UCRT64): g++ -std=c++17 -O2 -Wall -Wextra -shared scout_agent.cpp -o scout_agent.dll
// Uses only the professor-provided agent_api.h and the 5x5 WorldSnapshot.
// Does not assume lastAction succeeded, know its absolute position, or have a global map.

#include "agent_api.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <unordered_map>

namespace {
constexpr int CENTER = VIEW_SIZE / 2;
constexpr int DIRS = 4;
constexpr int DY[DIRS] = {-1, 0, 1, 0};
constexpr int DX[DIRS] = {0, 1, 0, -1};
constexpr uint32_t ACTIONS[DIRS] = {
    ACTION_MOVE_NORTH, ACTION_MOVE_EAST, ACTION_MOVE_SOUTH, ACTION_MOVE_WEST
};

bool in_view(int y, int x) {
    return y >= 0 && y < VIEW_SIZE && x >= 0 && x < VIEW_SIZE;
}

bool walkable(uint32_t cell) {
    return (cell & CELLTYPE_BITMASK) == CELLTYPE_FLOOR &&
           (cell & (CELLCONTENT_ENEMY | CELLCONTENT_SELF)) == 0;
}

uint64_t terrain_fingerprint(const WorldSnapshot& snapshot) {
    // FNV-1a over observable terrain only: dynamic opponent positions do not
    // become a remembered, permanent wall. This is NOT a global cell address.
    uint64_t hash = UINT64_C(14695981039346656037);
    for (int y = 0; y < VIEW_SIZE; ++y) {
        for (int x = 0; x < VIEW_SIZE; ++x) {
            hash ^= static_cast<uint64_t>(snapshot.cells[y][x] & CELLTYPE_BITMASK);
            hash *= UINT64_C(1099511628211);
        }
    }
    return hash;
}

uint64_t virtual_key(int x, int y) {
    // Key for a tentative displacement history; NOT a verified map coordinate.
    return (static_cast<uint64_t>(static_cast<uint32_t>(x)) << 32) |
           static_cast<uint32_t>(y);
}

int move_index(uint32_t action) {
    for (int d = 0; d < DIRS; ++d) {
        if (ACTIONS[d] == action) return d;
    }
    return -1;
}

// Find the FIRST action on a shortest known-floor path to a visible enemy flag.
// All search nodes must be inside this observation, never in an imagined map.
uint32_t action_to_visible_flag(const WorldSnapshot& snapshot) {
    std::queue<std::pair<int, int>> q;
    bool seen[VIEW_SIZE][VIEW_SIZE] = {};
    uint32_t first_action[VIEW_SIZE][VIEW_SIZE] = {};
    q.push({CENTER, CENTER});
    seen[CENTER][CENTER] = true;

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();
        if (snapshot.cells[y][x] & CELLCONTENT_ENEMY_FLAG) {
            // Exact pickup conditions are still subject to host verification.
            // An issued pickup action does NOT prove that the flag was obtained.
            if (y == CENTER && x == CENTER) return ACTION_PICK_UP_FLAG;
            return first_action[y][x];
        }
        for (int d = 0; d < DIRS; ++d) {
            const int ny = y + DY[d], nx = x + DX[d];
            if (!in_view(ny, nx) || seen[ny][nx] || !walkable(snapshot.cells[ny][nx])) {
                continue;
            }
            seen[ny][nx] = true;
            first_action[ny][nx] = (y == CENTER && x == CENTER)
                ? ACTIONS[d] : first_action[y][x];
            q.push({ny, nx});
        }
    }
    return ACTION_WAIT;  // No visible *reachable* enemy flag; explore locally.
}

} // namespace

struct AgentInstance {
    uint32_t mapWidth = 0;
    uint32_t mapHeight = 0;
    uint32_t turns = 0;
    uint32_t lastChosen = ACTION_WAIT;
    uint32_t pickupCooldown = 0;
    int tentativeX = 0, tentativeY = 0;
    std::unordered_map<uint64_t, uint32_t> tentativeVisits;
    // The tentative location is built from action REQUESTS, not confirmed motion.
    // It ONLY biases local exploration. Never use it for global BFS or map writes.
    // How often each action was requested from the SAME local terrain pattern.
    // This reduces repeated choices, but is not position-based visit counting.
    std::unordered_map<uint64_t, std::array<uint32_t, DIRS>> choicesByView;
};

extern "C" {

AGENT_API const char* AGENT_CALL Agent_GetName() {
    return "ScoutAgent";
}

AGENT_API AgentInstance* AGENT_CALL Agent_Create(uint32_t mapWidth, uint32_t mapHeight) {
    AgentInstance* agent = new AgentInstance();
    agent->mapWidth = mapWidth;
    agent->mapHeight = mapHeight;
    agent->tentativeVisits[virtual_key(0, 0)] = 1;
    return agent;
}

AGENT_API void AGENT_CALL Agent_Reset(AgentInstance* agent) {
    if (!agent) return;
    agent->turns = 0;
    agent->lastChosen = ACTION_WAIT;
    agent->pickupCooldown = 0;
    agent->tentativeX = agent->tentativeY = 0;
    agent->tentativeVisits.clear();
    agent->tentativeVisits[virtual_key(0, 0)] = 1;
    agent->choicesByView.clear();
}

AGENT_API void AGENT_CALL Agent_Destroy(AgentInstance* agent) {
    delete agent;
}

AGENT_API uint32_t AGENT_CALL Agent_Step(
    AgentInstance* agent, const WorldSnapshot* snapshot, uint32_t lastAction) {
    if (!agent || !snapshot) return ACTION_WAIT;
    ++agent->turns;

    // Objective: a shortest safe route inside the current 5x5 view.
    // Pickup is only REQUESTED, never interpreted as confirmation of possession.
    // After one request, pause objective chasing so an unacknowledged pickup
    // cannot trap the agent in an infinite pickup loop.
    if (agent->pickupCooldown > 0) --agent->pickupCooldown;
    else {
        uint32_t objectiveAction = action_to_visible_flag(*snapshot);
        if (objectiveAction != ACTION_WAIT) {
            if (objectiveAction == ACTION_PICK_UP_FLAG) agent->pickupCooldown = 24;
            else {
                int d = move_index(objectiveAction);
                if (d >= 0) {
                    agent->tentativeX += DX[d];
                    agent->tentativeY += DY[d];
                    ++agent->tentativeVisits[virtual_key(agent->tentativeX, agent->tentativeY)];
                }
            }
            agent->lastChosen = objectiveAction;
            return objectiveAction;
        }
    }

    const uint64_t signature = terrain_fingerprint(*snapshot);
    auto& choices = agent->choicesByView[signature];
    const int lastDirection = move_index(lastAction);

    int bestDirection = -1;
    int bestScore = -100000000;
    for (int d = 0; d < DIRS; ++d) {
        const int y = CENTER + DY[d], x = CENTER + DX[d];
        if (!walkable(snapshot->cells[y][x])) continue;

        // Score hypothetical, not confirmed, new locations. This is a bounded
        // navigation heuristic, not persistent mapping or actual visit counts.
        int score = 100;
        uint32_t virtualVisits = agent->tentativeVisits[
            virtual_key(agent->tentativeX + DX[d], agent->tentativeY + DY[d])];
        score -= static_cast<int>(virtualVisits > 100 ? 100 : virtualVisits) * 25;
        score -= static_cast<int>(choices[d] > 100 ? 100 : choices[d]) * 2;
        if (lastDirection >= 0 && d == (lastDirection + 2) % DIRS) score -= 13;
        if (agent->lastChosen == ACTIONS[d]) score += 18;
        const int fy = CENTER + 2 * DY[d], fx = CENTER + 2 * DX[d];
        if (in_view(fy, fx) && walkable(snapshot->cells[fy][fx])) score += 7;
        if (score > bestScore) {
            bestScore = score;
            bestDirection = d;
        }
    }

    if (bestDirection < 0) {
        agent->lastChosen = ACTION_WAIT;
        return ACTION_WAIT;
    }
    ++choices[bestDirection];
    // Tentative displacement only; a blocked/ambiguous host move could drift.
    agent->tentativeX += DX[bestDirection];
    agent->tentativeY += DY[bestDirection];
    ++agent->tentativeVisits[virtual_key(agent->tentativeX, agent->tentativeY)];
    agent->lastChosen = ACTIONS[bestDirection];
    return agent->lastChosen;
}

} // extern "C"
