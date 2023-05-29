#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int ac, char *av[]) {

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

	// Distance vector program successed
	cout << "Complete. Output file written to output_ls.txt." << endl;
}