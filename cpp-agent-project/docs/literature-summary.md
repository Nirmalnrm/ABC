# Literature Summary — Navigation and Exploration Research

**Project:** C++ Agent Project — WHZ Capture The Flag  
**Module:** Autonomous Research Project (KFT11050), coordinated with Software Project (ELT10210)  
**Research lead:** Nirmal Mohan  
**Software/project-planning reviewer:** Aiswarya Payakkal Vinayan  
**Review status:** Initial annotated literature survey. The publication details and available publisher/author summaries were checked; an in-depth, page-by-page review of each paper remains a research task.

## 1. Scope and selection method

The research problem is autonomous movement in a grid environment when the agent sees only a 5 × 5 local snapshot. Our initial safe agent could reject nearby obstacles but appeared to revisit a limited region. We therefore selected literature on **unknown-environment exploration, search/pathfinding, online decisions, replanning, and experimental benchmarking**.

Selection criteria: (1) an identifiable archival academic publication, (2) a clearly described method or evaluation contribution, (3) relevance to at least one of our research questions, and (4) a verifiable publisher/author/DOI link. This document does not imply that any of these algorithms has been implemented in our DLL.

## 2. Paper summaries

### [R1] Yamauchi (1997) — A Frontier-Based Approach for Autonomous Exploration

**Reference:** Yamauchi, B. (1997). *A Frontier-Based Approach for Autonomous Exploration*. Proceedings of the IEEE International Symposium on Computational Intelligence in Robotics and Automation (CIRA '97), pp. 146–151.  
**DOI:** https://doi.org/10.1109/CIRA.1997.613851  
**Author paper:** https://www.robotfrontier.com/papers/cira97.pdf

- **Research problem:** How can an autonomous system explore an initially unknown environment without repeatedly remaining in familiar territory?
- **Key idea:** Treat the boundary between already known traversable space and unexplored space as a **frontier**, and choose frontier regions as exploration objectives. The original work describes mapping and frontier navigation in robotic environments.
- **Relevance to our project:** Our `SafeMovementAgent` checks only adjacent cells. A frontier-based policy could provide a reason to travel into unexplored regions rather than making locally safe but repetitive choices.
- **Important limitation:** The method presumes a maintained map with spatial consistency. The supplied `WorldSnapshot` gives a local view and `Agent_Create` gives dimensions, but no absolute position. A dependable position/motion estimate is therefore a prerequisite for a persistent frontier map.
- **Potential research activity:** Investigate a local 'unexplored-neighbor' heuristic first; consider global frontiers only after movement-success detection and localization are validated.

### [R2] Hart, Nilsson & Raphael (1968) — A Formal Basis for the Heuristic Determination of Minimum Cost Paths

**Reference:** Hart, P. E., Nilsson, N. J., & Raphael, B. (1968). *A Formal Basis for the Heuristic Determination of Minimum Cost Paths*. IEEE Transactions on Systems Science and Cybernetics, 4(2), 100–107.  
**DOI:** https://doi.org/10.1109/TSSC.1968.300136  
**Authors' manuscript:** https://ai.stanford.edu/~nilsson/OnlinePubs-Nils/PublishedPapers/astar.pdf

- **Research problem:** How can pathfinding use domain information to guide the search toward low-cost routes?
- **Key idea:** Incorporate heuristic estimates into graph search, providing the theoretical basis for what is commonly called **A\***.
- **Relevance to our project:** If the agent knows a destination and has a consistent graph of traversable cells, A* could select routes toward a flag or target. It can be compared with BFS on the same graph.
- **Important limitation:** A* does not reveal unknown parts of the world by itself. It also relies on clearly specified movement costs and an appropriate heuristic. For four-direction unit-cost movement, Manhattan distance is an appropriate lower bound; changing to diagonals requires reassessing costs and heuristics.
- **Potential research activity:** Implement a small, synthetic-grid comparison of BFS and A* after the movement graph and allowed directions are confirmed.


### [R3] Korf (1990) — Real-Time Heuristic Search

**Reference:** Korf, R. E. (1990). *Real-time heuristic search*. Artificial Intelligence, 42(2–3), 189–211.  
**DOI:** https://doi.org/10.1016/0004-3702(90)90054-4

- **Research problem:** How can an agent make decisions while planning and acting are interleaved, rather than completing a full long-range search before every action?
- **Key idea:** Real-time heuristic search uses bounded deliberation and interleaves search with execution; the paper includes Real-Time-A*.
- **Relevance to our project:** The host requests one action per `Agent_Step()` call. This makes incremental decisions and replanning after new observations a natural design choice.
- **Important limitation:** The paper's assumptions and guarantees do not automatically transfer to our partially observed game. A movement may fail, a target may be unknown, and the local snapshot alone is not a complete graph.
- **Potential research activity:** Compare a low-computation local policy with a local planning policy; track per-step decision latency and behavior on changing snapshots.

### [R4] Koenig & Likhachev (2002) — D* Lite

**Reference:** Koenig, S., & Likhachev, M. (2002). *D* Lite*. Proceedings of the Eighteenth National Conference on Artificial Intelligence (AAAI-02), pp. 476–483.  
**Institutional publication record:** https://publications.ri.cmu.edu/d-lite  
**Conference record:** https://ocs.aaai.org/Library/AAAI/2002/aaai02-072.php

- **Research problem:** How can a navigation system efficiently revise its route when information about terrain changes or is revealed?
- **Key idea:** Incremental heuristic search reuses previous computations to replan after changes in the known graph.
- **Relevance to our project:** The game exposes terrain incrementally as the agent moves; a replanning method could eventually make use of a remembered map.
- **Important limitation:** D* Lite is overpowered for a first 5 × 5 local experiment and needs a meaningful persistent map and localization. It is a **later research candidate**, not a Week 3 deliverable.
- **Potential research activity:** Use this paper to motivate the longer-term Gantt milestone 'incremental replanning' only after BFS and map tracking are validated.


### [R5] Sturtevant (2012) — Benchmarks for Grid-Based Pathfinding

**Reference:** Sturtevant, N. R. (2012). *Benchmarks for Grid-Based Pathfinding*. IEEE Transactions on Computational Intelligence and AI in Games, 4(2), 144–148.  
**DOI:** https://doi.org/10.1109/TCIAIG.2012.2197681  
**Benchmark repository:** https://www.movingai.com/benchmarks/grids.html

- **Research problem:** How can researchers compare grid-search methods when each study uses different maps and test problems?
- **Key idea:** Supply shared grid benchmarks and encourage systematic characterization and comparisons across scenarios.
- **Relevance to our project:** A single screenshot is not evidence that one algorithm is superior. Our experiments should use comparable layouts, map rules, run lengths and logging, and should report unsuccessful runs instead of hiding them.
- **Important limitation:** Benchmark maps may not exactly represent the professor's game mechanics; the game's moving adversary, pickup/drop-off rules, and local visibility introduce additional constraints.
- **Potential research activity:** Establish a set of small fixed snapshot fixtures for unit tests; run practical agent comparisons under matched game conditions whenever possible.

### [R6] Koenig, Likhachev & Furcy (2004) — Lifelong Planning A*

**Reference:** Koenig, S., Likhachev, M., & Furcy, D. (2004). *Lifelong Planning A**. Artificial Intelligence, 155(1–2), 93–146.  
**DOI:** https://doi.org/10.1016/j.artint.2003.12.001  
**Institutional record:** https://publications.ri.cmu.edu/lifelong-planning-a

- **Research problem:** How can repeated related shortest-path problems be solved without repeating all computation after small graph changes?
- **Key idea:** Lifelong Planning A* reuses parts of a previous search when the graph is updated.
- **Relevance to our project:** This provides research background for efficient navigation if the agent eventually maintains a stable global graph that gains new information over time.
- **Important limitation:** The current agent lacks reliable absolute position and a persistent explored graph. Implementing LPA* now would add complexity without resolving those prerequisites.
- **Potential research activity:** Retain as a longer-term comparison to D* Lite and conventional A*.

## 3. Comparative synthesis

| Paper | Best contribution to our research | Read/implementation priority |
|---|---|---|
| R1 — Yamauchi | Avoiding repeated exploration by seeking known/unknown boundaries | **High — concept for RQ2/RQ4** |
| R5 — Sturtevant | Fair scenario design and reproducible evaluation | **High — experiment design** |
| R3 — Korf | Online action selection with incremental planning | High — decision architecture |
| R2 — Hart et al. | Heuristic pathfinding theory for a later A* implementation | Medium |
| R4 — D* Lite | Replanning when map information changes | Later |
| R6 — LPA* | Reuse of previous search work | Later |

### 4. Implications for the proposed agent
The preliminary software design combines Frontier-Based Exploration (FBE) for identifying where to explore, BFS for routing through known traversable cells, a Finite-State Machine (FSM) for selecting game objectives, and safe local exploration as a fallback whenever global position or map confidence is insufficient.

The papers above contribute background on exploration, heuristic search, online decisions, incremental replanning, and experimental design. They do not establish that the FBE + BFS + FSM combination will outperform the existing agents or win matches. That is a hypothesis requiring controlled tests on unseen maps and against multiple opponents.

Outstanding literature gap: None of the six papers directly evaluates FSM-based objective control in this Capture The Flag setting. A targeted search for relevant autonomous/game-agent state-machine research is planned. No new FSM publication is claimed as reviewed here.

Suggested research sequence: Verify host movement and interaction rules; measure random and safe-movement baselines; validate local navigation on synthetic grids; test reliable mapping and frontiers; integrate FSM-controlled objectives; finally evaluate generalization with held-out maps. Store experiment setups, revisions, failures and results before making performance claims.


## 5. Focused Literature Reading and Evidence Gaps 

**Status:** Reading plan based on the initial annotated survey above. This section does **not** claim that new full-text reading or experimental results have occurred. Update the notes below with page/section references and the actual reading date after reviewing each source.

| Priority | Existing reference | Specific point to verify in the source | Project connection | Reading evidence |
|---|---|---|---|---|
| High | R1 — Yamauchi (1997) | Exact definition of a frontier, selection conditions and map/pose assumptions | FBE depends on a spatially consistent remembered map | Detailed reading pending |
| High | R3 — Korf (1990) | What real-time decision-making assumes about state, available actions and search effort | `Agent_Step` makes one action decision per call | Detailed reading pending |
| High | R5 — Sturtevant (2012) | How benchmark problems, map classes and comparable test conditions are documented | Repeatable baselines and held-out competition maps | Detailed reading pending |
| Medium | R2 — Hart, Nilsson & Raphael (1968) | Path cost and heuristic assumptions behind A* | Optional later comparison with BFS | Detailed reading pending |
| Research gap | FSM-based game/robot agent control | Find and verify a suitable archival FSM/control-architecture source | Guarded transitions for flags, recovery and opponent response | **Not yet sourced; do not fabricate a citation** |

**Notes for the research lead:** Record the exact publication passage reviewed, a short paraphrase of its claim, applicability to the 5 × 5 API, and one limitation. Keep bibliographic verification separate from full-text reading. No paper currently establishes that FBE + BFS + FSM will win this specific Capture The Flag competition.

**Working research hypothesis:** Exploration-target selection and guarded game-state control may improve objective discovery and reliability on previously unseen maps relative to purely local safe movement. This remains *untested* and should be evaluated only after individual components are implemented.
