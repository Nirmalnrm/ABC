# C++ Capture The Flag Agent

**Modules:** Software Project (ELT10210) and Autonomous Research Project (KFT11050)  
**Institution:** West Saxon University of Applied Sciences Zwickau (WHZ)  
**Team:** Aiswarya Payakkal Vinayan — Software Engineering; Nirmal Mohan — Research and Evaluation  
**Repository:** [aip25p5h/cpp-agent-project](https://github.fh-zwickau.de/aip25p5h/cpp-agent-project)  
**Planning period:** 19 October 2026 – 31 January 2027 (final course deadline to be confirmed)

## Project overview

This project develops an autonomous **C++ Windows DLL agent** for the professor-provided graphical Capture The Flag host. The agent receives a **5 × 5 local `WorldSnapshot`** and returns one permitted action on each `Agent_Step()` call. The competition map is not known in advance, and another team's agent may compete against ours.

The combined project has two connected workstreams:

- **Software Engineering:** Maintain compatibility with the provided DLL interface; implement and test observation handling, navigation, exploration, decision-making and game behavior.
- **Autonomous Research:** Review relevant algorithms, formulate research questions, design controlled comparisons, measure performance and report limitations.

The development process is **iterative and incremental**, using GitHub Issues and lightweight Kanban tracking. The proposed schedule contains **14 work packages and 300 planned person-hours**. These are estimates, not recorded hours worked.

## Preliminary navigation strategy

The selected **research and development direction** is a hybrid of **Frontier-Based Exploration (FBE) + Breadth-First Search (BFS) + a Finite-State Machine (FSM)**, with safe local exploration as a fallback. **A\*** is optional, not a core requirement.

| Component | Role | Important condition |
|---|---|---|
| **FBE** | Select reachable boundaries between known traversable and unexplored cells to discover the unknown map. | Persistent frontiers require a reliable map and position estimate. |
| **BFS** | Find routes to known reachable frontiers or objectives on a known, unweighted traversable graph. | It cannot discover a flag outside the known area on its own. |
| **FSM** | Manage behavior such as exploring, approaching a flag, pickup, return, delivery, visible-opponent response and recovery. | Flag possession and movement-success transitions need evidence from the host. |
| **Safe local fallback** | Choose obstacle-aware local actions or `ACTION_WAIT` if the map, localization or route is uncertain. | Must not assume a global position. |

**Technical constraint:** The API provides local observations and map dimensions but no absolute agent position. `lastAction` does not by itself prove that a requested movement succeeded. Reliable movement tracking must therefore be tested before global mapping is trusted.

**This strategy is proposed, not yet implemented or experimentally validated.** The professor-provided host and `agent_api.h` will not be changed.

## Implementation and evidence status

| Component | Current status / evidence |
|---|---|
| **Week 1 DLL interface test** | Compiled and executed; inputs `10, 20, 30, 40` produced cumulative results `10, 30, 60, 100`. |
| **Week 2 reference `StupidAgent`** | Built and tested in the graphical host; pseudorandomly requests movement in eight directions. |
| **Custom `SafeMovementAgent`** | Initial safe agent loaded and ran in the graphical host; checks four cardinal neighboring cells for known traversable floor and occupancy. Repeated movement was observed. Current source also discourages immediate reverse-action requests, but that improvement has not been quantitatively evaluated. |
| **Movement-outcome / position tracking** | Investigation and controlled validation planned. |
| **Safe local exploration improvements** | Planned; existing SafeMovementAgent is the starting baseline. |
| **Persistent map and frontier exploration** | Planned; dependent on reliable localization and movement-outcome tracking. |
| **BFS and FSM integration** | Planned; no completed host-tested hybrid agent reported. |
| **Flag handling and opponent response** | Planned; game rules and observable feedback need verification. |
| **Controlled experimental comparisons** | Protocol and metrics proposed; no quantitative performance results reported yet. |

## Software development path

1. **Verify the API and game rules:** DLL lifecycle, movement failures, diagonals, flags and observable opponent information (**WP-01–02**).
2. **Establish reliable behavior:** improve local safe fallback and investigate movement/position confidence (**WP-02–04**).
3. **Explore an unknown map:** add mapping and reachable-frontier selection only when state estimates are sufficiently trustworthy (**WP-03–04**).
4. **Pursue game objectives:** integrate BFS routes and FSM transitions, then validate pickup, return, delivery and opponent-response behavior (**WP-05**).
5. **Integrate and harden the DLL:** host tests, failure recovery and opponent trials (**WP-12**). Evaluate A* only if core functionality and time allow.

## Research path

1. **Literature and hypotheses (WP-06):** study frontier exploration, online planning, heuristic navigation and fair grid benchmarks. Record which source sections were actually reviewed.
2. **Metrics and experiment protocol (WP-07):** define measurable safety, reliability, exploration, objective and competitive outcomes; confirm what the host exposes.
3. **Baselines and test fixtures (WP-08):** compare the existing random and safe agents under matched conditions where possible; keep some test maps unseen during development.
4. **Component comparisons (WP-09):** compare only implemented, validated variants (e.g. safe baseline, exploration, FBE+BFS, and FBE+BFS+FSM), while documenting any differences in allowed movement directions.
5. **Analysis and reporting (WP-10, WP-13):** assess failures, decision latency, exploration efficiency, robustness and match outcomes where verifiable. Report limitations and avoid unsupported numerical claims.

**Research focus:** Does hybrid exploration, known-route navigation and FSM control improve objective discovery, navigation reliability and competitive performance on previously unseen maps compared with simpler baselines?

## Project schedule and responsibilities

| Workstream | Lead / participation | Planned person-hours |
|---|---|---:|
| Software Engineering | Aiswarya | 110 |
| Research and Evaluation | Nirmal | 114 |
| Joint integration, planning, QA and documentation | Both | 76 |
| **Total** | | **300** |

Joint work is allocated equally for planning (38 hours each), resulting in **148 planned hours for Aiswarya** and **152 for Nirmal**. The dates and milestones are provisional. See the [project plan](docs/project-plan.md) and [project schedule](docs/project-schedule.md) for the 14 work packages, dates, dependencies and milestones.

## Progress and decisions

### Week 1 — DLL communication

Completed the simple DLL host test. The host loaded the agent and received the expected cumulative values for four inputs. See the [developer diary](devlog/developer-diary.md).

### Week 2 — Graphical host and initial movement agent

Studied the Week 2 API, compiled and tested the professor's reference agent, and demonstrated a custom obstacle-aware agent in the graphical host. The initial agent sometimes revisited a limited region, motivating further exploration research.

### Week 3 — Initial project and research planning

Established an iterative project-management approach; prepared requirements, team roles, work packages, an initial 300-hour Gantt chart and research documents. Initially considered BFS, A*, and exploration methods. **Those early planning choices are superseded where necessary by the current FBE + BFS + FSM proposal.** Existing baseline source files and historical records are preserved.

### Week 4 — Research progress and strategy review 

- **Literature:** Recorded initial findings on frontier exploration (Yamauchi), real-time action selection (Korf), heuristic pathfinding (Hart et al.) and reproducible grid benchmarks (Sturtevant). The annotated survey does not constitute a completed in-depth reading of all papers.
- **Preliminary choice:** Proposed **FBE + BFS + FSM**, retaining a safe local fallback and treating A* as optional. The proposal remains a hypothesis to investigate, not a demonstrated competition improvement.
- **Project review:** Revised the future work packages, dependencies and technical risks for unknown-map competition; retained **14 work packages and 300 planned person-hours**.
- **Progress presentation:** A five-minute group update is required for **30 October 2026**. Slides and speaking notes must be stored in the repository when finalized; their delivery and supervisor feedback should be recorded afterward.
- **Traceability:** Record actual individual contributions, reviewed sources, decisions, test evidence and any supervisor feedback in the developer diary and relevant GitHub Issues.

A completed FBE, BFS, FSM or competition-winning agent is **not** claimed at this stage.

### Week 5 — Automated testing, movement verification and research preparation 

**Work completed :** Nirmal executed the included MSYS2 UCRT64 test script and shared terminal output. All **29 automated checks passed**; the original agent DLLs were also built successfully.

| Tested component | Passed | Failed | Scope of evidence |
|---|---:|---:|---|
| SafeMovementAgent | 15 | 0 | API/lifecycle, known-floor/occupancy filtering, reverse-action preference and wait fallback on synthetic observations |
| StupidAgent | 9 | 0 | Basic API/lifecycle and random eight-direction action generation |
| Standalone BFS prototype | 5 | 0 | Shortest routes and unreachable/unknown-cell cases on known synthetic grids |


The baseline DLLs have also been demonstrated with the graphical host. The host's action log identifies requested movements; **it does not itself confirm successful displacement**. BFS is **not** integrated into the host, and no FBE, FSM, reliable global map memory, automatic flag capture or quantified competition advantage has been demonstrated.

**Responsibilities and current Gantt status:**
- **Aiswarya — Software Engineering (WP-02, WP-03: In progress):** Owns movement-outcome/API verification and map-memory/position-confidence design. Her review of the latest automated tests is **not yet confirmed**.
- **Nirmal — Research (WP-06, WP-07: In progress):** Executed and recorded the tests; continues literature evidence, metrics and fair baseline protocol design. Preparing test fixtures is early groundwork for **WP-08 (Planned; scheduled to start 9 November)**, not completion of that work package.
- **Both — Coordination (WP-11: Done in the official Gantt):** Keep the project documents aligned and confirm any outstanding presentation/supervisor feedback records.


## Week 6 — Own ScoutAgent and additional-map testing (11 November 2026 status)

Following professor feedback, our **primary implementation and evaluation target is our own `ScoutAgent`** rather than a performance comparison with the professor's example `StupidAgent`. The earlier examples and 29 Week 5 checks remain as historical API/regression evidence.

- **Own agent:** `tests/Week02_HostandAgent/scout_agent.cpp` reads the 5 × 5 local observation, chooses obstacle-aware cardinal moves, uses tentative observation/action history to prefer local novelty, and applies local BFS toward a visible reachable enemy flag. It is **not** yet full global FBE or an FSM.
- **Additional maps:** six purpose-built development maps plus one unused holdout fixture in `tests/Week02_HostandAgent/week06_maps/`; these are inputs to the **offline simulator**, not custom maps loaded into the supplied `host.exe`.
- **Separate-environment tests (11 Nov):** 19/19 ScoutAgent unit checks, 8/8 synthetic movement-observation checks, 6/6 offline map fixtures and 5/5 native exported API symbol checks. Five navigable fixtures reached the flag cell and issued pickup requests; **neither actual movement nor flag capture in the Windows host is yet confirmed**.
- **Focused research:** read only the paper sections that inform exploration, online decision making and grid-based testing; record actual reading evidence and evaluate versions of our own agent across different maps.
- **Still required locally:** build/load `scout_agent.dll` in MSYS2 UCRT64, observe behavior in the original host, reproduce test logs and record dated individual contributions before claiming team sign-off or a completed WP.

See [Week 6 action plan](docs/week06-action-plan.md), [agent strategy](docs/agent-strategy.md), [test results](docs/week06-map-test-results.md), [completion report](docs/week06-completion-report.md), and [Windows-host checklist](docs/week06-host-checklist.md).

**Week 6 professor handoff:** [Evidence-to-feedback guide](docs/week06-professor-submission.md) · [One-slide presentation](presentations/Week06_Progress_One_Slide.pptx) · [Three-minute speaking notes](presentations/Week06_Three_Minute_Presentation_Notes.md) · [Professor Q&A](presentations/Week06_Professor_Questions_and_Answers.md).

## Documentation

- [Project description](docs/project-description.md)
- [Project requirements](docs/project-requirements.md)
- [Project plan](docs/project-plan.md)
- [Project schedule](docs/project-schedule.md)
- [Software architecture](docs/software-architecture.md)
- [Research plan and evaluation metrics](docs/research-plan.md)
- [Literature summary](docs/literature-summary.md)
- [Algorithm research](docs/algorithm-research.md)
- [Algorithm comparison](docs/algorithm-comparison.md)
- [Developer diary](devlog/developer-diary.md)

## Repository layout

```text
cpp-agent-project/
├── README.md
├── docs/
│   ├── project-description.md
│   ├── project-requirements.md
│   ├── project-plan.md
│   ├── project-schedule.md
│   ├── software-architecture.md
│   ├── research-plan.md
│   ├── literature-summary.md
│   └── algorithm-comparison.md
├── devlog/
│   └── developer-diary.md
└── tests/
    ├── AgentDLLTest/           # Week 1 DLL communication example
    └── Week02_HostandAgent/    # Week 2 graphical host and agent source
```

## Build and run (Windows / MSYS2 UCRT64)

Use an **MSYS2 UCRT64** terminal with a compatible MinGW `g++` installed. Run each example from the repository root. Week 1 and Week 2 use **different API headers**; do not mix them.

### Week 1 — Simple DLL communication test

```bash
cd tests/AgentDLLTest
g++ -std=c++17 -c simple_agent.cpp -o simple_agent.o
g++ -shared -o simple_agent.dll simple_agent.o
./host.exe simple_agent.dll
```

Expected demonstration: inputs `10`, `20`, `30`, `40` produce outputs `10`, `30`, `60`, `100`.

### Week 2 — Graphical Capture The Flag host

```bash
cd tests/Week02_HostandAgent
g++ -std=c++17 -c stupid_agent.cpp -o stupid_agent.o
g++ -shared -o stupid_agent.dll stupid_agent.o

g++ -std=c++17 -c safe_agent.cpp -o safe_agent.o
g++ -shared -o safe_agent.dll safe_agent.o

./host.exe safe_agent.dll
```

Use `./host.exe stupid_agent.dll` to test the reference agent instead. The host and DLL must have compatible Windows architectures. The provided host supports loading an opposing agent DLL as described in the supplied Week 2 instructions.
