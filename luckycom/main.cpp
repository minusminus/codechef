/*
  LUCKYCOM
  https://www.codechef.com/problems/LUCKYCOM

  pomysl (na wyszukiwanie podciagu w ciagu):
  - liczymy: L4, LX, R7, RX (4 z lewej, 7 z prawej od danej pozycji - pozycja zawiera sie w obliczeniach)
  - liczymy L4+LX i R7+RX
  - dla kazdego szukanego ciagu szukamy minimalnego indeksu L4+LX (iL) i R7+RX (iR) - to sa granice poza ktorymi nie da sie znalezc rozwiazania - rozwiazanie jest pomiedzy tymi indeksami
  - w tym zakresie szukamy minimalnej ilosc X ktore trzeba zamienic zeby uzyskac ciag (albo maks 4 i 7, przy czym 7 z prawej, 4 do pozycji)
  - dodatkowe filtry przyspieszajace:
    - wyszukiwanie iL i iR binarysearch, jezeli ktorys nie znaleziony to nie ma rozwiazanie
    - generujac tablice zbieramy od razu: max(L4 + R7) ktory okresli nam makymalny podciag 4477 bez X
    - przeszukiwanie pomiedzy indeksami za pomoca RMQ (oparta na sqrt)
      https://www.topcoder.com/community/data-science/data-science-tutorials/range-minimum-query-and-lowest-common-ancestor/

*/
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <ctime>

using namespace std;

class RMQQueryObject{
public:
  virtual void clear(bool all = false) = 0;
  virtual void checkValue(int index) = 0;
};

class RMQrec{
public:
  ~RMQrec(){
    idx.resize(0);
  };

  int value;  //value
  vector<int> idx;  //table of indexes with value
};

//rmq - sqrt size, max version
class RMQsqrt{
private:
  unsigned int blocksize;
//  vector<int>* pdata;  //original data vector
//  vector< pair<int, int> > ixtbl; //first - index, second - value
  vector<RMQrec> ixtbl;
  RMQrec globalmax;

public:
 RMQsqrt(){}
  RMQsqrt(vector<int>& tbl){
    init(tbl);
  }
  ~RMQsqrt(){
    ixtbl.resize(0);
  }

  //initialization
  void init(vector<int>& tbl){
//    pdata = tbl;
    blocksize = (int)sqrt(tbl.size());
    int t = tbl.size() / blocksize;
    if( tbl.size() % blocksize != 0 ) t += 1;
    ixtbl.resize(t);
    globalmax.value = -1;
    int pos = 0;
    for(int i=0; i<ixtbl.size(); i++){
//      ixtbl[i].first = pos;
//      ixtbl[i].second = tbl->at(pos);
      ixtbl[i].value = tbl[pos];
      ixtbl[i].idx.resize(0);
      ixtbl[i].idx.push_back(pos);
      pos++;
      for(int j=1; (j<blocksize) && (pos < tbl.size()); j++){
        if(tbl[pos] > ixtbl[i].value){
//          ixtbl[i].first = pos;
//          ixtbl[i].second = tbl->operator[](pos);
          ixtbl[i].value = tbl[pos];
          ixtbl[i].idx.resize(0);
          ixtbl[i].idx.push_back(pos);
        } else if(tbl[pos] == ixtbl[i].value){
          ixtbl[i].idx.push_back(pos);
        }
        pos++;
      }
      if( globalmax.value < ixtbl[i].value )
        globalmax = ixtbl[i];
    }
  }

  //min between <ifrom, ito>, returns table of indexes in original table
//  void query(int ifrom, int ito, vector<int>& tbl, vector<int>& res){
//    res.resize(0);
//
//    int il, ir;
//    il = ifrom / blocksize;
//    ir = ito / blocksize;
//
////    int ires = ifrom;
////    int iresmax = tbl->operator[](ires);
//    int iresmax = tbl[ifrom];
//      //boundary blocks (il -> ir)
//    if(il == ir){  //both in the same block
//      for(int i = ifrom; i<=ito; i++)
//        if(tbl[i] > iresmax){
////          ires = i;
//          iresmax = tbl[i];
//          res.resize(0);
//          res.push_back(i);
//        } else if(tbl[i] == iresmax){
//          res.push_back(i);
//        }
//    } else {  //in different blocks
//      for(int i = ifrom; i<((il+1) * blocksize); i++)
//        if(tbl[i] > iresmax){
////          ires = i;
//          iresmax = tbl[i];
//          res.resize(0);
//          res.push_back(i);
//        } else if(tbl[i] == iresmax){
//          res.push_back(i);
//        }
//      for(int i = (ir * blocksize); i<=ito; i++)
//        if(tbl[i] > iresmax){
////          ires = i;
//          iresmax = tbl[i];
//          res.resize(0);
//          res.push_back(i);
//        } else if(tbl[i] == iresmax){
//          res.push_back(i);
//        }
//    }
//      //blocks between il and ir
//    for(int i = il+1; i<ir; i++)
//      if(ixtbl[i].value > iresmax){
////        ires = ixtbl[i].first;
////        iresmax = ixtbl[i].second;
//        iresmax = ixtbl[i].value;
//        res = ixtbl[i].idx;
//      } else if(ixtbl[i].value == iresmax){
//        for(int j=0; j<ixtbl[i].idx.size(); j++) res.push_back( ixtbl[i].idx[j] );
//      }
////    return ires;
//  }

  void query_2(int ifrom, int ito, vector<int>& tbl, RMQQueryObject& qob){
    qob.clear(true);

//    if( (ifrom <= globalmax.idx[0]) && (ito >= globalmax.idx[0]) ){
//      qob.checkValue(globalmax.idx[0]);
//      return;
//    }

    int il, ir;
    il = ifrom / blocksize;
    ir = ito / blocksize;

//    int iresmax = tbl[ifrom];
    int iresmax = -1;
      //boundary blocks (il -> ir)
    if(il == ir){  //both in the same block
      if( (ifrom <= ixtbl[il].idx[0]) && (ito >= ixtbl[il].idx[0]) ){
        iresmax = tbl[ ixtbl[il].idx[0] ];
        qob.checkValue(ixtbl[il].idx[0]);
      } else
      for(int i = ifrom; i<=ito; i++)
        if(tbl[i] > iresmax){
          iresmax = tbl[i];
          qob.clear();
          qob.checkValue(i);
        } else if(tbl[i] == iresmax){
          qob.checkValue(i);
        }
    } else {  //in different blocks
      if( (ifrom <= ixtbl[il].idx[ixtbl[il].idx.size()-1]) ){
        iresmax = tbl[ ixtbl[il].idx[ixtbl[il].idx.size()-1] ];
        qob.checkValue( ixtbl[il].idx[ixtbl[il].idx.size()-1] );
      } else
      for(int i = ifrom; i<((il+1) * blocksize); i++)
        if(tbl[i] > iresmax){
          iresmax = tbl[i];
          qob.clear();
          qob.checkValue(i);
        } else if(tbl[i] == iresmax){
          qob.checkValue(i);
        }
      if( (ito >= ixtbl[ir].idx[0]) ){
        if(tbl[ixtbl[ir].idx[0]] > iresmax){
          iresmax = tbl[ ixtbl[ir].idx[0] ];
          qob.checkValue(ixtbl[ir].idx[0]);
        }
      } else
      for(int i = (ir * blocksize); i<=ito; i++)
        if(tbl[i] > iresmax){
          iresmax = tbl[i];
          qob.clear();
          qob.checkValue(i);
        } else if(tbl[i] == iresmax){
          qob.checkValue(i);
        }
    }
      //blocks between il and ir
    for(int i = il+1; i<ir; i++)
      if(ixtbl[i].value > iresmax){
        iresmax = ixtbl[i].value;
        qob.clear();
//        for(int j=0; j<ixtbl[i].idx.size(); j++) qob.checkValue( ixtbl[i].idx[j] );
        qob.checkValue( ixtbl[i].idx[0] );
      } else if(ixtbl[i].value == iresmax){
//        for(int j=0; j<ixtbl[i].idx.size(); j++) qob.checkValue( ixtbl[i].idx[j] );
        qob.checkValue( ixtbl[i].idx[0] );
      }
  }
};


//entry for each string
class StrEntry{
private:
    //prepare blocks from string
  void prepare(string& s){
//    origstr = s;
    total4 = total7 = totalX = 0;
    if( s.length()==0 ) return;
    strlen = s.length();

    tL4.resize(strlen);
    tR7.resize(strlen);
    tLX.resize(strlen);
    tRX.resize(strlen);
//    tL4X.resize(strlen);
//    tR7X.resize(strlen);
    tiL4X.resize(strlen+1);
    tiR7X.resize(strlen+1);
    tiL4.resize(strlen+1, -1);
    tiR7.resize(strlen+1, -1);
//    tsumX.resize(strlen);
    tsum47.resize(strlen);

    //left
    tL4[0] = tLX[0] = 0;
    switch(s[0]){
      case '4' : tL4[0] = 1; break;
      case '?' : tLX[0] = 1; break;
    }
    for(int i=1; i<strlen; i++){
      tL4[i] = tL4[i-1];
      tLX[i] = tLX[i-1];
      switch(s[i]){
        case '4' : tL4[i]++; break;
        case '?' : tLX[i]++; break;
      };
    }
    total4 = tL4[strlen-1];
    totalX = tLX[strlen-1];
    //right
    tR7[strlen-1] = tRX[strlen-1] = 0;
    switch(s[strlen-1]){
      case '7' : tR7[strlen-1] = 1; break;
      case '?' : tRX[strlen-1] = 1; break;
    }
    for(int i=strlen-2; i>=0; i--){
      tR7[i] = tR7[i+1];
      tRX[i] = tRX[i+1];
      switch(s[i]){
        case '7' : tR7[i]++; break;
        case '?' : tRX[i]++; break;
      };
    }
    total7 = tR7[0];
    //sums
    int vl4x, vr7x;
    int pvl4x, pvr7x;
    for(int i=0; i<strlen; i++){
      vl4x = tL4[i] + tLX[i];
//      tL4X[i] = vl4x;
      vr7x = tR7[i] + tRX[i];
//      tR7X[i] = vr7x;
      if(i>0){
//        if(vl4x != tL4X[i-1]) tiL4X[vl4x] = i;
//        if(vr7x != tR7X[i-1]) tiR7X[tR7X[i-1]] = i-1;
        if(vl4x != pvl4x) tiL4X[vl4x] = i;
        if(vr7x != pvr7x) tiR7X[pvr7x] = i-1;
      }
      if(tiL4[tL4[i]] == -1) tiL4[tL4[i]] = i;
      if(tiR7[tR7[i]] == -1) tiR7[tR7[i]] = i;
      pvl4x = vl4x;
      pvr7x = vr7x;
    }
//    if( tR7X[strlen-1] != 0 ) tiR7X[tR7X[strlen-1]] = strlen-1;
    if( pvr7x != 0 ) tiR7X[pvr7x] = strlen-1;

    for(int i=0; i<strlen; i++)
      tsum47[i] = tL4[i] + tR7[i];

    //rmq init
    rmq.init(tsum47);
  };
public:
  StrEntry(){};
  StrEntry(string s){
    prepare(s);
  }
  ~StrEntry(){
    tL4.resize(0);
    tR7.resize(0);
    tLX.resize(0);
    tRX.resize(0);
//    tL4X.resize(0);
//    tR7X.resize(0);
    tiL4X.resize(0);
    tiR7X.resize(0);
    tiL4.resize(0);
    tiR7.resize(0);
//    tsumX.resize(0);
    tsum47.resize(0);
  };

  vector<int> tL4, tR7, tLX, tRX; //tables of sums of elements
  vector<int> /*tL4X, tR7X, tsumX,*/ tsum47;  //sums
  vector<int> tiL4X, tiR7X, tiL4, tiR7; //precalc indexes
  int total4, total7, totalX; //total number of 4, 7, X
  int strlen;
  int maxnoX4s, maxnoX7s; //max 4s and 7s without change
  RMQsqrt rmq;
  string origstr;

  void print(){
    cout << origstr << endl;
    cout << "4: " << total4 << ", 7: " << total7 << ", X: " << totalX << endl;
    cout << "L4: "; for(int i=0; i<strlen; i++) cout << tL4[i] << ","; cout << endl;
    cout << "LX: "; for(int i=0; i<strlen; i++) cout << tLX[i] << ","; cout << endl;
    cout << "R7: "; for(int i=0; i<strlen; i++) cout << tR7[i] << ","; cout << endl;
    cout << "RX: "; for(int i=0; i<strlen; i++) cout << tRX[i] << ","; cout << endl;
//    cout << "L4X: "; for(int i=0; i<strlen; i++) cout << tL4X[i] << ","; cout << endl;
//    cout << "R7X: "; for(int i=0; i<strlen; i++) cout << tR7X[i] << ","; cout << endl;
    cout << "iL4X: "; for(int i=0; i<strlen+1; i++) cout << tiL4X[i] << ","; cout << endl;
    cout << "iR7X: "; for(int i=0; i<strlen+1; i++) cout << tiR7X[i] << ","; cout << endl;
    cout << "iL4: "; for(int i=0; i<strlen+1; i++) cout << tiL4[i] << ","; cout << endl;
    cout << "iR7: "; for(int i=0; i<strlen+1; i++) cout << tiR7[i] << ","; cout << endl;
//    cout << "sumX: "; for(int i=0; i<strlen; i++) cout << tsumX[i] << ","; cout << endl;
    cout << "sum47: "; for(int i=0; i<strlen; i++) cout << tsum47[i] << ","; cout << endl;
  };
};

class RMQStrQueryObject : public RMQQueryObject{
private:
  StrEntry* _str;
  int _cnt4s, _cnt7s;
public:
  int minchanges;
  int checkcnt;

  RMQStrQueryObject(StrEntry* str, int cnt4s, int cnt7s){
    _str = str;
    _cnt4s = cnt4s;
    _cnt7s = cnt7s;
    minchanges = -1;
    checkcnt = 0;
  }

  void clear(bool all){
    minchanges = -1;
    if(all) checkcnt = 0;
  }

  void checkValue(int index){
    checkcnt++;
    int tmc = max(_cnt4s - _str->tL4[index], 0) + max(_cnt7s - _str->tR7[index], 0);
//    int tmc = (_cnt4s + _cnt7s) - (_str->tL4[index] + _str->tR7[index]);
    if( (minchanges == -1) || (tmc < minchanges) ) minchanges = tmc;
  }
};

bool cmp_gr(int lhs, int rhs){ return lhs >= rhs; }
bool cmp_gr2(int lhs, int rhs){ return lhs > rhs; }

int findMinChanges(int cnt4s, int cnt7s, StrEntry& str, int maxchanges){
    //longer than string
  if(cnt4s + cnt7s > str.strlen) return -1;
    //if required but there's nothing in string
  if( str.total4 + str.totalX < cnt4s ) return -1;
  if( str.total7 + str.totalX < cnt7s ) return -1;

    //if 4s and 7s fits max without change - return 0 changes
//  if( (str.maxnoX4s > -1) && (str.maxnoX7s > -1) && (str.maxnoX4s >= cnt4s) && (str.maxnoX7s >= cnt7s ) ) return 0;
//  if( (str.maxnoX4s >= cnt4s) && (str.maxnoX7s >= cnt7s ) ) return 0;

  /*
    RMQ search for minimal number of X required to prepare requested string
  */
    //left and right index to start from
  int il, ir;
  int ncl, ncr;
    //on precalc tables
  il = str.tiL4X[cnt4s];
  ir = str.tiR7X[cnt7s];
  ncl = str.tiL4[cnt4s];
  ncr = str.tiR7[cnt7s];
  if(ir <= il) return -1;
  if((ncl > -1) && (ncr > -1) && (ncl < ncr)) return 0;

  if((ncr > -1) && (il < ncr)) il = ncr;
  if((ncl > -1) && (ir > ncl)) ir = ncl;
  if(ir<il){swap(il, ir);}

//  cout << "il=" << il << ", ir=" << ir << endl;
    //the sequence exists
//  vector<int> ix;
  RMQStrQueryObject ix(&str, cnt4s, cnt7s);
  str.rmq.query_2(il, ir, str.tsum47, ix); //index with minimal X count
//  cout << "ix=" << ix.size() << endl;
//  cout << "ix.checkcount: " << ix.checkcnt << endl;
//  int minchanges = -1;
//  for(int t=0; t<ix.size(); t++){
//    int tmc = max(cnt4s - str.tL4[ix[t]], 0) + max(cnt7s - str.tR7[ix[t]], 0);
//    if( (minchanges == -1) || (tmc < minchanges) ) minchanges = tmc;
//  }
  int minchanges = ix.minchanges;

  if(minchanges <= maxchanges)
    return minchanges;
  else
    return -1;  //cant do the sequence with specified maxchanges of X
}


int n, k; //num of strings, fav num
vector<StrEntry*> data;
int min4, min7; //min num of 4s and 7s in all strings
int max4, max7;

//find max size of 44... and 77... having fav num of k
void solve_step1(){
  int csize;
    //4
  csize = min4 + 1;
  while(true){
    int kleft = k;
    bool b = true;
    for(int i=0; i<data.size(); i++){
      if(data[i]->total4 < csize){
        int rX = csize - data[i]->total4;  //requested count of X
        if((rX <= data[i]->totalX) && (rX <= kleft))
          kleft -= rX;
        else
          b = false;  //this csize cant be done
      }
    }
    if(b) //csize fits
      csize++;
    else{ //csize cant be done
      csize--;
      break;
    }
  }
  max4 = csize;
//  cout << "max 4: " << max4 << endl;

    //7
  csize = min7 + 1;
  while(true){
    int kleft = k;
    bool b = true;
    for(int i=0; i<data.size(); i++){
      if(data[i]->total7 < csize){
        int rX = csize - data[i]->total7;  //requested count of X
        if((rX <= data[i]->totalX) && (rX <= kleft))
          kleft -= rX;
        else
          b = false;  //this csize cant be done
      }
    }
    if(b) //csize fits
      csize++;
    else{ //csize cant be done
      csize--;
      break;
    }
  }
  max7 = csize;
//  cout << "max 7: " << max7 << endl;
}

//find concatenations
int solve_step2(){
  int res = 0;

  for(int x=1; x<=max4; x++){
    for(int y=1; y<=max7; y++){
      int currmaxchanges = k;
      bool b = true;
      for(int i=0; i<data.size(); i++){
        int t = findMinChanges(x, y, *data[i], currmaxchanges);
//        if(t==-1)
//          cout << i << "(" << currmaxchanges << "): " << x << "," << y << ": " << t << endl;
//        if(t==-1 && i==5678){
//          data[i]->print();
//        }
        if(t>-1){
          currmaxchanges -= t;
        } else {
          b = false;
          break;
        }
      }
      if(b){
        res++;
//        cout << x << "," << y << endl;
      }
    }
  }
  return res;
}

//find concatenations downwards
int solve_step2_2(){
  int res = 0;

  int last7 = 1;
  for(int x=max4; x>=1; x--){
    cout << x << ": " << last7 << " (" << max7 << ")" << endl;
    if(last7 == max7){  //max7 reached last time -> all possibilities can be done
      res += max7 * x;
      break;
    }
    for(int y=max7; y>=last7; y--){
      int currmaxchanges = k;
      bool b = true;
      for(int i=0; i<data.size(); i++){
        int t = findMinChanges(x, y, *data[i], currmaxchanges);
        if(t>-1){
          currmaxchanges -= t;
        } else {
          b = false;
          break;
        }
      }
      if(b){
//        res++;
        res += y; //can do all <=y
        last7 = y;  //all 4s<=y can be done so next search to y
        break;  //next x
      }
    }
  }

  return res;
}

bool check_7s_fit(int x, int y){
  int currmaxchanges = k;
  bool b = true;
  for(int i=0; i<data.size(); i++){
    int t = findMinChanges(x, y, *data[i], currmaxchanges);
//    if( x==46 && y==1){
//      cout << i << ": " << t << "(" << currmaxchanges << ") 4:" << data[i]->total4 << ", 7:" << data[i]->total7 << ", X:" << data[i]->totalX << endl;
//    }
    if(t>-1){
      currmaxchanges -= t;
    } else {
//      cout << "i: " << i << " " << x << "," << y << " (" << currmaxchanges << ") 4:" << data[i]->total4 << ", 7:" << data[i]->total7 << ", X:" << data[i]->totalX << endl;
//      cout << data[i]->origstr << endl;
      b = false;
      break;
    }
  }
  return b;
}

//find concatenations downwards, with binary search for split point in 7s
long long solve_step2_3(){
  long long res = 0;

  int last7 = 1;
  for(int x=max4; x>=1; x--){
//    cout << x << ": " << last7 << " (" << max7 << ")" << endl;
//    if(last7 == max7){  //max7 reached last time -> all possibilities can be done
//      res += max7 * x;
//      break;
//    }

      //asumming il - doesnt fit, ir - fits (il > ir)
    int il, ir, ic;
    il = max7;
    ir = last7;
    while(il - ir > 3) {  //shortening range
      ic = ir + (il - ir) / 2;
      if( check_7s_fit(x, ic) ){
        ir = ic;
      } else {
        il = ic;
      }
    }
    for(int y=il; y>=ir; y--){  //analyze very short range
      if( check_7s_fit(x, y) ){
//        for(int iy=1; iy<=y; iy++)
//          cout << x << "," << iy << endl;
        res += y; //can do all <=y
        last7 = y;  //all 4s<=y can be done so next search to y
        break;  //next x
      }
    }
  }

//  cout << "concats: " << res << endl;
  return res;
}


long long resolve(){
  long long res = 0;

  solve_step1();
//  int concats = solve_step2();
//  int concats = solve_step2_2();
  long long concats = solve_step2_3();
  res = max4 + max7 + concats;

//  cout << "res: " << res << endl;
  return res;
}

int main()
{
//  int arr[] = {4,4,3,3,2,2,1,1};
////  int arr[] = {1,1,2,2,3,3,4,4};
//  vector<int> tbl(arr, arr + sizeof(arr) / sizeof(arr[0]) );
//  vector<int>::iterator it;
//  it = tbl.begin();
//  advance(it, 3);
//  it = lower_bound(it, tbl.end(), 4, cmp_gr);
////  it = lower_bound(it, tbl.end(), 4);
//  if( it == tbl.end() )
//    cout << "end" << endl;
//  else
//    cout << it - tbl.begin() << endl;
//  return 0;

//  int arr[] = {10,9,9,8,6,6,5,5,4,4,4,3,3,2,1};
////  it = lower_bound(tbl.begin(), tbl.end(), 4);
////  cout << it - tbl.begin() << endl;
//  it = lower_bound(tbl.begin(), tbl.end(), 4, cmp_gr);
//  cout << it - tbl.begin() << endl;
//  it = lower_bound(tbl.begin(), tbl.end(), 1, cmp_gr);
//  cout << it - tbl.begin() << endl;
////  it = lower_bound(tbl.begin(), tbl.end(), 4, cmp_gr2);
////  cout << it - tbl.begin() << endl;
////  it = upper_bound(tbl.begin(), tbl.end(), 4);
////  cout << it - tbl.begin() << endl;
////  it = upper_bound(tbl.begin(), tbl.end(), 4, cmp_gr);
////  cout << it - tbl.begin() << endl;
////  it = upper_bound(tbl.begin(), tbl.end(), 4, cmp_gr2);
////  cout << it - tbl.begin() << endl;
//  return 0;

    //rmq testing
//  int arr[] = {3,7,4,1,8,10,0,2,44,5,4};
//  vector<int> tbl(arr, arr + sizeof(arr) / sizeof(arr[0]) );
//  RMQsqrt rmq(&tbl);
//
//  cout << rmq.query(2, 5) << endl;  //3
//  cout << rmq.query(2, 6) << endl;  //6
//  cout << rmq.query(8, 9) << endl;  //9
//  cout << rmq.query(5, 5) << endl;  //5
//  cout << rmq.query(0, sizeof(arr)-1) << endl;  //6
//  cout << rmq.query(1, 4) << endl;  //3
//  cout << rmq.query(1, 7) << endl;  //6
//
//  return 0;

    //testing lowerbound
//  vector< pair<int,int> > tbl;
//  tbl.push_back( make_pair(4, 1) );
//  tbl.push_back( make_pair(6, 2) );
////  vector< pair<int, int> >::iterator it = lower_bound(tbl.begin(), tbl.end(), make_pair(4, 0), cmp_el);
////  vector< pair<int, int> >::iterator it = lower_bound(tbl.begin(), tbl.end(), 4, cmp_el2);
////  vector< pair<int, int> >::iterator it = upper_bound(tbl.begin(), tbl.end(), make_pair(4, 0), cmp_el);
//  vector< pair<int, int> >::iterator start = tbl.begin();
//  advance(start, 1);
//  vector< pair<int, int> >::iterator it = upper_bound(start, tbl.end(), 6, cmp_el3);
//  if(it == tbl.end())
//    cout << "out of table" << endl;
//  else {
//    cout << "val: " << (*it).first << endl;
//    cout << "index: " << (it - tbl.begin()) << endl;
//  }
//  return 0;

//  cout << numeric_limits<int>::max() << endl;
//  int t = numeric_limits<int>::max();//1;
////  cout << (t < numeric_limits<int>::max()) << endl;
//  if(t == numeric_limits<int>::max())
//    cout << "equal" << endl;
//  else
//    cout << t << endl;
//  return 0;

    //test string finding
//  StrEntry e("??7?4?44??7??");
////  e.print();
//  cout << "0: " << findMinChanges(1, 1, e, 2) << endl; //0
//  cout << "-1: " <<  findMinChanges(3, 4, e, 2) << endl; //-1
//  cout << "3: " <<  findMinChanges(3, 4, e, 3) << endl; //3
//  StrEntry e2("?74?4?77");
////  e.print();
//  cout << "0: " <<  findMinChanges(1, 1, e2, 2) << endl; //0
//  cout << "0: " <<  findMinChanges(2, 1, e2, 2) << endl; //0
//  cout << "0: " <<  findMinChanges(2, 2, e2, 2) << endl; //0
//  cout << "1: " <<  findMinChanges(3, 2, e2, 2) << endl; //1
//  cout << "-1: " <<  findMinChanges(3, 2, e2, 0) << endl; //-1
//  cout << "-1: " <<  findMinChanges(5, 2, e2, 0) << endl; //-1
//  cout << "3: " <<  findMinChanges(5, 2, e2, 3) << endl; //3
//  cout << "3: " <<  findMinChanges(5, 2, e2, 4) << endl; //3
//  cout << "1: " <<  findMinChanges(2, 3, e2, 4) << endl; //1
//  cout << "1: " <<  findMinChanges(2, 3, e2, 1) << endl; //1
//  cout << "-1: " <<  findMinChanges(2, 3, e2, 0) << endl; //-1
//  StrEntry e3("??????");
////  e3.print();
//  cout << "2: " <<  findMinChanges(1, 1, e3, 2) << endl;
//  cout << "-1: " <<  findMinChanges(1, 1, e3, 1) << endl;
//  StrEntry e4("447");
//  cout << "0: " <<  findMinChanges(1, 1, e4, 2) << endl;
//  cout << "0: " <<  findMinChanges(2, 1, e4, 2) << endl;
//  cout << "-1: " <<  findMinChanges(2, 2, e4, 2) << endl;
//  StrEntry e5("477");
//  cout << "0: " <<  findMinChanges(1, 1, e5, 2) << endl;
//  cout << "-1: " <<  findMinChanges(2, 1, e5, 2) << endl;
//  cout << "-1: " <<  findMinChanges(2, 2, e5, 2) << endl;
//  StrEntry e6("47");
//  cout << "0: " <<  findMinChanges(1, 1, e6, 2) << endl;
//  StrEntry e7("74");
//  cout << "-1: " <<  findMinChanges(1, 1, e7, 2) << endl;
//  StrEntry e8("4747");
////  e8.print();
//  cout << "0: " <<  findMinChanges(2, 1, e8, 1) << endl;
//  cout << "-1: " <<  findMinChanges(2, 2, e8, 1) << endl;
//  StrEntry e9("47474747");
////  e9.print();
//  cout << "-1: " <<  findMinChanges(2, 4, e9, 1) << endl;
//  StrEntry e10("???744?74?74?7444?4??74?447?4??4774477");
////  e10.print();
//  cout << "0: " <<  findMinChanges(14, 1, e10, 52) << endl;
//  cout << "0: " <<  findMinChanges(15, 1, e10, 52) << endl;
//  cout << "11: " <<  findMinChanges(26, 1, e10, 52) << endl;
//  cout << "12: " <<  findMinChanges(27, 1, e10, 52) << endl;
//  cout << "13: " <<  findMinChanges(28, 1, e10, 52) << endl;
//  StrEntry e11("4?474?4444474?7474??4???4???77?77?4747?4447474?74?????7474??7???????77474?77?");
////  e11.print();
//  cout << "2: " <<  findMinChanges(27, 1, e11, 52) << endl;
//  StrEntry e12("4477474744777?74?4??477?74?4744?7474?4?4?7774?4474?477747?7?7?747777??4?447777??74?4??7?????74?44");
////  e12.print();
//  cout << "0: " <<  findMinChanges(1, 1, e12, 0) << endl;
//  StrEntry e13("4474744?7?77??7477???7??4?4774447777?77?477?7477??474?4?7?77744?77477?44?4?4?4?");
////  e12.print();
//  cout << "0: " <<  findMinChanges(1, 1, e13, 0) << endl;
//  StrEntry e14("44??4?74?7?7?4??4??44??????7???44?77?4???74?74744??47?4444?74447?7??4444??7?47??4?477??444?47?47444?");
//  cout << "1: " <<  findMinChanges(1, 20, e14, 9) << endl;
//  StrEntry e15("774744??447?4??74?7??77?47??47??477??47477?44??44444477???7444?7?7?74777?????77?4?7??4?44??44?44?4??");
//  cout << "14: " <<  findMinChanges(46, 1, e15, 100) << endl;
//  return 0;

    //read cin from file
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen10_test.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen10_test3.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen100.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen100_2.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen1000.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen47_100.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen47_100_3.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen47_1000_10.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\genX_100.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen100k.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen100k_X.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen10k_X.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen50k.txt");
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen50k_X.txt");
  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen100k_47_X.txt");  //20.9 sek, najlepsze: 0.858 zeszlo do 0.2
//  ifstream fin("d:\\projects\\codechef\\luckycom\\gen\\bin\\Debug\\gen100k_X_47.txt");  //4.55 sek, najlepsze 1.1 (559139467) zeszlo do 1.5 sek (optymalizacja 0.17sek)
  cin.rdbuf(fin.rdbuf());

    //speedup cin
//  ios::sync_with_stdio(false);
//  cin.tie(0);

  clock_t tbegin, tend;
  tbegin = clock();

  cin >> n >> k;

  min4 = min7 = numeric_limits<int>::max();
  data.reserve(n);
  for(int i=0; i<n; i++){
    string s;
    cin >> s;
    StrEntry* e = new StrEntry(s);
    data.push_back(e);
    if(min4 > e->total4) min4 = e->total4;
    if(min7 > e->total7) min7 = e->total7;
  }

  long long res = resolve();
  cout << res << endl;

  for(int i=0; i<data.size(); i++)
    delete data[i];
  data.resize(0);

  tend = clock();
  double elapsed_secs = double(tend - tbegin) / CLOCKS_PER_SEC;
  cout << "elapsed: " << elapsed_secs << endl;

  return 0;
}
