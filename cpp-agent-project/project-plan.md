# Project Plan — C++ Capture The Flag Agent

**Modules:** Software Engineering and Research Project  
**Institution:** West Saxon University of Applied Sciences Zwickau (WHZ)  
**Team:** Aiswarya Payakkal Vinayan and Nirmal Mohan  
**Project repository:** [cpp-agent-project](https://github.fh-zwickau.de/aip25p5h/cpp-agent-project)  
**Planning period:** 19 October 2026 – 31 January 2027  

## 1. Project Background and Problem

The project aims to build an autonomous C++ agent for the graphical **Capture The Flag** game. The game host loads an agent as a Windows dynamic-link library (DLL) and calls functions defined in `agent_api.h`. On each turn, the agent receives a **5 × 5 local view** of its surroundings and selects an action.

The original example agent chooses movement directions randomly. The team's initial SafeMovementAgent checks visible adjacent cells and avoids requesting movement into some known obstacles, but its behavior can still be repetitive. The research problem is to determine how an agent with limited observation can navigate more safely and efficiently, explore unfamiliar areas, and respond to game objectives.

The project combines **software engineering** (DLL integration, design, implementation, automated tests and maintainability) and **research** (literature review, hypotheses, baseline measurements, algorithm comparisons and analysis).

**Week 6 implementation focus (professor feedback):** Develop, explain and evaluate **our own ScoutAgent** on multiple authored maps. Keep prior random/safe agents for API history and regression only, and limit literature reading to techniques that change code or measurement. The existing 14 WP identifiers, names, owners, hours, dates and approved Gantt **remain unchanged**. Implementation and experiments should record actual outcomes and limitations.

## 2. Objectives and Expected Results

1. Maintain a host-compatible C++ agent DLL and the five required API exports.
2. Verify movement legality, failed-move behavior, flag interaction rules and useful opponent information.
3. Introduce conservative movement-state tracking and safe fallback before relying on persistent global coordinates.
4. Develop **Frontier-Based Exploration (FBE)** for unknown maps after position-confidence validation; use **safe local novelty exploration** as the always-available fallback.
5. Introduce a **Finite-State Machine (FSM)** for exploration, flag pickup/delivery, known-goal pursuit, visible-opponent response and recovery; use **BFS** to travel along known traversable routes. Keep **A*** optional.
6. Investigate opponent-aware responses based on visible evidence and confirmed rules.
7. Compare random, safe, exploration and hybrid policies across varied development maps and held-out unseen maps.
8. Deliver traceable source, tests, research analysis, diary and final report.

**Expected result:** a robust, documented competition-oriented agent and evidence-based experimental report. Match success and algorithm superiority are **research goals**.
## 3. Scope and Boundaries

**In scope:** supplied host/DLL API compatibility; local snapshot decoding; safe movement; movement-outcome and state verification; exploration under unknown maps; remembered map where consistent; BFS over known cells; observed flag and opponent behavior; controlled tests against baseline agents and multiple map layouts; documentation.

**Out of scope:** changing the supplied host or `agent_api.h`, obtaining the secret competition layout, using hidden opponent state unavailable through the API, developing a replacement GUI, or guaranteeing victory. A* and incremental search remain optional research comparisons.

The detailed functional and non-functional requirements, acceptance checks, assumptions and API restrictions are recorded in [Project Requirements](project-requirements.md). The project definition is recorded in [Project Description](project-description.md).

## 4. Development and Project Management Approach

We propose an **iterative and incremental software development model** supported by a **lightweight Kanban workflow in GitHub Issues**. Instead of waiting until all navigation features are implemented, the team will keep a working DLL, add one improvement at a time, and test it before the next change.

The recurring development cycle is:

**Plan → Design → Implement → Test → Evaluate → Review and update the backlog.**

We will track work using **To Do / In Progress / Review / Done** issue states (or equivalent GitHub labels/board columns). Each task should identify its work package, owner, deliverable, dependency and review evidence. Relevant code/document changes should be committed with descriptive messages; larger changes should be reviewed through a pull request where practical.

The **Gantt chart** describes the overall timetable and cross-stream dependencies; GitHub Issues track weekly implementation progress. They serve complementary purposes.

### Review and change management

- Meet or coordinate **at least weekly** to update issues, effort estimates and risks.
- Review a change before marking its task complete. GitHub Issues do not have a dedicated reviewer field; name the reviewer in the issue body/comment, or use a pull-request review.
- Record important technical decisions, tests, problems, individual contributions and professor feedback in `devlog/developer-diary.md`.
- Revise requirements and the Gantt chart when test results or supervisor feedback make the original estimates unrealistic.

## 5. Team Roles and Responsibilities

| Team member | Primary role | Main responsibilities |
|---|---|---|
| **Aiswarya Payakkal Vinayan** | Software Engineering lead and development coordination | Requirements, architecture/API verification, map memory, BFS prototype, A* and flag behavior, software development coordination |
| **Nirmal Mohan** | Research, testing and evaluation lead | Research questions, literature review, hypotheses, experimental design, baseline tests, comparisons, results analysis |
| **Both** | Joint delivery and quality | Gantt chart and progress reviews, GitHub collaboration, integration, debugging, QA, documentation, final review and presentation |

**Review practice:** Each student reviews the other's primary deliverables. Ownership means responsibility for the work package, **not** proof that it has been completed.

## 6. Work Packages and Effort Allocation

This plan is aligned with the accompanying [Project Schedule](project-schedule.md). It uses the **same WP identifiers, owners and estimates**; dates and the visual Gantt chart belong in that schedule document.

| WP | Work package | Responsible | Hours | Planned output |
|---|---|---|---:|---|
| WP-01 | Project Definition and Requirements | Aiswarya | 12 | Review objectives, host rules and competition-specific acceptance checks. |
| WP-02 | API, movement outcome & pose checks | Aiswarya | 14 | Validate DLL/API, movement failure detection and limits of relative position inference. |
| WP-03 | Map memory & confidence tracking | Aiswarya | 30 | Prototype local/persistent memory; represent uncertainty and reset on contradictions. |
| WP-04 | Frontier exploration & safe fallback | Aiswarya | 24 | Local novelty fallback, map-valid frontier detection/selection and reachability tests. |
| WP-05 | FSM, BFS, flags & opponent response | Aiswarya | 30 | Implement/test FSM state transitions, BFS known-route navigation, flag interactions and observed opponent responses.|
| WP-06 | Literature & hypotheses | Nirmal | 24 | Review exploration, real-time search, fair benchmarks and hypotheses; document actual reading depth. |
| WP-07 | Metrics & experimental design | Nirmal | 18 | Define objective detection, reliability, movement, win-rate metrics and test controls. |
| WP-08 | Test Harness & baseline runs | Nirmal | 24 | Build repeatable random/safe baselines on multiple maps; reserve unseen maps for evaluation. |
| WP-09 | Algorithm COmparison experiments | Nirmal | 28 | Compare safe baseline, FBE alone, FBE+BFS and full FBE+BFS+FSM when implemented, on held-out maps/opponents. |
| WP-10 | Robustness & validity analysis | Nirmal | 20 | Analyze win rate when observable, failures, uncertainty and between-map variation. |
| WP-11 | Schedule, review & competition briefing | Both | 12 | Update planning and prepare Week 4 research progress briefing; review feedback. |
| WP-12 | Agent integration, opponent tests & QA | Both | 34 | Integrate hybrid agent, perform host compatibility and opponent/robustness tests. |
| WP-13 | Final documentation & research report | Both | 20 | Reproduce source, test conditions, baseline comparisons and limitations. |
| WP-14 | Contingency & final review | Both | 10 | Use buffer for critical defects, unknown map failures and final review. |
| | **Total** | | **300** | |

### Workload distribution

| Workstream | Hours |
|---|---:|
| Software Engineering — Aiswarya | 110 |
| Research and evaluation — Nirmal | 114 |
| Joint tasks | 76 |
| **Total team effort** | **300** |


## 7. Schedule, Milestones and Dependencies

The detailed start/end dates for each work package and the Gantt chart are documented in [Project Schedule](project-schedule.md). The planning horizon is **19 October 2026 to 31 January 2027**.

| Milestone | Proposed timing | Expected evidence |
|---|---|---|
| M1 — Project definition | October 2026 | Requirements, initial research and Week 3 planning record |
| Week 4 progress review | **30 October 2026** | Literature findings, preliminary hybrid strategy and open questions |
| M2 — Design and baselines | November 2026 | API/movement validation, research protocol, baseline fixtures, early state design |
| M3 — Exploration and integration | December 2026 | Tested exploration/navigation prototypes, host and opponent QA |
| M4 — Competition evaluation and report | January 2027 | Held-out map comparisons, limitations and final report |

**Main dependencies:** confirm host rules and movement outcomes → implement safe local fallback and pose confidence → build map/frontier selection where reliable → add BFS for known routes → add the FSM for objective/flag/opponent/recovery decisions → run comparative competition tests → report. **Local fallback remains active even if the global-map branch fails.**

**Parallel work:** literature, metrics and baseline testing begin before advanced modules are ready. WP-09 may begin with earlier working policies while later variants are still being integrated. Tests include only implementations actually available.

The official final submission date and full competition rules require confirmation.

### 8. Research Approach and Evaluation

**Research focus:** Can an online **hybrid exploration + goal-navigation** strategy generalize to unseen competition maps better than existing movement baselines?

The revised research questions address (1) novelty/exploration on unknown maps, (2) failed movements and localization confidence, (3) value of BFS when goals are known, (4) reaction to visible opposing agents and (5) generalization to held-out maps and varied opponents.

**Preliminary approach, not a tested result:** **Frontier-Based Exploration + BFS + Finite-State Machine (FSM)**, with **safe local exploration** if map/pose confidence is insufficient. FBE selects *where* to explore, BFS decides *how* to reach a known reachable target, and the FSM decides *which game objective* has priority.

**Evaluation design:** develop multiple varied training/development maps and reserve separate unseen evaluation maps. Compare reference random, safe movement and any validated hybrid variants using consistent movement rules where possible, identical map/opponent conditions, repeated runs and documented seeds. Do not request or memorize the friend's secret competition layout.

**Early measurable outcomes:** API validity, known-obstacle requests, immediate requested reversals, decision latency and crashes. **Conditional later outcomes:** time to find an objective, verified revisits, flag deliveries, match outcomes/win rates, depending on position and host feedback. Record failed experiments and limitations.

The detailed methodology is in [Research Plan](research-plan.md) and [Algorithm Comparison](algorithm-comparison.md); the evidence base is in [Literature Summary](literature-summary.md).

## 9. Software Architecture and Technical Constraints

The architecture consists of the **graphical host** and our **C++ agent DLL**, which implements:

- `Agent_GetName`
- `Agent_Create`
- `Agent_Destroy`
- `Agent_Reset`
- `Agent_Step`

The host provides a `WorldSnapshot` containing `cells[5][5]`, with the agent at local cell `[2][2]`, plus a `lastAction` value. `Agent_Create` provides map dimensions, **not absolute coordinates**. The interface defines terrain/content bitmasks, eight movement directions, waiting, picking up the flag and putting it down.

The software should separate the host-facing API adapter from observation decoding, decision policy, agent state/memory and optional pathfinding modules. **Movement requested does not necessarily mean movement succeeded**; position tracking must be verified before relying on a global map.

The DLL must be compiled for a Windows architecture compatible with the provided host. Week 1 and Week 2 sample APIs are different and must not be mixed.

See [Project Requirements](project-requirements.md) for technical constraints and acceptance checks. Architecture documentation should be maintained in [Software Architecture](software-architecture.md).


## 10. Quality Assurance and Acceptance Criteria

| Area | Evidence required |
|---|---|
| DLL/API reliability | Host loads the compiled agent; API exports and reset/destroy lifecycle function correctly. |
| Local safe movement | Fixed 5 × 5 fixtures confirm known walls/occupied cells are not chosen where excluded by policy. |
| Movement uncertainty | Deliberately blocked actions do not silently corrupt persistent map or pose; fallback is logged. |
| Unknown-map exploration | Evaluations use maps not revealed to the tested policy before the run. |
| Navigation and flags | BFS plans only through known legal traversable cells; FSM pickup/return/drop transitions require confirmed host evidence. |
| Exploration and fallback | Reachable frontiers are selected only from trusted map data; failures and contradictory observations return to safe local movement. |
| Control module | Synthetic tests cover valid FSM transitions, state reset and absence of unsupported flag-possession assumptions. |
| Competitive robustness | Different opponent policies and map layouts are used; record win/loss if the host exposes verified outcomes. |
| Research reproducibility | Agent revision, map version, opponent, random seed, trials and failures documented. |
| Documentation | README, decisions, schedule, diary and experiment reports consistent and reviewed. |

Numerical success thresholds have **not** been confirmed; establish them after initial measurements rather than inventing targets.

## 11. Risks, Mitigations and Assumptions

| Risk / uncertainty | Consequence | Planned mitigation | Owner |
|---|---|---|---|
| Map hidden until game starts | Hard-coded routes fail | Explore online; use varied held-out maps | Both |
| Local 5 × 5 view without absolute coordinates | Map alignment unreliable | Validate move outcomes; record confidence; safe fallback | Aiswarya |
| Requested move may be rejected | Pose estimate drifts | Compare observations; treat action request as uncertain | Both |
| Unknown opponent mechanics | Incorrect tactical responses | Respond only to visible signals; confirm rules | Aiswarya |
| Secret map inadvertently used for tuning | Invalid generalization evidence | Never use final map for development; reserve independent test layouts | Nirmal |
| Different cardinal/diagonal movement sets | Unfair comparisons | Equal-direction controls; disclose differences | Nirmal |
| No reliable match score or flag telemetry | Cannot measure win rate objectively | Mark outcome metrics unavailable; use verifiable surrogate tests | Nirmal |
| Schedule overrun | Advanced features incomplete | Prioritize safe fallback, exploration and tested flags before optional A* | Both |
| DLL/compiler mismatch | Host cannot load agent | Keep Week 1/2 APIs separate; use compatible Windows toolchain | Both |
| Documentation out of sync | Conflicting claims | Weekly review, GitHub Issues, dated decision record | Both |


## 12. Deliverables and Repository Organization

The intended documentation set is:

```text
cpp-agent-project/
├── README.md                         # Overview, building and testing
├── docs/
│   ├── project-description.md        # Project background and objective
│   ├── project-requirements.md       # Functional/non-functional requirements
│   ├── project-plan.md              
│   ├── project-schedule.md           # work packages and Gantt chart
│   ├── software-architecture.md      # DLL architecture and API
│   ├── research-plan.md              # Questions and methodology
│   ├── literature-summary.md         # Academic sources
│   ├── algorithm-comparison.md       # Candidate strategy comparison
│   └── algorithm-research.md         # Optional index/overview
├── devlog/
│   └── developer-diary.md            # Dated contributions and decisions
└── tests/
    ├── AgentDLLTest/                 # reference
    └── Week02_HostandAgent/          # graphical-host example
```

## 13. Week 6 implementation priorities and professor feedback (9–15 November 2026)

**No Gantt changes:** the approved `docs/Gantt_Chart.xlsx` remains the source for all 14 WP titles, hours, dates, owners and statuses. The priorities below explain work *within* existing packages, not additional work packages or claimed completion.

| Professor's request | Existing WP / responsible | Practical evidence / acceptance |
|---|---|---|
| Effective steps for software and research | WP-02/03 Aiswarya; WP-06/07/08 Nirmal | Sequence host/API checks → agent tests → map metrics → review; record commit, compiler and failure cases |
| Create our **own** agent | WP-02 Aiswarya; WP-08 Nirmal testing | Distinct `scout_agent.cpp`, own test suite; 19 unit checks passed offline; Windows host remains pending |
| Only necessary literature | WP-06 Nirmal | Focus on Yamauchi 1997, Korf 1990 and Sturtevant 2012 only where relevant; record actual sections read |
| Explain strategy | WP-02/03 Aiswarya, research interpretation Nirmal | Safety gate → visible-goal local BFS → novelty-scored safe movement → WAIT; see `software-architecture.md` |
| Add map tests | WP-07/08 Nirmal | Six development layouts and one unopened holdout; collect blocked requests, visits, waits and flag-cell arrivals in the offline simulator |

**Suggested Week 6 execution order (assignments, not confirmed personal completion):**

1. **Aiswarya (WP-02):** inspect agent source and Windows build; confirm all five DLL exports with the original host; document real movement and pickup outcomes or state explicitly why they are unverified.
2. **Nirmal (WP-07/08):** reproduce the scripted unit/movement/map runs in MSYS2 UCRT64; record compiler, Git revision, map fixture, budget and results; leave `reserved_holdout.map` untouched during tuning.
3. **Aiswarya (WP-03):** investigate snapshot-based motion ambiguity and a trusted-pose gate before building a persistent global map.
4. **Nirmal (WP-06):** add only brief verified notes from the relevant sources showing exactly which strategy or measurement they influenced.
5. **Both:** review code/results, update the existing diary, confirm actual authors/testers and push the changes. Do not mark WP-02/03/06/07/08 complete based only on standalone tests.

**Recorded preparation evidence (11 Nov 2026):** 19/19 ScoutAgent tests, 8/8 synthetic movement tests, six development-map fixtures and 5/5 Linux exported symbols passed in a separate environment. This is not Windows host certification or personal execution evidence for either team member. See [Research plan](research-plan.md) for map metrics and hypotheses, [Software architecture](software-architecture.md) for technical implementation and host checklist, and the [Developer diary](../devlog/developer-diary.md) for dated status.
