import Sofa
import SofaCV
import sys
import cv2
import numpy as np

class ImageFilter(Sofa.PythonScriptController):

    def initGraph(self,node):
        self.input = node.getObject('input') ## get pointer to engine component holding the image to manipulate
        return 0


    def onBeginAnimationStep(self,dt):
        self.image = SofaCV.mat_as_numpy(self.input.img_out) ## Call SofaOR.mat_as_numpy (declared in SofaORCommon/python/SofaOR/__init__.py) on the data of interest 
        self.image[:] = cv2.flip(self.image, 1) ## Use 
        self.img = cv2.Canny(self.image, 100, 200)
        self.image[:] = cv2.cvtColor(self.img,cv2.COLOR_GRAY2RGB)

        return 0


    
