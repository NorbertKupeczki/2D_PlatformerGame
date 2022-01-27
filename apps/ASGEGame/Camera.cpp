#include "Camera.hpp"
#include <cmath>

Camera::Camera(ASGE::Renderer& renderer_ref, float width, float height) :
  camera(ASGE::Camera{ width, height }), renderer(renderer_ref),
  camera_start_pos(width * 0.5F, height * 0.5F), screen_size(width, height),
  camera_min_x(camera_start_pos.x)
{
  resetCamera();
  initText();
}

void Camera::setProjectionMatrix()
{
  renderer.setProjectionMatrix(camera.getView());
}

void Camera::moveCamera(float mario_x)
{
  if (mario_x < camera_min_x)
  {
    camera.lookAt(ASGE::Point2D(camera_min_x, camera.position().y));
  }
  else if (mario_x > camera_max_x)
  {
    camera.lookAt(ASGE::Point2D(camera_max_x, camera.position().y));
  }
  else
  {
    camera.lookAt(ASGE::Point2D(mario_x, camera.position().y));
  }
}

float Camera::moveCameraAttractScreen(float dt)
{
  if (
    camera.position().x + (camera_x_speed * dt) > camera_max_x ||
    camera.position().x + (camera_x_speed * dt) < camera_min_x)
  {
    camera_x_speed *= -1.0F;
  }
  camera.translateX(std::round(camera_x_speed * dt));

  return camera_x_speed * dt;
}

void Camera::resetCamera()
{
  camera.lookAt(ASGE::Point2D(camera_start_pos.x, camera_start_pos.y));
}

float Camera::getCameraX()
{
  return camera.position().x;
}

void Camera::initText()
{
  if (auto idx = renderer.loadFont("/data/fonts/KGHAPPY.ttf", 20); idx > 0)
  {
    font_idx = idx;
    score_text.setFont(renderer.getFont(font_idx))
      .setString("Score: 0")
      .setPosition({ 10, 25 })
      .setColour(ASGE::COLOURS::DARKORANGE)
      .setZOrder(4);
  }
}

void Camera::renderText(int score)
{
  score_text.setString("Score: " + std::to_string(score));
  score_text.setPositionX(camera.position().x - screen_size.x * 0.5F + 10.0F);
  renderer.renderText(score_text);
}
