#ifndef NOTMARIOGAME_SOUND_HPP
#define NOTMARIOGAME_SOUND_HPP

#include <Engine/FileIO.hpp>
#include <Engine/Logger.hpp>
#include <soloud.h>
#include <soloud_wav.h>

class Sound
{
 public:
  Sound();
  ~Sound();

  void playJump();
  void playCoin();
  void playGoomba();

 private:
  void loadSound(SoLoud::Wav& sound, std::string file_name, float volume);
  SoLoud::Soloud audio_engine;
  SoLoud::Wav background_audio;
  SoLoud::Wav coin_audio;
  SoLoud::Wav jump_audio;
  SoLoud::Wav goomba_audio;
};

#endif // NOTMARIOGAME_SOUND_HPP
