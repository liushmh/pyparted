/*
 * pyfilesys.h
 * pyparted type objects for pyfilesys.c
 *
 * Copyright (C) 2007, 2008  Red Hat, Inc.
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

#ifndef TYPEOBJECTS_PYFILESYS_H_INCLUDED
#define TYPEOBJECTS_PYFILESYS_H_INCLUDED

#include <Python.h>
#include <structmember.h>

/* _ped.FileSystemType type object */
static PyMemberDef _ped_FileSystemType_members[] = {
    {NULL}
};

static PyMethodDef _ped_FileSystemType_methods[] = {
    {"register", (PyCFunction) py_ped_file_system_type_register,
                 METH_VARARGS, file_system_type_register_doc},
    {"unregister", (PyCFunction) py_ped_file_system_type_unregister,
                   METH_VARARGS, file_system_type_unregister_doc},
    {NULL}
};

static PyGetSetDef _ped_FileSystemType_getset[] = {
    {"name", (getter) _ped_FileSystemType_get,
             (setter) _ped_FileSystemType_set,
             "The name of the FileSystemType.", "name"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_FileSystemType_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.FileSystemType",
    .tp_basicsize = sizeof(_ped_FileSystemType),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_FileSystemType_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
    .tp_call = NULL,
 /* .tp_str = XXX */
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_BASETYPE,
    .tp_doc = _ped_FileSystemType_doc,
    .tp_traverse = (traverseproc) _ped_FileSystemType_traverse,
    .tp_clear = (inquiry) _ped_FileSystemType_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_FileSystemType_methods,
    .tp_members = _ped_FileSystemType_members,
    .tp_getset = _ped_FileSystemType_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = NULL,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = NULL,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

/* _ped.FileSystem type object */
static PyMemberDef _ped_FileSystem_members[] = {
    {"type", T_OBJECT, offsetof(_ped_FileSystem, type), 0,
             "A _ped.FileSystemType object describing the filesystem on self.geom."},
    {"geom", T_OBJECT, offsetof(_ped_FileSystem, geom), 0,
             "The on-disk region where this FileSystem object exists."},
    {NULL}
};

static PyMethodDef _ped_FileSystem_methods[] = {
    {"clobber", (PyCFunction) py_ped_file_system_clobber, METH_VARARGS,
                file_system_clobber_doc},
    {"open", (PyCFunction) py_ped_file_system_open, METH_VARARGS,
             file_system_open_doc},
    {"create", (PyCFunction) py_ped_file_system_create, METH_VARARGS,
               file_system_create_doc},
    {"close", (PyCFunction) py_ped_file_system_close, METH_VARARGS,
              file_system_close_doc},
    {"check", (PyCFunction) py_ped_file_system_check, METH_VARARGS,
              file_system_check_doc},
    {"copy", (PyCFunction) py_ped_file_system_copy, METH_VARARGS,
             file_system_copy_doc},
    {"resize", (PyCFunction) py_ped_file_system_resize, METH_VARARGS,
               file_system_resize_doc},
    {"get_create_constraint", (PyCFunction)
                              py_ped_file_system_get_create_constraint,
                              METH_VARARGS, file_system_get_create_constraint_doc},
    {"get_resize_constraint", (PyCFunction)
                              py_ped_file_system_get_resize_constraint,
                              METH_VARARGS, file_system_get_resize_constraint_doc},
    {"get_copy_constraint", (PyCFunction)
                            py_ped_file_system_get_copy_constraint,
                            METH_VARARGS, file_system_get_copy_constraint_doc},
    {NULL}
};

static PyGetSetDef _ped_FileSystem_getset[] = {
    {"checked", (getter) _ped_FileSystem_get,
                (setter) _ped_FileSystem_set,
                "Has the filesystem been checked prior to calling copy or resize?",
                "checked"},
    {NULL}  /* Sentinel */
};

PyTypeObject _ped_FileSystem_Type_obj = {
    PyObject_HEAD_INIT(&PyType_Type)
    .tp_name = "_ped.FileSystem",
    .tp_basicsize = sizeof(_ped_FileSystem),
 /* .tp_itemsize = XXX */
    .tp_dealloc = (destructor) _ped_FileSystem_dealloc,
 /* .tp_getattr = XXX */
 /* .tp_setattr = XXX */
 /* .tp_compare = XXX */
 /* .tp_repr = XXX */
 /* .tp_as_number = XXX */
 /* .tp_as_sequence = XXX */
 /* .tp_as_mapping = XXX */
 /* .tp_hash = XXX */
    .tp_call = NULL,
 /* .tp_str = XXX */
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_setattro = PyObject_GenericSetAttr,
 /* .tp_as_buffer = XXX */
    .tp_flags = Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_CHECKTYPES |
                Py_TPFLAGS_HAVE_GC | Py_TPFLAGS_BASETYPE,
    .tp_doc = _ped_FileSystem_doc,
    .tp_traverse = (traverseproc) _ped_FileSystem_traverse,
    .tp_clear = (inquiry) _ped_FileSystem_clear,
 /* .tp_richcompare = XXX */
 /* .tp_weaklistoffset = XXX */
 /* .tp_iter = XXX */
 /* .tp_iternext = XXX */
    .tp_methods = _ped_FileSystem_methods,
    .tp_members = _ped_FileSystem_members,
    .tp_getset = _ped_FileSystem_getset,
    .tp_base = NULL,
    .tp_dict = NULL,
 /* .tp_descr_get = XXX */
 /* .tp_descr_set = XXX */
 /* .tp_dictoffset = XXX */
    .tp_init = (initproc) _ped_FileSystem_init,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = PyType_GenericNew,
 /* .tp_free = XXX */
 /* .tp_is_gc = XXX */
    .tp_bases = NULL,
 /* .tp_del = XXX */
};

#endif /* TYPEOBJECTS_PYFILESYS_H_INCLUDED */

/* vim:tw=78:ts=4:et:sw=4
 */
