import os
from pylatex import Document, LongTable, MultiColumn, MiniPage, LineBreak, VerticalSpace, StandAloneGraphic
from pylatex.base_classes import Environment, CommandBase, Arguments
from pylatex.package import Package
from pylatex import Document, Section, UnsafeCommand, LongTabu, Tabu, Center
from pylatex.utils import NoEscape, bold

dirname = os.path.dirname(__file__)

geometry_options = {
    "margin": "2.54cm",
    "includeheadfoot": False
}
doc = Document(page_numbers=True, geometry_options=geometry_options)


"""
New class for the subtitles, black box around a white text 
"""

class SubTitle(CommandBase):
    _latex_name = 'subtitle'
    packages = [Package('color')]

new_comm = UnsafeCommand('newcommand', '\subtitle', options=1,
                             extra_arguments=r'\noindent\colorbox{black}{\Large \textcolor{white}{\textbf{\MakeUppercase{#1}}}}\\ \\')

doc.append(new_comm)


"""
HEADER SECTION WITH LOGO AND HUGE TITLE
"""

with doc.create(MiniPage(align='c')) as logo_wrapper:
            logo_file = os.path.join(dirname, './public/Logo.png')
            logo_wrapper.append(StandAloneGraphic(image_options="width=25mm",
                                filename=logo_file))

doc.append(LineBreak())

with doc.create(MiniPage(align='c')):
    with doc.create(MiniPage(align='l')):
        doc.append("Hardware Instructions")
    with doc.create(MiniPage(align='r')):
        doc.append("toutatix.fr")
            


doc.append(LineBreak())

"""
SOURCED COMPONENTS SECTION
"""

doc.append(SubTitle(arguments=Arguments('Sourced Components')))

doc.append("Components to order from a local reseller. Those components are in every RayonX entity and some of them are common in ToutatiX's products.")

with doc.create(LongTabu("X[r] X[r] X[r] X[r] X[r] X[r]")) as data_table:
    header_row1 = ["Prov", "Num", "CurBal", "IntPay", "Total", "IntR"]
    data_table.add_row(header_row1, mapper=[bold])
    data_table.add_hline()
    data_table.end_table_header()
    row = ["PA", "9", "$100", "%10", "$1000", "Test"]
    for i in range(5):
        data_table.add_row(row)
        

"""
RAW MATERIALS SECTION
"""

doc.append(SubTitle(arguments=Arguments('Raw Materials')))

doc.append("Raw materials to have in order to be able to manufacture all the missing parts.")

with doc.create(LongTabu("X[r] X[r] X[r] X[r] X[r] X[r]")) as data_table:
    header_row1 = ["Prov", "Num", "CurBal", "IntPay", "Total", "IntR"]
    data_table.add_row(header_row1, mapper=[bold])
    data_table.add_hline()
    data_table.end_table_header()
    row = ["PA", "9", "$100", "%10", "$1000", "Test"]
    for i in range(5):
        data_table.add_row(row)


"""
PDF GENERATION
"""

doc.generate_pdf(os.path.join(dirname, "../InstructionsHardware"), clean_tex=True)
