//name: Rodolof Velasquez
//file: rvelasquez.h 
//
//functions prototype
#define RVELASQUEZ_H
#include <vector>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>

#include <fstream>

#include <X11/Xlib.h>
#include <GL/glx.h>

extern unsigned int managed_state_credits(unsigned int c);
extern void show_credits(int xres, int yres);

//extern unsigned managed_state_soundTestMode(unsigned int t);
//extern void sound_test_mode(int xres, int yres);

extern unsigned int managed_state_sound(unsigned int s);
extern void sound(int xres, int yres);

class SoundDevice
{
public:
	static SoundDevice* get();

private:
	SoundDevice();
	~SoundDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

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

	std::vector<ALuint> p_SoundEffectBuffers;
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


