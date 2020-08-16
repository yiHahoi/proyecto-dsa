#include <bits/stdc++.h>
#include "utils.h"
#include "quad_tree.h"

using namespace std;

int main(int argc, char const *argv[])
{
	// auto data = read_data("worldcitiespop_fixed.csv");
	auto data = read_data("test.csv");

	dsa::quad_tree qtree(data);
	cout << qtree.size << endl;

	// qtree.bfs();

	// cin.get();
	qtree.print();

	// auto v = qtree.find({40.920404, 39.19209})->val();
	// cout << v << endl;

	return 0;
}
