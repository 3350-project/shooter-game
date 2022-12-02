//name: Rodolfo Velasquez 
//file: rvelasquez.cpp
//
//CMPS 3350 Software engineering 
//Team 3
//
//functions prototype 


#ifndef RVELASQUEZ_H
#define RVELASQUEZ_H
#include <vector>
#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include <fstream>
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif

#ifdef AUDIO
#include <sndfile.h>
#endif

extern unsigned int managed_state_credits(unsigned int c);;
extern unsigned int managed_state_sound(unsigned int F4);

extern void show_credits(int xres, int yres);
extern void sound(int xres, int yres);
extern void Play(const ALuint buffer_to_Play);
extern void PlayMusic();
extern void UpdateStream();

#ifdef AUDIO
class SoundDevice
{
    public:
         SoundDevice();
        ~SoundDevice();

        static SoundDevice* get();

    private:
        ALCdevice* device;
        ALCcontext* context;
};

class SoundBuffer
{
    public:
        static SoundBuffer* get();

        ALuint addSoundEffect(const char* filename);
        bool removeSoundEffect(const ALuint& buffer);

    private:
        SoundBuffer();
        ~SoundBuffer();

        std::vector<ALuint> SoundEffectBuffers;
};


class SoundSource
{
    public:
        SoundSource();
        ~SoundSource();

        void Play(const ALuint buffer_to_play);

    private:
        ALuint p_Source;
        float p_Pitch = 1.f;
        float p_Gain = 1.f;
        float p_Position[3] = { 0,0,0 };
        float p_Velocity[3] = { 0,0,0 };
        bool p_LoopSound = false;
        ALuint p_Buffer = 0;
};

class MusicBuffer
{
public:
	void Play();
	void UpdateStream();
    
	ALint getSource();
    
    MusicBuffer(const char* filename);
    ~MusicBuffer();

private:
	ALuint p_Source;
	static const int BUFFER_SAMPLES = 8192;
	static const int NUM_BUFFERS = 4;
	ALuint p_Buffers[NUM_BUFFERS];
	SNDFILE* p_SndFile;
	SF_INFO p_Sfinfo;
	short* p_Membuf;
	ALenum p_Format;
 
	MusicBuffer() = delete;
};

#endif
