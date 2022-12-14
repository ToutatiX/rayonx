from genericpath import isfile
import os
import sys
from pylatex import Document, LongTable, MultiColumn, MiniPage, LineBreak, VerticalSpace, StandAloneGraphic, Hyperref, Tabular, Command, HorizontalSpace
from pylatex.base_classes import Environment, CommandBase, Arguments
from pylatex.package import Package
from pylatex import Document, Section, UnsafeCommand, LongTabu, Tabu, Center
from pylatex.utils import NoEscape, bold, escape_latex
import json
from utils.RenderSTL import renderSTL
from datetime import datetime, timedelta

dirname = os.path.dirname(__file__)

margin = 1.5

geometry_options = {
    "margin": str(margin)+"cm",
    "paper" : "a4paper",
	"headheight":"0.75cm",
	"footskip":"1cm",
	"headsep":"0.5cm",
    "columnsep":"1cm"
}
doc = Document(page_numbers=True, documentclass='extarticle', inputenc='utf8', geometry_options=geometry_options, indent=False, lmodern = False)

doc.packages.append(Package('raleway', 'default'))
doc = Document('test', geometry_options=geometry_options)

doc.preamble.append(Command('usepackage', 'multicol', 'hyperref'))

def hyperlink(url,text):
    text = escape_latex(text)
    return NoEscape(r'\href{' + url + '}{' + text + '}')

try:
    info = json.load(open(os.path.join(dirname, "../configs/product.config.json")))
except IOError:
    print(f"File `presets.config` not found :(")
    sys.exit(1)

document_delivery = datetime.fromisoformat(info["document_delivery"])
validity = document_delivery + timedelta(days = 31)


"""
Compute the quantities of each materials used for the product
"""

def computeQuantity(type, part):
    """
    If 3D printed material, we just return the weight times the quantity required, otherwise, we return the area times the quantity
    """
    if type in ["3D_printing"]:
        return part[3] * part[1]
    else:
        return round(part[3] * part[4] * part[1] / 100)

materials = dict()

for type in info["bom"]["non-sourced"]:
    for part in info["bom"]["non-sourced"][type]:
        #aterials[info["bom"]["non-sourced"][type][part][2]] = info["bom"]["non-sourced"][type][part][1]
        if part[2] in materials:
            materials[part[2]] += computeQuantity(type, part)
        else:
            materials[part[2]] = computeQuantity(type, part)



"""
New classes for titles and subtitles, black box around a white text 
"""

class SubTitle(CommandBase):
    _latex_name = 'subtitle'
    packages = [Package('color')]

subtitle_command = UnsafeCommand('newcommand', '\subtitle', options=1,
                             extra_arguments=r'\noindent\colorbox{black}{\Large{\textcolor{white}{\textbf{\MakeUppercase{#1}}}}}')

doc.append(subtitle_command)

class SubSubTitle(CommandBase):
    _latex_name = 'subsubtitle'
    packages = [Package('color')]

subsubtitle_command = UnsafeCommand('newcommand', '\subsubtitle', options=1,
                             extra_arguments=r'\noindent\colorbox{black}{\large{\textcolor{white}{\textbf{\MakeUppercase{#1}}}}}')

doc.append(subsubtitle_command)

class Title(CommandBase):
    _latex_name = 'hugetitle'
    packages = [Package('color'), Package('moresize')]

title_command = UnsafeCommand('newcommand', '\hugetitle', options=1,
                             extra_arguments=r'\noindent \colorbox{black}{\HUGE{\textcolor{white}{\textbf{\MakeUppercase{#1}}}}}')

doc.append(title_command)

#### TO DOOOO
#### Create a command to get an hypterlink NoEscape(r'\href{' + url + '}{' + text + '}')



"""
HEADER SECTION WITH LOGO AND HUGE TITLE
"""

with doc.create(MiniPage(align='c')) as logo_wrapper:
            logo_file = os.path.join(dirname, './public/Logo.png')
            logo_wrapper.append(StandAloneGraphic(image_options="width=25mm",
                                filename=logo_file))
        

doc.append(VerticalSpace('4mm'))
doc.append(LineBreak())

with doc.create(MiniPage(align='l')):
    with doc.create(MiniPage(width=str(4/5*(21-2*margin))+r"cm", align='l')):
        doc.append(Title(arguments=Arguments('Hardware Manual'))) ; doc.append(VerticalSpace('1mm')) ; doc.append(LineBreak())
        doc.append("Manufacturer Manual par ToutatiX")
    with doc.create(MiniPage(width=str(1/5*(21-2*margin))+r"cm", align='r')):
        with doc.create(MiniPage(width=str(1/5*(21-2*margin))+r"cm", align='l')):
            with doc.create(MiniPage(width=r"5mm", align='l')) as logo_wrapper:
                logo_file = os.path.join(dirname, './public/GlobeIcon.png')
                logo_wrapper.append(StandAloneGraphic(image_options="width=7mm",
                                    filename=logo_file))
            doc.append(HorizontalSpace('4mm'))
            doc.append(hyperlink("toutatix.fr", "toutatix.fr"))
        doc.append(LineBreak())
        with doc.create(MiniPage(width=str(1/5*(21-2*margin))+r"cm", align='l')):
            with doc.create(MiniPage(width=r"5mm", align='l')) as logo_wrapper:
                logo_file = os.path.join(dirname, './public/EmailIcon.png')
                logo_wrapper.append(StandAloneGraphic(image_options="width=7mm",
                                    filename=logo_file))
            doc.append(HorizontalSpace('4mm'))
            doc.append(hyperlink("mailto:eliott.sarrey@toutatix.fr","info@toutatix.fr"))


"""
DELIVERY INFORMATION AND FABLAB NAME
"""

doc.append(VerticalSpace('8mm'))
doc.append(LineBreak())

with doc.create(MiniPage(align='c')):
    with doc.create(MiniPage(align='l', width='4cm')) as delivery_info:
        delivery_info.append("Document delivery:")
        doc.append(LineBreak())
        delivery_info.append("Document valid until:")
        doc.append(LineBreak())
        delivery_info.append("Order Number:")
        doc.append(LineBreak())
        delivery_info.append("Serial Number:")
        doc.append(LineBreak())
        delivery_info.append("Client name:")
        doc.append(LineBreak())
        delivery_info.append("FabLab:")


    with doc.create(MiniPage(align='l', width='6cm')) as delivery_info:
        delivery_info.append(document_delivery.strftime("%A %B %d %Y"))
        doc.append(LineBreak())
        delivery_info.append(validity.strftime("%A %B %d %Y"))
        doc.append(LineBreak())
        delivery_info.append(info["order"])
        doc.append(LineBreak())
        delivery_info.append(info["serial"])
        doc.append(LineBreak())        
        delivery_info.append(info["client"])
        doc.append(LineBreak())
        delivery_info.append(info["fablab"])

doc.append(VerticalSpace('12mm'))
doc.append(LineBreak())


"""
SOURCED COMPONENTS SECTION
"""

doc.append(SubTitle(arguments=Arguments('Sourced Components')))
doc.append(VerticalSpace("4mm"))
doc.append(LineBreak())

doc.append("Components to order from a local reseller. Those components are in every RayonX entity and some of them are common in ToutatiX's products.")

with doc.create(LongTable('c|c|c|c')) as data_table:
    header_row1 = ["Part", "Quantity", "Manufacturer", "Quantity"]
    data_table.add_row(header_row1, mapper=[bold])
    data_table.add_hline()
    for row in info["bom"]["sourced"]:
        data_table.add_row(row)



"""
RAW MATERIALS SECTION
"""

doc.append(SubTitle(arguments=Arguments('Raw Materials')))
doc.append(VerticalSpace("4mm"))
doc.append(LineBreak())

doc.append("Raw materials to have in order to be able to manufacture all the missing parts.")

with doc.create(LongTable('c|c')) as data_table:
    header_row1 = ["Type", "Quantity"]
    data_table.add_row(header_row1, mapper=[bold])
    data_table.add_hline()
    for row in materials.items():
        data_table.add_row(row)

doc.append(VerticalSpace("4mm"))
doc.append(LineBreak())

"""
3D PRINTS
"""

doc.append(SubTitle(arguments=Arguments('3D Prints')))
doc.append(LineBreak())
doc.append(VerticalSpace("4mm"))

doc.append("List of the parts to be 3D printed")

with doc.create(LongTable("c|c|c|c|c|c")) as data_table:
    header_row1 = ["Part", "Quantity", "Material", "Weight", "Supports", "Layer Height"]
    data_table.add_row(header_row1, mapper=[bold])
    data_table.add_hline()
    for row in info["bom"]["non-sourced"]["3D_printing"]:
        data_table.add_row(row)


doc.append(LineBreak())

dims, preview_file  = renderSTL("Main")

with doc.create(MiniPage(width = str((21-2*margin)/2-0.5)+"cm", align='c')) :
    doc.append(SubSubTitle(arguments=Arguments('Main')))
    doc.append(LineBreak())
    with doc.create(MiniPage(width=str((21-2*margin)/2-0.5)+"cm", align='c')) as preview:
        preview.append(StandAloneGraphic(image_options="width="+str((21-2*margin)/2-2)+"cm",
                                filename=preview_file))
    doc.append(LineBreak())
    doc.append("Width: "+str(dims[0])+ " mm")
    doc.append(LineBreak())
    doc.append("Depth: "+str(dims[1])+ " mm")
    doc.append(LineBreak())
    doc.append("Height: "+str(dims[2])+ " mm")

dims, preview_file = renderSTL("BoxBack")

with doc.create(MiniPage(width = str((21-2*margin)/2-0.5)+"cm", align='c')) :
    doc.append(SubSubTitle(arguments=Arguments('BoxBack')))
    doc.append(LineBreak())
    with doc.create(MiniPage(width=str((21-2*margin)/2-0.5)+"cm", align='c')) as preview:
        preview.append(StandAloneGraphic(image_options="width="+str((21-2*margin)/2-2)+"cm",
                                filename=preview_file))
    doc.append(LineBreak())
    doc.append(LineBreak())
    doc.append("Width: "+str(dims[0])+ " mm")
    doc.append(LineBreak())
    doc.append("Depth: "+str(dims[1])+ " mm")
    doc.append(LineBreak())
    doc.append("Height: "+str(dims[2])+ " mm")

doc.append(VerticalSpace("4mm"))
doc.append(LineBreak())

"""
CNC Machines
"""
doc.append(VerticalSpace("4mm"))

doc.append(SubTitle(arguments=Arguments('CNC Machine')))
doc.append(LineBreak())
doc.append(VerticalSpace("4mm"))

doc.append("List of the parts to be manufactured using a CNC machine")

with doc.create(LongTable("c|c|c|c|c")) as data_table:
    header_row1 = ["Part", "Quantity", "Material", "Width", "Height"]
    data_table.add_row(header_row1, mapper=[bold])
    data_table.add_hline()
    for row in info["bom"]["non-sourced"]["CNC"]:
        data_table.add_row(row)


"""
Laser Cutting
"""

doc.append(SubTitle(arguments=Arguments('Laser Cutting')))
doc.append(LineBreak())

doc.append("List of the parts to laser cut.")

with doc.create(LongTable("c|c|c|c|c")) as data_table:
    header_row1 = ["Part", "Quantity", "Material", "Width", "Height"]
    data_table.add_row(header_row1, mapper=[bold])
    data_table.add_hline()
    #data_table.end_table_header()
    for row in info["bom"]["non-sourced"]["laser-cutting"]:
        data_table.add_row(row)

"""
PDF GENERATION
"""

doc.generate_pdf(os.path.join(dirname, "../InstructionsHardware"), clean_tex=False)