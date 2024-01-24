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
#include "InterPlanetrySystem.h"
using namespace std;
#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define BRIGHT_BLACK "\033[1;30m"
#define BRIGHT_WHITE "\033[1;37m"
#define RESET "\033[0m"

void clrscr();

int main()
{
	int bits = 0;
	int machines = 0;
	int treeWay = 0;
	int choice = 100;
	

	srand(time(0));
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	// 96 for gold 
	SetConsoleTextAttribute(h, 96);
	clrscr();
	SetConsoleTitle(L"InterPlanetary File System");

	COORD coord;
	coord.X = 44; 
	coord.Y = 10; 
	SetConsoleCursorPosition(h, coord);
	cout << "Interplanetary File System";

	coord.X = 41;  // X coordinate
	coord.Y = 24;
	SetConsoleCursorPosition(h, coord);
	system("pause");
	clrscr();

	SetConsoleTextAttribute(console_color, 96);
	cout << endl << endl;
	cout << "	Interplanetary File System";
	cout << endl << endl;
		cout <<"	Enter the number of bits : ";
		cin >> bits;
		while (bits < 0 || bits >160)
		{
			cout << "	Invalid Bit Input - Enter Again : ";
			cin >> bits;
		}
		cout << endl;
		cout << "	Enter the number of machines in working : ";
		cin >> machines;
		while (machines < 0)
		{
			cout << "	Invalid Machine Input - Enter Again : ";
			cin >> machines;
		}
		cout << endl;
		cout << "	Enter Order of machines : ";
		cin >> treeWay;
		while (treeWay < 0)
		{
			cout << "	Invalid Order Input - Enter Again : ";
			cin >> treeWay;
		}

		system("cls");
		cout << endl << endl;

		InterPlanetarySystem IPS(bits, machines, treeWay);
		IPS.AssignIDS();
		system("cls");
		IPS.SelectMachine();
		system("cls");

		while (choice) 
		{
			clrscr();
			cout << endl<<endl;
			cout << "	Interplanetary File System";
			cout << endl << endl;
			cout << "	Current Machine : " << IPS.dht.current->id<< endl << endl;
			cout << "	1. Insert File" << endl;
			cout << "	2. Remove File" << endl;
			cout << "	3. Routing Table" << endl;
			cout << "	4. Change Machine" << endl;
			cout << "	5. Add Machine" << endl;
			cout << "	6. Remove Machine" << endl;
			cout << "	7. Print Tree" << endl;
			cout << "	8. Exit" << endl;
			cout << endl << "\t";
			cin >> choice;

			switch(choice)
			{
			case 1: {
				// insert file
				clrscr();
				string filePath;
				fstream fileObject;
				cout << endl << endl;
				std::cout << "	InterPlanetary System - Enter File Path : ";
				cin >> filePath;
				fileObject.open(filePath, ios::in);

				while (!fileObject.is_open()) {
					std::cout << "	Invalid file Path, Enter again: ";
					cin >> filePath;
					fileObject.open(filePath, ios::in);
				}
				fileObject.close();
				IPS.dht.insertFile(filePath);
				cout << endl << "\t";
				system("pause");
				break;
			}
			case 2: {
				// delete file
				clrscr();
				string k;
				cout << endl << endl;
				std::cout << "	Enter File Key For Deletion : ";
				cin >> k;
				cout << endl;
				IPS.dht.fileDeletion(k);
				cout << endl << "\t";
				system("pause");
				break;
			}
				case 3: {
					// routing table
					clrscr();
					IPS.dht.current->t->PrintRoutingTable(IPS.dht.current);
					cout << endl << "\t";
					system("pause");
					break;
				}
				case 4: {
					// change machine
					clrscr();
					IPS.SelectMachine();
					cout << endl << "\t";
					system("pause");
					break;
				}
				case 5: {
					// add machine
					clrscr();
					IPS.AddMachine();
					clrscr();
					cout << endl << "\n\tMachine Added Succesfully\n";
					IPS.dht.display();
					cout << endl << endl << "\t";
					system("pause");
					break;
				}
				case 6: {
					// remove machine
					clrscr();
					IPS.RemoveMachine();
					if (IPS.NumOfMachines > 1) 
					{
						clrscr();
						cout << endl << "\n\tMachine Removed Succesfully\n";
					}
					IPS.dht.display();
					cout << endl << endl << "\t";
					system("pause");
					break;
				}
				case 8: {
					// exit
					choice = 0;
					break;
				}
				case 7: {
					//print tree
					clrscr();
					cout << endl << endl;
					cout << "	Tree Of Machine : "<< IPS.dht.current->id<< endl;
					IPS.dht.current->tree->print();
					IPS.dht.current->tree->inOrder();
					cout << endl << "\t";
					system("pause");
					break;
				}
				default: {
					cout << "	Invalid Input"<< endl;
					break;

				}

			}
		}

		cout << endl << endl << "\t";
		system("pause");
		clrscr();

	return 0;
}


void clrscr()
{
	system("cls");
}