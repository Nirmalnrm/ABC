// Compilation and linking to a DLL using GNU C Compiler:
// g++ -c simple_agent.cpp
// g++ -o simple_agent.dll simple_agent.o -shared -Wl,--subsystem,windows


// ------------------------------------------------------------
// includes
// ------------------------------------------------------------


#include "agent_api.h"
#include <stdio.h>
#include <vector>
#include <numeric>


// ------------------------------------------------------------
// data types
// ------------------------------------------------------------


struct AgentInstance
{
    uint32_t rounds = 0;
    std::vector<uint32_t> data;
};


// ------------------------------------------------------------
// functions
// ------------------------------------------------------------


uint32_t Step(AgentInstance* instance, uint32_t data)
{
    ++instance->rounds;
    instance->data.push_back(data);
    uint32_t sum = std::accumulate(instance->data.begin(), instance->data.end(), 0);
    printf("AGENT: I have been called %u times, argument was %u, returning total sum: %u\n", instance->rounds, data, sum);

    return sum;
}


// ------------------------------------------------------------
// DLL function interface
// ------------------------------------------------------------


extern "C" {

AGENT_API const char* AGENT_CALL Agent_GetName()
{
    return "SimpleAgent";
}


AGENT_API AgentInstance* AGENT_CALL Agent_Create()
{
    return new AgentInstance();
}


AGENT_API void AGENT_CALL Agent_Destroy(AgentInstance* instance)
{
    delete instance;
}


AGENT_API uint32_t AGENT_CALL Agent_Step(AgentInstance* instance, uint32_t data)
{
    if (instance) {
        return Step(instance, data);
    } else {
        return 0;
    }    
}

} // extern "C"
