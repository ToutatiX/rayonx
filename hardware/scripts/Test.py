import os
import sys
import json

dirname = os.path.dirname(__file__)
try:
    info = json.load(open(os.path.join(dirname, "../configs/product.config.json")))
except IOError:
    print(f"File `presets.config` not found :(")
    sys.exit(1)

def computeQuantity(type, part):
    """
    If 3D printed material, we just return the weight times the quantity required, otherwise, we return the area times the quantity
    """
    if type in ["3D_printing"]:
        return part[3] * part[1]
    else:
        return part[3] * part[4] * part[1] *1e-2

materials = dict()

for type in info["bom"]["non-sourced"]:
    for part in info["bom"]["non-sourced"][type]:
        #aterials[info["bom"]["non-sourced"][type][part][2]] = info["bom"]["non-sourced"][type][part][1]
        if part[2] in materials:
            materials[part[2]] += computeQuantity(type, part)
        else:
            materials[part[2]] = computeQuantity(type, part)

for row in materials.items():
    print(row)