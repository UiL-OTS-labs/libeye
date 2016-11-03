#!/usr/bin/env python
'''
This script is a utility to get the current libeye version, and to modify it.
The script reads libeyeVersion.py and modifies it. This program should modify
all files that needs to be changed when the libeye version changes.

This file is a utility in order to maintain and build libeye, it is however
not a source of libeye itself.

libeye tries to live to the rule that a minor version that is stable ends with
an even number.
''' 

import argparse
import re

#handle cmd arguments
ap = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
ap.add_argument('--major', help="bump the major version", action="store_true")
ap.add_argument('--minor', help="bump the minor version", action="store_true")
ap.add_argument('--micro', help="bump the micro version", action="store_true")
ap.add_argument('--set-major',  help="set the major version", type=int, default=-1)
ap.add_argument('--set-minor',  help="set the minor version", type=int, default=-1)
ap.add_argument('--set-micro',  help="set the micro version", type=int, default=-1)

#versions
current_major = -1
current_minor = -1
current_micro = -1

# regular expressions to find the versioning info in the toplevel CMakeLists.txt
re_major = re.compile(r'^set\s*\(LIBEYE_VERSION_MAJOR\s+(\d+)\)\s*$', re.M)
re_minor = re.compile(r'^set\s*\(LIBEYE_VERSION_MINOR\s+(\d+)\)\s*$', re.M)
re_micro = re.compile(r'^set\s*\(LIBEYE_VERSION_MICRO\s+(\d+)\)\s*$', re.M)

# the versioning file
versionfile = 'CMakeLists.txt'
# doxygen configuration file
doxyfile    = 'Doxyfile'

# contents of the versioning file.
version_content = ""

def updateiSpectorPy():
    '''
    Update iSpectorVersion.py
    This file is the file that maintains the final version info
    and is the leading source to what is the actual version of iSpector.
    '''

    content = str(version_content)

    #replacement strings
    majorstr = "set (LIBEYE_VERSION_MAJOR {})".format(current_major)
    minorstr = "set (LIBEYE_VERSION_MINOR {})".format(current_minor)
    microstr = "set (LIBEYE_VERSION_MICRO {})".format(current_micro)
    
    content, n = re_major.subn(majorstr ,content, 1)
    assert(n == 1)
    content, n = re_minor.subn(minorstr, content, 1)
    assert(n == 1)
    content, n = re_micro.subn(microstr, content, 1)
    assert(n == 1)

    f = None
    try:
        f = open(versionfile, 'w')
        f.write(content)
        f.close()
    except :
        exit("unable to open " + versionfile + " for writing" )

def updateDoxyfile():
    '''
    update the doxygen file
    '''

    global doxyfile
    f = open(doxyfile, 'r')

    content = f.read()

    #the line to be mached
    versionline = r"^PROJECT_NUMBER\s*=.*$"
    re_version = re.compile(versionline, re.M)

    #replacement string
    replstr = "PROJECT_NUMBER         = {}.{}.{}".format(
                                                    current_major,
                                                    current_minor,
                                                    current_micro
                                                    )
    print replstr
    
    content, n = re_version.subn(replstr, content, 1)
    print n
    assert(n == 1)

    f = None
    try:
        f = open(doxyfile, 'w')
        f.write(content)
        f.close()
    except :
        exit("unable to open " + versionfile + " for writing" )


def updateVersioningFiles():
    ''' Update all version files. '''
    updateiSpectorPy()
    updateDoxyfile()

def getVersion():
    '''
    returns a string with libeye to which the current version is concatenated.
    '''
    return "libeye-" + str(current_major) + "." + str(current_minor) +\
           "." + str(current_micro)

def getVersionInfo():
    '''
    this function read CMakeLists.txt to obtain the current version
    info.
    '''
    
    global current_major
    global current_minor
    global current_micro
    global version_content

    f = None
    try :
        f = open(versionfile,  'r')
    except IOError:
        exit ("Unable to open " + versionfile)

    version_content = f.read();
    if not version_content:
        exit(versionfile + "is empty.")

    mmajor = re_major.search(version_content)
    mminor = re_minor.search(version_content)
    mmicro = re_micro.search(version_content)

    if mmajor and mminor and mmicro :
        current_major = int(mmajor.group(1))
        current_minor = int(mminor.group(1))
        current_micro = int(mmicro.group(1))
    else:
        exit(versionfile + " is in an unexpected format.")

def bumpMicroVersion():
    '''
    bump microversion
    '''
    global current_micro
    current_micro +=1

def bumpMinorVersion():
    '''
    bump minorversion and set micro version to 0 
    '''
    global current_minor
    global current_micro
    current_micro = 0
    current_minor += 1

def bumpMajorVersion():
    '''
    bump minorversion and set micro and minor version to 0 
    '''
    global current_minor
    global current_micro
    global current_major
    current_micro = 0
    current_minor = 0
    current_major += 1

if __name__ == "__main__":

    ns = ap.parse_args()
    getVersionInfo()
    print "Current version: " + getVersion()

    bump    = ns.major or ns.minor or ns.micro
    setting = ns.set_major >= 0 or ns.set_minor >= 0 or ns.set_micro >= 0

    # bump specific versions 
    if ns.micro:
        bumpMicroVersion()
    if ns.minor:
        bumpMinorVersion()
    if ns.major:
        bumpMajorVersion()

    # set versions explicitly
    if ns.set_major >= 0:
        current_major = ns.set_major
    if ns.set_minor >= 0:
        current_minor = ns.set_minor
    if ns.set_micro >= 0:
        current_micro = ns.set_micro

    # write to output
    if bump or setting:
        updateVersioningFiles();
        print "New version: " + getVersion()
