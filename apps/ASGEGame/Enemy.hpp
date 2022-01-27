
#ifndef NOTMARIOGAME_ENEMY_HPP
#define NOTMARIOGAME_ENEMY_HPP

#include <GameObject.hpp>

class Enemy : public GameObject
{
 public:
  Enemy(ASGE::Renderer& renderer_ref, ASGE::Point2D spawn_pos);

  void render() override;
  void playAnimation(float dt);
  void update(float dt);
  void turn();
  void reset();
  [[nodiscard]] bool isJumped() const;
  [[nodiscard]] bool isVisible() const;
  int getValue();
  int getFrontIndex(float dt, int tile_size);
  ASGE::Sprite& getSprite();

 private:
  void setUpAnimation() override;

  bool visibility = true;
  int value       = 200;
  ASGE::Renderer& renderer;
  float speed         = 150.0F;
  bool is_jumped      = false;
  float visible_timer = 1.0F;
};

#endif // NOTMARIOGAME_ENEMY_HPP
