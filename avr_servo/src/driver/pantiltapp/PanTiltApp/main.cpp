#include <iostream>
#include <gtkmm/main.h>

#include "NavWin.hpp"

using namespace ServoCtrl;

int main(int argc, char **argv)
{
  if(argc!=1+3)
  {
    std::cerr<<argv[0]<<" <dev> <x-servo> <y-servo>"<<std::endl;
    return 1;
  }

  CommDevicePtrNN dev{ new CommDevice{argv[1]} };
  Gtk::Main kit(false);
  NavWin    win( dev, ServoName{argv[2][0]}, ServoName{argv[3][0]} );
  Gtk::Main::run(win);
  return 0;
}
