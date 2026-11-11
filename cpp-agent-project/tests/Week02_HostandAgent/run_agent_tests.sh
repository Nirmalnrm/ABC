
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p agent_test_build
CXX="${CXX:-g++}"
FLAGS=(-std=c++17 -O2 -Wall -Wextra)

printf '\n=== SafeMovementAgent: deterministic policy and DLL API checks ===\n'
"$CXX" "${FLAGS[@]}" -DEXPECT_SAFE_AGENT safe_agent.cpp agent_api_smoke_test.cpp -o agent_test_build/safe_policy_test.exe
./agent_test_build/safe_policy_test.exe

printf '\n=== StupidAgent: random baseline/API checks ===\n'
"$CXX" "${FLAGS[@]}" -DEXPECT_STUPID_AGENT stupid_agent.cpp agent_api_smoke_test.cpp -o agent_test_build/stupid_policy_test.exe
./agent_test_build/stupid_policy_test.exe

printf '\n=== BFS: known-grid synthetic tests ===\n'
"$CXX" "${FLAGS[@]}" bfs_known_grid_test.cpp -o agent_test_build/bfs_known_grid_test.exe
./agent_test_build/bfs_known_grid_test.exe

printf '\n=== Build original agent DLLs for the professor-provided Windows host ===\n'
"$CXX" "${FLAGS[@]}" -shared safe_agent.cpp -o safe_agent.dll
"$CXX" "${FLAGS[@]}" -shared stupid_agent.cpp -o stupid_agent.dll

printf '\nAll automatic checks passed. DLLs built.\n'
printf 'For the graphical tests on Windows, from this folder run:\n'
printf '  ./host.exe safe_agent.dll\n'
printf '  ./host.exe stupid_agent.dll\n'
printf '\nNote: A host screenshot shows the simulation, not proof of completed movements or winning.\n'
