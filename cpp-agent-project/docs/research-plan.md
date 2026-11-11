# Research Plan — Autonomous C++ Capture The Flag Agent

**Module:** Autonomous Research Project (KFT11050), coordinated with Software Project (ELT10210)  
**Project:** WHZ Capture The Flag agent DLL  
**Researchers:** Nirmal Mohan (research lead); Aiswarya Payakkal Vinayan (software planning and technical reviewer)  
**Status:** Research proposal; experimental findings have **not** yet been collected.

**Week 6 scope adjustment (11 November 2026):** Following supervisor feedback, the immediate study is **our own ScoutAgent** across different map fixtures, including internal improvements between its revisions. `StupidAgent` remains only an older API/regression reference, **not the main Week 6 performance comparison**. We will read only paper sections that directly inform implementation or measurements; results from the custom-map simulator must not be called Windows-host or competition results. The official Gantt is unchanged.

## 1. Background and motivation

The `StupidAgent` and the team's obstacle-aware `SafeMovementAgent` provide working baseline policies, but they do not systematically explore or pursue flags. The supplied host gives only a **5 × 5 local** snapshot, and `lastAction` is not guaranteed to mean a successful movement.


**Research aim:** evaluate whether an explicit **Frontier-Based Exploration + BFS + Finite-State Machine** design, protected by safe local fallback, improves unseen-map competition outcomes compared with reference movement policies. More specifically, study whether a conservative hybrid of safe local movement, novelty/frontier-inspired exploration (when memory is reliable), BFS routes to visible/remembered goals and observed-opponent responses generalizes to unseen maps. This is a proposal only; tournament results and winning performance are not yet known.

## 2. Objectives and research questions

**O1:** Establish repeatable measurements for our own ScoutAgent on multiple authored maps; retain prior example-agent checks as API regression evidence.  
**O2:** Evaluate exploration progress and goal discovery on unseen maps without leaking the secret competition layout.  
**O3:** Study movement-success uncertainty and the feasibility of persistent map memory.  
**O4:** Test whether known-route BFS and validated objective actions improve progression toward the flag.  
**O5:** Evaluate behavior across multiple opponent types and report uncertainty, failures and generalization.

| Research question | Hypothesis (not yet tested) |
|---|---|
| Does novelty/frontier-inspired exploration discover more useful map area or objectives than safe random movement on unseen maps? | Information-seeking movement may improve discovery when position inference works. |
| How do blocked or uncertain moves affect map memory and navigation? | Confidence-aware tracking plus fallback should reduce catastrophic mapping errors. |
| When a reachable objective is known, does BFS improve route efficiency over safe random movement? | BFS should provide shortest known unit-cost paths under confirmed movement rules. |
| Can visible-opponent-aware decisions improve reliable objective progress? | Avoiding occupied/threatening routes may help, conditional on actual game rules. |
| Does the hybrid agent generalize across held-out maps and opponent policies? | A robust hybrid may outperform local policies, but results could depend on layout and opponents. |


## 3. Literature search method

Search IEEE Xplore, ACM Digital Library, Google Scholar, and university/author publication pages using combinations of: `frontier exploration unknown maps`, `finite state machine autonomous game agents`, `online CTF strategy`, `grid pathfinding A*`, `autonomous frontier exploration partial map`, `real-time heuristic search`, `incremental replanning unknown terrain`, and `pathfinding benchmarks game maps`.

**Inclusion criteria:** peer-reviewed or archival conference/journal publications; a clear algorithm or evaluation methodology; relevance to grid maps, partial observation, exploration, replanning or experimental comparison. **Exclusion criteria:** unsourced tutorials as primary evidence, papers unrelated to discrete navigation, and works whose results cannot be adapted meaningfully to this API.

For each paper, record author, year, venue/DOI, key idea, potential adaptation, limitation, and whether the full text has been read. The publications below have been **identified and their bibliographic details checked**; identification is not the same as a completed in-depth literature review. Preliminary annotated contributions, project-specific limitations and full-text reading tasks are recorded in [literature-summary.md](literature-summary.md).

### Selected research papers

| Ref | Publication | Relevance to this project | Application priority |
|---|---|---|---|
| [R1] | B. Yamauchi (1997), *A Frontier-Based Approach for Autonomous Exploration*, IEEE CIRA, pp. 146–151. [DOI](https://doi.org/10.1109/CIRA.1997.613851) | Explores borders between known and unknown space; relevant to repeated-region behavior and eventual exploration memory. | **High** |
| [R2] | P. E. Hart, N. J. Nilsson, B. Raphael (1968), *A Formal Basis for the Heuristic Determination of Minimum Cost Paths*, IEEE Transactions on Systems Science and Cybernetics 4(2), pp. 100–107. [DOI](https://doi.org/10.1109/TSSC.1968.300136) | Foundational A* heuristic pathfinding; useful for later goal-directed navigation. | Medium |
| [R3] | R. E. Korf (1990), *Real-Time Heuristic Search*, Artificial Intelligence 42(2–3), pp. 189–211. [DOI](https://doi.org/10.1016/0004-3702(90)90054-4) | Interleaves decisions and execution; relevant when the host expects one action per call. | **High** |
| [R4] | S. Koenig, M. Likhachev (2002), **D\* Lite**, Proceedings of AAAI-02, pp. 476–483. [Paper](https://publications.ri.cmu.edu/d-lite) | Replanning on terrain that becomes known over time; possible later algorithm once map tracking exists. | Medium |
| [R5] | N. R. Sturtevant (2012), *Benchmarks for Grid-Based Pathfinding*, IEEE Transactions on Computational Intelligence and AI in Games 4(2), pp. 144–148. [DOI](https://doi.org/10.1109/TCIAIG.2012.2197681) | Motivates fair scenarios and consistent performance metrics for grid navigation experiments. | **High** |
| [R6] | S. Koenig, M. Likhachev, D. Furcy (2004), **Lifelong Planning A\***, Artificial Intelligence 155(1–2), pp. 93–146. [DOI](https://doi.org/10.1016/j.artint.2003.12.001) | Reuses search information when map costs change; advanced future work, not required for the initial prototype. | Low / future |

Additional comparison of requirements, implementation statuses, validity threats and next-step tests appears in [algorithm-comparison.md](algorithm-comparison.md).

## 4. Candidate algorithms and implementation status

| Method | Role in competition | State as of revised proposal | Key limitation |
|---|---|---|---|
| `StupidAgent` | Eight-way random baseline | Demonstrated with graphical host | Does not avoid obstacles or find goals |
| `SafeMovementAgent` | Four-way obstacle-aware baseline | Demonstrated with graphical host; version details must be logged | No dependable map memory |
| Reverse-disfavoring safe policy | Low-cost local fallback | Source available in repository; no controlled improvement demonstrated | Can repeat long loops; prior action may fail |
| Local novelty heuristic | Select locally less-familiar moves | Proposed | Without absolute position, novelty may be only approximate |
| Frontier-based exploration | Select boundary of known/unknown map | Literature-backed proposal | Requires stable pose/map alignment |
| BFS on known traversable graph | Route planner to discovered goals | Proposed | Unknown goal cannot be routed to directly |
| Visible-opponent policy | Avoid known collisions or threats | Proposed; rules unverified | Host may not expose enough opponent information |


## 5. Research methodology and experiment design

### Phase A — Verify observable mechanics

Use controlled 5 × 5 snapshots and graphical-host runs to verify cell decoding, waiting, blockers, DLL lifecycle and action codes. Determine whether movement success can be detected, how diagonals behave, what flag pickup/delivery requires and what opponent state is visible. Treat unverified rules as assumptions.

### Phase B — Baseline and generalization test design

Test random and safe baselines using multiple authored development maps with corridors, loops, dead ends and open cells. Reserve **separate held-out maps** created without referencing the secret competition layout. Document opponent policy, allowed moves, agent revision, map ID, seed if controllable, start conditions, trial count and failures. Consider a four-direction random control to normalize direction-set differences.

### Phase C — Build and validate FBE + BFS + FSM incrementally

Implement/test a local novelty fallback first. If displacement inference is validated, attempt persistent map memory and frontier objective selection. Add BFS to route between known traversable cells when the goal position is known. Check any predicted movement against the next observation before trusting state updates. Unit-test synthetic maps before host integration.

**FSM verification (additional gate):** use synthetic event sequences for `EXPLORE`, `APPROACH_FLAG`, `PICKUP`, `RETURN`, `DELIVER`, `EVADE` and `RECOVER`; verify action validity and no transition based solely on an unconfirmed requested movement or flag action. Fallback must work without a global map.

### Phase D — Evaluate behavior

Run validated policies on **held-out maps** against multiple available opponent policies (e.g., reference/random, safe baseline, another controlled agent), recording score or win result only if verifiable. Compare controlled ablations: safe baseline, frontier policy without BFS, frontier+BFS with simple objective rules, and frontier+BFS+FSM, where all variants can be implemented reliably. Compare optional opponent-response subvariants separately to attribute improvements. Report dispersion, failed runs and rule uncertainties.

## 6. Evaluation metrics and evidence

| Metric | Proposed operational definition | Availability |
|---|---|---|
| API/DLL stability | Runs with no load, lifecycle or step failure / attempted runs | Can test now |
| Valid action rate | Defined action code count / total `Agent_Step` calls | Can test now |
| Known-blocked request rate | Movement requests into **visible known** wall/occupied cells / movement requests | Can instrument now |
| Immediate requested reversal rate | Opposite movement requests / eligible action transitions | Can instrument now; not actual backtracking |
| Decision latency | Mean/median and upper percentile time per `Agent_Step` | Requires timer instrumentation |
| Objective discovery | Turns to first verified enemy-flag sighting | Requires repeatable observation logs |
| Coverage and physical revisits | Unique verified absolute cells / total reachable cells; revisit fraction | **Deferred** until localization is reliable |
| Flag capture/delivery  | Verified successful game events or wins / completed comparable trials | **Deferred** until host event/result observable |
| Generalization robustness | Outcomes summarized across held-out maps and opponent policies, including failures | After comparable competition tests |
| FSM transition validity | Transitions justified by confirmed observation/event or explicit uncertainty guard / tested transitions | Synthetic FSM fixtures first; host telemetry later |
| Recovery/fallback activation | Number of uncertainty/invalid-route cases that return safely to local movement or wait | Synthetic contradictory-snapshot tests, then host logs |


## 7. Software engineering and research integration

1. **API analysis** defines observable inputs and actions (software engineering).  
2. **Research questions** identify a behavioral weakness to investigate (research).  
3. **Helpers and policy variants** implement testable approaches (software engineering).  
4. **Repeatable tests and measurement** gather comparable evidence (both).  
5. **Results** guide the next algorithmic choice and changes to agent state or architecture (research feeding back into software engineering).

Keep the API exports stable. Separate any future snapshot decoder, movement validator, policy selection, and logging functionality as the code grows. Store experiment scripts, test fixtures, results, and links to source revisions in the repository. Record actual work and responsible contributor in `devlog/developer-diary.md`.

## 8. Risks, assumptions and open questions

- **Map unknown:** Do not use the actual secret layout as a development asset; evaluate on independent maps.
- **Localization uncertainty:** API does not supply absolute coordinates; a requested action does not confirm displacement.
- **Opponent rules uncertain:** Avoid assuming attacks, capture mechanics, flag possession or global opponent locations.
- **Direction-set confound:** Reference random agent uses eight moves; safe agent uses four. Normalize where possible.
- **Partially observed objectives:** BFS finds routes through known traversable cells; it does not independently discover an unseen flag.
- **Outcome measurement:** Win rate is measurable only when a verifiable result can be captured from the host.


## 9. Baseline Experiment Protocol and Measurement Definitions

**Status:** Proposed reproducible procedure for WP-07 and preparation for WP-08 (scheduled to begin 9 November). **No benchmark runs or numerical improvements are reported here.**

### 9.1 Measurement rules

| Measurement | Numerator / observation | Denominator / conditions | Feasibility and caution |
|---|---|---|---|
| Valid action rate | `Agent_Step` returns an action defined in the Week 2 API | All recorded `Agent_Step` calls | Requires recording each decision; action validity is not proof of successful execution |
| Known-blocked request rate | Movement action points to a neighbor observed as `CELLTYPE_WALL` or occupied by `SELF/ENEMY` | All recorded movement requests for which the requested neighbor is in the current snapshot | Logs policy compliance on **visible** blockers, not actual collision rate |
| Immediate requested reversal rate | Consecutive cardinal movement *requests* point in opposite directions | Eligible adjacent pairs of cardinal movement requests | Exclude waits/flag actions using a declared convention; not physical backtracking |
| DLL reliability | Host run finishes without a load, step or lifecycle failure | Attempted comparable runs | Record crashes/timeouts and complete/partial runs explicitly |
| Decision latency | Time within the agent decision callback | Each measured step | Use a sufficiently precise timer; distinguish debug printing overhead from policy cost |
| BFS path length (later) | Number of edges in a returned route | One known, reachable unweighted grid test | Check shortest route independently; report unreachable and unknown-cell cases separately |
| Actual revisits/coverage (later) | Verified physical positions or visited cells | Successful moves / known reachable cells, depending on metric | **Not valid** until location/movement tracking is reliable |
| Flag result/win rate (later) | Verified host pickup/delivery or match result | Comparable completed trials | Do not count `ACTION_PICK_UP_FLAG` requests as successful pickups |

When a metric has zero eligible observations, report **N/A** rather than dividing by zero. Store absolute counts alongside percentages. No target thresholds have yet been established.

### 9.2 Controlled baseline comparison

1. **First validate logging:** use known-wall, unknown, occupied-neighbor and wait conditions from [Software Architecture](software-architecture.md). Distinguish a requested move from its actual outcome.
2. **Define policies:** professor's eight-direction `StupidAgent`; existing four-direction `SafeMovementAgent`; optionally a four-direction random control to isolate obstacle filtering from movement-set differences.
3. **Specify scenarios:** open areas, narrow corridors, dead ends and loops, built independently of the secret competition map. Reserve different layouts as *held-out* tests for later generalization claims.
4. **Control the conditions:** record map identifier/version, agent source commit, host version, opponent policy, start conditions, allowed movement rules and random seed when controllable. Runs lacking comparable conditions are descriptive, not causal tests.
5. **Pilot design:** begin with up to 10 repeated runs per comparable agent/scenario **if** the graphical host supports repetition. The pilot count can be revised after checking practicality and variance; it is not a completed experiment or mandated sample size.
6. **Analyze only observed results:** retain failed runs, report counts, mean/median and range when available, and note any uncontrolled randomness or absent telemetry.

### 9.3 Known-layout BFS verification 

The known-grid BFS prototype has already passed five standalone tests in Week 5. For future extensions, use four cardinal directions and unit movement costs. An agent at local `[2][2]` (row, column) can start with: (A) an open 5 × 5 grid and a visible goal at `[4][4]` (shortest length **4**); (B) a wall at `[2][3]` and goal at `[2][4]` (shortest length **4** via a detour if the surrounding floor is open); (C) walls at `[1][2]`, `[2][1]`, `[2][3]`, `[3][2]` (no cardinal route from center). These are **analytically specified expected results**, not executed tests. Unknown cells are never silently treated as floor.



## 10. Week 6 — Focused own-agent experiment and reproducible results (11 November 2026)

Following professor feedback, the immediate research target is **our own ScoutAgent**, not a new performance comparison with `StupidAgent`. Earlier baseline discussions remain historical and WP-09's later validated-variant comparisons are not yet performed. This section merges strategy-linked literature, hypotheses, detailed map measurements, limits and pending research actions in the existing Research Plan.

### 10.1 Only literature relevant to current engineering decisions

| Existing source | Concept used | Our current implementation/measurement boundary |
|---|---|---|
| Yamauchi (1997), frontier exploration (R1) | Exploration at known/unknown boundaries | Motivates future map-aware frontier choice; **ScoutAgent is only local novelty, not FBE** |
| Korf (1990), real-time heuristic search (R3) | Repeated decisions using current perception | Motivates an online stepwise decision, **not** a claim that Korf's algorithm is implemented |
| Sturtevant (2012), grid benchmarks (R5) | Diverse map layouts and repeatable measurements | Motivates six authored development layouts and a reserved holdout, **not** use of official benchmark maps |

A*, D* Lite and LPA* reading/development are deferred unless a verified need arises. **This selection is based on the existing project literature survey; it is not proof that Nirmal personally read new full-text sections on 11 November.** Record page/section, date and exact design consequence after reading.

### 10.2 Own-agent testable hypotheses

| ID / question | Observable hypothesis | Current evidence and limitation |
|---|---|---|
| H1 / RQ1 | ScoutAgent requests **zero known-blocked moves** on the authored development maps | All six offline fixtures: zero such requests; host behavior unknown |
| H2 / RQ2 | ScoutAgent reaches predefined **simulator coverage** thresholds under fixed step budgets | Six fixtures met thresholds; simulator coordinates are not agent-known coordinates |
| H3 / RQ3 | For a visible reachable enemy flag, local BFS chooses a safe first step | Agent unit checks and five simulated flag-cell arrivals; flag pickup/possession unverified |
| H4 / RQ4 | Consecutive local views may leave physical movement uncertain | Eight synthetic movement-observation checks passed; helper is not integrated with host |

### 10.3 Executed separate-environment tests

**Environment:** Linux `g++` on 11 Nov 2026, not original Windows graphical host. Agent unit tests **19/19**, motion-observation tests **8/8**, map fixtures **6/6**, native API export symbol checks **5/5**; no claim of successful Windows DLL loading or competition wins. The test programs were compiled with `-std=c++17 -O2 -Wall -Wextra -pedantic` (native stricter compilation also checked separately in the earlier evidence).

| Authored map | Purpose | Step budget | Simulator visited / floor | Blocked requests | Waits | First flag-cell arrival (step) | Pickup requests |
|---|---|---:|---:|---:|---:|---:|---:|
| `open_arena.map` | Open floor | 180 | 77/77 | 0 | 0 | 13 | 5 |
| `corridor_turns.map` | Turns and narrow corridors | 180 | 49/49 | 0 | 0 | 15 | 3 |
| `branching_deadends.map` | Dead ends / branches | 180 | 54/54 | 0 | 0 | 20 | 4 |
| `blocked_start.map` | No legal adjacent floor | 12 | 1/1 | 0 | 12 | N/A (no flag) | 0 |
| `two_rooms.map` | Connection through narrow gap | 180 | 85/85 | 0 | 0 | 82 | 2 |
| `zigzag_barriers.map` | Offset walls | 180 | 111/111 | 0 | 0 | 90 | 2 |

**Acceptance rules:** all six require zero blocked requests; traversable-cell visit thresholds are 50, 30, 30, 1, 50, 70 respectively in the table order; for the five layouts with flags, require arrival at the flag cell and at least one **pickup request**. These are developer-authored simulator thresholds, **not official competition criteria**. The seventh fixture `reserved_holdout.map` is **not run during development**, to be used after freezing the agent revision. Raw captured separate-environment run: [`../evidence/week06-local-verification-2026-11-11.txt`](../evidence/week06-local-verification-2026-11-11.txt).

**Critical limitations:** The simulator knows physical positions and reports visited cells. ScoutAgent itself observes only local `cells[5][5]`, and its requested actions may fail in the real host. A pickup **request** is not evidence of possession, return, delivery or win. The movement-observation helper is experimental and can classify outcomes as ambiguous. No 80 × 50 competition map coverage, statistical superiority or reliable absolute location is claimed.

### 10.4 Research reproducibility and next steps

1. Nirmal: rerun `bash tests/Week02_HostandAgent/run_week6_tests.sh` on Windows, record date, `g++ -dumpmachine`, `g++ --version`, `git rev-parse --short HEAD`, exact outputs and any failures in the **existing developer diary**.
2. Aiswarya: build/load the Windows DLL in the unchanged original host; record observed movement and pickup feedback as evidence, not inferred success. The executable checklist is in [Software architecture](software-architecture.md).
3. Keep `reserved_holdout.map` unseen during tuning and run it only after freezing the agent version.
4. Maintain step budgets, initial state, compiler, source version, map input, blocked requests, waits, simulator visitation, flag-cell arrivals and pickup requests. If later controlled tests compare own-agent revisions, vary one planned strategy component at a time.
5. Record actual paper sections read, actual teammate review and GitHub push only when personally verified. The approved Gantt remains unchanged.
