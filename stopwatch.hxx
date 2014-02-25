#ifndef LLIBRARY_STOPWATCH_HXX
#define LLIBRARY_STOPWATCH_HXX

#ifdef __GNUC__
#include <sys/time.h>

namespace LLibrary
{
   
// Kleine Stopuhr
class StopWatch {
   timeval t1,t2;                                // Start und Stopzeit

protected:
   bool m_bValid;                                   // TRUE, wenn gestartet und gestoppt
   bool m_bRunning;                                 // TRUE, wenn gestartet.
   unsigned long m_nUsec;
   
public:
   StopWatch();
   virtual ~StopWatch() {}
   
   void start();                                 // Startet Timer
   void stop();                                  // Stoppt Timer
   
   double getSec() const;
   
   // ================ streams ================
    virtual std::ostream& print(std::ostream&) const;
   friend std::ostream& operator<< (std::ostream &_os, const StopWatch &_x);
   void printNiceOutput(std::ostream& _os);
};
#endif
} // namespace LLibrary

#endif
