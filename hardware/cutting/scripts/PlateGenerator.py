import sys
import ezdxf
import matplotlib.pyplot as plt
from ezdxf.addons.drawing import RenderContext, Frontend 
from ezdxf.addons.drawing.matplotlib import MatplotlibBackend

folder = "./DXF/"

# Try ro open the file or raise an error and exit the script
try:
    doc = ezdxf.readfile(folder+"BackPanel.dxf")
except IOError:
    print(f"File not found :(")
    sys.exit(1)
except ezdxf.DXFStructureError:
    print(f"Invalid or corrupted DXF file :(")
    sys.exit(2)

msp = doc.modelspace()

# Change the color of all the elements to Red

for e in msp:
    e.dxf.color = 1

center_y = 51;


# Add text boxes

info = {"project_name" : "RayonX",
        "city": "Zurich"}

style_title = dxfattribs={
    "style": "OpenSans",
    "char_height": 4,
    "attachment_point":5
    }

style_subtitle = dxfattribs={
    "style": "OpenSans",
    "char_height": 2,
    "attachment_point":5
    }

style_top = dxfattribs={
    "style": "OpenSans",
    "char_height": 2,
    "attachment_point":5
    }

msp.add_mtext(info["project_name"], style_title).set_location((center_y, -6))
msp.add_mtext("Made in "+info["city"], style_top).set_location((center_y, 6))
msp.add_mtext("By ToutatiX", style_subtitle).set_location((center_y, -14))

# Add lines between the title and the subtitle

msp.add_line((center_y-10, -10), (center_y+10, -10))
msp.add_line((center_y-10, -11), (center_y+10, -11))

# Save the new file

fig = plt.figure()
out = MatplotlibBackend(fig.add_axes([0, 0, 1, 1]))
Frontend(RenderContext(doc), out).draw_layout(doc.modelspace(), finalize=True)
fig.savefig("./BackPanel.svg")

#doc.saveas('BackPanelWithEngraving.dxf')