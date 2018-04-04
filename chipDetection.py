#!/usr/bin/env python

import cv2;

"""
image = cv2.imread('pcb.png')
gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
cv2.imwrite('gray_image.png', gray_image)

cv2.imshow('color_image', image)
cv2.imshow('gray_image',gray_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
"""
import numpy as np
import cv2
from matplotlib import pyplot as plt

im = cv2.imread('pcb.png')
#gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
#ret, thresh = cv2.threshold(gray, 80, 255, 0)
#blur = cv2.bilateralFilter(img,9,75,75)

kernel = np.ones((5,5),np.float32)/25
dst = cv2.filter2D(im,-1,kernel)
# find all the 'black' shapes in the image
lower = np.array([0, 0, 0])
upper = np.array([100, 100, 100])
shapeMask = cv2.inRange(dst, lower, upper)

(cnts, _) = cv2.findContours(shapeMask.copy(), cv2.RETR_EXTERNAL,
    cv2.CHAIN_APPROX_SIMPLE)
print "I found %d black shapes" % (len(cnts))

for c in cnts:
    cv2.drawContours(im, [c], -1, (0, 255, 0), 2)

cv2.imshow('shapemask', shapeMask)
cv2.imshow('contours', im)
cv2.waitKey(0) 
