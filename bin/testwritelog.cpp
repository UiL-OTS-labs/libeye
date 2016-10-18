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
#include "EyeLog.h"

using namespace std;

string usage = "%s <inputfile> <outputfile>\n";

int main(int argc, char **argv) {

    int ret;

    String output;

    if (argc != 3) {
        fprintf(stderr, usage.c_str(), argv[0]);
        return 0;
    }

    output = argv[2];

    PEyeLog log;
    ret = log.read(argv[1]);

    if (ret == 0) {
        ret = log.open(output);
        if (ret) {
            fprintf(stderr, "unable to open %s: %s\n", 
                    output.c_str(),
                    eyelog_error(ret)
                   );
            return EXIT_FAILURE;
        }
        ret = log.write();
        if (ret) {
            fprintf(stderr, "unable to write log: %s\n", 
                    eyelog_error(ret)
                   );
            return EXIT_FAILURE;
        }
    }
    return 0;
}

