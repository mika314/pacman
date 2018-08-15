#include "coefficient.hpp"
#include <cmath>

BaseCoefficient::BaseCoefficient(const char *aName) : name(aName) {}

namespace Internal
{
  void change(float &value, int changeAmount) { value *= pow(1.01, changeAmount); }
  void change(int &value, int changeAmount) { value += changeAmount; }
} // namespace Internal
