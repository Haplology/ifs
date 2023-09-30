import random, codecs, math
from scipy import interpolate
NO_FUNCTIONS = 4 # the number of transformations, like 3 or 4
BETWEEN = 800 # the number of frames between key frames
SEQUENCE_LENGTH = 4
ITERATIONS = 13

# make NO_FUNCTIONS copies of a spline for each parameter
# the number of points on the spline is SEQUENCE_LENGTH
# each point is random

splines = {}
for param in ["rotate", "xscale", "yscale", "xshift", "yshift", "red", "green", "blue"]:
    for function_number in range(1,NO_FUNCTIONS+1):
        spline_points = []
        for point_number in range(SEQUENCE_LENGTH-1):
            if param.find("rotate") != -1:
                spline_points.append(random.random()*2.0*math.pi)
            elif param.find("shift") != -1 or param.find("scale") != -1:
                #spline_points.append(random.random()*2.0 - 1.0)
                spline_points.append(random.random()*0.5 + 0.25)
            else:
                spline_points.append(random.randint(0,255))
        # add the first point as the last, so we return to it
        spline_points.append(spline_points[0])
        #print(spline_points)
        # interpolate
        x = range(0, SEQUENCE_LENGTH*BETWEEN, BETWEEN)
        tck = interpolate.splrep(x, spline_points)
        splines[param+str(function_number)] = tck
                                                                                                                         
ifs_calls = []
x = []
y = []
for frame in range(0,(SEQUENCE_LENGTH-1)*BETWEEN):
    command = "~/ifs/ifs "
    for param in ["rotate", "xscale", "yscale", "xshift", "yshift", "red", "green", "blue"]:
        for function_number in range(1,NO_FUNCTIONS+1):
            command += "--" + param + str(function_number) + " "
            command += str(interpolate.splev(frame, splines[param+str(function_number)])) + " "
            if param.find("xscale") != -1 and function_number == 1:
                x.append(frame)
                y.append(interpolate.splev(frame, splines["xscale1"]))
    command += "--iterations "+str(ITERATIONS)
    ifs_calls.append(command)

commands = []
commands.append("#!/bin/zsh")
for frame_number in range((SEQUENCE_LENGTH-1)*BETWEEN):
    head = "frame" + str(frame_number).zfill(4)
    png = head + ".png"
    commands.append('if [ ! -s ./'+png+' ]; then date; '+ifs_calls[frame_number]+ '; convert -verbose -format png -resize 3840x2160 -normalize -flip ifs.bmp '+png+'; fi')

commands.append("ffmpeg -r 30 -i frame%04d.png movie.mp4")
codecs.open("render.sh", "w", "utf8").write("\n".join(commands))
