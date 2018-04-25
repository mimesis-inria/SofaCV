
try:
    import numpy
    import SofaPython.SofaNumpy

    def mat_as_numpy(img):
        '''maps image content as a numpy array with shared memory'''

        ptr, shape, typename = img.getPtr()

        t = SofaPython.SofaNumpy.ctypeFromName.get(typename, None)
        if not type: raise Exception("can't map image of type " + typename)
        array = SofaPython.SofaNumpy.ctypes.cast(SofaPython.SofaNumpy.ctypes.c_void_p(ptr), SofaPython.SofaNumpy.ctypes.POINTER(t))
        nparr = numpy.ctypeslib.as_array(array, shape)
        return nparr


except ImportError: # numpy is not mandatory
    pass
