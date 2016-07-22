/*
  LEBOBBLE
  https://www.codechef.com/problems/LEBOBBLE

  pomysl:
  z wykorzystaniem Fenwick Tree (Binary Indexed Tree):
    https://www.topcoder.com/community/data-science/data-science-tutorials/binary-indexed-trees/
    http://cs.stackexchange.com/questions/10538/bit-what-is-the-intuition-behind-a-binary-indexed-tree-and-how-was-it-thought-a?newreg=900d4323f90440259fa55d20df788a52
    http://cs.stackexchange.com/questions/42811/what-are-the-main-ideas-used-in-a-fenwick-tree/42816#42816
    http://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/
    http://www.algorithmist.com/index.php/Fenwick_tree

  dodanie elementu: O(log n), biezaca suma dla indeksu: O(log n)
  nalezy sprawdzic ile wiekszych elementow jest po lewej stronie biezacego, tu z pomoca przyjdzie binary indexed tree z kumulowana suma elementow
  - trzeba utworzyc dodatkowa tablice 2*n elementow z <wartoscia elementu, indeksem elementu> wartosc na indeks 2*i, wartosc+d ma indeks 2*i+1
  - posortowac ta tablice po wartosciach i indeksach rosnaco (wazne: te same wartosci posortowane po indeksach rosnaca tak ze zawsze wiekszy indeks jest po mniejszym w wynikowej tabeli)
  - wykorzystac BIT do kumolowania sum - w tym wypadku prawdopodobienstw na poszczegolnych indeksach oryginalnej tabeli

  DLACZEGO: tree.sum(i-1) ? - czy to jest zabezpieczenie przed uwzglednieniem biezacego elementu z drugim prawdopodobienstwem
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//Binary Indexed Tree
class BIT{
private:
  int _size;
  vector<long long> _tbl; //always +1 element, index 0 is dummy
public:
    //constructor
  BIT(){
  }
    //destructor
  ~BIT(){
    _tbl.resize(0);
  }

    //tree initialization
  void init(int size){
    _tbl.resize(0);
    _tbl.resize(size+1, 0); //resize and init with 0
    _size = size+1;
  }
    //get sum
  long long sum(int idx){
    long long res = 0;
    idx++;
    while( idx > 0 ){
      res += _tbl[idx];
      idx -= (idx & -idx);  //subtract last bit set -> going to root
    }
    return res;
  }
    //add value at index
  void add(int idx, long long v){
    idx++;
    while( idx < _size ){
      _tbl[idx] += v;
      idx += (idx & -idx);  //add last bit set -> going down the tree
    }
  }
};


int t;
int n, d;
vector<int> btbl;
vector<int> ptbl;
vector< pair<int, int> > data;
BIT tree;

//solver
long long resolve(){
  long long globalsum = 0;

    //create data tbl
  for(int i=0; i<n; i++){
    data[2*i] = make_pair(btbl[i], 2*i);
    data[2*i+1] = make_pair(btbl[i] + d, 2*i+1);
  }
    //sort data tbl
  sort(data.begin(), data.end());

    //calculate from highest to lowest
  for( int k=2*n-1; k>=0; k-- ){
    int i = data[k].second; //index
    if( i % 2 == 0 ){
        //value
      i /= 2; //original index
//      globalsum += tree.sum(i) * (100 - ptbl[i]);
      globalsum += tree.sum(i-1) * (100 - ptbl[i]);
      tree.add(i, (100 - ptbl[i]));
    } else {
        //value + d
      i /= 2;
//      globalsum += tree.sum(i) * ptbl[i];
      globalsum += tree.sum(i-1) * ptbl[i];
      tree.add(i, ptbl[i]);
    }
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
    data.resize(2*n);
    tree.init(n);
    for(int i=0; i<n; i++)
      cin >> btbl[i];
    for(int i=0; i<n; i++)
      cin >> ptbl[i];

    long long res;
    res = resolve();
//    cout << "sum: " << res << endl;
    cout << fixed;
    cout.precision(4);
    cout << (double)res/10000.0 << endl;

    data.resize(0);
    btbl.resize(0);
    ptbl.resize(0);

    t--;
  }

  return 0;
}
