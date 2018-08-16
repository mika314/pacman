#include "coefficient.hpp"
#include <cmath>
#include <cpptoml/cpptoml.h>

BaseCoefficient::BaseCoefficient(const char *aName) : name(aName) {}

namespace Internal
{
  void change(float &value, int changeAmount) { value *= pow(1.01, changeAmount); }
  void change(int &value, int changeAmount) { value += changeAmount; }
  void serialize(cpptoml::table *root, const char *name, float value)
  {
    root->insert(name, static_cast<double>(value));
  }
  void serialize(cpptoml::table *root, const char *name, int value) { root->insert(name, value); }
  void deserialize(cpptoml::table *root, const char *name, float &value)
  {
    value = root->get_as<double>(name).value_or(value);
  }
  void deserialize(cpptoml::table *root, const char *name, int &value)
  {
    value = root->get_as<int>(name).value_or(value);
  }
} // namespace Internal
