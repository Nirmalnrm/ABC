# Our Agent Strategy — Week 6 ScoutAgent

**Implementation:** `tests/Week02_HostandAgent/scout_agent.cpp`  
**Status:** Self-developed Week 6 navigation prototype, **not a full competition agent**. The original `agent_api.h` and `host.exe` are unchanged.

## Decision sequence: one action per host step

1. Read the current `WorldSnapshot.cells[5][5]`, with self at `[2][2]`, and identify known floor/walls and occupied cells.
2. **Visible flag priority:** if an enemy flag is visible and reachable within the known cells of the current 5 × 5 snapshot, use four-direction BFS to find the shortest local route; return its first movement action.
3. If the enemy flag appears on the current cell, issue **one `ACTION_PICK_UP_FLAG` request**, then apply a cooldown. This is an experimental assumption—the host's pickup rules and possession feedback are **not confirmed**.
4. **Explore:** if there is no actionable visible flag (or pickup cooldown is active), consider only immediately adjacent known-floor, unoccupied cardinal cells. Prefer tentative unvisited directions, a continuous heading, forward open cells, and less-repeated local-view choices.
5. **Safety:** never request movement toward an immediately known wall, unknown cell or enemy-occupied cell. Return `ACTION_WAIT` when no candidate exists. No diagonal moves in this prototype.
6. On reset, clear local observation counts, tentative visit rankings, last heading and pickup cooldown.

## What memory means here

- `choicesByView` is keyed by **5 × 5 terrain-pattern fingerprint**. It records actions requested in similar *observations*, **not** physical cell visits.
- `tentativeX/tentativeY` and `tentativeVisits` count positions inferred by *assuming* requested moves succeeded. This is **only a scoring heuristic** and is explicitly **untrusted**.
- The heuristic **never** writes a global map or directs BFS outside the current visible 5 × 5 region. If requested movement fails, the tentative history may drift; WP-02/WP-03 must investigate this before real mapping.

## Compact pseudocode

```text
On Agent_Step(snapshot, lastAction):
    if instance or snapshot invalid: return WAIT
    if pickup cooldown elapsed:
        if enemy flag reachable inside current 5x5:
            if on flag: request PICKUP (NOT proof of possession)
            else: return first step of local BFS route
    legal = immediately adjacent known-floor, unoccupied cells
    if legal is empty: return WAIT
    score each legal cardinal step using tentative-visit count,
        local-view action count, non-reverse and forward-clear preference
    request the best-scoring legal step
```

## Why this is our own agent

It has a separate exported name, new source, deterministic stateful local decisions, a local visible-flag BFS objective, an explicit safety gate, and its own unit and multi-map test harness. The older `StupidAgent` remains a professor-provided educational example only; we do not use it as the current evaluation comparator.

## Later stages, not yet implemented

- Verify physical displacement from host observations and explicit confidence rules (WP-02).
- Persistent terrain map with untrusted/trusted pose separation (WP-03).
- True global frontier-based exploration across known cells (WP-04).
- Verified possession, delivery/return FSM and opponent responses (WP-05).
- Match and opponent evaluation once the game feedback is observable (WP-09/10/12).

## Test boundary

The custom ASCII maps are used **only by the separate offline simulator**, not by `host.exe`; the ZIP does not establish a custom-map loading API for the graphical host. The simulator knows the full map for scoring, but gives the agent only a **local 5 × 5 observation** each step. The simulator's visits/flag-cell arrival must not be described as Windows-host results or capture success.

## Movement-outcome experiment added 11 November 2026

`tests/Week02_HostandAgent/movement_evidence.h` compares two consecutive **terrain-only** 5 × 5 snapshots following a requested cardinal move. It returns one of three labels: `ConsistentWithMove`, `ConsistentWithStay` or `Ambiguous`. When both hypotheses fit (for example identical floor cells), the result is deliberately ambiguous; a non-fitting pair is also ambiguous rather than forcing a location update. Eight offline unit checks passed.

**This helper is not used by `ScoutAgent` yet.** It cannot confirm actual host movement, absolute location or flag possession. It is a conservative design/test artifact for WP-02/03; its output must not be used as a verified global map coordinate without host validation.
