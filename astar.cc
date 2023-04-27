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
#include <fstream>
#include <sstream>
#include <termcolor/termcolor.hpp>

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
            out << " " << B[j];
        }
        out << " ]" << endl;
    }
    return out;
}

// Print 2D vector with colour
void pretty_print(vector<vector<int>> grid, coordinates start, coordinates end) {
    for (int i = 0; i < grid.size(); i++) {
        cout << "[";
        vector<int> row = grid[i];
        for (int j = 0; j < row.size(); j++) {
            if (i == start.first && j == start.second) {
                cout << " " << termcolor::red << row[j] << termcolor::reset;
            } else if (i == end.first && j == end.second) {
                cout << " " << termcolor::blue << row[j] << termcolor::reset;
            } else {
                cout << " " << row[j];
            }
        }
        cout << " ]" << endl;
    }
    cout << endl;
}

// Colour trace
void colour_trace(vector<vector<int>> grid, vector<coordinates> xy, coordinates target) {
    int spot = xy.size() - 1;
    for (int i = 0; i < grid.size(); i++) {
        cout << "[";
        vector<int> row = grid[i];
        for (int j = 0; j < row.size(); j++) {
            if (i == xy[spot].first && j == xy[spot].second) {
                cout << " " << termcolor::green << row[i] << termcolor::reset;
                spot--;
            } else if (i == target.first && j == target.second) {
                cout << " " << termcolor::blue << row[i] << termcolor::reset;
            } else {
                cout << " " << row[i];
            }
        }
        cout << " ]" << endl;
    }
    cout << endl;
}



struct Node {
    coordinates location;
    double move_cost;
    double h_cost;
    double f;
    Node* parent = nullptr;

    Node (coordinates l, double m, double h) {
        location = l;
        move_cost = m;
        h_cost = h;
        f = m + h;
    }
};

// Print a Node
ostream & operator<<(ostream &out, Node* A) {
    out << "Node: " << endl;
    out << "Location: " << "(" << A->location << ")" << endl;
    out << "Movement cost: " << A->move_cost << endl;
    out << "Heuristic cost: " << A->h_cost << endl;
    out << "F-value: " << A->f << endl;
    out << "Parent: " << A->parent->location << endl;
    return out;
}

// Print vector of Nodes
ostream & operator<<(ostream &out, vector<Node*> nodes) {
    for (int i = 0; i < nodes.size(); i++) {
        out << nodes[i];
    }
    return out;
}

// Print vector of coordinates backwards
ostream & operator<<(ostream &out, vector<coordinates> cords) {
    for (int i = cords.size() -1 ; i > -1; i--) {
        out << cords[i];
    }
    return out;
}

// Overload the less than operator for Nodes
bool operator<(const Node& a, const Node& b) {
    return a.f > b.f;
}

double euclidian(coordinates a, coordinates b);

bool in_queue(priority_queue<Node*> &pq, coordinates xy) {
    priority_queue<Node*> copy = pq;
    while (!copy.empty()) {
        if (copy.top()->location == xy) {
            return true;
        }
        copy.pop();
    }
    return false;
}

bool in_vector(vector<Node*> list, coordinates xy) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i]->location == xy) {
            return true;
        }
    }
    return false;
}

vector<Node*> generate_successors(vector<vector<int>> grid, Node* parent, Node* target);
vector<coordinates> find_path(Node* target);

vector<coordinates> search(vector<vector<int>> grid, Node* start, Node* target) {
    priority_queue<Node*> open_list;
    vector<Node*> closed_list;
    vector<coordinates> path;

    open_list.push(start);

    int count = 0;
    while (!open_list.empty()) {
        Node* q = open_list.top();
        open_list.pop();

        vector<Node*> successors = generate_successors(grid, q, target);

        for (int i = 0; i < successors.size(); i++) {
            if (successors[i]->location == target->location) {
                path = find_path(successors[i]);
                cout << "The shortest path is: " << path << endl;
                return path; 
            } else {
                if (in_queue(open_list, successors[i]->location)) {
                    continue;
                }
                if (in_vector(closed_list, successors[i]->location)) {
                    continue;
                }
                open_list.push(successors[i]);

            }

        }
       closed_list.push_back(q);
    }
    return path;
}

bool valid_direction(vector<vector<int>> grid, coordinates xy) {
    return (xy.first >= 0 && xy.first < grid.size() && xy.second >= 0 && xy.second < grid[0].size());
}

bool unblocked(vector<vector<int>> grid, coordinates xy) {
    return grid[xy.first][xy.second] == 1;
}

vector<Node*> generate_successors(vector<vector<int>> grid, Node* parent, Node* target) {
    vector<Node*> successors;
    // To generate the different successors
    vector<int> x_values = {-1, -1, -1, 0, 0, 1, 1, 1};
    vector<int> y_values = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; i++) {
        int x = parent->location.first + x_values[i];
        int y = parent->location.second + y_values[i];

        coordinates new_cord(x, y);

        if (valid_direction(grid, new_cord) && !unblocked(grid, new_cord) && new_cord != parent->location) {
            int m_cost = euclidian(new_cord, parent->location);
            int h_cost = euclidian(new_cord, target->location);
            Node* successor = new Node(new_cord, m_cost, h_cost);
            successor->parent = parent;

            successors.push_back(successor);
        }
    }
    return successors;
}


void print_queue(priority_queue<Node*> &pq) {
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

vector<coordinates> find_path(Node* target) {
    vector<coordinates> path;
    Node* current = target;
    while (current->parent != nullptr) {
        path.push_back(current->location);
        current = current->parent;
    }
    path.push_back(current->location);
    return path;
}

int main(int argc, char ** argv) {
    string filename = argv[1];
    ifstream instream(filename);
    string line;

    vector<vector<int>> grid;

    while(getline(instream, line)) {
        vector<int> row;
        for (char c : line) {
            if (isdigit(c)) {
                row.push_back(c - '0');
            }
        }
        if (!row.empty()) {
            grid.push_back(row);
        }
    }

    cout << endl << grid << endl;
    int s_x;
    int s_y;
    int e_x;
    int e_y;

    cout << "Enter coordinates for the start value ((0, 0) is the top left, the first value " << endl;
    cout << "increments as you go down and the second increments as you move over): " << endl << endl;
    cout << "Enter x: ";
    cin >> s_x;
    cout << "Enter y: ";
    cin >> s_y;
    cout << endl << "Enter end coordinates: " << endl << endl;
    cout << "Enter x: ";
    cin >> e_x;
    cout << "Enter y: ";
    cin >> e_y;
    cout << endl;  

    coordinates st_cord(s_x, s_y);
    coordinates end_cord(e_x, e_y);

    pretty_print(grid, st_cord, end_cord);
    
    Node* start = new Node(st_cord, 0, 0);
    Node* end = new Node(end_cord, 0, 0);

    vector<coordinates> path;
    path = search(grid, start, end);

    colour_trace(grid, path, end->location);
    cout << endl << "Search completed." << endl;
}