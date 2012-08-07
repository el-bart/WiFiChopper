#include <iostream>
#include <utility>
#include <ctype.h>
#include <curses.h>

#include "ServoCtrl/Servo.hpp"

using namespace std;
using namespace ServoCtrl;

int main(int argc, char **argv)
{
  if(argc!=1+2)
  {
    cerr<<argv[0]<<" <dev> <servo>"<<endl;
    return 1;
  }

  CommDevicePtrNN dev{ new CommDevice{argv[1]} };
  Servo           servo{ ServoName{argv[2][0]}, dev };

  initscr();
  cbreak();
  noecho();

  uint8_t pos=256/2;
  while( cin.good() )
  {
    clear();
    printw("setting position to %d...\n", static_cast<int>(pos));
    const bool err=servo.setPos(pos).error();
    if(err)
      printw("ERROR SETTING POSITION\n");

    printw("\n");
    printw("Q=>quit\n");
    printw("H=>set current as max pos.\n");
    printw("L=>set current as min pos.\n");
    printw("B=>set current as begin pos.\n");
    printw("\n");
    printw("A=>+1   S=>+10   D=>+100\n");
    printw("Z=>-1   X=>-10   V=>-100\n");
    printw("\n");

    const char in=getch();
    const char c =tolower(in);

    // +-1
    if(c=='a')
    {
      pos=min(pos+1, 255);
      continue;
    }
    if(c=='z')
    {
      pos=max(pos-1, 0);
      continue;
    }

    // +-10
    if(c=='s')
    {
      pos=min(pos+10, 255);
      continue;
    }
    if(c=='x')
    {
      pos=max(pos-10, 0);
      continue;
    }

    // +-100
    if(c=='d')
    {
      pos=min(pos+100, 255);
      continue;
    }
    if(c=='c')
    {
      pos=max(pos-100, 0);
      continue;
    }

    // set min/max/def positions
    if(c=='l')
    {
      const bool ret=servo.setMinPos(pos).ok();
      if(!ret)
        printw("ERROR SETTING MINIMAL POSITION\n");
      continue;
    }
    if(c=='h')
    {
      const bool ret=servo.setMaxPos(pos).ok();
      if(!ret)
        printw("ERROR SETTING MAXIMAL POSITION\n");
      continue;
    }
    if(c=='b')
    {
      const bool ret=servo.setDefaultPos(pos).ok();
      if(!ret)
        printw("ERROR SETTING DEFAULT POSITION\n");
      continue;
    }

    if(c=='q')
      break;
  }

  endwin();

  cout<<"exiting..."<<endl;

  return 0;
}
