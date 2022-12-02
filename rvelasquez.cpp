//name: Rodolfo Velasquez 
//file: rvelasquez.cpp
//
//CMPS 3350 Software engineering 
//Team 3
//
//Features: Credit Screen and Sound

///////////////////////////////////////////////////////////////////////////////
// 					                    
// Code, Tech and tutorials openal-impl
// November 5, 2020
// openAL Tutorial pt.1 | Init and Play Sound Effects
// openAl Tutorial pt.2 | Music & Long Sound Buffering
// https://www.youtube.com/watch?v=kWQM1iQ1W0E
// https://www.youtube.com/watch?v=pYK8XZHV74s
// https://github.com/codetechandtutorials/openal-impl/releases/tag/vid1
//
// Mt. Ford Studios 
// June 18, 2018
// JS Asteroids Game Part 7 (Sounds effects and Music)
// https://www.youtube.com/watch?v=LfSBbrGqFV0
// https://drive.google.com/file/d/1tmjvMKxCcJeyTpi5pI6A8cgVxwWnyPXn/view
//
///////////////////////////////////////////////////////////////////////////////

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
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include "rvelasquez.h"
#include <malloc.h>
#include <cstddef>

#ifdef AUDIO
#include <climits>
#include <AL/alext.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <sndfile.h>
#endif

using namespace std;

unsigned int managed_state_credits(unsigned int c)
{
    c = c ^ 1;
    return c;
}

unsigned int managed_state_sound(unsigned int F4)
{
    F4 = F4 ^ 1;
    return F4;
}

void show_credits(int xres, int yres)
{	
    int xcent = xres / 2;	
    int ycent = yres / 2;

    int w = 200;
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_QUADS);
    glVertex2f(xcent-w, ycent-w);
    glVertex2f(xcent-w, ycent+w);
    glVertex2f(xcent+w, ycent+w);
    glVertex2f(xcent+w, ycent-w);
    glEnd();
    
    glColor3f(1.0,1.0,1.0);
    
    Rect r; 
    r.bot = yres - 240;
    r.left = xres / 2;
    ggprint16(&r, 20, 0xffffffff, "~ CREDITS SCREEN ~");

    r.bot = yres - 280;
    r.left = (xres / 2) - 75;
    ggprint16(&r, 20, 0xffffffff, "Axel Arcos");

    r.bot = yres - 320;
    r.left = (xres / 2) - 65;
    ggprint16(&r, 20, 0xffffffff, "Ryan Gordon");

    r.bot = yres - 360;
    r.left = (xres / 2) - 72;
    ggprint16(&r, 20, 0xffffffff, "Steven Nez");

    r.bot = yres - 400;
    ggprint16(&r, 20, 0xffffffff, "Reid Wyatt");

    r.bot = yres - 440;
    r.left = (xres/2) - 40;
    ggprint16(&r, 20, 0xffffffff, "Rodolfo Velasquez");
}

void sound(int xres, int yres)
{
    class MusicBuffer;

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

#ifdef AUDIO
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
        fprintf(stderr, "Could not open audio in %s: %s\n", filename, 
                sf_strerror(sndfile));
        return 0;
    }
    if (sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX / 
        sizeof(short)) / sfinfo.channels) {
            fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", 
                    filename, sfinfo.frames);
        sf_close(sndfile);
        return 0;
    }

    //Checks the audio file has the correct format
    format = AL_NONE;
    if (sfinfo.channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (sfinfo.channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else if (sfinfo.channels == 3) {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == 
            SF_AMBISONIC_B_FORMAT) format = AL_FORMAT_BFORMAT2D_16;
    } else if (sfinfo.channels == 4) {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == 
            SF_AMBISONIC_B_FORMAT) format = AL_FORMAT_BFORMAT3D_16;
    }
    if (!format) {
        fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
                sf_close(sndfile);
        return 0;
    }

    //Decodes the audio file into a buffer
    membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * 
             sfinfo.channels) * sizeof(short)));
    num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
    if (num_frames < 1) {
        free(membuf);
        sf_close(sndfile);
        fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", 
                filename, num_frames);
        return 0;
    }
    num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)
                 sizeof(short);

    //Loads audio file into a new buffer, frees memory and closees audio file 
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
        } else {
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
    alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], 
               p_Position[2]);
    alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], 
               p_Velocity[2]);
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

//Music buffer play function  
void MusicBuffer::Play()
{
	ALsizei i;
    alGetError();
	//Rewind source position and clear buffer queue
	alSourceRewind(p_Source);
	alSourcei(p_Source, AL_BUFFER, 0);
	//Fill the buffer queue(4 buffers)
	for (i = 0; i < NUM_BUFFERS; i++) {
        sf_count_t slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
		if (slen < 1) break;
		slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
		alBufferData(p_Buffers[i], p_Format, p_Membuf, (ALsizei)slen, 
            p_Sfinfo.samplerate);
	}
	if (alGetError() != AL_NO_ERROR) {
	    cout << "Error buffering for playback";
        exit(1);
	}
    //Queue and start playback 
	alSourceQueueBuffers(p_Source, i, p_Buffers);
	alSourcePlay(p_Source);
	if (alGetError() != AL_NO_ERROR) {
	    cout << "Error starting playback";
        exit(1);
	}
}

//Re-buffers what has been played 
void MusicBuffer::UpdateStream()
{
    ALint processed, state;
    alGetError();
    //Checks state and what has been processed 
    alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
    alGetSourcei(p_Source, AL_BUFFERS_PROCESSED, &processed);
    if (alGetError() != AL_NO_ERROR) {
        cout << "error checking music source state";
        exit(1);
    }
    //Unqueue and handle each processed buffer
    while (processed > 0) {
        ALuint bufid;
        sf_count_t slen;
        alSourceUnqueueBuffers(p_Source, 1, &bufid);
        processed--;
        //Reads data, refill buffer & queue it back to the source
        slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
        if (slen > 0) {
            slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
            alBufferData(bufid, p_Format, p_Membuf, (ALsizei)slen,
                p_Sfinfo.samplerate);
            alSourceQueueBuffers(p_Source, 1, &bufid);
        }
        if (alGetError() != AL_NO_ERROR) {
            cout << "error buffering music data";
            exit (1);
        }
    }
    //Ensures source hasn't underrun(lacking data)
    if (state != AL_PLAYING && state != AL_PAUSED) {
        ALint queued;
        //If no buffers are queued, playback is finished
        alGetSourcei(p_Source, AL_BUFFERS_QUEUED, &queued);
        if (queued == 0)
            return;
        alSourcePlay(p_Source);
        if (alGetError() != AL_NO_ERROR) {
            cout << "error restarting music playback";
        }
    }
}

//Returns source for music buffer 
ALint MusicBuffer::getSource()
{
	return p_Source;
}

//Instantiate a new music buffer  
MusicBuffer::MusicBuffer(const char* filename)
{
    //Generate source and buffers
    alGenSources(1, &p_Source);
    alGenBuffers(NUM_BUFFERS, p_Buffers);
    std::size_t frame_size;
    //open and read audio file   
    p_SndFile = sf_open(filename, SFM_READ, &p_Sfinfo);
    if (!p_SndFile) {
        cout << "error opening the file";
        exit(1);
    } else {
        cout << "Audio files loadup succesfully\n";
    }
    //Check audio file format and set it up to the correct OpenAL format 
    if (p_Sfinfo.channels == 1) {
        p_Format = AL_FORMAT_MONO16;
    } else if (p_Sfinfo.channels == 2) {
        p_Format = AL_FORMAT_STEREO16;
    } else if (p_Sfinfo.channels == 3) {
        if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == 
            SF_AMBISONIC_B_FORMAT) p_Format = AL_FORMAT_BFORMAT2D_16;
    } else if (p_Sfinfo.channels == 4) {
        if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) ==
            SF_AMBISONIC_B_FORMAT) p_Format = AL_FORMAT_BFORMAT3D_16;
    }
    if (!p_Format) {
        sf_close(p_SndFile);
        p_SndFile = NULL;
        cout << "Unsupported channel file";
        exit(1);
    }
    //Calculate frame size and allocates memory  
    frame_size = ((size_t)BUFFER_SAMPLES * (size_t)p_Sfinfo.channels) * 
                  sizeof(short);
    p_Membuf = static_cast<short*>(malloc(frame_size));
}

//Music buffer destructor 
MusicBuffer::~MusicBuffer()
{
	alDeleteSources(1, &p_Source);
	if (p_SndFile)
	    sf_close(p_SndFile);
	p_SndFile = nullptr;
	free(p_Membuf);
	alDeleteBuffers(NUM_BUFFERS, p_Buffers);
}

#endif


