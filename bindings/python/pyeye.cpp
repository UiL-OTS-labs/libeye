

#include <Python.h>

#include <log/EyeLog.h>

#ifdef __cplusplus
extern "C" {
#endif

const char* module_name = "pyeye";
const char* module_doc  = "The pyeye module is a pythonic wrapper around libeye";

/***** Module objects *****/

static PyObject*    PyEyeError = NULL;

/* constants */
//GazeEntries
static PyObject*    PyEyeLGaze      = NULL;
static PyObject*    PyEyeRGaze      = NULL;
static PyObject*    PyEyeLFix       = NULL;
static PyObject*    PyEyeRFix       = NULL;
static PyObject*    PyEyeStim       = NULL;
static PyObject*    PyEyeMessage    = NULL;
static PyObject*    PyEyeLSac       = NULL;
static PyObject*    PyEyeRSac       = NULL;
static PyObject*    PyEyeAvgGaze    = NULL;
static PyObject*    PyEyeAvgFix     = NULL;
static PyObject*    PyEyeAvgSac     = NULL;
static PyObject*    PyEyeTrial      = NULL;
static PyObject*    PyEyeTrialStart = NULL;
static PyObject*    PyEyeTrialEnd   = NULL;

//formats for logging
static PyObject*    PyEyeFormatBinary   = NULL;
static PyObject*    PyEyeFormatCsv      = NULL;

/**
 * adds the constants to the module
 */
void pyeye_module_add_constants(PyObject* module) {

    assert(module);

    PyEyeLGaze      = PyInt_FromLong(LGAZE);
    PyEyeRGaze      = PyInt_FromLong(RGAZE);
    PyEyeLFix       = PyInt_FromLong(LFIX);
    PyEyeRFix       = PyInt_FromLong(RFIX);
    PyEyeStim       = PyInt_FromLong(STIMULUS);
    PyEyeMessage    = PyInt_FromLong(MESSAGE);
    PyEyeLSac       = PyInt_FromLong(LSAC);
    PyEyeRSac       = PyInt_FromLong(RSAC);
    PyEyeAvgGaze    = PyInt_FromLong(AVGGAZE);
    PyEyeAvgFix     = PyInt_FromLong(AVGFIX);
    PyEyeAvgSac     = PyInt_FromLong(AVGSAC);
    PyEyeTrial      = PyInt_FromLong(TRIAL);
    PyEyeTrialStart = PyInt_FromLong(TRIALSTART);
    PyEyeTrialEnd   = PyInt_FromLong(TRIALEND);

    PyEyeFormatBinary = PyInt_FromLong(FORMAT_BINARY);
    PyEyeFormatCsv    = PyInt_FromLong(FORMAT_CSV);
    
    PyModule_AddObject(module, "LGAZE"      , PyEyeLGaze);
    PyModule_AddObject(module, "RGAZE"      , PyEyeRGaze);
    PyModule_AddObject(module, "LFIX"       , PyEyeLFix);
    PyModule_AddObject(module, "RFIX"       , PyEyeRFix);
    PyModule_AddObject(module, "STIMULUS"   , PyEyeStim);
    PyModule_AddObject(module, "MESSAGE"    , PyEyeMessage);
    PyModule_AddObject(module, "LSAC"       , PyEyeLSac);
    PyModule_AddObject(module, "RSAC"       , PyEyeRSac);
    PyModule_AddObject(module, "AVGGAZE"    , PyEyeAvgGaze);
    PyModule_AddObject(module, "AVGFIX"     , PyEyeAvgFix);
    PyModule_AddObject(module, "AVGSAC"     , PyEyeAvgSac);
    PyModule_AddObject(module, "TRIAL"      , PyEyeTrial);
    PyModule_AddObject(module, "TRIALSTART" , PyEyeTrialStart);
    PyModule_AddObject(module, "TRIALEND"   , PyEyeTrialEnd);

    PyModule_AddObject(module, "FORMAT_BINARY"  , PyEyeFormatBinary);
    PyModule_AddObject(module, "FORMAT_CSV"     , PyEyeFormatCsv);
}

/***** EyeLogEntry *****/

typedef struct {
    PyObject_HEAD
    PEntryPtr m_private;
} EyeLogEntry;

static void
EyeLogEntry_dealloc(EyeLogEntry* self)
{
    delete self->m_private;
    self->ob_type->tp_free((PyObject*) self);
}

static PyObject*
EyeLogEntry_new(PyTypeObject *type, PyObject* args, PyObject* kwds)
{
    EyeLogEntry* ret = NULL;
    ret = (EyeLogEntry*) type->tp_alloc(type, 0);

    if (ret)
        ret->m_private = NULL;
    else
        PyErr_NoMemory();

    return (PyObject*)ret;
}

static int
EyeLogEntry_init(EyeLogEntry* self, PyObject* args, PyObject* kwds)
{
    PyErr_SetString(PyExc_RuntimeError,
                    "Init of abstract class EyeLogEntry called."
                    );
    return -1;
}

static PyObject* EyeLogEntry_clone(EyeLogEntry* self, PyObject* args)
{
//    PyErr_SetString(PyExc_NotImplementedError,
//            "Bug: This must be implemented in child."
//            );
    EyeLogEntry* clone = NULL;
    clone = (EyeLogEntry*) EyeLogEntry_new(self->ob_type, NULL, NULL);
    if (!clone)
        return NULL;
    
    clone->m_private = self->m_private->clone();
    Py_INCREF(clone);

    return (PyObject*)clone;
}

static PyObject* EyeLogEntry_toString(EyeLogEntry* self, PyObject* args)
{
    return PyString_FromString(self->m_private->toString().c_str());
}

static PyObject* EyeLogEntry_writeBinary(PyObject* self, PyObject* args)
{
    PyErr_SetString(PyExc_NotImplementedError,
            "This function is as of yet not implemented, file a feature request."
            );
    return NULL;
}

static PyObject* EyeLogEntry_compare(EyeLogEntry* self, PyObject* args)
{
    PyObject* other = NULL;
    EyeLogEntry* o =NULL; 

    if(!PyArg_ParseTuple(args, "O", &other))
        return NULL;
    o = (EyeLogEntry*) other;

    if(!PyObject_IsInstance(other, (PyObject*)&self->ob_type))
        return NULL;

    int comp = self->m_private->compare(*o->m_private);
    if (comp) // python demands for -1, 0, or 1
        comp = comp < 0 ? -1 : 1;

    return PyInt_FromLong(comp);
}

static PyObject* EyeLogEntry_getSeparator(PyObject* self, PyObject* args)
{
    assert(self == NULL); // this is a class/static function.
    if (!PyArg_ParseTuple(args, ""))
        return NULL;

    String sep = PEyeLogEntry::getSeparator();

    return PyString_FromString(sep.c_str());
}

static PyObject* EyeLogEntry_setSeparator(PyObject* self, PyObject* args)
{
    assert(self == NULL);

    const char* str = NULL;
    if (!PyArg_ParseTuple(args, "s", &str))
        return NULL;

    PEyeLogEntry::setSeparator(str);
    
    Py_RETURN_NONE;
}

static PyObject* EyeLogEntry_getPrecision(PyObject* self, PyObject* args)
{
    assert(self == NULL);

    long prec = (long) PEyeLogEntry::getPrecision();

    return PyInt_FromLong(prec);
}

static PyObject* EyeLogEntry_setPrecision(PyObject* self, PyObject* args)
{
    assert(self == NULL);
    int prec;

    if(!PyArg_ParseTuple(args, "i", &prec))
        return NULL;
    
    if (prec < 0 || prec > 8) {
        PyErr_SetString(PyExc_ValueError,
                        "a valid range for precision is 0 >= precision <= 8.");
        return NULL;
    }

    PEyeLogEntry::setPrecision((unsigned) prec);
    Py_RETURN_NONE;
}

static PyObject* EyeLogEntry_getEntryType(EyeLogEntry* self, PyObject* args)
{
    return PyInt_FromLong(self->m_private->getEntryType());
}

static PyObject* EyeLogEntry_getTime(EyeLogEntry* self, PyObject* args)
{
    PEntryPtr ep = self->m_private;

    double time = ep->getTime();

    return PyFloat_FromDouble(time);
}

static PyMethodDef EyeLogEntry_methods[] = {
    {"clone", (PyCFunction) EyeLogEntry_clone, METH_NOARGS,
        "Make a Deepcopy from self."},
    {"toString", (PyCFunction) EyeLogEntry_toString, METH_NOARGS,
        "Get a string representation of self."},
    // figure out how to implement this version of the log.
    {"writeBinary", (PyCFunction) EyeLogEntry_writeBinary, METH_VARARGS,
        "Write this LogEntry in a binary representation."},
    {"compare", (PyCFunction)EyeLogEntry_compare, METH_VARARGS,
        "Compare this entry to another, usefull to sort these entries on time"},
    {"setSeparator", (PyCFunction)EyeLogEntry_setSeparator, METH_CLASS|METH_VARARGS,
        "Set the separator between field in a string format for all instances of eyelogentry"},
    {"getSeparator", (PyCFunction)EyeLogEntry_getSeparator, METH_CLASS|METH_NOARGS,
        "Get the separator between field in a string format for all instances of eyelogentry"},
    {"setPrecision", (PyCFunction)EyeLogEntry_setPrecision, METH_CLASS|METH_VARARGS,
        "Set the precision between field in a string format for all instances of eyelogentry"},
    {"getPrecision", (PyCFunction)EyeLogEntry_getPrecision, METH_CLASS|METH_NOARGS,
        "Get the precision between field in a string format for all instances of eyelogentry"},
    {"getEntryType", (PyCFunction)EyeLogEntry_getEntryType, METH_NOARGS,
        "Get the entry type of self, that determines what type of entry it is."},
    {"getTime", (PyCFunction)EyeLogEntry_getTime, METH_NOARGS,
        "Obtain the timestamp of this logevent."},
    {NULL} /*sentinal*/
};

static PyTypeObject EyeLogEntryType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/   // for binary compatibility
    "pyeye.EyeLogEntry",        /*tp_name*/
    sizeof(EyeLogEntry),        /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor) EyeLogEntry_dealloc,   /*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash */
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,    /*tp_flags*/
    "The base class for all log entries",
                                /*tp_doc*/
    0,		                    /*tp_traverse */
    0,		                    /*tp_clear */
    0,		                    /*tp_richcompare */
    0,		                    /*tp_weaklistoffset */
    0,		                    /*tp_iter */
    0,		                    /*tp_iternext */
    EyeLogEntry_methods,        /*tp_methods */
    0,//EyeLogEntry_members,        /*tp_members */
    0,                          /*tp_getset */
    0,                          /*tp_base */
    0,                          /*tp_dict */
    0,                          /*tp_descr_get */
    0,                          /*tp_descr_set */
    0,                          /*tp_dictoffset */
    (initproc)EyeLogEntry_init, /*tp_init */
    0,                          /*tp_alloc */
    EyeLogEntry_new,            /*tp_new */
};

/***** GazeEntry *****/

typedef struct {
    EyeLogEntry m_parent;
} GazeEntry;

static int
GazeEntry_init(GazeEntry* self, PyObject* args, PyObject* kwds)
{
    PGazeEntry* gaze = NULL;
    double time;
    float x, y, pupil;
    entrytype entry;

    static const char* kwlist[] = {
        "entrytype",
        "time",
        "x",
        "y",
        "pupil",
        NULL
    };

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "idfff",
                const_cast<char**>(kwlist),
                &entry,
                &time,
                &x,
                &y,
                &pupil
                )
      )
        return -1;

    try {
        gaze = new PGazeEntry(entry, time, x, y, pupil);
    } catch(std::bad_alloc& e) {
        PyErr_NoMemory();
        return -1;
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "GazeEntry_init: unknown error");
        return -1;
    }

    self->m_parent.m_private  = static_cast<PEntryPtr>(gaze);
    return 0;
}

static PyObject*
GazeEntry_getX(GazeEntry* self) {
    PGazeEntry* g = static_cast<PGazeEntry*>(self->m_parent.m_private);
    return PyFloat_FromDouble(g->getX());
}

static PyObject*
GazeEntry_getY(GazeEntry* self) {
    PGazeEntry* g = static_cast<PGazeEntry*>(self->m_parent.m_private);
    return PyFloat_FromDouble(g->getY());
}

static PyObject*
GazeEntry_setX(GazeEntry* self, PyObject* args) {
    float x;
    if (!PyArg_ParseTuple(args, "f", &x))
        return NULL;

    PGazeEntry* g = static_cast<PGazeEntry*>(self->m_parent.m_private);
    g->setX(x);

    Py_RETURN_NONE;
}

static PyObject*
GazeEntry_setY(GazeEntry* self, PyObject* args) {
    float y;
    if (!PyArg_ParseTuple(args, "f", &y))
        return NULL;

    PGazeEntry* g = static_cast<PGazeEntry*>(self->m_parent.m_private);
    g->setY(y);

    Py_RETURN_NONE;
}

static PyObject*
GazeEntry_setPupil(GazeEntry* self, PyObject* args) {
    float p;
    if (!PyArg_ParseTuple(args, "f", &p))
        return NULL;

    PGazeEntry* g = static_cast<PGazeEntry*>(self->m_parent.m_private);
    g->setPupil(p);

    Py_RETURN_NONE;
}

static PyObject*
GazeEntry_getPupil(GazeEntry* self) {
    PGazeEntry* g = static_cast<PGazeEntry*>(self->m_parent.m_private);
    return PyFloat_FromDouble(g->getPupil());
}

static PyObject*
GazeEntry_getCoordinate(GazeEntry* self, PyObject* args) {
    PyErr_SetString(PyExc_NotImplementedError,
            "getCoordinate must be implemented.");
    return NULL;
}

static PyObject*
GazeEntry_setCoordinate(GazeEntry* self, PyObject* args) {
    PyErr_SetString(PyExc_NotImplementedError,
            "setCoordinate must be implemented.");
    return NULL;
}

static PyMethodDef GazeEntry_methods[] = {
    {"getX", (PyCFunction) GazeEntry_getX, METH_NOARGS,
        "Returns the x coordinate of the gaze."},
    {"getY", (PyCFunction) GazeEntry_getY, METH_NOARGS,
        "Returns the y coordinate of the gaze."},
    {"setX", (PyCFunction) GazeEntry_setX, METH_VARARGS,
        "Sets the x coordinate of the gaze."},
    {"setY", (PyCFunction) GazeEntry_setY, METH_VARARGS,
        "Sets the y coordinate of the gaze."},
    {"getPupil", (PyCFunction) GazeEntry_getPupil, METH_NOARGS,
        "Get the pupil size of the gaze."},
    {"setPupil", (PyCFunction) GazeEntry_setPupil, METH_VARARGS,
        "Set the pupil size of the gaze."},
    {"getCoordinate", (PyCFunction) GazeEntry_getCoordinate, METH_NOARGS,
        "Get the x, y coordinate of the gaze."},
    {"setCoordinate", (PyCFunction) GazeEntry_setCoordinate, METH_VARARGS,
        "Set the x, y cooridnate of the gaze."},
    {NULL}
};

static PyTypeObject GazeEntryType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/   // for binary compatibility
    "pyeye.GazeEntry",          /*tp_name*/
    sizeof(GazeEntry),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    0,                          /*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash */
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,         /*tp_flags*/
    "A GazeEntry represents the gaze in an EyeLog.",
                                /*tp_doc*/
    0,		                    /*tp_traverse */
    0,		                    /*tp_clear */
    0,		                    /*tp_richcompare */
    0,		                    /*tp_weaklistoffset */
    0,		                    /*tp_iter */
    0,		                    /*tp_iternext */
    GazeEntry_methods,          /*tp_methods */
    0,                          /*tp_members */
    0,                          /*tp_getset */
    0,                          /*tp_base */
    0,                          /*tp_dict */
    0,                          /*tp_descr_get */
    0,                          /*tp_descr_set */
    0,                          /*tp_dictoffset */
    (initproc)GazeEntry_init,   /*tp_init */
    0,
    0,
};

/***** FixationEntry *****/

typedef struct {
    EyeLogEntry m_parent;
} FixationEntry;

static int
FixationEntry_init(FixationEntry* self, PyObject* args, PyObject* kwds)
{
    PFixationEntry* gaze = NULL;
    double time, duration;
    float x, y;
    entrytype entry;

    static const char* kwlist[] = {
        "entrytype",
        "time",
        "duration",
        "x",
        "y",
        NULL
    };

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "iddff",
                const_cast<char**>(kwlist),
                &entry,
                &time,
                &duration,
                &x,
                &y
                )
      )
        return -1;

    if (entry != LFIX && entry != RFIX && entry != AVGFIX) {
        PyErr_SetString(PyExc_ValueError,
                "entrytype must be LFIX, RFIX or AVGFIX"
                );
        return -1;
    }
    
    if (duration < 0) {
        PyErr_SetString(PyExc_ValueError,
                "duration must be positive (or 0)."
                );
        return -1;
    }

    try {
        gaze = new PFixationEntry(entry, time, duration, x, y);
    } catch(std::bad_alloc& e) {
        PyErr_NoMemory();
        return -1;
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "FixationEntry_init: unknown error");
        return -1;
    }

    self->m_parent.m_private  = static_cast<PEntryPtr>(gaze);
    return 0;
}

static PyObject*
FixationEntry_getX(FixationEntry* self) {
    PFixationEntry* g = static_cast<PFixationEntry*>(self->m_parent.m_private);
    return PyFloat_FromDouble(g->getX());
}

static PyObject*
FixationEntry_getY(FixationEntry* self) {
    PFixationEntry* g = static_cast<PFixationEntry*>(self->m_parent.m_private);
    return PyFloat_FromDouble(g->getY());
}

static PyObject*
FixationEntry_setX(FixationEntry* self, PyObject* args) {
    float x;
    if (!PyArg_ParseTuple(args, "f", &x))
        return NULL;

    PFixationEntry* g = static_cast<PFixationEntry*>(self->m_parent.m_private);
    g->setX(x);

    Py_RETURN_NONE;
}

static PyObject*
FixationEntry_setY(FixationEntry* self, PyObject* args) {
    float y;
    if (!PyArg_ParseTuple(args, "f", &y))
        return NULL;

    PFixationEntry* g = static_cast<PFixationEntry*>(self->m_parent.m_private);
    g->setY(y);

    Py_RETURN_NONE;
}

static PyObject*
FixationEntry_setPupil(FixationEntry* self, PyObject* args) {
    float p;
    if (!PyArg_ParseTuple(args, "f", &p))
        return NULL;

    PFixationEntry* g = static_cast<PFixationEntry*>(self->m_parent.m_private);
    g->setPupil(p);

    Py_RETURN_NONE;
}

static PyObject*
FixationEntry_getPupil(FixationEntry* self) {
    PFixationEntry* g = static_cast<PFixationEntry*>(self->m_parent.m_private);
    return PyFloat_FromDouble(g->getPupil());
}

static PyObject*
FixationEntry_getCoordinate(FixationEntry* self, PyObject* args) {
    PyErr_SetString(PyExc_NotImplementedError,
            "getCoordinate must be implemented.");
    return NULL;
}

static PyObject*
FixationEntry_setCoordinate(FixationEntry* self, PyObject* args) {
    PyErr_SetString(PyExc_NotImplementedError,
            "setCoordinate must be implemented.");
    return NULL;
}

static PyMethodDef FixationEntry_methods[] = {
    {"getX", (PyCFunction) FixationEntry_getX, METH_NOARGS,
        "Returns the x coordinate of the gaze."},
    {"getY", (PyCFunction) FixationEntry_getY, METH_NOARGS,
        "Returns the y coordinate of the gaze."},
    {"setX", (PyCFunction) FixationEntry_setX, METH_VARARGS,
        "Sets the x coordinate of the gaze."},
    {"setY", (PyCFunction) FixationEntry_setY, METH_VARARGS,
        "Sets the y coordinate of the gaze."},
    {"getCoordinate", (PyCFunction) FixationEntry_getCoordinate, METH_NOARGS,
        "Get the x, y coordinate of the gaze."},
    {"setCoordinate", (PyCFunction) FixationEntry_setCoordinate, METH_VARARGS,
        "Set the x, y cooridnate of the gaze."},
    {"getDuration", (PyCFunction) FixationEntry_getDuration, METH_NOARGS,
        "Returns the y coordinate of the gaze."},
    {"setDuration", (PyCFunction) FixationEntry_setDuration, METH_VARARGS,
        "Sets the x coordinate of the gaze."},
    {NULL}
};

static PyTypeObject FixationEntryType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/   // for binary compatibility
    "pyeye.FixationEntry",          /*tp_name*/
    sizeof(FixationEntry),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    0,                          /*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash */
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,         /*tp_flags*/
    "A FixationEntry represents the gaze in an EyeLog.",
                                /*tp_doc*/
    0,		                    /*tp_traverse */
    0,		                    /*tp_clear */
    0,		                    /*tp_richcompare */
    0,		                    /*tp_weaklistoffset */
    0,		                    /*tp_iter */
    0,		                    /*tp_iternext */
    FixationEntry_methods,          /*tp_methods */
    0,                          /*tp_members */
    0,                          /*tp_getset */
    0,                          /*tp_base */
    0,                          /*tp_dict */
    0,                          /*tp_descr_get */
    0,                          /*tp_descr_set */
    0,                          /*tp_dictoffset */
    (initproc)FixationEntry_init,   /*tp_init */
    0,
    0,
};



/***** Module functions *****/

static PyMethodDef PyEyeMethods[] = {
    //{"system",  spam_system, METH_VARARGS, "Execute a shell command."},
    {NULL}        /* Sentinel */
};

/***** Module initialization *****/


PyMODINIT_FUNC
initpyeye(void)
{
    PyObject *m;

    if (PyType_Ready(&EyeLogEntryType) < 0)
        return;
    
    GazeEntryType.tp_base = &EyeLogEntryType;
    if (PyType_Ready(&GazeEntryType) < 0)
        return;

    m = Py_InitModule3(module_name, PyEyeMethods, module_doc);
    if(m == NULL)
        return;

    Py_INCREF(&EyeLogEntryType);
    PyModule_AddObject(m, "EyeLogEntry", (PyObject*) &EyeLogEntryType);
    
    Py_INCREF(&GazeEntryType);
    PyModule_AddObject(m, "GazeEntry", (PyObject*) &GazeEntryType);

    pyeye_module_add_constants(m);

//    PyEyeError = PyErr_NewException("pyeye.PyEyeError", NULL, NULL);
//    Py_INCREF(&PyEyeError);
//
//    PyModule_AddObject(m, "PyError", PyEyeError);
}

#ifdef __cplusplus
}
#endif
