#ifndef NOTMARIOGAME_TILESQUARE_HPP
#define NOTMARIOGAME_TILESQUARE_HPP

#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>
#include <tmxlite/Tileset.hpp>

class TileSquare
{
 public:
  TileSquare(ASGE::Renderer& renderer_ref, const tmx::Tileset::Tile* tile, int mag, int x, int y);
  TileSquare(ASGE::Renderer& renderer_ref, bool is_visible);
  ~TileSquare() = default;

  void renderTile();
  bool isPassable();
  void setToBlocking();
  void setToDestroyable();
  void collision();

 private:
  std::unique_ptr<ASGE::Sprite> sprite;
  ASGE::Renderer& renderer;

  bool visibility;
  enum State
  {
    PASSABLE    = 0,
    BLOCKING    = 1,
    DESTROYABLE = 2,
    BONUS       = 3
  };
  State state;
};

#endif // NOTMARIOGAME_TILESQUARE_HPP
