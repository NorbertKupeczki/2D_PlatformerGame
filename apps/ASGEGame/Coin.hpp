
#ifndef NOTMARIOGAME_COIN_HPP
#define NOTMARIOGAME_COIN_HPP

#include <Engine/Sprite.hpp>
#include <GameObject.hpp>
#include <vector>

class Coin : public GameObject
{
 public:
  Coin(ASGE::Renderer& renderer_ref, ASGE::Point2D spawn_pos);

  void render() override;
  void playAnimation(float dt);
  void setVisibility(bool v);
  int getValue();
  ASGE::Sprite& getSprite();
  [[nodiscard]] bool isVisible() const;

 private:
  ASGE::Renderer& renderer;
  void setUpAnimation() override;

  bool visibility = true;
  int value       = 100;
};

#endif // NOTMARIOGAME_COIN_HPP
