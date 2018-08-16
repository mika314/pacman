#pragma once
#include <string>

namespace cpptoml
{
  class table;
}

class BaseCoefficient
{
public:
  BaseCoefficient(const char *name);
  virtual std::string display() const = 0;
  virtual void change(int) = 0;
  virtual void serialize(cpptoml::table *) const = 0;
  virtual void deserialize(cpptoml::table *) = 0;
  virtual ~BaseCoefficient() = default;

protected:
  const char *name;
};

namespace Internal
{
  void change(float &, int);
  void change(int &, int);
  void serialize(cpptoml::table *root, const char* name, float);
  void serialize(cpptoml::table *root, const char* name, int);
  void deserialize(cpptoml::table *root, const char* name, float&);
  void deserialize(cpptoml::table *root, const char* name, int&);
}

template <typename T>
class Coefficient : public BaseCoefficient
{
public:
  Coefficient(T &aValue, const char *name) : BaseCoefficient(name), value(aValue) {}
  std::string display() const override { return name + std::string(":") + std::to_string(value); }
  void change(int changeAmount) override { Internal::change(value, changeAmount); }
  void serialize(cpptoml::table *root) const override { Internal::serialize(root, name, value); }
  void deserialize(cpptoml::table *root) override
  {
    Internal::deserialize(root, name, value);
  }

private:
  T &value;
};
