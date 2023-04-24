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


using namespace std;

typedef pair<int, int> coordinates;

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


    Node (coordinates l) {
        location = l;
    }
};


int main() {
    coordinates c(5,6);
    Node a(c);

    cout << a.location << endl;

    vector<vector<int>> grid = {
        {0, 0, 0, 1, 1},
        {0, 1, 1, 1, 0}
    };

    cout << grid << endl;
}