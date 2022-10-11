// Reid Wyatt

#ifndef RWYATT_H
#define RWYATT_H

class rwyatt 
{
private:
    static void flip_state(bool &state);
public:
    static void pause_screen(bool &paused);
    static void network_mode(bool &networked);
};

#endif
