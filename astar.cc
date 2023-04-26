/*
 * CS-230 Assignment 8: A* Search Algorithm
 * @file astar.cc
 * @author Jake Mair
 * @date May 1, 2023
 */

#include <iostream>
#include <vector>
#include <string>
#include <utility> // std::pair
#include <cstdlib>
#include <queue>


using namespace std;

typedef pair<int, int> coordinates;

coordinates START_CORD(0, 0);

// Print coordinates
ostream & operator<<(ostream &out, coordinates A) {
    out << "[" << A.first << ", " << A.second << "]";
    return out;
}

// Print 2D vector
ostream & operator<<(ostream &out, vector<vector<int>> &A) {
    for (int i = 0; i < A.size(); i++) {
        out << "[";
        vector<int> B = A[i];
        for (int j = 0; j < B.size(); j++) {
            out << " " << B[j] << " ";
        }
        out << "]" << endl;
    }
    return out;
}

struct Node {
    coordinates location;
    int move_cost;
    int h_cost;
    int f;
    Node* parent;

    Node (coordinates l, int m, int h) {
        location = l;
        move_cost = m;
        h_cost = h;
        f = m + h;
    }
};

// Print a Node
ostream & operator<<(ostream &out, Node A) {
    out << "Node: " << endl;
    out << "Location: " << "(" << A.location << ")" << endl;
    out << "Movement cost: " << A.move_cost << endl;
    out << "Heuristic cost: " << A.h_cost << endl;
    out << "F-value: " << A.f << endl;
    return out;
}

// Overload the less than operator for Nodes
bool operator<(const Node& a, const Node& b) {
    return a.f > b.f;
}

int manhattan(coordinates a, coordinates b);

void create_nodes(vector<vector<int>> grid, priority_queue<Node> &pq) {
    int x = grid.size() - 1;
    int y = grid[0].size() - 1;

    coordinates end_pt(x, y);
    for (int i = 0; i < grid.size(); i++) {
        vector<int> temp = grid[i];
        for (int j = 0; j < temp.size(); j++) {
            coordinates current(i, j);
            Node a(current, 0, manhattan(current, end_pt));
            // cout << a << endl;
            pq.push(a);
        }
    }
}

// In progress
// int movement_cost(vector<vector<int>> grid, coordinates target) {
//     coordinates current(START_CORD);
//     while (current != target) {

//     }
// }

void print_queue(priority_queue<Node> &pq) {
    while (!pq.empty())
		{
			cout << pq.top() << endl;
			pq.pop();
		}
}

// Heuristic function
int manhattan(coordinates a, coordinates b) {
    return (abs(a.first - b.first) + abs(a.second - b.second));
}

int main() {
    coordinates st_cord(0, 0);
    coordinates end_cord(1, 4);
    Node start(st_cord, 0, manhattan(st_cord, end_cord));

    // cout << manhattan(a, b) << endl;

    // cout << a.location << endl;

    vector<vector<int>> grid = {
        {0, 0, 0, 1, 1},
        {0, 1, 0, 0, 0}
    };

    cout << grid << endl;

    priority_queue<Node> pq;
    create_nodes(grid, pq);
    print_queue(pq);
}