# Developer Diary

## Project: Software + Research Project(ELT10210/KFT11050)

**Team Members :-** Aiswarya Payakkal Vinayan , Nirmal Mohan

---

## Week 1 :-

### Task:— Compile and test AgentDLLTest

#### Work performed:

- Compiled `simple_agent.cpp` using the MinGW g++ compiler.
- Generated `simple_agent.dll` from `simple_agent.o`.
- Executed the provided host application using `.\host.exe simple_agent.dll`.
- Verified communication between the host and the agent DLL.

#### Test results:

| Input | Output |
|---|---|
| 10 | 10 |
| 20 | 30 |
| 30 | 60 |
| 40 | 100 |

**Outcome:** The host successfully loaded the agent DLL and all four test calls produced the expected cumulative sums.

---

## Week 2

### Task 1:- Graphical Host and Agent Integration

- Download, compile, link, and test the new graphical host and the provided agent.

#### Activities

We examined Week_02_HostAndAgent.zip, which contains:

- host.exe — graphical Capture The Flag host application.
- agent_api.h — interface between the host and the agent.
- stupid_agent.cpp — example agent implementation.
- readme.txt — compilation and testing instructions.

The new host differs from Week 1 because it provides a graphical game environment in which agents move, encounter obstacles, and can interact with flags.

We used the MSYS2 UCRT64 terminal to compile the example C++ agent into a DLL.

#### Compilation Commands

```bash
g++ -c stupid_agent.cpp -o stupid_agent.o
g++ -shared -o stupid_agent.dll stupid_agent.o
./host.exe stupid_agent.dll
```

#### Observations:-

- The graphical application started, and the original StupidAgent was observed moving through different areas of the game map.
- We subsequently loaded our custom SafeMovementAgent into the graphical host. The host recognized the agent and continued processing its actions.

#### Result:-

**Status:** Successfully demonstrated.

The graphical host-agent interaction works. A completed capture-the-flag game or successful flag capture has not yet been demonstrated.

---

### Task 2 :- API Understanding and Initial Agent Development

- Understand the technical details of the API and develop first agent functionality or helper functions.

#### API Analysis

We studied the following functions:

| Function | Purpose |
|---|---|
| Agent_GetName() | Returns the agent name |
| Agent_Create() | Initializes the agent instance |
| Agent_Reset() | Resets internal state |
| Agent_Step() | Processes an observation and selects an action |
| Agent_Destroy() | Releases allocated memory |

- The host passes the agent a WorldSnapshot containing a 5 × 5 local observation.
- Each cell contains encoded information about terrain and objects. We studied the use of bitmasks to identify floor cells, walls, agents, flags, and targets.

#### Initial SafeMovementAgent

- We created safe_agent.cpp (Used cp command to copy the code exactly form stupid_agent.cpp)
- Unlike StupidAgent, which randomly selects from eight movement directions, SafeMovementAgent checks the four cardinal neighboring cells.

**The initial algorithm:**

- Reads the local snapshot.
- Checks whether neighboring cells are traversable floor.
- Checks whether they contain agents.
- Selects a suitable movement.
- Returns ACTION_WAIT if no suitable cardinal movement is available.

#### Test Results:-

- The custom agent was compiled and loaded into the graphical host.
- The host recognized SafeMovementAgent and executed the simulation.
- However, we observed that the agent frequently moved within the same region instead of exploring larger areas.

#### Problem Analysis :-

- The algorithm uses local movement information but does not remember previously visited locations.
- Consequently, it can repeatedly return to nearby cells.

---

### Task 3:- Start developing the functionality

- Start to develop safe_agent.cpp

**-This version:**

- Checks neighboring cells.
- Rejects known walls and occupied cells.
- Selects randomly among suitable movement options.
- Discourages immediately reversing the previous movement.
- Allows reverse movement when necessary.

**Status:** The improved source is prepared; its successful graphical test still needs to be confirmed.

#### Planned Improvements:-

We plan to investigate movement history, explored-map memory, goal-directed movement, and BFS-based navigation.

---

## Team Contributions:-

### Aiswarya Payyakal Vinayan :-

Demonstrated activities include the Windows DLL tests, graphical host testing.

### Nirmal Mohan :-

Initial SafeMovementAgent development, and investigation of repetitive movement.

### Joint work:

API Analysis, Update Developer Diary, code review, algorithm discussion, Testing, GitHub integration.

## Week 3

## Task 1 — Project Management and Definition

**Work Performed:**
- Created the project description on GitHub.
- Created GitHub Issues to organize project activities.
- Defined project objectives, scope, requirements, and expected outcomes.
- Assigned responsibilities and reviewers for the GitHub Issues.

**Outcome:** Initial project organization and task management established.

## Task 2 — Team Roles and Project Schedule

**Work Performed:**
- Divided responsibilities between Software Engineering and Research.
- Prepared work packages, milestones, and estimated effort.
- Prepared an initial Gantt chart covering both modules.
- Identified parallel activities, dependencies, and time for testing.

**Outcome:** Initial project schedule prepared for team review and presentation.

## Task 3 — Research Planning and Literature Review

**Work Performed:**
- Prepared `research-plan.md` and `literature-summary.md`.
- Identified relevant research papers about autonomous exploration and pathfinding.
- Investigated BFS, A*, random movement, and frontier-based exploration.

**Outcome:** Initial research plan and literature summaries prepared.

## Task 4 — Algorithm Comparison

**Work Performed:**
- Prepared `algorithm-comparison.md`.
- Compared StupidAgent, SafeMovementAgent, BFS, A*, and exploration strategies.

**Outcome:** Algorithm comparison prepared. Experimental evaluation is planned for later development.

## Task 5 — Documentation and Presentation Preparation

**Work Performed:**
- Prepared documentation for the GitHub repository.
- Connected the Software Engineering and Research activities in the project plan.
- Prepared a five-minute presentation with a Gantt chart.
- Divided the presentation topics between both team members.

**Outcome:** Presentation materials prepared for the October 23 meeting.

## Team Contributions

**Aiswarya Payakkal Vinayan:**
- Software Engineering planning.
- Project schedule and Gantt chart preparation.
- Work packages and project management.

**Nirmal Mohan:**
- Research planning and literature review.
- Algorithm comparison.
- GitHub Issue organization.

**Joint Work:**
- Project discussions and documentation.
- GitHub management and reviews.
- Presentation preparation.

## Week 4 — Research Findings, Preliminary Strategy and Project Review

### Task 1 — Literature research and initial findings

The research review identified relevant methods for an agent that must compete on a map it has not seen before. The available paper summaries provide the following preliminary, literature-informed directions:

- **Yamauchi (1997), frontier-based exploration:** Seeking boundaries between known traversable cells and unknown regions can guide exploration. Persistent frontiers require a spatially consistent map and a trustworthy estimate of the agent's movement/position.
- **Korf (1990), real-time heuristic search:** Planning and acting can be interleaved, which is relevant because the host requests one action on each `Agent_Step()` call. The paper does not establish performance for this particular game.
- **Sturtevant (2012), grid pathfinding benchmarks:** Comparable maps, scenarios and repeatable measurements are important for evaluating navigation methods fairly.
- **Hart, Nilsson and Raphael (1968), A*:** Heuristic search can assist route planning on a known graph, but does not itself explore an unknown map. A* is optional rather than part of the required initial strategy.

**Research status:** The literature summary is an initial annotated review based on identified papers and available publication information. In-depth reading, direct FSM/game-agent literature and experimental validation remain to be completed. The papers motivate candidate methods; they do not prove that the proposed agent will outperform another team.

### Task 2 — Preliminary algorithm selection

**Proposed approach:** **Frontier-Based Exploration (FBE) + Breadth-First Search (BFS) + Finite-State Machine (FSM)**, with **safe local exploration as the fallback** and **A* as an optional later comparison**.

| Component | Intended responsibility | Prerequisite or limitation |
|---|---|---|
| FBE | Choose which boundary between known and unknown space to explore next | Requires reliable map alignment and movement/position estimates |
| BFS | Find routes to known reachable frontiers, flags or targets through known traversable cells | Cannot locate a hidden objective on its own; movement costs/rules must be verified |
| FSM | Select behavior such as explore, approach flag, pickup, return, deliver, opponent response or recovery | Transitions must use game events and observations that the API actually supports |
| Safe local fallback | Continue choosing obstacle-aware local actions, or `ACTION_WAIT`, when global state is unreliable | May still revisit areas; exploration effectiveness remains untested |

**Why this approach:** The map is unknown before competition, so a route planner alone cannot determine where to search. FBE addresses exploration, BFS handles known-route navigation, and FSM coordinates game objectives. This is a **design proposal**, not a completed or winning algorithm.

**Assumptions to test:** Whether attempted movements succeed; whether observations permit reliable position tracking; exact diagonal and flag-interaction rules; usefulness of opponent information; and whether exploration and navigation improvements generalize across unseen maps.

### Task 3 — Project definition and schedule review

The project documentation was revised around an unknown-map competition and a combined software-and-research workflow:

- **Requirements and architecture:** Retain the professor-supplied host and five-function DLL interface; prioritize verified movement behavior, safe fallback, exploration, known-route BFS and conditional FSM actions.
- **Project plan and schedule:** Maintain **14 work packages and 300 estimated person-hours** (110 software engineering, 114 research, 76 joint). The equal joint allocation gives Aiswarya **148 planned hours** and Nirmal **152 planned hours**.
- **Schedule checkpoint:** The Mermaid Gantt chart in `docs/project-schedule.md` includes the **30 October 2026 research-progress review**. Any separate Excel workbook must be checked against the same work-package names, dates and hours.
- **Open questions:** Detecting unsuccessful moves, estimating position, diagonal movement/corner rules, flag pickup/delivery feedback, opponent behavior, observable match outcomes and the official final deadline.

The revised schedule is a **plan**, not a record of hours completed. Future development tasks depend on validating the API behavior; there is no claim that FBE, BFS or FSM has been implemented.

### Task 4 — Software progress 

**Verified prior software progress:** The graphical host loaded the initial `SafeMovementAgent`. The source selects among known traversable cardinal neighbors, avoids known occupied cells and includes a preference against immediate reverse requests. Repeated local movement was observed in earlier tests.

**Not yet demonstrated:** Reliable absolute localization, persistent map/frontier exploration, integrated BFS, FSM control, automatic flag capture, opponent-aware strategy and quantitative match comparisons. A prior local syntax check, if performed, is not by itself evidence of successful Windows-host integration of a newer agent revision.

### Individual contributions

| Team member | Responsibility for this work | Evidence to record after confirmation |
|---|---|---|
| **Aiswarya Payakkal Vinayan** | Software/API feasibility, architecture, requirements, Gantt and development-plan review | Dated edits or commits, tests actually run, implementation issues and review decisions |
| **Nirmal Mohan** | Literature synthesis, research questions, preliminary algorithm comparison and experimental design | Dated paper-reading notes, research edits/commits, test definitions and decisions |
| **Both** | Agreement on strategy, repository coordination and progress presentation | GitHub Issues/commits, presentation contribution, meeting notes and supervisor feedback |

**Conclusion:** We have documented a competition-oriented preliminary architecture and a corresponding research path. Algorithm effectiveness and competition outcomes remain to be established by implementation and controlled experiments.


## Week 5 — Automated Software Testing and Research Preparation

### Work completed — Test execution and evidence

Nirmal executed `bash run_agent_tests.sh` using the testing files under `tests/Week02_HostandAgent/` and captured the terminal output. The script compiled and tested the existing agents and the standalone known-grid BFS prototype.

| Component | Automated checks | Result | What was checked |
|---|---:|---|---|
| SafeMovementAgent | 15/15 | Passed | API/lifecycle, known traversable cardinal neighbors, occupied cells, wait fallback and avoidance of immediate reversal when alternatives exist |
| StupidAgent | 9/9 | Passed | API/lifecycle and pseudorandom legal eight-direction action requests |
| Standalone BFS | 5/5 | Passed | Known-grid shortest routes, wall detour, no path, unknown cells blocked, and start equals goal |
| **Total** | **29/29** | **Passed** | Functional/synthetic tests, not competition performance |

Both original agent DLLs were built successfully in the test run. The `stupid_agent.cpp` unused-`snapshot` warning is non-fatal and consistent with a random policy that does not use observations. Earlier graphical-host runs of both baseline agents are documented separately.

**Limits of the evidence:** A passing unit test does not establish that a requested action physically succeeded inside `host.exe`. The standalone BFS test is not a BFS-enabled game DLL. Global pose tracking, persistent map memory, FBE, FSM, flag delivery and match win-rate measurements remain unverified.

### Individual work separation

**Nirmal Mohan — Research and Evaluation (WP-06, WP-07)**

- **Completed:** Ran the automated C++ checks in MSYS2 UCRT64; captured and reviewed the 15 + 9 + 5 results; retained the two baseline agent variants for future comparison.
- **In progress:** Literature findings and hypothesis refinement (WP-06); operational metric definitions and controlled baseline protocol (WP-07).


**Aiswarya Payakkal Vinayan — Software Engineering**

- **Assigned and documented:** Owns host/API movement-outcome verification, inspection of `safe_agent.cpp`, and design of position-confidence and map-memory mechanisms.
- **Current evidence:** `docs/software-architecture.md` records proposed testing and map-memory design. The current baseline agents and API are present in source.

**Joint activities**

- WP-11 is marked **Done** in the team's official Gantt. Project scheduling and review materials are documented; meeting delivery and professor feedback evidence should be confirmed separately.
- Review the test outputs and open issues together; update the GitHub work-package statuses only after the responsible owner confirms them.
- Maintain separate traceability for who wrote a test, who executed it, who reviewed it and who approved it.



## Week 6 — Own Agent Strategy and Practical Map Tests (9–15 November 2026)

**Progress recorded on:** 11 November 2026. **Professor feedback:** plan effective software/research steps; develop our own agent instead of centering the comparison on `StupidAgent`; restrict reading to relevant papers; explain the agent strategy; test with more map layouts.

### Prepared implementation and evidence

- A separate `ScoutAgent` C++ implementation and its own unit tests were added to `tests/Week02_HostandAgent/`; the earlier example and safe-agent code remain unchanged.
- The strategy prioritizes safe local moves, modest observation-based exploration, and local BFS for a visible reachable enemy flag. Global map localization, persistent frontiers, full FSM and confirmed flag acquisition remain open.
- Six authored development-map fixtures, one unused holdout map and an offline C++ simulator were added; these maps are **not** known to load into the original graphical host.
- **Independent separate-environment check (11 Nov):** 19/19 own-agent unit checks passed; 8/8 synthetic movement-evidence checks passed; 6/6 development-map fixtures passed; 5/5 native C-linkage exports found. Test logs and map measurements are saved in `docs/week06-test-log.txt` and `docs/week06-map-test-results.md`.
- These external preparation/test results must **not** be presented as a test run performed personally by a team member. Windows-host load and movement/flag behavior are unverified until run locally.

### Individual responsibility by work package

| Team member | WP | Week 6 responsibility / verified status |
|---|---|---|
| **Aiswarya Payakkal Vinayan** | WP-02 | Review ScoutAgent and compile/load the DLL with the original Windows host; document actual movement, blocked attempts, pickup rules and observed errors. **Local test/sign-off pending.** |
| **Aiswarya Payakkal Vinayan** | WP-03 | Review the movement-observation ambiguity tests and refine confidence-aware map-memory design; persistent map memory is not implemented. |
| **Nirmal Mohan** | WP-06 | Record concise reading notes for the three implementation-relevant papers; confirm which sections were actually read. **Personal reading evidence pending.** |
| **Nirmal Mohan** | WP-07 | Define own-agent measurements from several maps, document simulator limits and distinguish pickup request from successful capture. |
| **Nirmal Mohan** | WP-08 | Re-run the Week 6 test script locally, capture environment/commit/outputs and keep the holdout map unused during tuning. **Local run not yet confirmed.** |
| **Both** | Coordination | Review changed code, commit/push files and record actual personal contributions and supervisor feedback before any WP is signed off. |

**Unchanged Gantt:** WP-01 and WP-11 are marked Done; WP-02, WP-03, WP-06 and WP-07 In progress; the remaining WPs are Planned in the team's approved chart. These entries do not update the workbook or claim that full WPs are newly complete.

**Next concrete checks:** `bash tests/Week02_HostandAgent/run_week6_tests.sh`, then `./host.exe scout_agent.dll` from the Week 2 test directory on Windows; record results in `docs/week06-host-checklist.md`.
