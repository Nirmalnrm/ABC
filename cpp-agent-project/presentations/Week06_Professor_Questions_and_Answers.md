# Week 6 — Professor questions and short answers

**Evidence status:** Questions below distinguish code/tests in the submission from Windows-host behaviors that still need verification.

| Possible question | Accurate answer |
|---|---|
| What changed after our feedback? | We built our own `ScoutAgent`, documented its decision sequence, narrowed literature to three useful sources and ran it in an offline simulator on six additional maps. |
| Is this actually your own agent, or the professor's StupidAgent? | It is a separate C++ source with exported name `ScoutAgent`, a deterministic stateful local movement scorer, a visible-enemy-flag BFS and dedicated tests. The old example is retained only for API history. |
| What does `Agent_Step()` do? | It reads the `WorldSnapshot`, checks goal visibility and safe neighbors, then returns one action code to the host. |
| What are the five exported API methods? | `Agent_GetName`, `Agent_Create`, `Agent_Step`, `Agent_Reset` and `Agent_Destroy`. |
| How does the agent avoid walls? | Immediate movement candidates must be known floor and unoccupied in the 5×5 observation; unknown and wall cells are rejected. |
| Why do you use only four directions? | Cardinal moves simplify safe-neighbor checks and local BFS. This is a design choice, not proof diagonals are invalid. |
| Where is BFS used? | Only inside the current five-by-five snapshot when a reachable enemy flag is visible. It is not global BFS over a remembered map. |
| Is Frontier-Based Exploration implemented? | No. FBE needs a spatially consistent global map. Current ScoutAgent uses only an exploration scoring heuristic and tentative history. |
| Is FSM implemented? | No. The current priority rule is a small goal/explore policy, not a verified full objective/return/opponent FSM. |
| Does your map-memory work? | A reliable persistent global map is not implemented; tentative displacement history must not be treated as absolute coordinates. |
| What does `lastAction` mean? | It is the previous requested action, not confirmation that the host moved the agent. |
| Why have a movement-evidence test? | Two consecutive snapshots can support moving, staying or remain ambiguous. The helper has eight synthetic passing checks, but is not integrated into ScoutAgent or validated by the host. |
| How do the different maps work? | An offline C++ simulator loads our ASCII map files, gives the agent only a local snapshot per step, and measures movement requests and visited cells with its own map coordinates. |
| Did you load new maps into the professor's `host.exe`? | No. The supplied project does not establish a custom map-loading interface for the graphical host. The additional maps are for offline testing. |
| What are your quantitative results? | 19/19 ScoutAgent unit checks, 8/8 movement-observation checks, 6/6 map fixtures and 5/5 native C-linkage symbol checks, all passed offline. No real match-win result is claimed. |
| Why do some maps show pickup requests more than once? | The simulator counts action requests; the agent cannot yet confirm possession, so repeated requests are not evidence of captures. A cooldown limits immediate repeats. |
| Which papers are essential now? | Yamauchi (frontier exploration prerequisites), Korf (online decision-making context) and Sturtevant (reproducible grid evaluation). The research note does not claim a new full-text literature review. |
| How will you avoid overfitting the maps? | Tune only on six development maps, then freeze code and test a reserved unseen holdout once; record revision, compiler, map and action budget. |
| What work belongs to Aiswarya? | WP-02 API/host behavior, WP-03 reliable pose/map memory, and later agent software integration. Her personal validation is pending until recorded. |
| What work belongs to Nirmal? | WP-06 focused literature, WP-07 experimental protocol and metrics, WP-08 own-agent test runs and evidence collection. Local personal reproduction is pending until recorded. |
| Are any entire WPs newly completed? | No. The official Gantt still marks WP-01 and WP-11 Done, WP-02/03/06/07 In Progress, and others Planned. Tests are progress inside those WPs, not full WP completion. |
| What is the highest priority remaining? | Build/run the new DLL in the unchanged Windows graphical host; verify observed physical movement and flag mechanics; then trust or redesign position tracking. |
