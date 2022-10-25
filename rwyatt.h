// Reid Wyatt
#ifndef RWYATT_H
#define RWYATT_H

#include <string>

class RWyatt
{
private:
    static void flip_state(bool &state);
    bool network_state;
    bool multiplayer;
    std::string user_name;
    std::string server_ip;
public:
    static void pause_screen(bool &paused);
    static void draw_border(int xres, int yres);
    void draw_networking(int xres, int yres);
    bool start_networking();
    bool stop_networking();
    bool networked();
    bool mutliplayer();
    void set_network_config(int, char*[]);
    std::string player_name();
    std::string server_name();
    RWyatt();
};

#endif
