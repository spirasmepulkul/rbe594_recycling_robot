#!/usr/bin/env python

#by Saraj (Jetro) Pirasmepulkul 3/12/18

"""
This version is an improvement to version 1 (pythonToArduinoPCBCoordinates2.py)
because it is meant to work with stepper motors (does not write to servo angle anymore)
current programmed to work with one stepper motor first
"""

# Simple enough, just import everything from tkinter.
from Tkinter import *
#download and install pillow:
# http://www.lfd.uci.edu/~gohlke/pythonlibs/#pillow
from PIL import Image, ImageTk

#serial stuff
import serial
usbport = '/dev/ttyACM0' #can be '/dev/ttyS0'

ser = serial.Serial(usbport, 9600, timeout=1)


def move(xpos, ypos):
    #once we get x motor to work properly, then we can add ypos to the argument
    #print(xpos, ypos)
    pix2mm = 6 #conversion constant from click value to stepper motor mm coords
    x_coords = int(xpos/pix2mm)
    y_coords = int(ypos/pix2mm)
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

        load = Image.open("pic.png")
        render = ImageTk.PhotoImage(load)

        # labels can be text or images
        img = Label(self, image=render)
        img.image = render
        img.place(x=0, y=0)

    def showImg(self):
        load = Image.open("pic.png")
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





