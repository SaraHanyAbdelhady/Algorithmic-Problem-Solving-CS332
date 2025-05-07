#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
using namespace std;
const int N = 4, M = 3;  // 4x3 board
int dx[] = {2, 2, -2, -2, 1, 1, -1, -1};
int dy[] = {1, -1, 1, -1, 2, -2, 2, -2};
bool isValidMove(int x, int y, const vector<vector<int>>& board) 
{
    return x >= 0 && x < N && y >= 0 && y < M && board[x][y] == 0;
}
vector<pair<int, int>> validKnightMoves(int x, int y, const vector<vector<int>>& board)
 {
    vector<pair<int, int>> moves;
    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i], ny = y + dy[i];
        if (isValidMove(nx, ny, board)) {
            moves.emplace_back(nx, ny);
        }
    }
    return moves;
}
bool isTarget(const vector<vector<int>>& a, const vector<vector<int>>& b)
 {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (a[i][j] != b[i][j])
                return false;
    return true;
}
void printBoard(const vector<vector<int>>& board) 
{
    for (const auto& row : board) {
        for (int cell : row)
            cout << cell << " ";
        cout << "\n";
    }
    cout << "\n";
}
void initializeBoards(vector<vector<int>>& initial, vector<vector<int>>& target) 
{
    initial.assign(N, vector<int>(M, 0));
    target.assign(N, vector<int>(M, 0));
    initial[3][0] = initial[3][1] = initial[3][2] = 1;
    initial[0][0] = initial[0][1] = initial[0][2] = 2;
    target[0][0] = target[0][1] = target[0][2] = 1;
    target[3][0] = target[3][1] = target[3][2] = 2;
}
// For using vector<vector<int>> in map/set
struct BoardHash {
    size_t operator()(const vector<vector<int>>& board) const {
        size_t hash = 0;
        for (const auto& row : board)
            for (int val : row)
                hash = hash * 31 + val;
        return hash;
    }
};
void bfsKnightSwap(vector<vector<int>> start, const vector<vector<int>>& target) {
    queue<vector<vector<int>>> q;
    set<vector<vector<int>>> visited;
    map<vector<vector<int>>, vector<vector<int>>> parent;
    q.push(start);
    visited.insert(start);
    parent[start] = start;
    vector<vector<int>> goal;
    while (!q.empty()) {
        auto board = q.front();
        q.pop();
        if (isTarget(board, target)) {
            goal = board;
            break;
        }
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < M; ++y) {
                if (board[x][y] == 1 || board[x][y] == 2) {
                    int knight = board[x][y];
                    auto movesList = validKnightMoves(x, y, board);
                    for (auto [nx, ny] : movesList) {
                        auto newBoard = board;
                        newBoard[nx][ny] = knight;
                        newBoard[x][y] = 0;
                        if (!visited.count(newBoard)) {
                            visited.insert(newBoard);
                            parent[newBoard] = board;
                            q.push(newBoard);
                        }
                    }
                }
            }
        }
    }
    // Reconstruct and print the path from initial to goal
    if (goal.empty()) {
        cout << "No solution found.\n";
        return;
    }
    vector<vector<vector<int>>> path;
    while (!(goal == parent[goal])) {
        path.push_back(goal);
        goal = parent[goal];
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    for (size_t i = 0; i < path.size(); ++i) {
        cout << "Move " << i << ":\n";
        printBoard(path[i]);
    }
    cout << "Target reached in " << path.size() - 1 << " moves.\n";
}
int main() {
    vector<vector<int>> initial, target;
    initializeBoards(initial, target);
    cout << "Initial board:\n";
    printBoard(initial);
    bfsKnightSwap(initial, target);
    return 0;}
