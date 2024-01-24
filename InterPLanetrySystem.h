#pragma once

#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <iomanip>
#include "DHT.h"
#include <conio.h>
#include <time.h>
#include <cmath>
#include <string>
#include "RoutingTable.h"

using namespace std;

class InterPlanetarySystem
{
public:
	int NumOfBits = 0;
	int NumOfMachines = 0;
	int OrderOfTree = 0;
	DHT dht;

	InterPlanetarySystem(int bits, int machines, int order)
	{
		this->NumOfBits = bits;
		this->NumOfMachines = machines;
		this->OrderOfTree = order;
		dht = DHT(bits, this->OrderOfTree, this->NumOfMachines);
	}

	void manualAssign(string*& machArray, int machines, int bits);
	void SelectRandom(string*& machArray, int machines, int bits);
	
	void AssignIDS();
	void SelectMachine();

	void AddMachine();
	void MachineAutoInput();
	void MachineManualInput();
	void RemoveMachine();
	
};


void InterPlanetarySystem::manualAssign(string*& machArray, int machines, int bits)
{
	int k = 0;
	int orgBits = bits;


	for (int i = 0; i < machines; i++)
	{
		machArray[i] = "";
	}
	RandomNumberGen s;
	for (int i = 0; i < machines; i++)
	{
		bool entered = false;
		while (!entered)
		{
			string tp = "";
			string x = s.findTheMax(bits);
			//cout << x << endl;
			cout << endl;
			cout << "	Enter a hexa number for in Range (0 - " << x << ") for machine "<< i+1 <<" : ";
			if (i == 0)
				cin.ignore();
			getline(cin, tp);
			

			while (x.length() < tp.length() || tp[0]=='-' || x.length() > tp.length())
			{
				cout << "	The Entered Bit is out of Bit Limit\n";
				cout << "	Enter again = ";
				getline(cin, tp);
			}
			while (x.length() == tp.length() && x < tp)
			{
				cout << "	The Entered Bit Exceeds Bit Limit\n";
				cout << "	Enter again = ";
				getline(cin, tp);
			}
			
			bool exist = false;
			for (int j = 0; j < machines; j++)
			{
				if (tp == machArray[j])
				{
					exist = true;
					break;
				}
			}
			if (!exist)
			{
				machArray[i] = tp;
				entered = true;
			}
		}
	}
}

void InterPlanetarySystem::SelectRandom(string*& machArray, int machines, int bits)
{
	int k = 0;
	RandomNumberGen r;
	for (int i = 0; i < machines; i++)
	{
		machArray[i] = "";
	}
	for (int i = 0; i < machines; )
	{
		string tp = r.getID(bits);
		bool entered = false;
		int k = i;
		while (!entered)
		{
			bool exist = false;
			for (int j = 0; j < k; j++)
			{
				if (tp == machArray[j])
				{
					exist = true;
					entered = true;
					break;
				}
			}
			if (!exist)
			{
				machArray[i++] = tp;
				entered = true;
				k++;
			}
		}
	}
	
}

void InterPlanetarySystem::AssignIDS()
{
	int choice = 0;
	string* machArray = new string[this->NumOfMachines];

	cout << "	1. Assign IDs Manually\n	2. Assign IDs Automaticallly\n\t";
	cin >> choice;

	switch (choice)
	{
	case 1: {
		manualAssign(machArray, this->NumOfMachines, this->NumOfBits);
		dht.makeDHT(machArray);
		break;
	}
	case 2: {
		SelectRandom(machArray, this->NumOfMachines, this->NumOfBits);
		dht.makeDHT(machArray);
		break;
	}
	default: cout << "	Invalid Input\n";
	}
	cout << endl;
	dht.display();
	cout << endl;
}

void InterPlanetarySystem::SelectMachine()
{
	cout << endl << endl;
	cout << "	Machines In Working	\n";
	this->dht.display();
	cout << endl << endl;

	string inMach = "";
	cout << "	Enter the Machine to Access : ";
	cin.ignore();
	getline(cin, inMach);

	Machine* ptr = dht.SearchNode(inMach);
	if (ptr) {
		dht.current = ptr;
	}
	else {
		while (!ptr)
		{
			inMach = "";
			cout << "	Invalid Machine Input - Enter again : ";
			getline(cin, inMach);
			ptr = dht.SearchNode(inMach);
		}
		dht.current = ptr;
	}
}

void InterPlanetarySystem::AddMachine()
{
	int choice = 0;
	cout << endl << endl;
	cout << "	1. Assign ID Manually\n	2. Assign ID Automaticallly\n\t";
	cin >> choice;

	switch (choice)
	{
	case 1: {
		MachineManualInput();
		break;
	}
	case 2: {
		MachineAutoInput();
		break;
	}
	default: cout << "	Invalid Input\n";
	}
	
	this->NumOfMachines++;
	this->dht.numMachine++;
	this->dht.updateRouting();
}

void InterPlanetarySystem::MachineAutoInput()
{
	string* machArray = new string[1];
	SelectRandom(machArray, 1, this->NumOfBits);
	Machine* ptr = this->dht.SearchNode(machArray[0]);

	if (ptr)
	{
		// Checks if the machine has unique id
		while (ptr)
		{
			SelectRandom(machArray, 1, this->NumOfBits);
			ptr = this->dht.SearchNode(machArray[0]);
		}
	}
	
	// Machine insertion in DHT
	this->dht.insert(machArray[0]);
	// Returning the Machine for Tree Traversal 
	ptr = this->dht.SearchNode(machArray[0]);

	// insert code tree traversal 
	if(ptr)
	ptr->tree->transferFrom(*ptr->next->tree, ptr->id,ptr->next->id);
}

void InterPlanetarySystem::MachineManualInput()
{
	string* machArray = new string[1];
	manualAssign(machArray, 1, this->NumOfBits);
	Machine* ptr = this->dht.SearchNode(machArray[0]);

	if (ptr)
	{
		// Checks if the machine has unique id
		while (ptr)
		{
			manualAssign(machArray, 1, this->NumOfBits);
			ptr = this->dht.SearchNode(machArray[0]);
		}
	}

	// Machine insertion in DHT
	this->dht.insert(machArray[0]);
	// Returning the Machine for Tree Traversal 
	ptr = this->dht.SearchNode(machArray[0]);

	// insert code tree traversal
	if(ptr)
	ptr->tree->transferFrom(*ptr->next->tree, ptr->id,ptr->next->id); 
}

void InterPlanetarySystem::RemoveMachine() 
{
	cout << endl << endl;
	cout << "	Machines In Working	\n";
	this->dht.display();
	cout << endl << endl;

	if (this->NumOfMachines > 1)
	{
		string inMach = "";
		cout << "	Enter the Machine to Remove : ";
		cin.ignore();
		getline(cin, inMach);

		Machine* ptr = dht.SearchNode(inMach);
		if (ptr) {
			dht.current = ptr;
		}
		else {
			inMach = "";
			cout << "	Invalid Machine Input - Machine Not Active" << endl;
			return;
		}

		// transfers the ptr data to the next node 
		ptr->next->tree->transferFrom(*ptr->tree, ptr->id, ptr->id);

		Machine* prev = ptr->next;
		while (prev->next != ptr)
		{
			prev = prev->next;
		}
		if (prev->next == ptr)
		{
			prev->next = prev->next->next;
			this->dht.current = prev;
			if (this->dht.head == ptr)
			{
				this->dht.head = prev->next;
			}
		}

		this->NumOfMachines--;
		this->dht.numMachine--;
		this->dht.updateRouting();
	}
	else {
		cout << endl << endl;
		cout << "\tLast Machine can not be Removed\n";
	}

	

}