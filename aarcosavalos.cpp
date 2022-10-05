// Source File by: Axel Arcos
// Date: 09/13/2022
// CS 3350

#include <iostream>

void print_name()
{
  std::cout << "Axel\n";
}

unsigned int manage_state(unsigned int s)
{
  s = s ^ 1;
  return s;
}

bool finish_game (bool s)
{
  s = true;
  return s;
}
