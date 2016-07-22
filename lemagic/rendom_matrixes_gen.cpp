#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

const int cnt = 10;
const int m = 4;
const int n = 4;

int main() {
	srand (time(NULL));
	cout << cnt << endl;
	for(int i=0; i<cnt; i++){
		cout << n << " " << m << endl;
		for(int j=0; j<m; j++){
			for(int k=0; k<n; k++){
				cout << rand() % 10;
			}
			cout << endl;
		}
	}

	return 0;
}