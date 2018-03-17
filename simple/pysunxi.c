/*
 * pySUNXI.c
 *
 * Copyright 2013 Stefan Mavrodiev <support@olimex.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */


#include "Python.h"
#include "gpio_lib.h"


static PyObject *SetupException;
static PyObject *OutputException;
static PyObject *InputException;
static PyObject *inp;
static PyObject *out;
static PyObject *per;
static PyObject *high;
static PyObject *low;

#define PD0    SUNXI_GPD(0)
#define PD1    SUNXI_GPD(1)
#define PD2    SUNXI_GPD(2)
#define PD3    SUNXI_GPD(3)
#define PD4    SUNXI_GPD(4)
#define PD5    SUNXI_GPD(5)
#define PD6    SUNXI_GPD(6)
#define PD7    SUNXI_GPD(7)
#define PD8    SUNXI_GPD(8)
#define PD9    SUNXI_GPD(9)
#define PD10    SUNXI_GPD(10)
#define PD11    SUNXI_GPD(11)
#define PD12    SUNXI_GPD(12)
#define PD13    SUNXI_GPD(13)
#define PD14    SUNXI_GPD(14)
#define PD15    SUNXI_GPD(15)
#define PD16    SUNXI_GPD(16)
#define PD17    SUNXI_GPD(17)
#define PD18    SUNXI_GPD(18)
#define PD19    SUNXI_GPD(19)
#define PD20    SUNXI_GPD(20)
#define PD21    SUNXI_GPD(21)
#define PD22    SUNXI_GPD(22)
#define PD23    SUNXI_GPD(23)
#define PD24    SUNXI_GPD(24)
#define PD25    SUNXI_GPD(25)
#define PD26    SUNXI_GPD(26)
#define PD27    SUNXI_GPD(27)

#define PG     SUNXI_GPIO_G
#define PG0    SUNXI_GPG(0)
#define PG1    SUNXI_GPG(1)
#define PG2    SUNXI_GPG(2)
#define PG3    SUNXI_GPG(3)
#define PG4    SUNXI_GPG(4)
#define PG5    SUNXI_GPG(5)
#define PG6    SUNXI_GPG(6)
#define PG7    SUNXI_GPG(7)
#define PG8    SUNXI_GPG(8)
#define PG9    SUNXI_GPG(9)
#define PG10    SUNXI_GPG(10)
#define PG11    SUNXI_GPG(11)

#define PC      SUNXI_GPIO_C
#define PC19    SUNXI_GPC(19)
#define PC20    SUNXI_GPC(20)
#define PC21    SUNXI_GPC(21)
#define PC22    SUNXI_GPC(22)

#define PB      SUNXI_GPIO_B
#define PB14    SUNXI_GPB(14)
#define PB15    SUNXI_GPB(15)
#define PB16    SUNXI_GPB(16)
#define PB17    SUNXI_GPB(17)
#define PB18    SUNXI_GPB(18)
#define PB19    SUNXI_GPB(19)


#define MISO    SUNXI_GPE(3)
#define MOSI    SUNXI_GPE(2)
#define SCK     SUNXI_GPE(1)
#define CS      SUNXI_GPE(0)

static int module_setup(void) {
    int result;

    result = sunxi_gpio_init();
    if(result == SETUP_DEVMEM_FAIL) {
        PyErr_SetString(SetupException, "No access to /dev/mem. Try running as root!");
        return SETUP_DEVMEM_FAIL;
    }
    else if(result == SETUP_MALLOC_FAIL) {
        PyErr_NoMemory();
        return SETUP_MALLOC_FAIL;
    }
    else if(result == SETUP_MMAP_FAIL) {
        PyErr_SetString(SetupException, "Mmap failed on module import");
        return SETUP_MMAP_FAIL;
    }
    else {
        return SETUP_OK;
    }

    return SETUP_OK;
}



static PyObject* py_output(PyObject* self, PyObject* args) {
    int gpio;
    int value;

    if(!PyArg_ParseTuple(args, "ii", &gpio, &value))
        return NULL;

    if(value != 0 && value != 1) {
        PyErr_SetString(OutputException, "Invalid output state");
        return NULL;
    }

    if(sunxi_gpio_get_cfgpin(gpio) != SUNXI_GPIO_OUTPUT) {
        PyErr_SetString(OutputException, "GPIO is no an output");
        return NULL;
    }
    sunxi_gpio_output(gpio, value);

    Py_RETURN_NONE;
}

static PyObject* py_output_bank(PyObject* self, PyObject* args) {
    int bank;
    int value;

    if(!PyArg_ParseTuple(args, "ii", &bank, &value))
        return NULL;

    sunxi_gpio_output_bank(bank, value);

    Py_RETURN_NONE;
}

static PyObject* py_input(PyObject* self, PyObject* args) {
    int gpio;
    int result;

    if(!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;

    if(sunxi_gpio_get_cfgpin(gpio) != SUNXI_GPIO_INPUT) {
        PyErr_SetString(InputException, "GPIO is not an input");
        return NULL;
    }
    result = sunxi_gpio_input(gpio);

    if(result == -1) {
        PyErr_SetString(InputException, "Reading pin failed");
        return NULL;
    }


    return Py_BuildValue("i", result);
}

static PyObject* py_setcfg(PyObject* self, PyObject* args) {
    int gpio;
    int direction;

    if(!PyArg_ParseTuple(args, "ii", &gpio, &direction))
        return NULL;

    if(direction != 0 && direction != 1 && direction != 2) {
        PyErr_SetString(SetupException, "Invalid direction");
        return NULL;
    }
    sunxi_gpio_set_cfgpin(gpio, direction);

    Py_RETURN_NONE;
}
static PyObject* py_getcfg(PyObject* self, PyObject* args) {
    int gpio;
    int result;


    if(!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;

    result = sunxi_gpio_get_cfgpin(gpio);


    return Py_BuildValue("i", result);


}
static PyObject* py_init(PyObject* self, PyObject* args) {

    module_setup();

    Py_RETURN_NONE;
}
static PyObject* py_cleanup(PyObject* self, PyObject* args) {

    sunxi_gpio_cleanup();
    Py_RETURN_NONE;
}


PyMethodDef module_methods[] = {
    {"init", py_init, METH_NOARGS, "Initialize module"},
    {"cleanup", py_cleanup, METH_NOARGS, "munmap /dev/map."},
    {"setcfg", py_setcfg, METH_VARARGS, "Set direction."},
    {"getcfg", py_getcfg, METH_VARARGS, "Get direction."},
    {"output", py_output, METH_VARARGS, "Set output state"},
    {"outputBank", py_output_bank, METH_VARARGS, "Set bank output state"},
    {"input", py_input, METH_VARARGS, "Get input state"},
    {NULL, NULL, 0, NULL}
};
#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "SUNXI module",
    NULL,
    -1,
    module_methods
};
#endif
PyMODINIT_FUNC initSUNXI_GPIO(void) {
    PyObject* module = NULL;


#if PY_MAJOR_VERSION >= 3
    module = PyModule_Create(&module_methods);
#else
    module = Py_InitModule("SUNXI_GPIO", module_methods);
#endif


    if(module == NULL)
#if PY_MAJOR_VERSION >= 3
        return module;
#else
        return;
#endif



    SetupException = PyErr_NewException("PySUNXI.SetupException", NULL, NULL);
    PyModule_AddObject(module, "SetupException", SetupException);
    OutputException = PyErr_NewException("PySUNXI.OutputException", NULL, NULL);
    PyModule_AddObject(module, "OutputException", OutputException);
    InputException = PyErr_NewException("PySUNXI.InputException", NULL, NULL);
    PyModule_AddObject(module, "InputException", InputException);



    high = Py_BuildValue("i", HIGH);
    PyModule_AddObject(module, "HIGH", high);

    low = Py_BuildValue("i", LOW);
    PyModule_AddObject(module, "LOW", low);

    inp = Py_BuildValue("i", INPUT);
    PyModule_AddObject(module, "IN", inp);

    out = Py_BuildValue("i", OUTPUT);
    PyModule_AddObject(module, "OUT", out);

    per = Py_BuildValue("i", PER);
    PyModule_AddObject(module, "PER", per);



    PyModule_AddObject(module, "PD0", Py_BuildValue("i", PD0));
    PyModule_AddObject(module, "PD1", Py_BuildValue("i", PD1));
    PyModule_AddObject(module, "PD2", Py_BuildValue("i", PD2));
    PyModule_AddObject(module, "PD3", Py_BuildValue("i", PD3));
    PyModule_AddObject(module, "PD4", Py_BuildValue("i", PD4));
    PyModule_AddObject(module, "PD5", Py_BuildValue("i", PD5));
    PyModule_AddObject(module, "PD6", Py_BuildValue("i", PD6));
    PyModule_AddObject(module, "PD7", Py_BuildValue("i", PD7));
    PyModule_AddObject(module, "PD8", Py_BuildValue("i", PD8));
    PyModule_AddObject(module, "PD9", Py_BuildValue("i", PD9));

    PyModule_AddObject(module, "PG", Py_BuildValue("i", PG)); 
    PyModule_AddObject(module, "PG0", Py_BuildValue("i", PG0));
    PyModule_AddObject(module, "PG1", Py_BuildValue("i", PG1));
    PyModule_AddObject(module, "PG2", Py_BuildValue("i", PG2));
    PyModule_AddObject(module, "PG3", Py_BuildValue("i", PG3));
    PyModule_AddObject(module, "PG4", Py_BuildValue("i", PG4));
    PyModule_AddObject(module, "PG5", Py_BuildValue("i", PG5));
    PyModule_AddObject(module, "PG6", Py_BuildValue("i", PG6));
    PyModule_AddObject(module, "PG7", Py_BuildValue("i", PG7));
    PyModule_AddObject(module, "PG8", Py_BuildValue("i", PG8));
    PyModule_AddObject(module, "PG9", Py_BuildValue("i", PG9));

    PyModule_AddObject(module, "PC", Py_BuildValue("i", PC)); 
    PyModule_AddObject(module, "PC19", Py_BuildValue("i", PC19));
    PyModule_AddObject(module, "PC20", Py_BuildValue("i", PC20));
    PyModule_AddObject(module, "PC21", Py_BuildValue("i", PC21));
    PyModule_AddObject(module, "PC22", Py_BuildValue("i", PC22));
    
    PyModule_AddObject(module, "PB", Py_BuildValue("i", PB)); 
    PyModule_AddObject(module, "PB14", Py_BuildValue("i", PB14));
    PyModule_AddObject(module, "PB15", Py_BuildValue("i", PB15)); 
    PyModule_AddObject(module, "PB16", Py_BuildValue("i", PB16));
    PyModule_AddObject(module, "PB17", Py_BuildValue("i", PB17));
    PyModule_AddObject(module, "PB18", Py_BuildValue("i", PB18));
    PyModule_AddObject(module, "PB19", Py_BuildValue("i", PB19));

    PyModule_AddObject(module, "MISO", Py_BuildValue("i", MISO));
    PyModule_AddObject(module, "MOSI", Py_BuildValue("i", MOSI));
    PyModule_AddObject(module, "SCK", Py_BuildValue("i", SCK));
    PyModule_AddObject(module, "CS", Py_BuildValue("i", CS));
    
    if(Py_AtExit(sunxi_gpio_cleanup) != 0){
        
        sunxi_gpio_cleanup();
        
#if PY_MAJOR_VERSION >= 3
        return NULL;
#else
        return;
#endif
    }



}

