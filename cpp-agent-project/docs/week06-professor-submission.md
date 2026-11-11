# Week 6 — Professor-Ready Submission Guide

**Week:** 9–15 November 2026  
**Prepared/checkpoint:** 11 November 2026  
**Project:** Autonomous C++ Capture the Flag agent (two linked modules: Software Engineering and Research)  
**Status:** Offline prototype and documentation ready; Windows-host verification and team review still pending.  
**Source of the Week 6 scope:** five professor feedback points reported by the team. No separate official Week 6 assessment sheet was provided, so this is an evidence-based handoff, **not a claim of formal professor sign-off**.

## 1. What the professors asked for, and where the work is

| Professor feedback | Deliverable | Evidence / limit |
|---|---|---|
| Effective steps for project and research planning | `docs/week06-action-plan.md`, `docs/research-plan.md`, this guide | Assigned milestones and acceptance rules; planned personal tasks not automatically completed |
| Create your **own** agent, not a StupidAgent comparison | `tests/Week02_HostandAgent/scout_agent.cpp`, `scout_agent_test.cpp` | Independent C++ agent with exported API; 19 own-agent unit checks pass |
| Do only research directly needed | `docs/week06-research-to-code-decisions.md`, existing bibliography | Three practical links from literature to code; full-text personal reading must be recorded separately |
| Explain the agent's strategy | `docs/agent-strategy.md`, `docs/software-architecture.md` | Local safety + novelty bias + local BFS toward visible reachable enemy flag; global mapping/FBE/FSM not finished |
| Create other maps to test | `week06_maps/`, `map_simulator_test.cpp`, `docs/week06-map-metrics.csv`, `docs/week06-map-test-results.md` | Six authored offline development maps passed; seventh map reserved as untouched holdout; not host maps |

**Meeting presentation:** `presentations/Week06_Progress_One_Slide.pptx` (one slide, three minutes speaking + two minutes questions) and the associated speech and question bank. These follow the previously provided presentation format; no new weekly slide instructions were supplied.

## 2. What the self-developed ScoutAgent actually does

1. Receives the original professor's `WorldSnapshot.cells[5][5]` and uses the agent at `[2][2]`.
2. Excludes known walls, unknown cells and occupied cardinal neighbors from immediate movement requests.
3. If the enemy flag is **visible and reachable within that current local snapshot**, uses BFS to choose a shortest first step on known floor; if standing on it, **requests** pickup.
4. Otherwise scores safe moves with terrain-pattern choice history and a **tentative**, unverified displacement heuristic to discourage repetitive behavior.
5. If there is no safe known neighbor, requests `ACTION_WAIT`.
6. Exposes the five required API functions; reset clears agent-local counters.

**Crucial boundary:** This is **not** verified position tracking or persistent map exploration; it is not the full FBE + BFS + FSM competition architecture. Requesting pickup is not confirmed flag capture.

## 3. Reproduced offline checks (11 November 2026)

Command executed from project root: `bash tests/Week02_HostandAgent/run_week6_tests.sh`.

| Test group | Outcome | What it establishes |
|---|---|---|
| ScoutAgent unit tests | **19/19 pass** | Local API, safety, visible-goal BFS and reset policy on synthetic observations |
| Synthetic movement-evidence tests | **8/8 pass** | Recognizes ambiguous movement evidence; separate helper **not integrated** into ScoutAgent |
| Offline simulator development maps | **6/6 pass** | The scripted fixture safety/coverage checks on six authored maps |
| Native shared-object symbol checks | **5/5 pass** | C-linkage function symbols visible in the Linux library, **not** Windows DLL loading |

The six offline maps had **zero blocked action requests**. The simulator recorded arrival at the flag cell on all five maps that contain a reachable flag and some pickup requests. The simulator is not the real game; this proves neither successful Windows movement nor flag possession, return or a win.

**Log:** `docs/week06-test-log.txt` and a separately rerun log at `evidence/week06-local-verification-2026-11-11.txt`.  
**Map metrics:** `docs/week06-map-metrics.csv` and `docs/week06-map-test-results.md`.  
**SHA-256 of ScoutAgent source:** `512dd6e9d81a3fcbfc3913bb0078f077fa54e32d019bbda76fde953b23fbb37b`.

## 4. Work packages / team separation

The **approved** `docs/Gantt_Chart.xlsx` is copied **unchanged** from the user-supplied official version. The workbook's recorded statuses are not revised to claim completion from these prototype tests.

| WP | Official status | Owner | Week 6 evidence and what remains |
|---|---|---|---|
| WP-01 Project definition/requirements | Done | Aiswarya | Existing requirements and scope; confirm final review evidence separately |
| WP-02 API, movement outcome and pose checks | In progress | Aiswarya | Own-agent source and synthetic tests available; **actual Windows host movement and flag rules pending** |
| WP-03 Map memory and confidence | In progress | Aiswarya | Tentative history and standalone ambiguity helper; **trusted persistent mapping not built** |
| WP-06 Literature and hypotheses | In progress | Nirmal | Three focused research-to-code notes prepared; verify personally read sections |
| WP-07 Metrics and experimental design | In progress | Nirmal | Metrics and six-map offline records prepared; **controlled host measurements pending** |
| WP-08 Test harness and baseline runs | Planned in approved Gantt | Nirmal | Own-agent map simulator run is early preparatory work; do **not** claim full WP-08 done |
| WP-11 Schedule, review and briefing | Done | Both | Planning documents present; earlier meeting/feedback traceability should be confirmed |
| WP-04, 05, 09, 10, 12, 13, 14 | Planned | Owners per Gantt | Future FBE, integrated FSM/flag rules, comparative/robustness experiments, integration and final delivery |

**Attribution:** New source and offline tests were prepared in this assistance package. The files alone do **not** prove Aiswarya personally wrote or ran them or Nirmal personally reran them on Windows. Confirm and date individual contributions in the diary and Git commits.

## 5. Practical demo / what is still required

On the team's **Windows MSYS2 UCRT64** machine:

```bash
cd /c/Users/nirma/class/cpp/cpp-agent-project
bash tests/Week02_HostandAgent/run_week6_tests.sh
cd tests/Week02_HostandAgent
./host.exe scout_agent.dll
```

Record the compiler, Git revision, date, host start/load result, action request log, **observed actual displacement**, wall response and pickup feedback in `docs/week06-host-checklist.md`. Use the professor's unchanged original `host.exe` and `agent_api.h`. This is the main pending technical verification.

Afterward, capture Nirmal's own repeated runs and reading notes; have Aiswarya review host behavior and map-confidence design; have both review the diary and GitHub issues; commit and push the changed files. **Do not mark full WP-02/03/06/07/08 complete without the corresponding deliverables.**

## 6. Submission package structure

- `README.md` — overall project summary and Week 6 status.
- `devlog/developer-diary.md` — Week 1–6 chronology and individual responsibility boundaries.
- `docs/` — research plan, architecture, requirements, Gantt, Week 6 strategy/plan/evidence.
- `tests/Week02_HostandAgent/` — professor API/host (unaltered), prior reference agents, own ScoutAgent, C++ tests, simulator and authored maps.
- `presentations/` — one editable slide, 3-minute solo speaker notes and likely questions.
- `evidence/` — a fresh independent local reproduction of the offline test output.

**No extra paper survey, separate game host, or fabricated competition metrics are required for these five feedback items.**
