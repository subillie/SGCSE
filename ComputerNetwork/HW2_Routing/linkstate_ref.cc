// linkstate.cc - Link State Routing Algorithm

#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

class LinkStateRouter {
private:
	int numNodes;
	vector<vector<int>> graph;

public:
	LinkStateRouter(int n) : numNodes(n) {
		graph.resize(numNodes, vector<int>(numNodes, INF));
		for (int i = 0; i < numNodes; ++i) {
			graph[i][i] = 0;
		}
	}

	void addLink(int node1, int node2, int cost) {
		graph[node1][node2] = cost;
		graph[node2][node1] = cost;
	}

	void updateShortestPaths() {
		vector<bool> visited(numNodes, false);
		vector<int> distance(numNodes, INF);

		distance[0] = 0;

		for (int i = 0; i < numNodes - 1; ++i) {
			int minDist = INF;
			int minIndex = -1;

			// Find the node with the minimum distance
			for (int j = 0; j < numNodes; ++j) {
				if (!visited[j] && distance[j] < minDist) {
					minDist = distance[j];
					minIndex = j;
				}
			}

			// Mark the node as visited
			visited[minIndex] = true;

			// Update the distances of adjacent nodes
			for (int j = 0; j < numNodes; ++j) {
				if (!visited[j] && graph[minIndex][j] != INF && distance[minIndex] + graph[minIndex][j] < distance[j]) {
					distance[j] = distance[minIndex] + graph[minIndex][j];
				}
			}
		}

		// Print the shortest paths
		cout << "Shortest Paths:\n";
		for (int i = 0; i < numNodes; ++i) {
			cout << "Node " << i << ": " << distance[i] << "\n";
		}
	}
};

int main() {
	int numNodes = 4;
	LinkStateRouter router(numNodes);

	// Adding links and their costs
	router.addLink(0, 1, 1);
	router.addLink(0, 2, 5);
	router.addLink(1, 2, 2);
	router.addLink(1, 3, 3);
	router.addLink(2, 3, 1);

	// Updating the shortest paths
	router.updateShortestPaths();

	return 0;
}
