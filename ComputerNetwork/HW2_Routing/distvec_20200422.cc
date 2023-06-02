#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

class DistanceVector {
private:
	int nodeNum;
	vector<vector<int> > table;
	vector<int> distance;
	vector<int> nextHop;

public:
	DistanceVector(int n) : nodeNum(n) {
		table.resize(n, vector<int>(n, INF));
		distance.resize(n, INF);
		nextHop.resize(n, -1);
		for (int i = 0; i < n; ++i) table[i][i] = 0;
	}

	void parseTopology(ifstream &topologyfile) {
		while (!topologyfile.eof()) {
			// Get one line from topologyfile
			string line;
			getline(topologyfile, line);

			// Parse the line in router table
			int pos, curPos = 0, val[3];
			for (int i = 0; (pos = line.find(" ", curPos)) != string::npos; i++) {
				val[i] = stoi(line.substr(curPos, pos - curPos));
				curPos = pos + 1;
			}
			table[val[0]][val[1]] = val[2];
			table[val[1]][val[0]] = val[2];
		}
	}

	void updateTable() {
		bool updated = false;
		while(!updated) {
			updated = false;
			for (int node = 0; node < nodeNum; ++node) {
				for (int dest = 0; dest < nodeNum; ++dest) {
					if (node == dest) continue;
					int minDistance = INF, minHop = -1;
					for (int neighbor = 0; neighbor < nodeNum; ++neighbor) {
						if (table[node][neighbor] != INF && distance[neighbor] + table[node][neighbor] < minDistance) {
							minDistance = distance[neighbor] + table[node][neighbor];
							minHop = neighbor;
						}
					}
					if (minDistance < distance[dest]) {
						distance[dest] = minDistance;
						nextHop[dest] = minHop;
						updated = true;
					}
				}
			}
		}
	}

	void writeMessages(ifstream &messagesfile) {
		ofstream outfile("output_dv.txt");
		string line;
		while (getline(messagesfile, line)) {
			// Write src and dest on outfile
			int src = stoi(line.substr(0, line.find(" ")));
			int dest = stoi(line.substr(line.find(" ") + 1));
			outfile << "from " << src << " to " << dest;
			
			// Write cost and hops on outfile
			// if (distance[dest] == INF || nextHop[dest] == -1)
			// 	outfile << " cost infinite hops unreachable ";
			// else {
			// 	outfile << " cost " << table[src][dest] << " hops ";
			// 	for (int node = src; node != dest; node = nextHop[node])
			// 		outfile << node << " ";
			// }

			// Write message on outfile
			int pos, curPos = 0;
			for (int i = 0; i < 2; i++) {
				pos = line.find(" ", curPos);
				curPos = pos + 1;
			}
			outfile << "message " << line.substr(curPos, pos - curPos) << endl;
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

	// Initialize variables
	int nodeNum = 0;
	string line;
	if (getline(topologyfile, line)) nodeNum = stoi(line);
	DistanceVector router(nodeNum);

	// Process distance vector program
	router.parseTopology(topologyfile);	// Set up network
	router.updateTable();				// Update routing table
	router.writeMessages(messagesfile); // Write messages on outfile

	// The end
	topologyfile.close();
	messagesfile.close();
	changesfile.close();
	cout << "Complete. Output file written to output_dv.txt." << endl;

	return 0;
}
