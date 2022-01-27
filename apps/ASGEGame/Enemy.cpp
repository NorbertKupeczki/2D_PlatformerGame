
#include "Enemy.hpp"

Enemy::Enemy(ASGE::Renderer& renderer_ref, ASGE::Point2D spawn_pos) : renderer(renderer_ref)
{
  Enemy::setUpAnimation();
  animation_index = 1;

  sprite = renderer.createUniqueSprite();
  if (sprite->loadTexture("/data/img/enemies.png"))
  {
    GameObject::setSrcRect();

    sprite->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);

    sprite->width(36.0F);
    sprite->height(36.0F);

    sprite->xPos(spawn_pos.x + 6);
    sprite->yPos(spawn_pos.y + 12);

    sprite->setGlobalZOrder(2);
  }
}

void Enemy::render()
{
  if (visibility)
  {
    renderer.renderSprite(*sprite);
  }
}

void Enemy::playAnimation(float dt)
{
  animation_timer += dt * 0.5F;
  if (animation_timer >= ANIMATION_FRAME_RATE)
  {
    animation_timer = 0.0F;
    if (animation_index == 2)
    {
      animation_index = 1;
    }
    else
    {
      ++animation_index;
    }
  }
  setSrcRect();
}

void Enemy::setUpAnimation()
{
  // Index 0 - Dead
  animation.emplace_back(SrcRect{ 60, 0, 16, 16 });
  // Index 1-2 - Walking
  animation.emplace_back(SrcRect{ 0, 4, 16, 16 });
  animation.emplace_back(SrcRect{ 30, 4, 16, 16 });
}

int Enemy::getValue()
{
  if (visibility)
  {
    is_jumped       = true;
    animation_index = 0;
    setSrcRect();
    return value;
  }
  return 0;
}

ASGE::Sprite& Enemy::getSprite()
{
  return *sprite;
}

bool Enemy::isVisible() const
{
  return visibility;
}

void Enemy::update(float dt)
{
  if (!is_jumped)
  {
    playAnimation(dt);
    sprite->xPos(sprite->xPos() + speed * dt);
  }
  else if (visible_timer > 0.0F)
  {
    visible_timer -= dt;
  }
  else
  {
    visibility = false;
  }
}

int Enemy::getFrontIndex(float dt, int tile_size)
{
  ASGE::Point2D future_coords = { 0.0F, 0.0F };
  if (speed > 0.0F)
  {
    future_coords = { sprite->getWorldBounds().v2.x + speed * dt, sprite->getWorldBounds().v2.y };
  }
  else if (speed < 0.0F)
  {
    future_coords = { sprite->getWorldBounds().v1.x + speed * dt, sprite->getWorldBounds().v1.y };
  }

  int tile_x     = static_cast<int>(future_coords.x / static_cast<float>(tile_size * 3));
  int tile_y     = static_cast<int>(future_coords.y / static_cast<float>(tile_size * 3));
  int tile_index = static_cast<int>(tile_x + (tile_y * 211));

  return tile_index;
}

void Enemy::turn()
{
  speed *= -1;
}

bool Enemy::isJumped() const
{
  return is_jumped;
}

void Enemy::reset()
{
  animation_index = 1;
  setSrcRect();
  is_jumped     = false;
  visibility    = true;
  visible_timer = 1.0F;
}
