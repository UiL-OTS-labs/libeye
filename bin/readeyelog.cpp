/*
 * readeyelog.cpp this file is part of libeye and can extracts logfiles
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
#include <cassert>
#include <cstdio>
#include <cstring>
#include "EyeLog.h"

/**
 * the next hack is taken from http://www.zlib.net/zlib_how.html
 */
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

using namespace std;


std::string usage = "%s [options] <intput> [output]\n\
\n\
    arguments are input is mandatory and output is optional\n\
    unless -b is specified, then output is mandatory aswell\n\
\n\
    options are:\n\
        -b      write in binary mode output must be specified.\n\
        -csv    write in csv form\n";

std::string input;
std::string output;

bool write_csv(true);
bool write_binary(false);
bool write_stdout(true);


/**
 * print usage and exit.
 */
void print_usage(string name) {
    fprintf(stderr, usage.c_str(), name.c_str());
    exit(EXIT_FAILURE);
}

void parse_cmd(int argc, char** argv) {
    
    bool has_option(false);

    if (argc < 2 || argc > 4)
        print_usage(argv[0]);
    
    string arg1(argv[1]);
    if (arg1 == "-b") {
        has_option = true;
        write_binary = true;
        write_csv = false;
    }
    else if(arg1 == "-csv") {
        has_option = true;
    }
    
    if (has_option) {
        input = argv[2];
    } else {
        input = argv[1];
    }

    if (has_option && argc == 4) {
        output = argv[3];
        write_stdout = false;
    }
    else if (!has_option && argc == 3) {
        output = argv[2];
        write_stdout = false;
    }
    if (write_stdout && write_binary)
        print_usage(argv[0]);
}

int main(int argc, char **argv) {

    int ret;

    parse_cmd(argc, argv);
    assert (write_csv != write_binary);

    EyeLog log;
    ret = readLog(&log, input);
    if (ret) {
        cerr << "Unable to open log: " << strerror(ret) << endl;
        return EXIT_FAILURE;
    }

    if (write_stdout) {
        // set to binary mode to always write '\n' as line ending
        SET_BINARY_MODE(stdout); 
        auto& entries = log.getEntries();
        for (auto e : entries)
            cout << e->toString() << '\n';
    } else {
        ret = log.open(output);
        if (ret) {
            cerr << "Unable to open " << output << ": "<< strerror(ret) << endl;
            return EXIT_FAILURE;
        }
        if (write_csv)
            ret = log.write(EyeLog::CSV);
        else
            ret = log.write(EyeLog::BINARY);
        
        if (ret) {
            cerr << "unable to write: " << strerror(ret) << endl;
            return EXIT_FAILURE;
        }
    }

    return 0;
}
