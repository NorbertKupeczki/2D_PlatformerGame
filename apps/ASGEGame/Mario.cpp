#include "Mario.hpp"
#include <Engine/Logger.hpp>
#include <cmath>

Mario::Mario(ASGE::Renderer& renderer_ref, int magnification, Map& map) :
  state(IDLE), renderer(renderer_ref), tile_map(map)
{
  initMario(magnification);
}

void Mario::initMario(int magnification)
{
  Mario::setUpAnimation();

  sprite = renderer.createUniqueSprite();
  if (sprite->loadTexture("/data/img/mario-luigi-bordered.png"))
  {
    Logging::DEBUG("Mario initialized");
    setSrcRect();

    sprite->width(static_cast<float>(16.0F));
    sprite->height(static_cast<float>(16.0F));

    sprite->scale(static_cast<float>(magnification));
    sprite->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
    sprite->xPos(start_pos.x);
    sprite->yPos(start_pos.y);

    sprite->setGlobalZOrder(10);
  }
}

void Mario::moveMario(float x, float dt)
{
  move_vector.x = x * speed;
  move_vector.y += GRAVITY * dt;

  if (move_vector.x == 0.0F && state != JUMPING)
  {
    state           = IDLE;
    animation_index = 0;
    setSrcRect();
  }
  else
  {
    if (state != JUMPING)
    {
      if (state == IDLE)
      {
        animation_index = 2;
        state           = RUNNING;
      }
      run(dt);
    }
  }

  if (sprite->yPos() > 0)
  {
    move_vector = checkCollision();
  }

  float new_x = sprite->xPos() + move_vector.x;
  sprite->yPos(sprite->yPos() + move_vector.y);

  if (new_x < -3)
  {
    sprite->xPos(-3);
  }
  else if (new_x > 10083)
  {
    sprite->xPos(10083);
  }
  else
  {
    sprite->xPos(std::round(new_x));
  }
}

float Mario::getX()
{
  return sprite->xPos();
}

void Mario::setUpAnimation()
{
  // Index 0, Idle
  animation.emplace_back(SrcRect{ 80, 34, 16, 16 });
  // Index 1-3, Run animation
  animation.emplace_back(SrcRect{ 97, 34, 16, 16 });
  animation.emplace_back(SrcRect{ 114, 34, 16, 16 });
  animation.emplace_back(SrcRect{ 131, 34, 16, 16 });
  // Index 4, Jump
  animation.emplace_back(SrcRect{ 165, 34, 16, 16 });
  // Index 5, Dead
  animation.emplace_back(SrcRect{ 182, 34, 16, 16 });
}

void Mario::run(float dt)
{
  animation_timer += dt;
  if (animation_timer >= ANIMATION_FRAME_RATE)
  {
    static bool is_increasing = true;
    animation_timer           = 0.0F;
    if (animation_index % 2 == 1)
    {
      is_increasing = !is_increasing;
    }
    if (is_increasing)
    {
      ++animation_index;
    }
    else
    {
      --animation_index;
    }
  }
  setSrcRect();
}

bool Mario::isJumping()
{
  return state == JUMPING;
}

void Mario::jump(float dt)
{
  if (move_vector.y < 0.1F)
  {
    state           = JUMPING;
    animation_index = 4;
    setSrcRect();
    move_vector.y -= jump_speed * dt;
  }
}

ASGE::Point2D Mario::checkCollision()
{
  ASGE::Point2D new_move_vector = move_vector;

  std::vector<ASGE::Point2D> bounds;
  bounds.emplace_back(ASGE::Point2D(sprite->getWorldBounds().v1));
  bounds.emplace_back(ASGE::Point2D(sprite->getWorldBounds().v2));
  bounds.emplace_back(ASGE::Point2D(sprite->getWorldBounds().v3));
  bounds.emplace_back(ASGE::Point2D(sprite->getWorldBounds().v4));

  for (ASGE::Point2D coords : bounds)
  {
    int tile_x      = static_cast<int>((coords.x + move_vector.x) / tile_map.getTileSize());
    int tile_y      = static_cast<int>(coords.y / tile_map.getTileSize());
    auto tile_index = static_cast<unsigned int>(tile_x + (tile_y * 211));

    if (tile_map.checkCollision(static_cast<int>(tile_index)))
    {
      if (move_vector.x > 0.0F)
      {
        new_move_vector.x = 0.0;
        sprite->xPos(static_cast<float>(tile_x - 1) * tile_map.getTileSize() - 1.0F);
      }
      else
      {
        new_move_vector.x = 0.0;
        sprite->xPos(static_cast<float>(tile_x + 1) * tile_map.getTileSize());
      }
    }

    tile_x     = static_cast<int>(coords.x / tile_map.getTileSize());
    tile_y     = static_cast<int>((coords.y + move_vector.y) / tile_map.getTileSize());
    tile_index = static_cast<unsigned int>(tile_x + (tile_y * 211));
    if (tile_map.checkCollision(static_cast<int>(tile_index)))
    {
      if (move_vector.y > 0.0F)
      {
        if (std::abs(move_vector.x) > 0.0F && state == JUMPING)
        {
          state           = RUNNING;
          animation_index = 2;
        }
        else if (std::abs(move_vector.x) == 0.0F && state == JUMPING)
        {
          state           = IDLE;
          animation_index = 0;
        }
        new_move_vector.y = 0.0;
        sprite->yPos(static_cast<float>(tile_y - 1) * tile_map.getTileSize() - 0.5F);
      }
      else
      {
        new_move_vector.y = 0.0;
        sprite->yPos(static_cast<float>(tile_y + 1) * tile_map.getTileSize());
      }
    }
  }
  ASGE::Point2D mario_center = {
    sprite->getWorldBounds().v1.x + sprite->width() * 0.5F * sprite->scale(),
    sprite->getWorldBounds().v1.y + sprite->height() * 0.5F * sprite->scale()
  };
  score += tile_map.coinCollision(mario_center);
  int enemy_collision = tile_map.enemyCollision(*sprite, new_move_vector.y > 0.0F);
  if (enemy_collision < 0)
  {
    animation_index = 5;
    setSrcRect();
    is_in_game = false;
  }
  else if (enemy_collision > 0)
  {
    state           = JUMPING;
    animation_index = 4;
    setSrcRect();
    new_move_vector.y = -5.0F;
    score += enemy_collision;
  }
  return new_move_vector;
}

void Mario::resetMario()
{
  is_in_game      = true;
  score           = 0;
  animation_index = 0;
  setSrcRect();
  sprite->xPos(start_pos.x);
  sprite->yPos(start_pos.y);
}

bool Mario::isInGame()
{
  if (
    sprite->yPos() > 710.0F ||
    tile_map.checkExit(ASGE::Point2D(
      sprite->xPos() + sprite->width() * 0.5F, sprite->yPos() + sprite->height() * 0.5F)))
  {
    is_in_game = false;
  }
  return is_in_game;
}

void Mario::render()
{
  renderer.renderSprite(*sprite);
}

int Mario::getScore() const
{
  return score;
}
