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
#include <cmath>


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
    double move_cost;
    double h_cost;
    double f;
    Node* parent;

    Node (coordinates l, double m, double h) {
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

double euclidian(coordinates a, coordinates b);

// void create_nodes(vector<vector<int>> grid, priority_queue<Node> &pq) {
//     int x = grid.size() - 1;
//     int y = grid[0].size() - 1;

//     coordinates end_pt(x, y);
//     for (int i = 0; i < grid.size(); i++) {
//         vector<int> temp = grid[i];
//         for (int j = 0; j < temp.size(); j++) {
//             coordinates current(i, j);
//             Node a(current, 0, euclidian(current, end_pt));
//             // cout << a << endl;
//             pq.push(a);
//         }
//     }
// }

void search(vector<vector<int>> grid) {
    priority_queue<Node> open_list;
    vector<Node> closed_list;

    Node start_node(START_CORD, 0, 0);
    open_list.push(start_node);

    while (!open_list.empty()) {
        Node q = open_list.top();
        open_list.pop();

        // vector<Node> successors = generate_successors(grid, q);

        /*
        for (int i = 0; i < successors.size(); i++) {
            if successors[i] is the goal stop search

            else 
                compute g and h
                successors[i].move_cost = euclidian(successors[i].location, q.location);
                successors[i].h_cost = euclidian(successors[i].location, target_node.location);
            
            
            check open list
            check closed list

            add successors to open list
        }
        
        */

       closed_list.push_back(q);

    }
}

bool valid_direction(vector<vector<int>> grid, coordinates xy) {
    return (xy.first >= 0 && xy.first < grid[0].size() && xy.second >= 0 && xy.second < grid.size());
}

/* vector<Node> generate_successors(vector<vector<int>> grid, Node parent) {
    // Look in all directions and find which directions are valid
    // Store those nodes in a vector
}

*/

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
double euclidian(coordinates a, coordinates b) {
    int x_diff = a.first - b.first;
    int y_diff = a.second - b.second;
    return sqrt(x_diff * x_diff + y_diff * y_diff);
}

int main() {
    coordinates st_cord(0, 0);
    coordinates end_cord(1, 4);
    Node start(st_cord, 0, euclidian(st_cord, end_cord));

    coordinates invalid(0, 8);
    // cout << manhattan(a, b) << endl;

    // cout << a.location << endl;

    vector<vector<int>> grid = {
        {0, 0, 0, 1, 1},
        {0, 1, 0, 0, 0}
    };

    cout << boolalpha << valid_direction(grid, invalid) << endl;
}