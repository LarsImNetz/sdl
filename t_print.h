#ifndef T_PRINT_H
#define T_PRINT_H

#include <stdarg.h>

enum T_Print_Params {
    T_INFO = 1,
    T_VERBOSE,
    T_FLOWCONTROL,
    T_AUDIOPLAYCONTROL,
    T_DEBUG,
    T_ERROR
};

#define Bit_Flag_DEBUG     0x1
#define Bit_Flag_VERBOSE   0x2
#define Bit_Flag_RECTANGLE 0x4
#define Bit_Flag_QUIET     0xffff

// void t_print(const char*);
void t_print(const char*, ...);
void t_print(T_Print_Params, const char*, ...);

// print this always
void output(const char*, ...);

class GlobalStructure
{
    int m_nBits;
public:
    GlobalStructure()
            :m_nBits(0)
        {}
    
    bool isBit(int _nBitToCheck);
    void setBit(int _nBit);
    void delBit(int _nBit);
};

extern GlobalStructure aGlobalStructure;

#endif
