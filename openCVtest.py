import cv2;
import numpy;
vc = cv2.VideoCapture(1)

if vc.isOpened(): # try to get the first frame
    rval, frame = vc.read()
else:
    rval = False
rval, frame = vc.read()
cv2.imwrite("pcb.png", frame);
vc.release()

