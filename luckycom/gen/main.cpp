#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main()
{
  srand(time(NULL));

  int n,k;
//  n = rand() % 1000 + 1;  //num of strings 1-1000
//  n = 7474;
//  k = rand() % 100 + 1; //k 1-100
  n = 2;
  k = 1000;

  cout << n << " " << k << endl;
  for( int i=0; i<n; i++ ){
//    int l = 10 + rand() % 90 + 1; //len of string, min 10
    int l = 50000;
    for( int x=0; x<l; x++){
//      int c = rand() % 3;
//      if(c==0)
//        cout << '4';
//      else if (c==1)
//        cout << '7';
//      else
        cout << '?';
    }
    cout << endl;
  }


    return 0;
}
