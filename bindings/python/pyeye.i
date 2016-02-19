
%module pyeye

%{

// Make sure swig is defined while parseing our wrapper.
#ifndef SWIG
#define SWIG
#endif

#include "EyeLog.h"
%}

%include "std_vector.i"
%include "std_shared_ptr.i"
%include "std_string.i"

%shared_ptr(EyeLogEntry)
%shared_ptr(GazeEntry)
%shared_ptr(FixationEntry)
%shared_ptr(MessageEntry)
%shared_ptr(SaccadeEntry)

// Make sure swig is defined while parseing our wrapper.
#ifndef SWIG
#define SWIG
#endif

%include "constants.h"
%include "EyeLog.h"

namespace std {
    %template (EntryVector) vector<shared_ptr<EyeLogEntry> >;
};
