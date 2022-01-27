#ifndef NOTMARIOGAME_MAP_HPP
#define NOTMARIOGAME_MAP_HPP

#include <Engine/FileIO.hpp>
#include <Engine/Sprite.hpp>

#include <Coin.hpp>
#include <Enemy.hpp>
#include <Sound.hpp>
#include <TileSquare.hpp>
#include <tmxlite/Map.hpp>
#include <vector>

class Map
{
 public:
  Map(int magnification, Sound& sound_ref);
  ~Map() = default;
  void initMap(ASGE::Renderer& renderer);
  void updateGameObjects(float dt);
  void resetGameObjects();
  bool isPassable(unsigned int index);
  [[nodiscard]] float getTileSize() const;
  std::vector<std::unique_ptr<TileSquare>>& getTiles();
  bool checkCollision(int index);
  int coinCollision(ASGE::Point2D mario_center);
  int enemyCollision(ASGE::Sprite& mario, bool mario_falls);
  [[nodiscard]] bool checkExit(ASGE::Point2D mario_pos) const;

 private:
  void buildTiles(ASGE::Renderer& renderer, tmx::Layer& layer);

  int magnification = 3;
  int tile_size     = 16;
  ASGE::FILEIO::File tile_map;
  ASGE::FILEIO::IOBuffer buffer;
  tmx::Map map;
  std::vector<std::unique_ptr<TileSquare>> tiles;
  std::vector<std::unique_ptr<Coin>> coins;
  std::vector<std::unique_ptr<Enemy>> enemies;
  Sound& sound;
  ASGE::Point2D exit_pos  = { 9792.0F, 528.0F };
  ASGE::Point2D exit_size = { 48.0F, 96.0F };
};

#endif // NOTMARIOGAME_MAP_HPP
