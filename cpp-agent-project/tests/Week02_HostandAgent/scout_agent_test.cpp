// Week 6 tests for *our own* ScoutAgent. No StupidAgent comparison.
#include "agent_api.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
int checks = 0;
void check(bool condition, const char* name) {
    ++checks;
    if (!condition) throw std::runtime_error(std::string("FAIL: ") + name);
    std::cout << "[PASS] " << name << '\n';
}
WorldSnapshot base() {
    WorldSnapshot s{};
    s.cells[2][2] = CELLTYPE_FLOOR | CELLCONTENT_SELF;
    return s;
}
void put(WorldSnapshot& s, int y, int x, uint32_t terrain, uint32_t content = 0) {
    s.cells[y][x] = terrain | content;
}
} // namespace

int main() {
    try {
        check(std::strcmp(Agent_GetName(), "ScoutAgent") == 0, "Own ScoutAgent name");
        WorldSnapshot s = base();
        check(Agent_Step(nullptr, &s, ACTION_WAIT) == ACTION_WAIT, "Null instance -> wait");
        AgentInstance* a = Agent_Create(80, 50);
        check(a != nullptr, "Create instance");
        check(Agent_Step(a, nullptr, ACTION_WAIT) == ACTION_WAIT, "Null snapshot -> wait");
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_WAIT, "Unknown neighbors -> wait");

        s = base();
        put(s, 1, 2, CELLTYPE_FLOOR);
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_NORTH, "Only north floor -> north");
        s = base();
        put(s, 1, 2, CELLTYPE_WALL);
        put(s, 2, 3, CELLTYPE_FLOOR);
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_EAST, "Wall north, east floor -> east");
        s = base();
        put(s, 2, 3, CELLTYPE_FLOOR, CELLCONTENT_ENEMY);
        put(s, 3, 2, CELLTYPE_FLOOR);
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_SOUTH, "Avoid visible occupied neighbor");

        Agent_Reset(a);
        s = base();
        put(s, 1, 2, CELLTYPE_FLOOR);
        put(s, 2, 3, CELLTYPE_FLOOR);
        put(s, 2, 4, CELLTYPE_FLOOR, CELLCONTENT_ENEMY_FLAG);
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_EAST,
              "Local BFS heads east to visible enemy flag");
        s = base();
        s.cells[2][2] |= CELLCONTENT_ENEMY_FLAG;
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_PICK_UP_FLAG,
              "Request pickup when standing on enemy flag (host rule unverified)");

        Agent_Reset(a);
        s = base();
        put(s, 1, 2, CELLTYPE_FLOOR);
        put(s, 2, 3, CELLTYPE_FLOOR);
        bool allLegal = true;
        for (int i = 0; i < 8; ++i) {
            uint32_t next = Agent_Step(a, &s, ACTION_WAIT);
            allLegal &= (next == ACTION_MOVE_NORTH || next == ACTION_MOVE_EAST);
        }
        check(allLegal, "Repeated calls only request observed legal neighboring floor");

        Agent_Reset(a);
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_NORTH,
              "Reset clears learned local choice counts");

        // Goal routing must not walk through unknown cells, walls or opponents.
        Agent_Reset(a);
        s = base();
        for (int y = 0; y < VIEW_SIZE; ++y)
            for (int x = 0; x < VIEW_SIZE; ++x)
                s.cells[y][x] = CELLTYPE_FLOOR;
        s.cells[2][2] |= CELLCONTENT_SELF;
        put(s, 2, 3, CELLTYPE_WALL);
        s.cells[2][4] |= CELLCONTENT_ENEMY_FLAG;
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_NORTH,
              "Local BFS detours around an east-side wall");

        Agent_Reset(a);
        s = base();
        put(s, 2, 3, CELLTYPE_FLOOR, CELLCONTENT_ENEMY | CELLCONTENT_ENEMY_FLAG);
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_WAIT,
              "Occupied visible flag is not treated as walkable");

        Agent_Reset(a);
        s = base();
        s.cells[2][2] |= CELLCONTENT_OWN_FLAG;
        put(s, 1, 2, CELLTYPE_FLOOR);
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_NORTH,
              "Own flag does not trigger enemy-flag pickup");

        Agent_Reset(a);
        s = base();
        s.cells[2][2] |= CELLCONTENT_ENEMY_FLAG;
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_PICK_UP_FLAG,
              "First visible center-flag pickup is requested");
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_WAIT,
              "Pickup cooldown avoids immediate repeated pickup requests");
        Agent_Reset(a);
        check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_PICK_UP_FLAG,
              "Reset clears pickup cooldown");
        Agent_Destroy(a);
        Agent_Reset(nullptr);
        Agent_Destroy(nullptr);
        check(true, "Destroy and null lifecycle calls safe");
        std::cout << "SUCCESS: " << checks << " ScoutAgent unit checks passed.\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
