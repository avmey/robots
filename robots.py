import sys, pygame, serial
from pygame.locals import *

running = True
up = down = left = right = 0
ser = serial.Serial('com3',115200)

while running:
    for event in pygame.event.get():
        if not hasattr(event, 'key'): continue
        if event.key == K_UP: up = 1
        if event.key == K_DOWN: down = 2
        if event.key == K_LEFT: left = 4
        if event.key == K_RIGHT: right = 8

    num = up + down + left + right
    hexNum = chr(num)

    ser.write(hexNum)
