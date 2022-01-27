#ifndef NOTMARIOGAME_GAMEOVER_HPP
#define NOTMARIOGAME_GAMEOVER_HPP

#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>

class GameOver
{
 public:
  GameOver(ASGE::Renderer& renderer_ref, ASGE::Point2D screen_size);
  void setSpritePos(float x);
  void render();

 private:
  ASGE::Renderer& renderer;
  std::unique_ptr<ASGE::Sprite> menu;
};

#endif // NOTMARIOGAME_GAMEOVER_HPP
