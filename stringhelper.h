/*-*-c++-*-
 *
 *    $RCSfile: stringhelper.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/02/18 15:54:37 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.4 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: stringhelper.h,v $
 *       Revision 1.4  2005/02/18 15:54:37  langha_l
 *       cleanups for FileHelper:: and StringHelper::
 *
 *       Revision 1.3  2005/02/17 19:50:22  langha_l
 *       cleanups
 *
 *       Revision 1.2  2005/01/19 12:13:59  langha_l
 *       much changes for version 2, more modulisation
 *
 *       Revision 1.1  2003/05/12 16:25:58  langha_l
 *       lot of changes
 *
 */

#ifndef _stringhelper_h
#define _stringhelper_h

#include <string>
#include <vector>

namespace StringHelper
{
    
    typedef std::vector<std::string> StringList;

    std::string Quote(std::string const& _sStr);
    std::string shortString(std::string const& _sStr);
    
    std::string toLowerCase(std::string const& _sStr);
} // namespace StringHelper

#endif
