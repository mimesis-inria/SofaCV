
import Sofa
import SofaCV

def createScene(node):
    node.createObject('RequiredPlugin', pluginName='SofaCV')
    node.createObject('RequiredPlugin', pluginName='SofaPython')

    # Asynchronous grab (a thread grabs frames and submits the latest to the simulation at each step. (Real-time, No Schedulers needed)
    node.createObject('VideoGrabber', name='input', cam_index="0", async="true")

    node.createObject('PythonScriptDataEngine', filename='ImageFilterController.py', classname='ImageFilter', printLog="true")

    node.createObject('FrameViewer', name='viewer', img='@input.img_out', corners="-1 1 0 1 1 0 1 -1 0 -1 -1 0", mode="ORTHO")
    
    return 0
