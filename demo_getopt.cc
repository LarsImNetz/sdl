/*-*-c++-*-
 *
 *    $RCSfile: demo_getopt.cc,v $
 *     Project:
 * Description:
 *
 *   Author(s): Lars Langhans
 *       $Date: 2005/01/19 12:13:58 $
 *   Copyright: (c)1996 by Lars Langhans
 *   $Revision: 1.2 $
 */

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include "t_print.h"

void usage(const char* sFilename)
{
    t_print("usage:\n %s [OPTIONS] AUDIOFILE\n", sFilename);
    t_print("OPTIONS:\n");
    t_print(" -f  force execution\n");
    t_print(" -n  don't create info file.\n");
}

int main(int argc, char** argv)
{
    const char* sOptString = "fnt:";
    int nOpt = 0;
    bool bForceExec = false;
    bool bCreateInfo = true;

    while ((nOpt = getopt(argc, argv, sOptString)) != -1)
    {
        switch (nOpt)
        {
        case 'f':
            bForceExec = true;
            break;
        case 'n':
            bCreateInfo = false;
            break;
        case 't':
            t_print(T_DEBUG, "option t with %s\n", optarg);
            break;
        case '?':
            break;
        default:
            t_print (T_DEBUG, "?? getopt returned character code 0%o ??\n", nOpt);
        }
    }
    if (optind < argc)
    {
        t_print ("non-option ARGV-elements: ");
        while (optind < argc)
        {
            t_print ("%s ", argv[optind++]);
            fflush(stdout);
        }
        
        t_print ("\n");
    }
    
}
