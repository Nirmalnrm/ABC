# Algorithm Comparison — C++ Capture The Flag Agent

**Authors:** Nirmal Mohan (research lead); Aiswarya Payakkal Vinayan (technical reviewer)  
**Status:** Historical design comparison based on source inspection and literature.

**Week 6 scope note:** The professor requested **our own agent rather than another `StupidAgent` comparison**. This document remains useful as background and API history; practical Week 6 implementation and evaluation now center on `scout_agent.cpp`, authored map fixtures, and within-agent improvements. See `agent-strategy.md` and `week06-map-test-results.md`. The Gantt is unchanged.
## 1. Game and API assumptions

The `agent_api.h` defines a `WorldSnapshot` with `cells[5][5]` indexed as `[y][x]`, with the agent at the center `[2][2]`. Each `uint32_t` cell combines terrain (`CELLTYPE_BITMASK = 0x00FF`) and contents (`CELLCONTENT_BITMASK = 0xFF00`). The agent selects from eight compass moves, wait, pick-up, or put-down actions. Five DLL exports handle agent creation, state, reset, step decisions, and cleanup.

**Crucial limitations:** `Agent_Create(mapWidth, mapHeight)` tells us the dimensions but does not give global coordinates. `lastAction` is an input to `Agent_Step`, but the supplied header does not explicitly guarantee movement success. A map-wide algorithm therefore requires additional evidence or validated state estimation.

## 2. Candidate algorithms

| Algorithm | Decision method | Needs a map? | Handles known walls? | Exploration / goal handling | Current project status |
|---|---|---|---|---|---|
| **A. StupidAgent** | `ACTION_MOVE_NORTH + (rand() % 8)`: random direction across eight action codes | No | No | No deliberate goal pursuit | **Professor's example tested in host** |
| **B. SafeMovementAgent** | Evaluate four cardinal neighbors; choose an eligible known floor cell | 5 × 5 local view | Yes, by filtering | Can revisit the same area; no visit memory | **Custom baseline tested in host** |
| **C. SafeMovementAgentV2** | Random choice among known traversable cardinal neighbors; prefer not to reverse `lastAction` | 5 × 5 local view | Yes, by filtering | Reduces candidate immediate reversals, but not longer loops | **Source prepared; controlled game test not confirmed** |
| **D. Greedy visible-goal** | Select eligible move that decreases heuristic distance to a *visible* goal | Local view + detected goal | If combined with filtering | May get stuck near obstacles | Planned |
| **E. Local BFS** | Search shortest route across known, unweighted traversable cells | Local grid + visible target | Yes on graph | Shortest path within the observed grid if one exists | Planned |
| **F. A*** | Search graph with path cost + heuristic | Known graph and target | Yes on graph | Efficient directed search possible, no automatic exploration | Literature candidate |
| **G. Frontier exploration** | Seek boundary between explored and unknown space | Persistent localized map | With known map | Designed for systematic exploration | Research candidate; prerequisites missing |
| **H. D* Lite / LPA*** | Incremental shortest-path replanning after graph changes | Persistent graph and tracked pose | Yes on graph | Efficient recomputation as new map information appears | Longer-term only |

## 3. How the implemented baselines actually differ

### A. StupidAgent

```cpp
uint32_t action = ACTION_MOVE_NORTH + (rand() % 8);
```

The eight movement action constants are consecutive (`0x11` through `0x18`). This agent ignores the `snapshot`, may request an obstructed move, and has no objective awareness. Its usefulness is as a working DLL/API baseline, **not** as a competitive navigation method.

### B. Initial SafeMovementAgent

Our first variant checks whether adjacent cardinal cells are floor (`(cell & CELLTYPE_BITMASK) == CELLTYPE_FLOOR`) and unoccupied (`(cell & (CELLCONTENT_SELF | CELLCONTENT_ENEMY)) == 0`). It returns a move deemed eligible or waits when no cardinal move is eligible. The graphical host loaded it; during observation it often remained in a limited area. This observation requires systematic measurement before causal conclusions are drawn.

### C. Improved SafeMovementAgent

The prepared source (`safe_agent.cpp`) builds two candidate lists: **valid** safe cardinal moves and **preferred** moves that are valid and not the opposite of `lastAction`. It selects pseudorandomly from preferred moves, or valid moves when necessary, otherwise `ACTION_WAIT`.

This filters known walls and discourages immediate reverse *requests*, but it **does not** store visited cells or confirm that the last move succeeded. There is no evidence yet that V2 explores more of the map than V1.

## 4. Algorithmic trade-offs

| Method | Why it may help | Why it may fail | First useful test |
|---|---|---|---|
| Random (8-way) | Variety of directions with almost no computation | Repeated invalid moves; no purpose | Log requests into visible walls |
| Safe (4-way) | Straightforward obstacle filtering | Loops, misses useful diagonal moves | Synthetic corridor/dead-end snapshots |
| Safe V2 (4-way) | Diversifies eligible movements and reduces reversals | Can still loop; last action may have failed | Rate of opposite-direction requests |
| Greedy | Approaches a visible flag cheaply | Dead ends and obstacle detours | Visible goal behind a wall |
| BFS | Shortest path on a **known unit-cost graph** | No route to unseen goal; no full map | 5 × 5 synthetic fixtures |
| A* | Heuristic direction toward goal | Requires appropriate cost/heuristic; incomplete map | Compare node expansions with BFS on same fixtures |
| Frontier exploration | Seeks newly observable regions | Requires spatially consistent memory | Controlled map + verified localization |
| Incremental planners | Reuse search results after changes | Too complex before state tracking | Defer until map tracking works |

**Fairness warning:** Comparing the original eight-direction agent with four-direction safe agents confounds *number of available directions* with obstacle filtering. For a clean obstacle-avoidance test, optionally build a four-direction random-control agent and hold other conditions constant.

## 5. Complexity and movement assumptions

- **Local filtering:** O(1) for four neighbors; essentially constant computational work per step.
- **BFS:** O(V + E) in a known unweighted graph, with at most **V = 25 observed cells** for the local snapshot.
- **A*:** Complexity depends on graph, heuristic and priority-queue implementation; avoid claiming it always beats BFS on a tiny grid.
- **Four-direction grid with unit-step cost:** Manhattan distance `abs(dx) + abs(dy)` can be an admissible heuristic.
- **Eight-direction movement:** The professor's API includes diagonals but does not explain diagonal corner rules or costs. Define those before choosing graph edges and a heuristic. If every compass move costs one turn and unrestricted diagonals are legal, a Chebyshev-distance lower bound may be appropriate.
## 6. Next prototypes: safe exploration, frontier selection and local BFS

**BFS remains a route-planning component, not the whole competition policy.** In the final contest, the map and enemy flag location are unknown in advance, so the agent must choose where to explore until a useful goal becomes visible or reliably remembered.
    First validate the API and movement outcomes; then prototype local novelty-based
exploration with safe filtering; attempt persistent frontier targets when mapping/pose tracking is reliable; use local BFS for a **visible, reachable** objective or mapped route; finally add verified flag handling and opponent-aware reactions.

```text
Observe local 5×5 snapshot
If flag interaction is known and legal: choose interaction action
Else if a reachable known objective exists: return first safe BFS route step
Else: explore a safe novel/frontier target where confidence permits
If observations conflict with pose/map estimate: clear unsafe plan and fall back safely
```

**BFS synthetic tests:** known open corridor, known wall detour, blocked goal, unknown neighboring cell, only one legal cardinal direction, and occupied cell. These are planned tests, not results.

## 7. Recommended decision and development order

**preliminary research decision (not implemented):** Use a **three-part hybrid — Frontier-Based Exploration + BFS + Finite-State Machine — with safe local exploration fallback**. FBE chooses *where to explore*, BFS chooses *how to travel through known traversable cells*, and the FSM chooses *which game objective to pursue* under verified observations.

1. Preserve source revisions for original random and safe-movement baseline DLLs.
2. Test the meaning of `lastAction`, observed movement, legal diagonals and interactions.
3. Implement a safe local novelty heuristic that does not depend on absolute coordinates.
4. Prototype reliable movement tracking; only then build a consistent persistent map and frontier selection.
5. Implement BFS paths through known traversable grid cells; handle unreachable/unknown objectives via exploration/fallback.
6. Build/test an FSM (`EXPLORE`, `APPROACH_FLAG`, `PICKUP`, `RETURN`, `DELIVER`, `EVADE`, `RECOVER`) using only documented observations; study visible-opponent response after verifying host rules.
7. Compare safe, FBE, FBE+BFS and FBE+BFS+FSM ablations on diverse development maps and **held-out unseen maps** under repeatable opponent conditions.

## References

- [R1] Yamauchi (1997), https://doi.org/10.1109/CIRA.1997.613851
- [R2] Hart, Nilsson, Raphael (1968), https://doi.org/10.1109/TSSC.1968.300136
- [R3] Korf (1990), https://doi.org/10.1016/0004-3702(90)90054-4
- [R4] Koenig & Likhachev (2002), https://publications.ri.cmu.edu/d-lite
- [R5] Sturtevant (2012), https://doi.org/10.1109/TCIAIG.2012.2197681
- [R6] Koenig, Likhachev, Furcy (2004), https://doi.org/10.1016/j.artint.2003.12.001