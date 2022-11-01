//name: Rodolfo Velasquez 
//file: rvelasquez.cpp
//
//Team 3
//credits screen
//sound 
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>

#include <AL/al.h>
#include <AL/alc.h>

#ifdef LIBAUDIO
	#include <audio/wave.h>
	#define BACKEND	"libaudio"
#else
	#include <AL/alut.h>
	#define BACKEND "alut"
#endif

#include <AL/alut.h>

#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"

		ALboolean enumeration;
		const ALCchar *devices;
		//const ALCchar *defaultDeviceName = argv[1];
		int ret;
#ifdef LIBAUDIO
		WaveInfo *wave;
#endif
		char *bufferData;
		ALCdevice *device;
		ALvoid *data;
		ALCcontext *context;
		ALsizei size, freq;
		ALenum format;
		ALuint buffer, source;
		ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
		ALboolean loop = AL_FALSE;
		ALCenum error;
		ALint source_state;

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
	Rect r; 
	r.bot = yres - 80;
	r.left = xres / 2;
	r.center = 0;
	ggprint16(&r, 20, 0x00ffffff, "CREDITS");
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
}

/*
*	Sound framework
*
*/

/*
	//Listing audio devices 
	static void list_audio_devices(const ALCchar *devices)
	{
		const ALCchar *device = devices, *next = devices + 1;
		size_t len = 0;

		fprintf(stdout, "Devices list:\n");
		fprintf(stdout, "----------\n");
		while (device && *device != '\0' && next && *next != '\0') {
			fprintf(stdout, "%s\n", device);
			len = strlen(device);
			device += (len + 1);
			next += (len + 2);
		}
		fprintf(stdout, "----------\n");
	}

#define TEST_ERROR(_msg)		\
	error = alGetError();		\
	if (error != AL_NO_ERROR) {	\
		fprintf(stderr, _msg "\n");	\
		return -1;		\
	}

	static inline ALenum to_al_format(short channels, short samples)
	{
	
		bool stereo = (channels > 1);

		switch (samples) {
			case 16:
				if (stereo)
					return AL_FORMAT_STEREO16;
				else
					return AL_FORMAT_MONO16;
			case 8:
				if (stereo)
					return AL_FORMAT_STEREO8;
				else
					return AL_FORMAT_MONO8;
			default:
				return -1;
		}
	}

void play_sound()
{ 	
v		ALboolean enumeration;
		const ALCchar *devices;
		//const ALCchar *defaultDeviceName = argv[1];
		int ret;
#ifdef LIBAUDIO
		WaveInfo *wave;
#endif
		char *bufferData;
		ALCdevice *device;
		ALvoid *data;
		ALCcontext *context;
		ALsizei size, freq;
		ALenum format;
		ALuint buffer, source;
		ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
		ALboolean loop = AL_FALSE;
		ALCenum error;
		ALint source_state;

		fprintf(stdout, "Using " BACKEND " as audio backend\n");

		//check if OpenAL supports enumeration devices 
		enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
		if (enumeration == AL_FALSE)
			fprintf(stderr, "enumeration extension not available\n");

		//if enumeration is supported then list the audio devices 
		list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

		if (!defaultDeviceName)
			defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

		//Open a handle to a device 
		device = alcOpenDevice(defaultDeviceName);	
		if (!device) {
			fprintf(stderr, "unable to open default device\n");
			return -1;
		}

		fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

		//reset the error stack 
		alGetError();

		//To render a scene create and initialize a context
		context = alcCreateContext(device, NULL);
		if (!alcMakeContextCurrent(context)) {
			fprintf(stderr, "failed to make default context\n");
			return -1;
		}
		TEST_ERROR("make default context");

		//set orientation 
		alListener3f(AL_POSITION, 0, 0, 1.0f);
		TEST_ERROR("listener position");
		alListener3f(AL_VELOCITY, 0, 0, 0);
		TEST_ERROR("listener velocity");
		alListenerfv(AL_ORIENTATION, listenerOri);
		TEST_ERROR("listener orientation");

		alGenSources((ALuint)1, &source);
		TEST_ERROR("source generation");

		alSourcef(source, AL_PITCH, 1);
		TEST_ERROR("source pitch");
		alSourcef(source, AL_GAIN, 1);
		TEST_ERROR("source gain");
		alSource3f(source, AL_POSITION, 0, 0, 0);
		TEST_ERROR("source position");
		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		TEST_ERROR("source velocity");
		alSourcei(source, AL_LOOPING, AL_FALSE);
		TEST_ERROR("source looping");

		//reference to the buffer object 
		alGenBuffers(1, &buffer);
		TEST_ERROR("buffer generation");

#ifdef LIBAUDIO
		//loading an audio stream to a buffer
		wave = WaveOpenFileForReading("test.wav");
		if (!wave) {
			fprintf(stderr, "failed to read wave file\n");
			return -1;
		}

		ret = WaveSeekFile(0, wave);
		if (ret) {
			fprintf(stderr, "failed to seek wave file\n");
			return -1;
		}

		bufferData = malloc(wave->dataSize);
		if (!bufferData) {
			perror("malloc");
			return -1;
		}

		ret = WaveReadFile(bufferData, wave->dataSize, wave);
		if (ret != wave->dataSize) {
			fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
			return -1;
		}

		//load raw audio stream into our buffer
		alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
				bufferData, wave->dataSize, wave->sampleRate);
		TEST_ERROR("failed to load buffer data");
#else
		alutLoadWAVFile("test.wav", &format, &data, &size, &freq, &loop);
		TEST_ERROR("loading wav file");

		alBufferData(buffer, format, data, size, freq);
		TEST_ERROR("buffer copy");
#endif

		//binding source with its buffer 
		alSourcei(source, AL_BUFFER, buffer);
		TEST_ERROR("buffer binding");

		//
		alSourcePlay(source);
		TEST_ERROR("source playing");

		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		TEST_ERROR("source state get");
		while (source_state == AL_PLAYING) {
			alGetSourcei(source, AL_SOURCE_STATE, &source_state);
			TEST_ERROR("source state get");
		}

		//exit context
		alDeleteSources(1, &source);
		alDeleteBuffers(1, &buffer);
		device = alcGetContextsDevice(context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);

	
}
*/



