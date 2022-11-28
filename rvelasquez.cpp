//name: Rodolfo Velasquez 
//file: rvelasquez.cpp
//
//CMPS 3350 Software engineering 
//Team 3
//
//Features: Credit Screen and Sound

//////////////////////////////////////////////////////////////////////////////////////////////////
// 					Works Cited: 
// Code, Tech and tutorials openal-impl
// November 5, 2020
// openAL Tutorial pt.1 | Init and Play Sound Effects
// https://www.youtube.com/watch?v=kWQM1iQ1W0E
// https://github.com/codetechandtutorials/openal-impl/releases/tag/vid1
//
// Mt. Ford Studios 
// June 18, 2018
// JS Asteroids Game Part 7 (Sounds effects and Music)
// https://www.youtube.com/watch?v=LfSBbrGqFV0
// https://drive.google.com/file/d/1tmjvMKxCcJeyTpi5pI6A8cgVxwWnyPXn/view
//
//////////////////////////////////////////////////////////////////////////////////////////////////

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
    r.bot = yres - 240;
    r.left = xres / 2;
    ggprint16(&r, 20, 0x00000000, "CREDITS");

    r.bot = yres - 280;
    r.left = (xres / 2) - 75;
    ggprint16(&r, 20, 0x00000000, "Axel Arcos");

    r.bot = yres - 320;
    r.left = (xres / 2) - 65;
    ggprint16(&r, 20, 0x00000000, "Ryan Gordon");

    r.bot = yres - 360;
    r.left = (xres / 2) - 72;
    ggprint16(&r, 20, 0x00000000, "Steven Nez");

    r.bot = yres - 400;
    ggprint16(&r, 20, 0x00000000, "Reid Wyatt");

    r.bot = yres - 440;
    r.left = (xres/2) - 40;
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
}

//Create a device
SoundDevice* SoundDevice::get()
{
    static SoundDevice* snd_device = new SoundDevice();
    return snd_device;
}

//Open device 
SoundDevice::SoundDevice()
{
    device = alcOpenDevice(nullptr);                      
    if(device) {
        context = alcCreateContext(device, nullptr);                                
        alcMakeContextCurrent(context); 
    } else {
        cout << "could not open device" << endl;
        exit(1);	
    }

    const ALCchar* name = nullptr;
    if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    if (!name || alcGetError(device) != AL_NO_ERROR)
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    printf("Opened \"%s\"\n", name);
}

//Device destructor 
SoundDevice::~SoundDevice() 
{
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);	
}

//Creating a buffer
SoundBuffer* SoundBuffer::get()
{
    static SoundBuffer* sndbuf = new SoundBuffer();
    return sndbuf;
}

//Fill buffer with audio file/
ALuint SoundBuffer::addSoundEffect(const char* filename)
{
    ALenum err, format;
    ALuint buffer;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    short* membuf;
    sf_count_t num_frames;
    ALsizei num_bytes;

    //Check the audio file 
    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if (!sndfile) {
        fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
        return 0;
    }
    if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels) {
        fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
        sf_close(sndfile);
        return 0;
    }

    //Checks the audio file has the correct format
    format = AL_NONE;
    if (sfinfo.channels == 1)
        format = AL_FORMAT_MONO16;
    else if (sfinfo.channels == 2)
        format = AL_FORMAT_STEREO16;
    else if (sfinfo.channels == 3) {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            format = AL_FORMAT_BFORMAT2D_16;
    }
    else if (sfinfo.channels == 4) {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            format = AL_FORMAT_BFORMAT3D_16;
    }
    if (!format) {
        fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
        sf_close(sndfile);
        return 0;
    }

    //Decodes the audio file into a buffer
    membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
    num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
    if (num_frames < 1) {
        free(membuf);
        sf_close(sndfile);
        fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
        return 0;
    }
    num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

    //Buffers the audio file into a new buffer, frees the memory and close the audio file 
    buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);
    free(membuf);
    sf_close(sndfile);

    //Checks for errors and clean up is So
    err = alGetError();
    if (err != AL_NO_ERROR) {
        fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
        if (buffer && alIsBuffer(buffer))
            alDeleteBuffers(1, &buffer);
        return 0;
    }

    //Adds buffer to a vector of sound effects 
    SoundEffectBuffers.push_back(buffer); 
    return buffer;
}

//removes sound effect from sound effects vector
bool SoundBuffer::removeSoundEffect(const ALuint& buffer)
{
    auto t = SoundEffectBuffers.begin();
    while (t != SoundEffectBuffers.end()) {
        if (*t == buffer) {
            alDeleteBuffers(1, &*t);
            t = SoundEffectBuffers.erase(t);
            return true;
        }
        else {
            ++t;
        }
    }
    return false;  
}

//Clears the sound effects' vector  
SoundBuffer::SoundBuffer()
{
    SoundEffectBuffers.clear();

}

//Buffer destructor
SoundBuffer::~SoundBuffer() 
{
    alDeleteBuffers(SoundEffectBuffers.size(), SoundEffectBuffers.data());
    SoundEffectBuffers.clear();
}

//Defines the source 
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

//Deletes the source 
SoundSource::~SoundSource() 
{
    alDeleteSources(1, &p_Source);
}

//Plays the sound effect 
void SoundSource::Play(const ALuint buffer_to_play)
{

    if (buffer_to_play != p_Buffer) {
        p_Buffer = buffer_to_play;
        alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
    }

    alSourcePlay(p_Source);
    ALint state = AL_PLAYING;
    alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
}



