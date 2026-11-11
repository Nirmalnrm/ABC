// S = start, G = goal, . = traversable, # = wall, ? = unknown (blocked).

#include <array>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
constexpr int N=5;
using Grid = std::array<std::string, N>;
using Point = std::pair<int,int>; // y,x

std::vector<Point> bfs_path(const Grid& g, Point start, Point goal) {
    std::array<std::array<bool,N>,N> seen{};
    std::array<std::array<Point,N>,N> parent{};
    auto valid = [&](Point p) {
        int y=p.first, x=p.second;
        return y>=0 && y<N && x>=0 && x<N && (g[y][x]=='.' || g[y][x]=='S' || g[y][x]=='G');
    };
    if (!valid(start) || !valid(goal)) return {};
    std::queue<Point> q;
    q.push(start);
    seen[start.first][start.second]=true;
    parent[start.first][start.second]=start;
    constexpr int dy[4]={-1,0,1,0};
    constexpr int dx[4]={0,1,0,-1};
    while (!q.empty()) {
        Point p=q.front(); q.pop();
        if (p==goal) {
            std::vector<Point> route;
            Point cur=goal;
            while (cur!=start) {
                route.push_back(cur);
                cur=parent[cur.first][cur.second];
            }
            route.push_back(start);
            return {route.rbegin(),route.rend()};
        }
        for (int d=0; d<4; ++d) {
            Point next={p.first+dy[d],p.second+dx[d]};
            if (valid(next) && !seen[next.first][next.second]) {
                seen[next.first][next.second]=true;
                parent[next.first][next.second]=p;
                q.push(next);
            }
        }
    }
    return {};
}

void test(const std::string& title, const Grid& g, Point s, Point goal, int expected_moves) {
    std::vector<Point> route=bfs_path(g,s,goal);
    int actual=route.empty()?-1:static_cast<int>(route.size())-1;
    if (actual!=expected_moves) throw std::runtime_error(title + ": expected " +
        std::to_string(expected_moves) + " moves, got " + std::to_string(actual));
    for (size_t i=1;i<route.size();++i) {
        int dy=std::abs(route[i].first-route[i-1].first);
        int dx=std::abs(route[i].second-route[i-1].second);
        if (dx+dy!=1) throw std::runtime_error(title+": invalid diagonal/nonadjacent move");
    }
    std::cout << "[PASS] " << title << " (moves=" << actual << ")\n";
}
} // namespace

int main() {
    try {
        test("Open grid shortest route", Grid{".....",".....","..S..",".....","....G"}, {2,2}, {4,4}, 4);
        test("Detour around known wall", Grid{".....",".....","..S#G",".....","....."}, {2,2}, {2,4}, 4);
        test("Goal unreachable (four blocked neighbors)", Grid{".....","..#..",".#S#.","..#..","....G"}, {2,2}, {4,4}, -1);
        test("Unknown cells never treated as traversable", Grid{"?????","?????","??S?G","?????","?????"}, {2,2}, {2,4}, -1);
        test("Start equals goal", Grid{".....",".....","..S..",".....","....."}, {2,2}, {2,2}, 0);
        std::cout << "\nSUCCESS: 5 synthetic BFS cases passed.\n";
        return EXIT_SUCCESS;
    } catch(const std::exception& e) {
        std::cerr << "[FAIL] " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
