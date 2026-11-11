#pragma once

// Week 6 experiment: what can successive local observations actually support?
// This does not change host.exe or claim a verified global position.
#include "agent_api.h"
#include <cstdint>

enum class MovementEvidence {
    ConsistentWithMove,
    ConsistentWithStay,
    Ambiguous
};

inline MovementEvidence compare_movement_observations(
    const WorldSnapshot& before, const WorldSnapshot& after, uint32_t requestedAction) {
    int dx = 0, dy = 0;
    switch (requestedAction) {
        case ACTION_MOVE_NORTH: dy = -1; break;
        case ACTION_MOVE_EAST: dx = 1; break;
        case ACTION_MOVE_SOUTH: dy = 1; break;
        case ACTION_MOVE_WEST: dx = -1; break;
        default: return MovementEvidence::Ambiguous;
    }

    // Contents (agents/flags) can move; compare only terrain in overlapping views.
    bool supportsStay = true, supportsMove = true;
    for (int y = 0; y < VIEW_SIZE; ++y) {
        for (int x = 0; x < VIEW_SIZE; ++x) {
            const uint32_t current = after.cells[y][x] & CELLTYPE_BITMASK;
            if ((before.cells[y][x] & CELLTYPE_BITMASK) != current) supportsStay = false;
            const int oldX = x + dx, oldY = y + dy;
            if (oldX >= 0 && oldX < VIEW_SIZE && oldY >= 0 && oldY < VIEW_SIZE &&
                (before.cells[oldY][oldX] & CELLTYPE_BITMASK) != current) {
                supportsMove = false;
            }
        }
    }
    // If both are possible (e.g. a featureless corridor) no pose claim is justified.
    if (supportsMove && !supportsStay) return MovementEvidence::ConsistentWithMove;
    if (supportsStay && !supportsMove) return MovementEvidence::ConsistentWithStay;
    return MovementEvidence::Ambiguous;
}
