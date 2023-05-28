#USE PYTHON 

from utils.DXFHandler import *
import json
import imageio.v3 as iio
import numpy as np
from matplotlib.offsetbox import (OffsetImage, AnnotationBbox)

def main():
    
    dirname = os.path.dirname(__file__)
    folder = os.path.join(dirname, "../cutting/")
    doc, msp = openDXF(folder+"DXF/FrontPanel.DXF")

    preset_name = input("Enter preset name:")

    changeColorAll(msp, 1)

    center_x = 51.4
    hs = 0.86602540378

    coor = [[center_x, 18], [center_x + 18*hs,18/2], [center_x + 18*hs, -18/2], [center_x, -18], [center_x - 18*hs, -18/2], [center_x - 18*hs, +18/2]]

    try:
        presets = json.load(open(folder+'../configs/presets.'+preset_name+'.config.json'))
        presets = presets[:6]
    except IOError:
        print("File `presets."+preset_name+".config.json not found :(")
        sys.exit(1)

    fig, ax = createFigure(doc)
        
    for i, preset in enumerate(presets):
        icon = iio.imread(preset["icon_url"], mode="RGBA", index=None)
        icon_size = icon.shape[:2]
        gray_icon = np.dot(icon, [0.2989, 0.5870, 0.1140, 0.])
        gray_icon = np.round(gray_icon).astype(np.uint8)
        gray_icon_as_rgba = np.concatenate((np.stack([gray_icon] * 3, axis=-1), [[[x] for x in y] for y in icon[:,:, -1]]), axis=2)
        for y in range(0, icon.shape[1]):
            for x in range(0, icon.shape[0]):
                if (y-icon.shape[0]/2)**2/icon.shape[0]**2 + (x-icon.shape[1]/2)**2/icon.shape[1]**2 > 1/4*1.25:
                    gray_icon_as_rgba[x, y, -1] = 0

        imagebox = OffsetImage(gray_icon_as_rgba, zoom=26/max(icon_size))
        ab = AnnotationBbox(imagebox, coor[i], frameon = False)
        ax.add_artist(ab)

    fig.savefig(folder+"FrontPanel.svg", format='svg')
    plt.close(fig)

    fin = open(folder + "FrontPanel.svg", "rt").readlines()
    del fin[23:31]
    out = open(folder + "FrontPanel.svg", 'wt')
    out.writelines(fin)
    out.close()

if __name__ == '__main__':
    main()