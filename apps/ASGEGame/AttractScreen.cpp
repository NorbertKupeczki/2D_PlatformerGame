
#include "AttractScreen.hpp"
#include <cmath>

AttractScreen::AttractScreen(ASGE::Renderer& renderer_ref, ASGE::Point2D screen_size) :
  renderer(renderer_ref)
{
  menu = renderer.createUniqueSprite();
  if (menu->loadTexture("/data/img/menu.png"))
  {
    ASGE::Point2D img_dimensions = { menu->width(), menu->height() };
    menu->yPos(screen_size.y * 0.5F - img_dimensions.y * 0.5F);
    menu->xPos(screen_size.x * 0.5F - img_dimensions.x * 0.5F);
    menu->setGlobalZOrder(5);
  }
}

void AttractScreen::render()
{
  renderer.renderSprite(*menu);
}

void AttractScreen::moveSprite(float x)
{
  menu->xPos(std::round(menu->xPos() + x));
}

void AttractScreen::setSpritePos(float x)
{
  menu->xPos(static_cast<float>(x - menu->width() * 0.5));
}
