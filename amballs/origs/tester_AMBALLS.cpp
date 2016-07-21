#include<cstdio>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<algorithm>
#include<string>
#include<utility>
#include<cmath>
#include<cstring>
#include<cassert>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define ll long long
#define M 1000000007

ll read_int(ll mn, ll mx, char next){
  int c, fg = 1;
  ll res = 0;
  c=getchar();
  if(c=='-') fg = -1, c = getchar();
  assert('0'<=c && c<='9');
  res = c - '0';
  for(;;){
    c = getchar();
    if(c==next) break;
    assert(res!=0);
    assert('0'<=c && c<='9');
    res = res * 10 + (c - '0');
  }
  res *= fg;
  assert(mn<=res && res<=mx);
  return res;
}

void read_arr(int n, int res[], ll mn, ll mx){
  int i;
  rep(i,n-1) res[i] = read_int(mn, mx, ' ');
  res[n-1] = read_int(mn, mx, '\n');
}


int main(){
  int T, N, C[201];

  int i, j, k, all, sum;
  int b, r;
  ll mul, add;
  static ll dp[201], next[201]; // dp[number of "bad" place] = number of ways
  static ll comb[201][201]; // binomial coefficient

  rep(j,201) comb[0][j] = 0;
  rep(i,201) comb[i][0] = 1;
  REP(i,1,201) REP(j,1,201) comb[i][j] = (comb[i-1][j-1] + comb[i-1][j]) % M;

  T = read_int(1, 7, '\n');
  while(T--){
    N = read_int(1, 200, '\n');
    read_arr(N, C, 1, 200);

    sum = 0;
    rep(i,N) sum += C[i];
    assert(sum <= 200);
    
    all = 1; // number of place, which can be inserted
    rep(i,sum+1) dp[i] = 0; dp[0] = 1;

    rep(k,N){
      rep(i,sum+1) next[i] = 0;
      rep(i,sum+1) if(dp[i]){
        REP(b,1,C[k]+1){ // insert into exactly b place
          mul = (dp[i] * comb[C[k]-1][b-1]) % M;
          rep(r, b+1){ // insert into r "bad" place
            add = (((mul * comb[i][r]) % M) * comb[all-i][b-r]) % M;
            if(add) next[i+(C[k]-b)-r] += add;
          }
        }
      }
      rep(i,sum+1) dp[i] = next[i] % M;
      all += C[k];
    }

    printf("%d\n", (int)dp[0]);
  }

  assert( scanf("%*c")==EOF );
  return 0;
}
