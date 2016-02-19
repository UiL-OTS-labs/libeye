/*
 * testwritelog.cpp this file is part of libeye and can extracts logfiles
 *
 * Copyright (C) 2016  Maarten Duijndam
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include "EyeLog.h"

/*
 * Silence MS VC++ about strdup not being part of ISO C++ it will remain valid POSIX C
 */
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  define strdup _strdup
#  pragma warning(push)
#  pragma warning(disable:4996)
#endif
using namespace std;

string usage = "%s <inputfile> <outputfile>\n";

int main(int argc, char **argv) {

    int ret;

    string output;

    if (argc != 3) {
        fprintf(stderr, usage.c_str(), argv[0]);
        return 0;
    }

    output= argv[2];

    EyeLog log;
    ret = log.read(argv[1]);

    if (ret == 0) {
        ret = log.open(output);
        if (ret) {
            fprintf(stderr, "unable to open %s: %s\n", 
                    output.c_str(),
                    strerror(ret)
                   );
            return EXIT_FAILURE;
        }
        ret = log.write();
        if (ret) {
            fprintf(stderr, "unable to write log: %s\n", 
                    strerror(ret)
                   );
            return EXIT_FAILURE;
        }
    }
    return 0;
}

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  pragma warning(pop)
#endif
