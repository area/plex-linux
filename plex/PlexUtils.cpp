#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "PlexUtils.h"

using namespace std;
using namespace boost;

///////////////////////////////////////////////////////////////////////////////
bool IsValidIP(const std::string& address)
{
  bool valid = false;
  
  vector<string> octets;
  split(octets, address, is_any_of("."));
  if (octets.size() == 4)
  {
    BOOST_FOREACH(string octet, octets)
    {
      try { int i = lexical_cast<int>(octet); if (i < 0 || i > 255) return false; }
      catch (...) { return false; }
    }
    
    valid = true;
  }    
  
  return valid;
}

///////////////////////////////////////////////////////////////////////////////
string GetHostName() 
{
  char hostname[256];
  gethostname(hostname, 256);
  string friendlyName = hostname;
  boost::replace_last(friendlyName, ".local", "");
  
  return friendlyName;
}

///////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  // Define a structure to receive the current Windows filetime
  FILETIME ft;
 
  // Initialize the present time to 0 and the timezone to UTC
  unsigned __int64 tmpres = 0;
  static int tzflag = 0;
 
  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);
 
    // The GetSystemTimeAsFileTime returns the number of 100 nanosecond 
    // intervals since Jan 1, 1601 in a structure. Copy the high bits to 
    // the 64 bit tmpres, shift it left by 32 then or in the low 32 bits.
    //
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
 
    // Convert to microseconds by dividing by 10
    tmpres /= 10;
 
    // The Unix epoch starts on Jan 1 1970.  Need to subtract the difference 
    // in seconds from Jan 1 1601.
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
 
    // Finally change microseconds to seconds and place in the seconds value. 
    // The modulus picks up the microseconds.
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }
 
  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
  
    // Adjust for the timezone west of Greenwich
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }
 
  return 0;
}

int usleep(useconds_t useconds)
{
  Sleep(useconds / 1000);
  return 0;
}

#endif
