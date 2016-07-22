/*
  MISSP
  http://www.codechef.com/problems/MISSP

  pomysl:
  jezeli tylko jedna liczba jest bez pary to problem rozwiazuje XOR wszsytkich elementow w kolejnosci wczytania - zostanie tylko liczba ktora wystapila raz
  wszystkie pary zostawia bity ktore byly przed nia ustawione bez wzgledu na kolejnosc liczb
*/
#include <iostream>

using namespace std;

int main()
{
  int cases;
  int n;
  unsigned int x, a;

    //speedup cin
  ios::sync_with_stdio(false);
  cin.tie(0);

  cin >> cases;
  for( int i=0; i<cases; i++ ){
    cin >> n;
    x = 0;
    for( int j=0; j<n; j++ ){
      cin >> a;
      x ^= a;
    }
    cout << x << endl;
  }

    return 0;
}
