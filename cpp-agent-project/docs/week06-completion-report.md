# Week 6 Delivery Status — ScoutAgent and Practical Research

**Date of this report:** 11 November 2026  
**Week 6:** 9–15 November 2026  
**Official Gantt:** `docs/Gantt_Chart.xlsx` **not edited**; SHA-256 `92cdc219d962a9797c8d048825ed861124c930fd554e1827277d99dbc56b680c`.  
**Status:** **Offline deliverables prepared and verified; Windows-host validation and team sign-off remain open.**

## Professor feedback → delivered evidence

| Feedback | Delivered this week | Verification status |
|---|---|---|
| Effective project and research steps | Updated [action plan](week06-action-plan.md), with order, WP owners and checkable acceptance criteria | Document prepared; team prioritization review pending |
| Create our own agent, not a StupidAgent comparison | Self-developed `ScoutAgent` in `tests/Week02_HostandAgent/scout_agent.cpp`, using existing unmodified API | 19/19 unit checks, native 5/5 symbol checks; Windows host pending |
| Do not read unnecessary papers | Focused on Yamauchi (frontiers later), Korf (online decisions), Sturtevant (grid testing); A*/D* Lite deferred | Scope and practical uses documented; cannot certify team members read full papers |
| Clearly define agent strategy | Local safe cardinal decisions + observation-pattern action counts + tentative-history scoring + local BFS to visible flag | Implemented and unit-tested; flag acquisition unconfirmed |
| Test other maps | Six authored development maps plus one **unused** holdout map; repeatable offline simulator | All 6 fixtures passed under Linux; no custom map loading into host.exe claimed |

## Work packages and ownership (no Gantt changes)

| WP | Gantt standing | Lead | Source-supported Week 6 progress | Not yet completed |
|---|---|---|---|---|
| WP-02 — API/movement outcome/pose | In progress | **Aiswarya** | Own agent follows API; synthetic movement-observation diagnostic 8/8; native exports 5/5 | Windows DLL load, real displacement/failed-move and flag-rule validation |
| WP-03 — Map memory/confidence | In progress | **Aiswarya** | Architecture gate distinguishing *possible* movement/stay/ambiguity; no false map coordinate claims | Confirmed pose, persistent map, update/recovery tests |
| WP-06 — Literature/hypotheses | In progress | **Nirmal** | Focused three-paper priority, RQ1–RQ4 tied to actual agent behavior and uncertainty | Personal reading notes with pages/quotes, final hypotheses review |
| WP-07 — Metrics/experiment | In progress | **Nirmal** | Reproducible own-agent outcomes: visited simulation cells, blocked requests, waits, first flag-cell arrival, pickup requests | Team reproduction, future host-measured latency/movement/win metrics |
| WP-08 — Own-agent test harness/map runs | Planned in unchanged Gantt (scheduled from 9 Nov) | **Nirmal** | Early/initial work: six-map simulator, six runs, metrics table and untouched holdout | Confirm official WP start/status with team; later repeated/frozen-revision trials |
| WP-04 / WP-05 | Planned | **Aiswarya** | Early local-exploration heuristic and *local*, visible-goal BFS only | True persistent FBE, trusted global route planning, complete FSM/flag delivery |

**Prior Done status stays per your official Gantt:** WP-01 and WP-11; meeting feedback and personal contribution evidence still need confirmation. The work-package statuses are *reported*, not changed by this package.

## Independent execution evidence (separate Linux environment)

- ScoutAgent: **19/19** native unit checks passed.
- Movement-evidence experiment: **8/8** synthetic checks passed, with ambiguous observations explicitly represented.
- Authored development maps: **6/6** simulator fixture checks passed; zero blocked requests in all runs.
- Five navigable maps reached their flag cells and issued pickup requests; this is **not** actual flag capture.
- Linux shared library: **5/5** expected C-linkage symbols exported.
- Native tests also compiled with `-Werror`.
- **Agent source SHA-256:** `512dd6e9d81a3fcbfc3913bb0078f077fa54e32d019bbda76fde953b23fbb37b`.

Detailed metrics: [week06-map-test-results.md](week06-map-test-results.md) and `docs/week06-map-metrics.csv`. Complete captured run: [`docs/week06-test-log.txt`](week06-test-log.txt) in the repository.

## Team contribution record — no unverified attribution

- **Aiswarya (software lead):** assigned review, Windows DLL host integration, real movement/flag validation, pose/map confidence logic. These individual activities are **not confirmed completed** by the supplied evidence.
- **Nirmal (research lead):** assigned controlled map experiment reproduction, concise paper decision notes, metric interpretation, and Git-revision recording. Prior Week 5 run is confirmed by earlier screenshots; personal execution of the added 11 November runs is **not yet documented**.
- **Separate preparation/testing environment:** the Week 6 additions and the above Linux tests were generated and run for this project deliverable. They do not establish who in the two-person team implemented or executed them locally.

## What must still happen to call Week 6 fully team-verified

1. On Windows, execute `bash tests/Week02_HostandAgent/run_week6_tests.sh` using **MSYS2 UCRT64**; verify that `scout_agent.dll` builds and `host.exe` loads it.
2. Record real-host action/position evidence and test what `ACTION_PICK_UP_FLAG` actually does (avoid claims that the action request itself equals possession).
3. Have Nirmal run the same script on the project PC and record test revision/outputs. Preserve the holdout map for later frozen-revision testing.
4. Have Aiswarya review the agent and the movement-evidence design, then record dated contributor notes and any decisions.
5. Commit/push files to the WHZ GitHub repo after checking `git status`; do **not** replace or edit the approved Gantt.

**Do not mark the entire WP-02, WP-03, WP-06, WP-07 or WP-08 as Done from these offline results.** They have later dependencies and current Gantt assignments.
