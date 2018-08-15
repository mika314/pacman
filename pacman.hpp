#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <functional>
#include <memory>
#include <shade/var.hpp>
#include <vector>

class TextureLibrary;
class Obj;
class Pacman
{
public:
  Pacman(TextureLibrary &, std::function<char &(int, int)> &&aMap);
  float getDisplayX() const;
  float getDisplayY() const;
  void draw(Var<glm::mat4> &mvp);
  enum class Direction { Right = 0, Down, Left, Up };
  void setDirection(Direction);

private:
  float displayX = 26.0f;
  float displayY = 9.0f;
  int x = 26;
  int y = 9;
  int frame = 0;
  Direction direction = Direction::Right;
  float displayDirection = static_cast<float>(direction);
  std::vector<std::unique_ptr<Obj>> frames;
  std::function<char &(int, int)> map;
};
