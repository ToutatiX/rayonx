from sympy import O
import vtkplotlib as vpl
from stl.mesh import Mesh
import numpy as np
import os

def renderSTL(fileName):

    dirname = os.path.dirname(__file__)

    ModelViewer = vpl.figure()
    ModelViewer.background_color = [255,255,255]

    mesh = Mesh.from_file(os.path.join(dirname, "../../printing/")  + fileName + ".STL", mode='bin')

    points  = np.int16(np.around(np.unique(mesh.vectors.reshape([int(mesh.vectors.size/3), 3]), axis=0),2))
    maximaa = np.int16(np.round(np.array([np.max(points[:,0]), np.max(points[:,1]), np.max(points[:,2])])))
    minimaa = np.int16(np.round(np.array([np.min(points[:,0]), np.min(points[:,1]), np.min(points[:,2])])))

    # TO DO : Add colors, returns the dimenssions of the bouncing box
    bounds = [
        [minimaa[0], maximaa[1], minimaa[2]],
        [minimaa[0], minimaa[1], minimaa[2]],
        [minimaa[0], minimaa[1], maximaa[2]],
        [minimaa[0], maximaa[1], maximaa[2]],
        [minimaa[0], maximaa[1], minimaa[2]],
        [maximaa[0], maximaa[1], minimaa[2]],
        [maximaa[0], maximaa[1], maximaa[2]],
        [minimaa[0], maximaa[1], maximaa[2]],
    ]

    vpl.plot(bounds, line_width=1, join_ends=False, color='grey')

    bounds = [
        [maximaa[0], minimaa[1], minimaa[2]],
        [minimaa[0], minimaa[1], minimaa[2]],
        [minimaa[0], minimaa[1], maximaa[2]],
        [maximaa[0], minimaa[1], maximaa[2]],
        [maximaa[0], minimaa[1], minimaa[2]],
        [maximaa[0], maximaa[1], minimaa[2]],
        [maximaa[0], maximaa[1], maximaa[2]],
        [maximaa[0], minimaa[1], maximaa[2]],
    ]

    vpl.plot(bounds, line_width=1, join_ends=False, color='grey')

    bounds = [
        [minimaa[0], minimaa[1], minimaa[2]],
        [maximaa[0], minimaa[1], minimaa[2]]
    ]

    vpl.plot(bounds, line_width=2, join_ends=False, color='red')

    bounds = [
        [minimaa[0], minimaa[1], minimaa[2]],
        [minimaa[0], minimaa[1], maximaa[2]]
    ]

    vpl.plot(bounds, line_width=2, join_ends=False, color='blue')

    bounds = [
        [minimaa[0], maximaa[1], minimaa[2]],
        [minimaa[0], minimaa[1], minimaa[2]]
    ]

    vpl.plot(bounds, line_width=2, join_ends=False, color='green')

    vpl.view(camera_direction=[0, 1, -1])
    vpl.mesh_plot(mesh, color='orange',fig=ModelViewer)
    ModelViewer.update()
    vpl.save_fig(os.path.join(dirname, "./../public/3dPrinted/") + fileName + ".jpg", pixels=(600, 600), off_screen=False)

    return [maximaa[0] - minimaa[0], maximaa[1] - minimaa[1], maximaa[2] - minimaa[2]], os.path.join(dirname, "./../public/3dPrinted/") + fileName + ".jpg"