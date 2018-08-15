#include "coefficient_registry.hpp"

CoefficientRegistry &CoefficientRegistry::instance()
{
  static CoefficientRegistry inst;
  return inst;
}

std::string CoefficientRegistry::display() const
{
  if (currentIdx >= data.size())
    return "Not selected";
  return data[currentIdx]->display();
}

void CoefficientRegistry::rotate(bool isUp)
{
  if (data.empty())
    return;
  if (isUp)
    currentIdx = (currentIdx + 1) % data.size();
  else
    currentIdx = (currentIdx + data.size() - 1) % data.size();
}

void CoefficientRegistry::change(int value)
{
  if (currentIdx >= data.size())
    return;
  data[currentIdx]->change(value);
}
