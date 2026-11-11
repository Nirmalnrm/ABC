// ------------------------------------------------------------
// University of Applied Sciences Zwickau
// Joint project module: Software Project (ELT10210)
// and Autonomous Research Project (KFT11050)
//
// Agent API
// Version 1.0 (2026-10-09)
// ------------------------------------------------------------


#pragma once


// ------------------------------------------------------------
// includes
// ------------------------------------------------------------


#include <stdint.h>


// ------------------------------------------------------------
// constants
// ------------------------------------------------------------


#define VIEW_SIZE                   5


#define CELLTYPE_BITMASK            0x00FF
#define CELLTYPE_UNKNOWN            0x0000  /* unknown, not visible, or outside the map borders */
#define CELLTYPE_FLOOR              0x0001  /* floor, agents can move here */
#define CELLTYPE_WALL               0x0002  /* wall */


#define CELLCONTENT_BITMASK         0xFF00
#define CELLCONTENT_NONE            0x0000
#define CELLCONTENT_SELF            0x0100  /* your agent is on the cell */
#define CELLCONTENT_ENEMY           0x0200  /* enemy agent is on the cell */
#define CELLCONTENT_OWN_FLAG        0x0400  /* your flag is on the cell (the enemy wants to capture it) */
#define CELLCONTENT_ENEMY_FLAG      0x0800  /* the flag of the enemy is on the cell (you want to capture it) */
#define CELLCONTENT_OWN_TARGET      0x1000  /* this is the target where to put down your captured flag */
#define CELLCONTENT_ENEMY_TARGET    0x2000  /* the enemy will have to put down his captured flag here */


#define ACTION_WAIT                 0x00
#define ACTION_MOVE_NORTH           0x11
#define ACTION_MOVE_NORTHEAST       0x12
#define ACTION_MOVE_EAST            0x13
#define ACTION_MOVE_SOUTHEAST       0x14
#define ACTION_MOVE_SOUTH           0x15
#define ACTION_MOVE_SOUTHWEST       0x16
#define ACTION_MOVE_WEST            0x17
#define ACTION_MOVE_NORTHWEST       0x18
#define ACTION_PICK_UP_FLAG         0x21
#define ACTION_PUT_DOWN_FLAG        0x22


// ------------------------------------------------------------
// data types
// ------------------------------------------------------------


struct WorldSnapshot {
    uint32_t cells[VIEW_SIZE][VIEW_SIZE];  /* note that cells are stored as cells[row][column], i.e., cells[y][x] */
};


struct AgentInstance;  /* only forward declaration, actual declaration is done in your agent .cpp file */


// ------------------------------------------------------------
// DLL function interface
// ------------------------------------------------------------


#ifdef _WIN32
    #define AGENT_API __declspec(dllexport)
    #define AGENT_CALL __cdecl
#else
    #define AGENT_API
    #define AGENT_CALL
#endif


#if defined(__cplusplus)
extern "C" {
#endif
AGENT_API const char* AGENT_CALL Agent_GetName();
AGENT_API AgentInstance* AGENT_CALL Agent_Create(uint32_t mapWidth, uint32_t mapHeight);
AGENT_API void AGENT_CALL Agent_Destroy(AgentInstance* instance);
AGENT_API void AGENT_CALL Agent_Reset(AgentInstance* instance);
AGENT_API uint32_t AGENT_CALL Agent_Step(AgentInstance* instance, const WorldSnapshot* snapshot, uint32_t lastAction);

#if defined(__cplusplus)
}
#endif
