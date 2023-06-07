#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

#define TRUE 1
#define FALSE 0

using namespace std;

const int INF = numeric_limits<int>::max();

class LinkState {
private:
	int verticesNum;
	vector<int> found;
	vector<int> dist;
	vector<int> parent;
	vector<int> path;
	vector< vector<int> > cost;

	int choose(int verticesNum) {
		// find the smallest dist not yet checked
		int minDist = INF, minPos = -1;
		
		for (int i = 0; i < verticesNum; i++) {
			if (dist[i] < minDist && !found[i]) {
				minDist = dist[i];
				minPos = i;
			}
		}
		return minPos;
	}

public:
	LinkState(int v) : verticesNum(v) {
		cost.resize(v, vector<int>(v, INF));
		found.resize(v);
		dist.resize(v, INF);
		parent.resize(v, -1);
		path.resize(v);
	}

	void createGraph(ifstream &topologyfile) {
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
			cost[val[0]][val[1]] = val[2];
		}
	}

	void shortestPath(int vertice) {
		for (int i = 0; i < verticesNum; i++) {
			found[i] = FALSE;
			dist[i] = cost[vertice][i];
			if (dist[i] < INF) parent[i] = vertice;
		}

		found[vertice] = TRUE;
		dist[vertice] = 0;
		for (int i = 0; i < verticesNum - 1; i++) {
			int chosen = choose(verticesNum);
			if (chosen == -1) break;
			found[chosen] = TRUE;
			for (int j = 0; j < verticesNum; j++) {
				if (!found[j] && (chosen != j)) {
					if (dist[chosen] + cost[chosen][j] < dist[j]) {
						dist[j] = dist[chosen] + cost[chosen][j];
						parent[j] = chosen;
					}
				}
			}
		}
	}

	// void printRoutes(ofstream &outfile) {
	// 	for (int v = 0; v < verticesNum; ++v) {
	// 		for (int dest = 0; dest < verticesNum; ++dest)
	// 			// if (dist[dest] != INF)
	// 				outfile << dest << " " << dist[dest] << " " << dest << endl;
	// 		outfile << endl;
	// 	}
	// }

	void printRoutes(ofstream& outfile) {
		for (int v = 0; v < verticesNum; ++v) {
			for (int dest = 0; dest < verticesNum; ++dest) {
				if (v != dest) {
					outfile << v << " " << dest << " " << dist[dest];
					int pathCurr = dest;
					int pathCount = 0;
					while (parent[pathCurr] > -1) {
						path[pathCount++] = pathCurr;
						pathCurr = parent[pathCurr];
					}
					outfile << endl;
				}
			}
			outfile << endl;
		}
	}

	void printOutputs(ifstream &messagesfile, ofstream &outfile) {
		// int i, j, pathCurr, pathCount;
		// for (i = 0; i < verticesNum; i++) {
		// 	if (dist[i] != INF) {
		// 		cout << "SRC: " << vertice << ", DST: " << i << ", LENGTH: " << dist[i] << ", PATH: " << vertice;
				
		// 		// Print path
		// 		pathCurr = i;
		// 		pathCount = 0;
		// 		while (parent[pathCurr] > -1) {
		// 			path[pathCount++] = pathCurr;
		// 			pathCurr = parent[pathCurr];
		// 		}
		// 		for (j = pathCount - 1; j >= 0; j--)
		// 			cout << " " << path[j];
		// 		cout << endl;
		// 	}
		// 	else
		// 		cout << "SRC: " << vertice << ", DST: " << i << ", LENGTH: ---, PATH: -" << endl;
		// }

		int i = 0, j, pathCurr, pathCount;
		string line;
		while (getline(messagesfile, line)) {
			// Write src and dest on outfile
			int src = stoi(line.substr(0, line.find(" ")));
			int dest = stoi(line.substr(line.find(" ") + 1));
			outfile << "from " << src << " to " << dest;
			
			// Write cost and hops on outfile
			if (dist[i] == INF)
				outfile << " cost infinite hops unreachable ";
			else {
				outfile << " cost " << dist[dest] << " hops ";
				pathCurr = dest;
				pathCount = 0;
				while (parent[pathCurr] > -1) {
					path[pathCount++] = pathCurr;
					pathCurr = parent[pathCurr];
				}
				for (j = pathCount - 1; j >= 0; j--)
					outfile << " " << path[j];
			}

			// Write message on outfile
			int pos, curPos = 0;
			for (int i = 0; i < 2; i++) {
				pos = line.find(" ", curPos);
				curPos = pos + 1;
			}
			outfile << "message " << line.substr(curPos) << endl;
		}
		outfile << endl;
	}
};

int main(int ac, char *av[]) {
	// Deal with the input arguments
	if (ac != 4) {
		cout << "usage: linkstate topologyfile messagesfile changesfile" << endl;
		return 1;
	}
	ifstream topologyfile(av[1]);
	ifstream messagesfile(av[2]);
	ifstream changesfile(av[3]);
	if (!topologyfile.is_open() || !messagesfile.is_open() || !changesfile.is_open()) {
		cout << "Error: open input file." << endl;
		return 1;
	}
	ofstream outfile("output_ls.txt");
	if (!outfile.is_open()) {
		cout << "Error: open output file." << endl;
		return 1;
	}

	// Initialize variables
	string line;
	int verticesNum = 0;
	if (getline(topologyfile, line)) verticesNum = stoi(line);

	for (int vertice = 0; vertice < verticesNum; ++vertice) {
		LinkState router(verticesNum);
		router.createGraph(topologyfile);
		router.shortestPath(vertice);
		router.printRoutes(outfile);
		router.printOutputs(messagesfile, outfile);
	}

	// The end
	topologyfile.close();
	messagesfile.close();
	changesfile.close();
	outfile.close();
	cout << "Complete. Output file written to output_ls.txt." << endl;
	return 0;
}
