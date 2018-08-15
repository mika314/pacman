#pragma once
#include "coefficient.hpp"
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class BaseCoefficient;
class CoefficientRegistry
{
public:
  static CoefficientRegistry &instance();
  template <typename T>
  void add(T &value, const char *name)
  {
    if (names.find(name) != std::end(names))
      throw std::runtime_error(std::string("Duplicate coefficient name: ") + name);
    data.push_back(std::make_unique<Coefficient<T>>(value, name));
    currentIdx = data.size() - 1;
  }
  std::string display() const;
  void rotate(bool);
  void change(int);

private:
  CoefficientRegistry() = default;
  std::vector<std::unique_ptr<BaseCoefficient>> data;
  std::unordered_set<std::string> names;
  size_t currentIdx = 0;
};

namespace Internal
{
  class Declare
  {
  public:
    template <typename T>
    Declare(T &value, const char *name)
    {
      CoefficientRegistry::instance().add(value, name);
    }
  };
} // namespace Internal

#define COEFF(x, y)  \
  static auto x = y; \
  Internal::Declare COEFF##x(x, #x);
