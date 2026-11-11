# Algorithm Research — Autonomous C++ Capture The Flag Agent

**Project:** C++ Capture The Flag Agent — Software Project (ELT10210) and Autonomous Research Project (KFT11050)  
**Research lead:** Nirmal Mohan  
**Software engineering and technical review:** Aiswarya Payakkal Vinayan  
**Research status:** Preliminary algorithm investigation and experimental design.

## 1. Research problem and game constraints

The agent competes against another team's agent on a map that is not known before the match. The professor-provided Windows graphical host loads a C++ agent DLL and calls `Agent_Step` to request one action at a time. The host supplies a **5 × 5 local `WorldSnapshot`**, with the agent at local cell `[2][2]`, and a `lastAction` value.

The supplied API exposes terrain, nearby agents, flags and target bitmasks, but it does **not** directly provide absolute agent coordinates or guarantee that the last requested movement succeeded. These constraints make online exploration, movement-outcome verification and recovery from uncertain map information important.

The initial `StupidAgent` requests random movement in eight directions. The custom `SafeMovementAgent` filters four adjacent cardinal cells for known traversable terrain and occupancy. Both have been loaded in the graphical host in earlier work, but the safe policy was observed to move repeatedly in a small region. This qualitative observation is **not** a controlled quantitative result.

**Research objective:** Investigate whether a modular strategy can explore previously unseen maps, navigate toward discovered objectives and handle observable game events more effectively than the existing movement baselines.

## 2. Preliminary algorithm choice

The proposed approach is **Frontier-Based Exploration (FBE) + Breadth-First Search (BFS) + Finite-State Machine (FSM)**, with **safe local exploration as the fallback**. **A\*** is optional for later comparison.

| Component | Research and software role | Prerequisite / limitation | Status |
|---|---|---|---|
| **FBE** | Choose *where to explore*: a reachable known-floor boundary adjacent to unknown space | Requires a consistent remembered map and dependable position/movement estimates | Proposed |
| **BFS** | Choose *how to reach a known target* through legal, known traversable cells | Requires a reachable target, graph and confirmed movement costs | Planned |
| **FSM** | Decide *what objective to pursue*: explore, approach, pick up, return, deliver, respond or recover | State changes must be based on actual observations and confirmed game rules | Proposed |
| **Safe local fallback** | Continue obstacle-aware decisions when localization, map or route confidence is inadequate | May still repeat movement; cannot guarantee map coverage | Basic safe baseline exists; improved exploration fallback planned |


This choice addresses two different problems: **exploration** when the destination is unknown and **path planning** after a useful destination becomes known. It does not guarantee successful flag capture or a competition win.

## 3. Algorithm details and applicability

### 3.1 Frontier-Based Exploration (FBE)

A frontier is a **known traversable cell or region bordering still-unknown space**. In a trustworthy map, the agent can identify reachable frontiers, select a useful one, travel to it, observe additional cells and repeat. This approach is motivated by Yamauchi's research on autonomous exploration [R1].

**Project adaptation:** Initially investigate local novelty-seeking without global coordinates. Persistent frontier detection is permitted only after movement success and spatial map alignment have been validated. Frontier scoring may later consider reachability, estimated path length, expected unknown-area discovery and observable opponent risk; such scoring is a **design proposal**, not an established result for this host.

**Limitations:** Incorrect pose estimates can place observations in the wrong map cells and produce false frontiers or impossible routes. Frontiers do not determine how to capture a flag; they select regions to investigate.

### 3.2 Breadth-First Search (BFS)

BFS explores an unweighted graph in increasing number of edges from a start node. For **known, legal, equal-cost moves**, it can find a route with the minimum number of steps to a reachable known destination.

**Project adaptation:** Begin with controlled 5 × 5 fixtures using known floor cells and **four cardinal directions**, matching the safe baseline's action set. Check reachable and unreachable objectives, walls, occupied cells and unknown cells. Integrate the first action of the computed path into the host-compatible DLL only after the movement graph has been tested. A larger trusted map can later support routes to selected frontiers or remembered objectives.

**Limitations:** BFS does not reveal a hidden flag and should not assume unknown cells are traversable. Shortest-path claims apply only to the modeled movement graph. Diagonal moves and variable action costs require separate confirmation.

### 3.3 Finite-State Machine (FSM)

The FSM separates **game-level priorities** from the exploration and route planners. Proposed states and transition guards are:

| State | Purpose | Example guarded transition |
|---|---|---|
| `EXPLORE` | Discover new areas or objectives | Enter `APPROACH_FLAG` if a flag is known and a usable route exists |
| `APPROACH_FLAG` | Travel toward a known flag | Enter `PICKUP` only when a legal pickup situation is verified |
| `PICKUP` | Issue a permitted pickup command | Enter `RETURN` **only if possession is verified**; otherwise recover or continue observing |
| `RETURN` | Navigate toward the required delivery location | Enter `DELIVER` only when delivery conditions are verified |
| `DELIVER` | Attempt the legal delivery interaction | Continue based on confirmed interaction feedback |
| `EVADE` / `RESPOND` | React to a visible opposing agent | Return to the previous objective when the threat/obstruction no longer applies |
| `RECOVER` | Handle inconsistent position, failed route or no safe action | Clear unreliable state and use safe local fallback |

These are **proposed control states**. The API's action codes alone do not prove how possession, scoring or movement success is reported. Unverifiable transitions must remain disabled until tested. The current six research references support exploration/search/benchmarking, **not a claim that this FSM is experimentally validated**; targeted academic reading on agent control architectures is still needed.

### 3.4 Safe local exploration fallback

When the global map is unreliable or a path cannot be justified, the agent should consider known free neighboring cells, avoid known occupied cells, optionally discourage immediate reverse *requests*, and use `ACTION_WAIT` if no suitable action exists. A future local novelty heuristic may prefer less-recently attempted directions, while acknowledging that a requested move might fail.

Avoid equating **requested non-reversal** with confirmed physical progress. The fallback is an operational safety mechanism, not proof of effective exploration.

## 4. Proposed integrated decision flow

```text
Host calls Agent_Step(snapshot, lastAction)
    Decode the current 5 x 5 observation
    Check movement outcome / update confidence only when justified
    If remembered pose or map is inconsistent:
        Enter RECOVER and use safe local exploration
    Otherwise:
        Update validated local / persistent knowledge
        FSM selects a permitted game objective
        If a confirmed flag interaction is legal:
            Return the relevant interaction action
        Else if a known target is reachable on trusted cells:
            Use BFS and return its first valid move
        Else if a reachable frontier can be selected reliably:
            Use frontier exploration (with known-route planning)
        Else:
            Use safe local exploration or ACTION_WAIT
    Return one documented ACTION_* code
```

The ordering is conceptual; the integrated controller remains **future work**. No unsupported host feedback or secret competition-map information should be assumed.

## 5. Research questions and testable hypotheses

These identifiers match the current [`research-plan.md`](research-plan.md).

| Research question | Preliminary, untested hypothesis |
|---|---|
| Does novelty/frontier-inspired exploration discover more useful map area or objectives than safe random movement on unseen maps? | Information-seeking actions may improve objective discovery when positioning is sufficiently reliable. |
| How do blocked or uncertain moves affect map memory and navigation? | Confidence-aware state updates and fallback may prevent some mapping errors. |
| When a reachable objective is known, does BFS improve route efficiency over safe random movement? | BFS should give shortest paths on the tested equal-cost known graph and may improve observed route completion. |
| Can visible-opponent-aware decisions improve reliable objective progress? | Responses to verified visible opponent information may help under some game rules and maps. |
| 
Does the hybrid agent generalize across held-out maps and opponent policies? | Hybrid control may be more robust, but the effect must be measured and could vary by map or opponent. |

## 6. Research methodology and experimental path

1. **API and behavior verification (WP-02 / WP-07):** Test known walls, occupied cells, unknown cells, blocked actions, diagonal/corner rules, reset and flag interactions. Distinguish the last *requested* action from confirmed movement. Define which game outcomes can actually be measured.
2. **Baselines (WP-08):** Preserve `StupidAgent` and `SafeMovementAgent`; test repeatable scenarios, recording versions, maps, opponents, seeds if controllable, turns, failures and observable metrics. Since the original random agent uses eight movement directions and the safe agent uses four, add an equal-direction random control before attributing differences solely to obstacle filtering.
3. **Local exploration and map confidence (WP-03 / WP-04):** Compare the safe baseline against a local novelty policy; separately validate pose tracking, map consistency and frontier detection on synthetic maps. Do not report physical coverage until cell positions can be verified.
4. **BFS path planning (WP-05):** Validate shortest legal known-cell routes with fixed grid fixtures; test blocked goals, unknown barriers, and route invalidation after new observations.
5. **FSM controller (WP-05 / WP-12):** Use synthetic sequences to test state transitions, resets and refusal to infer unconfirmed possession or movement success; then test real interactions only after confirming host behavior.
6. **Comparisons and generalization (WP-09 / WP-10):** Compare implemented variants under matched conditions, such as safe movement, exploration-only, exploration+BFS, and exploration+BFS+FSM. Use multiple development maps and reserve **held-out map layouts** for final evaluation. Test against more than one opponent policy if the host allows it; do not acquire the private competition map in advance.
7. **Analysis:** Report averages/medians, variability, failed trials, conditions, source revision and threats to validity. Include results for only those policies actually implemented and tested.

**Initial trial plan:** The research plan proposes approximately **10 repeatable runs per implemented agent/scenario**, subject to host support and practical constraints. This is not a completed dataset or a statistically guaranteed sample size.

## 7. Evaluation metrics and feasibility

| Metric | Proposed measurement | Feasibility |
|---|---|---|
| DLL reliability | Number of runs without load errors/crashes | Measurable in host tests |
| Valid action rate | Defined API actions / total `Agent_Step` decisions | Measurable now |
| Known-blocked request rate | Movement requests into visible known walls/occupied cells / movement requests | Requires decision logging; measurable without global position |
| Requested reversal rate | Immediately opposite-direction requests / eligible transitions | Measurable; **not** a physical revisit rate |
| Decision latency | Per-step compute time (mean/median/upper percentile) | Requires suitable timer instrumentation |
| Known-route length | Moves in the BFS route on a fixed test grid | Synthetic-grid tests |
| Map coverage / cell revisits | Verified unique or revisited absolute cells | Only after reliable localization or trusted test instrumentation |
| Objective discovery time | Turns until a verified objective is observed | Requires event/observation logging |
| Valid FSM transitions | State changes supported by confirmed events / transitions checked | Synthetic tests first; host tests later |
| Flag delivery / win rate | Verified delivery outcomes / wins under specified map and opponent conditions | Only if game events or results are reliably observable |

The operational measurement details are in [`research-plan.md`](research-plan.md). 

## 8. Research evidence, known limitations and open questions

- **Available implementation evidence:** The original random and initial safe-movement DLLs have been exercised in the graphical host, according to the developer diary. Controlled tests of the proposed hybrid remain outstanding.
- **Literature review depth:** The six cited works have been identified and summarized from verifiable bibliographic and available source information. A full-text critical review of every paper is not yet documented.
- **Mapping uncertainty:** `Agent_Create` provides map dimensions, but not absolute agent position; `lastAction` does not guarantee successful displacement.
- **Experimental confounds:** Different action sets, stochastic seeds, map difficulty, opponent behavior and unobserved collisions may affect comparisons.
- **Unconfirmed game mechanics:** Diagonal movement, flag pickup/delivery, flag-possession indication, opponent interactions and score reporting require host verification.
- **Research gap:** A direct publication supporting FSM-based control under this exact game API has not yet been established in the current literature survey.
- **Research integrity:** Keep the final competition map outside development/tuning and distinguish planned work, implementation, test evidence and actual findings.

## 9. Software–research responsibilities and outputs

| Track | Responsibility | Expected evidence |
|---|---|---|
| **Software Engineering — Aiswarya** | API validation, safe fallback, position confidence/map memory, frontier detection, BFS, FSM and DLL integration | Source revisions, repeatable unit/host tests, architecture decisions |
| **Research and Evaluation — Nirmal** | Literature review, research questions, baselines, experiment controls, measured comparisons and limitations | Paper notes, test protocol, logs, comparison tables and research report |
| **Joint** | Reviews, integration, opponent tests, project schedule and final documentation | GitHub Issues/commits, diary entries, reviewed milestones |

The related schedule uses **14 work packages and 300 planned person-hours**, including 110 software, 114 research and 76 joint hours. These are planned estimates, not completed work.

## 10. Academic references

The following are **existing project literature references**, not new claims of full-paper review:

- **[R1]** Yamauchi, B. (1997). *A Frontier-Based Approach for Autonomous Exploration*. IEEE CIRA. https://doi.org/10.1109/CIRA.1997.613851
- **[R2]** Hart, P. E., Nilsson, N. J., & Raphael, B. (1968). *A Formal Basis for the Heuristic Determination of Minimum Cost Paths*. IEEE Transactions on Systems Science and Cybernetics. https://doi.org/10.1109/TSSC.1968.300136
- **[R3]** Korf, R. E. (1990). *Real-Time Heuristic Search*. Artificial Intelligence. https://doi.org/10.1016/0004-3702(90)90054-4
- **[R4]** Koenig, S., & Likhachev, M. (2002). *D* Lite*. AAAI. https://publications.ri.cmu.edu/d-lite
- **[R5]** Sturtevant, N. R. (2012). *Benchmarks for Grid-Based Pathfinding*. IEEE Transactions on Computational Intelligence and AI in Games. https://doi.org/10.1109/TCIAIG.2012.2197681
- **[R6]** Koenig, S., Likhachev, M., & Furcy, D. (2004). *Lifelong Planning A**. Artificial Intelligence. https://doi.org/10.1016/j.artint.2003.12.001

## Related project documents

- [Research Plan](research-plan.md)
- [Literature Summary](literature-summary.md)
- [Algorithm Comparison](algorithm-comparison.md)
- [Software Architecture](software-architecture.md)
- [Project Plan](project-plan.md)
- [Project Schedule](project-schedule.md)
- [Project Requirements](project-requirements.md)
- [Developer Diary](../devlog/developer-diary.md)


## 11. Prototype Acceptance Criteria 

**Status:** Preliminary task specification. BFS, FBE and FSM remain unimplemented in the uploaded baseline agent; the cases below are **expected behaviors**, not observed test results.

### 11.1 BFS on known grids

Test only known traversable cells initially, using four cardinal, equal-cost moves. From start `[2][2]`:

| Scenario | Known goal | Blocking condition | Expected BFS result |
|---|---|---|---|
| Open grid | `[4][4]` | None | Reachable; shortest path 4 edges |
| Direct path blocked | `[2][4]` | Wall at `[2][3]`, remaining relevant cells open | Reachable; shortest path 4 edges |
| Surrounded | `[4][4]` | Walls at `[1][2]`, `[2][1]`, `[2][3]`, `[3][2]` | Unreachable |
| Goal not observed | None | No confirmed goal cell | No claimed goal route; fall back to exploration |

A BFS planner is **not** a movement-outcome sensor. Its path is correct for the modeled known graph; following that path in the host still depends on actual legal move execution.

### 11.2 Exploration and FSM acceptance gates

- **Local fallback:** known adjacent walls and occupied cells are excluded; `ACTION_WAIT` is possible when all four cardinal neighbors are invalid.
- **Frontiers:** enable persistent FBE only when map alignment and movement/position estimates have been validated. Select a reachable **known floor** adjacent to unknown terrain, not an assumed passable unknown cell.
- **FSM:** test guarded transitions `EXPLORE` → `APPROACH_FLAG` only for a known usable goal; `PICKUP` → `RETURN` only after proven possession; move to `RECOVER` on contradictory route/pose data. Opponent reactions remain conditional on actual API observations/rules.
- **Optional A*:** postpone until basic BFS/unknown-map navigation is validated and a measured computational need exists.

### 11.3 Research checkpoints

Re-read relevant portions of Yamauchi (FBE prerequisites), Korf (online search/action) and Sturtevant (benchmark design) when full texts are available; add a verified FSM/control source rather than inventing one. Finalize the objective and denominator for each proposed metric in the [Research Plan](research-plan.md), and document the different eight-way versus four-way baseline action sets before claiming any performance difference.

