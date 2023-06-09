/*
 * CS-230 Assignment 8: A* Search Algorithm
 * @file astar.cc
 * @author Jake Mair
 * @date May 1, 2023
 */


// CITE: GeeksforGeeks
// DESC: I did not know what A*search was so I used the geeksforgeeks website to learn what it was and used their pseudocode
//       to develope my code

// CITE: https://sentry.io/answers/char-to-int-in-c-and-cpp/
// DESC: How to convert from a char to and int

#include <iostream>
#include <vector>
#include <string>
#include <utility> // std::pair
#include <cstdlib>
#include <queue>
#include <cmath>
#include <fstream>
#include <sstream>

// Colouring the termnial
// Can install with "brew install termcolor"
#include <termcolor/termcolor.hpp>

using namespace std;

typedef pair<int, int> coordinates;

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
            } else if (row[j] == 1) {
                cout << " " << termcolor::color<219, 212, 212> << row[j] << termcolor::reset;
            } else {
                cout << " " << row[j];
            }
        }
        cout << " ]" << endl;
    }
    cout << endl;
}

// Prints the grid with red for not blocked and black for blocked
void print_blocked(vector<vector<int>> grid) {
    cout << endl;
    for (int i = 0; i < grid.size(); i++) {
        cout << "[";
        vector<int> row = grid[i];
        for (int j = 0; j < row.size(); j++) {
            if (row[j] == 0) {
                cout << " " << termcolor::red << row[j] << termcolor::reset;
            } else {
                cout << " " << termcolor::color<219, 212, 212> << row[j] << termcolor::reset;
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

// Prototyping
bool in_cord_vector(vector<coordinates> list, coordinates xy);

// Colour trace
void colour_trace(vector<vector<int>> grid, vector<coordinates> xy, Node* start, Node* end) {
    for (int i = 0; i < grid.size(); i++) {
        cout << "[";
        vector<int> row = grid[i];
        for (int j = 0; j < row.size(); j++) {
            coordinates temp(i, j);
            if (temp == start->location) {
                cout << " " << termcolor::red << row[j] << termcolor::reset;  
            } else if (temp == end->location) {
                cout << " " << termcolor::blue << row[j] << termcolor::reset;  
            } else if (in_cord_vector(xy, temp)) {
                cout << " " << termcolor::green << row[j] << termcolor::reset;  
            } else {
                cout << " " << termcolor::color<219, 212, 212> << row[j] << termcolor::reset;
            }
        }
        cout << " ]" << endl;
    }
    cout << endl;
}

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
    for (int i = cords.size() - 1 ; i > -1; i--) {
        out << cords[i];
    }
    return out;
}

// Overload the less than operator for Nodes
bool operator<(const Node& a, const Node& b) {
    return a.f > b.f;
}

// Prototyping
double euclidian(coordinates a, coordinates b);

// Checks if the coordinates are already in the queue
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

// Checks if the coordinates are already in the vector of nodes
bool in_vector(vector<Node*> list, coordinates xy) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i]->location == xy) {
            return true;
        }
    }
    return false;
}

// Checks if the coordinates are already in the vector of coordinates
bool in_cord_vector(vector<coordinates> list, coordinates xy) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == xy) {
            return true;
        }
    }
    return false;
}

// Prototyping
vector<Node*> generate_successors(vector<vector<int>> grid, Node* parent, Node* target);
vector<coordinates> find_path(Node* target);

// A*search algorithm
vector<coordinates> search(vector<vector<int>> grid, Node* start, Node* target) {
    priority_queue<Node*> open_list;
    vector<Node*> closed_list;
    vector<coordinates> path;

    open_list.push(start);

    while (!open_list.empty()) {
        Node* q = open_list.top();
        open_list.pop();

        vector<Node*> successors = generate_successors(grid, q, target);

        for (int i = 0; i < successors.size(); i++) {
            if (successors[i]->location == target->location) {
                path = find_path(successors[i]);
                cout << termcolor::yellow << "The shortest path is: " << path << endl << termcolor::reset;
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

// Checks if the given coordinates are a valid direction within the grid
bool valid_direction(vector<vector<int>> grid, coordinates xy) {
    return (xy.first >= 0 && xy.first < grid.size() && xy.second >= 0 && xy.second < grid[0].size());
}

// Checks if the given coordinates are blocked
bool unblocked(vector<vector<int>> grid, coordinates xy) {
    return grid[xy.first][xy.second] == 1;
}

// Helper function for search to find the neighbours
vector<Node*> generate_successors(vector<vector<int>> grid, Node* parent, Node* target) {
    vector<Node*> successors;
    // To generate the different successors
    vector<int> x_values = {-1, -1, -1, 0, 0, 1, 1, 1};
    vector<int> y_values = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < SUCCESSORS_POSSIBLE; i++) {
        int x = parent->location.first + x_values[i];
        int y = parent->location.second + y_values[i];

        coordinates new_cord(x, y);

        if (valid_direction(grid, new_cord) && !unblocked(grid, new_cord) && new_cord != parent->location) {
            int m_cost = euclidian(new_cord, parent->location) + parent->move_cost;
            int h_cost = euclidian(new_cord, target->location);
            Node* successor = new Node(new_cord, m_cost, h_cost);
            successor->parent = parent;

            successors.push_back(successor);
        }
    }
    return successors;
}

// If needed to check, can print the queue
void print_queue(priority_queue<Node*> &pq) {
    while (!pq.empty())
		{
			cout << pq.top() << endl;
			pq.pop();
		}
}

// Generates a grid
vector<vector<int>> generate_grid(int rows, int cols) {
    vector<vector<int>> grid;
    for (int i = 0; i < rows; i++) {
        vector<int> row;
        for (int j = 0; j < cols; j++) {
            int rand_num = rand() % 2;
            row.push_back(rand_num);
        }
        grid.push_back(row);
    }
    return grid;
}

// Heuristic function
double euclidian(coordinates a, coordinates b) {
    int x_diff = a.first - b.first;
    int y_diff = a.second - b.second;
    return sqrt(x_diff * x_diff + y_diff * y_diff);
}

// Retraces the steps to find the path
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

    // Reading in the graph
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

    // Starting and ending values for coordiantes to be built
    int s_x, s_y, e_x, e_y;

    string affirm;
    int rows, cols;

    cout << endl << termcolor::yellow << "Would you like a generated graph? (Y or N)" << endl;
    cin >> affirm;
    cout << termcolor::reset;

    if (affirm == "Y" || affirm == "y") {
        cout << endl << termcolor::cyan << "# of rows: ";
        cin >> rows;
        cout << "# of cols: ";
        cin >> cols;
        cout << termcolor::reset;
        grid = generate_grid(rows, cols);
        print_blocked(grid);
    } else {
        print_blocked(grid);
    }

    cout << termcolor::yellow << "Enter coordinates for the start value ((0, 0) is the top left, the first value " << endl;
    cout << "increments as you go down and the second increments as you move over): " << termcolor::reset << endl << endl;
    cout << termcolor::cyan << "Enter x: ";
    cin >> s_x;
    cout << "Enter y: ";
    cin >> s_y;
    cout << termcolor::reset << endl << termcolor::yellow << "Enter end coordinates: " << endl << termcolor::reset << endl;
    cout << termcolor::cyan << "Enter x: ";
    cin >> e_x;
    cout << "Enter y: ";
    cin >> e_y;
    cout << termcolor::reset << endl;  

    coordinates st_cord(s_x, s_y);
    coordinates end_cord(e_x, e_y);

    pretty_print(grid, st_cord, end_cord);
    
    Node* start = new Node(st_cord, 0, 0);
    Node* end = new Node(end_cord, 0, 0);

    vector<coordinates> path;
    path = search(grid, start, end);

    cout << endl;

    colour_trace(grid, path, start, end);
    cout << endl << termcolor::red << "Search completed." << endl << termcolor::reset;
    
    if (path.empty()) {
        cout << termcolor::red << "No results yielded. No available path." << endl << termcolor::reset;
    }

}