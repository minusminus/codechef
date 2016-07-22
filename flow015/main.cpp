/*
  FLOW015
  http://www.codechef.com/problems/FLOW015

  pomysl:
  utworzyc date systemowa i sprawdzic dzien tygodnia
*/

#include <iostream>
#include <ctime>

//using namespace std;

int main()
{
  time_t rawtime;
  struct tm * timeinfo;
  const char * weekday[] = { "sunday", "monday",
                             "tuesday", "wednesday",
                             "thursday", "friday", "saturday"};

  int t, year;

  std::cin >> t;
  for( int i=0; i<t; i++ ){
    std::cin >> year;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
//    timeinfo = gmtime(&rawtime);
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = 0;
    timeinfo->tm_mday = 1;
    mktime(timeinfo);

    std::cout << weekday[timeinfo->tm_wday] << std::endl;
  }

    return 0;
}
