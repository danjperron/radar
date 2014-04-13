#!/usr/bin/python
import Tkinter as tk
from Tkinter import Button
import math
from collections import deque

import serial
#import sys

class Reading:
	x = None
	y = None
	lastX = None
	lastY = None
	angleRead = None
	lastAngleRead = None
	line = None
	values = deque()
	lastLines = deque()
	#x2 = None
	#y2 = None

	# The class "constructor" - It's actually an initializer 
	#def __init__(self, x1, y1, x2, y2):
	#	self.x1 = x1
	#	self.y1 = y1
	#	self.x2 = x2
	#	self.y2 = y2

center = 200
size = 400
rangeMax = 4.0
lastAngleRead = 0.0
#lastLines = deque()

root = tk.Tk()
root.title('Radar')
canvas = tk.Canvas(root)
canvas.pack(expand=True, fill=tk.BOTH)
bg_circle = canvas.create_oval(0, 0, 600, 600, width=4, fill='darkgreen', outline='green')
bg_1m_circle = canvas.create_oval(0, 0, 600, 600, width=1, outline='green')
bg_2m_circle = canvas.create_oval(0, 0, 600, 600, width=1, outline='green')
bg_3m_circle = canvas.create_oval(0, 0, 600, 600, width=1, outline='green')

#readingValues = dict()

readings = [Reading(), Reading()]

def clear():
	canvas.delete('lines')

b = Button(root, text="Reset", command=clear)
b.pack()

def updateRadar(radar, direction, value):
	canvas.delete('updateLine')
	canvas.delete('center')
	if lastAngleRead != readings[radar].angleRead:
		if readings[radar].angleRead == None:
			readings[radar].angleRead = 0.0
		readings[radar].lastAngleRead = readings[radar].angleRead
		readings[radar].lastX = readings[radar].x
		readings[radar].lastY = readings[radar].y
		readings[radar].values = deque()
		readings[radar].line = None
		canvas.delete((`min(lastAngleRead, readings[radar].lastAngleRead)` + "" + `max(lastAngleRead, readings[radar].lastAngleRead)`))
	
	readings[radar].values.append(value)
	value = sum(readings[radar].values) / len(readings[radar].values)
		
	x = center + (value / rangeMax) * math.cos(math.radians(lastAngleRead - 90)) * (size - 20) / 2.0
	y = center + (value / rangeMax) * math.sin(math.radians(lastAngleRead - 90)) * (size - 20) / 2.0
	#if len(readings[radar].lastLines) > 30:
	#	canvas.delete(readings[radar].lastLines.popleft())

	if readings[radar].line == None:
		try:
			readings[radar].line = canvas.create_line(readings[radar].lastX, readings[radar].lastY, x, y, width=4, fill='red', tags=('lines', `min(lastAngleRead, readings[radar].lastAngleRead)` + "" + `max(lastAngleRead, readings[radar].lastAngleRead)`))
		except:
			readings[radar].line = canvas.create_line(x, y, x, y, width=4, fill='red', tags=('lines', `min(lastAngleRead, readings[radar].lastAngleRead)` + "" + `max(lastAngleRead, readings[radar].lastAngleRead)`))
		print canvas.gettags(readings[radar].line)
		
		#readings[radar].lastLines.append(readings[radar].line)
		
	else:
		canvas.coords(readings[radar].line, readings[radar].lastX, readings[radar].lastY, x, y)
	readings[radar].x = x
	readings[radar].y = y
	
	readings[radar].angleRead = lastAngleRead

	canvas.create_line(center - math.cos(math.radians(lastAngleRead - 90)) * (size - 20) / 2.0, center - math.sin(math.radians(lastAngleRead - 90)) * (size - 20) / 2.0, center + math.cos(math.radians(lastAngleRead - 90)) * (size - 20) / 2.0, center + math.sin(math.radians(lastAngleRead - 90)) * (size - 20) / 2.0, width=1, fill='green', tags=('lines', 'updateLine'))
	canvas.create_oval(center - 4, center - 4, center + 4, center + 4, width=0, fill='green', tags=('lines', 'center'))
	

def resize(event):
	global size, center 
	size = min(event.width, event.height)
	center = size / 2
	canvas.delete('lines')
	canvas.coords(bg_circle, 10, 10, size - 10, size - 10)
	radius = (size - 20) / 2.0 / rangeMax
	canvas.coords(bg_1m_circle, center - 1 * radius, center - 1 * radius, center + 1 * radius, center + 1 * radius)
	canvas.coords(bg_2m_circle, center - 2 * radius, center - 2 * radius, center + 2 * radius, center + 2 * radius)
	canvas.coords(bg_3m_circle, center - 3 * radius, center - 3 * radius, center + 3 * radius, center + 3 * radius)
	#redraw()

canvas.bind("<Configure>", resize)

ser = serial.Serial('/dev/ttyUSB0', 9600)
while True:
	read = ser.readline().split(',')
	if len(read) == 4:
		try:
			lastAngleRead = float(read[2])
			updateRadar(int(read[0]), int(read[1]), float(read[3].rstrip('\r\n')))
			#if float(read[0]) in readingValues:
			#	readingValues[float(read[0])].append(float(read[1].rstrip('\r\n')))
			#else:
			#	readingValues[float(read[0])] = deque([0.0,0.0,float(read[1].rstrip('\r\n'))], 3)
		except ValueError, e:
			continue
	root.update()	
	
