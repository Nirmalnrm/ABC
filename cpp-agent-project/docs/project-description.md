# Project Description — C++ Capture The Flag Agent

**Modules:** Software Project (ELT10210) and Autonomous Research Project (KFT11050)  
**Team:** Aiswarya Payakkal Vinayan (Software Engineering) and Nirmal Mohan (Research and Evaluation)  
**Project Duration:** October 2026 – January 2027

## 1. Project Overview

This project aims to develop an autonomous **C++ Capture The Flag agent** capable of navigating an unknown game environment, exploring unexplored areas, identifying objectives, and competing against an opposing team's agent.

The agent will be implemented as a **Windows Dynamic Link Library (DLL)** and integrated with the graphical Capture The Flag host provided by the university.

The host provides the agent with a **5 × 5 local WorldSnapshot**, containing information about nearby terrain, obstacles, agents, flags, and targets. Based on this limited observation, the agent must select an appropriate action using the supplied API.

## 2. Problem Statement

Autonomous navigation in an unknown environment presents several challenges.

The agent has limited visibility, does not receive its absolute position directly, and may encounter walls, unexplored regions, dead ends, and opposing agents.

Simple random movement can lead to collisions with obstacles and inefficient exploration. Basic obstacle-aware movement improves local safety but may still cause repeated movements or prevent the agent from discovering important objectives.

Traditional pathfinding algorithms such as Breadth-First Search (BFS) and A* can find routes through known environments, but they do not independently solve the problem of exploring an unknown map.

The central problem is therefore to design an autonomous agent that can **explore unfamiliar environments, navigate efficiently toward discovered objectives, and adapt its decisions to changing game conditions**.

## 3. Project Objectives

The main objective is to develop and evaluate a reliable autonomous Capture The Flag agent using a combination of exploration, navigation, and decision-making techniques.

The specific objectives are:

1. Develop a C++ agent compatible with the supplied graphical host and DLL interface.
2. Implement safe movement decisions based on locally observed terrain and obstacles.
3. Investigate reliable movement tracking and map-memory techniques for partially observable environments.
4. Use Frontier-Based Exploration to discover previously unexplored areas when reliable mapping is available.
5. Apply BFS to calculate routes through known traversable cells toward exploration targets and game objectives.
6. Develop a Finite-State Machine to coordinate exploration, flag interactions, navigation, and responses to opposing agents.
7. Provide a safe local exploration strategy when map information or position estimates are insufficient.
8. Evaluate the agent's navigation performance, exploration efficiency, reliability, and competition behavior using reproducible experiments.

## 4. Proposed Algorithmic Approach

The project adopts a **hybrid autonomous navigation architecture** combining Frontier-Based Exploration (FBE), Breadth-First Search (BFS), and a Finite-State Machine (FSM).

### 4.1 Frontier-Based Exploration (FBE)

Frontier-Based Exploration is used to identify boundaries between known traversable areas and unexplored regions.

The agent can select reachable frontiers as exploration objectives to discover additional information about the environment.

Reliable position estimation and map representation are prerequisites for persistent frontier-based navigation.

### 4.2 Breadth-First Search (BFS)

Breadth-First Search is used for path planning through known traversable cells.

It can calculate shortest paths on unweighted graphs toward selected frontiers, observed objectives, or remembered destinations.

BFS supports navigation within the environment but does not independently determine where unknown objectives are located.

### 4.3 Finite-State Machine (FSM)

The Finite-State Machine coordinates the agent's behavior according to the current game situation.

The proposed behavioral states include:

- **Explore:** Search for unexplored areas and objectives.
- **Approach Flag:** Navigate toward a known enemy flag.
- **Pick Up Flag:** Perform a valid flag pickup action.
- **Return:** Navigate toward the required delivery location.
- **Deliver Flag:** Complete the flag delivery interaction.
- **Opponent Response:** React to visible opposing agents according to game rules.
- **Recovery:** Handle blocked movements, unavailable routes, or unreliable position information.

State transitions must be based on information available through the host interface and verified game behavior.

### 4.4 Safe Local Exploration Fallback

When reliable map information, localization, or route planning is unavailable, the agent uses an obstacle-aware local movement strategy.

This fallback allows the agent to continue making decisions without depending on an uncertain global map. It may also select `ACTION_WAIT` when no suitable movement is available.


## 5. Software Engineering Component

The Software Engineering component focuses on designing, implementing, integrating, and testing the C++ agent.

The software architecture separates the host API interface, snapshot processing, movement and position tracking, map representation, exploration strategy, pathfinding, and behavioral control.

This modular structure supports incremental development and allows individual components to be tested independently.

## 6. Research Component

The Autonomous Research Project investigates the effectiveness of exploration and navigation strategies in unknown Capture The Flag environments.

The research focuses on:

- Autonomous exploration under partial observation.
- Comparison of random, obstacle-aware, and frontier-based exploration methods.
- Effectiveness of BFS for navigating toward known objectives.
- Contribution of FSM-based behavioral control to game performance.
- Robustness of navigation strategies across different previously unseen maps and opposing agents.
- Evaluation of movement safety, exploration efficiency, computational performance, and competition outcomes.

The research follows a structured methodology involving literature review, hypothesis formulation, experimental design, controlled testing, performance measurement, and analysis.

Experimental conclusions will be based on collected evidence rather than assumed algorithm performance.

## 7. Expected Outcomes

The project aims to produce:

1. A functional C++ Capture The Flag agent DLL compatible with the supplied host.
2. An integrated exploration and navigation strategy using FBE, BFS, and FSM-based decision-making.
3. A reliable safe-movement fallback for uncertain situations.
4. A documented and maintainable software architecture.
5. Reproducible experiments comparing the implemented navigation strategies.
6. An evaluation of the agent's performance on unfamiliar maps and against opposing agents.
7. Technical documentation and a research report presenting the methodology, experimental findings, limitations, and conclusions.

The overall goal is to develop an autonomous agent that can **adapt to unknown environments, explore systematically, navigate toward game objectives, and make informed decisions during competition**, while supporting its design through software engineering practices and scientific evaluation.
