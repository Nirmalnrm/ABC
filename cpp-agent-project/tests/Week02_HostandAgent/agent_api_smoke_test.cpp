#include "agent_api.h"
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
int tests_run = 0;

void check(bool ok, const std::string& what) {
    ++tests_run;
    if (!ok) throw std::runtime_error("FAILED: " + what);
    std::cout << "[PASS] " << what << '\n';
}

WorldSnapshot snapshot() {
    WorldSnapshot s{}; // Unknown everywhere except the agent's center.
    s.cells[2][2] = CELLTYPE_FLOOR | CELLCONTENT_SELF;
    return s;
}
#if defined(EXPECT_SAFE_AGENT)
void put(WorldSnapshot& s, int y, int x, uint32_t terrain, uint32_t contents = CELLCONTENT_NONE) {
    s.cells[y][x] = terrain | contents;
}


#endif

void common_api_tests() {
    check(Agent_GetName() != nullptr && std::strlen(Agent_GetName()) > 0,
          "Agent_GetName returns a nonempty name");
    WorldSnapshot s = snapshot();
    check(Agent_Step(nullptr, &s, ACTION_WAIT) == ACTION_WAIT,
          "Null agent instance is handled safely");
    AgentInstance* a = Agent_Create(25, 25);
    check(a != nullptr, "Agent_Create returns an instance");
    check(Agent_Step(a, nullptr, ACTION_WAIT) == ACTION_WAIT,
          "Null WorldSnapshot is handled safely");
    Agent_Reset(a);
    Agent_Destroy(a);
    Agent_Reset(nullptr);
    Agent_Destroy(nullptr);
    check(true, "Reset/Destroy lifecycle calls do not crash");
}

#if defined(EXPECT_SAFE_AGENT)
void safe_agent_tests() {
    check(std::strcmp(Agent_GetName(), "SafeMovementAgent") == 0,
          "Safe agent name matches source");
    AgentInstance* a = Agent_Create(25,25);
    if (!a) throw std::runtime_error("Cannot create SafeMovementAgent");
    std::srand(1234);

    WorldSnapshot s = snapshot();
    check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_WAIT,
          "All four unknown neighbors -> ACTION_WAIT");

    s = snapshot();
    put(s, 1, 2, CELLTYPE_FLOOR);
    check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_NORTH,
          "Only north floor -> north");

    s = snapshot();
    put(s, 2, 1, CELLTYPE_FLOOR);
    check(Agent_Step(a, &s, ACTION_MOVE_EAST) == ACTION_MOVE_WEST,
          "Only west floor -> west, even if reversing is necessary");

    s = snapshot();
    put(s, 1, 2, CELLTYPE_WALL);
    put(s, 2, 3, CELLTYPE_FLOOR);
    check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_EAST,
          "Wall north and floor east -> east");

    s = snapshot();
    put(s, 2, 3, CELLTYPE_FLOOR, CELLCONTENT_ENEMY);
    put(s, 2, 1, CELLTYPE_FLOOR);
    check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_WEST,
          "Occupied east and floor west -> west");

    s = snapshot();
    put(s, 3, 2, CELLTYPE_FLOOR);
    check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_MOVE_SOUTH,
          "Unknown north and floor south -> south");

    s = snapshot();
    put(s, 1, 2, CELLTYPE_FLOOR);
    put(s, 2, 3, CELLTYPE_FLOOR);
    check(Agent_Step(a, &s, ACTION_MOVE_SOUTH) == ACTION_MOVE_EAST,
          "Disfavors reverse of previous south request (north) when east is open");

    s = snapshot();
    put(s, 1, 2, CELLTYPE_WALL);
    put(s, 2, 3, CELLTYPE_WALL);
    put(s, 3, 2, CELLTYPE_WALL);
    put(s, 2, 1, CELLTYPE_WALL);
    check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_WAIT,
          "All four known walls -> ACTION_WAIT");

    s = snapshot();
    put(s, 1, 1, CELLTYPE_FLOOR); // NW is diagonal only.
    check(Agent_Step(a, &s, ACTION_WAIT) == ACTION_WAIT,
          "Diagonal-only floor -> still wait (safe agent is cardinal-only)");

    Agent_Reset(a);
    Agent_Destroy(a);
}

#endif

#if defined(EXPECT_STUPID_AGENT)
void stupid_agent_tests() {
    check(std::strcmp(Agent_GetName(), "StupidAgent") == 0,
          "Random baseline agent name matches source");
    AgentInstance* a = Agent_Create(25,25);
    if (!a) throw std::runtime_error("Cannot create StupidAgent");
    std::srand(1234);
    WorldSnapshot s = snapshot();
    bool all_in_range = true;
    bool used_diagonal = false;
    for (int i=0; i<100; ++i) {
        uint32_t action = Agent_Step(a, &s, ACTION_WAIT);
        if (action < ACTION_MOVE_NORTH || action > ACTION_MOVE_NORTHWEST) all_in_range = false;
        if (action == ACTION_MOVE_NORTHEAST || action == ACTION_MOVE_SOUTHEAST ||
            action == ACTION_MOVE_SOUTHWEST || action == ACTION_MOVE_NORTHWEST) used_diagonal = true;
    }
    check(all_in_range, "100 random actions are legal movement action codes");
    check(used_diagonal, "Random baseline samples diagonal directions");
    // The snapshot has no known traversable neighbors; StupidAgent still requests movement.
    uint32_t action = Agent_Step(a, &s, ACTION_WAIT);
    check(action >= ACTION_MOVE_NORTH && action <= ACTION_MOVE_NORTHWEST,
          "Baseline requests movement even with no known floor neighbors");
    Agent_Reset(a);
    Agent_Destroy(a);
}

#endif
} // namespace

int main() {
    try {
        common_api_tests();
#ifdef EXPECT_SAFE_AGENT
        safe_agent_tests();
#elif defined(EXPECT_STUPID_AGENT)
        stupid_agent_tests();
#else
#error Define EXPECT_SAFE_AGENT or EXPECT_STUPID_AGENT
#endif
        std::cout << "\nSUCCESS: " << tests_run << " checks passed.\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
