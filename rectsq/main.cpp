/*
  RECTSQ
  http://www.codechef.com/problems/RECTSQ

  pomysl:
  trzeba obliczyc najwiekszy wspolny dzielnik (https://en.wikipedia.org/wiki/Binary_GCD_algorithm)
  potem iloczyn dzielen
*/

#include <iostream>

using namespace std;


unsigned int binary_gcd( unsigned int u, unsigned int v)
{
  int shift;

  /* GCD(0,v) == v; GCD(u,0) == u, GCD(0,0) == 0 */
  if (u == 0) return v;
  if (v == 0) return u;

  /* Let shift := lg K, where K is the greatest power of 2
        dividing both u and v. */
  for (shift = 0; ((u | v) & 1) == 0; ++shift) {
         u >>= 1;
         v >>= 1;
  }

  while ((u & 1) == 0)
    u >>= 1;

  /* From here on, u is always odd. */
  do {
       /* remove all factors of 2 in v -- they are not common */
       /*   note: v is not zero, so while will terminate */
       while ((v & 1) == 0)  /* Loop X */
           v >>= 1;

       /* Now u and v are both odd. Swap if necessary so u <= v,
          then set v = v - u (which is even). For bignums, the
          swapping is just pointer movement, and the subtraction
          can be done in-place. */
       if (u > v) {
         unsigned int t = v; v = u; u = t;}  // Swap u and v.
       v = v - u;                       // Here v >= u.
     } while (v != 0);

  /* restore common factors of 2 */
  return u << shift;
}

int main()
{
  int t;
  unsigned int m, n;
  unsigned int gcd, sqs;

  cin >> t;
  for(int i=0; i<t; i++){
    cin >> m >> n;

    gcd = binary_gcd(m, n);
//    cout << "gcd=" << gcd << endl;
    sqs = (m/gcd) * (n/gcd);

    cout << sqs << endl;
  }

    return 0;
}
