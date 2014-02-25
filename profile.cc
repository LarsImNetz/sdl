/*-*-c++-*-
 *
 *    $RCSfile: profile.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/11/21 20:13:48 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.3 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: profile.cc,v 1.3 2005/11/21 20:13:48 langha_l Exp $";

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: profile.cc,v $
 *       Revision 1.3  2005/11/21 20:13:48  langha_l
 *       store quality file all the time changes was created
 *
 *       Revision 1.2  2005/11/20 13:57:13  langha_l
 *       right handling of quality
 *
 *       Revision 1.1  2005/02/17 19:47:41  langha_l
 *       new profile
 *
 */

#include <fstream>
#include <cctype>

#include "profile.h"
#include "stringhelper.h"

#define MAX_READ_BUFFER 128


std::string str_trim(std::string const& _sStr) 
{
   // PRE  : %
   // POST : alle fuehrenden und abschliessenden Spaces werden geloescht
   int beg = 0;
   int nSize = _sStr.size();
   int end = nSize - 1;
   //   Bool isEnd = FALSE;
   
   while (_sStr[beg] == ' ') {                 // gibt es Spaces
      beg++;                                   // den Start raufsetzen
      if (beg >= nSize) {                      // ende erreicht?
         return "";                            // LeerString zurueck
      }
   }
   while (_sStr[end] == ' ') {                 // Am Ende suchen
      end--;
   }
   return _sStr.substr(beg, end - beg + 1);    // String zurueckgeben
}

namespace profile
{
// -----------------------------------------------------------------------
std::string readLine(std::istream &is, char _cDelimeter = '\n')
{
    char* pCharString = new char[MAX_READ_BUFFER];            // Hole Speicher
    int p = 0;                                    // zeiger in das Array

    std::string sReadChars;
    while(1)
    {
        char ch;
        if (is.get(ch))
        {                       // ein Zeichen lesen
            if (ch == '\r')                      // overread this char
            {
                continue;
            }
            // if (std::char_traits<char>::eof())
            if (is.eof())
            {
                break;
            }
            
            if (ch != _cDelimeter) // isalpha(ch) || isdigit(ch))
            {
                // sReadChars += ch;               // im Buffer speichern
                pCharString[p++] = ch;
            }
            else 
            {
                // is.putback(ch);                   // das Zeichen wieder zuruecklegen
                break;
            }
            if (p >= MAX_READ_BUFFER)            // 2. cancel condition, buffer full.
            {
                break;
            }
            
        }
        else
        {
            break;
        }
    }
    pCharString[p] = 0;
    sReadChars = std::string(pCharString);
    return sReadChars;
}
} // namespace profile

// -----------------------------------------------------------------------------

Section::Section()
{
    m_sSectionName = "# unknown";
}

Section::Section(std::string const& _sSectionName)
{
    m_sSectionName = _sSectionName;
}

int Section::getSize()
{
    return m_aMap.size();
}

void Section::write(std::ostream &os)
{
    for (MapStringString::const_iterator it=m_aMap.begin();
         it != m_aMap.end();
         it++)
    {
        std::pair<std::string, std::string> aPair = *it;
        std::string sKey = aPair.first;
        if (! isalpha(sKey[0]))
        {
            // handle as comment
            os << sKey;
        }
        else
        {
            std::string sValue = aPair.second;
            os << sKey << "=" << sValue << std::endl;
        }
    }
}

bool Section::insertValue(std::string const& _sKey, int _nValue)
{
    char text[20];
    sprintf(text, "%i", _nValue);
    std::string sValue(text);
    return insertValue(_sKey, sValue);
}

bool Section::insertValue(std::string const& _sKey, std::string const& _sValue)
{
    MapStringString::const_iterator it = m_aMap.find(_sKey);
    if (it != m_aMap.end())
    {
        std::string sOldValue = m_aMap[_sKey];
        if (sOldValue != _sValue)
        {
            m_aMap[_sKey] = _sValue;
            return true;
        }
    }
    else
    {
        m_aMap[_sKey] = _sValue;
        return true;
    }
    // new value is equal to old value 
    return false;
}

std::string Section::getValue(std::string const& _sKey)
{
    MapStringString::const_iterator it = m_aMap.find(_sKey);
    if (it != m_aMap.end())
    {
        return m_aMap[_sKey];
    }
    return "";
}

bool Section::removeValue(std::string const& _sKey)
{
    MapStringString::iterator it = m_aMap.find(_sKey);
    if (it != m_aMap.end())
    {
        m_aMap.erase(it);
        return true;
    }
    return false;
}


// -----------------------------------------------------------------------------


Profile::Profile(std::string const& _sName)
        :m_sName(_sName),
         m_bAlreadyLoaded(false),
         m_bContentChanged(false)
{}

Profile::~Profile()
{
    store();
}

void Profile::store()
{
    // store only if values has changed
    if (m_bContentChanged)
    {
        std::ofstream oss(m_sName.c_str(), std::ios::out);
        if (oss)
        {
            write(oss);
            m_bContentChanged = false;
        }
    }
}

void Profile::insertValue(std::string const& _sSection, std::string const& _sKey, std::string const& _sValue)
{
    load();

    bool bSectionExist = false;
    for (MapStringPtrSection::const_iterator it=m_aMap.begin();
         it != m_aMap.end();
         it++)
    {
        std::pair<std::string, Section*> aPair = *it;
        std::string sSectionName = aPair.first;
        if (sSectionName == _sSection)
        {
            Section* pSection = aPair.second;
            if (pSection->insertValue(_sKey, _sValue))
            {
                m_bContentChanged = true;
            }
            bSectionExist = true;
            break;
        }
    }
    if (bSectionExist == false)
    {
        Section *pSection = new Section(_sSection);
        pSection->insertValue(_sKey, _sValue);
        m_aMap[ _sSection ] = pSection;
        m_bContentChanged = true;
    }
}

void Profile::insertValue(std::string const& _sSection, std::string const& _sKey, int _nValue)
{
    char text[20];
    sprintf(text, "%d", _nValue);
    std::string sValue(text);
    insertValue(_sSection, _sKey, sValue);
}

void Profile::insertValue(std::string const& _sSection, std::string const& _sKey, double _nValue)
{
    char text[64];
    sprintf(text, "%f", _nValue);
    std::string sValue(text);
    insertValue(_sSection, _sKey, sValue);
}

std::string Profile::getValue(std::string const& _sSection, std::string const& _sKey)
{
    load();

    std::string sValue;
    bool bSectionExist = false;
    for (MapStringPtrSection::const_iterator it=m_aMap.begin();
         it != m_aMap.end();
         it++)
    {
        std::pair<std::string, Section*> aPair = *it;
        std::string sSectionName = aPair.first;
        if (sSectionName == _sSection)
        {
            Section *pSection = aPair.second;
            sValue = pSection->getValue(_sKey);
            bSectionExist = true;
            break;
        }
    }
    return sValue;
}

void Profile::removeValue(std::string const& _sSection, std::string const& _sKey)
{
    load();
    // search for the section
    for (MapStringPtrSection::const_iterator it=m_aMap.begin();
         it != m_aMap.end();
         it++)
    {
        std::pair<std::string, Section*> aPair = *it;
        std::string sSectionName = aPair.first;
        if (sSectionName == _sSection)
        {
            Section *pSection = aPair.second;
            if (pSection->removeValue(_sKey))
            {
                m_bContentChanged = true;
            }
            break;
        }
    }
}

bool Profile::load()
{
    if (m_bAlreadyLoaded == true)
    {
        return true;
    }
    
    std::ifstream iss(m_sName.c_str(), std::ios::in);
    if (iss)
    {
        read(iss);
        m_bAlreadyLoaded = true;
        return true;
    }
    return false;
}

void Profile::write(std::ostream &os)
{
    for (MapStringPtrSection::const_iterator it=m_aMap.begin();
         it != m_aMap.end();
         it++)
    {
        std::pair<std::string, Section*> aPair = *it;
        std::string sSectionName = aPair.first;
        if (isalpha(sSectionName[0]) ||
            sSectionName[0] == '/')       /* Filename */
        {
            Section *pSection = aPair.second;
            if (pSection->getSize() > 0)
            {
                os << "[" << sSectionName << "]" << std::endl;
                pSection->write(os);
            }
        }
        else
        {
            // handle as comment
            os << sSectionName;
        }
        // os << std::endl;
    }
}

// std::string Profile::readSection()
// {
// }

void Profile::read(std::istream &is)
{
    // std::string a;
    // a = str_trim("");
    // a = str_trim("a");
    // a = str_trim("ab");
    // a = str_trim(" ab");
    // a = str_trim(" ab ");
    // a = str_trim("  a bc   ");

    Section *pCurrentSection = NULL;
    while (1)
    {
        std::string aLine = str_trim(profile::readLine(is));
        if (aLine.length() == 0)
        {
        }
        else if (aLine[0] == '[')
        {
            // New Section
            int nIdx = aLine.find("]");
            if (nIdx != -1)
            {
                std::string sSectionName = str_trim(aLine.substr(1, nIdx - 1));
                pCurrentSection = new Section(sSectionName);
                m_aMap[ sSectionName ] = pCurrentSection;
            }
        }
        // else if (aLine[0] == '#' || aLine[0] == ';')
        // {
        //     // Comment
        //     std::string sSectionName = aLine;
        //     aCurrentSection = Section(sSectionName);
        // }
        else
        {
            int nIdx = aLine.find("=");
            std::string sKeyName;
            std::string sValue;
            if (nIdx != -1)
            {
                sKeyName = str_trim(aLine.substr(0, nIdx));
                sValue   = aLine.substr(nIdx + 1);
            }
            else
            {
                sKeyName = aLine;
            }
            if (pCurrentSection)
            {
                pCurrentSection->insertValue(sKeyName, sValue);
            }
        }

        // should be at the end, or maybe last line will lost.
        if (is.eof())
        {
            break;
        }
    }    
}


// # Comment
// # Structure
// [Music tag]
// title=
// year=
// [format]
// type=mp3
// [wave info]
// left_low=

void writeProfile(std::string const& _sFilename)
{
    
//             FILE *out = fopen(sInfoSoundFile.c_str(), "w");
//             if (out)
//             {
//                 fprintf(out, "# Autogenerated loudness information file for\n");
//                 fprintf(out, "# file: %s\n", sSoundFile.c_str());
//                 fprintf(out, "left_low   = %d\n", nLeftLow);
//                 fprintf(out, "left_high  = %d\n", nLeftHigh);
//                 fprintf(out, "right_low  = %d\n", nRightLow);
//                 fprintf(out, "right_high = %d\n", nRightHigh);
//                 fprintf(out, "abs_maximum = %f\n", fMaximum);
//                 fprintf(out, "wave_length = %d\n", nWaveLength);
//                 fclose(out);
//             }
}

void testProfile()
{
    Profile aProfile("/tmp/profiletest");
    std::string sValue = aProfile.getValue("Section", "a Key");
    // std::cout << sValue << std::endl;
    sValue = aProfile.getValue("unknown Section", "joint");
    // std::cout << sValue << std::endl;
    sValue = aProfile.getValue("Section", "unknown key");
    // std::cout << sValue << std::endl;
    
    aProfile.insertValue("Section2", "newkey", "new value.");
    aProfile.insertValue("Section2", "integer", 1);
    aProfile.insertValue("Section2", "double", 3.141592653589);
}

