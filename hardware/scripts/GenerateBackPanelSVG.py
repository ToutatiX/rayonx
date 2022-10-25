from genericpath import isfile
from DXFUtils import *
import json
import imageio.v3 as iio
from pyradios import RadioBrowser
from matplotlib.offsetbox import (OffsetImage, AnnotationBbox)
import matplotlib.font_manager as font_manager

info = {"project_name" : "RAYONX",
        "city": "Zürich"}

def main():
    global info

    dirname = os.path.dirname(__file__)
    custom_fonts_paths = os.path.join(dirname, "../scripts/fonts")

    for font in font_manager.findSystemFonts(custom_fonts_paths):
        font_manager.fontManager.addfont(font)

    dirname = os.path.dirname(__file__)
    folder = os.path.join(dirname, "../cutting/")
    doc, msp = openDXF(folder+"DXF/BackPanel.DXF")

    doc.styles.new('myStandard', dxfattribs={'font' : 'Classical Garamond.ttf'})

    logo_doc = ezdxf.new()

    importer = Importer(doc, logo_doc)
    importer.import_modelspace()
    importer.import_paperspace_layouts()
    importer.finalize()

    changeColorAll(msp, 1)

    center_y = 52

    style_title = dxfattribs={
        "style": "myStandard",
        "char_height": 4,
        "attachment_point":5,
        "color": 250
        }

    style_subtitle = dxfattribs={
        "style": "myStandard",
        "char_height": 1.6,
        "attachment_point":5,
        "color": 250
        }

    style_top = dxfattribs={
        "style": "myStandard",
        "char_height": 2,
        "attachment_point":5,
        "color": 250
        }

    msp.add_mtext("Made in "+info["city"], style_top).set_location((center_y, 16))
    msp.add_mtext(info["project_name"], style_title).set_location((center_y, -4))
    msp.add_mtext("Par ToutatiX", style_subtitle).set_location((center_y, -8))
    msp.add_mtext("Manufacturé par", style_subtitle).set_location((center_y, -14))

    # Add lines between the title and the subtitle
    msp.add_line((center_y-16, 0), (center_y+16, 0), dxfattribs={'color': 250})
    msp.add_line((center_y-16, 0.5), (center_y+16, 0.5), dxfattribs={'color': 250})

    fig, ax = createFigure(doc)
    logo = plt.imread(folder+'../scripts/public/Logo.png')
    imagebox = OffsetImage(logo, zoom=40/1080)
    ab = AnnotationBbox(imagebox, (center_y, 6), frameon = False)
    ax.add_artist(ab)

    fig.savefig(folder+"BackPanel.svg", format='svg')
    plt.close(fig)

    fin = open(folder + "BackPanel.svg", "rt").readlines()
    del fin[23:31]
    out = open(folder + "BackPanel.svg", 'wt')
    out.writelines(fin)
    out.close()

if __name__ == '__main__':
    main()