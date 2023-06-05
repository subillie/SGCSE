#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int UNLINKED = -999;
const int INF = numeric_limits<int>::max();

class DistanceVector {
private:
	int nodeNum;
	struct Node {
		vector<int> cost;
		vector<int> nextHop;
		Node(int node, int c, int h) : cost(node, c), nextHop(node, h) {}
		int operator[](int i) { return cost[i]; }
		int operator()(int i) { return nextHop[i]; }
		void update(int dest, int c, int h) {
			cost[dest] = c;
			nextHop[dest] = h;
		}
	};
	vector<Node> table;

public:
	DistanceVector(int n) : nodeNum(n) {
		// Initialize routing table
		table.resize(n, Node(n, UNLINKED, -1));
		// Initialize cost and nextHop of the node itself
		for (int i = 0; i < n; ++i)
			table[i].update(i, 0, i);
	}

	void parseTopology(ifstream& topologyfile) {
		// Get each line from topologyfile
		string line;
		while (getline(topologyfile, line)) {
			if (line.empty()) break;
			// Parse the line in router table
			int pos, curPos = 0, val[3];
			for (int i = 0; (pos = line.find(" ", curPos)) != string::npos; i++) {
				val[i] = stoi(line.substr(curPos, pos - curPos));
				curPos = pos + 1;
			}
			table[val[0]].update(val[1], val[2], val[1]);
			table[val[1]].update(val[0], val[2], val[0]);
		}
	}

	void printRoutes(ofstream &outfile) {
		for (int node = 0; node < nodeNum; ++node) {
			for (int dest = 0; dest < nodeNum; ++dest)
				// if (table[node][dest] != UNLINKED)
					outfile << dest << " " << table[node][dest] << " " << table[node](dest) << endl;
			outfile << endl;
		}
	}

	void updateTable() {
		bool updated = true; // Set to true to enter the loop initially
		while (updated) {
			updated = false;
			// For each node, update the cost and nextHop to each destination
			for (int node = 0; node < nodeNum; ++node) {
				for (int dest = 0; dest < nodeNum; ++dest) {
					// Skip the node itself
					if (node == dest) continue;

					// Find the minimum cost and nextHop to the destination
					int minCost = table[node][dest];
					int minHop = table[node](dest);

					// cout << "\n" << node << endl;
					// Iterate through all neighbors of the node
					for (int neighbor = 0; neighbor < nodeNum; ++neighbor) {
						if (table[node][neighbor] != UNLINKED || neighbor == dest) {
							int cost = table[node][neighbor] + table[neighbor][dest];
							// cout << "cost " << cost << " neighbor " << table[node][neighbor] << " dest " << table[neighbor][dest] << endl;
							if (cost < minCost) {
								minCost = cost;
								minHop = neighbor;
							}
						}
					}

					// Update the routing table if necessary
					if (minCost < table[node][dest]) {
						table[node].update(dest, minCost, minHop);
						cout << "Node " << node << " to " << dest << " updates cost to " << minCost << endl;
						updated = true;
					}
				}
			}
		}
	}

	void printOutputs(ifstream &messagesfile, ofstream &outfile) {
		string line;
		while (getline(messagesfile, line)) {
			// Write src and dest on outfile
			int src = stoi(line.substr(0, line.find(" ")));
			int dest = stoi(line.substr(line.find(" ") + 1));
			outfile << "from " << src << " to " << dest;
			
			// Write cost and hops on outfile
			if (table[src][dest] == UNLINKED || table[src](dest) == -1)
				outfile << " cost infinite hops unreachable ";
			else {
				outfile << " cost " << table[src][dest] << " hops ";
				vector<int> path;
				int node = src;
				while (node != dest) {
					path.push_back(node);
					node = table[node](dest);
				}
				path.push_back(dest);
				for (int i = 0; i < path.size(); ++i)
					outfile << path[i] << " ";
			}

			// Write message on outfile
			int pos, curPos = 0;
			for (int i = 0; i < 2; i++) {
				pos = line.find(" ", curPos);
				curPos = pos + 1;
			}
			outfile << "message " << line.substr(curPos) << endl;
		}
	}
};

int main(int ac, char* av[]) {
	// Deal with the input arguments
	if (ac != 4) {
		cout << "usage: distvec topologyfile messagesfile changesfile" << endl;
		return 1;
	}
	ifstream topologyfile(av[1]);
	ifstream messagesfile(av[2]);
	ifstream changesfile(av[3]);
	if (!topologyfile.is_open() || !messagesfile.is_open() || !changesfile.is_open()) {
		cout << "Error: open input file." << endl;
		return 1;
	}
	ofstream outfile("output_dv.txt");
	if (!outfile.is_open()) {
		cout << "Error: open output file." << endl;
		return 1;
	}

	// Initialize variables
	int nodeNum = 0;
	string line;
	if (getline(topologyfile, line)) nodeNum = stoi(line);
	DistanceVector router(nodeNum);

	// Execute cost vector program
	router.parseTopology(topologyfile); // Set up network
	router.printRoutes(outfile); // Print routing table
	router.updateTable(); // Update routing table
	router.printOutputs(messagesfile, outfile); // Write messages on outfile

	// The end
	cout << "Complete. Output file written to output_dv.txt." << endl;
	topologyfile.close();
	messagesfile.close();
	changesfile.close();

	return 0;
}
