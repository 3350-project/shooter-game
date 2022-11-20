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
//#include "vector3d.h"

extern unsigned int managed_state_credits(unsigned int c);
extern void show_credits(int xres, int yres);

//extern unsigned managed_state_soundTestMode(unsigned int t);
//extern void sound_test_mode(int xres, int yres);

extern unsigned int managed_state_sound(unsigned int s);
extern void sound(int xres, int yres);

/*
class sound {

	struct sound {
		unsigned int sourceid, bufferid;
		char* buffer;
		std::string name;
		data(unsigend int si, unsigend int bi, char* d, const char* n) 
		: sourceid(si), buffer(bi), buffer(d), name(n) {}
	}
	std::vector<data> datas;
	ALCcontext context;
	ALCdevice* device;
	bool isBigEndian;
	char* loadWAV(const char* fn, int& chan, int& samplerate, int &bps, int& size);
	int convertToInt(char* buffer, int len);
	bool isBigEndian();
	public: 
		sound();
		~sound();
		unsigned int source loadSound(const char* filename);
		void deleteSound(unsigned int id);
		void playSound(unsigned int id, const vector3d& pos);
		void stopSound(unsigned int id);
		void setListener(const vector3d& position, const vector3d& orientation);	
}
*/


