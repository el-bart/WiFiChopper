#ifndef INCLUDE_CHRONOTABLE_HPP_FILE
#define INCLUDE_CHRONOTABLE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "Positions.hpp"
#include "EntryTable.hpp"
#include "Noncopyable.hpp"

/** \brief class gathering and parsing positions, providing
 *         fast-to-process output table.
 */
class ChronoTable: private Noncopyable
{
public:
  /** \brief initialize data.
   */
  explicit ChronoTable(const Positions &defaultPositions);

  /** \brief return reference to table of current positions.
   *  \return reference fo current positions to be changed.
   */
  Positions &currentPos(void)
  {
    return cur_;
  }

  /** \brief compute new entry table basing on new settings.
   */
  void update(void);

  /** \brief get preprocessed entries, so that it can be applied to the ports.
   */
  const EntryTable &currentEntries(void) const
  {
    return e_;
  }

private:
  Positions  cur_;      // current positions
  EntryTable e_;        // processed entries
}; // class ChronoTable

#endif
