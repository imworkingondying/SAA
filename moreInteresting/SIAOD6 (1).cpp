#include <iostream>
#include <vector>
#include <stack>
#include <climits>

using namespace std;


class Graph {
private:
    int numNodes;
    vector<vector<int>> adjacencyMatrix;

public:
    Graph(int n) : numNodes(n), adjacencyMatrix(n, vector<int>(n, 0)) {}

    void addNode() {
        numNodes++;
        for (int i = 0; i < numNodes - 1; ++i) {
            adjacencyMatrix[i].push_back(0);
        }
        adjacencyMatrix.push_back(vector<int>(numNodes, 0));
    }

    void addEdge(int from, int to, int distance) {
        if (from >= 0 && from < numNodes && to >= 0 && to < numNodes) {
            adjacencyMatrix[from][to] = distance;
            adjacencyMatrix[to][from] = distance; 
        }
    }

    void depthFirstSearch(int startNode, vector<bool>& visited) {
        stack<int> stack;
        stack.push(startNode);
        visited[startNode] = true;

        while (!stack.empty()) {
            int currentNode = stack.top();
            stack.pop();
            cout << currentNode << " ";

            for (int i = 0; i < numNodes; ++i) {
                if (adjacencyMatrix[currentNode][i] != 0 && !visited[i]) {
                    stack.push(i);
                    visited[i] = true;
                }
            }
        }
    }

    void depthFirstSearch() {
        vector<bool> visited(numNodes, false);

        for (int i = 0; i < numNodes; ++i) {
            if (!visited[i]) {
                depthFirstSearch(i, visited);
            }
        }
    }

    void printGraph() {
        for (int i = 0; i < numNodes; ++i) {
            for (int j = 0; j < numNodes; ++j) {
                cout << adjacencyMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    Graph graph(7); 

    graph.addEdge(1, 2, 2);
    graph.addEdge(1, 3, 4);
    graph.addEdge(1, 4, 5);
    graph.addEdge(2, 4, 4);
    graph.addEdge(2, 5, 3);
    graph.addEdge(2, 6, 0);
    graph.addEdge(4, 5, 6);
    graph.addEdge(3, 6, 7);
    graph.addEdge(4, 6, 4);
    graph.addEdge(5, 6, 2);

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Add Node\n";
        cout << "2. Add Edge\n";
        cout << "3. Print Graph\n";
        cout << "4. Depth First Search\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            graph.addNode();
            break;

        case 2: {
            int from, to, distance;
            cout << "Enter source node: ";
            cin >> from;
            cout << "Enter destination node: ";
            cin >> to;
            cout << "Enter distance: ";
            cin >> distance;
            graph.addEdge(from, to, distance);
            break;
        }

        case 3:
            cout << "Graph:" << endl;
            graph.printGraph();
            break;

        case 4:
            cout << "Depth First Search:" << endl;
            graph.depthFirstSearch();
            break;

        case 0:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
