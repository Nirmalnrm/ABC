// A transparent OFFLINE map simulator, not a modification to host.exe.
// Measures *our own* agent's actions on developer-authored ASCII map fixtures.
#include "agent_api.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>
#include <utility>

struct Map {
    std::string name;
    std::vector<std::string> rows;
    int x = -1, y = -1, fx = -1, fy = -1;
    int walkable = 0;
};

Map load_map(const std::string& file) {
    Map m;
    m.name = file;
    std::ifstream input(file);
    if (!input) throw std::runtime_error("Cannot open " + file);
    for (std::string line; std::getline(input, line);) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (!line.empty()) m.rows.push_back(line);
    }
    if (m.rows.empty()) throw std::runtime_error("Empty map " + file);
    size_t width = m.rows[0].size();
    for (int y = 0; y < static_cast<int>(m.rows.size()); ++y) {
        if (m.rows[y].size() != width) throw std::runtime_error("Unequal line widths in " + file);
        for (int x = 0; x < static_cast<int>(width); ++x) {
            char c = m.rows[y][x];
            if (c != '#' && c != '.' && c != 'S' && c != 'F')
                throw std::runtime_error("Unsupported map character");
            if (c != '#') ++m.walkable;
            if (c == 'S') {
                if (m.x >= 0) throw std::runtime_error("Multiple starts");
                m.x = x; m.y = y;
            }
            if (c == 'F') {
                if (m.fx >= 0) throw std::runtime_error("Multiple flags");
                m.fx = x; m.fy = y;
            }
        }
    }
    if (m.x < 0) throw std::runtime_error("Missing start");
    return m;
}

WorldSnapshot observe(const Map& m) {
    WorldSnapshot s{};
    for (int y = 0; y < VIEW_SIZE; ++y) {
        for (int x = 0; x < VIEW_SIZE; ++x) {
            int gx = m.x + x - VIEW_SIZE / 2;
            int gy = m.y + y - VIEW_SIZE / 2;
            if (gx < 0 || gy < 0 || gy >= static_cast<int>(m.rows.size()) ||
                gx >= static_cast<int>(m.rows[0].size())) continue;
            char c = m.rows[gy][gx];
            s.cells[y][x] = c == '#' ? CELLTYPE_WALL : CELLTYPE_FLOOR;
            if (c == 'F') s.cells[y][x] |= CELLCONTENT_ENEMY_FLAG;
            if (x == VIEW_SIZE / 2 && y == VIEW_SIZE / 2) s.cells[y][x] |= CELLCONTENT_SELF;
        }
    }
    return s;
}

void run_fixture(const std::string& path, int maxSteps, int minUnique, bool expectFlag = true) {
    Map m = load_map(path);
    AgentInstance* agent = Agent_Create(static_cast<uint32_t>(m.rows[0].size()),
                                        static_cast<uint32_t>(m.rows.size()));
    if (!agent) throw std::runtime_error("Agent_Create failed");
    std::set<std::pair<int, int>> visited;
    visited.insert({m.x, m.y});
    int blocked = 0, waits = 0, pickupRequests = 0, flagReachedAt = -1;
    uint32_t last = ACTION_WAIT;
    for (int step = 0; step < maxSteps; ++step) {
        WorldSnapshot s = observe(m);
        uint32_t action = Agent_Step(agent, &s, last);
        int dx = 0, dy = 0;
        switch (action) {
            case ACTION_MOVE_NORTH: dy = -1; break;
            case ACTION_MOVE_EAST: dx = 1; break;
            case ACTION_MOVE_SOUTH: dy = 1; break;
            case ACTION_MOVE_WEST: dx = -1; break;
            case ACTION_WAIT: ++waits; break;
            case ACTION_PICK_UP_FLAG:
                ++pickupRequests;
                if (m.x != m.fx || m.y != m.fy) {
                    Agent_Destroy(agent);
                    throw std::runtime_error("Pickup requested away from flag on " + path);
                }
                break;
            default:
                Agent_Destroy(agent);
                throw std::runtime_error("Unexpected action code on " + path);
        }
        if (dx || dy) {
            int nx = m.x + dx, ny = m.y + dy;
            bool legal = nx >= 0 && ny >= 0 && ny < static_cast<int>(m.rows.size()) &&
                         nx < static_cast<int>(m.rows[0].size()) && m.rows[ny][nx] != '#';
            if (!legal) ++blocked;
            else { m.x = nx; m.y = ny; visited.insert({m.x, m.y}); }
        }
        if (flagReachedAt < 0 && m.x == m.fx && m.y == m.fy) flagReachedAt = step + 1;
        last = action;  // The API's lastAction is the REQUEST, not verified movement.
    }
    Agent_Destroy(agent);
    std::cout << "[MAP] " << path << " | steps=" << maxSteps
              << " | visited=" << visited.size() << '/' << m.walkable
              << " | blocked_requests=" << blocked
              << " | waits=" << waits
              << " | flag_cell_first_reached=" << flagReachedAt
              << " | pickup_requests=" << pickupRequests << '\n';
    if (blocked != 0) throw std::runtime_error("Requested a blocked move in " + path);
    if (static_cast<int>(visited.size()) < minUnique)
        throw std::runtime_error("Did not explore expected minimum distinct floor cells on " + path);
    // Flag-cell arrival/pickup *request* are offline simulator goals, not game wins.
    if (expectFlag && (flagReachedAt < 0 || pickupRequests == 0))
        throw std::runtime_error("Did not reach flag cell and request pickup on " + path);
}

int main(int argc, char** argv) {
    try {
        if (argc != 2) throw std::runtime_error("Usage: ./map_simulator_test.exe week06_maps");
        std::string base = argv[1];
        run_fixture(base + "/open_arena.map", 180, 50);
        run_fixture(base + "/corridor_turns.map", 180, 30);
        run_fixture(base + "/branching_deadends.map", 180, 30);
        run_fixture(base + "/blocked_start.map", 12, 1, false);
        run_fixture(base + "/two_rooms.map", 180, 50);
        run_fixture(base + "/zigzag_barriers.map", 180, 70);
        // reserved_holdout.map is intentionally NOT used during development.
        std::cout << "SUCCESS: 6 development map fixtures passed safety/coverage checks.\n";
        std::cout << "NOTE: This is an offline simulator; host.exe behavior and flag acquisition are NOT verified.\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "[FAIL] " << ex.what() << '\n';
        return 1;
    }
}
