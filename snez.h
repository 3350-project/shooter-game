//header file

namespace snez{
extern unsigned int manage_stateF1(unsigned int F1);

extern unsigned int manage_stateF2(unsigned int F2);

extern void Show_HelpScr(int xres, int yres);

extern void Featuremode(int xres, int yres, int Collision, int hp, int nasteroids);

extern void collision_detection(int sizeasteroids, int newshape);

extern void FeatureMode_Indication(int xres, int yres);

}
