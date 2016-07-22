/*
  TRISQ
  http://www.codechef.com/problems/TRISQ

  pomysl:
  suma skonczonego ciagu arytmetycznego
  ilosc elementow ciagu: (b div 2)-1
*/

#include <iostream>

using namespace std;

int main()
{
  int t;
  int b, x;

    //speedup cin
  ios::sync_with_stdio(false);
  cin.tie(0);

  cin >> t;
  for( int i=0; i<t; i++){
    cin >> b;
    x = (b >> 1) - 1;
    x = x*(1 + x)/2;
    cout << x << endl;
  }

  return 0;
}
