#ifndef INCLUDE_COMMLOSTPORTECTOR_HPP_FILE
#define INCLUDE_COMMLOSTPORTECTOR_HPP_FILE

/** \brief class wrapping protection against not receiving commands.
 */
struct CommLostProtector
{
  static void enable(void);

  static void reset(void);
  static void reactIfTimeout(void);
};

#endif
