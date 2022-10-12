// Reid Wyatt
#ifndef RWYATT_H
#define RWYATT_H

class RWyatt
{
private:
    static void flip_state(bool &state);
    static void draw_border(int xres, int yres);
    bool network_state;
public:
    static void pause_screen(bool &paused);
    bool startNetworking();
    bool stopNetworking();
    bool networked();
    RWyatt();
};

#endif
