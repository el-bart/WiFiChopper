#include <iostream>
#include <mutex>

#include "NavWin.hpp"

using namespace std;

/** \brief helper thread object, for doing communications.
 */
struct NavWin::CommThread
{
  /** \brief short name for lock object. */
  typedef std::unique_lock<std::mutex> Lock;

  /** \brief creates communication thread.
   *  \param dev   device to use for communications.
   *  \param nameX name of the OX axis servo.
   *  \param nameY name of the OY axis servo.
   */
  CommThread(ServoCtrl::CommDevicePtrNN dev, ServoCtrl::ServoName nameX, ServoCtrl::ServoName nameY):
    srvX_{nameX, dev},
    srvY_{nameY, dev},
    posXsent_{0},
    posYsent_{0},
    quit_{false}
  {
    setX(127);
    setY(127);
  }

  /** \brief signals thread to exit.
   */
  void quit(void)
  {
    Lock lock{m_};
    quit_=true;
  }

  /** \brief sets OX servo to a given position.
   *  \param x position to set.
   */
  void setX(uint8_t x)
  {
    Lock lock{m_};
    posX_=x;
  }
  /** \brief gets currently set OX servo position.
   */
  uint8_t getX(void) const
  {
    Lock lock{m_};
    return posX_;
  }

  /** \brief sets OY servo to a given position.
   *  \param y position to set.
   */
  void setY(uint8_t y)
  {
    Lock lock{m_};
    posY_=y;
  }
  /** \brief gets currently set OY servo position.
   */
  uint8_t getY(void) const
  {
    Lock lock{m_};
    return posY_;
  }

  /** \brief main thread's loop.
   */
  void run(void)
  {
    while(!quitNow())
    {
      if( posXsent_!=getX() )
      {
        const uint8_t p=getX();
        if( send(srvX_, p) )
          posXsent_=p;
      }

      if( posYsent_!=getY() )
      {
        const uint8_t p=getY();
        if( send(srvY_, p) )
          posYsent_=p;
      }

      // TODO: make this conditional
      while(posXsent_==getX() && posYsent_==getY() && !quitNow())
      {
        const struct timespec ts={0, 100*1000};
        nanosleep(&ts, nullptr);
      }
    }
  }

private:
  bool quitNow(void) const
  {
    Lock lock{m_};
    return quit_;
  }

  bool send(ServoCtrl::Servo &srv, const uint8_t pos)
  {
    cout<<"sending "<<int{pos}<<endl;
    try
    {
      srv.setPos(pos);
      return true;
    }
    catch(const std::exception &ex)
    {
      cerr<<"oops: "<<ex.what()<<" - proceeding"<<endl;
    }
    return false;
  }

  mutable std::mutex m_;

  ServoCtrl::Servo srvX_;   ///< OX axis servo.
  ServoCtrl::Servo srvY_;   ///< OY axis servo.

  uint8_t posXsent_;        ///< last-sent position OX.
  uint8_t posYsent_;        ///< last-sent position OY.

  uint8_t posX_;            ///< requested OX position.
  uint8_t posY_;            ///< requested OY position.

  bool quit_;               ///< exit-indicator flag.
}; // struct NavWin::CommThread



NavWin::NavWin(ServoCtrl::CommDevicePtrNN dev, ServoCtrl::ServoName nameX, ServoCtrl::ServoName nameY):
  pressed_{false},
  thImpl_{ new CommThread{dev, nameX, nameY} },
  th_{ [&]{ thImpl_->run(); } }
{
  set_title("mouse position event");
  add_events( Gdk::POINTER_MOTION_MASK | Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
  show_all_children();
}

NavWin::~NavWin(void)
{
  cout<<"exit..."<<endl;
  thImpl_->quit();
  thImpl_->setX(166);
  thImpl_->setY(166);
  if(th_.joinable())
    th_.join();
}


bool NavWin::on_motion_notify_event(GdkEventMotion* event)
{
  if(event==nullptr)
    return false;

  if(!pressed_)
    return true;

  posUpdate(posX_, lastX_, event->x);
  thImpl_->setX(posX_);
  lastX_=event->x;

  posUpdate(posY_, lastY_, event->y);
  thImpl_->setY(posY_);
  lastY_=event->y;

  return true;
}

bool NavWin::on_button_press_event(GdkEventButton* event)
{
  if(event==nullptr)
    return false;

  pressed_=true;
  lastX_  =event->x;
  lastY_  =event->y;
  return true;
}


bool NavWin::on_button_release_event(GdkEventButton* event)
{
  if(event==nullptr)
    return false;

  pressed_=false;
  return true;
}


void NavWin::posUpdate(uint8_t &pos, double last, double now)
{
  const int dp=posDiff(last, now);
  int       p =pos+dp;
  // limit it a bit...
  if(p<50)
    p=50;
  if(p>200)
    p=200;
  // ok
  pos=p;
}


int NavWin::posDiff(double last, double now)
{
  const int d=2;

  if(last<now)
    return +d;
  if(last>now)
    return -d;
  return 0;
}
