#ifndef NOTMARIOGAME_ATTRACTSCREEN_HPP
#define NOTMARIOGAME_ATTRACTSCREEN_HPP

#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>

class AttractScreen
{
 public:
  AttractScreen(ASGE::Renderer& renderer_ref, ASGE::Point2D screen_size);
  void moveSprite(float x);
  void render();
  void setSpritePos(float x);

 private:
  ASGE::Renderer& renderer;
  std::unique_ptr<ASGE::Sprite> menu;
};

#endif // NOTMARIOGAME_ATTRACTSCREEN_HPP
