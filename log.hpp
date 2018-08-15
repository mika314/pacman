#pragma once
#include <iostream>

namespace Internal
{
  inline void logToStrm(std::ostream &strm) { strm << std::endl; }

  template <typename Arg, typename... Args>
  void logToStrm(std::ostream &strm, Arg &&arg, Args &&... args)
  {
    strm << " " << arg;
    logToStrm(strm, args...);
  }

  template <typename... Args>
  void log(const char *fileName, int line, Args &&... args)
  {
    std::cout << fileName << ":" << line;
    logToStrm(std::cout, args...);
  }
} // namespace Internal

#define LOG(...) Internal::log(__FILE__, __LINE__, __VA_ARGS__)
