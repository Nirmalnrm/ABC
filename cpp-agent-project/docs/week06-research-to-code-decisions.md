# Week 6 — Focused Research-to-Code Decisions

**Date:** 11 November 2026. **Research responsibility:** Nirmal (WP-06/07/08). This note summarizes *the existing project literature survey and current implementation*, **not** a claim that new paper sections were read personally or that the full papers were verified on this date.

## Keep only three papers in the immediate research scope

| Existing reference | Useful project concept | Concrete implementation/evaluation decision | Boundary |
|---|---|---|---|
| Yamauchi (1997), frontier exploration — R1 | Explore borders of known/unknown terrain | Keep persistent frontier logic as a later design stage; current ScoutAgent uses a local novelty heuristic instead | No spatially consistent full map, so this **does not implement FBE** |
| Korf (1990), real-time heuristic search — R3 | Choose an action while perception and planning repeat | One short decision in `Agent_Step` using the latest 5 × 5 view; local BFS only for a *visible* reachable flag | This **does not implement Korf's algorithm** |
| Sturtevant (2012), grid benchmarks — R5 | Evaluate on varied, documented layouts | Six fixed authored development maps, fixed step budgets and per-map metrics; preserve a separate holdout | These are our **own simulated layouts**, not Sturtevant's official benchmark and not host maps |

**Deferred:** A*, D* Lite, LPA* and broad new paper searches. Revisit only if a verified engineering constraint requires them.

## Testable Week 6 hypotheses (our agent only)

- **H1 / RQ1 — Observed safety:** In each authored development map, ScoutAgent will issue **zero** requests into a known wall or blocked neighbor. **Offline evidence:** 0 blocked requests in six fixtures. **Host outcome:** unverified.
- **H2 / RQ2 — Locally guided exploration:** Given fixed step budgets, ScoutAgent will visit at least the pre-registered fixture minimum number of cells *in the offline simulator*. **Offline evidence:** all six thresholds passed. This is **not** verified real-world coverage or an algorithm-superiority claim.
- **H3 / RQ3 — Visible-goal routing:** If the observed enemy flag is reachable within known local floor, ScoutAgent's BFS will choose a safe first cardinal step. **Unit evidence:** visible flag and wall-detour checks passed; the offline simulator also recorded flag-cell arrivals on five maps. **Flag acquisition:** not verified.
- **H4 / RQ4 — Motion ambiguity:** Consecutive terrain-only snapshots cannot always disambiguate a successful movement request from staying still. **Offline evidence:** eight synthetic movement-evidence checks, including ambiguous uniform terrain. **Windows host movement:** unverified.

## Data collection and research traceability

1. Only use `ScoutAgent` as the current experimental agent; the professor's `StupidAgent` is historical API material.
2. Record agent source SHA-256, compiler, map file, start position, fixed action budget and observed metrics. Preserve source and script history in Git.
3. Keep `reserved_holdout.map` out of development runs. Use it once after a frozen revision, not for iterative tuning.
4. Explicitly distinguish **requests** from confirmed host outcomes, and **simulator-observed coordinates** from agent-known coordinates.
5. After the team reviews the actual papers, append reading date, source section/page and the particular code/metric influenced. **No unverified reading or personal contribution is claimed here.**

**Evidence:** [map results](week06-map-test-results.md) · [test log](week06-test-log.txt) · [agent strategy](agent-strategy.md) · [research plan](research-plan.md).
