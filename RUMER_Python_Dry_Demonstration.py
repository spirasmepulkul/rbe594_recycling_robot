#!/usr/bin/env python

#by Saraj (Jetro) Pirasmepulkul 4/19/18

"""
This is the final version of the python program to run RUMER. simply open up a terminal, type python <name_of_file.py> and press enter
If the system is powered on, ideally before you run this script, the system will begin homing itself. 

This program can be scaled up to incorporate more CV and AI techniques to detect the chips on the PCB
"""

import cv2;
import numpy;

# Simple enough, just import everything from tkinter.
from Tkinter import *
#download and install pillow:
# http://www.lfd.uci.edu/~gohlke/pythonlibs/#pillow
from PIL import Image, ImageTk

#serial stuff
import serial
usbport = '/dev/ttyACM0' #check arduino "COM port"

ser = serial.Serial(usbport, 9600, timeout=1)

homing = True;
while homing:
	message = ser.readline()
	print(message)
	if (message == 'home'):
		homing = False
	


#openCV stuff
vc = cv2.VideoCapture(1)
width = 1366#1024#1280
height = 768#576#720
vc.set(3,width)
vc.set(4,height)

if vc.isOpened(): # try to get the first frame
    rval, frame = vc.read()
else:
    rval = False
rval, frame = vc.read()
cv2.imwrite("pcb.png", frame);
vc.release()

def move(xpos, ypos):
    #once we get x motor to work properly, then we can add ypos to the argument
    #print(xpos, ypos)
    x_pix2mm = 3#5.2#14.5 #conversion constant from click value to stepper motor mm coords
    y_pix2mm = 3#4.1#2
    x_offset = 197 #pixels
    y_offset = 28 #pixels
    x_coords = int(((xpos-x_offset)/x_pix2mm)-18)
    y_coords =  int(((ypos-y_offset)/y_pix2mm)-18)
    coords = str(x_coords) + "," + str(y_coords)
    print(coords)
    if ((0 <= x_coords <= 220) and (0 <= y_coords <= 220)):
        
        ser.write(str(coords).encode('utf-8'))
        
    else:
        print("IC location out of range.\n")

# Here, we are creating our class, Window, and inheriting from the Frame
# class. Frame is a class from the tkinter module. (see Lib/tkinter/__init__)
class Window(Frame):

    # Define settings upon initialization. Here you can specify
    def __init__(self, master=None):
        
        # parameters that you want to send through the Frame class. 
        Frame.__init__(self, master)   

        #reference to the master widget, which is the tk window                 
        self.master = master

        #with that, we want to then run init_window, which doesn't yet exist
        self.init_window()

    #Creation of init_window
    def init_window(self):

        # changing the title of our master widget      
        self.master.title("GUI")

        # allowing the widget to take the full space of the root window
        self.pack(fill=BOTH, expand=1)

        # creating a menu instance
        menu = Menu(self.master)
        self.master.config(menu=menu)

        # create the file object)
        file = Menu(menu)

        # adds a command to the menu option, calling it exit, and the
        # command it runs on event is client_exit
        file.add_command(label="Exit", command=self.client_exit)

        #added "file" to our menu
        menu.add_cascade(label="File", menu=file)


        # create the file object)
        edit = Menu(menu)

        load = Image.open("pcb.png")
        render = ImageTk.PhotoImage(load)

        # labels can be text or images
        img = Label(self, image=render)
        img.image = render
        img.place(x=0, y=0)

    def showImg(self):
        load = Image.open("pcb.png")
        render = ImageTk.PhotoImage(load)

        # labels can be text or images
        img = Label(self, image=render)
        img.image = render
        img.place(x=0, y=0)

    def client_exit(self):
        exit()


    def get_mouse_click_coor(x,y):
        return(x,y)
        #print(x,y)

# root window created. Here, that would be the only window, but
# you can later have windows within windows.
root = Tk()

root.geometry("400x300")

#creation of an instance
app = Window(root)

def f(event):
    xpos = event.x
    ypos = event.y
    #print(xpos)
    move(xpos, ypos) #this is what actually sends to Arduino serial communication

root.bind("<Button-1>", f)
#mainloop 
root.mainloop()





