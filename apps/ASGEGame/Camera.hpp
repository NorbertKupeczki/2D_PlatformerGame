#ifndef NOTMARIOGAME_CAMERA_HPP
#define NOTMARIOGAME_CAMERA_HPP

#include <Engine/Camera.hpp>
#include <Engine/Renderer.hpp>

class Camera
{
 public:
  Camera(ASGE::Renderer& renderer_ref, float width, float height);
  ~Camera() = default;

  void setProjectionMatrix();
  void moveCamera(float mario_x);
  float moveCameraAttractScreen(float dt);
  void resetCamera();
  float getCameraX();
  void renderText(int score);

 private:
  void initText();

  int font_idx;
  ASGE::Text score_text;
  ASGE::Camera camera;
  ASGE::Renderer& renderer;
  ASGE::Point2D camera_start_pos;
  ASGE::Point2D screen_size;
  float camera_min_x;
  float camera_max_x   = 9488.0F;
  float camera_x_speed = 150.0F;
};

#endif // NOTMARIOGAME_CAMERA_HPP
