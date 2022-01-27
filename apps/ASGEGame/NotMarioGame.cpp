
#include "NotMarioGame.hpp"

/// Initialises the game.
/// Setup your game and initialise the core components.
/// @param settings
NotMarioGame::NotMarioGame(const ASGE::GameSettings& settings) :
  OGLGame(settings), game_state(ATTRACT_SCREEN),
  key_callback_id(inputs->addCallbackFnc(ASGE::E_KEY, &NotMarioGame::keyHandler, this)),
  mouse_callback_id(inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &NotMarioGame::clickHandler, this)),
  tile_map(magnification, sound), controller(Controller(inputs->getGamePad(0))),
  mario(Mario(*renderer, magnification, tile_map)),
  camera(
    *renderer, static_cast<float>(settings.window_width),
    static_cast<float>(settings.window_height)),
  att_screen(
    *renderer,
    { static_cast<float>(settings.window_width), static_cast<float>(settings.window_height) }),
  game_over_screen(
    *renderer,
    { static_cast<float>(settings.window_width), static_cast<float>(settings.window_height) })
{
  renderer->setClearColour(ASGE::COLOURS::CORNFLOWERBLUE);
  //  toggleFPS();
  tile_map.initMap(*renderer);
}

/// Destroys the game.
NotMarioGame::~NotMarioGame()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));
  this->inputs->unregisterCallback(static_cast<unsigned int>(mouse_callback_id));
}

/// Processes key inputs.
/// This function is added as a callback to handle the game's keyboard input.
/// For this game, calls to this function are not thread safe, so you may alter
/// the game's state but your code needs to designed to prevent data-races.
/// @param data
/// @see KeyEvent
void NotMarioGame::keyHandler(ASGE::SharedEventData data)
{
  const auto* key = dynamic_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }
}

/// Updates your game and all it's components using a fixed
/// timestep. This allows calculations to resolve correctly and
/// stop physics simulations from imploding
/// https://gamedev.stackexchange.com/questions/1589/when-should-i-use-a-fixed-or-variable-time-step
/// @param us
void NotMarioGame::update(const ASGE::GameTime& us)
{
  controller.updateController(inputs->getGamePad(0));

  if (game_state == ATTRACT_SCREEN)
  {
    att_screen.moveSprite(camera.moveCameraAttractScreen(static_cast<float>(us.deltaInSecs())));
    if (controller.xPressed())
    {
      camera.resetCamera();
      game_state = PLAYING;
    }
  }
  else if (game_state == GAME_OVER)
  {
    if (controller.jumpPressed())
    {
      camera.resetCamera();
      att_screen.setSpritePos(camera.getCameraX());
      mario.resetMario();
      tile_map.resetGameObjects();
      game_state = ATTRACT_SCREEN;
    }
    else if (controller.xPressed())
    {
      camera.resetCamera();
      mario.resetMario();
      tile_map.resetGameObjects();
      game_state = PLAYING;
    }
  }

  if (controller.exitPressed())
  {
    Logging::DEBUG("Exiting game");
    signalExit();
  }
}

/// Simulates the visuals and renders your game scenes.
/// These use a variable time-step, which allows for extra smoothing and
/// interpolation based solely on the framerate of the game.
/// @param us
void NotMarioGame::render(const ASGE::GameTime& us)
{
  if (game_state == PLAYING)
  {
    float move_x = 0.0F;

    if (std::abs(controller.leftStickState().x) > 0.15)
    {
      move_x = static_cast<float>(controller.leftStickState().x * us.deltaInSecs());
    }
    if (controller.jumpPressed())
    {
      if (!mario.isJumping())
      {
        mario.jump(static_cast<float>(us.deltaInSecs()));
        sound.playJump();
      }
    }

    mario.moveMario(move_x, static_cast<float>(us.deltaInSecs()));
    camera.moveCamera(mario.getX());

    if (!mario.isInGame())
    {
      game_state = GAME_OVER;
      game_over_screen.setSpritePos(camera.getCameraX());
    }
  }

  camera.setProjectionMatrix();

  for (auto& sprite : tile_map.getTiles())
  {
    sprite->renderTile();
  }
  tile_map.updateGameObjects(static_cast<float>(us.deltaInSecs()));

  if (game_state == ATTRACT_SCREEN)
  {
    att_screen.render();
  }
  else if (game_state == PLAYING)
  {
    camera.renderText(mario.getScore());
  }
  else if (game_state == GAME_OVER)
  {
    game_over_screen.render();
  }
  mario.render();
}

/// @brief   Processes any click inputs
/// @details This function is added as a callback to handle the game's
///          mouse button input. For this game, calls to this function
///          are thread safe, so you may alter the game's state as you
///          see fit.
/// @param   data The event data relating to key input.
/// @see     ClickEvent
/// @return  void
void NotMarioGame::clickHandler(ASGE::SharedEventData data)
{
  const auto* click = dynamic_cast<const ASGE::ClickEvent*>(data.get());
  if (click->action == ASGE::MOUSE::BUTTON_PRESSED)
  {
    double x_pos = click->xpos;
    double y_pos = click->ypos;

    //  Logging::DEBUG("x_pos: " + std::to_string(x_pos));
    //  Logging::DEBUG("y_pos: " + std::to_string(y_pos));

    int tile_x      = static_cast<int>(x_pos / tile_map.getTileSize());
    int tile_y      = static_cast<int>(y_pos / tile_map.getTileSize());
    auto tile_index = static_cast<unsigned int>(tile_x + (tile_y * 211));

    Logging::DEBUG("Tile space: " + std::to_string(tile_x) + "/" + std::to_string(tile_y));
    //  Logging::DEBUG("Tile index: " + std::to_string(tile_index));

    if (tile_map.checkCollision(static_cast<int>(tile_index)))
    {
      Logging::DEBUG("Collision");
    }
    else
    {
      Logging::DEBUG("Passable");
    }
  }
}
