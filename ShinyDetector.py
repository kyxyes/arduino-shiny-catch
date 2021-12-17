'''
This Python program runs in the host computer and communicates with the Arduino.
The main objective of this program is to wait for instructions from the arduino, and once the message is received from the Arduino, capture a picture with the webcam and use OpenCV to detect if the Rowlet is shiny or not. Then, the program notifies the Arduino if it's shiny to determine if to soft reset the 3DS.

更新！
抓rayquaza
检查左边是不是白的，确定在稳定的对战画面
确认后检测右边是不是绿的，不是绿的就是shiny

'''

import datetime
import time

import serial
import cv2
import numpy as np

WEBCAM_NUMBER = 0
ARDUINO_PORT = 'COM3'
ARDUINO_BAUD_RATE = 9600
HUMAN_CLASSIFICATION = False
PHOTO_CALIBRATION = False

# coordinates of the pixel in the middle of Rayquaza's body, to be used to determine if the Rayquaza is shiny (black) or not (green)
RAYQUAZA_PIXEL_X = 1350
RAYQYAZA_PIXEL_Y = 298


# coordinates of Togekiss, she is white, determine Togekiss is on left side to make sure we have right screenshot
TOGEKISS_PIXEL_X = 475
TOGEKISS_PIXEL_Y = 314

sensitivity = 15
LOWER_WHITE = np.array([80, 10, 40])
UPPER_WHITE = np.array([100, 80, 255])
LOWER_GREEN = np.array([40, 90, 20])
UPPER_GREEN = np.array([95, 255, 250])

mainFolder = '/Users/yuxinkang/Documents/Arduino/'

def putText(img, x, y): 
    font = cv2.FONT_HERSHEY_SIMPLEX
    b = img[y, x, 0]
    g = img[y, x, 1]
    r = img[y, x, 2]
    cv2.putText(img, '*',
                (x,y), font, 1,
                (255, 0, 0), 2)

    cv2.putText(img, str(r) + ',' +
                str(g) + ',' + str(b),
                (x,y), font, 1,
                (0, 0, 255), 2)
    cv2.imshow('image', img)

def calibrate():
     # initialize camera (we need to do this every time because the camera has a frame buffer and the first frame it gives us is at the time of initialization rather than the time of capture)
        cap = cv2.VideoCapture(WEBCAM_NUMBER)
        
        # set camera gain (to have stable lighting)
        cap.set(15, -2.0)
        
        # capture webcam image
        ret, frame = cap.read()
        
        # release control of camera
        cap.release()
        
        # blur the image a little bit to avoid camera noise or compression artifacts from messing with the algorithm
        frame = cv2.GaussianBlur(frame, (11,11), 0)

        putText(frame, RAYQUAZA_PIXEL_X, RAYQYAZA_PIXEL_Y)

        putText(frame, TOGEKISS_PIXEL_X, TOGEKISS_PIXEL_Y)
                
       
        currentDateTimeString = datetime.datetime.today().strftime('%Y-%m-%dT%H-%M-%S')
        cv2.imwrite(mainFolder + 'img/evidence/' + currentDateTimeString + '_calibrate.png', frame)
        print(mainFolder + 'img/evidence/' + currentDateTimeString + '_calibrate.png')
        print('Done testing images!')


def isItShiny():
    retry = 0
    while True:        
         # initialize camera (we need to do this every time because the camera has a frame buffer and the first frame it gives us is at the time of initialization rather than the time of capture)
        cap = cv2.VideoCapture(WEBCAM_NUMBER)
        
        # set camera gain (to have stable lighting)
        cap.set(15, -2.0)
        
        # capture webcam image
        ret, frame = cap.read()
        
        # release control of camera
        cap.release()
        
        # blur the image a little bit to avoid camera noise or compression artifacts from messing with the algorithm
        frame = cv2.GaussianBlur(frame, (11,11), 0)
        
        # Convert BGR to HSV
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        currentDateTimeString = datetime.datetime.today().strftime('%Y-%m-%dT%H-%M-%S')

        # Threshold the HSV image to get a black and white image that is white (255) where the original image is blue and white (0) if not.
        mask_white = cv2.inRange(hsv, LOWER_WHITE, UPPER_WHITE)
        mask_green = cv2.inRange(hsv, LOWER_GREEN, UPPER_GREEN)

        cv2.imwrite(mainFolder + 'img/evidence/' + currentDateTimeString + '_frame.png', frame)

        # Apply a bitwise-AND between the mask and the original image
        res_white = cv2.bitwise_and(frame,frame, mask= mask_white)
        cv2.imwrite(mainFolder + 'img/evidence/' + currentDateTimeString + '_mask_white.png', mask_white)
        cv2.imwrite(mainFolder + 'img/evidence/' + currentDateTimeString + '_res_white.png', res_white)

        res_green = cv2.bitwise_and(frame,frame, mask= mask_green)
        cv2.imwrite(mainFolder + 'img/evidence/' + currentDateTimeString + '_mask_green.png', mask_green)
        cv2.imwrite(mainFolder + 'img/evidence/' + currentDateTimeString + '_res_green.png', res_green)
        print(mainFolder + 'img/evidence/' + currentDateTimeString + '_frame.png')

        if HUMAN_CLASSIFICATION:
            isShiny = input('Is this Pokemon shiny? [y/n]')
            return isShiny == 'y'
        else:
            # if left side is not white (togekiss is not showing up), we need to retry
            if mask_white[TOGEKISS_PIXEL_Y, TOGEKISS_PIXEL_X] == 0:
                print('Togekiss is not showing up on left side of screen! we will take another webcam picture and try again')
                time.sleep(1)
                retry = retry + 1
                if retry == 3:
                    print('Togekiss is not showing up on left side of screen! Can not check shiny! Need manully reset')
                    return False
                else:
                    continue
            else:
                print('Togekiss is showing up!')
                if mask_green[RAYQYAZA_PIXEL_Y, RAYQUAZA_PIXEL_X] == 0:
                    print('Can`t detect green color, It is a black Rayquaza and it is shiny')
                    return True
                else:
                    print('Detect green color It is a regular Rayquaza')
                    return False

if __name__ == '__main__':
    # ser = serial.Serial(ARDUINO_PORT, ARDUINO_BAUD_RATE)
    # while True:
    #     # Empty line
    #     print()

    #     # Wait for message from Arduino
    #     #message = b'Command: checkIfShiny\r\n'
    #     message = ser.readline()
    #     print("Message from Arduino: ")
    #     print(message)
        if PHOTO_CALIBRATION: 
            calibrate()
        else: 
    
    #     if message == b'Command: checkIfShiny\r\n':
            if isItShiny():
                print('Shiny!')
     #           ser.write(b'y')
            else:
                print('Not shiny')
      #          ser.write(b'n')
        # else:
        #     '''
        #     Do nothing
        #     '''