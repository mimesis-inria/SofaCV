#include "Binding_cvMatData.h"
#include <SofaPython/Binding_Data.h>
#include <SofaPython/PythonToSofa.inl>

using namespace sofa::core::objectmodel;
using namespace sofa::defaulttype;

static PyObject* cvMatData_getPtr(PyObject* self, PyObject* /*args*/)
{
  Data<sofaor::common::cvMat>* data =
      sofa::py::unwrap<Data<sofaor::common::cvMat> >(self);
  sofaor::common::cvMat& image =
      *data->beginEdit();  // where should be the endedit?

  PyObject* imgdata = PyLong_FromVoidPtr(image.data); // data


  PyObject* shape = PyTuple_New(3);
  PyTuple_SetItem(shape, 0, PyInt_FromSsize_t(image.rows));        // rows
  PyTuple_SetItem(shape, 1, PyInt_FromSsize_t(image.cols));        // cols
  PyTuple_SetItem(shape, 2, PyInt_FromSsize_t(image.channels()));  // channels

  // output = tuple( list(pointers), shape tuple, type name)
  PyObject* res = PyTuple_New(3);

  // the data pointer
  PyTuple_SetItem(res, 0, imgdata);

  // the shape
  PyTuple_SetItem(res, 1, shape);

  // the channel depth as a recognisable string for ctypeFromName (c.f SofaNumpy
  // in SofaPython plugin)

  std::string s = "";
  switch (image.depth())
  {
  case CV_64F:
      s = "double";
      break;
  case CV_32F:
      s = "float";
      break;
  case CV_32S:
      s = "int";
      break;
  case CV_16S:
      s = "short";
      break;
  case CV_16U:
      s = "unsigned short";
      break;
  case CV_8S:
      s = "char";
      break;
  case CV_8U:
  default:
      s = "unsigned char";
      break;
  }

  PyTuple_SetItem(res, 2, PyString_FromString(s.c_str()));

  return res;
}

static PyObject* cvMatData_getData(PyObject* self, PyObject* /*args*/)
{
  Data<sofaor::common::cvMat>* data =
      sofa::py::unwrap<Data<sofaor::common::cvMat> >(self);
  sofaor::common::cvMat& image =
      *data->beginEdit();  // where should be the endedit?

  PyObject* imgdata = PyString_FromStringAndSize(
      (char*)image.data, image.total() * image.elemSize());
  PyObject* shape = PyTuple_New(3);
  PyTuple_SetItem(shape, 0, PyInt_FromSsize_t(image.rows));        // rows
  PyTuple_SetItem(shape, 1, PyInt_FromSsize_t(image.cols));        // cols
  PyTuple_SetItem(shape, 2, PyInt_FromSsize_t(image.channels()));  // channels

  // output = tuple( list(pointers), shape tuple, type name)
  PyObject* res = PyTuple_New(3);

  // the data pointer
  PyTuple_SetItem(res, 0, imgdata);

  // the shape
  PyTuple_SetItem(res, 1, shape);

  // the type name
  std::string s = "";
  switch (image.depth())
  {
  case CV_64F:
      s = "double";
      break;
  case CV_32F:
      s = "float";
      break;
  case CV_32S:
      s = "int";
      break;
  case CV_16S:
      s = "short";
      break;
  case CV_16U:
      s = "unsigned short";
      break;
  case CV_8S:
      s = "char";
      break;
  case CV_8U:
  default:
      s = "unsigned char";
      break;
  }

  PyTuple_SetItem(res, 2, PyString_FromString(s.c_str()));
  data->endEdit();
  return res;
}

SP_CLASS_METHODS_BEGIN(cvMatData)
SP_CLASS_METHOD_DOC(cvMatData, getPtr,
                    "retrieves a Tuple containing:\n 1. The pointer to "
                    "Mat::data (as a numpy array)\n 2. The dimensions of the "
                    "matrix (rows, cols, channels, depth\n 3. The name of the "
                    "Sofa type ('cvMat').")
SP_CLASS_METHOD_DOC(cvMatData, getData,
                    "retrieves a Tuple containing:\n 1. a deep copy of the "
                    "Mat::data byte array (as a numpy array)\n 2. The "
                    "dimensions of the matrix (rows, cols, channels, depth\n "
                    "3. The name of the Sofa type ('cvMat').")
SP_CLASS_METHODS_END

// eventual attributes
SP_CLASS_ATTRS_BEGIN(cvMatData)
// SP_CLASS_ATTR(CVMat, paramName)
SP_CLASS_ATTRS_END

SP_CLASS_TYPE_PTR_ATTR(cvMatData, sofa::Data<sofaor::common::cvMat>, Data)
