#include "pacman.hpp"
#include "coefficient_registry.hpp"

#include <shade/obj.hpp>

COEFF(PacmanMoveK, 0.2f);
COEFF(FPS, 40);

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
  mvp =
    glm::translate(glm::vec3(2.0f * displayX, 0.0f, 2.0f * displayY)) *
    glm::rotate(static_cast<float>(M_PI / 2.0f * displayDirection), glm::vec3(0.0f, 1.0f, 0.0f));
  mvp.update();
  auto f = (frame++ * FPS / 60) % (frames.size() * 2);
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


  displayX = (1.0f - PacmanMoveK) * displayX + PacmanMoveK * x;
  displayY = (1.0f - PacmanMoveK) * displayY + PacmanMoveK * y;
  displayDirection = (1.0f - PacmanMoveK) * displayDirection + PacmanMoveK * static_cast<float>(direction);
  if (displayDirection - static_cast<float>(direction) > 2.0f)
    displayDirection -= 4.0f;
  if (displayDirection - static_cast<float>(direction) < -2.0f)
    displayDirection += 4.0f;
}

void Pacman::setDirection(Direction value)
{
  direction = value;
}
