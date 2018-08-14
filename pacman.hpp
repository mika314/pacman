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
  enum class Direction { Left, Right, Up, Down };
  void setDirection(Direction);

private:
  float displayX = 26.0f;
  float displayY = 9.0f;
  int x = 26;
  int y = 9;
  int frame = 0;
  Direction direction = Direction::Right;
  std::vector<std::unique_ptr<Obj>> frames;
  std::function<char &(int, int)> map;
};
