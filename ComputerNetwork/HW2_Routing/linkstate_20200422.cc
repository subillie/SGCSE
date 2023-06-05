#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define MAX_VERTICES 10000
#define INF 999999999
#define TRUE 1
#define FALSE 0

int verticesNum;
int edgesNum;
int distance[MAX_VERTICES];
int parent[MAX_VERTICES];
int path[MAX_VERTICES];

class LinkState {
private:
	int cost[MAX_VERTICES][MAX_VERTICES];
	int found[MAX_VERTICES];

public:
	void createGraph(ifstream &topologyfile) {
		// cost adjacency matrix
		for(int i = 0; i < verticesNum; i++)
			for(int j = 0; j < verticesNum; j++)
				cost[i][j] = INF;

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

	int choose() {
		/* find the smallest distance not yet checked */
		int min = INF, minpos = -1;
		
		for (int i = 0; i < verticesNum; i++) {
			if (distance[i] < min && !found[i]) {
				min = distance[i];
				minpos = i;
			}
		}
		return minpos;
	}

	void shortestPath() {
		int vertice = 0;

		for (int i = 0; i < verticesNum; i++) {
			found[i] = FALSE;
			distance[i] = cost[vertice][i];
			if (distance[i] < INF) parent[i] = vertice;
		}

		found[vertice] = TRUE;
		distance[vertice] = 0;
		for (int i = 0; i < verticesNum - 2; i++) {
			int chosen = choose();
			if (chosen == -1) break;
			found[chosen] = TRUE;
			for (int j = 0; j < verticesNum; j++) {
				if (!found[j] && (chosen != j)) {
					if (distance[chosen] + cost[chosen][j] < distance[j]) {
						distance[j] = distance[chosen] + cost[chosen][j];
						parent[j] = chosen;
					}
				}
			}
		}
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

	// Initialize variables
	int i, j, path_curr, path_count, nodeNum = 0;
	string line;
	if (getline(topologyfile, line)) nodeNum = stoi(line);
	LinkState router;

	ifstream tmpfile(av[1]);
	if (!tmpfile.is_open())	{
		cout << "Error: open file." << endl;
		return 1;
	}
	int count = 0;
	while (getline(tmpfile, line)) ++count;
	verticesNum = count - 1;

	router.createGraph(topologyfile);
	for (i = 0; i < verticesNum; i++) {
		distance[i] = INF;
		parent[i] = -1;
	}
	router.shortestPath();

	for (i = 0; i < verticesNum; i++) {
		if(distance[i] != INF) {
			// printf("SRC: %d, DST: %d, LENGTH: %3d, PATH: %d", v, i, distance[i], v);
			
			/* print path */
			path_curr = i;
			path_count = 0;
			while (parent[path_curr] > -1) {
				path[path_count++] = path_curr;
				path_curr = parent[path_curr];
			}
			// for (j = path_count - 1; j >= 0; j--)
				// printf(" %d", path[j]);
			// printf("\n");
		}
		// else
		// 	printf("SRC: %d, DST: %d, LENGTH: ---, PATH: -\n", v, i);
	}

	// Distance vector program successed
	cout << "Complete. Output file written to output_ls.txt." << endl;
}
