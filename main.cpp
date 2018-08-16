#include "pacman.hpp"

#include "coefficient_registry.hpp"
#include "text.hpp"
#include "log.hpp"
#include <sdlpp/sdlpp.hpp>
#include <shade/obj.hpp>
#include <shade/shader_program.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>

static const char Level[] = "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
                            "H...................................H.H............H"
                            "H.HHHHHHHHHHHHHHHHH.HHHHHH.HHHHHHH..H.H............H"
                            "H........................H.H.....H..H.H............H"
                            "H.H.HHH.H.H.H..H.H..H..H.H.H.......................H"
                            "H.H..H..H.H.HHHH.HHHHHHHHH.HHHHHHH..H.H............H"
                            "H.H.HHH.H.H......H...............H..H.H............H"
                            "H.H..H..H.H.HHHH.H.HHHHHHHHHHHHH.H..H.H............H"
                            "H.H.HHH.H.H........H...........H.H..H.HHHHHHH.HHHHHH"
                            "H.H..H..H.H.HHHH.H.H...........H.H.................H"
                            "H.H.HHH.H.H......H.HHHHHHHH.HHHH.H..H.H.H.H.H.H....H"
                            "H.H..H..H.H.HHHH.H...............H..H.H.H.H.H.H.HH.H"
                            "H................HHHHHHHHH.HHHHHHH..H.H.H.H.H.H....H"
                            "H..HHHHHHHHHHHHH.H.......H.H.....H..H.H...H.H.H.HH.H"
                            "H..................................................H"
                            "H..HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH..H.H.H.H.H.H.HH.H"
                            "H...................................H.H.H.H........H"
                            "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
static const auto LevelWidth = 52;

COEFF(CameraMoveK, 0.06f);
COEFF(CameraZ, -20.0f);
COEFF(TextX, -12);
COEFF(TextY, -6);
COEFF(TextZoom, 0.3f);

int main()
{
  static_assert((sizeof(Level) - 1) % LevelWidth == 0, "Incorrect level width");

  CoefficientRegistry::instance().load();

  sdl::Init init(SDL_INIT_EVERYTHING);
  const auto Width = 1280;
  const auto Height = 720;
  sdl::Window window("Pacman", 64, 100, Width, Height, SDL_WINDOW_OPENGL);
  sdl::Renderer renderer(window.get(), -1, SDL_RENDERER_PRESENTVSYNC);
  sdl::EventHandler eventHandler;
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  auto done = false;
  eventHandler.quit = [&done](const SDL_QuitEvent &) { done = true; };
  TextureLibrary textureLibrary(renderer.get());
  std::string level = Level;
  Pacman pacman(textureLibrary, [&level](int x, int y) -> char & {
    return level[x + y * LevelWidth];
  });

  Var<glm::mat4> projection(
    "projection",
    glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f));
  Var<glm::mat4> view("view");
  Var<glm::mat4> mvp("mvp");

  ShaderProgram pacmanShaider("pacman", "pacman", mvp, projection, view);

  Text coeffText(textureLibrary, "font");
  coeffText.setText(CoefficientRegistry::instance().display());
  eventHandler.keyDown = [&pacman, &coeffText](const SDL_KeyboardEvent &e) {
    switch (e.keysym.sym)
    {
    case SDLK_LEFT: pacman.setDirection(Pacman::Direction::Left); break;
    case SDLK_RIGHT: pacman.setDirection(Pacman::Direction::Right); break;
    case SDLK_UP: pacman.setDirection(Pacman::Direction::Up); break;
    case SDLK_DOWN: pacman.setDirection(Pacman::Direction::Down); break;
    case SDLK_PAGEUP:
      CoefficientRegistry::instance().rotate(true);
      LOG(CoefficientRegistry::instance().display());
      coeffText.setText(CoefficientRegistry::instance().display());
      break;
    case SDLK_PAGEDOWN:
      CoefficientRegistry::instance().rotate(false);
      LOG(CoefficientRegistry::instance().display());
      coeffText.setText(CoefficientRegistry::instance().display());
      break;
    case SDLK_KP_PLUS:
      CoefficientRegistry::instance().change(1);
      LOG(CoefficientRegistry::instance().display());
      coeffText.setText(CoefficientRegistry::instance().display());
      break;
    case SDLK_KP_MINUS:
      CoefficientRegistry::instance().change(-1);
      LOG(CoefficientRegistry::instance().display());
      coeffText.setText(CoefficientRegistry::instance().display());
      break;
    }
  };
  eventHandler.mouseWheel = [&coeffText](const SDL_MouseWheelEvent& e)
    {
      CoefficientRegistry::instance().change(e.y);
      LOG(CoefficientRegistry::instance().display());
      coeffText.setText(CoefficientRegistry::instance().display());
    };
  Obj dot(textureLibrary, "dot");
  Obj wall(textureLibrary, "wall");

  float camX = 0;
  float camY = 0;

  while (!done)
  {
    while (eventHandler.poll()) {}
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    pacmanShaider.use();
    projection.update();

    camX = CameraMoveK * pacman.getDisplayX() + (1.0f - CameraMoveK) * camX;
    camY = CameraMoveK * pacman.getDisplayY() + (1.0f - CameraMoveK) * camY;
    view = glm::lookAt(glm::vec3(camX * 2.0f, CameraZ, camY * 2.0f),
                       glm::vec3(camX * 2.0f, 0, camY * 2.0f), // and looks at the origin
                       glm::vec3(0, 0, 1));
    view.update();

    pacman.draw(mvp);

    for (size_t y = 0; y < (sizeof(Level) - 1) / LevelWidth; ++y)
      for (size_t x = 0; x < LevelWidth; ++x)
      {
        mvp = glm::translate(glm::vec3(2.0f * x, 0.0f, 2.0f * y));
        mvp.update();
        switch (level[x + y * LevelWidth])
        {
        case 'H': wall.activate(); break;
        case '.': dot.activate(); break;
        }
      }

    for (float x = 2.0f; x < 10.0f; x += 2.0f)
    {
      mvp = glm::translate(glm::vec3(x, 0.0f, 0.0f));
      mvp.update();
      dot.activate();
    }

    {
      view = glm::lookAt(glm::vec3(0, CameraZ, 0),
                         glm::vec3(0, 0, 0), // and looks at the origin
                         glm::vec3(0, 0, 1));
      view.update();
      mvp = glm::translate(glm::vec3(TextX, -3.0f, TextY)) *
            glm::scale(glm::vec3(TextZoom, TextZoom, TextZoom));
      mvp.update();
      coeffText.draw();
    }

    renderer.present();
  }
}
