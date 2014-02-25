/*-*-c++-*-
 *
 *    $RCSfile: stopwatch.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/10/19 06:11:21 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.1 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: stopwatch.cc,v 1.1 2005/10/19 06:11:21 langha_l Exp $";

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: stopwatch.cc,v $
 *       Revision 1.1  2005/10/19 06:11:21  langha_l
 *       add StopWatch for time measure
 *
 */

#include <iostream>
#include <stdlib.h>

#include "stopwatch.hxx"

namespace LLibrary
{
   
// ================================= Stop Watch =================================

// Eine kleine Stop-Uhr fuer den internen Gebrauch.
// (c) Lars Langhans 29.12.1996 22:10

#ifdef __GNUC__
   StopWatch::StopWatch()
         :m_bValid(false),
          m_bRunning(false) {}

void StopWatch::start() {
// pre: %
// post: Startet Timer

   m_bValid = false;
   m_bRunning = true;
   gettimeofday(&t1, 0);
}

void StopWatch::stop() {
// pre: Timer sollte gestartet sein.
// post: Timer wird gestoppt.
   
   gettimeofday(&t2, 0);                         // Timer ausfragen

   if (m_bRunning) {                                // check ob gestartet.
      while (t2.tv_sec > t1.tv_sec) {
         t2.tv_usec += 1000000;
         t1.tv_sec++;
      }
   
      m_nUsec = t2.tv_usec - t1.tv_usec;            // mircosec. berechnen
      m_bValid = true;
      m_bRunning = false;
   }
}

double StopWatch::getSec() const {
// pre: gueltig = TRUE
// BACK: Zeit in Sekunden.

   double nBack = 0;
   if (m_bValid) {
      nBack = double(m_nUsec) / 1000000.0;
   }
   return nBack;
}

std::ostream& StopWatch::print(std::ostream& _aOstream) const {
   _aOstream << getSec();
   return _aOstream;
}

void StopWatch::printNiceOutput(std::ostream& _os) {
// post: Ausgabe als Text.
   _os << "Benoetigte Zeit: ";
   print(_os);
   _os << " sec." << std::endl;
}

std::ostream& operator<< (std::ostream &_os, const StopWatch &_x) {
   return _x.print(_os);
}

#else
#error SORRY, NO STOPWATCH
#endif
} // namespace LLibrary
