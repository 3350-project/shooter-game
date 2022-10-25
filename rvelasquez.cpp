//name: Rodolfo Velasquez 
//file: rvelasquez.cpp
//
//Team 3
//credits screen
//sound 
//
#include <X11/Xlib.h>
#include <GL/glx.h>
//#include <AL/al.h>
//#include <AL/alc.h>

unsigned int managed_state_credits(unsigned int c)
{
	c = c ^ 1;
	return c;
}

unsigned int managed_state_soundTestMode(unsigned int t)
{
	t = t ^ 1;
	return t;
}

unsigned int managed_state_sound(unsigned int x)
{
	x = x ^ 1;
	return x;
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
}

void sound_test_mode(int xres, int yres)
{
	int xcent = xres / 2;
	int ycent = yres / 2;
	int w = 200;
	glColor3f(0.0, 0.5, 0.5);
	glBegin(GL_QUADS);
		glVertex2f(xcent-w, ycent-w);
		glVertex2f(xcent-w, ycent+w);
		glVertex2f(xcent+w, ycent+w);
		glVertex2f(xcent+w, ycent-w);
	glEnd();
}

void play_sound()
{

}

/*	
	ALCdevice * device;
	Alboolean enumeration;
	
	
	device = alcOpenDevice(NULL);
	if (!device)
		 //error
	
	enumeration = alcISExtensionPresent(NULL,"ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		//not supported 
	else
		//suported
	
	static void list_audio_devices(const ALCchar *devices)
	{
		const ALCchar *device = devices, *next = devices + 1;
		size_t len = 0;
		fprint(stdout, "devices list:\n");
		fprint(stdout,"-----------\n"
		while(device && device != '\0" && next && "next != '\0') {
			fprint(stdout, "%s\n", device);
			len = strlen(device);
			device += (len + 1);
			next += (len + 2);
	}
	printf(stdout,"-----------\n";
	}
}
*/









