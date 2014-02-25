#ifndef STORAGE_HXX
#define STORAGE_HXX

#define STORAGE_READ       1
#define STORAGE_WRITE      2
#define STORAGE_WRITE_NEW  3

extern const char* STORAGE_CURRENT_FILENAME;
extern const char* STORAGE_CURRENT_POS_AS_DOUBLE;
extern const char* STORAGE_CURRENT_VOLUMN;

#include <string>
#include <stdio.h>
#include "ioextend.h"

class Storage
{
    std::string m_sStorageName;
    FILE       *m_pStorage;

private:
    Storage(Storage const&);
    Storage& operator=(Storage const&);

    // open the file 'm_sStorageName'
    bool open(int _nType);
    void close();

    std::string getValue(std::string const&);
public:
    
    Storage();
    Storage(std::string const& _sStr);

    ~Storage();
    
    void empty();
    void store(std::string const& _sName, std::string const& _sString);
    void store(std::string const& _sName, double _nValue);
    void store(std::string const& _sName, int _nValue);

    double restore(std::string const& _sName, double _nDummy);
    std::string restore(std::string const& _sName, std::string const& _nDummy);
    int restore(std::string const& _sName, int _nDummy);
};

#endif

