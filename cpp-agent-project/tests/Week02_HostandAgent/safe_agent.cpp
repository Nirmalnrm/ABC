// Compilation and linking:
// g++ -c stupid_agent.cpp
// g++ -o stupid_agent.dll stupid_agent.o -shared -Wl,--subsystem,windows


// ------------------------------------------------------------
// includes
// ------------------------------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include "agent_api.h"


// ------------------------------------------------------------
// constants
// ------------------------------------------------------------

#define AGENT_NAME "SafeMovementAgent"


// ------------------------------------------------------------
// data types
// ------------------------------------------------------------


struct AgentInstance
{
    int rounds = 0;
    unsigned int mapWidth = 0;
    unsigned int mapHeight = 0;
    /* put your agent's data structures here -> the content is being remembered across calls */
};


// ------------------------------------------------------------
// functions (where to implement your agent)
// ------------------------------------------------------------


AgentInstance* Create(uint32_t mapWidth, uint32_t mapHeight)
{
    AgentInstance* instance = new AgentInstance();
    instance->mapWidth = mapWidth;
    instance->mapHeight = mapHeight;

    printf("AGENT: initialized data structures, map dimensions %u x %u\n", mapWidth, mapHeight);

    return instance;
}


void Destroy(AgentInstance* instance)
{
    printf("AGENT: cleaning up memory\n");

    delete instance;
}


void Reset(AgentInstance* instance)
{
    printf("AGENT: performing reset of internal states\n");

    instance->rounds = 0;
}


uint32_t Step(AgentInstance* instance, const WorldSnapshot* snapshot, uint32_t lastAction)
{
    ++instance->rounds;
    // uint32_t action = ACTION_MOVE_NORTH + (rand() % 8);
        
    /*
    example to access the cell left (west) of the player
    (which is centered in the view at position [y = 2][x = 2]):
    uint32_t cell = snapshot->cells[2][1];
    */
    struct MoveOption { int y; int x; uint32_t action; };
    const MoveOption moves[4] = {
        {1, 2, ACTION_MOVE_NORTH},
        {2, 3, ACTION_MOVE_EAST},
        {3, 2, ACTION_MOVE_SOUTH},
        {2, 1, ACTION_MOVE_WEST}
    };

    uint32_t reverse = ACTION_WAIT;
    switch (lastAction) {
        case ACTION_MOVE_NORTH: reverse = ACTION_MOVE_SOUTH; break;
        case ACTION_MOVE_EAST:  reverse = ACTION_MOVE_WEST;  break;
        case ACTION_MOVE_SOUTH: reverse = ACTION_MOVE_NORTH; break;
        case ACTION_MOVE_WEST:  reverse = ACTION_MOVE_EAST;  break;
    }

    uint32_t valid[4], preferred[4];
    int validCount = 0, preferredCount = 0;

    for (const MoveOption& move : moves) {
        uint32_t cell = snapshot->cells[move.y][move.x];
        bool floor = (cell & CELLTYPE_BITMASK) == CELLTYPE_FLOOR;
        bool occupied = (cell & (CELLCONTENT_SELF | CELLCONTENT_ENEMY)) != 0;
        if (floor && !occupied) {
            valid[validCount++] = move.action;
            if (move.action != reverse) {
                preferred[preferredCount++] = move.action;
            }
        }
    }

    uint32_t action = ACTION_WAIT;
    if (preferredCount > 0) {
        action = preferred[rand() % preferredCount];
    } else if (validCount > 0) {
        action = valid[rand() % validCount];
    }

    printf("AGENT: I have been called %u times, last action %u, returning new action: %u\n", instance->rounds, lastAction, action);

    return action;
}


// ------------------------------------------------------------
// DLL function interface (do not modify)
// ------------------------------------------------------------


extern "C" {

AGENT_API const char* AGENT_CALL Agent_GetName()
{
    return AGENT_NAME;
}


AGENT_API AgentInstance* AGENT_CALL Agent_Create(uint32_t mapWidth, uint32_t mapHeight)
{
    return Create(mapWidth, mapHeight);
}


AGENT_API void AGENT_CALL Agent_Destroy(AgentInstance* instance)
{
    if (instance) {
        Destroy(instance);
    }
}


AGENT_API void AGENT_CALL Agent_Reset(AgentInstance* instance)
{
    if (instance) {
        Reset(instance);
    }
}


AGENT_API uint32_t AGENT_CALL Agent_Step(AgentInstance* instance, const WorldSnapshot* snapshot, uint32_t lastAction)
{
    if ((instance) && (snapshot)) {
        return Step(instance, snapshot, lastAction);
    } else {
        return ACTION_WAIT;
    }
}

} // extern "C"
