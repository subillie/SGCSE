#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <climits>
#include <sstream>

using namespace std;

class Table {
	int nodesNum;
	vector<int> dist;
	vector<vector<int> > table;

public:
	Table(int nodes) {
		nodesNum = nodes;
		table.resize(nodesNum, vector<int>(nodesNum, INT_MAX));
	}

	void addEdge(int u, int v, int w) {
		table[u][v] = w;
		table[v][u] = w;
	}

	int findMinDistance(vector<int> &dist, vector<bool> &visited) {
		int minDist = INT_MAX;
		int minIndex = -1;

		for (int n = 0; n < nodesNum; n++) {
			if (dist[n] < minDist && !visited[n]) {
				minDist = dist[n];
				minIndex = n;
			}
		}
		return minIndex;
	}

	vector<int> getShortestPath(vector<int> &parent, int n) {
		vector<int> path;
		while (n != -1) {
			path.insert(path.begin(), n);
			n = parent[n];
		}
		return path;
	}

	int getWeight(int u, int n) {
		return table[u][n];
	}

	vector<int> distvec(int src, int dest) {
		vector<int> dist(nodesNum, INT_MAX);
		vector<bool> visited(nodesNum, false);
		vector<int> parent(nodesNum, -1);

		dist[src] = 0;
		for (int i = 0; i < nodesNum; i++) {
			// Find the node with the minimum distance
			int minDist = findMinDistance(dist, visited);
			if (minDist >= 0) visited[minDist] = true;
			// Update the distance of the adjacent nodes of the picked node
			for (int n = 0; n < nodesNum; n++) {
				if (minDist >=0 && !visited[n] && table[minDist][n] != INT_MAX && dist[minDist] + table[minDist][n] < dist[n]) {
					dist[n] = dist[minDist] + table[minDist][n];
					parent[n] = minDist;
				}
			}
		}
		// Get the shortest path from src to dest
		vector<int> path = getShortestPath(parent, dest);
		return path;
	}
};

class DistanceVector {
private:
	int nodesNum;
	ifstream topologyfile;
	ifstream messagesfile;
	ifstream changesfile;
	ofstream outfile;
	vector<string> msg;
	vector<vector<int> > messages;
	Table table;

public:
	DistanceVector(char *av[]) : table(0) {
		topologyfile.open(av[1]);
		messagesfile.open(av[2]);
		changesfile.open(av[3]);
		if (!topologyfile.is_open() || !messagesfile.is_open() || !changesfile.is_open()) {
			cout << "Error: open input file." << endl;
			exit(1);
		}
		outfile.open("output_dv.txt");
		if (!outfile.is_open()) {
			cout << "Error: open output file." << endl;
			exit(1);
		}

		nodesNum = 0;
		table = readTopologyFile();
		messages = readMessagesFile();
	}

	Table readTopologyFile() {
		string line;
		getline(topologyfile, line);
		stringstream(line) >> nodesNum;
		Table table(nodesNum);

		while (getline(topologyfile, line)) {
			stringstream ss(line);
			int u, v, w;
			if (ss >> u >> v >> w)
				table.addEdge(u, v, w);
		}
		topologyfile.close();
		return table;
	}

	vector<vector<int> > readMessagesFile() {
		vector<vector<int> > messages;
		string line;

		while (getline(messagesfile, line)) {
			stringstream ss(line);
			int src, dest;
			if (ss >> src >> dest) {
				vector<int> message;
				message.push_back(src);
				message.push_back(dest);
				msg.push_back(line);
				messages.push_back(message);
			}
		}
		messagesfile.close();
		return messages;
	}

	vector<vector<int> > readChangesFile() {
		vector<vector<int> > changes;
		string line;

		while (getline(changesfile, line)) {
			stringstream ss(line);
			int u, v, w;
			if (ss >> u >> v >> w) {
				vector<int> change;
				change.push_back(u);
				change.push_back(v);
				change.push_back(w);
				changes.push_back(change);
			}
		}
		changesfile.close();
		return changes;
	}

	void applyChanges(vector<int> &changes) {
		int u = changes[0];
		int v = changes[1];
		int w = (changes[2] == -999) ? INT_MAX : changes[2];
		table.addEdge(u, v, w);
	}


	void printRoutes() {
		for (int src = 0; src < nodesNum; src++) {
			for (int dest = 0; dest < nodesNum; dest++) {
				// Get the shortest path from src to dest
				vector<int> path = table.distvec(src, dest);
				// Calculate the cost of the path
				int cost = 0;
				if (path[1] < 0 || path[1] >= nodesNum) cost = INT_MAX;
				else
					for (size_t j = 0; j < path.size() - 1; ++j)
						cost += table.getWeight(path[j], path[j + 1]);
				// Print the path
				if (src == dest)
					outfile << src << " " << src << " " << 0 << endl;
				else if (cost < INT_MAX)
						outfile << dest << " " << path[1] << " " << cost << endl;
			}
			outfile << endl;
		}
	}

	void printOutputs() {
		for (size_t i = 0; i < messages.size(); i++) {
			int src = messages[i][0];
			int dest = messages[i][1];
			vector<int> path = table.distvec(src, dest);

			// Print the path
			outfile << "from " << src << " to " << dest;
			if (path[1] < 0 || path[1] >= nodesNum)
				outfile << " cost infinite hops unreachable ";
			else {
				int cost = 0;
				for (size_t j = 0; j < path.size() - 1; ++j)
					cost += table.getWeight(path[j], path[j + 1]);
				outfile << " cost " << cost << " hops ";
				for (size_t k = 0; k < path.size() - 1; ++k)
					outfile << path[k] << " ";
			}

			// Print the message
			size_t pos, curPos = 0;
			string line = msg[i];
			for (size_t j = 0; j < 2; j++) {
				pos = line.find(" ", curPos);
				curPos = pos + 1;
			}
			outfile << "message " << line.substr(curPos) << endl;
		}
		outfile << endl;
	}
};

int main(int ac, char *av[]) {
	if (ac != 4) {
		cout << "Usage: ./distvec_20200422 topology.txt messages.txt changes.txt" << endl;
		return 1;
	}
	// Initialize with the input arguments
	DistanceVector router(av);
	vector<vector<int> > changes = router.readChangesFile();

	// Print the initial routing table
	router.printRoutes();
	router.printOutputs();

	// Apply changes
	for (size_t i = 0; i < changes.size(); i++) {
		router.applyChanges(changes[i]);
		router.printRoutes();
		router.printOutputs();
	}

	cout << "Complete. Output file written to output_dv.txt." << endl;
	return 0;
}
