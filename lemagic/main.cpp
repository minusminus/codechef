/*
  !!! wersja do badan statystycznych

  LEMAGIC
  https://www.codechef.com/problems/LEMAGIC

  pomysl:
  przetwarzanie macierzy podobnie jak w integral image - z gory na dol z lewej na prawa
  DynamicProgramming
  - status: R (czy zmod w wierszu), C (czy zmod w kolumnie), LASTVAL (ostatni element w ciagu), LEN (dl ciagu)
  - w komorce analizujemy: poprawe gornego elementu, poprawe lewego elementu, biezacy element
  - najlepsze rozwiazanie pamietane jest nie jedno, ale dla kazdego konczacego elementu - od 0 do 9 (najlepsze rozwiazanie konczace sie na N)
    zawsze pamietane sa 4 wersje rozwiazania w elemencie: bez modyfiakcji, z mod wiersza (R), z mod kolumny (C), z oboma modyfikacjami (RC)

*/
#include <iostream>
#include <vector>
//#include <largeint.h>

using namespace std;

struct singlecase{
  public:
    int x, y, len;  //where is it from and length
};

struct status{
  public:
    singlecase N, R, C, RC; //no change, change in row, in column, both
};

int t;
int n,m;
vector< vector<int> > tbl;
vector< vector<status> > tmp;
vector<status> ctmp;
int tblbestt[9];

void printstatus( status s ){
  cout << "N(" << s.N.len << "," << s.N.x << "," << s.N.y << ") ";
  cout << "C(" << s.C.len << "," << s.C.x << "," << s.C.y << ") ";
  cout << "R(" << s.R.len << "," << s.R.x << "," << s.R.y << ") ";
  cout << "RC(" << s.RC.len << "," << s.RC.x << "," << s.RC.y << ")";
  cout << endl;
}

void printtmp(){
  for(int i=0; i<m; i++){
    for(int j=0; j<=9; j++){
      cout << i << "," << j << ": ";
      cout << "N(" << tmp[i][j].N.len << "," << tmp[i][j].N.x << "," << tmp[i][j].N.y << ") ";
      cout << "C(" << tmp[i][j].C.len << "," << tmp[i][j].C.x << "," << tmp[i][j].C.y << ") ";
      cout << "R(" << tmp[i][j].R.len << "," << tmp[i][j].R.x << "," << tmp[i][j].R.y << ") ";
      cout << "RC(" << tmp[i][j].RC.len << "," << tmp[i][j].RC.x << "," << tmp[i][j].RC.y << ")";
      cout << endl;
    }
    cout << endl;
  }
}

//magic op on element
int mop(int p, int v){
  return (v + p) % 10;
}

void setbetter_singlecase( singlecase &s1, singlecase &s2 ){
  if(s2.len > s1.len){
    s1.len = s2.len;
    s1.x = s2.x;
    s1.y = s2.y;
//    s1 = s2;
  }
}

void setbetter_status( status &s1, status &s2 ){
  setbetter_singlecase(s1.N, s2.N);
  setbetter_singlecase(s1.C, s2.C);
  setbetter_singlecase(s1.R, s2.R);
  setbetter_singlecase(s1.RC, s2.RC);
}

void checkelem_vec( int x, int y, int p, bool checkleft, bool checktop){
  status curr;
  int v, vp, v2p;
  int starti;

  v = tbl[y][x]; vp = mop(p, v); v2p = mop(p, vp);
  starti = min(v, min(vp, v2p));  //starting index of analysis

    //current tmp cleanup
  for(int i=0; i<=9; i++){
    ctmp[i].N.len = ctmp[i].N.x = ctmp[i].N.y = 0;
    ctmp[i].C.len = ctmp[i].C.x = ctmp[i].C.y = 0;
    ctmp[i].R.len = ctmp[i].R.x = ctmp[i].R.y = 0;
    ctmp[i].RC.len = ctmp[i].RC.x = ctmp[i].RC.y = 0;
  }

    //current
  ctmp[v].N.len=1; ctmp[v].N.x=x; ctmp[v].N.y=y;
  ctmp[vp].R.len=1; ctmp[vp].R.x=x; ctmp[vp].R.y=y;
  ctmp[vp].C.len=1; ctmp[vp].C.x=x; ctmp[vp].C.y=y;
  ctmp[v2p].RC.len=1; ctmp[v2p].RC.x=x; ctmp[v2p].RC.y=y;

    //left + current
  if(checkleft){
      //copy
    for(int i=0; i<=9; i++){
      if( (i==v) || (i==vp) || (i==v2p) ){
        setbetter_status(ctmp[i], tmp[x-1][i]);
      } else
        ctmp[i] = tmp[x-1][i];
    }
      //with next elem, or improved elem
//    for(int i=0; i<=9; i++){
    for(int i=starti; i<=9; i++){ //skip elements that cant be changed
      curr = tmp[x-1][i];
      curr.N.len++; curr.N.x = x; curr.N.y = y;
      curr.C.len++; curr.C.x = x; curr.C.y = y;
      curr.R.len++; curr.R.x = x; curr.R.y = y;
      curr.RC.len++; curr.RC.x = x; curr.RC.y = y;
        //v
      if( (i>=v) ){
        if(tmp[x-1][i].N.len>0) setbetter_singlecase(ctmp[v].N, curr.N);  //no change
        if(tmp[x-1][i].C.len>0) setbetter_singlecase(ctmp[v].N, curr.C);  //change in any C on left
        if((tmp[x-1][i].R.len>0) && (tmp[x-1][i].R.y<y)) setbetter_singlecase(ctmp[v].N, curr.R); //change in R but before current row
        if((tmp[x-1][i].RC.len>0) && (tmp[x-1][i].RC.y<y)) setbetter_singlecase(ctmp[v].N, curr.RC);  //change in RC but before current row
      }
        //vp
      if( i>=vp ){
        if(tmp[x-1][i].N.len>0){  //no change on left
          setbetter_singlecase(ctmp[vp].C, curr.N); //any C
          if(tmp[x-1][i].N.y<y) setbetter_singlecase(ctmp[vp].R, curr.N); //R but only if N was before current row
        }
        if(tmp[x-1][i].C.len>0){  //change in C on left
          setbetter_singlecase(ctmp[vp].C, curr.C); //any C
          if(tmp[x-1][i].C.y<y) setbetter_singlecase(ctmp[vp].R, curr.C); //R but only if C was before current row
        }
        if(tmp[x-1][i].R.len>0){  //change in R on left
          setbetter_singlecase(ctmp[vp].R, curr.R); //any R
          if(tmp[x-1][i].R.y<y) setbetter_singlecase(ctmp[vp].C, curr.R); //C but only if R was before current row
        }
        if(tmp[x-1][i].RC.len>0){ //change in RC on left
          setbetter_singlecase(ctmp[vp].R, curr.RC); //any R
          if(tmp[x-1][i].RC.y<y) setbetter_singlecase(ctmp[vp].C, curr.RC);  //C but only if RC was before current row
        }
      }
        //v2p
      if( i>=v2p ){
        if((tmp[x-1][i].N.len>0) && (tmp[x-1][i].N.y<y)) setbetter_singlecase(ctmp[v2p].RC, curr.N);  //no change but before current row
        if((tmp[x-1][i].C.len>0) && (tmp[x-1][i].C.y<y)) setbetter_singlecase(ctmp[v2p].RC, curr.C);  //change in C but before current row
        if(tmp[x-1][i].R.len>0) setbetter_singlecase(ctmp[v2p].RC, curr.R); //change in any R on left
        if(tmp[x-1][i].RC.len>0) setbetter_singlecase(ctmp[v2p].RC, curr.RC); //change in any RC on left
      }
    }
  }

    //top + current
  if(checktop){
      //copy
    for(int i=0; i<=9; i++){
      setbetter_status(ctmp[i], tmp[x][i]);
    }
      //with next elem, or improved elem
//    for(int i=0; i<=9; i++){
    for(int i=starti; i<=9; i++){ //skip elements that cant be changed
      curr = tmp[x][i];
      curr.N.len++; curr.N.x = x; curr.N.y = y;
      curr.C.len++; curr.C.x = x; curr.C.y = y;
      curr.R.len++; curr.R.x = x; curr.R.y = y;
      curr.RC.len++; curr.RC.x = x; curr.RC.y = y;
        //v
      if( (i>=v) ){
        if(tmp[x][i].N.len>0) setbetter_singlecase(ctmp[v].N, curr.N);  //no change
        if((tmp[x][i].C.len>0) && (tmp[x][i].C.x<x)) setbetter_singlecase(ctmp[v].N, curr.C);  //change in C but before current column
        if(tmp[x][i].R.len>0) setbetter_singlecase(ctmp[v].N, curr.R); //change in any R on top
        if((tmp[x][i].RC.len>0) && (tmp[x][i].RC.x<x)) setbetter_singlecase(ctmp[v].N, curr.RC);  //change in RC but before current column
      }
        //vp
      if( i>=vp ){
        if(tmp[x][i].N.len>0){  //no change on top
          setbetter_singlecase(ctmp[vp].R, curr.N); //any R
          if(tmp[x][i].N.x<x) setbetter_singlecase(ctmp[vp].C, curr.N); //C but only if N was before current column
        }
        if(tmp[x][i].C.len>0){  //change in C on top
          setbetter_singlecase(ctmp[vp].C, curr.C); //any C
          if(tmp[x][i].C.x<x) setbetter_singlecase(ctmp[vp].R, curr.C); //R but only if C was before current column
        }
        if(tmp[x][i].R.len>0){  //change in R on top
          setbetter_singlecase(ctmp[vp].R, curr.R); //any R
          if(tmp[x][i].R.x<x) setbetter_singlecase(ctmp[vp].C, curr.R); //C but only if R was before current column
        }
        if(tmp[x][i].RC.len>0){ //change in RC on top
          setbetter_singlecase(ctmp[vp].C, curr.RC); //any C
          if(tmp[x][i].RC.x<x) setbetter_singlecase(ctmp[vp].R, curr.RC);  //R but only if RC was before current column
        }
      }
        //v2p
      if( i>=v2p ){
        if((tmp[x][i].N.len>0) && (tmp[x][i].N.x<x)) setbetter_singlecase(ctmp[v2p].RC, curr.N);  //no change but before current column
        if(tmp[x][i].C.len>0) setbetter_singlecase(ctmp[v2p].RC, curr.C);  //change in any C on top
        if((tmp[x][i].R.len>0) && (tmp[x][i].R.x<x)) setbetter_singlecase(ctmp[v2p].RC, curr.R); //change in R but before current column
        if(tmp[x][i].RC.len>0) setbetter_singlecase(ctmp[v2p].RC, curr.RC); //change in any RC on top
      }
    }
  }

    //set best
  for(int i=0; i<=9; i++){
//    tmp[x][i] = ctmp[i];
    tmp[x][i].N = ctmp[i].N;
    tmp[x][i].R = ctmp[i].R;
    tmp[x][i].C = ctmp[i].C;
    tmp[x][i].RC = ctmp[i].RC;
  }
}

int resolve(){
  int bestlen = 0;

    //for each p, no need for p==0 -> its analyzed in every case
  for(int p=1; p<=9; p++){
//  for(int p=0; p<=9; p++){
//  for(int p=0; p<=0; p++){
      //tmp zero
    for(int i=0; i<m; i++)
      for(int j=0; j<=9; j++){
        tmp[i][j].N.len = tmp[i][j].N.x = tmp[i][j].N.y = 0;
        tmp[i][j].C.len = tmp[i][j].C.x = tmp[i][j].C.y = 0;
        tmp[i][j].R.len = tmp[i][j].R.x = tmp[i][j].R.y = 0;
        tmp[i][j].RC.len = tmp[i][j].RC.x = tmp[i][j].RC.y = 0;
      }

//    cout << "P: " << p << endl;
      //first row
    checkelem_vec(0, 0, p, false, false);
    for(int x=1; x<m; x++){
      checkelem_vec(x, 0, p, true, false);
    }
//    printtmp();
      //next rows
    for(int y=1; y<n; y++){
      checkelem_vec(0, y, p, false, true);
      for(int x=1; x<m; x++){
        checkelem_vec(x, y, p, true, true);
      }
//      printtmp();
    }

    int bestt = 0;
    for(int i=0; i<=9; i++){
      int t = max( max( tmp[m-1][i].N.len, tmp[m-1][i].C.len), max(tmp[m-1][i].R.len, tmp[m-1][i].RC.len ) );
      if(t > bestt) bestt = t;
      if(t > bestlen) bestlen = t;
    }
    tblbestt[p-1] = bestt;
//    cout << p << " " << bestt << endl;
//    if(bestlen == (m+n-1)) return bestlen;
  }

  return bestlen;
}

int main()
{
  bool hdr = true;
//    //speedup cin
//  ios::sync_with_stdio(false);
//  cin.tie(0);

  cin >> t;
  while(t>0){
      //read
    cin >> n >> m;

    if(hdr){
      cout << "index;";
      for(int i=0; i<m; i++)
        cout << "i" << i << ";";
      for(int i=1; i<=9; i++)
        cout << "P" << i << ";";
      cout << endl;
      hdr = false;
    }

    tbl.resize(n, vector<int>(m));
    tmp.resize(m, vector<status>(10));
    ctmp.resize(10);
    for( int i=0; i<n; i++ ){
      for(int j=0; j<m; j++){
        char c;
        cin >> c;
        tbl[i][j] = c - '0';
      }
    }


//    LARGE_INTEGER frequency, t1, t2;
//    double elapsedTime;
//    QueryPerformanceFrequency(&frequency);
//
//    QueryPerformanceCounter(&t1);
      //calc
    int res = resolve();
//    QueryPerformanceCounter(&t2);
//    elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
//    cout << elapsedTime << endl;

//    cout << res << endl;
    for( int i=0; i<n; i++ ){
      cout << t << ";";
      for(int j=0; j<m; j++){
        cout << tbl[i][j] << ";";
      }
      if(i==0){
        for(int j=0; j<9; j++){
          cout << tblbestt[j] << ";";
        }
      }
      cout << endl;
    }

    tbl.resize(0, vector<int>(0));
    tmp.resize(0, vector<status>(0));
    ctmp.resize(0);
      //next
    t--;
  }

  return 0;
}
