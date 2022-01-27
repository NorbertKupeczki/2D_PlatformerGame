#ifndef NOTMARIOGAME_MARIO_HPP
#define NOTMARIOGAME_MARIO_HPP

#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>
#include <GameObject.hpp>
#include <Map.hpp>
#include <vector>

class Mario : public GameObject
{
 public:
  Mario(ASGE::Renderer& renderer_ref, int magnification, Map& map);

  void moveMario(float x, float dt);
  void jump(float dt);
  void resetMario();
  bool isJumping();
  bool isInGame();
  float getX();
  [[nodiscard]] int getScore() const;

  void render() override;

 private:
  void initMario(int magnification);
  void setUpAnimation() override;
  void run(float dt);
  ASGE::Point2D checkCollision();

  ASGE::Point2D start_pos = { 384.0F, 576.0F };
  bool is_in_game         = true;

  enum States
  {
    IDLE    = 0,
    RUNNING = 1,
    JUMPING = 2
  };

  States state;
  ASGE::Renderer& renderer;

  float speed               = 570.0F;
  float jump_speed          = 1000.0F;
  const float GRAVITY       = 40.0F;
  ASGE::Point2D move_vector = { 0.0F, 0.0F };
  Map& tile_map;
  int score = 0;
};

#endif // NOTMARIOGAME_MARIO_HPP
