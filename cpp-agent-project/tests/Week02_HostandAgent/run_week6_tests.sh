#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p agent_test_build
CXX="${CXX:-g++}"
FLAGS=(-std=c++17 -O2 -Wall -Wextra -pedantic)

printf '\n=== Week 6: ScoutAgent unit tests (own agent) ===\n'
"$CXX" "${FLAGS[@]}" scout_agent.cpp scout_agent_test.cpp -o agent_test_build/scout_agent_test.exe
./agent_test_build/scout_agent_test.exe

printf '\n=== Week 6: Movement-outcome observation uncertainty (offline only) ===\n'
"$CXX" "${FLAGS[@]}" movement_evidence_test.cpp -o agent_test_build/movement_evidence_test.exe
./agent_test_build/movement_evidence_test.exe

printf '\n=== Week 6: Other map layouts (offline simulator) ===\n'
"$CXX" "${FLAGS[@]}" scout_agent.cpp map_simulator_test.cpp -o agent_test_build/map_simulator_test.exe
./agent_test_build/map_simulator_test.exe week06_maps

TARGET="$("$CXX" -dumpmachine)"
if [[ "$TARGET" == *mingw* ]]; then
    printf '\n=== Windows DLL for the original professor-provided host ===\n'
    "$CXX" "${FLAGS[@]}" -shared scout_agent.cpp -o scout_agent.dll
    printf 'Created scout_agent.dll. To test with the Windows host:\n'
    printf '  ./host.exe scout_agent.dll\n'
    printf 'The host must be tested manually; this script cannot verify flag/game outcomes.\n'
else
    printf '\n=== Native shared-library symbol check (not a Windows-host test) ===\n'
    "$CXX" "${FLAGS[@]}" -fPIC -shared scout_agent.cpp -o agent_test_build/libscout_agent.so
    for symbol in Agent_GetName Agent_Create Agent_Step Agent_Reset Agent_Destroy; do
        if ! nm -D --defined-only agent_test_build/libscout_agent.so | awk '{print $3}' | grep -qx "$symbol"; then
            printf 'Missing exported symbol: %s\n' "$symbol" >&2
            exit 1
        fi
        printf '[PASS] Exported symbol %s\n' "$symbol"
    done
    printf 'Non-Windows compiler (%s): Windows DLL and graphical host test still required.\n' "$TARGET"
fi
