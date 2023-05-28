from genericpath import isfile
from utils.DXFHandler import *
import json
import imageio.v3 as iio
from pyradios import RadioBrowser
from matplotlib.offsetbox import (OffsetImage, AnnotationBbox)
import matplotlib.font_manager as font_manager


def main():

    dirname = os.path.dirname(__file__)
    folder = os.path.join(dirname, "../cutting/")

    try:
        info = json.load(open(folder+'../configs/product.config.json'))
    except IOError:
        print(f"File `presets.config` not found :(")
        sys.exit(1)

    custom_fonts_paths = os.path.join(dirname, "../scripts/fonts")
    
    for font in font_manager.findSystemFonts(custom_fonts_paths):
        font_manager.fontManager.addfont(font)

    doc, msp = openDXF(folder+"DXF/BackPanel.DXF")

    doc.styles.new('myStandard', dxfattribs={'font' : 'Classical Garamond.ttf'})

    logo_doc = ezdxf.new()

    importer = Importer(doc, logo_doc)
    importer.import_modelspace()
    importer.import_paperspace_layouts()
    importer.finalize()

    changeColorAll(msp, 1)

    center_y = 52/2

    style_title = dxfattribs={
        "style": "myStandard",
        "char_height": 4,
        "attachment_point":5,
        "color": 250
        }

    style_subtitle = dxfattribs={
        "style": "myStandard",
        "char_height": 2,
        "attachment_point":5,
        "color": 250
        }

    # msp.add_mtext("Made in "+info["city"], style_subtitle).set_location((center_y, 20))
    msp.add_mtext(info["project_name"], style_title).set_location((center_y, -5.8))
    msp.add_mtext("Par ToutatiX", style_subtitle).set_location((center_y, -9.5))
    msp.add_mtext("Manufactured in "+info["city"]+" by", style_subtitle).set_location((center_y, -16))
    msp.add_mtext("Joyeux anniversaire !", style_subtitle).set_location((center_y, 16))
    #msp.add_mtext(info["serial"][16:], style_subtitle).set_location((0, -15.5))

    # Add lines between the title and the subtitle
    hatch = msp.add_hatch(color=250)
    hatch.paths.add_polyline_path(
        [(center_y-14, -2), (center_y+14, -2), (center_y+14, -2.2), (center_y-14, -2.2)], is_closed=True
    )
    hatch2 = msp.add_hatch(color=250)
    hatch2.paths.add_polyline_path(
        [(center_y-14, -1.6), (center_y+14, -1.6), (center_y+14, -1.2), (center_y-14, -1.2)], is_closed=True
    )

    # Add logo
    fig, ax = createFigure(doc)
    logo = plt.imread(folder+'../scripts/public/Logo.png')
    imagebox = OffsetImage(logo, zoom=40/1080)
    ab = AnnotationBbox(imagebox, (center_y, 4.3), frameon = False)
    ax.add_artist(ab)

    fig.savefig(folder+"BackPanel.svg", format='svg')
    plt.close(fig)

    fin = open(folder + "BackPanel.svg", "rt").readlines()
    del fin[23:31]
    out = open(folder + "BackPanel.svg", 'wt')
    out.writelines(fin)
    print("File saved to: " + folder + "BackPanel.svg")
    out.close()

if __name__ == '__main__':
    main()