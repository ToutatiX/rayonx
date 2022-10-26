from genericpath import isfile
from DXFUtils import *
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

    msp.add_mtext("Made in "+info["city"], style_top).set_location((center_y, 20))
    msp.add_mtext(info["project_name"], style_title).set_location((center_y, -3.8))
    msp.add_mtext("Par ToutatiX", style_subtitle).set_location((center_y, -7))
    msp.add_mtext("Manufacturé par", style_subtitle).set_location((center_y, -14))

    # Add lines between the title and the subtitle
    hatch = msp.add_hatch(color=250)
    hatch.paths.add_polyline_path(
        [(center_y-16, 0), (center_y+16, 0), (center_y+16, -0.2), (center_y-16, -0.2)], is_closed=True
    )
    hatch2 = msp.add_hatch(color=250)
    hatch2.paths.add_polyline_path(
        [(center_y-16, 0.4), (center_y+16, 0.4), (center_y+16, 0.8), (center_y-16, 0.8)], is_closed=True
    )

    fig, ax = createFigure(doc)
    logo = plt.imread(folder+'../scripts/public/Logo.png')
    imagebox = OffsetImage(logo, zoom=40/1080)
    ab = AnnotationBbox(imagebox, (center_y, 6.3), frameon = False)
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