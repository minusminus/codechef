#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main()
{
  srand(time(NULL));

//  const int randomizer = 20;
//  const int maxnum = 200 / randomizer;
  const int seqs = 10;
  int randomizer;
  int maxnum;

  cout << seqs << endl;
  for(int s=0; s<seqs; s++){
    randomizer = (rand() % 200) + 1;
    maxnum = 200 / randomizer;

    cout << maxnum << endl;
    for( int i=0; i<maxnum; i++ ){
      int c = (rand() % (int)(randomizer)) + 1;
      cout << c << " ";
    }
    cout << endl;
  }

  return 0;
}
