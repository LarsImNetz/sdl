/*-*-c++-*-
 *
 *    $RCSfile: stringhelper.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/04/11 17:01:48 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.4 $
 */

// -----------------------------------------------------------------------------

#include "stringhelper.h"

namespace StringHelper
{
    
std::string Quote(std::string const& _sStr)
{
    std::string aStr = "\"";
    aStr += _sStr;
    aStr += "\"";
    return aStr;
}

std::string toLowerCase(std::string const& _sStr)
{
#warning unready code toLowerCase()
    int nLength = _sStr.length();
// for (int i=0;i<nLength;i++)
// {
//     _sStr[i] = tolower(_sStr[i]);
// }
    
    return _sStr;
}


std::string shortStringMiddle(std::string const& _sStr)
{
    int nLength = _sStr.length();
    int nLengthFirstPart = nLength / 2;
    int nLengthLastPart = nLengthFirstPart + 2;

    int n3Dots = _sStr.find("...");
    if (n3Dots != -1)
    {
        nLengthFirstPart = n3Dots - 1;
        nLengthLastPart = n3Dots + 3;
    }
    
    std::string sFirstPart = _sStr.substr(0, nLengthFirstPart);
    std::string sLastPart = _sStr.substr(nLengthLastPart + 1);

    std::string sNew = sFirstPart + "..." + sLastPart;
    return sNew;
}

// -----------------------------------------------------------------------------
std::string shortString(std::string const& _sStr)
{
    int nLength = _sStr.length();
    int nLengthFirstPart = nLength / 3;
    int nIndexStartLastPart = nLengthFirstPart + 2;

    int n3Dots = _sStr.find("...");
    if (n3Dots != -1)
    {
        nLengthFirstPart = n3Dots - 1;
        nIndexStartLastPart = n3Dots + 3;
    }
    
    std::string sFirstPart = _sStr.substr(0, nLengthFirstPart);
    std::string sLastPart = _sStr.substr(nIndexStartLastPart + 1);

    std::string sNew = sFirstPart + "..." + sLastPart;
    return sNew;
}
 
} // namespace StringHelper
