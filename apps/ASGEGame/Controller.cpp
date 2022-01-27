#include "Controller.hpp"
#include <Engine/Logger.hpp>

Controller::Controller(ASGE::GamePadData game_pad_data) : game_pad(game_pad_data)
{
  if (game_pad.is_connected)
  {
    Logging::DEBUG("Gamepad is connected");
    Logging::DEBUG(game_pad.name);
  }
  else
  {
    Logging::ERRORS("Gamepad is NOT CONNECTED!");
  }
}

void Controller::updateController(ASGE::GamePadData game_pad_data)
{
  game_pad = game_pad_data;
}

ASGE::Point2D Controller::leftStickState() const
{
  return { game_pad.axis[0], game_pad.axis[1] };
}

bool Controller::jumpPressed() const
{
  return game_pad.buttons[0] != 0U;
}

bool Controller::exitPressed() const
{
  return game_pad.buttons[1] != 0U;
}

bool Controller::xPressed() const
{
  return game_pad.buttons[2] != 0U;
}
