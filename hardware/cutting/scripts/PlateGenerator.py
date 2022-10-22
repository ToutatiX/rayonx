import sys
import ezdxf
import matplotlib.pyplot as plt
from ezdxf.addons.drawing import RenderContext, Frontend 
from ezdxf.addons.drawing.matplotlib import MatplotlibBackend
import json
import imageio.v3 as iio
from pyradios import RadioBrowser
from matplotlib.offsetbox import (OffsetImage, AnnotationBbox)
import numpy as np

rb = RadioBrowser()

folder = "./DXF/"

# Try ro open the file or raise an error and exit the script
try:
    doc = ezdxf.readfile(folder+"BackPanel.DXF")
except IOError:
    print(f"File `BackPanel` not found :(")
    sys.exit(1)
except ezdxf.DXFStructureError:
    print(f"Invalid or corrupted DXF file :(")
    sys.exit(2)

msp = doc.modelspace()
doc.units = ezdxf.units.MM

# Change the color of all the elements to Red
for e in msp:
    e.dxf.color = 1

center_y = 52;


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


# Try ro open the file or raise an error and exit the script
try:
    doc = ezdxf.readfile(folder+"FrontPanel.DXF")
except IOError:
    print(f"File `FrontPanel` not found :(")
    sys.exit(1)
except ezdxf.DXFStructureError:
    print(f"Invalid or corrupted DXF file :(")
    sys.exit(2)

msp = doc.modelspace()

# Change the color of all the elements to Red
for e in msp:
    e.dxf.color = 1

hs = 0.86602540378

coor = [[center_y, 18], [center_y, -18],
        [center_y + 18*hs,18/2],  [center_y + 18*hs, -18/2],
        [center_y - 18*hs,18/2],  [center_y - 18*hs, -18/2]]

for c in coor:
    msp.add_circle(
    # Use UCS coordinates to place the 2d circle in 3d space
    center=(c),
    radius=5.4,
    dxfattribs={'color': 250}
)

try:
    presets = json.load(open('../configs/presets.config.json'))
    presets = presets[:6]
except IOError:
    print(f"File `presets.config` not found :(")
    sys.exit(1)

fig = plt.figure()
ax = fig.add_axes([0, 0, 1, 1])
ctx = RenderContext(doc)
out = MatplotlibBackend(ax)
Frontend(ctx, out).draw_layout(doc.modelspace(), finalize=True)

for i, preset in enumerate(presets):
    icon = iio.imread(preset["icon_url"], mode="RGBA", index=None)
    icon_size = icon.shape[:2]
    gray_icon = np.dot(icon, [0.2989, 0.5870, 0.1140, 0.])
    gray_icon = np.round(gray_icon).astype(np.uint8)
    gray_icon_as_rgba = np.concatenate((np.stack([gray_icon] * 3, axis=-1), [[[x] for x in y] for y in icon[:,:, -1]]), axis=2)
    for y in range(0, icon.shape[0]):
        for x in range(0, icon.shape[1]):
            if (y-icon.shape[0]/2)**2/icon.shape[0]**2 + (x-icon.shape[1]/2)**2/icon.shape[1]**2 > 1/4*1.25:
                gray_icon_as_rgba[x, y, -1] = 0

    imagebox = OffsetImage(gray_icon_as_rgba, zoom=37/max(icon_size))
    ab = AnnotationBbox(imagebox, coor[i], frameon = False)
    ax.add_artist(ab)


fig.savefig("./FrontPanel.svg", format='svg', transparent=True, pad_inches=0, dpi=1000)
