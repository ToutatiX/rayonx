import os
import pandas as pd
from markdownTable import markdownTable

dirname = os.path.dirname(__file__)
df = pd.read_csv(os.path.join(dirname, "../BillOfMaterials.csv"))
md = markdownTable(df.to_dict(orient='records')).setParams(row_sep = 'markdown', padding_weight = 'centerleft').getMarkdown()

f = open(os.path.join(dirname, "../README.md"), "w")
f.write(md)
f.close()