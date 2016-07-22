#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main()
{
  srand(time(NULL));

  int n;
  n = 2500;

  const int imin = -9999;
  const int imax = 9999;

  cout << n << endl;
  for( int i=0; i<n; i++ ){
    for( int j=0; j<n; j++){
        int c = imin + (rand() % (int)(imax - imin + 1));
        cout << c << " ";
    }
    cout << endl;
  }

  return 0;
}
