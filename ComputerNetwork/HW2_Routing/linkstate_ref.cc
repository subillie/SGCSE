// linkstate.cc

#include <iostream>
#include <vector>
#include <limits>
#include <set>

using namespace std;

struct LinkState {
	int nodeCount;
	vector<vector<int> > costMatrix;
	vector<int> distance;
	vector<int> parent;
	set<int> unvisited;

	LinkState(int n) : nodeCount(n) {
		costMatrix.resize(n, vector<int>(n, numeric_limits<int>::max()));
		distance.resize(n, numeric_limits<int>::max());
		parent.resize(n, -1);
		for (int i = 0; i < n; ++i)
			costMatrix[i][i] = 0;

		for (int i = 0; i < n; ++i)
			unvisited.insert(i);
	}

	void updateCost(int from, int to, int cost) {
		costMatrix[from][to] = cost;
		costMatrix[to][from] = cost;
	}

	void printRoutingTable() {
		cout << "Routing Table:\n";
		for (int i = 0; i < nodeCount; ++i) {
			cout << "Node " << i << ": ";
			cout << "Distance = " << distance[i] << ", Parent = " << parent[i] << "\n";
		}
		cout << "\n";
	}

	int getMinDistanceNode() {
		int minDistance = numeric_limits<int>::max();
		int minNode = -1;
		for (int node : unvisited) {
			if (distance[node] < minDistance) {
				minDistance = distance[node];
				minNode = node;
			}
		}
		return minNode;
	}

	void calculateLinkState(int sourceNode) {
		distance[sourceNode] = 0;

		while (!unvisited.empty()) {
			int currentNode = getMinDistanceNode();
			if (currentNode == -1)
				break;

			unvisited.erase(currentNode);

			for (int i = 0; i < nodeCount; ++i) {
				if (unvisited.find(i) != unvisited.end() && costMatrix[currentNode][i] != numeric_limits<int>::max()) {
					int newDistance = distance[currentNode] + costMatrix[currentNode][i];
					if (newDistance < distance[i]) {
						distance[i] = newDistance;
						parent[i] = currentNode;
					}
				}
			}
		}
	}
};

int main() {
	int nodeCount = 4;
	LinkState ls(nodeCount);

	ls.updateCost(0, 1, 2);
	ls.updateCost(0, 2, 5);
	ls.updateCost(1, 2, 1);
	ls.updateCost(1, 3, 7);
	ls.updateCost(2, 3, 3);

	int sourceNode = 0;
	ls.calculateLinkState(sourceNode);
	ls.printRoutingTable();

	return 0;
}
