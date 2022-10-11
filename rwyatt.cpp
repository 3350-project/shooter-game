// Reid Wyatt
#include <iostream>
#include "rwyatt.h"

void rwyatt::flip_state(bool &state)
{
    state = !state;
}

void rwyatt::pause_screen(bool &paused) 
{
    flip_state(paused);
}

void rwyatt::network_mode(bool &networked) 
{
    flip_state(networked);
}
