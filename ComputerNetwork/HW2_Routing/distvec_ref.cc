// distvec.cc - Distance Vector Routing Algorithm

#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

class DistanceVectorRouter {
private:
	int numNodes;
	vector<vector<int>> distanceTable;

public:
	DistanceVectorRouter(int n) : numNodes(n) {
		distanceTable.resize(numNodes, vector<int>(numNodes, INF));
		for (int i = 0; i < numNodes; ++i) {
			distanceTable[i][i] = 0;
		}
	}

	void addLink(int node1, int node2, int cost) {
		distanceTable[node1][node2] = cost;
		distanceTable[node2][node1] = cost;
	}

	void updateDistanceTable() {
		for (int k = 0; k < numNodes; ++k) {
			for (int i = 0; i < numNodes; ++i) {
				for (int j = 0; j < numNodes; ++j) {
					if (distanceTable[i][k] != INF && distanceTable[k][j] != INF &&
						distanceTable[i][k] + distanceTable[k][j] < distanceTable[i][j]) {
						distanceTable[i][j] = distanceTable[i][k] + distanceTable[k][j];
					}
				}
			}
		}
	}

	void printDistanceTable() {
		for (int i = 0; i < numNodes; ++i) {
			for (int j = 0; j < numNodes; ++j) {
				cout << distanceTable[i][j] << "\t";
			}
			cout << endl;
		}
	}
};

int main() {
	int numNodes = 4;
	DistanceVectorRouter router(numNodes);

	// Adding links and their costs
	router.addLink(0, 1, 1);
	router.addLink(0, 2, 5);
	router.addLink(1, 2, 2);
	router.addLink(1, 3, 3);
	router.addLink(2, 3, 1);

	// Updating the distance table
	router.updateDistanceTable();

	// Printing the distance table
	cout << "Distance Table:\n";
	router.printDistanceTable();

	return 0;
}
