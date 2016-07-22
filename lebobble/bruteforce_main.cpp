/*
  LEBOBBLE
  https://www.codechef.com/problems/LEBOBBLE

  pomysl:
  - tablica liczb analizowana od lewej do prawej
  - bubble sort oznacza analizowanie w kazdym kroku jednego elementu mniej
  - sprawdzamy ile elementow po prawej jest mneijszych od aktualnie sprawdzanego - to jest ilosc zmian dla tego elementu
  - mozna podzielic tablice na podtablice gdy dojdziemy do elementu dla ktorego (max el ze sprawdzonych)+d jest mniejsze od biezacego elementu w tablicy - mozna ograniczyc w ten sposob ilosc krokow
    do lewego segmentu kierujemy wszsytkie elementy mniejsze od biezacgo poczatkowego do osiagniecia elementu wiekszego od najwiekszego z dotychczas wydzielonych (granica) - od tego momentu reszta elementow trafia do prawego segmentu

  z wykorzystaniem Fenwick Tree (Binary Indexed Tree):
http://cs.stackexchange.com/questions/10538/bit-what-is-the-intuition-behind-a-binary-indexed-tree-and-how-was-it-thought-a?newreg=900d4323f90440259fa55d20df788a52
http://cs.stackexchange.com/questions/42811/what-are-the-main-ideas-used-in-a-fenwick-tree/42816#42816
https://www.topcoder.com/community/data-science/data-science-tutorials/binary-indexed-trees/
    http://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/
    http://www.algorithmist.com/index.php/Fenwick_tree



*/
#include <iostream>
#include <vector>

using namespace std;

struct ixelement{
  public:
    int i1, i2;
};

int t;
int n, d;
vector<int> btbl;
vector<int> ptbl;
vector< ixelement > ixtbl;
int ixcnt;

//calc expected value for 2 elements
int calcexpected( int i1, int i2 ){
  int res = 0;
  if( btbl[i1] > btbl[i2] ){  //both without and with added d
    res += ptbl[i1]*ptbl[i2] + (100-ptbl[i1])*(100-ptbl[i2]);
  }
  if( btbl[i1]+d > btbl[i2] ){  //i1 + d
    res += ptbl[i1]*(100-ptbl[i2]);
  }
  if( btbl[i1] > btbl[i2]+d ){  //i2 + d
    res += (100-ptbl[i1])*ptbl[i2];
  }
  return res;
}

//solve subsegment
int resolvesubsegment(int ix){
  int i1 = ixtbl[ix].i1;
  int i2 = ixtbl[ix].i2;
  int localsum = 0;

    //one element - no swaps
  if( i1 == i2 ) return 0;
    //two elements - calc swaps
  if( i1+1 == i2 ) return calcexpected(i1, i2);
    //more elements - analyze segment
  for( int i=i1; i<i2; i++ ){
    for( int j=i+1; j<=i2; j++ ){
      localsum += calcexpected(i, j);
    }
  }

  return localsum;
}

//solver
int resolve(){
  int ixcurr = 0;
  int globalsum = 0;

    //first segment - whole length
  ixtbl[0].i1 = 0;
  ixtbl[0].i2 = n-1;
  ixcnt = 1;

    //solve all segments
  while( ixcurr < ixcnt ){
    globalsum += resolvesubsegment(0);
    ixcurr++;
  }

  return globalsum;
}


int main()
{
//    //speedup cin
//  ios::sync_with_stdio(false);
//  cin.tie(0);

  cin >> t;
  while(t>0){
    cin >> n >> d;

    btbl.resize(n);
    ptbl.resize(n);
    ixtbl.resize(n);
    for(int i=0; i<n; i++)
      cin >> btbl[i];
    for(int i=0; i<n; i++)
      cin >> ptbl[i];

    int res;
    res = resolve();
//    cout << "sum: " << res << endl;
    cout << fixed;
    cout.precision(4);
    cout << (double)res/10000.0 << endl;

    ixtbl.resize(0);
    btbl.resize(0);
    ptbl.resize(0);

    t--;
  }

  return 0;
}
