# Week 6 Action Plan — Professor Feedback Applied

**Week:** 9–15 November 2026  
**Official schedule:** `Gantt_Chart.xlsx` is kept **unchanged**: 14 work packages, existing dates, hours and owners.  
**Project objective:** build **our own autonomous C++ DLL agent** and gather practical evidence from different maps, instead of focusing on the professor's example agent or expanding the paper survey.

## Feedback translated into deliverables

| Professor's instruction | Concrete Week 6 response | File / evidence | Next verification |
|---|---|---|---|
| Effective steps for project and research planning | Explicit task order, owner, measurable acceptance criteria | This file; `research-plan.md` | Agree priorities in team review |
| Create your own agent; do not compare with StupidAgent | New independent `ScoutAgent`, own API and decision tests | `tests/Week02_HostandAgent/scout_agent.cpp`, `scout_agent_test.cpp` | Aiswarya builds/loads Windows DLL |
| Only necessary research papers | Read only literature needed for current exploration, online decisions and map testing | `research-plan.md`, existing `literature-summary.md` | Nirmal records pages actually read |
| Agent strategy | Explicit priority: visible flag, local BFS, safe novelty decisions, fallback | `agent-strategy.md` | Confirm game rules; extend only when verified |
| Create another map to test | Six independent development fixtures + one reserved untouched holdout, offline simulator | `week06_maps/`, `map_simulator_test.cpp` | Collect dated Windows-host evidence separately |

## Week 6 concrete tasks, ownership and acceptance

| Day / order | WP | Lead | Action | Done when |
|---|---|---|---|---|
| Mon 9 Nov | WP-02 / WP-03 (early prototype) | **Aiswarya** | Review new ScoutAgent policy, verify five exported API methods and try the DLL in unmodified Windows host | Local MSYS2 build + recorded load/action logs; any failures described |
| Mon–Tue | WP-07 / WP-08 | **Nirmal** | Run the own-agent test script on all development fixtures; record compiler, source revision and outcomes | A reproducible run log and distinct-map measurements |
| Tue–Wed | WP-02 | **Aiswarya** | Use a controlled wall/corridor scenario to investigate successful/failed moves; validate pickup positioning and response | Observations demonstrating rule or explicit unresolved uncertainty |
| Wed | WP-06 | **Nirmal** | Extract only implementation-relevant insights from selected sources | 1–3 brief notes linked to an algorithm decision or measured variable |
| Thu | WP-03 | **Aiswarya** | Design confidence-aware mapping gate, without treating attempted movement as success | Documented trusted/untrusted pose rule + test case |
| Thu–Fri | WP-07 / WP-08 | **Nirmal** | Finalize within-agent metrics and organize untouched holdout policy | Result table, limits, repeatability conditions and future comparison design |
| Fri–Sun | WP-11 follow-up / WP-08 | **Both** | Review code, evidence, diary, GitHub commits and open questions | Dated review decision; diary records **actual** individual contributions |

**Important:** Assignments above are a Week 6 **plan**, not proof that either person already performed all listed activities. This update package's code and offline tests have been prepared and run in a separate environment; local Windows-host verification and team sign-off are still pending.

## Priority dependencies

1. **Own DLL + legal moves first.** Without a working self-developed agent, further paper comparisons have limited value.
2. **Different map fixtures second.** Use open/corridor/dead-end/blocked-start observations to reveal policy loops and unsafe requests.
3. **Movement and flag mechanics third.** `lastAction` is only a request; map position, possession and physical game success remain unverified.
4. **Trusted mapping before true FBE.** Tentative counts cannot substitute for confirmed global coordinates.
5. **Only then expand to full FSM/competition evaluation.** Do not mark WP-04/WP-05 done early merely because local BFS exists.

## Minimal research questions

- Can our agent reliably avoid known blocked cells while choosing legal moves?
- How much of each authored map does the simulator observe it visiting under fixed action budgets?
- Does short-route BFS guide it to a visible reachable flag cell?
- What prevents us from claiming that simulated movement or pickup was successful in the original host?

## Week 6 status boundary

- **Already created in this preparation package:** ScoutAgent source, standalone unit checks, offline map runner, six development maps, one untouched reserved map, initial documentation and test results.
- **Verified in a separate Linux environment on 11 Nov:** 19 ScoutAgent unit checks, 8 movement-evidence unit checks, 6 development-map fixtures, and 5 native shared-library symbol exports passed. Not a Windows `host.exe` verification.
- **Still to do on your Windows project:** build DLL in MSYS2 UCRT64, run professor host, record behavior, test movement/flag rules, obtain teammate review and commit to GitHub.

**Related:** [Strategy](agent-strategy.md) · [Research plan](research-plan.md) · [Map tests](week06-map-test-results.md) · [Schedule](project-schedule.md).

## Evidence update — 11 November 2026

- **WP-02/03 research support:** Added `movement_evidence.h` and eight synthetic tests that distinguish observations *consistent with movement* versus *consistent with staying*, or explicitly *ambiguous*. This is a standalone diagnostic experiment, not confirmed localization and not yet integrated into ScoutAgent's movement state.
- **WP-07/08:** Expanded the authored development scenarios to six distinct maps, including two-room navigation and zigzag barriers. The reserved holdout remains excluded from all scripted development runs.
- **Acceptance now exercised:** 19 ScoutAgent unit checks; 8 offline movement-evidence checks; six map fixtures checking minimum simulator coverage, zero blocked requests, and flag-cell arrival plus pickup requests on five maps containing flags; five C-linkage symbols in the Linux shared library.
- **Missing before team sign-off:** Original Windows-host DLL loading, successful physical movements and flag mechanics, Aiswarya's review, Nirmal's own local run and version-controlled submission. These must not be marked completed based on the offline environment alone.

See [the dated completion report](week06-completion-report.md) for exact results and responsibilities.

**Focused research deliverable:** [week06-research-to-code-decisions.md](week06-research-to-code-decisions.md) translates three relevant sources into four testable hypotheses without claiming new full-text reading.
