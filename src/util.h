#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

inline
std::string transfer_text(uint64_t bps) {
  size_t unit = 0;
  while(bps >= 1000) {
    bps /= 1000;
    ++unit;
  }

  std::string result = std::to_string(bps);

  if(unit == 0) result += "bps";
  else if(unit == 1) result += "Kbps";
  else if(unit == 2) result += "Mbps";
  else if(unit == 3) result += "Gbps";
  else if(unit == 4) result += "Tbps";

  return result;
}

#endif // __UTIL_H__
