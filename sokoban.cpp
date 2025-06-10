#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
using namespace std;

const char WALL = 'X', STORAGELOC = 'O', BOB = 'B', VACANT = '-';
const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
const vector<string> dirLabels = {"U", "D", "L", "R"};
const vector<string> pullLabels = {"DP", "UP", "RP", "LP"};

class Coordinate
{
private:
    unordered_map<int, unordered_set<int>> coords; // row, column

public:
    // Insert a point (x, y)
    void insert(int x, int y)
    {
        coords[y].insert(x);
    }

    // Check if a point (x, y) exists
    bool contains(int x, int y) const
    {
        auto rowIt = coords.find(y);
        if (rowIt == coords.end())
            return false;
        return rowIt->second.count(x) > 0;
    }
};

struct State
{
    pair<int, int> bob;
    vector<pair<int, int>> boxes;
    vector<string> moves;

    bool operator==(const State &other) const
    {
        return bob == other.bob && boxes == other.boxes;
    }
};

namespace std
{
    template <>
    struct hash<State>
    {
        size_t operator()(const State &s) const
        {
            size_t h = hash<int>()(s.bob.first * 31 + s.bob.second);
            for (auto &p : s.boxes)
            {
                h ^= hash<int>()(p.first * 37 + p.second);
            }
            return h;
        }
    };
}

bool gameEnd(const vector<pair<int, int>> &boxes, const Coordinate &storage)
{
    for (auto &b : boxes)
    {
        if (!storage.contains(b.first, b.second))
            return false;
    }
    return true;
}

bool containBox(const vector<pair<int, int>> &boxes, int x, int y)
{
    for (auto &p : boxes)
        if (p.first == x && p.second == y)
            return true;
    return false;
}

void moveBox(vector<pair<int, int>> &boxes, pair<int, int> from, pair<int, int> to)
{
    for (auto &p : boxes)
    {
        if (p == from)
        {
            p = to;
            return;
        }
    }
}

vector<string> BFSShortestPath(State initialState, const Coordinate storage, const Coordinate walls)
{
    queue<State> q;               // tiles to be visited
    unordered_set<State> visited; // store the visited states

    q.push(initialState);
    visited.insert(initialState);

    while (!q.empty())
    {
        State curState = q.front();
        q.pop();

        if (gameEnd(curState.boxes, storage))
        {
            return curState.moves;
        }

        for (int i = 0; i < 4; i++)
        {                                                                                                           // four possible accessible tiles
            pair<int, int> next_dir = directions[i];                                                                // next direction
            pair<int, int> next_pos = {curState.bob.first + next_dir.first, curState.bob.second + next_dir.second}; // next position of Bob

            bool wallOnPos = walls.contains(next_pos.first, next_pos.second); // is a wall on the position in the next direction
            if (wallOnPos)
                continue;
            bool boxOnPos = containBox(curState.boxes, next_pos.first, next_pos.second); // is a box on the position in the next direction
            // if there's no wall or box on the next position, then Bob can move
            if (!boxOnPos)
            {
                State nextState = curState;
                nextState.bob = next_pos;
                nextState.moves.emplace_back(dirLabels[i]);
                if (!visited.count(nextState))
                {
                    visited.insert(nextState);
                    q.push(nextState);
                }
            }
            // if there's a box on the position and it is vacant behind the box in the same direction, then Bob can push
            pair<int, int> tileBehind = {next_pos.first + next_dir.first, next_pos.second + next_dir.second};
            bool isVacant = !containBox(curState.boxes, tileBehind.first, tileBehind.second) && !walls.contains(tileBehind.first, tileBehind.second);
            if (boxOnPos && isVacant)
            {
                State nextState = curState;
                nextState.bob = next_pos;
                moveBox(nextState.boxes, next_pos, tileBehind);
                nextState.moves.emplace_back(dirLabels[i]);
                if (!visited.count(nextState))
                {
                    visited.insert(nextState);
                    q.push(nextState);
                }
            }
            // if there's a box on the position and it is vacant behind Bob in the opposite direction, then Bob can pull
            pair<int, int> box_pos = next_pos;
            next_pos = {curState.bob.first - next_dir.first, curState.bob.second - next_dir.second}; // next position of Bob
            wallOnPos = walls.contains(next_pos.first, next_pos.second);
            if (wallOnPos)
                continue;
            if (boxOnPos && !containBox(curState.boxes, next_pos.first, next_pos.second))
            {
                State nextState = curState;
                nextState.bob = next_pos;
                moveBox(nextState.boxes, box_pos, curState.bob);
                nextState.moves.emplace_back(pullLabels[i]);
                if (!visited.count(nextState))
                {
                    visited.insert(nextState);
                    q.push(nextState);
                }
            }
        }
    }
    return {};
}

int main()
{
    int NUM_BOXES, ROOM_WIDTH;
    pair<int, int> bob;
    Coordinate storage; // positions of storage locations
    Coordinate walls;   // positions of walls
    vector<pair<int, int>> boxes;
    cin >> NUM_BOXES >> ROOM_WIDTH;

    for (int i = 0; i < NUM_BOXES; i++)
    {
        int x, y;
        cin >> x >> y;
        boxes.emplace_back(x, y);
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < ROOM_WIDTH; i++)
    {
        string line;
        getline(cin, line);
        for (int j = 0; j < ROOM_WIDTH; j++)
        {
            char input = line[j];
            if (input == WALL)
                walls.insert(i, j);
            else if (input == STORAGELOC)
                storage.insert(i, j);
            else if (input == BOB)
                bob = pair<int, int>(i, j);
            else if (input == VACANT)
                continue;
        }
    }
    State initialState = {bob, boxes, {}};
    vector<string> shortestPath = BFSShortestPath(initialState, storage, walls);

    cout << shortestPath.size() << endl;
    for (string &move : shortestPath)
        cout << move << " ";
    cout << endl;
}
