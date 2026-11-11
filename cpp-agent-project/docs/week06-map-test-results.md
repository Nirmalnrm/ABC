# Week 6 ScoutAgent — Reproducible Offline Test Results

**Latest run:** 11 November 2026, Linux x86_64; `g++ (Debian 14.2.0-19) 14.2.0`, C++17.  
**Command:** `bash tests/Week02_HostandAgent/run_week6_tests.sh` from repository root.  
**Source SHA-256 (`scout_agent.cpp`):** `512dd6e9d81a3fcbfc3913bb0078f077fa54e32d019bbda76fde953b23fbb37b`.  
**Scenario:** our own `ScoutAgent`, no `StupidAgent` comparison.  
**Log:** [`docs/week06-test-log.txt`](week06-test-log.txt) in the repository.

## Test suites

| Suite | Result | Verified scope |
|---|---:|---|
| ScoutAgent unit tests | **19/19** | API lifecycle, legality of cardinal choices, wall/occupant avoidance, local visible-flag BFS with detour, pickup **requests**, cooldown and reset |
| Movement-observation tests | **8/8** | Whether consecutive *synthetic terrain* views support moving, staying or remain ambiguous; not integrated into ScoutAgent |
| Authored development-map fixtures | **6/6** | Zero offline blocked requests, minimum simulator cell coverage, and flag-cell arrival/pickup **request** for the five maps that contain a flag |
| Native C-linkage exports | **5/5** | Linux `.so` exports `Agent_GetName/Create/Step/Reset/Destroy`; **not** Windows host compatibility |

All builds also passed `-Wall -Wextra -Werror -pedantic` for the native test programs. The host executable was not run here.

## Map-by-map results

| Map | Challenge | Step budget | Simulator cells visited | Blocked requests | Waits | First flag-cell arrival (step) | Pickup requests |
|---|---|---:|---:|---:|---:|---:|---:|
| `open_arena.map` | Open floor | 180 | 77/77 | 0 | 0 | 13 | 5 |
| `corridor_turns.map` | Narrow turning corridors | 180 | 49/49 | 0 | 0 | 15 | 3 |
| `branching_deadends.map` | Branches and dead ends | 180 | 54/54 | 0 | 0 | 20 | 4 |
| `blocked_start.map` | No legal neighboring floor | 12 | 1/1 | 0 | 12 | N/A (no flag) | 0 |
| `two_rooms.map` | Two rooms joined through a central gap | 180 | 85/85 | 0 | 0 | 82 | 2 |
| `zigzag_barriers.map` | Offset narrow passages | 180 | 111/111 | 0 | 0 | 90 | 2 |

**Acceptance thresholds:** the five navigable maps require at least 50, 30, 30, 50, and 70 simulator-visited floor cells respectively; each must reach its flag cell and issue at least one pickup *request*. `blocked_start` requires at least its start cell visited and no blocked requests. Every fixture must request zero blocked movements. These are developer-authored maps, not competition tests.

## Important limitations

- The **offline simulator** knows physical positions, counts visited cells and interprets four movement codes; ScoutAgent sees **only a local 5 × 5 snapshot**. Its tentative map coordinates are not reliable positions.
- The simulator's successful movement/coverage does **not** verify real movement, obstacle collisions or game outcomes in professor `host.exe`.
- Reaching a flag cell and requesting `ACTION_PICK_UP_FLAG` does **not** establish pickup, possession, return/delivery or a match win.
- The movement-evidence test is a standalone experiment and **does not prove the actual host supplies a unique movement signal**.
- The sixth map (`zigzag_barriers.map`) and fifth map (`two_rooms.map`) are additional **development** fixtures. `reserved_holdout.map` was **not used by any scripted development test** and remains reserved for a future frozen-agent test.
- The Windows DLL was **not** built or loaded in the Linux runner. Aiswarya or a designated team member must test it using MSYS2 UCRT64 on Windows and record the run.

## Windows/MSYS2 reproduction

```bash
cd /c/Users/nirma/class/cpp/cpp-agent-project
bash tests/Week02_HostandAgent/run_week6_tests.sh
cd tests/Week02_HostandAgent
./host.exe scout_agent.dll
```

Record the actual date, command, compiler target, Git commit, host output and observed movement/flag behavior. See [host checklist](week06-host-checklist.md).
