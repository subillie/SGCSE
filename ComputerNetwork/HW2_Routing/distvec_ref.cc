// distvec.cc

#include <iostream>
#include <vector>
#include <limits>

using namespace std;

struct DistanceVector {
	int nodeCount;
	vector<vector<int> > costMatrix;
	vector<int> distance;
	vector<int> nextHop;

	DistanceVector(int n) : nodeCount(n) {
		costMatrix.resize(n, vector<int>(n, numeric_limits<int>::max()));
		distance.resize(n, numeric_limits<int>::max());
		nextHop.resize(n, -1);
		for (int i = 0; i < n; ++i)
			costMatrix[i][i] = 0;
	}

	void updateCost(int from, int to, int cost) {
		costMatrix[from][to] = cost;
		costMatrix[to][from] = cost;
	}

	void printRoutingTable() {
		cout << "Routing Table:\n";
		for (int i = 0; i < nodeCount; ++i) {
			cout << "Node " << i << ": ";
			cout << "Distance = " << distance[i] << ", Next Hop = " << nextHop[i] << "\n";
		}
		cout << "\n";
	}

	void calculateDistanceVector(int sourceNode) {
		distance[sourceNode] = 0;

		// Initialize distance vector
		for (int i = 0; i < nodeCount; ++i) {
			if (i != sourceNode && costMatrix[sourceNode][i] != numeric_limits<int>::max()) {
				distance[i] = costMatrix[sourceNode][i];
				nextHop[i] = i;
			}
		}

		// Bellman-Ford algorithm
		for (int k = 0; k < nodeCount; ++k) {
			for (int i = 0; i < nodeCount; ++i) {
				for (int j = 0; j < nodeCount; ++j) {
					if (distance[i] != numeric_limits<int>::max() &&
						costMatrix[i][j] != numeric_limits<int>::max() &&
						distance[i] + costMatrix[i][j] < distance[j]) {
						distance[j] = distance[i] + costMatrix[i][j];
						nextHop[j] = nextHop[i];
					}
				}
			}
		}
	}
};

int main() {
	int nodeCount = 4;
	DistanceVector dv(nodeCount);

	dv.updateCost(0, 1, 2);
	dv.updateCost(0, 2, 5);
	dv.updateCost(1, 2, 1);
	dv.updateCost(1, 3, 7);
	dv.updateCost(2, 3, 3);

	int sourceNode = 0;
	dv.calculateDistanceVector(sourceNode);
	dv.printRoutingTable();

	return 0;
}
