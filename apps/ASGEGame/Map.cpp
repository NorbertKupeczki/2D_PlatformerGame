#include "Map.hpp"

#include <cmath>
#include <iostream>
#include <tmxlite/TileLayer.hpp>

Map::Map(int mag, Sound& sound_ref) : magnification(mag), sound(sound_ref) {}

void Map::initMap(ASGE::Renderer& renderer)
{
  if (tile_map.open("data/map/World_Map.tmx"))
  {
    buffer = tile_map.read();
    std::string file_string(buffer.as_char(), buffer.length);

    map.loadFromString(file_string, ".");

    for (const auto& layer : map.getLayers())
    {
      if (layer->getType() == tmx::Layer::Type::Tile)
      {
        buildTiles(renderer, *layer);
      }
    }
  }
}

void Map::buildTiles(ASGE::Renderer& renderer, tmx::Layer& layer)
{
  auto tile_layer = layer.getLayerAs<tmx::TileLayer>();
  int y           = 0;
  for (unsigned int row = 0; row < layer.getSize().y; ++row)
  {
    int x = 0;
    for (unsigned int col = 0; col < layer.getSize().x; ++col)
    {
      auto tile_info   = tile_layer.getTiles()[row * tile_layer.getSize().x + col];
      const auto* tile = map.getTilesets()[0].getTile(tile_info.ID);
      if (layer.getName() == "Coins")
      {
        if (tile != nullptr)
        {
          coins.emplace_back(std::make_unique<Coin>(
            renderer, ASGE::Point2D(static_cast<float>(x), static_cast<float>(y))));
        }
      }
      else if (layer.getName() == "Enemies")
      {
        if (tile != nullptr)
        {
          enemies.emplace_back(std::make_unique<Enemy>(
            renderer, ASGE::Point2D(static_cast<float>(x), static_cast<float>(y))));
        }
      }
      else
      {
        if (tile != nullptr)
        {
          auto& tile_temp =
            tiles.emplace_back(std::make_unique<TileSquare>(renderer, tile, magnification, x, y));
          if (layer.getName() == "Pipes" || layer.getName() == "Ground")
          {
            tile_temp->setToBlocking();
          }
          else if (layer.getName() == "Blocks")
          {
            tile_temp->setToDestroyable();
          }
        }
        else
        {
          tiles.emplace_back(std::make_unique<TileSquare>(renderer, false));
        }
      }
      x += tile_size * magnification;
    }
    y += tile_size * magnification;
  }
}

std::vector<std::unique_ptr<TileSquare>>& Map::getTiles()
{
  return tiles;
}

float Map::getTileSize() const
{
  return static_cast<float>(tile_size * magnification);
}

bool Map::isPassable(unsigned int index)
{
  return tiles[index]->isPassable();
}

bool Map::checkCollision(int index)
{
  for (int i = static_cast<int>(index); i < static_cast<int>(tiles.size());
       i     = i + static_cast<int>(tiles.size() / 5))
  {
    if (!isPassable(static_cast<unsigned int>(i)))
    {
      tiles[static_cast<unsigned int>(i)]->collision();
      return true;
    }
  }
  return false;
}

bool Map::checkExit(ASGE::Point2D mario_pos) const
{
  return (
    mario_pos.x > exit_pos.x && mario_pos.x < exit_pos.x + exit_size.x &&
    mario_pos.y > exit_pos.y && mario_pos.y < exit_pos.y + exit_size.y);
}

int Map::coinCollision(ASGE::Point2D mario_center)
{
  for (auto& coin : coins)
  {
    if (coin->isVisible())
    {
      ASGE::Point2D coin_center = {
        coin->getSprite().getWorldBounds().v1.x + coin->getSprite().width() * 0.5F,
        coin->getSprite().getWorldBounds().v1.y + coin->getSprite().height() * 0.5F
      };

      double dist =
        sqrt(pow(coin_center.x - mario_center.x, 2) + pow(coin_center.y - mario_center.y, 2));

      if (dist < 36.0F)
      {
        sound.playCoin();
        return coin->getValue();
      }
    }
  }
  return 0;
}

int Map::enemyCollision(ASGE::Sprite& mario, bool mario_falls)
{
  for (auto& goomba : enemies)
  {
    if (goomba->isVisible() && !goomba->isJumped())
    {
      if (
        mario.getWorldBounds().v2.x - 7 >= goomba->getSprite().getWorldBounds().v1.x &&
        mario.getWorldBounds().v1.x + 7 <= goomba->getSprite().getWorldBounds().v2.x &&
        mario.getWorldBounds().v3.y >= goomba->getSprite().getWorldBounds().v1.y &&
        mario.getWorldBounds().v2.y <= goomba->getSprite().getWorldBounds().v3.y && mario_falls)
      {
        sound.playGoomba();
        return goomba->getValue();
      }
      else if (
        mario.getWorldBounds().v2.x - 7 >= goomba->getSprite().getWorldBounds().v1.x &&
        mario.getWorldBounds().v1.x + 7 <= goomba->getSprite().getWorldBounds().v2.x &&
        mario.getWorldBounds().v3.y >= goomba->getSprite().getWorldBounds().v1.y &&
        mario.getWorldBounds().v2.y <= goomba->getSprite().getWorldBounds().v3.y)
      {
        return -1;
      }
    }
  }
  return 0;
}

void Map::updateGameObjects(float dt)
{
  for (auto& coin : coins)
  {
    coin->playAnimation(dt);
    coin->render();
  }

  for (auto& goomba : enemies)
  {
    if (
      checkCollision(goomba->getFrontIndex(dt, tile_size)) ||
      !checkCollision(goomba->getFrontIndex(dt, tile_size) + 211))
    {
      goomba->turn();
    }
    goomba->update(dt);
    goomba->render();
  }
}

void Map::resetGameObjects()
{
  for (auto& coin : coins)
  {
    coin->setVisibility(true);
  }

  for (auto& goomba : enemies)
  {
    goomba->reset();
  }
}
