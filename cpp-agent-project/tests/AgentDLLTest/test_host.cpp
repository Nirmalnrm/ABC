#include <windows.h>
#include <stdint.h>
#include <iostream>

struct AgentInstance;

using GetNameFn = const char* (__cdecl *)();
using CreateFn = AgentInstance* (__cdecl *)();
using DestroyFn = void (__cdecl *)(AgentInstance*);
using StepFn = uint32_t (__cdecl *)(AgentInstance*, uint32_t);

int main() {
    HMODULE dll = LoadLibraryA("simple_agent.dll");

    if (!dll) {
        std::cerr << "Failed to load DLL. Windows error: "
                  << GetLastError() << '\n';
        return 1;
    }

    auto getName = reinterpret_cast<GetNameFn>(
        GetProcAddress(dll, "Agent_GetName"));
    auto create = reinterpret_cast<CreateFn>(
        GetProcAddress(dll, "Agent_Create"));
    auto destroy = reinterpret_cast<DestroyFn>(
        GetProcAddress(dll, "Agent_Destroy"));
    auto step = reinterpret_cast<StepFn>(
        GetProcAddress(dll, "Agent_Step"));

    if (!getName || !create || !destroy || !step) {
        std::cerr << "Missing required DLL function.\n";
        FreeLibrary(dll);
        return 1;
    }

    std::cout << "Agent name: " << getName() << '\n';

    AgentInstance* agent = create();

    if (!agent) {
        std::cerr << "Failed to create agent.\n";
        FreeLibrary(dll);
        return 1;
    }

    for (uint32_t i = 1; i <= 5; ++i) {
        uint32_t result = step(agent, i);
        std::cout << "Input: " << i
                  << ", Output: " << result << '\n';
    }

    destroy(agent);
    FreeLibrary(dll);

    std::cout << "DLL test completed successfully!\n";
    return 0;
}
