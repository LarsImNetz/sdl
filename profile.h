/*-*-c++-*-
 *
 *    $RCSfile: profile.h,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/21 20:13:48 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.2 $
 */

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: profile.h,v $
 *       Revision 1.2  2005/11/21 20:13:48  langha_l
 *       store quality file all the time changes was created
 *
 *       Revision 1.1  2005/02/17 19:47:41  langha_l
 *       new profile
 *
 */

#ifndef _PROFILE_H
#define _PROFILE_H

#include <map>
#include <iostream>
#include <string>

class Section
{
    typedef std::map<std::string, std::string> MapStringString;
    std::string m_sSectionName;
    MapStringString m_aMap;
public:
    Section();
    Section(std::string const& _sSectionName);
    // write the content to the ostream
    void write(std::ostream &os);
    // insert a value, return true, if the value doesn't exist
    bool insertValue(std::string const& _sKey, std::string const& _sValue);
    bool insertValue(std::string const& _sKey, int _nValue);

    std::string getValue(std::string const& _sKey);
    // remove a key, return true, if really removed
    bool removeValue(std::string const& _sKey);
    // return size of the content list
    int getSize();
};

class Profile
{
    typedef std::map<std::string, Section*> MapStringPtrSection;
    MapStringPtrSection m_aMap;
    std::string m_sName;
    bool m_bAlreadyLoaded;
    bool m_bContentChanged;

public:
    Profile(std::string const& _sName);
    ~Profile();

    void insertValue(std::string const& _sSection, std::string const& _sKey, std::string const& _sValue);
    void insertValue(std::string const& _sSection, std::string const& _sKey, int _sValue);
    void insertValue(std::string const& _sSection, std::string const& _sKey, double _sValue);

    std::string getValue(std::string const& _sSection, std::string const& _sKey);
    void removeValue(std::string const& _sSection, std::string const& _sKey);

    void write(std::ostream &os);
    void read(std::istream &is);

    // returns true, if load is done
    bool load();

    // store the whole file
    //! we a a little unstable with playing music
    void store();
};


void testProfile();

#endif
