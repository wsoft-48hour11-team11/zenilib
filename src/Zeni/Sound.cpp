/* This file is part of the Zenipex Library.
* Copyleft (C) 2006 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

#include <Zeni/Sound.hxx>

#include <Zeni/Coordinate.hxx>

#include <SDL/SDL.h>
#include <iostream>

using std::string;

namespace Zeni {

  Sound_Buffer::Sound_Buffer()
    : m_buffer(AL_NONE)
  {
    Sound::get_reference();

    m_buffer = alutCreateBufferHelloWorld();

    if(m_buffer == AL_NONE) {
      std::cerr << "ALUT error on Hello World: " << alutGetErrorString(alutGetError()) << std::endl;
      throw Sound_Buffer_Init_Failure();
    }
  }

  Sound_Buffer::Sound_Buffer(const Sound_Buffer &rhs)
    : m_buffer(rhs.m_buffer)
  {
    Sound::get_reference();

    rhs.m_buffer = AL_NONE;
    rhs.m_buffer = alutCreateBufferHelloWorld();
    
    if(rhs.m_buffer == AL_NONE) {
      std::cerr << "ALUT error on Hello World: " << alutGetErrorString(alutGetError()) << std::endl;
      throw Sound_Buffer_Init_Failure();
    }
  }

  Sound_Buffer::Sound_Buffer(const string &filename)
    : m_buffer(AL_NONE)
  {
    Sound::get_reference();

    m_buffer = alutCreateBufferFromFile(filename.c_str());

    if(m_buffer == AL_NONE) {
      std::cerr << "ALUT error on '" << filename << "': " << alutGetErrorString(alutGetError()) << std::endl;
      throw Sound_Buffer_Init_Failure();
    }
  }

  Sound_Buffer::~Sound_Buffer() {
    if(m_buffer != AL_NONE)
      alDeleteBuffers(1, &m_buffer);
  }
  
  Sound_Buffer & Sound_Buffer::operator=(const Sound_Buffer &rhs) {
    Sound_Buffer temp(rhs);
    std::swap(m_buffer, temp.m_buffer);
    return *this;
  }

  static Sound_Buffer g_Hello_World_Buffer;

  Sound_Source::Sound_Source()
    : m_source(AL_NONE)
  {
    init(g_Hello_World_Buffer.get_id());
  }

  Sound_Source::Sound_Source(const Sound_Source &rhs)
    : m_source(rhs.m_source)
  {
    rhs.m_source = AL_NONE;
    rhs.init(g_Hello_World_Buffer.get_id());
  }

  Sound_Source::Sound_Source(const Sound_Buffer &buffer, const float &pitch, const float &gain,
    const Point3f &position, const Point3f &velocity, const bool &looping)
    : m_source(AL_NONE)
  {
    init(buffer.get_id(), pitch, gain, position, velocity, looping);
  }

  Sound_Source::Sound_Source(const ALuint &buffer, const float &pitch, const float &gain,
    const Point3f &position, const Point3f &velocity, const bool &looping)
    : m_source(AL_NONE)
  {
    init(buffer, pitch, gain, position, velocity, looping);
  }

  Sound_Source::~Sound_Source() {
    if(m_source != AL_NONE)
      alDeleteSources(1, &m_source);
  }

  void Sound_Source::init(const ALuint &buffer, const float &pitch, const float &gain,
                          const Point3f &position, const Point3f &velocity, const bool &looping) const {
    Sound::get_reference();

    alGenSources(1, &m_source);

    if(m_source == AL_NONE) {
      std::cerr << "ALUT error" << ": " << alutGetErrorString(alutGetError()) << std::endl;
      throw Sound_Source_Init_Failure();
    }

    alSourcei(m_source, AL_BUFFER, buffer);
    alSourcef(m_source, AL_PITCH, pitch);
    alSourcef(m_source, AL_GAIN, gain);
    alSourcefv(m_source, AL_POSITION, reinterpret_cast<const float *>(&position));
    alSourcefv(m_source, AL_VELOCITY, reinterpret_cast<const float *>(&velocity));
    alSourcei(m_source, AL_LOOPING, looping);
  }

  Sound_Source & Sound_Source::operator=(const Sound_Source &rhs) {
    Sound_Source temp(rhs);
    std::swap(m_source, temp.m_source);
    return *this;
  }

  Sound::Sound()
    : m_bgm(0),
    m_bgm_source(0)
  {
    // Ensure Core is initialized
    Core::get_reference();

    if(!alutInit(0, 0))
      throw Sound_Init_Failure();

    // Check for Vorbis extension functionality; seems to always fail :(
    alIsExtensionPresent("AL_EXT_vorbis");
    std::cerr << "Valid Audio Formats: " << alutGetMIMETypes(ALUT_LOADER_BUFFER) << std::endl;

    ALfloat listener_position[] = {0.0f, 0.0f, 0.0f};
    ALfloat listener_velocity[] = {0.0f, 0.0f, 0.0f};
    ALfloat listener_forward_and_up[] = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    alListenerfv(AL_POSITION, listener_position);
    alListenerfv(AL_VELOCITY, listener_velocity);
    alListenerfv(AL_ORIENTATION, listener_forward_and_up);
  }

  Sound::~Sound() {
    delete m_bgm_source;
    delete m_bgm;
    alutExit();
  }

  Sound & Sound::get_reference() {
    static Sound e_sound;
    return e_sound;
  }

  void Sound::set_BGM(const string &filename) {
    assert_m_bgm();

    bool playing = m_bgm_source->is_playing() ? true : false;
    float pitch = m_bgm_source->get_pitch();
    float gain = m_bgm_source->get_gain();
    Point3f position = m_bgm_source->get_position();
    Point3f velocity = m_bgm_source->get_velocity();
    bool looping = m_bgm_source->is_looping();

    m_bgm_source->stop();
    delete m_bgm_source;
    m_bgm_source = 0;

    m_bgmusic = filename;
    *m_bgm = Sound_Buffer(filename);

    m_bgm_source = new Sound_Source(*m_bgm, pitch, gain, position, velocity, looping);

    if(playing)
      m_bgm_source->play();
  }

  void Sound::assert_m_bgm() {
    if(!m_bgm) {
      m_bgm = new Sound_Buffer();
      if(!m_bgm) {
        alutExit();
        throw Sound_Init_Failure();
      }
    }

    if(!m_bgm_source) {
      m_bgm_source = new Sound_Source(*m_bgm);
      if(!m_bgm_source) {
        delete m_bgm;
        alutExit();
        throw Sound_Init_Failure();
      }
    }
  }

}