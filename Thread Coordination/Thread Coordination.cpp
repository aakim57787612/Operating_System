#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int battery = 0;
int aircraft = 0;
int propeller = 0;
int type;
int total = 0;
int t1total = 0;
int t2total = 0;
int t3total = 0;
bool t = false;
typedef struct my_data {
	int who;
	int b = 0;
	int a = 0;
	int p = 0;
} my_data;
pthread_mutex_t mutex;
void* child(void* data)
{
	my_data* input = (my_data *)data;
	int who = input->who;
	if (input->b == 1 && type == 1) {
		pthread_exit(NULL);
	}
	if (input->a == 1 && type == 2) {
		pthread_exit(NULL);
	}
	if (input->p == 1 && type == 3) {
		pthread_exit(NULL);
	}
	if (who == 1 && type == 2) {
		pthread_exit(NULL);
	}
	pthread_mutex_lock(&mutex);
	if (t == true) {
		pthread_mutex_unlock(&mutex);
		pthread_exit(NULL);
	}
	t = true;
	if (type == 1) {
		input->b = 1;
	}
	else if (type == 2) {
		input->a = 1;
	}
	else if (type == 3) {
		input->p = 1;
	}
	sleep(1);
	if (who == 1) {
		cout << "Producer 1 (aircraft): get ";
		if (type == 1) {
			battery++;
			cout << "battery\n";
		}
		else if (type == 2) {
			aircraft++;
			cout << "aircraft\n";
		}
		else if (type == 3) {
			propeller++;
			cout << "propeller\n";
		}
	}
	else if (who == 2) {
		cout << "Producer 2: get ";
		if (type == 1) {
			battery++;
			cout << "battery\n";
		}
		else if (type == 2) {
			aircraft++;
			cout << "aircraft\n";
		}
		else if (type == 3) {
			propeller++;
			cout << "propeller\n";
		}
	}
	else if (who == 3) {
		cout << "Producer 3: get ";
		if (type == 1) {
			battery++;
			cout << "battery\n";
		}
		else if (type == 2) {
			aircraft++;
			cout << "aircraft\n";
		}
		else if (type == 3) {
			propeller++;
			cout << "propeller\n";
		}
	}
	if (input->b == 1 && who == 1 && input->p == 1) {
		total++;
		input->b = 0;
		input->p = 0;
		aircraft++;
		t1total++;
		cout << "Producer 1 (aircraft): OK, " << t1total << " drone(s)\n";
	}
	else if (input->b == 1 && input->a == 1 && input->p == 1 && who != 1) {
		total++;
		input->b = 0;
		input->a = 0;
		input->p = 0;
		switch (who) {
		case 2:
			t2total++;
			cout << "Producer 2: OK, " << t2total << " drone(s)\n";
			break;
		case 3:
			t3total++;
			cout << "Producer 3: OK, " << t3total << " drone(s)\n";
			break;
		}
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
void* child1(void* data)
{
	if (total == 50) {
		pthread_exit(NULL);
	}
	int* type = (int *)data;
	switch (*type) {
	case 1:
		cout << "Dispatcher: battery\n";
		break;
	case 2:
		cout << "Dispatcher: aircraft\n";
		break;
	case 3:
		cout << "Dispatcher: propeller\n";
		break;
	}
	pthread_exit(NULL);
}
int main() {
	int features, random, i = 0, a;
	cin >> features >> random;
	if (features == 1) {
		cout << "I didn't do the Advanced Features!\n";
		exit(0);
	}
	if (features != 0) {
		cout << "This is not correct input!!!\n";
		exit(0);
	}
	pthread_t t0, t1, t2, t3;
	my_data data1, data2, data3;
	data1.who = 1;
	data2.who = 2;
	data3.who = 3;
	while (total < 50) {
		srand(random + i);
		type = rand() % (3 - 1 + 1) + 1;
		if (data1.b == 1 && data2.b == 1 && data3.b == 1 && type == 1) {
			i++;
			continue;
		}
		else if (data2.a == 1 && data3.a == 1 && type == 2) {
			i++;
			continue;
		}
		else if (data1.p == 1 && data2.p == 1 && data3.p == 1 && type == 3) {
			i++;
			continue;
		}
		pthread_mutex_init(&mutex, 0);
		pthread_create(&t0, NULL, child1, (void *)&type);
		pthread_create(&t1, NULL, child, (void *)&data1);
		pthread_create(&t2, NULL, child, (void *)&data2);
		pthread_create(&t3, NULL, child, (void *)&data3);
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		pthread_join(t3, NULL);
		pthread_join(t0, NULL);
		i++;
		t = false;
	}
	cout << "total propellers = " << propeller << endl;
	cout << "total batterys   = " << battery << endl;
	cout << "total aircrafts  = " << aircraft << endl;
	if (t1total >= t2total && t1total >= t3total) {
		cout << "Producer 1 drones number = " << t1total << endl;
		if (t2total >= t3total) {
			cout << "Producer 2 drones number = " << t2total << endl;
			cout << "Producer 3 drones number = " << t3total << endl;
		}
		else {
			cout << "Producer 3 drones number = " << t3total << endl;
			cout << "Producer 2 drones number = " << t2total << endl;
		}
	}
	else if (t2total >= t1total && t2total >= t3total) {
		cout << "Producer 2 drones number = " << t2total << endl;
		if (t1total >= t3total) {
			cout << "Producer 1 drones number = " << t1total << endl;
			cout << "Producer 3 drones number = " << t3total << endl;
		}
		else {
			cout << "Producer 3 drones number = " << t3total << endl;
			cout << "Producer 1 drones number = " << t1total << endl;
		}
	}
	else if (t3total >= t2total && t3total >= t1total) {
		cout << "Producer 3 drones number = " << t3total << endl;
		if (t2total > t1total) {
			cout << "Producer 2 drones number = " << t2total << endl;
			cout << "Producer 1 drones number = " << t1total << endl;
		}
		else {
			cout << "Producer 1 drones number = " << t1total << endl;
			cout << "Producer 2 drones number = " << t2total << endl;
		}
	}
	cout << "total drones number = " << total << endl;
	return 0;
}
