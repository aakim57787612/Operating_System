#include <sys/types.h>  
#include <errno.h>     
#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>    
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <pthread.h>
#include <iomanip>
#include <map>
#include <time.h>
using namespace std;
int abc[100][100]; int e = 0, j = 0; int ma; double maxcos = 0;
void *thread_func(void *threadid)
{
	long tid; clock_t prev1 = clock();
	tid = (long)threadid;
	cout << "[Main thread] create TID:" << pthread_self() << ", DocID:";
	cout << setfill('0') << setw(4) << tid + 1;
	cout << "\n[TID=" << pthread_self() << "] DocID:";
	cout << setfill('0') << setw(4) << tid + 1 << " [";
	for (int k = 0; k < j; k++) {
		cout << abc[tid][k];
		if (k == j - 1)
			break;
		cout << ",";
	}
	cout << "]\n";
	double anss = 0;
	for (int k = 0; k < e; k++) {
		if (k == tid)
			continue;
		double x = 0, y = 0, z = 0, ans;
		cout << "[TID=" << pthread_self() << "] cosine(";
		cout << setfill('0') << setw(4) << tid + 1; cout << ",";
		cout << setfill('0') << setw(4) << k + 1; cout << ")=";
		for (int mm = 0; mm < j; mm++) {
			x += abc[tid][mm] * abc[k][mm];
			y += abc[tid][mm] * abc[tid][mm];
			z += abc[k][mm] * abc[k][mm];
		}
		y = sqrt(y);
		z = sqrt(z);
		ans = x / (y*z); anss += ans;
		cout << ans;
		cout << endl;
	}
	cout << "[TID=" << pthread_self() << "] Avg_cosine:";
	cout << anss / (e - 1) << endl;
	if (maxcos < anss / (e - 1)) {
		maxcos = anss / (e - 1);
		ma = tid + 1;
	}
	cout << "[TID=" << pthread_self() << "] CPU time:" << 1000 * ((double)(clock() - prev1) / CLOCKS_PER_SEC) << "ms\n";
	pthread_exit(NULL);
}
int main()
{
	clock_t prev = clock();
	string filename;
	cin >> filename;
	ifstream inFile(filename, ios::in);
	string a, array1[100][100]; char words[100][20] = {};
	while (!inFile.eof()) {
		int i = 0; int f = 0;
		getline(inFile, a);
		getline(inFile, a);
		while (a[i] != '\0') {
			char word[20] = {}; int p = 0; bool o = false;
			while (a[i] != ' ') {
				word[p] = a[i];
				p++;
				if (a[i] == '\0') {
					o = true;
					break;
				}
				i++;
			}
			array1[e][f] = word;
			f++;
			int aa = 0;
			for (int k = 0; k < j; k++) {
				for (int m = 0; m < 20; m++) {
					if (word[m] != words[k][m]) {
						aa++;
						break;
					}
				}
			}
			if (aa == j) {
				for (int m = 0; m < 20 && word[m] != '\0'; m++) {
					words[j][m] = word[m];
				}
				j++;
			}
			i++;
			if (o == true)
				break;
		}
		e++;
	}
	e--;
	for (int q = 0; q < e; ++q) {
		for (int m = 0; m < j; ++m) {
			for (int n = 0; n < j; ++n) {
				if (array1[q][m] == words[n])
					abc[q][n]++;
			}
		}
	}
	pthread_t thread[e];
	for (long xx = 0; xx < e; xx++) {
		pthread_create(&thread[xx], NULL, thread_func, (void *)xx);
		sleep(1);
	}
	for (long xx = 0; xx < e; xx++) {
		pthread_join(thread[xx], NULL);
	}
	cout << "[Main thread] KeyDocID:";
	cout << setfill('0') << setw(4) << ma;
	cout << " Average Cosine:" << maxcos << endl;
	cout << "[Main thread] CPU time:";
	cout << 1000 * ((double)(clock() - prev) / CLOCKS_PER_SEC) << "ms\n";
} 