#include "GameObject.hpp"

void GameObject::render()
{
  // Placeholder
}

void GameObject::setUpAnimation()
{
  //  Placeholder
  //  animation.emplace_back(SrcRect{ 0, 0, 0, 0 });
}

void GameObject::setSrcRect()
{
  sprite->srcRect()[0] = static_cast<float>(animation[animation_index].x_pos);
  sprite->srcRect()[1] = static_cast<float>(animation[animation_index].y_pos);
  sprite->srcRect()[2] = static_cast<float>(animation[animation_index].w);
  sprite->srcRect()[3] = static_cast<float>(animation[animation_index].h);
}
