#pragma once


// ------------------------------------------------------------
// includes
// ------------------------------------------------------------


#include <stdint.h>


// ------------------------------------------------------------
// data types
// ------------------------------------------------------------


struct AgentInstance;


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
AGENT_API AgentInstance* AGENT_CALL Agent_Create();
AGENT_API void AGENT_CALL Agent_Destroy(AgentInstance* instance);
AGENT_API uint32_t AGENT_CALL Agent_Step(AgentInstance* instance, uint32_t data);

#if defined(__cplusplus)
}
#endif
