#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_VERTICES 10000
#define MAX_EDGES 50000000
 
 /* global variables */
typedef struct {
	int src;
	int dest;
	int weight;
} Graph;

typedef struct {
	int* root;
	int* leaf;
} Set;

/* function prototypes */
Graph *initialize(int ac, char *av[], int *verticesNum, int *edgesNum);
Graph *kruskal(int verticesNum, int edgesNum, Graph *edges, int *disjointSet);
void printMST(int verticesNum, Graph *mst, int isConnected);

/* $start main */
int main(int ac, char *av[]) {
	int verticesNum, edgesNum, disjointSet;

	clock_t start = clock();
	Graph *edges = initialize(ac, av, &verticesNum, &edgesNum);
	Graph *mst = kruskal(verticesNum, edgesNum, edges, &disjointSet);
	printMST(verticesNum, mst, disjointSet); // isConnected 확인하기
	clock_t end = clock();
	
	printf("output written to hw3_result.txt\n");
	printf("running time: %.6lf seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
	free(edges);
	free(mst);
	return 0;
}
/* $end main */

Graph *initialize(int ac, char *av[], int *verticesNum, int *edgesNum) {
	// Invalid input arguments
	if (ac != 2) {
		printf("usage: ./hw3 input_filename\n"); exit(1);
	}

	// Open input file
	FILE *infile = fopen(av[1], "r");
	if (infile == NULL) {
		printf("The input file does not exist.\n"); exit(1);
	}
	// Read input files
	if (fscanf(infile, "%d", verticesNum) != 1 || fscanf(infile, "%d", edgesNum) != 1) {
		printf("The input file is not in the right format.\n"); exit(1);
	}
	Graph *edges = (Graph *)malloc(sizeof(Graph) * (*edgesNum));
	for (int i = 0; i < *edgesNum; i++) {
		Graph edge;
		if (fscanf(infile, "%d %d %d", &edge.src, &edge.dest, &edge.weight) != 3) {
			printf("The input file is not in the right format.\n"); exit(1);
		}
		edges[i] = edge;
	}
	fclose(infile);
	return edges;
}

/* $start union find */
static void initializeSet(Set *set, int numVertices) {
	set->root = malloc(numVertices * sizeof(int));
	set->leaf = malloc(numVertices * sizeof(int));

	for (int i = 0; i < numVertices; i++) {
		set->root[i] = i;
		set->leaf[i] = 0;
	}
}

static int findRoot(Set* set, int vertex) {
	if (set->root[vertex] != vertex)
		set->root[vertex] = findRoot(set, set->root[vertex]);
	return set->root[vertex];
}

void unionFind(Set* set, int vertex1, int vertex2) {
	int r1 = findRoot(set, vertex1);
	int r2 = findRoot(set, vertex2);

	if (r1 == r2) return;

	if (set->leaf[r1] < set->leaf[r2])
		set->root[r1] = r2;
	else if (set->leaf[r1] > set->leaf[r2])
		set->root[r2] = r1;
	else {
		set->root[r2] = r1;
		set->leaf[r1]++;
	}

}
/* $end union find */

/* $start Kruskal's algorithm */
static int compareEdges(const void *a, const void *b) {
	Graph *edgeA = (Graph *)a;
	Graph *edgeB = (Graph *)b;
	return (edgeA->weight - edgeB->weight);
}

Graph *kruskal(int verticesNum, int edgesNum, Graph *edges, int *disjointSet) {
	Set set;
	initializeSet(&set, verticesNum);

	// Sort edges by weight
	qsort(edges, edgesNum, sizeof(Graph), compareEdges);

	// Find mininum spanning tree
	int count = 0;
	Graph *mst = (Graph *)malloc(sizeof(Graph) * (verticesNum - 1));
	for (int i = 0; i < edgesNum; i++) {
		Graph edge = edges[i];
		int srcParent = findRoot(&set, edge.src);
		int destParent = findRoot(&set, edge.dest);
		// If the edge does not form a cycle, add it to the MST
		if (srcParent != destParent) {
			mst[count++] = edge;
			unionFind(&set, srcParent, destParent);
		}
	}

	// Check if all vertices are connected
	*disjointSet = 0;
	int root = findRoot(&set, 0);
	for (int i = 1; i < verticesNum; i++) {
		if (findRoot(&set, i) != root) {
			*disjointSet = 1;
			break;
		}
	}

	free(set.root);
	free(set.leaf);
	return mst;
}
/* $end Kruskal's algorithm */

void printMST(int verticesNum, Graph *mst, int disjointSet) {
	FILE *outfile = fopen("hw3_result.txt", "w");
	if (outfile == NULL) {
		printf("error: file open\n"); exit(1);
	}

	int totalCost = 0;
	for (int i = 0; i < verticesNum - 1; i++) {
		fprintf(outfile, "%d %d %d\n", mst[i].src, mst[i].dest, mst[i].weight);
		totalCost += mst[i].weight;
	}
	fprintf(outfile, "%d\n", totalCost);
	fprintf(outfile, "%s\n", disjointSet ? "DISCONNECTED" : "CONNECTED");
	fclose(outfile);
}
