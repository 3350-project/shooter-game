//name: Rodolfo Velasquez 
//file: rvelasquez.cpp
//CMPS 3350 Software engineering 
//Team 3
//Classes and functions prototype 

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

#endif
