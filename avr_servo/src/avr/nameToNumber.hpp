#ifndef INCLUDE_NAMETONUMBER_HPP_FILE
#define INCLUDE_NAMETONUMBER_HPP_FILE

#include "config.hpp"

/** \brief converts name of the servo ('a'...'r') to it's number (0..17).
 *  \param n name of the servo.
 *  \return servo number [0;17] or -1 on error (no such servo).
 */
int nameToNumber(char n);

#endif
