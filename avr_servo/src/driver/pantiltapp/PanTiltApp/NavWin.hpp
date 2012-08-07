#ifndef HELLOWORLDWINDOW_HPP_FILE
#define HELLOWORLDWINDOW_HPP_FILE

#include <thread>
#include <memory>
#include <cinttypes>
#include <gtkmm/window.h>

#include "ServoCtrl/CommDevice.hpp"
#include "ServoCtrl/ServoName.hpp"
#include "ServoCtrl/Servo.hpp"

/** \brief navigation window for moving two servos.
 */
class NavWin: public Gtk::Window
{
public:
  /** \brief create window instance.
   *  \param dev   device to use for communications.
   *  \param nameX name of the OX axis servo.
   *  \param nameY name of the OY axis servo.
   */
  NavWin(ServoCtrl::CommDevicePtrNN dev, ServoCtrl::ServoName nameX, ServoCtrl::ServoName nameY);
  /** \brief destroy window.
   */
  ~NavWin(void);

private:
  struct CommThread;

  virtual bool on_motion_notify_event(GdkEventMotion* event);
  virtual bool on_button_press_event(GdkEventButton* event);
  virtual bool on_button_release_event(GdkEventButton* event);

  void posUpdate(uint8_t &pos, double last, double now);
  int posDiff(double last, double now);
  void send(ServoCtrl::Servo &srv, uint8_t pos);

  bool    pressed_;
  double  lastX_;
  double  lastY_;

  uint8_t posX_;
  uint8_t posY_;

  std::unique_ptr<CommThread> thImpl_;
  std::thread                 th_;
};

#endif
