/*
  AMBALLS
  https://www.codechef.com/problems/AMBALLS

  pomysl:
  - jest prekalkulowana tablica kombinacji bez powtorzen (n po k) od 0 do 200 (max kul)
  - kolory posortowane od najwiekszego do najmniejszego
  - istotne sa przewy miedzy kulami, w ktore wkladamy nastepne kule: "+" oznacza przerwe pomiedzy dwoma roznymi kulami, "-" pomiedzy takimi samymi
  - tablica obliczen:
    - wiersz - kolejny dodawany kolor
    - kolumna - ilosc kombinajcji z dana iloscia "-"
  - dla kazdego wiersza i kazdej pozycji liczone:
    - analizowane wszystkie mozliwe kombinacje + i -
    - dla kazdej kombinacji tworzony jest koszyk i "+" oraz j "-"
    - do kazdego koszyka musimy wlozyc wszystkie nowe elementy:
      kombinacje bez powtorzen "+" i "-", oraz z powtorzeniami pozostlych elementow do tego koszyka
  - wynik bedzie w ostatnim wierszu w kolumnie 0 - 0 "-"
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

typedef unsigned long long ull;

using namespace std;

const ull M = 1000000007;

ull comb[201][201]; //combinations precalc (n after k): k - row, n - column
int t;
int n;
vector<int> colors;
vector<int> sums;
int totalsum;
vector< vector<ull> > tbl;  //rows - next color, columns - number of "-"

void printtbl(){
  for(int y=0; y<tbl.size(); y++){
    cout << y << ": ";
    for(int x=0; x<tbl[y].size(); x++){
      cout << tbl[y][x] << ",";
    }
    cout << endl;
  }
}

void printrow(int y){
  cout << y << ": ";
  for(int x=0; x<tbl[y].size(); x++){
    cout << tbl[y][x] << ",";
  }
  cout << endl;
}

ull resolve(){
  if(colors.size() == 1){
    if(colors[0] == 1)
      return 1;
    else
      return 0;
  }

    //first row
//  tbl[0][0] = 1;
  tbl[0][colors[0]-1] = 1;
//  printrow(0);

    //next colors
//  int prevsum = 0;
  int prevsum = sums[0];
  for(int y=1; y<colors.size(); y++){
      //for all previously generated "-" elements
    for(int x=0; x<=prevsum+1; x++){
      if(tbl[y-1][x] > 0)
          //check all blocks
        for(int ij = 0; ij<=colors[y]; ij++){
            //all "-" count
          for(int cminus = 0; cminus <= min(ij, x); cminus++){
            int cplus = ij - cminus;
            int oplus = prevsum + 1 - x;
            int cntminus = x - cminus + colors[y] - ij;

  //          ull combplusy = comb[cplus][oplus];
  //          ull combminusy = comb[cminus][x];
  //          ull combzpowt = comb[colors[y] - ij][colors[y] - 1];
  //          ull res1 = tbl[y-1][x] * combplusy * combminusy * combzpowt;
  //          ull res2 = tbl[y][cntminus] + res1;
  //          tbl[y][cntminus] = (res2) % M;

            tbl[y][cntminus] = ( tbl[y][cntminus]
                                + ((tbl[y-1][x] * comb[cplus][oplus]) % M)
                                * ((comb[cminus][x] * comb[colors[y] - ij][colors[y] - 1]) % M) ) % M;
          }
      }
    }

    prevsum = sums[y];

//    cout << "step: " << y << endl;
//    printtbl();
//    printrow(y);
  }

  return tbl[colors.size()-1][0];
}

bool sort_rev_order(int i,int j) { return (i>j); }

int main()
{
//  ifstream fin("d:\\projects\\codechef\\amballs\\test01.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test02.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test03.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test04.txt");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test05.txt");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test06.txt");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test07.txt");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test08.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test09.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test10.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test11.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test11a.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test12.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test13.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test14.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test20.txt ");
  ifstream fin("d:\\projects\\codechef\\amballs\\test20a.txt ");
//  ifstream fin("d:\\projects\\codechef\\amballs\\test21.txt ");
  cin.rdbuf(fin.rdbuf());

    //speedup cin
//  ios::sync_with_stdio(false);
//  cin.tie(0);

    //precalc combinations modulo M
  for(int i=0; i<201; i++)
    comb[0][i] = 1;
  for(int k=1; k<201; k++){
    comb[k][0] = 0;
    for(int n=1; n<201; n++)
      comb[k][n] = (comb[k][n-1] + comb[k-1][n-1]) % M;
  }
//  for(int k=0; k<=5; k++){
//    cout << k << ": ";
//    for(int n=0; n<=5; n++)
//      cout << comb[k][n] << ",";
//    cout << endl;
//  }
//  cout << comb[37][68] << endl;
//  return 0;


  cin >> t;
  for(int it=0; it<t; it++){
      //read colors
    cin >> n;
    colors.resize(n);
    sums.resize(n);
    for(int i=0; i<n; i++){
      int c;
      cin >> c;
      colors[i] = c;
    }
      //prepare colors data
    sort(colors.begin(), colors.end(), sort_rev_order);
    sums[0] = colors[0];
    for(int i=1; i<colors.size(); i++)
      sums[i] = sums[i-1] + colors[i];
    totalsum = sums[sums.size()-1];

    tbl.resize(n, vector<ull>(totalsum+1, 0));

    ull res = resolve();
    cout << res << endl;

    tbl.resize(0);
    sums.resize(0);
    colors.resize(0);
  }


  return 0;
}
