/*-*-c++-*-
 *
 *    $RCSfile: ioextend.h,v $
 *     Project: l_object
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2009/09/24 06:50:41 $
 *   Copyright: (c)1995 Lars Langhans
 *   $Revision: 1.4 $
 */



#ifndef IOEXTEND_H
#define IOEXTEND_H

#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

// #include <assert.h>
// #include <ctype.h>
// #include <string.h>

// Haeufig benoetigte IO Funktionen
std::string readInt(std::istream &is,int maxlen);

int readInt(std::istream &is);
std::string readString(std::istream &is);

int readNumber(std::istream &is);
// LObject* readLObject(std::istream &is,std::string const& typ);

void writeNumber(std::ostream &os,int value);


void writeTyp(std::ostream &os,std::string const &string);
std::string readTyp(std::istream &is);

// template <typename T>
void* readObject(std::istream &is, std::string const& _sType /*, T & t */);

// template <typename T>
#define CreateFunc_readVector(T) \
int readVector(std::istream &is, std::vector< T > &_aVector)          \
{                                                                     \
    int i;                                                            \
    /* std::vector<std::string> aVector; */                           \
                                                                      \
    i = readNumber(is);                        /* Anzahl der Items */ \
    if (is.good() && i > 0)                                           \
    {                                                                 \
        for (int n = 0;n<i;n++)                                       \
        {                                                             \
            std::string sType = readTyp(is);                          \
            char ch;                                                  \
            is.get(ch);                                               \
            if (ch == '\t')                                           \
            {                                                         \
                void* pType = readObject(is, sType);                  \
                T *pt = static_cast<T*>(pType);                       \
                _aVector.push_back(*pt);                              \
                delete pt;                                            \
            }                                                         \
            else                                                      \
            {                                                         \
                /* Format unknown */                                  \
            }                                                         \
        }                                                             \
    }                                                                 \
    return i;                                                         \
}

#define use_readVector(T) \
extern int readVector(std::istream &is, std::vector< T > &_aVector);

// template <typename T>
// int readVector(std::istream &is, std::vector<T> &_aVector);

template <class T>
void writeObject(std::ostream &os, T const& _aObj)
{
    if ( typeid(_aObj) == typeid(std::string) )
    {
        writeTyp(os, "string");
        os << _aObj << std::endl;
    }
    if ( typeid(_aObj) == typeid(int) )
    {
        writeTyp(os, "int");
        os << _aObj << std::endl;
    }
    // unknown?
}

// template <typename T>
// std::ostream& writeVector(std::ostream &os, std::vector< T > const& _aVector);

#define CreateFunc_writeVector(T) \
std::ostream& writeVector(std::ostream &os, std::vector< T > const& _aVector) \
{                                                                             \
   /* PRE  : */                                                                 \
   /* POST : */                                                                 \
                                                                              \
    /* LLA?: how can I be save it's a vector<X>???  */                          \
                                                                              \
    int nItems = _aVector.size();                                             \
    writeNumber(os, nItems);                        /* Anzahl der ArrayItems */ \
                                                                              \
    for(std::vector< T >::const_iterator it = _aVector.begin();                              \
        it != _aVector.end();                                                 \
        ++it)                                                                 \
    {                                                                         \
        writeObject(os, (*it));                                               \
    }                                                                         \
    return os;                                                                \
}                                                                             

#define use_writeVector(T) \
extern std::ostream& writeVector(std::ostream &os, std::vector< T > const& _aVector);


use_writeVector(std::string);
use_readVector(std::string);

#endif

