#pragma once
#include <iostream>
using namespace std;

struct Nodedata {
	string key;
	string path;
	Nodedata(string k = "", string p = "") {
		key = k;
		path = p;
	}

};

class BTreeNode {
public:
	int numOfKeys, order;
	Nodedata* keys;
	BTreeNode** child, * parent;
	bool leaf;

	BTreeNode(int ord, BTreeNode* Parent = nullptr, bool isleaf = true) {
		parent = Parent;
		order = ord;
		keys = new Nodedata[order];
		child = new BTreeNode * [order + 1];
		for (int i = 0; i < order; i++)
			child[i] = nullptr;
		leaf = isleaf;
		numOfKeys = 0;
	}

	void insert(Nodedata data, BTreeNode* nodesChild = nullptr) {
		int ind = numOfKeys++;
		while (ind > 0) {
			if (keys[ind - 1].key < data.key) {
				break;
			}
			keys[ind] = keys[ind - 1];
			child[ind + 1] = child[ind];
			ind--;
		}
		keys[ind] = data;
		child[ind + 1] = nodesChild;
		if (nodesChild)
			leaf = false;

	}
	void insertkey(Nodedata data) {
		int ind = numOfKeys++;
		while (ind > 0) {
			if (keys[ind - 1].key < data.key) {
				break;
			}
			keys[ind] = keys[ind - 1];
			ind--;
		}
		keys[ind] = data;
	}
	void remove(Nodedata data) {
		int i = 0;
		for (; i < numOfKeys; i++) {
			if (keys[i].key == data.key)
				break;
		}
		numOfKeys--;
		while (i < numOfKeys) {
			keys[i] = keys[i + 1];
			i++;
		}

	}

	bool isFull() {
		return numOfKeys >= order;
	}

	void print() {
		cout <<"\tTotal Keys In Node : " << numOfKeys << endl << "\tKeys : ";
		for (int i = 0; i < numOfKeys; i++) {
			cout <<"\t" << keys[i].key << "   ";
		}
		cout << endl;
	}
};