#include "text.hpp"
#include <shade/texture_library.hpp>

const auto CharWidth = 59;
const auto CharHeight = 115;

Text::Text(TextureLibrary &textureLibrary, const std::string &fontTexture)
  : texture(textureLibrary.get(fontTexture))
{
}

void Text::setText(const std::string &text)
{
  // TODO  
}

void Text::draw()
{
  if (!vertices || !uvs || !texture)
    return;
  vertices->activate();
  uvs->activate();
  texture->glBind(nullptr, nullptr);
  glDrawArrays(GL_TRIANGLES, 0, n);
}
