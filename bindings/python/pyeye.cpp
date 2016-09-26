

#include <Python.h>

#include <log/EyeLog.h>
//#include "pyshapes.h"

// python takes many strings as char* therefore we turn -Wwrite-strings off.
#pragma GCC diagnostic ignored "-Wwrite-strings"


#ifdef __cplusplus
extern "C" {
#endif

const char* module_name = "pyeye";
const char* module_doc  = "The pyeye module is a pythonic wrapper around libeye";

/***** Forward declarations *****/

struct _EyeLogEntry;

static PyObject* EyeLogEntry_compare(struct _EyeLogEntry* self, PyObject* args);
static int EyeLogEntry_Compare(struct _EyeLogEntry* self, PyObject* rhs);

/***** utilities *****/


/**
 * pretty much all eye log entries use self->m_parent.m_private to store the
 * C++ PEyeLogEntry this makes it a bit more conveniant to obtain it.
 */
#define PRIV_POINTER self->m_parent.m_private

const PyTypeObject* getEyeLogEntryClass();


/***** Module objects *****/

static PyObject*    PyEyeError = NULL;

static _EyeLogEntry* TheEyeLogEntry  = NULL;

/* constants */
//GazeEntries
static PyObject*    PyEyeLGAZE      = NULL;
static PyObject*    PyEyeRGAZE      = NULL;
static PyObject*    PyEyeLFIX       = NULL;
static PyObject*    PyEyeRFIX       = NULL;
static PyObject*    PyEyeSTIM       = NULL;
static PyObject*    PyEyeMESSAGE    = NULL;
static PyObject*    PyEyeLSAC       = NULL;
static PyObject*    PyEyeRSAC       = NULL;
static PyObject*    PyEyeAVGGAZE    = NULL;
static PyObject*    PyEyeAVGFIX     = NULL;
static PyObject*    PyEyeAVGSAC     = NULL;
static PyObject*    PyEyeTRIAL      = NULL;
static PyObject*    PyEyeTRIALSTART = NULL;
static PyObject*    PyEyeTRIALEND   = NULL;

//formats for logging
static PyObject*    PyEyeFORMATBINARY   = NULL;
static PyObject*    PyEyeFORMATCSV      = NULL;

/**
 * adds the constants to the module
 */
void pyeye_module_add_constants(PyObject* module) {

    assert(module);

    PyEyeLGAZE      = PyInt_FromLong(LGAZE);
    PyEyeRGAZE      = PyInt_FromLong(RGAZE);
    PyEyeLFIX       = PyInt_FromLong(LFIX);
    PyEyeRFIX       = PyInt_FromLong(RFIX);
    PyEyeSTIM       = PyInt_FromLong(STIMULUS);
    PyEyeMESSAGE    = PyInt_FromLong(MESSAGE);
    PyEyeLSAC       = PyInt_FromLong(LSAC);
    PyEyeRSAC       = PyInt_FromLong(RSAC);
    PyEyeAVGGAZE    = PyInt_FromLong(AVGGAZE);
    PyEyeAVGFIX     = PyInt_FromLong(AVGFIX);
    PyEyeAVGSAC     = PyInt_FromLong(AVGSAC);
    PyEyeTRIAL      = PyInt_FromLong(TRIAL);
    PyEyeTRIALSTART = PyInt_FromLong(TRIALSTART);
    PyEyeTRIALEND   = PyInt_FromLong(TRIALEND);

    PyEyeFORMATBINARY = PyInt_FromLong(FORMAT_BINARY);
    PyEyeFORMATCSV    = PyInt_FromLong(FORMAT_CSV);
    
    PyModule_AddObject(module, "LGAZE"      , PyEyeLGAZE);
    PyModule_AddObject(module, "RGAZE"      , PyEyeRGAZE);
    PyModule_AddObject(module, "LFIX"       , PyEyeLFIX);
    PyModule_AddObject(module, "RFIX"       , PyEyeRFIX);
    PyModule_AddObject(module, "STIMULUS"   , PyEyeSTIM);
    PyModule_AddObject(module, "MESSAGE"    , PyEyeMESSAGE);
    PyModule_AddObject(module, "LSAC"       , PyEyeLSAC);
    PyModule_AddObject(module, "RSAC"       , PyEyeRSAC);
    PyModule_AddObject(module, "AVGGAZE"    , PyEyeAVGGAZE);
    PyModule_AddObject(module, "AVGFIX"     , PyEyeAVGFIX);
    PyModule_AddObject(module, "AVGSAC"     , PyEyeAVGSAC);
    PyModule_AddObject(module, "TRIAL"      , PyEyeTRIAL);
    PyModule_AddObject(module, "TRIALSTART" , PyEyeTRIALSTART);
    PyModule_AddObject(module, "TRIALEND"   , PyEyeTRIALEND);

    PyModule_AddObject(module, "FORMAT_BINARY"  , PyEyeFORMATBINARY);
    PyModule_AddObject(module, "FORMAT_CSV"     , PyEyeFORMATCSV);
}

/***** EyeLogEntry *****/

typedef struct _EyeLogEntry{
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


static PyObject*
EyeLogEntry_str(EyeLogEntry* self)
{
    return PyString_FromString(self->m_private->toString().c_str());
}

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
    (cmpfunc)EyeLogEntry_Compare,        /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash */
    0,                          /*tp_call*/
    EyeLogEntry_str,            /*tp_str*/
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

/*
 * This compare is to implement the comparison operators, the other
 * just compares like the C++ function does.
 */
static int
EyeLogEntry_Compare(EyeLogEntry* lhs, PyObject* rhs)
{
    if(! PyObject_IsInstance(rhs, (PyObject*) &EyeLogEntryType)) {
        PyErr_SetString(PyExc_TypeError,
                "Comparing EyeLogEntry with another object type"
                );
        return -2;
    }

    EyeLogEntry* righths = (EyeLogEntry*) rhs;

    int ret = static_cast<PEyeLogEntry*>(lhs->m_private)->compare(
            *righths->m_private
        );
    
    if (ret) ret = ret < 0 ? -1 : 1;

    return ret;
}

static PyObject* EyeLogEntry_compare(EyeLogEntry* self, PyObject* args)
{
    PyObject* other = NULL;
    EyeLogEntry* o  = NULL; 
    PEyeLogEntry* lhs=NULL, *rhs = NULL;
    int comp;

    if(!PyArg_ParseTuple(args, "O!", &EyeLogEntryType, &other))//, &EyeLogEntryType))
        return NULL;
    
//    if (!PyObject_IsInstance(other, (PyObject*)&EyeLogEntryType)) {
//        PyErr_SetString(PyExc_TypeError,
//                "Can only compare with another instance (derived) of EyeLogEntry.");
//        return NULL;
//    }

    o = (EyeLogEntry*) other;

    lhs = static_cast<PEyeLogEntry*>(self->m_private);
    rhs = static_cast<PEyeLogEntry*>(o->m_private);

    comp = lhs->compare(*rhs);

    return PyInt_FromLong(comp);
}

//static const PyObject*
//getEyeLogEntryClass()
//{
//    return &EyeLogEntryType;
//}

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

    if (entry != LGAZE && entry != RGAZE && entry != AVGGAZE) {
        PyErr_SetString(PyExc_ValueError, "entry must be LGAZE, RGAZE or AVGGAZE");
        return -1;
    }

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
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,         /*tp_flags*/
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
    PFixationEntry* f = static_cast<PFixationEntry*>(self->m_parent.m_private);
    return PyFloat_FromDouble(f->getX());
}

static PyObject*
FixationEntry_getY(FixationEntry* self) {
    PFixationEntry* f = static_cast<PFixationEntry*>(self->m_parent.m_private);
    return PyFloat_FromDouble(f->getY());
}

static PyObject*
FixationEntry_setX(FixationEntry* self, PyObject* args) {
    float x;
    if (!PyArg_ParseTuple(args, "f", &x))
        return NULL;

    PFixationEntry* f = static_cast<PFixationEntry*>(self->m_parent.m_private);
    f->setX(x);

    Py_RETURN_NONE;
}

static PyObject*
FixationEntry_setY(FixationEntry* self, PyObject* args) {
    float y;
    if (!PyArg_ParseTuple(args, "f", &y))
        return NULL;

    PFixationEntry* f = static_cast<PFixationEntry*>(self->m_parent.m_private);
    f->setY(y);

    Py_RETURN_NONE;
}

static PyObject*
FixationEntry_setDuration(FixationEntry* self, PyObject* args) {
    double dur;
    if (!PyArg_ParseTuple(args, "d", &dur))
        return NULL;

    PFixationEntry* f = static_cast<PFixationEntry*>(self->m_parent.m_private);
    f->setDuration(dur);

    Py_RETURN_NONE;
}

static PyObject*
FixationEntry_getDuration(FixationEntry* self) {
    PFixationEntry* f = static_cast<PFixationEntry*>(self->m_parent.m_private);
    return PyFloat_FromDouble(f->getDuration());
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

static PyObject*
FixationEntry_XGetter(FixationEntry* self, void*)
{
    return FixationEntry_getX(self);
}

static int
FixationEntry_XSetter(FixationEntry* self, PyObject* value,  void*)
{
    float x;

    if (!PyFloat_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "x must be float");
        return -1;
    }
    x = PyFloat_AsDouble(value);

    static_cast<PFixationEntry*>(self->m_parent.m_private)->setX(x);
    
    return 0;
}

static PyObject*
FixationEntry_YGetter(FixationEntry* self, void*)
{
    return FixationEntry_getY(self);
}

static int
FixationEntry_YSetter(FixationEntry* self, PyObject* value,  void*)
{
    float y;

    if (!PyFloat_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "y must be float");
        return -1;
    }
    y = PyFloat_AsDouble(value);


    static_cast<PFixationEntry*>(self->m_parent.m_private)->setY(y);
    return 0;
}

static PyObject*
FixationEntry_DurationGetter(FixationEntry* self, void*)
{
    return FixationEntry_getDuration(self);
}

static int
FixationEntry_DurationSetter(FixationEntry* self, PyObject* value,  void*)
{
    double dur;

    if (!PyFloat_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "Duration must be float");
        return -1;
    }
    
    dur = PyFloat_AsDouble(value);

    static_cast<PFixationEntry*>(self->m_parent.m_private)->setDuration(dur);
    return 0;
}

static PyObject*
FixationEntry_CoorGetter(FixationEntry* self, void*) 
{
    PFixationEntry* fix = static_cast<PFixationEntry*>(
            self->m_parent.m_private
            );
    float x = fix->getX();
    float y = fix->getY();
    PyObject* result = Py_BuildValue("(ff)", x, y);
    return result;
}

static int
FixationEntry_CoorSetter(FixationEntry* self, PyObject* coor, void*)
{
    float x,y;
    if (!PyArg_ParseTuple(coor, "(ff)", &x, &y)) {
        return -1;
    }
    PCoordinate c(x,y);
    static_cast<PFixationEntry*>(self->m_parent.m_private)->setCoordinate(c);
    return 0;
}

static PyGetSetDef FixationEntry_getset[] {
    {"x", (getter)FixationEntry_XGetter, (setter)FixationEntry_XSetter,
        "The x coordinate.", NULL},
    {"y", (getter)FixationEntry_YGetter, (setter)FixationEntry_YSetter,
        "The y coordinate.", NULL},
    {"duration", (getter) FixationEntry_DurationGetter,
        (setter)FixationEntry_DurationSetter,
        "Duration of the fixation.", NULL},
    {"coordinate", (getter)FixationEntry_CoorGetter,
        (setter)FixationEntry_CoorSetter,
        "The x,y coordinate.", NULL},
    { NULL }
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
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,         /*tp_flags*/
    "A FixationEntry represents the a fixation in an EyeLog.",
                                /*tp_doc*/
    0,		                    /*tp_traverse */
    0,		                    /*tp_clear */
    0,		                    /*tp_richcompare */
    0,		                    /*tp_weaklistoffset */
    0,		                    /*tp_iter */
    0,		                    /*tp_iternext */
    FixationEntry_methods,      /*tp_methods */
    0,                          /*tp_members */
    FixationEntry_getset,       /*tp_getset */
    0,                          /*tp_base */
    0,                          /*tp_dict */
    0,                          /*tp_descr_get */
    0,                          /*tp_descr_set */
    0,                          /*tp_dictoffset */
    (initproc)FixationEntry_init,   /*tp_init */
    0,
    0,
};

/***** MessageEntry *****/

typedef struct {
    EyeLogEntry m_parent;
} MessageEntry;

static int
MessageEntry_init(MessageEntry* self, PyObject* args, PyObject* kwds)
{
    PMessageEntry* message = NULL;
    double time;
    const char* msg;

    static const char* kwlist[] = {
        "time",
        "message",
        NULL
    };

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "ds",
                const_cast<char**>(kwlist),
                &time,
                &msg
                )
      )
        return -1;
    
    try {
        message = new PMessageEntry(time, msg);
    } catch(std::bad_alloc& e) {
        PyErr_NoMemory();
        return -1;
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "FixationEntry_init: unknown error");
        return -1;
    }

    self->m_parent.m_private  = static_cast<PEntryPtr>(message);
    return 0;
}

static PyObject*
MessageEntry_getMessage(MessageEntry* self)
{
    const char* msg;
    msg = static_cast<PMessageEntry*>(PRIV_POINTER)->getMessage().c_str();
    return PyString_FromString(msg);
}

static PyObject*
MessageEntry_setMessage(MessageEntry* self, PyObject* args) {
    const char* msg;
    if (!PyArg_ParseTuple(args, "s", &msg))
        return NULL;

    static_cast<PMessageEntry*>(PRIV_POINTER)->setMessage(msg);
    Py_RETURN_NONE;
}

static PyMethodDef MessageEntry_methods[] = {
    {"getMessage", (PyCFunction) MessageEntry_getMessage, METH_NOARGS,
        "Returns the x coordinate of the gaze."},
    {"setMessage", (PyCFunction) MessageEntry_setMessage, METH_VARARGS,
        "Returns the y coordinate of the gaze."},
    {NULL}
};

static PyObject*
MessageEntry_MessageGetter(MessageEntry* self, void*) 
{
    PMessageEntry* msg = static_cast<PMessageEntry*>(
            self->m_parent.m_private
            );
    return PyString_FromString(msg->toString().c_str());
}

static int
MessageEntry_MessageSetter(MessageEntry* self, PyObject* message, void*)
{
    const char* msg;
    if(!PyString_Check(message))
        return -1;

    msg = PyString_AsString(message);
      
    static_cast<PMessageEntry*>(self->m_parent.m_private)->setMessage(msg);
    return 0;
}

static PyGetSetDef MessageEntry_getset[] {
    {"message", (getter)MessageEntry_MessageGetter,
        (setter)MessageEntry_MessageSetter,
        "The contained message.", NULL},
    { NULL }
};

static PyTypeObject MessageEntryType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/   // for binary compatibility
    "pyeye.MessageEntry",          /*tp_name*/
    sizeof(MessageEntry),          /*tp_basicsize*/
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
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,         /*tp_flags*/
    "A MessageEntry gives opportunities to present some "
        "metadata in an EyeLog.",
                                /*tp_doc*/
    0,		                    /*tp_traverse */
    0,		                    /*tp_clear */
    0,		                    /*tp_richcompare */
    0,		                    /*tp_weaklistoffset */
    0,		                    /*tp_iter */
    0,		                    /*tp_iternext */
    MessageEntry_methods,       /*tp_methods */
    0,                          /*tp_members */
    MessageEntry_getset,        /*tp_getset */
    0,                          /*tp_base */
    0,                          /*tp_dict */
    0,                          /*tp_descr_get */
    0,                          /*tp_descr_set */
    0,                          /*tp_dictoffset */
    (initproc)MessageEntry_init,   /*tp_init */
    0,
    0,
};

/***** SaccadeEntry *****/

typedef struct {
    EyeLogEntry m_parent;
} SaccadeEntry;

static int
SaccadeEntry_init(SaccadeEntry* self, PyObject* args, PyObject* kwds)
{
    PSaccadeEntry* saccade = NULL;
    entrytype entry;
    double time, duration;
    float x1, y1, x2, y2;

    static const char* kwlist[] = {
        "entry",
        "time",
        "duration",
        "x1",
        "y1",
        "x2",
        "y2",
        NULL
    };

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "iddffff",
                const_cast<char**>(kwlist),
                &entry,
                &time,
                &duration,
                &x1,
                &y1,
                &x2,
                &y2
                )
      )
        return -1;
    if (entry != LSAC && entry != RSAC && entry != AVGSAC) {
        PyErr_SetString(PyExc_TypeError,
                "Entry must be one of LSAC, RSAC or AVGSAC");
        return -1;
    }
    
    try {
        saccade = new PSaccadeEntry(entry, time, duration, x1, y1, x2, y2);
    } catch(std::bad_alloc& e) {
        PyErr_NoMemory();
        return -1;
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "SaccadeEntry_init: unknown error");
        return -1;
    }

    self->m_parent.m_private  = static_cast<PEntryPtr>(saccade);
    return 0;
}

static PyObject*
SaccadeEntry_getDuration(SaccadeEntry* self)
{
    PSaccadeEntry* sac = static_cast<PSaccadeEntry*>(PRIV_POINTER);
    return PyFloat_FromDouble(sac->getDuration());
}

static int
SaccadeEntry_setDuration(SaccadeEntry* self, PyObject* args)
{
    double dur;
    if (!PyArg_ParseTuple(args, "d", &dur))
        return -1;
    static_cast<PSaccadeEntry*>(PRIV_POINTER)->setDuration(dur);
    return 0;
}

static PyObject*
SaccadeEntry_getX1(SaccadeEntry* self)
{
    PSaccadeEntry* sac = static_cast<PSaccadeEntry*>(PRIV_POINTER);
    return PyFloat_FromDouble(sac->getX1());
}

static int
SaccadeEntry_setX1(SaccadeEntry* self, PyObject* args)
{
    double x1;
    if (!PyArg_ParseTuple(args, "f", &x1))
        return -1;
    static_cast<PSaccadeEntry*>(PRIV_POINTER)->setX1(x1);
    return 0;
}

static PyObject*
SaccadeEntry_getY1(SaccadeEntry* self)
{
    PSaccadeEntry* sac = static_cast<PSaccadeEntry*>(PRIV_POINTER);
    return PyFloat_FromDouble(sac->getY1());
}

static int
SaccadeEntry_setY1(SaccadeEntry* self, PyObject* args)
{
    double y1;
    if (!PyArg_ParseTuple(args, "f", &y1))
        return -1;
    static_cast<PSaccadeEntry*>(PRIV_POINTER)->setY1(y1);
    return 0;
}

static PyObject*
SaccadeEntry_getX2(SaccadeEntry* self)
{
    PSaccadeEntry* sac = static_cast<PSaccadeEntry*>(PRIV_POINTER);
    return PyFloat_FromDouble(sac->getX2());
}

static int
SaccadeEntry_setX2(SaccadeEntry* self, PyObject* args)
{
    double x2;
    if (!PyArg_ParseTuple(args, "f", &x2))
        return -1;
    static_cast<PSaccadeEntry*>(PRIV_POINTER)->setX2(x2);
    return 0;
}

static PyObject*
SaccadeEntry_getY2(SaccadeEntry* self)
{
    PSaccadeEntry* sac = static_cast<PSaccadeEntry*>(PRIV_POINTER);
    return PyFloat_FromDouble(sac->getY2());
}

static int
SaccadeEntry_setY2(SaccadeEntry* self, PyObject* args)
{
    double y2;
    if (!PyArg_ParseTuple(args, "f", &y2))
        return -1;
    static_cast<PSaccadeEntry*>(PRIV_POINTER)->setY2(y2);
    return 0;
}

static PyMethodDef SaccadeEntry_methods[] = {
    {"getDuration", (PyCFunction) SaccadeEntry_getDuration, METH_NOARGS,
        "Returns the duration of the saccade."},
    {"setDuration", (PyCFunction) SaccadeEntry_setDuration, METH_VARARGS,
        "Sets the duration of the saccade."},
    {"getX1", (PyCFunction) SaccadeEntry_getX1, METH_NOARGS,
        "Returns the x coordinate of the starting point of the saccade."},
    {"setX1", (PyCFunction) SaccadeEntry_setX1, METH_VARARGS,
        "Sets the x coordinate of the starting point of the saccade."},
    {"getY1", (PyCFunction) SaccadeEntry_getY1, METH_NOARGS,
        "Returns the y coordinate of the starting point of the saccade."},
    {"setX1", (PyCFunction) SaccadeEntry_setY1, METH_VARARGS,
        "Sets the y coordinate of the starting point of the saccade."},
    {"getX2", (PyCFunction) SaccadeEntry_getX2, METH_NOARGS,
        "Returns the x coordinate of the end point of the saccade."},
    {"setX2", (PyCFunction) SaccadeEntry_setX2, METH_VARARGS,
        "Sets the x coordinate of the end point of the saccade."},
    {"getY2", (PyCFunction) SaccadeEntry_getY2, METH_NOARGS,
        "Returns the y coordinate of the end point of the saccade."},
    {"setX2", (PyCFunction) SaccadeEntry_setY2, METH_VARARGS,
        "Sets the y coordinate of the end point of the saccade."},
    {NULL}
};

static PyObject*
SaccadeEntry_DurationGetter(SaccadeEntry* self, void*)
{
    return PyFloat_FromDouble(
            static_cast<PSaccadeEntry*>(PRIV_POINTER)->getDuration()
            );
}

static int
SaccadeEntry_DurationSetter(SaccadeEntry* self, PyObject* value, void*)
{
    double dur;
    if (!PyFloat_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "Duration must be a float.");
        return -1;
    }
    dur = PyFloat_AsDouble(value);
    static_cast<PSaccadeEntry*>(PRIV_POINTER)->setDuration(dur);
    return 0;
}

static PyObject*
SaccadeEntry_X1Getter(SaccadeEntry* self, void*)
{
    return PyFloat_FromDouble(
            static_cast<PSaccadeEntry*>(PRIV_POINTER)->getX1()
            );
}

static int
SaccadeEntry_X1Setter(SaccadeEntry* self, PyObject* value, void*)
{
    float x1;
    if (!PyFloat_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "x1 must be a float.");
        return -1;
    }

    x1 = PyFloat_AsDouble(value);

    static_cast<PSaccadeEntry*>(PRIV_POINTER)->setX1(x1);
    return 0;
}

static PyObject*
SaccadeEntry_Y1Getter(SaccadeEntry* self, void*)
{
    return PyFloat_FromDouble(
            static_cast<PSaccadeEntry*>(PRIV_POINTER)->getY1()
            );
}

static int
SaccadeEntry_Y1Setter(SaccadeEntry* self, PyObject* value, void*)
{
    float y1;
    if (!PyFloat_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "y1 must be a float.");
        return -1;
    }

    y1 = PyFloat_AsDouble(value);

    static_cast<PSaccadeEntry*>(PRIV_POINTER)->setY1(y1);
    return 0;
}

static PyObject*
SaccadeEntry_X2Getter(SaccadeEntry* self, void*)
{
    return PyFloat_FromDouble(
            static_cast<PSaccadeEntry*>(PRIV_POINTER)->getX2()
            );
}

static int
SaccadeEntry_X2Setter(SaccadeEntry* self, PyObject* value, void*)
{
    float x2;
    if (!PyFloat_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "x2 must be a float.");
        return -1;
    }

    x2 = PyFloat_AsDouble(value);

    static_cast<PSaccadeEntry*>(PRIV_POINTER)->setX2(x2);
    return 0;
}

static PyObject*
SaccadeEntry_Y2Getter(SaccadeEntry* self, void*)
{
    return PyFloat_FromDouble(
            static_cast<PSaccadeEntry*>(PRIV_POINTER)->getY2()
            );
}

static int
SaccadeEntry_Y2Setter(SaccadeEntry* self, PyObject* value, void*)
{
    float y2;
    if (!PyFloat_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "y2 must be a float.");
        return -1;
    }

    y2 = PyFloat_AsDouble(value);

    static_cast<PSaccadeEntry*>(PRIV_POINTER)->setY2(y2);
    return 0;
}

static PyGetSetDef SaccadeEntry_getset[] {
    {"duration", (getter)SaccadeEntry_DurationGetter,
        (setter)SaccadeEntry_DurationSetter,
        "The duration of the saccade.", NULL},
    {"x1", (getter)SaccadeEntry_X1Getter,
        (setter)SaccadeEntry_X1Setter,
        "The start x coordinate of the saccade.", NULL},
    {"y1", (getter)SaccadeEntry_Y1Getter,
        (setter)SaccadeEntry_Y1Setter,
        "The start y coordinate of the saccade.", NULL},
    {"x2", (getter)SaccadeEntry_X2Getter,
        (setter)SaccadeEntry_X2Setter,
        "the end x coordinate of the saccade.", NULL},
    {"y2", (getter)SaccadeEntry_Y2Getter,
        (setter)SaccadeEntry_Y2Setter,
        "The end y coordinate of the saccade.", NULL},
    { NULL }
};

static PyTypeObject SaccadeEntryType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/   // for binary compatibility
    "pyeye.SaccadeEntry",       /*tp_name*/
    sizeof(SaccadeEntry),       /*tp_basicsize*/
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
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,         /*tp_flags*/
    "A SaccadeEntry represents one saccade in an EyeLog.",
                                /*tp_doc*/
    0,		                    /*tp_traverse */
    0,		                    /*tp_clear */
    0,		                    /*tp_richcompare */
    0,		                    /*tp_weaklistoffset */
    0,		                    /*tp_iter */
    0,		                    /*tp_iternext */
    SaccadeEntry_methods,       /*tp_methods */
    0,                          /*tp_members */
    SaccadeEntry_getset,        /*tp_getset */
    0,                          /*tp_base */
    0,                          /*tp_dict */
    0,                          /*tp_descr_get */
    0,                          /*tp_descr_set */
    0,                          /*tp_dictoffset */
    (initproc)SaccadeEntry_init,   /*tp_init */
    0,
    0,
};

/***** TrialEntry *****/

typedef struct {
    EyeLogEntry m_parent;
} TrialEntry;

static int
TrialEntry_init(TrialEntry* self, PyObject* args, PyObject* kwds)
{
    PTrialEntry* trial = NULL;
    double time;
    const char* identifier, *group;

    static const char* kwlist[] = {
        "time",
        "identifier",
        "group",
        NULL
    };

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "dss",
                const_cast<char**>(kwlist),
                &time,
                &identifier,
                &group
                )
      )
        return -1;
    
    try {
        trial = new PTrialEntry(time, identifier, group);
    } catch(std::bad_alloc& e) {
        PyErr_NoMemory();
        return -1;
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "TrialEntry_init: unknown error");
        return -1;
    }

    self->m_parent.m_private  = static_cast<PEntryPtr>(trial);
    return 0;
}

static PyObject*
TrialEntry_getIdentifier(TrialEntry* self)
{
    PTrialEntry* sac = static_cast<PTrialEntry*>(PRIV_POINTER);
    return PyString_FromString(sac->getIdentifier().c_str());
}

static int
TrialEntry_setIdentifier(TrialEntry* self, PyObject* args)
{
    const char* identifier;
    if (!PyArg_ParseTuple(args, "s", &identifier))
        return -1;
    static_cast<PTrialEntry*>(PRIV_POINTER)->setIdentifier(identifier);
    return 0;
}

static PyObject*
TrialEntry_getGroup(TrialEntry* self)
{
    PTrialEntry* sac = static_cast<PTrialEntry*>(PRIV_POINTER);
    return PyString_FromString(sac->getGroup().c_str());
}

static int
TrialEntry_setGroup(TrialEntry* self, PyObject* args)
{
    const char* group;
    if (!PyArg_ParseTuple(args, "s", &group))
        return -1;
    static_cast<PTrialEntry*>(PRIV_POINTER)->setGroup(group);
    return 0;
}

static PyMethodDef TrialEntry_methods[] = {
    {"getIdentifier", (PyCFunction) TrialEntry_getIdentifier, METH_NOARGS,
        "Returns the identifier of the trial."},
    {"setIdentifier", (PyCFunction) TrialEntry_setIdentifier, METH_VARARGS,
        "Sets the identifier of the trial."},
    {"getGroup", (PyCFunction) TrialEntry_getGroup, METH_NOARGS,
        "Returns the group of the trial."},
    {"setGroup", (PyCFunction) TrialEntry_setGroup, METH_VARARGS,
        "Sets the group of the trial."},
    {NULL}
};

static PyObject*
TrialEntry_IdentifierGetter(TrialEntry* self, void*)
{
    return PyString_FromString(
            static_cast<PTrialEntry*>(PRIV_POINTER)->getIdentifier().c_str()
            );
}

static int
TrialEntry_IdentifierSetter(TrialEntry* self, PyObject* value, void*)
{
    const char* identifier;
    if (!PyString_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "Identifier must be a string.");
        return -1;
    }

    identifier = PyString_AsString(value);

    static_cast<PTrialEntry*>(PRIV_POINTER)->setIdentifier(identifier);
    return 0;
}

static PyObject*
TrialEntry_GroupGetter(TrialEntry* self, void*)
{
    return PyString_FromString(
            static_cast<PTrialEntry*>(PRIV_POINTER)->getGroup().c_str()
            );
}

static int
TrialEntry_GroupSetter(TrialEntry* self, PyObject* value, void*)
{
    const char* group;
    if (!PyString_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "Identifier must be a string.");
        return -1;
    }

    group = PyString_AsString(value);

    static_cast<PTrialEntry*>(PRIV_POINTER)->setGroup(group);
    return 0;
}

static PyGetSetDef TrialEntry_getset[] {
    {"identifier", (getter)TrialEntry_IdentifierGetter,
        (setter)TrialEntry_IdentifierSetter,
        "This identifies a trial", NULL},
    {"group", (getter)TrialEntry_GroupGetter,
        (setter)TrialEntry_GroupSetter,
        "This tell the group to which the trial belongs.", NULL},
    { NULL }
};

static PyTypeObject TrialEntryType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/   // for binary compatibility
    "pyeye.TrialEntry",         /*tp_name*/
    sizeof(TrialEntry),         /*tp_basicsize*/
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
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,         /*tp_flags*/
    "A TrialEntry represents one trial this \"groups\" trials in a experiment.",
                                /*tp_doc*/
    0,		                    /*tp_traverse */
    0,		                    /*tp_clear */
    0,		                    /*tp_richcompare */
    0,		                    /*tp_weaklistoffset */
    0,		                    /*tp_iter */
    0,		                    /*tp_iternext */
    TrialEntry_methods,         /*tp_methods */
    0,                          /*tp_members */
    TrialEntry_getset,          /*tp_getset */
    0,                          /*tp_base */
    0,                          /*tp_dict */
    0,                          /*tp_descr_get */
    0,                          /*tp_descr_set */
    0,                          /*tp_dictoffset */
    (initproc)TrialEntry_init,  /*tp_init */
    0,
    0,
};

/***** TrialStartEntry *****/

typedef struct {
    EyeLogEntry m_parent;
} TrialStartEntry;

static int
TrialStartEntry_init(TrialStartEntry* self, PyObject* args, PyObject* kwds)
{
    PTrialStartEntry* trialstart = NULL;
    double time;

    static const char* kwlist[] = {
        "time",
        NULL
    };

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "d",
                const_cast<char**>(kwlist),
                &time
                )
      )
        return -1;
    
    try {
        trialstart = new PTrialStartEntry(time);
    } catch(std::bad_alloc& e) {
        PyErr_NoMemory();
        return -1;
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "TrialStartEntry_init: unknown error");
        return -1;
    }

    self->m_parent.m_private  = static_cast<PEntryPtr>(trialstart);
    return 0;
}

static PyMethodDef TrialStartEntry_methods[] = {
    {NULL}
};


static PyGetSetDef TrialStartEntry_getset[] {
    { NULL }
};

static PyTypeObject TrialStartEntryType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/   // for binary compatibility
    "pyeye.TrialStartEntry",         /*tp_name*/
    sizeof(TrialStartEntry),         /*tp_basicsize*/
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
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,         /*tp_flags*/
    "A TrialStartEntry redefines the trial start point many event are ignored "
    "when they occur before a TrialStartEntry.",
                                /*tp_doc*/
    0,		                    /*tp_traverse */
    0,		                    /*tp_clear */
    0,		                    /*tp_richcompare */
    0,		                    /*tp_weaklistoffset */
    0,		                    /*tp_iter */
    0,		                    /*tp_iternext */
    TrialStartEntry_methods,         /*tp_methods */
    0,                          /*tp_members */
    TrialStartEntry_getset,          /*tp_getset */
    0,                          /*tp_base */
    0,                          /*tp_dict */
    0,                          /*tp_descr_get */
    0,                          /*tp_descr_set */
    0,                          /*tp_dictoffset */
    (initproc)TrialStartEntry_init,  /*tp_init */
    0,
    0,
};

/***** TrialEndEntry *****/

typedef struct {
    EyeLogEntry m_parent;
} TrialEndEntry;

static int
TrialEndEntry_init(TrialEndEntry* self, PyObject* args, PyObject* kwds)
{
    PTrialEndEntry* trialstart = NULL;
    double time;

    static const char* kwlist[] = {
        "time",
        NULL
    };

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "d",
                const_cast<char**>(kwlist),
                &time
                )
      )
        return -1;
    
    try {
        trialstart = new PTrialEndEntry(time);
    } catch(std::bad_alloc& e) {
        PyErr_NoMemory();
        return -1;
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "TrialEndEntry_init: unknown error");
        return -1;
    }

    self->m_parent.m_private  = static_cast<PEntryPtr>(trialstart);
    return 0;
}

static PyMethodDef TrialEndEntry_methods[] = {
    {NULL}
};


static PyGetSetDef TrialEndEntry_getset[] {
    { NULL }
};

static PyTypeObject TrialEndEntryType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/   // for binary compatibility
    "pyeye.TrialEndEntry",         /*tp_name*/
    sizeof(TrialEndEntry),         /*tp_basicsize*/
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
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,         /*tp_flags*/
    "A TrialEndEntry redefines the trial end point many event are ignored "
    "when they occur after a TrialEndEntry.",
                                /*tp_doc*/
    0,		                    /*tp_traverse */
    0,		                    /*tp_clear */
    0,		                    /*tp_richcompare */
    0,		                    /*tp_weaklistoffset */
    0,		                    /*tp_iter */
    0,		                    /*tp_iternext */
    TrialEndEntry_methods,         /*tp_methods */
    0,                          /*tp_members */
    TrialEndEntry_getset,          /*tp_getset */
    0,                          /*tp_base */
    0,                          /*tp_dict */
    0,                          /*tp_descr_get */
    0,                          /*tp_descr_set */
    0,                          /*tp_dictoffset */
    (initproc)TrialEndEntry_init,  /*tp_init */
    0,
    0,
};


/***** end of eyelog entries *****/

// We only use this for the Wrappers arround EyeLogEntry.
#undef PRIV_POINTER

/***** EyeLog *****/

/*
 * This function is used to create A EyeLogEntry wrapper around a PEyeLogEntry
 * pointer. The entry provided by this function will be owned by the returned
 * reference. Therefore think about cloning the entry before providing it to 
 * this function to prevent double delete.
 */
static EyeLogEntry*
PEyeLogEntry_createWrapper(PEyeLogEntry* entry)
{
    assert(entry);

    EyeLogEntry* newentry = NULL;

    switch (entry->getEntryType()) {
        case LGAZE:
        case RGAZE:
        case AVGGAZE:
            newentry = (EyeLogEntry*) EyeLogEntry_new(&GazeEntryType, NULL, NULL);
            newentry->m_private = entry;
            break;
        case LFIX:
        case RFIX:
        case AVGFIX:
            newentry = (EyeLogEntry*) EyeLogEntry_new(&FixationEntryType, NULL, NULL);
            newentry->m_private = entry;
            break;
        case STIMULUS:
            assert(0); // implement
            break;
        case MESSAGE:
            newentry = (EyeLogEntry*) EyeLogEntry_new(&MessageEntryType, NULL, NULL);
            newentry->m_private = entry;
            break;
        case LSAC:
        case RSAC:
        case AVGSAC:
            newentry = (EyeLogEntry*) EyeLogEntry_new(&SaccadeEntryType, NULL, NULL);
            newentry->m_private = entry;
            break;
        case TRIAL:
            newentry = (EyeLogEntry*) EyeLogEntry_new(&TrialEntryType, NULL, NULL);
            newentry->m_private = entry;
            break;
        case TRIALSTART:
            newentry = (EyeLogEntry*) EyeLogEntry_new(&TrialStartEntryType, NULL, NULL);
            newentry->m_private = entry;
            break;
        case TRIALEND:
            newentry = (EyeLogEntry*) EyeLogEntry_new(&TrialEndEntryType, NULL, NULL);
            newentry->m_private = entry;
            break;
        default:
            assert(0); // unimplemented type
    }

    Py_INCREF(newentry);

    return newentry;
}

typedef struct {
    PyObject_HEAD
    PEyeLog* m_log;
} EyeLog;

void
EyeLog_dealloc(EyeLog* self)
{
    delete self->m_log;
    Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyObject*
EyeLog_new(PyTypeObject *type, PyObject* args, PyObject* kwds)
{
    EyeLog* ret = NULL;
    ret = (EyeLog*) type->tp_alloc(type, 0);

    if (ret)
        ret->m_log = NULL;
    else
        PyErr_NoMemory();

    return (PyObject*)ret;
}

static int
EyeLog_init(EyeLog* self, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
        return -1;

    try {
        self->m_log = new PEyeLog;
    }
    catch (std::bad_alloc& e) {
        PyErr_NoMemory();
        return -1;
    }
    catch (...) {
        PyErr_SetString(PyExc_RuntimeError, "EyeLog_init: unknown exception.");
        return -1;
    }
    return 0;
}

static PyObject*
EyeLog_open(EyeLog* self, PyObject* args)
{
    const char* filename = NULL;
    int ret;

    if(!PyArg_ParseTuple(args, "s", &filename))
        return NULL;

    ret = self->m_log->open(filename);
    if (ret != 0) {
        return PyErr_SetFromErrno((PyObject*)Py_TYPE(self));
    }

    Py_RETURN_NONE;
}

static PyObject*
EyeLog_close(EyeLog* self, PyObject* args)
{
    if (! PyArg_ParseTuple(args, ""))
        return NULL;

    self->m_log->close();
    Py_RETURN_NONE;
}

static PyObject*
EyeLog_clear(EyeLog* self, PyObject* args)
{
    if (! PyArg_ParseTuple(args, ""))
        return NULL;

    self->m_log->clear();
    Py_RETURN_NONE;
}

static PyObject*
EyeLog_reserve(EyeLog* self, PyObject* args)
{
    int n;
    if (!PyArg_ParseTuple(args, "i", &n))
        return NULL;

    try {
        self->m_log->reserve(n);
    }
    catch (std::bad_alloc& e) {
        return PyErr_NoMemory();
    }
    Py_RETURN_NONE;
}

static PyObject*
EyeLog_addEntry(EyeLog* self, PyObject* args)
{
    EyeLogEntry* entry = NULL;
    if (!PyArg_ParseTuple(args, "O!", &EyeLogEntryType, entry))
        return NULL;

    try {
        self->m_log->addEntry(entry->m_private->clone());
    } catch (std::bad_alloc& e) {
        return PyErr_NoMemory();
    }
    
    Py_RETURN_NONE;
}

static PyObject*
EyeLog_write(EyeLog* self, PyObject*args)
{
    int ret;
    eyelog_format format;
    if (!PyArg_ParseTuple(args, "i", &format))
        return NULL;

    if (format != FORMAT_CSV && format != FORMAT_BINARY) {
        PyErr_SetString(PyExc_ValueError,
                "write expects pyeye.BINARY or pyeye.CSV");
        return 0;
    }

    ret = self->m_log->write(format);
    if (ret)
        return PyErr_SetFromErrno((PyObject*)Py_TYPE(self));

    return PyInt_FromLong(ret);
}

static PyObject*
EyeLog_read(EyeLog* self, PyObject* args)
{
    const char* filename = NULL;
    int clear = 1;
    if(!PyArg_ParseTuple(args, "s|i", &filename, &clear))
        return NULL;

    int ret = self->m_log->read(filename, clear);
    if (ret)
        return PyErr_SetFromErrno((PyObject*)Py_TYPE(self));
    
    return PyInt_FromLong(ret);
}

static PyObject*
EyeLog_isOpen(EyeLog* self)
{
    return PyBool_FromLong(self->m_log->isOpen());
}

static PyObject*
EyeLog_getFilename(EyeLog* self)
{
    const char* filename;
    filename = self->m_log->getFilename();
    return PyString_FromString(filename);
}

static PyObject*
EyeLog_getEntries(EyeLog* self)
{
    const DArray<PEntryPtr>& array = self->m_log->getEntries();

    PyObject *list = PyList_New(array.size());
    if (!list)
        return NULL;

    for ( unsigned i = 0; i < array.size(); i++) {
        PEntryPtr clone = array[i]->clone();
        if(!clone) {
            Py_DECREF(list);
            return PyErr_NoMemory();
        }
        
        EyeLogEntry* entry = PEyeLogEntry_createWrapper(clone);
        
        if (!entry) {
            Py_DECREF(list);
            return PyErr_NoMemory();
        }

        PyList_SET_ITEM(list, i, (PyObject*) entry);
    }

    return list;
}

static PyObject*
EyeLog_setEntries(EyeLog* self, PyObject* args)
{
    PyObject* list = NULL;
    int i, clear=1;
    Py_ssize_t size;
    PEntryVec clones;

    if(!PyArg_ParseTuple(args, "O!|i", &PyList_Type, &list, &clear))
        return NULL;
    
    size = PyList_Size(list);
    
    /* loop over all entries to determine whether all type derive
     * from EyeLogEntry
     */
    try {
        clones.reserve(size);
        for (i = 0; i < size ; i++) {
            EyeLogEntry* entry = (EyeLogEntry*) PyList_GET_ITEM(list, i);
            if (!PyObject_IsInstance((PyObject*) entry,
                                    (PyObject*) &EyeLogEntryType)
                    ) {
                PyErr_SetString(PyExc_TypeError,
                        "Not all instance are derived of EyeLogEntry"
                        );
                destroyPEntyVec(clones);
                return NULL;
            }
            clones.push_back(entry->m_private->clone());
        }
    }
    catch (std::bad_alloc& e) {
        destroyPEntyVec(clones);
        return PyErr_NoMemory();
    }

    // The log will clear the cloned entries.
    self->m_log->setEntries(clones, clear);
    Py_RETURN_NONE;
}

static PyMethodDef EyeLog_methods[] = {
    {"open", (PyCFunction) EyeLog_open, METH_VARARGS,
        "Open the logfile."},
    {"close", (PyCFunction) EyeLog_close, METH_NOARGS,
        "Close the logfile."},
    {"clear", (PyCFunction) EyeLog_clear, METH_NOARGS,
        "Clear all logentries from the log."},
    {"reserve", (PyCFunction) EyeLog_reserve, METH_VARARGS,
        "Reserve a number of places inside the log, this is probably a small optimalisation in python."},
    {"addEntry",(PyCFunction) EyeLog_addEntry,METH_VARARGS,
        "Add a log entry to the log."},
    {"write",(PyCFunction) EyeLog_write,METH_VARARGS,
        "Write the format to the specified location with a given format."},
    {"read",(PyCFunction) EyeLog_read,METH_VARARGS,
        "Read a file from a specified location."},
    {"isOpen",(PyCFunction) EyeLog_isOpen, METH_NOARGS,
        "Returns whether the file is opened."},
    {"getFilename",(PyCFunction) EyeLog_getFilename, METH_NOARGS,
        "Get the filename."},
    {"getEntries",(PyCFunction) EyeLog_getEntries, METH_NOARGS,
        "Get the entries in the log."},
    {"setEntries",(PyCFunction) EyeLog_setEntries, METH_VARARGS,
        "Get the entries in the log."},
    {NULL}
};


static PyTypeObject EyeLogType = {
    PyObject_HEAD_INIT(NULL)
    0,                          /*ob_size*/   // for binary compatibility
    "pyeye.EyeLog",             /*tp_name*/
    sizeof(EyeLog),             /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    (destructor)EyeLog_dealloc, /*tp_dealloc*/
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
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,         /*tp_flags*/
    "A EyeLog redefines the trial end point many event are ignored "
    "when they occur after a EyeLog.",
                                /*tp_doc*/
    0,		                    /*tp_traverse */
    0,		                    /*tp_clear */
    0,		                    /*tp_richcompare */
    0,		                    /*tp_weaklistoffset */
    0,		                    /*tp_iter */
    0,		                    /*tp_iternext */
    EyeLog_methods,             /*tp_methods */
    0,                          /*tp_members */
    0,                          /*tp_getset */
    0,                          /*tp_base */
    0,                          /*tp_dict */
    0,                          /*tp_descr_get */
    0,                          /*tp_descr_set */
    0,                          /*tp_dictoffset */
    (initproc)EyeLog_init,  /*tp_init */
    0,
    EyeLog_new,
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

    /*
     * Ready eyelogentries 
     */
    if (PyType_Ready(&EyeLogEntryType) < 0)
        return;
    
    GazeEntryType.tp_base = &EyeLogEntryType;
    if (PyType_Ready(&GazeEntryType) < 0)
        return;

    FixationEntryType.tp_base = &EyeLogEntryType;
    if (PyType_Ready(&FixationEntryType) < 0)
        return;
    
    MessageEntryType.tp_base = &EyeLogEntryType;
    if (PyType_Ready(&MessageEntryType) < 0)
        return;

    SaccadeEntryType.tp_base = &EyeLogEntryType;
    if (PyType_Ready(&SaccadeEntryType) < 0)
        return;
    
    TrialEntryType.tp_base = &EyeLogEntryType;
    if (PyType_Ready(&TrialEntryType) < 0)
        return;

    TrialStartEntryType.tp_base = &EyeLogEntryType;
    if (PyType_Ready(&TrialStartEntryType) < 0)
        return;

    TrialEndEntryType.tp_base = &EyeLogEntryType;
    if (PyType_Ready(&TrialEndEntryType) < 0)
        return;

    // Ready eyelog
    if(PyType_Ready(&EyeLogType) < 0)
        return;

    m = Py_InitModule3(module_name, PyEyeMethods, module_doc);
    if(m == NULL)
        return;


    /*
     * add eyelog entries
     */
    Py_INCREF(&EyeLogEntryType);
    PyModule_AddObject(m, "EyeLogEntry", (PyObject*) &EyeLogEntryType);
    
    Py_INCREF(&GazeEntryType);
    PyModule_AddObject(m, "GazeEntry", (PyObject*) &GazeEntryType);
    
    Py_INCREF(&FixationEntryType);
    PyModule_AddObject(m, "FixationEntry", (PyObject*) &FixationEntryType);
    
    Py_INCREF(&MessageEntryType);
    PyModule_AddObject(m, "MessageEntry", (PyObject*) &MessageEntryType);

    Py_INCREF(&SaccadeEntryType);
    PyModule_AddObject(m, "SaccadeEntry", (PyObject*) &SaccadeEntryType);
    
    Py_INCREF(&TrialEntryType);
    PyModule_AddObject(m, "TrialEntry", (PyObject*) &TrialEntryType);
    
    Py_INCREF(&TrialStartEntryType);
    PyModule_AddObject(m, "TrialStartEntry", (PyObject*) &TrialStartEntryType);
    
    Py_INCREF(&TrialEndEntryType);
    PyModule_AddObject(m, "TrialEndEntry", (PyObject*) &TrialEndEntryType);

    // Add eyelog.
    Py_INCREF(&EyeLogType);
    PyModule_AddObject(m, "EyeLog", (PyObject*) &EyeLogType);

    
    pyeye_module_add_constants(m);
    
    // for internal use only.
    TheEyeLogEntry = (EyeLogEntry*) EyeLogEntry_new(&EyeLogEntryType, NULL, NULL);
    if (!TheEyeLogEntry)
        return;
            
    TheEyeLogEntry->m_private = NULL;
    Py_INCREF(TheEyeLogEntry);
    PyModule_AddObject(m, "_TheEyeLogEntry", (PyObject*)TheEyeLogEntry);

//    PyEyeError = PyErr_NewException("pyeye.PyEyeError", NULL, NULL);
//    Py_INCREF(&PyEyeError);
//
//    PyModule_AddObject(m, "PyError", PyEyeError);
}

#ifdef __cplusplus
}
#endif
