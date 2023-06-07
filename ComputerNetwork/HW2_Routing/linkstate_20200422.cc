#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <sstream>

using namespace std;

class Graph {
	int verticesNum;
	vector<int> dist;
	vector<vector<int> > graph;

public:
	Graph(int vertices) {
		verticesNum = vertices;
		graph.resize(verticesNum, vector<int>(verticesNum, INT_MAX));
	}

	void addEdge(int u, int v, int w) {
		graph[u][v] = w;
		graph[v][u] = w;
	}

	int findMinDistance(vector<int> &dist, vector<bool> &visited) {
		int minDist = INT_MAX;
		int minIndex = -1;

		for (int v = 0; v < verticesNum; v++) {
			if (dist[v] < minDist && !visited[v]) {
				minDist = dist[v];
				minIndex = v;
			}
		}
		return minIndex;
	}

	vector<int> getShortestPath(vector<int> &parent, int v) {
		vector<int> path;
		while (v != -1) {
			path.insert(path.begin(), v);
			v = parent[v];
		}
		return path;
	}

	int getWeight(int u, int v) {
		return graph[u][v];
	}

	vector<int> dijkstra(int src, int dest) {
		vector<int> dist(verticesNum, INT_MAX);
		vector<bool> visited(verticesNum, false);
		vector<int> parent(verticesNum, -1);

		dist[src] = 0;
		for (int i = 0; i < verticesNum; i++) {
			// Find the vertex with the minimum distance
			int minDist = findMinDistance(dist, visited);
			visited[minDist] = true;

			// Update the distance of the adjacent vertices of the picked vertex
			for (int v = 0; v < verticesNum; v++) {
				if (!visited[v] && graph[minDist][v] != INT_MAX && dist[minDist] + graph[minDist][v] < dist[v]) {
					dist[v] = dist[minDist] + graph[minDist][v];
					parent[v] = minDist;
				}
			}
		}
		// Get the shortest path from src to dest
		vector<int> path = getShortestPath(parent, dest);
		return path;
	}
};

class LinkStateRouter {
private:
	int verticesNum;
	ifstream topologyfile;
	ifstream messagesfile;
	ifstream changesfile;
	ofstream outfile;
	vector<string> msg;
	vector<vector<int> > messages;
	Graph graph;

public:
	LinkStateRouter(char* av[]) : graph(0) {
		topologyfile.open(av[1]);
		messagesfile.open(av[2]);
		changesfile.open(av[3]);
		if (!topologyfile.is_open() || !messagesfile.is_open() || !changesfile.is_open()) {
			cout << "Error: open input file." << endl;
			exit(1);
		}
		outfile.open("output_ls.txt");
		if (!outfile.is_open()) {
			cout << "Error: open output file." << endl;
			exit(1);
		}

		verticesNum = 0;
		graph = readTopologyFile();
		messages = readMessagesFile();
	}

	Graph readTopologyFile() {
		string line;
		getline(topologyfile, line);
		stringstream(line) >> verticesNum;
		Graph graph(verticesNum);

		while (getline(topologyfile, line)) {
			stringstream ss(line);
			int u, v, w;
			if (ss >> u >> v >> w) {
				graph.addEdge(u, v, w);
			}
		}
		topologyfile.close();
		return graph;
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
		graph.addEdge(u, v, w);
	}


	void printRoutes() {
		for (int src = 0; src < verticesNum; src++) {
			for (int dest = 0; dest < verticesNum; dest++) {
				// Get the shortest path from src to dest
				vector<int> path = graph.dijkstra(src, dest);
				// Calculate the cost of the path
				int cost = 0;
				if (path.empty()) cost = INT_MAX;
				else
					for (size_t j = 0; j < path.size() - 1; ++j)
						cost += graph.getWeight(path[j], path[j + 1]);
				// Print the path
				if (cost < INT_MAX) {
					if (src == dest)
						outfile << src << " " << src << " " << 0 << endl;
					else
						outfile << dest << " " << path[1] << " " << cost << endl;
				}
			}
			outfile << endl;
		}
	}

	void printOutputs() {
		for (size_t i = 0; i < messages.size(); i++) {
			int src = messages[i][0];
			int dest = messages[i][1];
			vector<int> path = graph.dijkstra(src, dest);

			// Print the path
			outfile << "from " << src << " to " << dest;
			if (path.empty())
				outfile << " cost infinite hops unreachable" << endl;
			else {
				int cost = 0;
				for (size_t j = 0; j < path.size() - 1; ++j)
					cost += graph.getWeight(path[j], path[j + 1]);
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

int main(int ac, char* av[]) {
	// Initialize with the input arguments
	if (ac < 4) {
		cout << "Usage: ./linkstate_20200422 topology.txt messages.txt changes.txt" << endl;
		return 1;
	}
	LinkStateRouter router(av);
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

	cout << "Complete. Output file written to output_ls.txt." << endl;
	return 0;
}
