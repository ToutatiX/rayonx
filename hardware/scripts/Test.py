from os.path import join
import matplotlib.font_manager as font_manager

custom_fonts_paths = "/Users/eliottsarrey/rayonx/hardware/scripts/fonts/ClassicalGaramondRegular.otf"

for font in font_manager.findSystemFonts(custom_fonts_paths):
    font_manager.fontManager.addfont(font)


class FixedFontManager(font_manager.FontManager):
    def __init__(self, size=None, weight='normal'):
        super().__init__(size, weight)
        for font in font_manager.findSystemFonts(custom_fonts_paths):
            self.addfont(font)


font_manager.FontManager = FixedFontManager