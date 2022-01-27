#include "TileSquare.hpp"

TileSquare::TileSquare(
  ASGE::Renderer& renderer_ref, const tmx::Tileset::Tile* tile, int mag, int x, int y) :
  renderer(renderer_ref),
  visibility(true), state(PASSABLE)
{
  sprite = renderer.createUniqueSprite();
  if (sprite->loadTexture(tile->imagePath))
  {
    // The rectangle will use [0] and [1] for the beginning x and y
    // position in the texture and [2] and [3] for the width and height.
    sprite->srcRect()[0] = static_cast<float>(tile->imagePosition.x);
    sprite->srcRect()[1] = static_cast<float>(tile->imagePosition.y);
    sprite->srcRect()[2] = static_cast<float>(tile->imageSize.x);
    sprite->srcRect()[3] = static_cast<float>(tile->imageSize.y);

    sprite->width(static_cast<float>(tile->imageSize.x));
    sprite->height(static_cast<float>(tile->imageSize.y));

    sprite->scale(static_cast<float>(mag));
    sprite->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);

    sprite->yPos(static_cast<float>(y));
    sprite->xPos(static_cast<float>(x));
  }

  if (state == PASSABLE)
  { /* To avoid compile error due to not using the state */
  }
}

TileSquare::TileSquare(ASGE::Renderer& renderer_ref, bool is_visible) :
  sprite(nullptr), renderer(renderer_ref), visibility(is_visible), state(PASSABLE)
{
}

void TileSquare::renderTile()
{
  if (visibility)
  {
    renderer.renderSprite(*sprite);
  }
}

bool TileSquare::isPassable()
{
  return state == PASSABLE;
}

void TileSquare::setToBlocking()
{
  state = BLOCKING;
}

void TileSquare::setToDestroyable()
{
  state = DESTROYABLE;
}

void TileSquare::collision()
{
  //  if (state == DESTROYABLE)
  //  {
  //    visibility = false;
  //    state      = PASSABLE;
  //  }
}
