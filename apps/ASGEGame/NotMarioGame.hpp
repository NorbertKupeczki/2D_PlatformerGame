#pragma once

#include <ASGEGameLib/GComponent.hpp>
#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>
#include <random>
#include <vector>

#include <AttractScreen.hpp>
#include <Camera.hpp>
#include <Controller.hpp>
#include <GameOver.hpp>
#include <Map.hpp>
#include <Mario.hpp>
#include <Sound.hpp>

class NotMarioGame : public ASGE::OGLGame
{
 public:
  explicit NotMarioGame(const ASGE::GameSettings& settings);
  ~NotMarioGame() override;
  NotMarioGame(const NotMarioGame&) = delete;
  NotMarioGame& operator=(const NotMarioGame&) = delete;

 private:
  void keyHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);
  void update(const ASGE::GameTime& us) override;
  void render(const ASGE::GameTime& us) override;

 private:
  enum GameStates
  {
    ATTRACT_SCREEN = 0,
    PLAYING        = 1,
    GAME_OVER      = 2
  };
  GameStates game_state;

  int magnification = 3;
  std::vector<std::unique_ptr<GameComponent>> game_components;
  int key_callback_id   = -1; /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */
  Map tile_map;
  Controller controller;
  Mario mario;
  Camera camera;
  AttractScreen att_screen;
  GameOver game_over_screen;
  Sound sound;
};
