#include "pacman.hpp"

#include <shade/obj.hpp>

Pacman::Pacman(TextureLibrary &textureLibrary, std::function<char &(int, int)> &&aMap):
  map(aMap)
{
  for (int i = 1; i <= 5; ++i)
    frames.push_back(std::make_unique<Obj>(textureLibrary, "pacman" + std::to_string(i), "pacman"));
}

float Pacman::getDisplayX() const
{
  return displayX;
}

float Pacman::getDisplayY() const
{
  return displayY;
}

void Pacman::draw(Var<glm::mat4> &mvp)
{
  mvp = glm::translate(glm::vec3(2.0f * displayX, 0.0f, 2.0f * displayY));
  mvp.update();
  auto f = frame++ / 3 % (frames.size() * 2);
  if (f >= frames.size())
    f = frames.size() * 2 - 1 - f;
  auto&& tail = map(x, y);
  tail = ' ';
  frames[f]->activate();
  auto newX = x;
  auto newY = y;
  if (frame % 12 == 0)
  {
    switch (direction)
    {
    case Direction::Left:
      --newX;
      break;
    case Direction::Right:
      ++newX;
      break;
    case Direction::Up:
      ++newY;
      break;
    case Direction::Down:
      --newY;
      break;
    }
  }
  if (map(newX, newY) != 'H')
  {
    x = newX;
    y = newY;
  }

  displayX = 0.9f * displayX + 0.1f * x;
  displayY = 0.9f * displayY + 0.1f * y;
}

void Pacman::setDirection(Direction value)
{
  direction = value;
}
