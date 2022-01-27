#ifndef NOTMARIOGAME_CONTROLLER_HPP
#define NOTMARIOGAME_CONTROLLER_HPP

#include <Engine/OGLGame.hpp>

class Controller
{
 public:
  explicit Controller(ASGE::GamePadData game_pad_data);

  void updateController(ASGE::GamePadData game_pad_data);
  [[nodiscard]] ASGE::Point2D leftStickState() const;
  [[nodiscard]] bool jumpPressed() const;
  [[nodiscard]] bool exitPressed() const;
  [[nodiscard]] bool xPressed() const;

 private:
  ASGE::GamePadData game_pad;
};

#endif // NOTMARIOGAME_CONTROLLER_HPP
