
%module pyeye

%{
#include "EyeLog.h"
%}

%include "std_vector.i"
%include "std_shared_ptr.i"
%include "std_string.i"

%shared_ptr (EyeLogEntry);
%shared_ptr (GazeEntry);
%shared_ptr (FixationEntry);
%shared_ptr (MessageEntry);
%shared_ptr (SaccadeEntry);
%shared_ptr (StimulusEntry);

namespace std{
    %template(vector_entry_ptr) vector<EntryPtr>;
}

%include "EyeLog.h"
