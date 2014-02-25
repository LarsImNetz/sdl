#include "storage.hxx"
#include "t_print.h"

const char* STORAGE_CURRENT_FILENAME      = "lastname";
const char* STORAGE_CURRENT_POS_AS_DOUBLE = "pos";
const char* STORAGE_CURRENT_VOLUMN        = "volumn";

bool Storage::open(int _nType)
{
    switch(_nType)
    {
    case STORAGE_READ:
        m_pStorage = fopen(m_sStorageName.c_str(), "r");
        break;
    case STORAGE_WRITE:
        m_pStorage = fopen(m_sStorageName.c_str(), "a");
        break;
    case STORAGE_WRITE_NEW:
        m_pStorage = fopen(m_sStorageName.c_str(), "w");
        break;
    default:
        t_print(T_DEBUG, "STORAGE TYPE unknown\n");
    }
    return m_pStorage != NULL ? true : false;
}
void Storage::close()
{
    if (m_pStorage)
    {
        fclose(m_pStorage);
        m_pStorage = NULL;
    }
}



Storage::Storage()
        : m_pStorage(NULL)
{
}

Storage::Storage(std::string const& _sStr)
        :m_sStorageName(_sStr),
         m_pStorage(NULL)
{
}

Storage::~Storage()
{
}

void Storage::empty()
{
    if (!open(STORAGE_WRITE_NEW)) return;
    close();
}

void Storage::store(std::string const& _sName, std::string const& _sString)
{
    if (!open(STORAGE_WRITE)) return;
    fprintf(m_pStorage, "%s:=%s\n", _sName.c_str(), _sString.c_str());
    close();
}

void Storage::store(std::string const& _sName, double _nValue)
{
    if (!open(STORAGE_WRITE)) return;
    fprintf(m_pStorage, "%s:=%f\n", _sName.c_str(), _nValue);
    close();
}

void Storage::store(std::string const& _sName, int _nValue)
{
    if (!open(STORAGE_WRITE)) return;
    fprintf(m_pStorage, "%s:=%d\n", _sName.c_str(), _nValue);
    close();
}

std::string Storage::getValue(std::string const& _sName)
{
    char buffer[1024];
    std::string sValue;
    if (!open(STORAGE_READ))
    {
        return std::string();
    }
    
    char* sStr = NULL;
    do
    {
        sStr = fgets(buffer, 1024, m_pStorage);
        if (sStr != NULL)
        {
            std::string sString(sStr);
            int nPos = sString.find(_sName);
            if (nPos == 0)
            {
                unsigned int nPos2 = sString.find(":=");
                if ( nPos2 == _sName.length())
                {
                    std::string sName = sString.substr( nPos2 + 2, sString.length() - nPos2 - 2 - 1);
                    sValue = sName;
                    sStr = NULL;
                    break;
                    // volatile int dummy = 0;
                }
            }
        }
    } while (sStr != NULL);
    close();
    return sValue;
}

std::string Storage::restore(std::string const& _sName, std::string const& _nValue)
{
    return getValue(_sName);
}

double Storage::restore(std::string const& _sName, double _nValue)
{
    std::string sValue = getValue(_sName);
    if (sValue.length() == 0) return 0.0;

    double nValue;
    /* int nBack = */ sscanf( sValue.c_str(), "%lf", &nValue );

    return nValue;
}

int Storage::restore(std::string const& _sName, int _nValue)
{
    std::string sValue = getValue(_sName);
    if (sValue.length() == 0) return 0;

    int nValue;
    /* int nBack = */ sscanf( sValue.c_str(), "%d", &nValue );

    return nValue;
}
