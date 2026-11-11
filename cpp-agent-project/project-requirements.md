# Project Requirements — C++ Capture The Flag Agent

**Project:** Software Engineering + Research Project  
**Team:** Aiswarya Payakkal Vinayan and Nirmal Mohan  
**Status:** Initial definition; subject to review and refinement

## 1. Purpose and Scope

The project develops an autonomous C++ agent DLL for the professor-provided **WHZ Capture The Flag** graphical host. The agent receives limited information about its surroundings and returns movement or flag-interaction decisions. Research activities will compare navigation and exploration strategies using reproducible experiments.
**In scope:** agent DLL, host API compatibility, snapshot decoding, safe movement, robust unknown-map exploration, movement tracking where verifiable, flag objectives, opponent-aware reactions to visible information, testing, algorithm comparison, logging and documentation.  


## 2. Functional Requirements

| ID | Requirement | Priority | Status / proposed acceptance evidence |
|---|---|---|---|
| FR-01 | The DLL shall export `Agent_GetName`, `Agent_Create`, `Agent_Destroy`, `Agent_Reset`, and `Agent_Step` with the required signatures. | Mandatory (API) | Implemented in examples; host loading demonstrated for the first SafeMovementAgent. |
| FR-02 | The agent shall process the host-provided 5 × 5 `WorldSnapshot` with `cells[y][x]`, centered on the agent at `[2][2]`. | Mandatory (API) | Local observation reading implemented in SafeMovementAgent. |
| FR-03 | The agent shall interpret terrain and cell content using `CELLTYPE_BITMASK`, `CELLCONTENT_*`, and defined terrain values. | Core | Floor and agent occupancy checks implemented; other content interpretation remains planned. |
| FR-04 | Each `Agent_Step` call shall return a defined action code, such as a movement, `ACTION_WAIT`, `ACTION_PICK_UP_FLAG`, or `ACTION_PUT_DOWN_FLAG`. | Mandatory (API) | Defined movement/wait actions returned; pickup and put-down logic not yet implemented. |
| FR-05 | The agent shall initialize, retain, reset, and release its instance state using the API lifecycle functions. | Core | Basic turn counter and map dimensions implemented; more extensive navigation memory planned. |
| FR-06 | The navigation policy should avoid requesting movements into **known** walls or occupied cells when checking visible adjacent tiles. | Initial agent capability | Basic four-direction filtering implemented; game-level movement success needs systematic testing. |
| FR-07 | The agent should detect **visible** enemy flags, targets and useful exploration objectives, and select goal-directed actions when a safe known route exists. | Future feature | Planned; detection/route integration unverified. |
| FR-08 | The agent should handle flag pickup and delivery according to confirmed host rules and observed game state. | Future feature | Planned; possession and interaction semantics require verification. |
| FR-09 | The agent should prefer newly informative reachable regions when the map is not known beforehand, using validated movement tracking or a local novelty fallback. | Future improvement | No reliable persistent spatial map yet; heuristic candidates require tests. |
| FR-10 | The project shall compare reference and proposed strategies on multiple development and **held-out unseen** maps, with documented opponents, seeds and rules where controllable. | Research goal | Test protocol proposed; no tournament results collected. |
| FR-11 | The agent should react to **visible** opposing agents or occupied cells without assuming hidden opponent positions or moves. | Future feature | Planned; opponent interaction and combat/collision rules need verification. |
| FR-12 | The agent should preserve a safe fallback (local obstacle filtering or `ACTION_WAIT`) when its localization, map memory or planned route is unreliable. | Future feature | Design proposal; not yet verified for the hybrid agent. |
| FR-13 | A finite-state controller shall select among exploration, approaching an objective, pickup, returning, delivery, opponent response and recovery **only when relevant observations support each transition**. | Planned control module | Synthetic transition tests and host integration required; not implemented. |
| FR-14 | A reachable frontier shall be selected only from a spatially consistent known map; otherwise the agent shall use the local-safe fallback. | Planned exploration safety | Unit tests with uncertain, contradictory and dead-end observations required. |

## 3. Non-Functional Requirements

| ID | Requirement | How it will be verified |
|---|---|---|
| NFR-01 — Compatibility | The agent DLL shall work with the supplied Windows graphical host and the matching Week 2 API version. | Compile using a compatible 64-bit Windows C++ toolchain, then load the DLL in `host.exe`. |
| NFR-02 — Reliability | The DLL should not crash the host during ordinary creation, repeated step calls, reset, or destruction. | Test repeated simulations and lifecycle operations; log crashes or unexpected termination. |
| NFR-03 — Performance | Action selection should be efficient enough for interactive host execution. | Measure decision time per call (e.g., average and 95th percentile); agree on a threshold after initial measurements. |
| NFR-04 — Maintainability | Source should be readable, version-controlled, and organized so decision policies can be modified without changing exported API signatures. | Peer review code structure and Git commits. |
| NFR-05 — Reproducibility | Other team members should be able to build and run agent versions from repository instructions. | Document toolchain, build commands, agent versions, and test procedure in the README. |
| NFR-06 — Testability | The team should record meaningful baseline and algorithm test results. | Store test procedures, metrics, results, and limitations in `docs/` or an experiment-results folder. |
| NFR-07 — Traceability | Requirements, assigned tasks, implementation work, and decisions should be traceable. | Maintain GitHub Issues, commits, and dated developer-diary entries with authors and review notes. |
| NFR-08 — Generalization | Competition performance should not depend on memorizing one known development map. | Evaluate across multiple held-out maps; disclose number of maps, opponents, runs and limitations. |


## 4. Constraints and Assumptions

### Constraints imposed by the supplied host / API

1. The host is provided by the professor; the team develops the agent DLL rather than changing the host.
2. API defines five exported functions, their parameter types, and their calling convention; these declarations must remain compatible.
3. `Agent_Create` receives `mapWidth` and `mapHeight`, while `Agent_Step` receives a pointer to a 5 × 5 `WorldSnapshot` and `lastAction`.
4. The snapshot is local and uses `[row][column]` (`[y][x]`) indexing. It does **not** directly provide absolute agent coordinates.
5. A cell encodes terrain and content with bitmasks; unknown, floor, and wall are distinct terrain values.
6. The interface defines eight movement directions, waiting, flag pickup, and flag put-down. The host decides whether an action succeeds.
7. The Windows host and compiled DLL must be architecture-compatible. Keep Week 1 and Week 2 headers/hosts separate because their interfaces differ.

### Assumptions requiring confirmation

- A requested movement may be rejected by host rules; `lastAction` alone should not be treated as proof of successful movement.
- For initial planning, navigation can first use known visible floor cells; mapping unseen regions requires additional state estimation.
- The exact conditions for diagonal movement and flag pickup/delivery require testing or instructor clarification.

## 5. Initial Acceptance and Evaluation Criteria

| Criterion | Suggested verification | Current status |
|---|---|---|
| Host integration | Graphical host loads and executes the agent DLL. | Demonstrated for the first SafeMovementAgent. |
| API compatibility | Required DLL functions exist and can be invoked by the host. | Basic integration demonstrated; formal lifecycle tests pending. |
| Obstacle-aware action choice | For controlled local snapshots, the algorithm avoids known blocked cardinal neighbors. | Logic implemented; automated cases pending. |
| Reliable execution | Repeated games run without a crash. | Not yet systematically measured. |
| Research comparison | Baseline and improved versions evaluated under comparable conditions using documented metrics. | Planned. |
| Documentation completeness | Project definition, schedule/Gantt chart, research documents and developer diary are available in the repository. | Verify against latest GitHub commits. |

**Proposed evaluation metrics:** known-blocked-move request rate, immediate-reversal frequency, decision time, number of successful game runs, goal-reaching turns, and (once position tracking is reliable) unique explored cells and flag captures. No quantitative improvement is claimed yet.

## 6. Expected Results

- A working, documented Windows agent DLL compatible with the professor's host.
- An incremental set of agent strategies: baseline, obstacle-aware, and future navigation/exploration versions.
- A documented research comparison using a repeatable test method and clearly defined evaluation metrics.
- Source code, plans, testing evidence, decisions, and limitations recorded in the GitHub repository.
