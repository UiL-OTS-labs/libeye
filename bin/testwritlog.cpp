
#include <iostream>
#include <cstdio>
#include <cstring>
#include "EyeLog.h"

using namespace std;

string usage = "%s <inputfile> <outputfile>";

int main(int argc, char **argv) {

    int ret;

    string output;

    if (argc != 3) {
        fprintf(stderr, usage.c_str(), argv[0]);
        return 0;
    }

    output= argv[2];

    EyeLog log;
    ret = readLog(&log, argv[1]);

    if (ret == 0) {
        ret = log.open(output);
        if (ret) {
            fprintf(stderr, "unable to open %s: %s\n", 
                    output.c_str(),
                    strerror(ret)
                   );
            return EXIT_FAILURE;
        }
        ret = log.writeBinary();
        if (ret) {
            fprintf(stderr, "unable to write log: %s\n", 
                    strerror(ret)
                   );
            return EXIT_FAILURE;
        }
    }
    return 0;
}
