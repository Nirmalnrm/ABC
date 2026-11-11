# Software Architecture — C++ Capture The Flag Agent

**Project:** C++ Capture The Flag Agent (Software Engineering + Research Project)  
**Team:** Aiswarya Payakkal Vinayan (software development and coordination); Nirmal Mohan (research, testing and evaluation)  
**Planning period:** October 2026 – January 2027  
**Document status:** Week 3 architecture preserved; Week 4 unknown-map competition changes proposed for review  
**Related work packages:** WP-02 (API and Movement Verification), WP-03 (Map Memory and Confidence), WP-04 (Frontier Exploration and Safe Fallback), WP-05 (FSM, BFS, Flags and Opponent Response), WP-12 (Integration and QA)

## 1. Purpose and Architectural Approach

The aim is to create an autonomous **C++ agent DLL** that can be loaded by the graphical Capture The Flag host (`host.exe`). The agent observes a limited region of the map, selects a legal API action, and aims to improve navigation and eventually capture the opposing team's flag.

The design uses an **iterative and incremental approach**. A working host-compatible agent is the starting point. New capabilities—snapshot helpers, outcome-aware movement tracking, map confidence, online exploration, BFS routes and flag/opponent response—will be introduced and tested in stages, without changing the DLL interface.

**Architecture boundary:** Our team develops the agent DLL, its internal strategy, tests and documentation. The graphical game host and the exported API definitions are supplied externally and are **not** to be modified.

## 2. High-Level System Architecture

```text
              Professor-provided graphical game host
                            host.exe
                               |
              WorldSnapshot (5 x 5), lastAction
                               v
         +--------------------------------------------+
         |              C++ AGENT DLL                 |
         |                                            |
         |  DLL API / lifecycle adapter [existing]    |
         |       Agent_Create / Agent_Reset /         |
         |       Agent_Step / Agent_Destroy /         |
         |       Agent_GetName                        |
         |                 |                          |
         |         Snapshot reading [existing]        |
         |                 |                          |
         |     Safe movement selection [existing]     |
         |                 |                          |
         |  ----------------------------------------  |
         |  Future modules:                           |
         |   - Position and movement tracking         |
         |   - Persistent map / exploration memory    |
         |   - Frontier selector, BFS, and FSM              |
         |   - Flag/opponent policy and safe fallback        |
         +--------------------------------------------+
                               |
                       ACTION_* code
                               v
                       Graphical host
```

The DLL returns one documented action code on each `Agent_Step` call. The host controls the game state and subsequent observations.

## 3. Existing API and Component Interface

The repository includes two **different** test interfaces:

- **1 — `tests/AgentDLLTest/agent_api.h`:** a basic DLL communication exercise in which the host sends a numeric value to `Agent_Step`. This proves the general host/DLL integration concept; it is **not** the graphical game's API.
- **2 — `tests/Week02_HostandAgent/agent_api.h`:** the actual graphical Capture The Flag interface, version 1.0, used for this architecture. **Do not mix Week 1 and Week 2 headers.**

The 2nd interface header requires these five exported functions:

| Function | Responsibility |
|---|---|
| `Agent_GetName()` | Return a stable name identifying the DLL agent. |
| `Agent_Create(uint32_t mapWidth, uint32_t mapHeight)` | Allocate an agent instance and initialize game-independent/per-game state. Map dimensions do **not** give the player's position. |
| `Agent_Reset(AgentInstance*)` | Clear the state that must not persist between games. |
| `Agent_Step(AgentInstance*, const WorldSnapshot*, uint32_t lastAction)` | Read the observation and return one `ACTION_*` command. |
| `Agent_Destroy(AgentInstance*)` | Release the allocated instance and its resources. |

The functions use the declarations supplied in `agent_api.h`, including `extern "C"`, `AGENT_API`, and `AGENT_CALL`. These export and calling conventions must stay compatible with the host.

### Observation format

```cpp
#define VIEW_SIZE 5
struct WorldSnapshot {
    uint32_t cells[VIEW_SIZE][VIEW_SIZE];
};
```

- The snapshot is indexed as `cells[y][x]` (row, column). The agent is at `[2][2]`.
- `CELLTYPE_BITMASK` separates terrain such as `CELLTYPE_FLOOR`, `CELLTYPE_WALL` and `CELLTYPE_UNKNOWN`.
- Content flags identify the agent, enemy agent, own/enemy flags and own/enemy targets.
- Movement commands cover eight compass directions; additional commands are `ACTION_WAIT`, `ACTION_PICK_UP_FLAG`, and `ACTION_PUT_DOWN_FLAG`.
- The callback supplies `lastAction`, but the header does **not** guarantee that the previous requested movement succeeded. No absolute agent coordinates are provided.

**Implication:** Accurate whole-map memory, revisit counting, and global route planning require a tested position-tracking approach; they cannot be assumed from the API alone.

## 4. Current Implemented Components

The current repository contains two Week 2 C++ agents:

| Component | Source file | Current behavior |
|---|---|---|
| **StupidAgent** (reference baseline) | `tests/Week02_HostandAgent/stupid_agent.cpp` | Selects one of eight movement action codes pseudorandomly, without checking the visible obstacles. |
| **SafeMovementAgent** | `tests/Week02_HostandAgent/safe_agent.cpp` | Checks the four adjacent cardinal cells, keeps known floor cells without agent occupancy, prefers options that do not immediately reverse `lastAction`, and returns `ACTION_WAIT` when none are available. |

The `AgentInstance` in `safe_agent.cpp` currently stores a round counter and map dimensions. The five DLL exports and basic instance lifecycle are implemented. The exported `Agent_Step` returns `ACTION_WAIT` when its instance or snapshot pointer is null.

**Not implemented in these source files:** persistent spatial map memory, reliable absolute localization, BFS, A*, automated flag pickup/delivery, and a quantitative benchmarking harness. Those are future milestones, not Week 3 achievements.

## 5. Proposed Internal Modules

| Module | Responsibility | Status / dependency |
|---|---|---|
| API adapter | Preserve five DLL exports and lifecycle functions | Existing |
| Snapshot decoder | Read terrain, content flags and known occupancy from 5 × 5 input | Basic inline checks exist; reusable helper planned |
| Safe local exploration | Choose known legal adjacent movement, prefer novelty when supported, otherwise `ACTION_WAIT` | Basic safe movement baseline exists; separate novelty policy planned |
| Movement/pose confidence tracker | Distinguish requested action from confirmed movement; invalidate inconsistent position estimates | Proposed; must verify host semantics |
| Partial map and frontier detector (FBE) | Keep *trusted* known cells, find reachable known floor adjacent to unknown cells, rank frontiers | Proposed; map requires credible localization |
| BFS planner | Return first action of a known valid route to frontier, flag or return target | Proposed; test on known-grid fixtures |
| **Finite-State Machine (FSM)** | Select task mode: explore, approach, pickup, return, deliver, avoid visible opponent, recover | **Core proposed orchestration module** |
| Flag/opponent policy | Use only verified flag, possession, target and visible-enemy information | Proposed; interaction semantics pending |
| Fallback and recovery | Clear invalid targets/routes on contradictory observations and continue safe local movement or wait | Required safe behavior; hybrid implementation pending |
| Test harness and telemetry | Test transitions, routes, exploration and match outcomes without changing host API | WP-07–WP-10 planned |

**Separation of concerns:** FBE is an *exploration-target selector*; BFS is a *known-route planner*; FSM is an *objective controller*. They are not competing substitutes. A* is optional after a reliable core is demonstrated.

## 6. Agent Decision Flow

**Existing, demonstrated SafeMovementAgent:** check four cardinal neighbors, filter known obstacles/occupied cells, optionally avoid immediately reversing the last *requested* movement, choose an action or `ACTION_WAIT`. This implementation has **no FBE/BFS/FSM**.

**Proposed hybrid loop (not yet implemented):**

```text
Agent_Step(instance, snapshot, lastAction):
    decode terrain, flags, targets and visible opponents
    update only motion/map facts supported by observations
    if pose or stored route contradicts observation:
        invalidate global plan and enter RECOVER
    choose FSM state from verified observations/events and priorities
    if state == RECOVER:
        action = safe_local_exploration_or_wait()
    else if state == EXPLORE:
        if map_pose_confident AND reachable_frontier_exists:
            frontier = select_known_reachable_frontier()
            action = first_BFS_step_to(frontier)
        else:
            action = safe_local_exploration_or_wait()
    else if state == APPROACH_FLAG or state == RETURN:
        action = trusted_first_BFS_step_to_known_goal()
        if no such route: action = safe_local_exploration_or_wait()
    else if state == PICKUP or state == DELIVER:
        action = confirmed_legal_flag_action_or_wait()
    else if state == EVADE:
        action = safe_response_based_only_on_visible_opponent_and_rules()
    validate action against defined API codes
    return action
```

**Hard safety gates:** never use unknown terrain as confirmed floor; never assume `lastAction` succeeded; never infer successful pickup from an unconfirmed requested action; never follow a global BFS route if pose confidence is invalid; never assume hidden enemy positions.

The FSM should not enter `RETURN` because a pickup action was merely **requested**. Require a documented possession indication or a validated host-observation inference before depending on that state.

## 7. Architecture Constraints and Risks

1. The actual map is **unknown at agent start**; no hidden map file or hard-coded layout is assumed.
2. Host API supplies only a local 5 × 5 view; no direct global position.
3. `lastAction` is not explicit proof of successful displacement. Use conservative confidence-aware state and recovery.
4. The host defines movement, interaction and opponent rules; verify diagonals, collision, pickup/drop and match result before implementing tactics.
5. The safe fallback must operate even without reliable persistent localization.
6. Keep the Windows DLL architecture and professor-provided API unchanged; Week 1 and Week 2 headers must remain separate.
7. Comparing agents with different allowed movement sets (eight versus four directions) requires a documented control.
8. Test on independent unseen layouts to avoid overfitting to the competition map.

## 8. Testing and Quality Assurance Plan

- **API/lifecycle:** Build compatible DLL, verify exports, create/step/reset/destroy repeatedly.
- **Snapshot fixtures:** Known walls, unknown cells, enemy occupied cells, single safe neighbor, no safe neighbor, visible flag.
- **Outcome inference:** Controlled blocked moves and observation changes; reject unsupported pose updates.
- **Exploration:** Loop/dead-end/corridor maps; measure novelty only when visit counts are verifiable.
- **Known-goal planning:** BFS shortest paths on synthetic unit-cost grids; test no-known-route fallback.
- **Game objectives:** Verify correct pickup/drop, possession/status and response to visible opponents against actual host rules.
- **Generalization:** Compare validated agent versions on held-out layouts; capture opponent type, map ID, seed, revision, game results if observable, and failures.
- **Resource use:** Track `Agent_Step` latency and crashes; maintain a robust fallback.

No claim of full map coverage, match wins or superior performance should be made before measured experimental results exist.

## 9. Development Plan and Responsibilities

**Aiswarya — Software Engineering:** WP-02 API/movement verification; WP-03 memory/confidence; WP-04 FBE/fallback; WP-05 BFS plus FSM, verified flag behavior and visible-opponent response.  
**Nirmal — Research and Evaluation:** WP-06 unknown-map literature; WP-07 evaluation protocol; WP-08 independent maps/baselines; WP-09 opponent and strategy comparisons; WP-10 robustness/results analysis.  
**Both:** WP-11–14 coordination, integration, QA, documentation and final review.

The existing 14 work packages still total **300 planned person-hours** (110 software, 114 research, 76 joint). Details: [Project Schedule](project-schedule.md). The current implementation remains the baseline; new hybrid components are scheduled, **not demonstrated**.

## Related Documents

[Project description](project-description.md) · [Project requirements](project-requirements.md) · [Project plan](project-plan.md) · [project schedule](project-schedule.md) · [Research plan](research-plan.md) · [Algorithm comparison](algorithm-comparison.md) ·


## 10. API Verification and Map-Memory Design 

**Status:** Test and design specification. The table below defines *expected checks*, not experimental outcomes. WP-02 and WP-03 are active in the schedule; successful Windows-host verification and persistent mapping are **not yet evidenced** by this document.

### 10.1 Controlled movement and API checks

Use the existing Week 2 `agent_api.h`, `safe_agent.cpp` and graphical host. In every synthetic 5 × 5 fixture, the agent is at `cells[2][2]`; indexes are `cells[y][x]`. Terrain is identified with `CELLTYPE_BITMASK`, and occupied neighbors with `CELLCONTENT_SELF | CELLCONTENT_ENEMY`.

| Test ID | Prepared observation or sequence | Expected behavior to verify | Evidence/status |
|---|---|---|---|
| API-01 | Only north (`[1][2]`) is a known, unoccupied floor; other cardinal neighbors are walls | Select `ACTION_MOVE_NORTH` | Not run; record action returned |
| API-02 | All four cardinal neighbors are walls | Return `ACTION_WAIT` | Not run |
| API-03 | North is `CELLTYPE_UNKNOWN`, east is the only valid floor | Do not request north; east is a valid candidate | Not run |
| API-04 | East is known floor with `CELLCONTENT_ENEMY`; south is open | Do not request east; south is a valid candidate | Not run |
| API-05 | North and south are valid; `lastAction` is north | Prefer north over an immediate south *request* when possible | Not run; this does **not** prove previous movement succeeded |
| API-06 | `Agent_Step` receives a null snapshot or null instance | Exported wrapper returns `ACTION_WAIT` | Source behavior visible; runtime test pending |
| API-07 | Create → several steps → Reset → Step → Destroy | Agent state resets and lifecycle executes without failures | Not run |
| API-08 | In the graphical host, request a movement into a known wall, then compare successive observations | Determine whether the host rejects the move and whether observed state can distinguish success from failure | Unknown; controlled host test needed |
| API-09 | Request diagonal movement near corners (using a separate controlled experimental policy) | Determine corner-cutting rules and whether diagonals are accepted | Not run; **not** a claim that current safe agent chooses diagonals |
| API-10 | Attempt flag pickup/delivery under known host conditions | Determine legal position, possession feedback and success/failure evidence | Rules unverified; do not infer success from an action request |

**Record for each actual run:** date, contributor, source commit, host build, map/test setup, requested action, subsequent observation, expected/observed outcome, and uncertainty. Use the existing developer diary for dated findings; do not fill the result column until the test occurs.

### 10.2 Map-memory proposal and confidence gates 

The host provides map **dimensions**, but not the player's absolute coordinates. Map memory must therefore be designed as a conditional feature:

- **State candidate:** store an estimated relative or absolute pose *only if* movement outcomes can be inferred; keep an explicit `poseTrusted`/confidence state and the previous observation/action for comparison. The current `AgentInstance` stores only map dimensions and a round counter.
- **Map candidate:** represent each trusted cell as `UNKNOWN`, `FLOOR` or `WALL`, optionally with an observation count or last-observed turn. Store *dynamic contents* (opponents, flags) separately from terrain and refresh them from observations; do not make a previously seen opponent a permanent wall.
- **Update rule:** treat `lastAction` as a **request**, not proof of displacement. Only merge the 5 × 5 snapshot into persistent coordinates after a validated pose update. Ambiguous observations (e.g., identical corridors) must not silently increase position certainty.
- **Inconsistency rule:** if a new observation contradicts an existing aligned map, invalidate the affected alignment/route and use a conservative local policy until the uncertainty is resolved.
- **Fallback:** when reliable localization is unavailable, use the known-floor, unoccupied-neighbor filter from SafeMovementAgent, or `ACTION_WAIT` when no safe candidate exists; do not generate global frontier targets.
- **Reset:** clear per-game map, current target, prior observations and confidence on `Agent_Reset`, once those fields are implemented.

**Acceptance gate for future FBE:** demonstrate in controlled runs that observed displacement and map alignment are sufficiently reliable; then test known-floor cells adjacent to unknown cells as candidate frontiers, verifying their reachability with BFS. No FBE implementation is asserted here.

### 10.3 Coordination with Research

Nirmal will use the WP-02 test definitions to decide which measured outcomes can be logged directly (valid actions, known-blocked *requests*) and which require additional host evidence (actual movement, revisits, flag success). The same fixture definitions should be reused for comparisons wherever possible. See [Research Plan](research-plan.md) for the Week 5 experimental protocol.

