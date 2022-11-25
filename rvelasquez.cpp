//name: Rodolfo Velasquez 
//file: rvelasquez.cpp
//
//Team 3
//
//features: credit screen and sound 

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <vector>
#include <climits>
#include <AL/alext.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <sndfile.h>

#include <X11/Xlib.h>
#include <GL/glx.h>

#include "fonts.h"
#include "rvelasquez.h"

using namespace std;

unsigned int managed_state_credits(unsigned int c)
{
	c = c ^ 1;
	return c;
}

unsigned int managed_state_sound(unsigned int s)
{
	s = s ^ 1;
	return s;
}

void show_credits(int xres, int yres)
{	
	int xcent = xres / 2;	
	int ycent = yres / 2;

	int w = 200;
	glColor3f(0.57, 0.82, 1.0);
	glBegin(GL_QUADS);
		glVertex2f(xcent-w, ycent-w);
		glVertex2f(xcent-w, ycent+w);
		glVertex2f(xcent+w, ycent+w);
		glVertex2f(xcent+w, ycent-w);
	glEnd();

	Rect r; 
	r.bot = yres - 200;
	r.left = xres / 2;
	r.center = 10;
	ggprint16(&r, 20, 0x00000000, "CREDITS");
 	
	r.bot = yres - 240;
    r.left = (xres / 2) - 75;
	r.center = 60;
    ggprint16(&r, 20, 0x00000000, "Axel Arcos");
   
	r.bot = yres - 280;
    r.left = (xres / 2) - 73;
	r.center = 61;
    ggprint16(&r, 20, 0x00000000, "Ryan Gordon");

	r.bot = yres - 320;
    r.left = (xres / 2) - 72;
	r.center = 60;
    ggprint16(&r, 20, 0x00000000, "Steven Nez");

	r.bot = yres - 360;
	r.center = 63;
    ggprint16(&r, 20, 0x00000000, "Reid Wyatt");

	r.bot = yres - 400;
	r.center = 70;
    ggprint16(&r, 20, 0x00000000, "Rodolfo Velasquez");
}

void sound(int xres, int yres)
{
	Rect s; 
	s.bot = yres - 80;
	s.left = xres / 2;
	s.center = 0;

	ggprint16(&s, 16, 0x00ffffff, "SOUND FEATURE");

	// draw a border using a triangle strip
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4f(0.09, 0.88, 0.24, 1.0);
	int w = 20;
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2i(0, 0);
	glVertex2i(0+w, w);
	glVertex2i(0, yres);
	glVertex2i(0+w, yres-w);
	glVertex2i(xres, yres);
	glVertex2i(xres-w, yres-w);
	glVertex2i(xres, 0);
	glVertex2i(xres-w, w);
	glVertex2i(0, 0);
	glVertex2i(0+w, w);
	glEnd();
	glDisable(GL_BLEND);

	std::cout << "starting...\n";

	SoundDevice * mysounddevice = SoundDevice::get();


	//Set up sound effects 
	uint32_t /*ALuint*/ explodeSound = SoundBuffer::get()->addSoundEffect("./soundFiles/big_explosion.wav");
	uint32_t /*ALuint*/ hitSound = SoundBuffer::get()->addSoundEffect("./soundFiles/gun_fire.wav");
	uint32_t /*ALuint*/ laserSound = SoundBuffer::get()->addSoundEffect("./soundFiles/gun_fire.wav");
	uint32_t /*ALuint*/ thrustSound = SoundBuffer::get()->addSoundEffect("./soundFiles/big_explosion.wav");

	SoundSource mySpeaker;

	mySpeaker.Play(sound1);
	mySpeaker.Play(sound2);
	mySpeaker.Play(sound3);
	mySpeaker.Play(sound4);
	mySpeaker.Play(sound5);
	mySpeaker.Play(sound6);
	mySpeaker.Play(sound7);

	std::cout << "got here\n";
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Source Code: Code, Tech and tutorials openal-impl
// https://github.com/codetechandtutorials/openal-impl/releases/tag/vid1
//
//////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////DEVICE
/*
class SoundDevice
{
	public:
		static SoundDevice* get();

	private:
		SoundDevice() noexcept;
		~SoundDevice();

		ALCdevice* p_ALCDevice;
		ALCcontext* p_ALCContext;
};
*/

SoundDevice* SoundDevice::get()
{
	static SoundDevice* snd_device = new SoundDevice();
	return snd_device;
}

SoundDevice::SoundDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr); // nullptr = get default device
	if (!p_ALCDevice)
		throw("failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);  // create context
	if(!p_ALCContext)
		throw("Failed to set sound context");

	if (!alcMakeContextCurrent(p_ALCContext))   // make context current
		throw("failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice() 
{
	if (!alcMakeContextCurrent(nullptr))
		throw("failed to set context to nullptr");

	alcDestroyContext(p_ALCContext);
	if (p_ALCContext)
		throw("failed to unset during close");

	if (!alcCloseDevice(p_ALCDevice))
		throw("failed to close sound device");
}

/////////////////////////////////////////////BUFFER
/*
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
*/

SoundBuffer* SoundBuffer::get()
{
	static SoundBuffer* sndbuf = new SoundBuffer();
	return sndbuf;
}

ALuint SoundBuffer::addSoundEffect(const char* filename)
{
	ALenum err, format;
	ALuint buffer;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;

	//Open the audio file and check that it's usable. 

	sndfile = sf_open(filename, SFM_READ, &sfinfo);
	if (!sndfile)
	{
		fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
		return 0;
	}
	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
		fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
		sf_close(sndfile);
		return 0;
	}

	/* Get the sound format, and figure out the OpenAL format */

	format = AL_NONE;
	if (sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	else if (sfinfo.channels == 3)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (sfinfo.channels == 4)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!format)
	{
		fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
		sf_close(sndfile);
		return 0;
	}

	/*Decode the whole audio file to a buffer. */
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
		fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
		return 0;
	}
	num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);


	/* Buffer the audio data into a new buffer object, then free the data and
	* close the file.
	*/

	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

	free(membuf);
	sf_close(sndfile);


	/*Check if an error occured, and clean up if so. */
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}

	p_SoundEffectBuffers.push_back(buffer);  // add to the list of known buffers

	return buffer;
}


bool SoundBuffer::removeSoundEffect(const ALuint& buffer)
{
	auto it = p_SoundEffectBuffers.begin();
	while (it != p_SoundEffectBuffers.end())
	{
		if (*it == buffer)
		{
			alDeleteBuffers(1, &*it);

			it = p_SoundEffectBuffers.erase(it);

			return true;
		}
		else {
			++it;
		}
	}
	return false;  // couldn't find to remove
}

SoundBuffer::SoundBuffer()
{
	p_SoundEffectBuffers.clear();

}

SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(p_SoundEffectBuffers.size(), p_SoundEffectBuffers.data());

	p_SoundEffectBuffers.clear();
}

//////////////////////////////////////////////SOURCE
/*
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
*/

SoundSource::SoundSource()
{
	alGenSources(1, &p_Source);
	alSourcef(p_Source, AL_PITCH, p_Pitch);
	alSourcef(p_Source, AL_GAIN, p_Gain);
	alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
	alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
	alSourcei(p_Source, AL_LOOPING, p_LoopSound);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &p_Source);
}

void SoundSource::Play(const ALuint buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
			p_Buffer = buffer_to_play;
			alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}

	alSourcePlay(p_Source);


	ALint state = AL_PLAYING;
	std::cout << "playing sound\n";
	while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		std::cout << "currently playing sound\n";
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	}
	std::cout << "done playing sound\n";
}

/*
int main()
{
	std::cout << "starting...\n";

	SoundDevice * mysounddevice = SoundDevice::get();
*/
//	uint32_t /*ALuint*/ sound1 = SoundBuffer::get()->addSoundEffect("./big_explosion.wav");
//	uint32_t /*ALuint*/ sound2 = SoundBuffer::get()->addSoundEffect("./gun_fire.wav");
//	uint32_t /*ALuint*/ sound3 = SoundBuffer::get()->addSoundEffect("./gun_fire.wav");
//	uint32_t /*ALuint*/ sound4 = SoundBuffer::get()->addSoundEffect("./big_explosion.wav");
//	uint32_t /*ALuint*/ sound5 = SoundBuffer::get()->addSoundEffect("./gun_fire.wav");
//	uint32_t /*ALuint*/ sound6 = SoundBuffer::get()->addSoundEffect("./gun_fire.wav");
//	uint32_t /*ALuint*/ sound7 = SoundBuffer::get()->addSoundEffect("./big_explosion.wav");
/*
	SoundSource mySpeaker;

	mySpeaker.Play(sound1);
	mySpeaker.Play(sound2);
	mySpeaker.Play(sound3);
	mySpeaker.Play(sound4);
	mySpeaker.Play(sound5);
	mySpeaker.Play(sound6);
	mySpeaker.Play(sound7);

	std::cout << "got here\n";

	return 0;
}

*/
