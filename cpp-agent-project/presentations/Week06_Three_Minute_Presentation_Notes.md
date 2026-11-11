# Week 6 — Solo three-minute presentation script

**Meeting format:** One slide; **three minutes speaking**, then **two minutes questions**.  
**Presenter:** Nirmal (presenting the whole team's documented status; Aiswarya's attendance may vary).  
**Week:** 9–15 November 2026. Results checkpoint 11 November.

Good morning, Professor.

I will explain our Week 6 progress in Software Engineering and Research. Following your feedback, our priority is no longer to compare the example StupidAgent. Instead, we have focused on developing **our own C++ agent**, explaining its strategy, and testing it on different maps.

**First, the work completed.** Our project now contains a separate **ScoutAgent** implementation using the professor's existing five-function DLL interface. It reads the local five-by-five snapshot and chooses safe cardinal movements. When it sees a reachable enemy flag, it uses local Breadth-First Search, or BFS, to choose the first step toward it. Otherwise, it chooses an exploration move based on visible free cells and tentative movement history. When blocked, it waits.

We also created **six development maps**, covering an open arena, turning corridors, branching dead ends, a blocked start, two rooms and zigzag barriers. A seventh map is reserved for later testing.

**Second, our results.** In a separate offline C++ test environment, the ScoutAgent passed **nineteen unit checks**, the movement-observation helper passed **eight checks**, and all **six development maps** passed their scripted tests. The simulator recorded zero blocked movement requests. It reached the flag cell on the five maps containing a reachable flag. These are simulator observations; a pickup request is not proof of actually capturing a flag in the graphical game.

**Third, the main challenge.** The host gives only a local snapshot. Its `lastAction` value is a requested action, not proof the move succeeded. Our tentative movement history is therefore not a reliable global position. We are deliberately not claiming full map memory, frontier exploration, a complete FSM, or a working competition strategy yet.

**Finally, next steps and responsibilities.** Aiswarya owns API and movement verification and confidence-aware mapping under WP-02 and WP-03. Her next step is to compile the DLL and verify physical movement and flag behavior in the original Windows host. For Research, I am responsible for WP-06, WP-07 and initial WP-08 preparation: keeping paper reading focused on methods that affect the code, documenting metrics, and reproducing our own-agent map tests. We will then review the results and update GitHub together.

Our question for you is: **what feedback from the host can reliably confirm a successful move and flag pickup?**

Thank you.

## Notes for delivery

- Point at the one slide's four areas: **Completed work → Results → Challenges → Next steps**.
- Say **nineteen, eight, six** clearly; do not combine unlike checks as one competition score.
- Say **offline simulator**, **pickup request**, and **Windows host still pending**.
- If asked who wrote/run the latest source, state that the preparation package provides the source and its independently rerun test evidence; the team will confirm personal contributions in the diary/Git history.
