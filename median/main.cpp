/*
  MEDIAN
  https://www.codechef.com/problems/MEDIAN

  pomysl (PD):
  - tabela enkodowana do liczby binarnej, gdzie 1 - max, 0 - pozostale wartosci
  - w kazdym kroku analizujemy wszystkie mozliwe kombincje rozszerzania 1, dla kazdej kombinacji generujemy maksymalna tablice 1 jaka mozna z niej utowrzyc:
    kazda maska podzielona na bloki 1 i 0
    kazdy blok rozszerzany w obie strony (obejmujemy tym wszystkie przypadki)
    jezeli osiagniemy same 1 w tablicy to mamy rozwiazanie w biezacym kroku
  - dodatkowo tablica asocjacyjna (map) kombinacji juz sprawdzonych - tych nie analizujemy ponownie (nie dodajemy ich do listy dla nastepnego kroku)
  - z kazdego kroku wynikowo wychodzi tablica z maskami o maksymalnej liczbie 1 (n masek o liczbie 1 np 20)
*/
#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct status{
public:
  status() : bits(0), ones(0) {};
  status(unsigned int i1, int i2) : bits(i1), ones(i2) {};

  unsigned int bits;  //encoded table
  int ones; //count of ones
};

int t;
int n;
vector<int> tbl;
vector<status> states, states2;
map<unsigned int, bool> checkedstates;

void encodetable(status& r){
  int vmax = -1;
    //find max
  for(int i=0; i<n; i++)
    if( tbl[i] > vmax) vmax=tbl[i];

    //encode
  r.bits = 0;
  r.ones = 0;
  unsigned int mask = 1;
  for( int i=0; i<n; i++ ){
    if(tbl[i] == vmax){
      r.bits |= mask;
      r.ones++;
    }
    mask = mask << 1;
  }
}

void bitstoblocks(vector< pair<int, int> >& blocks, unsigned int bits){
  blocks.resize(0);

  unsigned int mask = 1;
  for(int i=0; i<n; i++){
    int val = 0;
    if( (bits & mask) != 0) val = 1;

    if( (blocks.size()==0) || (blocks[blocks.size()-1].first != val) ){
      blocks.push_back( make_pair(val, 1) );
    } else {
      blocks[blocks.size()-1].second++;
    }

    mask = mask << 1;
  }
}

unsigned int blockstobits(vector< pair<int, int> >& blocks, int& totalonescnt){
  unsigned int res = 0;
  unsigned int mask = 1;
  totalonescnt = 0;
  for(int k=0; k<blocks.size(); k++){
    if( blocks[k].first == 0 )  //0 - shift mask
      mask = mask << blocks[k].second;
    else{  //1 - set bits in result
      for(int i=0; i<blocks[k].second; i++){
        res |= mask;
        mask = mask << 1;
      }
      totalonescnt += blocks[k].second;
    }
  }
  return res;
}

void coverblocksingle(vector< pair<int, int> >& blocks, int iones, int icovered, int& onesleft){
  if( blocks[icovered].second > onesleft ){
    blocks[icovered].second -= onesleft;
    blocks[iones].second += onesleft;
    onesleft = 0;
  } else {
    blocks[iones].second += blocks[icovered].second;
    onesleft -= blocks[icovered].second;
    blocks[icovered].second = 0;
  }
}

void coverblockwithnext(vector< pair<int, int> >& blocks, int iones, int icovered, int icoverednext, int& onesleft){
  onesleft = (onesleft + blocks[icoverednext].second) - blocks[icovered].second;
  blocks[iones].second += blocks[icovered].second + blocks[icoverednext].second;
  blocks[icovered].second = 0;
  blocks[icoverednext].second = 0;
}

int scanfornonempty0(vector< pair<int, int> >& blocks, int ifrom, int dir){
  int i = ifrom;// + dir;
  while( (i>=0) && (i<blocks.size()) ){
    if( (blocks[i].first == 0) && (blocks[i].second > 0) )
      return i;
    else
      i += dir;
  }
  return -1;
}

void printblock(vector< pair<int, int> >& blocks){
  for(int i=0; i<blocks.size(); i++){
    cout << "(" << blocks[i].first << "," << blocks[i].second << "),";
  }
  cout << endl;
}

unsigned int enlargeblock(vector< pair<int, int> >& blocks, int index, int& curronescovered){
    //boundary cases
//  if( index - 1 < 0 ) return 0; //left bound
//  if( index + 1 >= blocks.size() ) return 0;  //right bound

//  cout << index << ":" << endl;
//  printblock(blocks);

  vector< pair<int, int> > work;
  work = blocks;

  int onesleft = work[index].second;
  int ileft, iright;
  ileft = iright = index;
  while( (onesleft>0) && (work[index].second<n) ) {
      //check left block only, left + next with ones, the same for right block
      //choose one option that gives highest coverage, or leaves smallest gap
    int bestcover = -1;
    int covercnt = 0;
    ileft = scanfornonempty0(work, ileft, -1);
    iright = scanfornonempty0(work, iright, 1);
    if( (ileft == -1) && (iright == -1 )) break;  //no zeroes left

    if( (iright > -1) && (iright + 1 < work.size()) )
      if(work[iright + 1].second + onesleft >= work[iright].second) {bestcover = 3; covercnt = work[iright + 1].second + work[iright].second;}
    if( (ileft > -1) && (ileft - 1 >=0) )
      if( (work[ileft - 1].second + onesleft >= work[ileft].second) && (work[ileft].second + work[ileft - 1].second > covercnt) ) {bestcover = 1; covercnt = work[ileft].second + work[ileft - 1].second;}
    if(bestcover == -1){
      if(iright == -1) bestcover = 0;
      else if(ileft == -1) bestcover = 2;
      else if(work[iright].second > work[ileft].second) bestcover = 2; else bestcover = 0;
  //    covercnt = onesleft;
    }
    switch( bestcover ){
    case 0 : //left only
      coverblocksingle(work, index, ileft, onesleft);
      break;
    case 1 : //left + next
      coverblockwithnext(work, index, ileft, ileft - 1, onesleft);
      break;
    case 2 : //right only
      coverblocksingle(work, index, iright, onesleft);
      break;
    case 3 : //right + next
      coverblockwithnext(work, index, iright, iright + 1, onesleft);
      break;
    }
//    printblock(work);
  }

  return blockstobits(work, curronescovered);
}

void analyzeblocks(vector< pair<int, int> >& blocks, vector<status>& snext, int& maxones){
  for(int i=0; i<blocks.size(); i++){
    if( blocks[i].first == 0 ) continue;
    //analysis only for block of 1
    unsigned int newbits;
    int onescovered;
    newbits = enlargeblock(blocks, i, onescovered);
    if( onescovered >= maxones ){ //only greater or equal to current max
      if(checkedstates.find(newbits) == checkedstates.end()){ //not analyzed yet
        snext.push_back( status(newbits, onescovered) );
        checkedstates[newbits] = true;
      }
      maxones = onescovered;
    }
    if( maxones == n ) return;
  }
}

int processtoend(int initialmaxones){
  if( initialmaxones == n ) return 0;

  int currmaxones = initialmaxones;
  int maxones;
  int steps=1;
  vector<status>& scurr = states;
  vector<status>& snext = states2;
  vector< pair<int, int> > blocks;  //blocks: first - 0 or 1, second - count of elements

  for(int x=0; x<5; x++){ //no more than 5 steps for 30 bits number
//    cout << "step: " << steps << endl;
    maxones = currmaxones;
    checkedstates.clear();
    for(int i=0; i<scurr.size(); i++){
      if(scurr[i].ones != currmaxones) continue;  //only max
        //bits to blocks
      bitstoblocks(blocks, scurr[i].bits);
        //analyze blocks
      analyzeblocks(blocks, snext, maxones);
      if( maxones == n ) return steps; //if found setup with all 1 return steps
    }
      //swap status tables
    vector<status>& t = scurr;
    scurr = snext;
    snext = t;
    snext.resize(0);
    snext.reserve(n*n);
    currmaxones = maxones;

    steps++;
  }
  return -1;  //some error occurred in analysis
}

int resolve(){
  states.reserve(n*n);
  states2.reserve(n*n);

    //initial state
  status r;
  encodetable(r);
  states.push_back(r);
//  checkedstates[r.bits] = true;

    //process
  int res = processtoend(r.ones);
  return res;
}

int main()
{
//    //speedup cin
//  ios::sync_with_stdio(false);
//  cin.tie(0);

  cin >> t;
  while(t>0){
    cin >> n;

    tbl.resize(n);
//    states.resize(n*n);
    for(int i=0; i<n; i++)
      cin >> tbl[i];

    int res;
    res = resolve();
    cout << res << endl;

    checkedstates.clear();
    states.resize(0);
    states2.resize(0);
    tbl.resize(0);

    t--;
  }

  return 0;
}
