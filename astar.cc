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
int SUCCESSORS_POSSIBLE = 8;

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

bool in_queue(priority_queue<Node> &pq, coordinates xy) {
    priority_queue<Node> copy = pq;
    while (!copy.empty()) {
        if (copy.top().location == xy) {
            return true;
        }
        copy.pop();
    }
    return false;
}

bool in_vector(vector<Node> list, coordinates xy) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i].location == xy) {
            return true;
        }
    }
    return false;
}

vector<Node> generate_successors(vector<vector<int>> grid, Node parent, Node target);

void search(vector<vector<int>> grid, Node start, Node target) {
    priority_queue<Node> open_list;
    vector<Node> closed_list;

    open_list.push(start);

    while (!open_list.empty()) {
        Node q = open_list.top();
        open_list.pop();

        vector<Node> successors = generate_successors(grid, q, target);

        for (int i = 0; i < successors.size(); i++) {
            if (successors[i].location == target.location) {
                return;
            } else {
                if (in_queue(open_list, successors[i].location)) {
                    continue;
                }
                if (in_vector(closed_list, successors[i].location)) {
                    continue;
                }
                open_list.push(successors[i]);

            }

        }

       closed_list.push_back(q);
    }
}

bool valid_direction(vector<vector<int>> grid, coordinates xy) {
    return (xy.first >= 0 && xy.first < grid[0].size() && xy.second >= 0 && xy.second < grid.size());
}

bool unblocked(vector<vector<int>> grid, coordinates xy) {
    return grid[xy.first][xy.second] == 1;
}

vector<Node> generate_successors(vector<vector<int>> grid, Node parent, Node target) {
    vector<Node> successors;
    vector<int> x_values = {-1, -1, -1, 0, 0, 1, 1, 1};
    vector<int> y_values = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; i++) {
        int x = parent.location.first + x_values[i];
        int y = parent.location.second + y_values[i];

        coordinates new_cord(x, y);

        if (valid_direction(grid, new_cord) && !unblocked(grid, new_cord)) {
            int m_cost = euclidian(new_cord, parent.location);
            int h_cost = euclidian(new_cord, target.location);
            Node successor(new_cord, m_cost, h_cost);
            successor.parent = &parent;

            successors.push_back(successor);
        }
    }
    return successors;
}


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
    
    Node start(st_cord, 0, 0);
    Node end(end_cord, 0, 0);

    vector<vector<int>> grid = {
        {0, 0, 0, 1, 1},
        {0, 1, 0, 0, 0}
    };

    search(grid, start, end);

}