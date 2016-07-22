/*
  FLOW016
  http://www.codechef.com/problems/FLOW016

  pomysl:
  gcd z binarygcd (rectsq)
  lcm: https://en.wikipedia.org/wiki/Least_common_multiple
*/

#include <iostream>

using namespace std;

//unsigned int binary_gcd( unsigned int u, unsigned int v)
unsigned long long int binary_gcd( unsigned long long int u, unsigned long long int v)
{
  int shift;

  if (u == 0) return v;
  if (v == 0) return u;

  for (shift = 0; ((u | v) & 1) == 0; ++shift) {
         u >>= 1;
         v >>= 1;
  }

  while ((u & 1) == 0)
    u >>= 1;

  do {
       while ((v & 1) == 0)
           v >>= 1;

       if (u > v) {
         unsigned int t = v; v = u; u = t;}
       v = v - u;
     } while (v != 0);

  return u << shift;
}



int main()
{
  int t;
//  unsigned int a, b;
//    unsigned int gcd, lcm;
  unsigned long long int a, b;
  unsigned long long int gcd, lcm;

  cin >> t;
  for( int i=0; i<t; i++ ){

    cin >> a >> b;
    gcd = binary_gcd(a, b);
//    lcm = a*(b/gcd);
    lcm = (a*b)/gcd;

    cout << gcd << " " << lcm << endl;
  }

    return 0;
}
