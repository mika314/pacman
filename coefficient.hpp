#pragma once
#include <string>

class BaseCoefficient
{
public:
  BaseCoefficient(const char *name);
  virtual std::string display() const = 0;
  virtual void change(int) = 0;
  virtual ~BaseCoefficient() = default;

protected:
  const char *name;
};

namespace Internal
{
  void change(float &, int);
  void change(int &, int);
}

template <typename T>
class Coefficient : public BaseCoefficient
{
public:
  Coefficient(T &aValue, const char *name) : BaseCoefficient(name), value(aValue) {}
  std::string display() const override { return name + std::string(":") + std::to_string(value); }
  void change(int changeAmount) override { Internal::change(value, changeAmount); }

private:
  T &value;
};
