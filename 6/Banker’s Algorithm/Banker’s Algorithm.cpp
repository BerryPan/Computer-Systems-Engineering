#include<iostream>
#include<fstream>
using namespace std;

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);
bool safe(int customer_num, int request[]); 

void init() {
	cout << "input the available resources: " << endl;
	for (int i = 0; i < NUMBER_OF_RESOURCES; ++i)
		cin >> available[i];
	ifstream f("C:\\Users\\lenovo\\Desktop\\max.txt");
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
		for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
			f >> maximum[i][j];
	for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
		for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
			allocation[i][j] = 0;
			need[i][j] = maximum[i][j];
		}
}

int request_resources(int customer_num, int request[]) {
	int flag = 1;
	for (int i = 0; i < NUMBER_OF_RESOURCES; ++i)
		if (request[i] > need[customer_num][i] || request[i] > available[i])
			flag = 0;
	if(flag)
		if (!safe(customer_num, request)) {
			flag = 0;
		}
	return flag;
}

void release_resources(int customer_num, int release[]) {
	int flag = 1;
	for(int i=0;i<NUMBER_OF_RESOURCES;++i)
		if (release[i] > allocation[customer_num][i])
			flag = 0;
	if (flag) {
		cout << "release successfully" << endl;
		for (int i = 0; i < NUMBER_OF_RESOURCES; ++i)
			allocation[customer_num][i] -= release[i];
	}
	else
		cout << "release failed" << endl;
}

bool safe(int customer_num, int request[]) {
	int work[NUMBER_OF_RESOURCES];
	int finish[NUMBER_OF_RESOURCES];
	for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
		work[i] = available[i];
		finish[i] = false;
	}
	for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
		if (!finish[i] && need[customer_num][i] <= work[i]) {
			work[i] += allocation[customer_num][i];
			finish[i] = true;
		}
		else {
			for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
				if (!finish[i])
					return false;
				else
					return true;
			}
		}
	}
}

int main() {
	init();
	int request[NUMBER_OF_RESOURCES];
	int release[NUMBER_OF_RESOURCES];
	int customer;
	while (true) {
		char command[3];
		cin >> command;
		if (command[1] == 'Q') {
			cin >> customer;
			for (int i = 0; i < NUMBER_OF_RESOURCES; ++i)
				cin >> request[i];
			if (request_resources(customer, request)) {
				cout << "request succesfully" << endl;
				for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
					allocation[customer][i] += request[i];
					need[customer][i] -= request[i];
					available[i] -= request[i];
				}
			}
			else {
				cout << "request failed" << endl;
				continue;
			}
		}
		else if (command[1] == 'L'){
			cin >> customer;
			for (int i = 0; i < NUMBER_OF_RESOURCES; ++i)
				cin >> release[i];
			release_resources(customer, release);
		}
		else if (command[0] == '*') {
			cout << "Available" << endl;
			for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
				cout << available[j] << '\t';
			cout << endl;
			cout << "Maximum" << endl;
			for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
				for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
					cout << maximum[i][j] << '\t';
				cout << endl;
			}
			cout << "Allocation" << endl;
			for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
				for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
					cout << allocation[i][j] << '\t';
				cout << endl;
			}
			cout << "Need" << endl;
			for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
				for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
					cout << need[i][j] << '\t';
				cout << endl;
			}
		}
		else
			break;
	}
}