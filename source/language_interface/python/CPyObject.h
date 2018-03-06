#pragma once

#include <Python.h>

class CPyObject
{
public:
	CPyObject() : m_pObj(NULL) {};
	CPyObject(PyObject* obj) : m_pObj(obj) {}
	CPyObject(CPyObject& obj) : m_pObj(obj.GetObject()){
		AddRef();
	}
	~CPyObject() {
		Release();
	}

	PyObject*	AddRef() {
		if (m_pObj) {
			Py_INCREF(m_pObj);
		}
		return m_pObj;
	}
	void		Release() {
		if (m_pObj) {
			Py_DECREF(m_pObj);
		}
		m_pObj = nullptr;
	}

	PyObject*	GetObject() {
		return m_pObj;
	}
	PyObject*	SetObject(PyObject* obj) {
		return (m_pObj = obj);
	}

	PyObject* operator ->() {
		return m_pObj;
	}

	bool is() {	return m_pObj ? true : false; }
	PyObject* operator = (PyObject* a) {
		m_pObj = a;
	}

	operator PyObject*(){
		return m_pObj;
	}

	operator bool() {
		return is();
	}
private:
	PyObject* m_pObj;
};