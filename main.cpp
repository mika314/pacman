#include <sdlpp/sdlpp.hpp>
#include <shade/obj.hpp>
#include <shade/shader_program.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>

int main()
{
  sdl::Init init(SDL_INIT_EVERYTHING);
  const auto Width = 1280;
  const auto Height = 720;
  sdl::Window window("Pacman", 64, 126, Width, Height, SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL);
  sdl::Renderer renderer(window.get(), -1, SDL_RENDERER_PRESENTVSYNC);
  sdl::EventHandler eventHandler;
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  auto done = false;
  eventHandler.quit = [&done](const SDL_QuitEvent &) { done = true; };
  TextureLibrary textureLibrary(renderer.get());
  Obj pacman(textureLibrary, "pacman");
  Obj dot(textureLibrary, "dot");
  glm::mat4 Projection =
    glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f);
  glm::mat4 View = glm::lookAt(glm::vec3(0, -10, 3),
                               glm::vec3(0, 0, 0), // and looks at the origin
                               glm::vec3(0, 0, 1)
  );
  Var<glm::mat4> mvp("mvp");
  ShaderProgram pacmanShaider("pacman", "pacman", mvp);

  while (!done)
  {
    while (eventHandler.poll()) {}
    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    pacmanShaider.use();
    mvp = Projection * View * glm::mat4(1);
    mvp.update();

    pacman.activate();

    for (float x = 1.0f; x < 10.0f; x += 2.0f)
    {
      mvp = Projection * View * glm::translate(glm::vec3(x, 0.0f, 0.0f));
      mvp.update();
      dot.activate();
    }
    renderer.present();
  }
}
