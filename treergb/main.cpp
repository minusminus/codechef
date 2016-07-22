/*
  TREERGB
  https://www.codechef.com/problems/TREERGB

  pomysl:
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

typedef unsigned long long ull;

using namespace std;

const ull M = 1000000007;
ull comb[301][301]; //combinations precalc (n after k): k - row, n - column

int n;  //number of nodes
int cr, cg, cb; //rgb constraints


int main()
{
  ifstream fin("d:\\projects\\codechef\\treergb\\test01.txt");
  cin.rdbuf(fin.rdbuf());

    //speedup cin
//  ios::sync_with_stdio(false);
//  cin.tie(0);

    //precalc combinations modulo M
  for(int i=0; i<301; i++)
    comb[0][i] = 1;
  for(int k=1; k<301; k++){
    comb[k][0] = 0;
    for(int n=1; n<301; n++)
      comb[k][n] = (comb[k][n-1] + comb[k-1][n-1]) % M;
  }

  cin >> n >> cr >> cg >> cb;
  for(int i=0; i<n-1; i++){
    int n1, n2;
    cin >> n1 >> n2;
  }

//  cin >> t;
//  for(int it=0; it<t; it++){
//      //read colors
//    cin >> n;
//    colors.resize(n);
//    sums.resize(n);
//    maxanalysis.resize(n);
//    for(int i=0; i<n; i++){
//      int c;
//      cin >> c;
//      colors[i] = c;
//    }
//      //prepare colors data
//    sort(colors.begin(), colors.end(), sort_rev_order);
//    sums[0] = colors[0];
//    for(int i=1; i<colors.size(); i++)
//      sums[i] = sums[i-1] + colors[i];
//      //maximum number of required blocks to analyse to get 0 in last row
//    maxanalysis[n-1] = colors[n-1];
//    for(int i=n-2; i>=0; i--)
//      maxanalysis[i] = maxanalysis[i+1] + colors[i];
//    totalsum = sums[sums.size()-1];
//
//    tbl.resize(n, vector<ull>(totalsum+1, 0));
//
//    ull res = resolve();
//    cout << res << endl;
//
//    tbl.resize(0);
//    maxanalysis.resize(0);
//    sums.resize(0);
//    colors.resize(0);
//  }


  return 0;
}
