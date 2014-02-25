/*-*-c++-*-
 *
 *    $RCSfile: test_exec_mplayer.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2008/10/24 17:57:28 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.1 $
 */

// damit kann RCS im executeable noch die Version feststellen
// static char rcsid[] = "$Id: test_exec_mplayer.cc,v 1.1 2008/10/24 17:57:28 langha_l Exp $";

/* ==========================================================================
 *
 *               HISTORY
 *               -------
 *       $Log: test_exec_mplayer.cc,v $
 *       Revision 1.1  2008/10/24 17:57:28  langha_l
 *       add test for mplayer
 *
 *       Revision 1.1  2006/04/13 11:49:35  langha_l
 *       test call to a external program
 *
 */

#include <iostream>
#include <string>

#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// If you get an error such as this:
// /usr/bin/ld: errno: TLS definition in /lib/libc.so.6 section .tbss mismatches
// non-TLS reference in [...]
// 
// What you need to do is this: open the appropriate makefile (i.e. in TKIGES), find the line DEFAULT_INCLUDES, and add -include /usr/include/errno.h to the end. 
#include <errno.h>

int errno;

class DoExec
{
    // int m_aFile;

    DoExec(DoExec const&);

int m_nPid;
std::string m_sResult;
std::string m_sFilename;
int status;

#define FILE_READ 0
#define FILE_WRITE 1


void init()
{
    int DEV_null = open("/dev/null", O_WRONLY);

    int aFileDescriptor[2];
    if (pipe(aFileDescriptor) != 0)
    {
        // throws
        return;
    }
    
    m_nPid = fork();
    if (m_nPid == -1)
    {
        std::cout << "fork fails: " << std::endl;
        exit(0);
    }
    
    if (m_nPid == 0)
    {
        // This is the child process
        
        close(aFileDescriptor[FILE_READ]);          /* Close unused read end */
        dup2(aFileDescriptor[FILE_WRITE], fileno(stdout));
        dup2(DEV_null, fileno(stderr));
        int n = execlp( "/usr/bin/mplayer", "mplayer", "-ao", "null", /* "-slave", "-vo", "null", */ m_sFilename.c_str(), (const char*)NULL);
        std::cerr << "failure to call mplayer. It returns: " << errno << std::endl;
        exit(n);
        // close(aFileDescriptor[FILE_WRITE]);          /* Reader will see EOF */
    }
    else
    {
        close(aFileDescriptor[FILE_WRITE]);
        
        char buf;
        while (read(aFileDescriptor[FILE_READ], &buf, 1) > 0)
        {
            // write(STDOUT_FILENO, &buf, 1);
            m_sResult += buf;
            std::cout << buf;
            if (buf == '\r')
            {
                std::cout << std::endl;
                std::flush(std::cout);
            }
            // write(STDOUT_FILENO, " ", 1);
        }
        
        // write(STDOUT_FILENO, "\n", 1);
        close(aFileDescriptor[FILE_READ]);
        
        // dup2(aFileDescriptor[FILE_READ], fileno(stdin));
        // close(aFileDescriptor[FILE_READ]);
        
        // dup2(m_aFile, fileno(stdout));
        // dup2(DEV_null, fileno(stderr));
        
        // int n = execve("/usr/bin/mp3info", argv, env);
    }
    // close(aFileDescriptor[FILE_READ]);
    // decinpipe = aFileDescriptor[FILE_WRITE];
    close(DEV_null);
}

void end()
{
    if (m_nPid) 
    {
        // if (wait)
        //     ioctl(m_aFile, SNDCTL_DSP_POST, 0);
        // close(decinpipe);
        // if (wait)
        //    ioctl(m_aFile, SNDCTL_DSP_SYNC, 0);
        waitpid(m_nPid, &status, WNOHANG);
        // kill(m_nPid, SIGKILL);
        // close(m_aFile);
        m_nPid = 0;
        // decinpipe = 0;
    }
}

public:
    DoExec(std::string _sFilename)
        {
            m_sFilename = _sFilename;
            init();
        }

    ~DoExec()
        {
            end();
        }
    std::string getResult() 
        {
            return m_sResult;
        }
};

// -----------------------------------------------------------------------------
int main(int argc, char* argv[], char* env[])
{
    std::cout << "fork test" << std::endl;
    for (int i=0;i<argc;i++)
    {
        std::cout << argv[i] << std::endl;
    }
    if (argc < 2)
    {
        exit(1);
    }
    
    // -----------------------------------------------------------------------------

    // we will wait, until mp3info returns successfully
    // int status;
    // waitpid(nPid, &status, WNOHANG);
    std::string sResult;
    DoExec *pExec = new DoExec(argv[1]);
    sResult = pExec->getResult();
    delete pExec;
    pExec = NULL;
    std::cout << "result is: " << sResult.c_str() << std::endl;
    // write(STDOUT_FILENO, "\n", 1);
    // exit(0);
}
