import Sofa
import SofaCV

def createScene(node):
    node.createObject('RequiredPlugin', pluginName='SofaCV')
    # node.createObject('RequiredPlugin', pluginName='ImageProcessing')

    node.createObject('VideoGrabber', name='input', cam_index="0", async="false") ## Multithread not possible with python!
    node.createObject('PythonScriptController', filename='ImageFilterController.py', classname='ImageFilter', printLog="true") ## Add your python code in a controller to respect execution order in visitors
        
    node.createObject('FrameViewer', name='viewer', img='@input.img_out', corners="-1 1 0 1 1 0 1 -1 0 -1 -1 0", mode="ORTHO")
    
    return 0
