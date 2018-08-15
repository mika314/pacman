#pragma once
#include <memory>
#include <sdlpp/sdlpp.hpp>
#include <shade/array_buffer.hpp>

class TextureLibrary;
class Text
{
public:
  Text(TextureLibrary &, const std::string &fontTexture);
  void setText(const std::string &);
  void draw();

private:
  sdl::Texture *texture;
  std::unique_ptr<ArrayBuffer> vertices;
  std::unique_ptr<ArrayBuffer> uvs;
  int n;
};
