#include "Sound.hpp"

Sound::Sound()
{
  audio_engine.init();
  loadSound(background_audio, "/data/audio/SuperMarioBros.mp3", 0.15F);
  loadSound(coin_audio, "/data/audio/coin.mp3", 0.3F);
  loadSound(jump_audio, "/data/audio/jump.mp3", 4.0F);
  loadSound(goomba_audio, "/data/audio/goomba.mp3", 5.0F);

  audio_engine.play(background_audio);
}

Sound::~Sound()
{
  audio_engine.deinit();
}

void Sound::loadSound(SoLoud::Wav& sound, std::string file_name, float volume)
{
  ASGE::FILEIO::File audio_file;
  if (audio_file.open(file_name))
  {
    auto buffer = audio_file.read();
    sound.loadMem(
      buffer.as_unsigned_char(), static_cast<unsigned int>(buffer.length), false, false);
  }
  sound.mVolume = volume;
}

void Sound::playJump()
{
  audio_engine.play(jump_audio);
}

void Sound::playCoin()
{
  audio_engine.play(coin_audio);
}

void Sound::playGoomba()
{
  audio_engine.play(goomba_audio);
}
