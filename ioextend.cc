/*-*-c++-*-
 *
 *    $RCSfile: ioextend.cc,v $
 *     Project: l_object
 * Description: IO Erweiterungen
 *              Haeufig gebrauchte IO Funktionen fuer die Bibliothek
 *
 *   Author(s): Lars Langhans
 *       $Date: 2009/09/24 06:50:41 $
 *   Copyright: (c)1995 Lars Langhans
 *   $Revision: 1.6 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: ioextend.cc,v 1.6 2009/09/24 06:50:41 langha_l Exp $";


// #include <l_object/text.h>                       // Strings
#include <string>
#include <vector>

//#include "tint.h"
//#include "tfloat.h"
#include <iostream>
#include "ioextend.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

std::string readInt(std::istream &is,int _nMaxLen) {
   //  pre: den inputStream und die maximale Laenge an Zeichen
   // post: der eingelesenen String
   
   char* pCharString = new char[_nMaxLen+1];            // Hole Speicher
   // das macht der new operator selbst
   // assert(pCharString);
   int p = 0;                                    // zeiger in das Array
   char ch;
   
   is >> ch;                                     // 1. Zeichen einlesen
   // evtl. 'whitespaces' werden am Anfang ueberlesen!
   
   if (ch == '+' || ch == '-' || isdigit(ch)) {
      pCharString[p++] = ch;
      ch = '1';                                  // Pseudozahl wird nur fuer Schleifenstart benoetigt.
      
      while (isdigit(ch)) {                      // Leseschleife
         is.get(ch);                             // Zeichen holen
         pCharString[p++] = ch;                       // Egal was, rein in buffer
         assert(p < _nMaxLen);
      }
      pCharString[--p] = '\0';                        // Das letzte Zeichen war keine Zahl ==>
      // buffer um 1 zurueck, letztes Zeichen durch
      // '\0' ersetzen
      is.putback(ch); // zeichen zurueck in Stream
      
   }
   else {
      is.clear(std::ios::failbit|std::ios::badbit);        // Fehler kein Int
      strcpy(pCharString,"0");
   }
   std::string aBack(pCharString);
   delete pCharString;
   return aBack;
}

// -----------------------------------------------------------------------
std::string readLine(std::istream &is, char _cDelimeter = '\n')
{
    std::string sReadChars;
    while(1)
    {
        char ch;
        if (is.get(ch))
        {                       // ein Zeichen lesen
            if (ch != _cDelimeter) // isalpha(ch) || isdigit(ch))
            {
                sReadChars += ch;                // im Buffer speichern
            }
            else 
            {
                is.putback(ch);                   // das Zeichen wieder zuruecklegen
                break;
            }
        }
    }
    return sReadChars;    
}

// -----------------------------------------------------------------------
int readNumber(std::istream &is) {
   //  pre: ein initialisierter Stream
   // post: liefert ein int, der die Anzahl zu lesender Values enthaelt.
   // Format [n] n ist Element von Natuerlichen Zahlen

   char ch;
   int value;
   
   is >> ch;
   if (ch == '[') {                              // '['
      is >> value >> ch;                         // dimY
      if (ch != ']') {                           // ']'
         is.putback(ch);
         is.clear(std::ios::failbit|std::ios::badbit);     // ']' erwartet
      }
   }
   else {
      is.putback(ch);
      is.clear(std::ios::failbit|std::ios::badbit);        // '[' erwartet
   }
   return value;
}
// =============================================================================
std::string readTyp(std::istream &is) 
{
   //  pre: ein initialisierter Stream
   // post: liefert ein std::string Object, das den Namen des Objectes beschreibt.
   // Format <n> n ist Name einen Objektes aus 'LObject'.

   char ch;
   std::string sString;
   
   is >> ch;
   if (ch == '<')
   {                                             // '<'
       sString = readLine(is, '>');                   // String einlesen
       is >> ch;
       if (ch != '>')
       {                                         // '>'
           is.putback(ch);
           is.clear(std::ios::failbit|std::ios::badbit);     // '>' erwartet
       }
   }
   else {
       is.putback(ch);
       is.clear(std::ios::failbit|std::ios::badbit);        // '<' erwartet
   }
   is.get(ch); // should be '\t'
   return sString;
}
// -----------------------------------------------------------------------
void writeNumber(std::ostream &os,int value) {
   //  pre: ein initialisierter Stream
   // post: schreibe '[value]' in den Stream

   os << '[' << value << ']' << std::endl;
}

// -----------------------------------------------------------------------
void writeTyp(std::ostream &os,const std::string & string) {
   //  pre: ein initialisierter Stream
   // post: schreibe '<string>' in den Stream

   os << '<' << string << '>';                   // <TypeName>
   os << '\t';                                   // Separator
}

// -----------------------------------------------------------------------
// LObject* readLObject(std::istream &is,std::string& typ) {
   //  pre: ein initialisierter Stream
   // post: lese ein LObject mit hilfe seines Names!

   // Hier muessen LEIDER ALLE vorhandenen Objekttypen eingetragen werden.
   // Es handelt sich um eine unsaubere Methode, die zwar viel aufwand
   // erfordert, aber trotzdem funktioniert.

/*
#define TYP(text)               \
   if (typ == Text(#text)) {    \
   text obj;                    \
   is >> ws >> obj;             \
   return obj.clone();          \
}
*/

// 
//    TYP(Text);
//    //   TYP(TInt);
//    //   TYP(TFloat);
//    
//    TYP(Date);                                    // Datum & Zeit
//    TYP(Time);
//    TYP(DateTime);
//    TYP(Period);
//    
//    TYP(BigInt);                                  // Zahlen
//    TYP(Int);
//    TYP(Double);
//    TYP(Matrix);
//    TYP(Vector);
//    
//    TYP(Assoc);                                   // Sammeltypen
//    
//    TYP(Array);                                   // Container
//    TYP(Queue);
//    TYP(Stack);
//    TYP(Dict);
//    
// #undef TYP
   
//    is.clear(std::ios::failbit|std::ios::badbit);           // Fehler kein Typ vorhanden
//    return NULL;
// }


// -----------------------------------------------------------------------------
std::string readString(std::istream &is)
{
    std::string sTyp = readTyp(is);
    if (sTyp == "string")
    {
        std::string aStr = readLine(is);
        return aStr;
    }
    return std::string();
}

int readInt(std::istream &is)
{
    std::string sTyp = readTyp(is);
    if (sTyp == "int")
    {
        std::string sValue = readInt(is, 30);
        int nValue = atoi(sValue.c_str());
        return nValue;
    }
    return 0;
}

// -----------------------------------------------------------------------------

void* readObject(std::istream &is, std::string const& _sType /*, T & t */)
{
    if ( _sType == "string" /* && typeid(std::string) == typeid(t)*/ )
    {
        std::string *pStr = new std::string;
        *pStr = readLine(is);
        return static_cast<void*>(pStr);
    }
    if ( _sType == "int" /* && typeid(int) == typeid(t) */ )
    {
        int *pValue = new int;
        *pValue = -1; /* DEFAULT */
        std::string sValue = readInt(is, 30);
        *pValue = atoi(sValue.c_str());
        return static_cast<void*>(pValue);
    }
    return NULL;
}

// -----------------------------------------------------------------------------
// template <typename T>
// std::ostream& writeVector(std::ostream &os, T const& _aVector)
// {
//    // PRE  :
//    // POST :
//    
//     // LLA?: how can I be save it's a vector<X>???
// 
//     int nItems = _aVector.size();
//     writeNumber(os, nItems);                        // Anzahl der ArrayItems
//    
//     for(T::const_iterator it = _aVector.begin();
//         it != _aVector.end();
//         ++it)
//     {
//         writeObject(os, (*it));
//     }
//     
//    // for(int i=0; i<Items; i++) {
//    //    if(a[i]) {                                 // Gibt es an dieser Stelle ein Objekt?
//    //       os << i << ' ';                         // Den Index
//    //       writeTyp(os, a[i]->Class()->name());
//    //       // os << '\t';
//    //       a[i]->print(os);                        // Das enthaltende LObject
//    //       os << endl;                             // Return
//    //    }
//    //    else        
//    //       os << i << " <nil>" << endl;              // nil falls leer
//    // }
//    return os;
// }

CreateFunc_writeVector( std::string );
CreateFunc_readVector( std::string );

