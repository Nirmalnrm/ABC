// Offline interpretation tests; these are NOT assertions about host mechanics.
#include "movement_evidence.h"
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
int checks = 0;
void check(bool pass, const char* label) {
    ++checks;
    if (!pass) throw std::runtime_error(std::string("FAIL: ") + label);
    std::cout << "[PASS] " << label << '\n';
}
WorldSnapshot textured() {
    WorldSnapshot s{};
    for (int y = 0; y < VIEW_SIZE; ++y)
        for (int x = 0; x < VIEW_SIZE; ++x)
            s.cells[y][x] = ((x * 3 + y * 7) % 5 == 0) ? CELLTYPE_WALL : CELLTYPE_FLOOR;
    return s;
}
WorldSnapshot translated(const WorldSnapshot& from, int dy, int dx) {
    WorldSnapshot to{};
    for (int y = 0; y < VIEW_SIZE; ++y)
        for (int x = 0; x < VIEW_SIZE; ++x) {
            int oldY = y + dy, oldX = x + dx;
            to.cells[y][x] = (oldY >= 0 && oldY < VIEW_SIZE && oldX >= 0 && oldX < VIEW_SIZE)
                ? from.cells[oldY][oldX] : CELLTYPE_UNKNOWN;
        }
    return to;
}
} // namespace

int main() {
    try {
        WorldSnapshot before = textured();
        check(compare_movement_observations(before, translated(before, 0, 1), ACTION_MOVE_EAST)
                  == MovementEvidence::ConsistentWithMove, "East translation supports movement");
        check(compare_movement_observations(before, translated(before, -1, 0), ACTION_MOVE_NORTH)
                  == MovementEvidence::ConsistentWithMove, "North translation supports movement");
        check(compare_movement_observations(before, before, ACTION_MOVE_EAST)
                  == MovementEvidence::ConsistentWithStay, "Unchanged textured view supports staying");
        WorldSnapshot featureless{};
        for (auto& row : featureless.cells) for (auto& cell : row) cell = CELLTYPE_FLOOR;
        check(compare_movement_observations(featureless, featureless, ACTION_MOVE_EAST)
                  == MovementEvidence::Ambiguous, "Uniform terrain cannot confirm displacement");
        WorldSnapshot changed = before;
        for (auto& row : changed.cells) for (auto& cell : row) cell = CELLTYPE_UNKNOWN;
        check(compare_movement_observations(before, changed, ACTION_MOVE_EAST)
                  == MovementEvidence::Ambiguous, "Inconsistent observations remain ambiguous");
        WorldSnapshot dynamic = before;
        dynamic.cells[2][2] |= CELLCONTENT_ENEMY;
        dynamic.cells[1][1] |= CELLCONTENT_ENEMY_FLAG;
        check(compare_movement_observations(before, dynamic, ACTION_MOVE_WEST)
                  == MovementEvidence::ConsistentWithStay, "Dynamic cell contents are ignored");
        check(compare_movement_observations(before, before, ACTION_WAIT)
                  == MovementEvidence::Ambiguous, "Waiting does not provide movement evidence");
        check(compare_movement_observations(before, before, ACTION_MOVE_NORTHEAST)
                  == MovementEvidence::Ambiguous, "Unmodelled diagonal action is ambiguous");
        std::cout << "SUCCESS: " << checks << " offline movement-evidence checks passed.\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
