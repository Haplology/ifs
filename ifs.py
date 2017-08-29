#!/usr/bin/python 
# -*- coding: utf-8 -*-
### ifs.py
### render a fractal generated with an iterated function system (IFS) to an SVG file
# 
### recommended way to invoke this program:
### while true; do python ifs.py; SVG=$(ls -t ifs*.svg | head -n 1); mogrify -format png "$SVG"; PNG=$(ls -t ifs*.png | head -n 1); convert -resize 1400x1400 -normalize "$PNG" "normalized_""$PNG"; rm headless*.svg; gzip *.svg; sleep 10; done


import random, math, codecs


def iterate_ifs(iteration, point, transform, branches):
    if iteration == iteration_limit:
        for i in range(len(colors)):
            c = colors[i]
            outfile.write('<circle cx="'+
                          str(point[0])+'" '+"\n"
                          'cy="'+
                          str(point[1])+'" '+"\n"
                          'r="0.5" fill="'+c+
                          '" fill-opacity="'+str(float(branches.count(str(i)))/float(len(branches)))+
                          '" />'+
                      "\n")
    else:
        for t in transform:
            p = point
            # first, rotate
            p = (
                p[0]*math.cos(t['rotate']) - p[1]*math.sin(t['rotate']),
                p[1]*math.cos(t['rotate']) + p[0]*math.sin(t['rotate'])
                )
            # second, scale, and third, translate
            p = (p[0]*t['xscale'] + t['xshift']*size, p[1]*t['yscale'] + t['yshift']*size)
            iterate_ifs(iteration+1, p, transform, branches+str(t['index'])*pow(2,iteration))

def random_transform(size):
    rt = []
    for x in range(3):
        rt.append(
            {
                'xscale': round((0.5 + random.random())/2.0, 1), 
                'xshift': round(random.random(), 1), 
                'yscale': round((0.5 + random.random())/2.0, 1), 
                'yshift': round(random.random(), 1), 
                'rotate': round(random.random()*2.0*math.pi, 2),
                'index': x # label for this function, used for coloring
            }
        )
    return rt

def descriptive_filename(transform):
    filename = "headless"
    for t in transform:
        for component in ['rotate', 'xscale', 'yscale', 'xshift', 'yshift']:
            filename += '_' + component + '_' + str(t[component])
    return filename

def find_bounds():
    # scan file just written to find upper and lower bounds
    bounds = {
        'xmin': 1000000.0,
        'xmax': -1000000.0,
        'ymin': 1000000.0,
        'ymax': -1000000.0
    }

    xmatch = '<circle cx="'
    ymatch = 'cy="'
    for line in codecs.open(headless_filename, "r", "utf8"):
        if line.find(xmatch) == 0:
            x = line[len(xmatch):]
            x = x[:x.find('"')]
            x = float(x)
            bounds['xmin'] = min(bounds['xmin'], x)
            bounds['xmax'] = max(bounds['xmax'], x)
        elif line.find(ymatch) == 0:
            y = line[len(ymatch):]
            y = y[:y.find('"')]
            y = float(y)
            bounds['ymin'] = min(bounds['ymin'], y)
            bounds['ymax'] = max(bounds['ymax'], y)
            
    return bounds



# set the scene
iteration_limit = 12
size = float(pow(2, iteration_limit)) 
colors = ["red", "green", "blue"]
examples =[
    [{'rotate': 2.20, 'xscale': 0.7, 'yscale': 0.4, 'xshift': 0.6, 'yshift': 0.2, 'index': 0},
     {'rotate': 6.15, 'xscale': 0.4, 'yscale': 0.6, 'xshift': 0.1, 'yshift': 0.6, 'index': 1},
     {'rotate': 0.57, 'xscale': 0.7, 'yscale': 0.6, 'xshift': 0.7, 'yshift': 0.9, 'index': 2}],
    [{'rotate': 3.77, 'xscale': 0.7, 'yscale': 0.6, 'xshift': 0.9, 'yshift': 0.3, 'index': 0},
     {'rotate': 3.74, 'xscale': 0.4, 'yscale': 0.3, 'xshift': 0.4, 'yshift': 0.0, 'index': 1},
     {'rotate': 5.97, 'xscale': 0.5, 'yscale': 0.5, 'xshift': 0.7, 'yshift': 1.0, 'index': 2}]
    ]
#transform = examples[0]
transform = random_transform(size)
headless_filename = descriptive_filename(transform)+".svg"



# calculate the fractal
# save all of the SVG file except the head
outfile = codecs.open(headless_filename, "a", "utf8")
iterate_ifs(0, (size, size), transform, '')
outfile.write("\n"+'</svg>'+"\n\n")
outfile.close()



# scan the headless SVG file to find the upper and lower bounds,
# use that to calculate the best viewBox
bounds = find_bounds()
width = bounds['xmax'] - bounds['xmin']
height = bounds['ymax'] - bounds['ymin']
center_x = bounds['xmin'] + width*0.5
center_y = bounds['ymin'] + height*0.5
greater_of_height_width = max(height, width)
print "lesser of height or width:", min(height, width)
padding = greater_of_height_width*0.01



# write a second SVG file with head plus the headless SVG file saved just previously 
headed_filename = headless_filename.replace('headless', 'ifs')
outfile = codecs.open(headed_filename, "a", "utf8")
outfile.write('<?xml version="1.0" encoding="UTF-8"?>')
outfile.write("\n\n"+'<svg xmlns="http://www.w3.org/2000/svg" width="100%" height="100%" viewBox="'+
              str(int(round(center_x - greater_of_height_width*0.5 - padding, 0)))+' '+
              str(int(round(center_y - greater_of_height_width*0.5 - padding, 0)))+' '+
              str(int(round(greater_of_height_width + padding*2.0, 0)))+' '+
              str(int(round(greater_of_height_width + padding*2.0, 0)))+
              '" xmlns:xlink="http://www.w3.org/1999/xlink">'+"\n")

# black background
outfile.write("\n\n"+'<rect x="'+
              str(int(round(center_x - greater_of_height_width*0.5 - padding, 0)))+
              '" y="'+
              str(int(round(center_y - greater_of_height_width*0.5 - padding, 0)))+
              '" width="'+
              str(int(round(greater_of_height_width + padding*2.0, 0)))+
              '" height="'+
              str(int(round(greater_of_height_width + padding*2.0, 0)))+
              '" fill="black" />'+"\n\n")

for line in codecs.open(headless_filename, "r", "utf8"):
    outfile.write(line)

outfile.close()
