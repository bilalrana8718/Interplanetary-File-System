#pragma once

#include <iostream>
#include <time.h>
#include <cmath> 
#include "HexToDec.h"
#include "SHA_1.h"
#include "BTree.h"
#include "BTreeNode.h"
#include "RoutingTable.h"

using namespace std;

class RoutingTable;
class DHT;
class Machine;

class RoutingTable
{

public:
	int machineNum;
	RandomNumberGen r;
	Machine** machines12;
	RoutingTable();
	RoutingTable(Machine* head, Machine* M, int m, int bits);
	void PrintRoutingTable(Machine* m);
};

class DHT
{
public:
	int treeWays;
	int numMachine;
	Machine* head;
	Machine* current;
	int bits;
	DHT();
	DHT(int bits, int ways, int num_mach);
	void makeDHT(string* machArray);
	void insert(string d);
	void display();
	void updateRouting();
	void insertFile(string path);
	void fileInsertion(string key, string path);
	void fileDeletion(string key, string path);
	Machine* Succ(string key);
	Machine* SearchNode(string inMach);
};

class Machine
{
public:

	BTree* tree;
	string key;
	string id;
	Machine* next;
	RoutingTable* t;
	Machine();
	Machine(Machine* head, string d, int ways, int m, int bits);

};



	Machine::Machine() {
	tree = NULL;
	next = NULL;
	t = NULL;
	}
	Machine::Machine(Machine* head, string d, int ways, int m, int bits)
	{
	tree = new BTree(ways);
	this->key = d;
	this->id = d;
	this->next = NULL;
	}


	DHT::DHT()
	{
		this->treeWays = 0;
		numMachine = 0;
		head = NULL;
		this->current = NULL;
		this->bits = 0;
	}

	DHT::DHT(int bits, int ways ,int num_mach)
	{
		this->treeWays = ways;
		numMachine = num_mach;
		head = NULL;
		this->current = NULL;
		this->bits = bits;
	}

	void DHT::makeDHT(string* machArray)
	{
		for (int i = 0; i < this->numMachine; i++)
		{
			DHT::insert(machArray[i]);
		}

		this->updateRouting();
	}

	void DHT::insert(string d)
	{
		Machine* temp = new Machine(head, d, this->treeWays, numMachine, bits);
		
		if (!head)
		{
			head = temp;
			head->next = head;
		}
		else if (head && head == head->next) {
			temp->next = head;
			head->next = temp;
			if (temp->id < head->id)
				head = temp;
		}
		else {
			Machine* ptr = head->next;
			Machine* prev = head;

			while (ptr != head)
			{
				prev = ptr;
				ptr = ptr->next;
			}

			if (ptr->id > d)
			{
				temp->next = ptr;
				prev->next = temp;
				head = temp;
			}
			else if (prev->id < d)
			{
				temp->next = ptr;
				prev->next = temp;
			}
			else {
				while (ptr->id < d)
				{
					prev = ptr;
					ptr = ptr->next;
				}
				temp->next = ptr;
				prev->next = temp;
			}

		}
	}
	

	void DHT::display()
	{
		cout << endl;
		Machine* ptr = head;
		int i = 1;
		while (i <= this->numMachine)
		{
			cout << "\t" << i << " . " << ptr->id << endl;
			ptr = ptr->next;
			i++;
		}
		ptr = head->next;

	}

	void DHT::updateRouting()
	{
		Machine* ptr = head;
		int i = 1;
		while (i <= this->numMachine)
		{
			ptr->t = new RoutingTable(this->head, ptr, this->numMachine, this->bits);
			ptr = ptr->next;
			i++;
		}
	}

	Machine* DHT::SearchNode(string inMach)
	{
		Machine* ptr = head;
		int i = 1;
		while (i <= this->numMachine)
		{
			if (inMach == ptr->id)
			{
				return ptr;
			}
			ptr = ptr->next;
			i++;
		}
		return NULL;
	}

	RoutingTable::RoutingTable()
	{
		machines12 = NULL;
		machineNum = 0;
	}
	RoutingTable::RoutingTable(Machine* head, Machine* M, int m, int bits)
	{
		machines12 = new Machine * [m];

		machineNum = m;
		for (int i = 0; i < m; i++)
		{
			string x1 = r.AddDectoHex(M->id, pow(2, i));
			x1 = r.mod(x1, bits);
			Machine* t = M;
			int j = 0;
			Machine *min = M;
			while (t->id >= x1 && j<=bits)
			{
				j++;
				t = t->next;
				if (t->id < min->id)
				{
					min = t;
				}
			}
			if (t->id > x1)
			{
				machines12[i] = min;
			}
			
			else if (t->id == x1)
			{
				machines12[i] = t;
			}
			else
			{
				while (t->next->id < x1 && t->next != head)
				{
					t = t->next;
				}
				machines12[i] = t->next;
			}
		}
	}

	void RoutingTable::PrintRoutingTable(Machine* m)
	{
		cout << endl << endl;
		cout << "	Routing Table for " << m->id << ":" << endl;

		for (int i = 0; i < machineNum; i++)
		{
			cout << "\t" << i + 1 << " - " << machines12[i]->id << endl;
		}
	}


	Machine* DHT::Succ(string key) {
		bool found = false;
		string traversal = current->id;

		std::cout <<endl << "	Key:  "<< key  << endl;


		while (!found)
		{
			if (key == current->id) {
				found = true;
			}
			else if (key > current->id && key <= current->t->machines12[0]->id)
			{
				current = current->t->machines12[0];
				std::cout << "	Accessing Machine : " << current->id << endl;
				traversal += " -> " + current->id;
				found = true;
			}
			else if (key > current->id && current->id > current->t->machines12[0]->id)
			{
				current = current->t->machines12[0];
				std::cout << "	Accessing Machine : " << current->id << endl;

				traversal += " -> " + current->id;
				found = true;
			}
			else if (/*key < current->id &&*/ current->id > current->t->machines12[0]->id && key <= current->t->machines12[0]->id) {
				current = current->t->machines12[0];
				std::cout << "	Accessing Machine: " << current->id << endl;

				traversal += " -> " + current->id;
				found = true;
			}
			else
			{
				bool currentChanged = false;
				Machine* max = current->t->machines12[0];
				Machine* min = current->t->machines12[0];
				for (int i = 0; i < current->t->machineNum - 1; i++)
				{
					if (key > current->t->machines12[i]->id && key < current->t->machines12[i + 1]->id)
					{
						current = current->t->machines12[i];
						currentChanged = true;
						std::cout << "	Accessing Machine : " << current->id << endl;
						traversal += " -> " + current->id;
						break;
					}
					if (current->t->machines12[i]->id > max->id)
					{
						max = current->t->machines12[i];

					}
					if (current->t->machines12[i]->id < min->id)
					{
						min = current->t->machines12[i];
					}
				}
				if (!currentChanged) {

					if (current->id > key /*&& current->next->id > key*/)
					{
						if (min->id >= key && min->id < current->id)
						{
							current = min;
							traversal += " -> " + current->id;
							std::cout << "	Accessing Machine : " << current->id << endl;
							found = true;
						}
						else
						{
							current = max;
							std::cout << "	Accessing Machine : " << current->id << endl;
							traversal += " -> " + current->id;
						}
					}
					else if (current->id < key && max->id < key)
					{
						current = max;
						std::cout << "	Accessing Machine : " << current->id << endl;
						traversal += " -> " + current->id;
					}
				}
			}
		}
		std::cout << "\t" << traversal << endl << endl;
		return current;
	}

	void DHT::insertFile(string path)
	{
		SHA1 temp;
		//bits = 10;
		string output = temp.from_file(path);
		int strlength = output.length();

		int index = strlength - 1 - ((bits - 1) / 4);
		string desiredString = output.substr(index, (bits - 1) / 4 + 1);

		int x = 0;

		for (int i = desiredString.length() - 1, j = 0; i >= 0; i--, j++)
		{
			if (desiredString[i] < 'a')
				x += (desiredString[i] - '0') * pow(16, j);
			else
				x += (desiredString[i] - 'a' + 10) * pow(16, j);
		}
		string binary = std::bitset<160>(x).to_string();
		string req = binary.substr(160 - 1 - bits, bits);
		char* d1 = new char[4 * (1 + ((bits - 1) / 4))];
		for (int i = 0; i < 4 * (1 + ((bits - 1) / 4)); i++)
		{
			d1[i] = '0';
		}
		for (int i = 4 * (1 + ((bits - 1) / 4)) - 1, j = req.length() - 1; j >= 0; j--, i--)
		{
			d1[i] = req[j];
		}
		string returnwali = "";
		for (int i = 0, z = 0; i < ((bits - 1) / 4) + 1; i++)
		{
			int y = 0;
			for (int j = 3; j >= 0; j--, z++)
			{
				y += (d1[z] - '0') * pow(2, j);
			}

			if (y < 10)
			{
				returnwali += to_string(y);
			}
			else
			{
				char hexChar = 'a' + (y - 10);
				returnwali += hexChar;
			}
		}

		//std::cout <<"\t" << returnwali << endl;

		fileInsertion(returnwali, path);
	}
	void DHT::fileInsertion(string key, string path) {
		Succ(key);  // updates current pointer, current pointer points to tree in which node needs to be added
		Nodedata* data = new Nodedata(key, path);

		current->tree->insert(*data);

	}

	void DHT::fileDeletion(string key, string path = "") {
		if (current->tree->deletion(Nodedata(key, path))) {
			return;
		}
		Succ(key);

		current->tree->deletion(Nodedata(key, path));
	}
