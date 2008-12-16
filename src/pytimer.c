/*
 * pytimer.c
 *
 * Copyright (C) 2007  Red Hat, Inc.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions of
 * the GNU General Public License v.2, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY expressed or implied, including the implied warranties of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.  You should have received a copy of the
 * GNU General Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
 * source code or documentation are not subject to the GNU General Public
 * License and may only be used or replicated with the express permission of
 * Red Hat, Inc.
 *
 * Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
 *                    Chris Lumens <clumens@redhat.com>
 */

#include <Python.h>

#include "convert.h"
#include "exceptions.h"
#include "pytimer.h"
#include "typeobjects/pytimer.h"

/* _ped.Timer functions */
void _ped_Timer_dealloc(_ped_Timer *self) {
    PyObject_GC_UnTrack(self);
    PyObject_GC_Del(self);
}

int _ped_Timer_traverse(_ped_Timer *self, visitproc visit, void *arg) {
    return 0;
}

int _ped_Timer_clear(_ped_Timer *self) {
    return 0;
}

int _ped_Timer_init(_ped_Timer *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {"frac", "start", "now", "predicted_end",
                             "state_name", NULL};

    self->state_name = NULL;

    /* XXX: timers aren't really done yet in pyparted */
    PyErr_SetString(PyExc_NotImplementedError, NULL);
    return -1;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|fdddz", kwlist,
                                     &self->frac, &self->start, &self->now,
                                     &self->predicted_end, &self->state_name))
        return -1;
    else
        return 0;
}

PyObject *_ped_Timer_get(_ped_Timer *self, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Timer()");
        return NULL;
    }

    if (!strcmp(member, "frac")) {
        return Py_BuildValue("f", self->frac);
    } else if (!strcmp(member, "start")) {
        return Py_BuildValue("d", self->start);
    } else if (!strcmp(member, "now")) {
        return Py_BuildValue("d", self->now);
    } else if (!strcmp(member, "predicted_end")) {
        return Py_BuildValue("d", self->predicted_end);
    } else if (!strcmp(member, "state_name")) {
        return PyString_FromString(self->state_name);
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Timer object has no attribute %s", member);
        return NULL;
    }
}

int _ped_Timer_set(_ped_Timer *self, PyObject *value, void *closure) {
    char *member = (char *) closure;

    if (member == NULL) {
        PyErr_SetString(PyExc_TypeError, "Empty _ped.Timer()");
        return -1;
    }

    if (!strcmp(member, "frac")) {
        if (!PyArg_ParseTuple(value, "f", &self->frac)) {
            return -1;
        }
    } else if (!strcmp(member, "start")) {
        self->start = PyFloat_AsDouble(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "now")) {
        self->now = PyFloat_AsDouble(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "predicted_end")) {
        self->predicted_end = PyFloat_AsDouble(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else if (!strcmp(member, "state_name")) {
        self->state_name = PyString_AsString(value);
        if (PyErr_Occurred()) {
            return -1;
        }
    } else {
        PyErr_Format(PyExc_AttributeError, "_ped.Timer object has no attribute %s", member);
        return -1;
    }

    return 0;
}

/* 1:1 function mappings for timer.h in libparted */
PyObject *py_ped_timer_destroy(PyObject *s, PyObject *args) {
    Py_CLEAR(s);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_timer_new_nested(PyObject *s, PyObject *args) {
    float nest_frac;
    PedTimer *parent = NULL, *timer = NULL;
    _ped_Timer *ret = NULL;

    if (!PyArg_ParseTuple(args, "f", &nest_frac))
        return NULL;

    parent = _ped_Timer2PedTimer(s);
    if (parent == NULL) {
        return NULL;
    }

    timer = ped_timer_new_nested(parent, nest_frac);
    if (timer) {
        ret = PedTimer2_ped_Timer(timer);
        if (ret == NULL) {
            return NULL;
        }
    }
    else {
        PyErr_SetString(CreateException, "Could not create new nested timer");
        return NULL;
    }

    ped_timer_destroy(parent);
    ped_timer_destroy(timer);

    return (PyObject *) ret;
}

PyObject *py_ped_timer_destroy_nested(PyObject *s, PyObject *args) {
    PedTimer *timer = NULL;

    timer = _ped_Timer2PedTimer(s);
    if (timer == NULL) {
        return NULL;
    }

    ped_timer_destroy_nested(timer);
    ped_timer_destroy(timer);
    Py_CLEAR(s);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_timer_touch(PyObject *s, PyObject *args) {
    PedTimer *timer = NULL;

    timer = _ped_Timer2PedTimer(s);
    if (timer == NULL) {
        return NULL;
    }

    ped_timer_touch(timer);
    ped_timer_destroy(timer);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_timer_reset(PyObject *s, PyObject *args) {
    PedTimer *timer = NULL;

    timer = _ped_Timer2PedTimer(s);
    if (timer == NULL) {
        return NULL;
    }

    ped_timer_reset(timer);
    ped_timer_destroy(timer);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_timer_update(PyObject *s, PyObject *args) {
    float frac;
    PedTimer *timer = NULL;

    if (!PyArg_ParseTuple(args, "f", &frac))
        return NULL;

    timer = _ped_Timer2PedTimer(s);
    if (timer == NULL) {
        return NULL;
    }

    ped_timer_update(timer, frac);
    ped_timer_destroy(timer);

    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *py_ped_timer_set_state_name(PyObject *s, PyObject *args) {
    char *str = NULL;
    PedTimer *timer = NULL;

    if (!PyArg_ParseTuple(args, "z", &str)) {
        return NULL;
    }

    timer = _ped_Timer2PedTimer(s);
    if (timer == NULL) {
        return NULL;
    }

    ped_timer_set_state_name(timer, str);

    ped_timer_destroy(timer);
    free(str);

    Py_INCREF(Py_None);
    return Py_None;
}

/* vim:tw=78:ts=4:et:sw=4
 */
