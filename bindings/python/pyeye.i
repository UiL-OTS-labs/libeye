
%module pyeye

%{

// Make sure swig is defined while parseing our wrapper.
#ifndef SWIG
#define SWIG
#endif

#include "eyelog_export.h"
#include "EyeLog.h"
#include "cError.h"

%}

%include "std_vector.i"
%include "std_shared_ptr.i"
%include "std_string.i"

//%shared_ptr(EyeLogEntry)
//%shared_ptr(GazeEntry)
//%shared_ptr(FixationEntry)
//%shared_ptr(MessageEntry)
//%shared_ptr(SaccadeEntry)

// Make sure swig is defined while parseing our wrapper.
#ifndef SWIG
#define SWIG
#endif


%include "eyelog_export.h"
%include "constants.h"
%include "Shapes.h"
%include "cError.h"
%include "EyeLog.h"

%template (Coordinatei) Coordinate<int>;
%template (Coordinatef) Coordinate<float>;
%template (Recti) Rectangle<int>;
%template (Rectf) Rectangle<float>;

namespace std {
    %template (EntryVector) vector<shared_ptr<EyeLogEntry> >;
};
