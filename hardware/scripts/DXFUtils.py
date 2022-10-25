from typing import Tuple
import matplotlib.pyplot as plt
import sys
import ezdxf
from ezdxf.addons.drawing import RenderContext, Frontend 
from ezdxf.addons.drawing.matplotlib import MatplotlibBackend
import os
from ezdxf.addons import Importer

def createFigure(doc) :
    
    fig = plt.figure(dpi=1000)
    ax = plt.Axes(fig, (0, 0, 1, 1))
    ax.set_axis_off()
    fig.add_axes(ax)
    ax.set_axis_off()
    ax.set_aspect('equal')
    ctx = RenderContext(doc)
    out = MatplotlibBackend(ax)
    Frontend(ctx, out).draw_layout(doc.modelspace(), finalize=True)
    xmin, xmax = ax.get_xlim()
    ymin, ymax = ax.get_ylim()

    total_width = (xmax - xmin)
    total_height = (ymax - ymin)
    plt.figaspect(total_height / total_width)

    fig.set_size_inches(total_width / 25.4, total_height/25.4)

    return fig, ax

def openDXF(path):
    """ Try to open the DXF file or raise an error """
    try:
        doc = ezdxf.readfile(path)
        return doc, doc.modelspace()
    except IOError:
        print(f"File `BackPanel` not found :(")
        sys.exit(1)
    except ezdxf.DXFStructureError:
        print(f"Invalid or corrupted DXF file :(")
        sys.exit(2)

def changeColorAll(modelspace, color):
    """ Change the color of all the elements of a given model space to `color` """
    for e in modelspace: e.dxf.color = color