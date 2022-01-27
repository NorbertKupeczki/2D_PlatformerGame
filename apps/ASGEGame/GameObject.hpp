
#ifndef NOTMARIOGAME_GAMEOBJECT_HPP
#define NOTMARIOGAME_GAMEOBJECT_HPP

#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>
#include <vector>

class GameObject
{
 public:
  GameObject()          = default;
  virtual ~GameObject() = default;

  virtual void render();

 protected:
  virtual void setUpAnimation();
  virtual void setSrcRect();

  std::unique_ptr<ASGE::Sprite> sprite;

  struct SrcRect
  {
    float x_pos;
    float y_pos;
    float w;
    float h;
  } __attribute__((aligned(16)));

  std::vector<SrcRect> animation;
  unsigned int animation_index     = 0;
  const float ANIMATION_FRAME_RATE = 0.03F;
  float animation_timer            = 0.0F;
};

#endif // NOTMARIOGAME_GAMEOBJECT_HPP
