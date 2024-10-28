from glob import glob
import graphviz
import os

default_dir = 'control_flow_graph'
try:
    os.mkdir()
except:
    pass

for dot_file in glob('**/*.cfg.dot', recursive=True):
    if 'network' in dot_file:
        continue
    output_png = os.path.basename(dot_file).partition('.')[0] + '.png'
    print(output_png)
    graphviz.render('dot', format = 'png', filepath = dot_file, outfile = default_dir + '/' + output_png)
