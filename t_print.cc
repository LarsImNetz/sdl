#include <stdio.h>
#include <limits.h>
#include "t_print.h"
// -----------------------------------------------------------------------------
// ----------------------------- own printf method -----------------------------
// -----------------------------------------------------------------------------

GlobalStructure aGlobalStructure;

bool GlobalStructure::isBit(int _nBitToCheck)
{
    if ((m_nBits & _nBitToCheck) == _nBitToCheck)
    {
        return true;
    }
    return false;
}
void GlobalStructure::setBit(int _nBit)
{
    m_nBits |= _nBit;
}
void GlobalStructure::delBit(int _nBit)
{
    int m_nBitMask = INT_MAX ^ _nBit;
    m_nBits &= m_nBitMask;
}

bool isDebug()
{
    // return true;
	if ( aGlobalStructure.isBit( Bit_Flag_DEBUG ) ) 
	{ 
        return true;
	} 
    return false;
}

bool isVerbose()
{
	if ( aGlobalStructure.isBit( Bit_Flag_VERBOSE ) ) 
	{ 
        return true;
	} 
    return false;
}

bool isQuiet()
{
	if ( aGlobalStructure.isBit( Bit_Flag_QUIET ) ) 
	{ 
        return true;
	} 
    return false;
}

void impl_t_print(const char* _pFormatStr, va_list &args)
{
    if (! isQuiet())
    {
        // printf("# ");
        vprintf(_pFormatStr, args);
    }
}

void t_print(const char* _pFormatStr, ...)
{
    va_list  args;
    va_start( args, _pFormatStr );
    impl_t_print(_pFormatStr, args);
    va_end( args );
}

void t_print(T_Print_Params _eType, const char* _pFormatStr, ...)
{
    if (_eType == T_VERBOSE && isVerbose())
    {
        // if T_VERBOSE and we should be verbose, print info
        va_list  args;
        va_start( args, _pFormatStr );
        impl_t_print(_pFormatStr, args);
        va_end( args );
    }

    if (_eType == T_DEBUG && isDebug())
    {
        // if T_DEBUG and we should be debug output, print debug info
        va_list  args;
        va_start( args, _pFormatStr );
        impl_t_print(_pFormatStr, args);
        va_end( args );
    }
    
    if (_eType == T_AUDIOPLAYCONTROL && isVerbose())
    {
        // if T_AUDIOPLAYCONTROL and we should be verbose, print flow control information
        va_list  args;
        va_start( args, _pFormatStr );
        impl_t_print(_pFormatStr, args);
        va_end( args );
    }

    if (_eType == T_FLOWCONTROL && isVerbose())
    {
        // if T_FLOWCONTROL and we should be verbose, print flow control information
        va_list  args;
        va_start( args, _pFormatStr );
        impl_t_print(_pFormatStr, args);
        va_end( args );
    }

    if (_eType == T_INFO && isVerbose())
    {
        // if T_INFO and we should be verbose, print info
        va_list  args;
        va_start( args, _pFormatStr );
        impl_t_print(_pFormatStr, args);
        va_end( args );
    }
}

void impl_output(const char* _pFormatStr, va_list &args)
{
    vprintf(_pFormatStr, args);
}

void output(const char* _pFormatStr, ...)
{
// printf("%s\n", _pString);

    va_list  args;
    va_start( args, _pFormatStr );
    impl_output(_pFormatStr, args);
    va_end( args );
}
