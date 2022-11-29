/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * Contains methods for drawing on screen.
*/
#ifndef RWYATT_H
#define RWYATT_H

class RWyatt
{
private:
    static void flipState(bool &state);
public:
    static void pauseScreen(bool &paused);
    static void drawBorder(int xres, int yres);
    static void drawScore(int, int, int);
    RWyatt();
};

#endif
