
#include "Coin.hpp"

Coin::Coin(ASGE::Renderer& renderer_ref, ASGE::Point2D spawn_pos) : renderer(renderer_ref)
{
  Coin::setUpAnimation();

  sprite = renderer.createUniqueSprite();
  if (sprite->loadTexture("/data/img/coin.png"))
  {
    GameObject::setSrcRect();

    sprite->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);

    sprite->width(36.0F);
    sprite->height(36.0F);

    sprite->xPos(spawn_pos.x + 6);
    sprite->yPos(spawn_pos.y + 6);

    sprite->setGlobalZOrder(1);
  }
}

void Coin::render()
{
  if (visibility)
  {
    renderer.renderSprite(*sprite);
  }
}

void Coin::playAnimation(float dt)
{
  animation_timer += dt * 0.5F;
  if (animation_timer >= ANIMATION_FRAME_RATE)
  {
    animation_timer = 0.0F;
    if (animation_index > 4)
    {
      animation_index = 0;
    }
    else
    {
      ++animation_index;
    }
  }
  setSrcRect();
}

void Coin::setUpAnimation()
{
  animation.emplace_back(SrcRect{ 0, 0, 36, 36 });
  animation.emplace_back(SrcRect{ 0, 37, 36, 36 });
  animation.emplace_back(SrcRect{ 0, 74, 36, 36 });
  animation.emplace_back(SrcRect{ 0, 111, 36, 36 });
  animation.emplace_back(SrcRect{ 0, 148, 36, 36 });
  animation.emplace_back(SrcRect{ 0, 185, 36, 36 });
}

int Coin::getValue()
{
  if (visibility)
  {
    visibility = false;
    return value;
  }
  return 0;
}

ASGE::Sprite& Coin::getSprite()
{
  return *sprite;
}

void Coin::setVisibility(bool v)
{
  visibility = v;
}

bool Coin::isVisible() const
{
  return visibility;
}
