/*
  TWINRO
  http://www.codechef.com/problems/TWINRO

  pomysl (podobny do integral image):
  - pierwszy robot porusza sie w lewo i dol, drugi w dol i prawo, wiec wspolrzedne punktow w ktorych w danym momencie sa to (x,y) i (y,x)
  - trzeba stowrzyc macierz z sumowanymi wartosciami robotow w kazdym punkcie i w niej przejsc z lewego gornego rogu do prawego dolnego
  - macierz zsumowanych wartosci moze byc wirtualna - wyliczana na biezaco z macierzy wejsciowej, bo kazdy punkt potrzebny jest tylko raz
  - potrzebne sa wartosci z poprzedniego wiersza, wiec dodatkowo wystarczy tablica z n elementami
  - liczymy sumy w pierwszym wierszu
  - dla kolejnego wiersza liczymy pierwszy element sumowany z pierwszym z tempa i potem dla kazdego wartosc biezaca + max( lewy, gorny )
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

int n;
vector< vector<int> > tbl;
vector<int> tmp;

inline int sum(int x, int y){
  return tbl[y][x] + tbl[x][n-1-y];
}

int main()
{
  ifstream fin("d:\\projects\\codechef\\twinro\\gen\\bin\\Debug\\test_03.txt");
  cin.rdbuf(fin.rdbuf());

    //speedup cin
//  ios::sync_with_stdio(false);
//  cin.tie(0);

  clock_t tbegin, pbegin, tend;
  tbegin = clock();

    //read
  cin >> n;
  tbl.resize(n, vector<int>(n));
  tmp.resize(n);
  for( int i=0; i<n; i++ )
    for( int j=0; j<n; j++ ){
      cin >> tbl[i][j];
    }


  pbegin = clock();
    //first row
  tmp[0] = sum(0,0);
  for( int x=1; x<n; x++)
    tmp[x] = tmp[x-1] + sum(x, 0);

    //next rows
  for( int y=1; y<n; y++){
    tmp[0] = tmp[0] + sum(0, y);
    for(int x=1; x<n; x++){
      tmp[x] = sum(x,y) + max(tmp[x-1], tmp[x]);
    }
  }

    //result
  cout << tmp[n-1] << endl;

  tend = clock();
  double elapsed_secs = double(tend - tbegin) / CLOCKS_PER_SEC;
  cout << "elapsed: " << elapsed_secs << endl;
  elapsed_secs = double(tend - pbegin) / CLOCKS_PER_SEC;
  cout << "processing only: " << elapsed_secs << endl;

  return 0;
}
