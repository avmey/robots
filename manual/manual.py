import pygame, serial, time
from pygame import *
from time import sleep

running = True
up = down = left = right = esc = False
num = 0
ser = serial.Serial('/dev/cu.linvor-DevB', 9600)
pygame.init()
screen = pygame.display.set_mode((200,200))
sendarr = bytearray(2)
sendarr[0] = 13
clock = pygame.time.Clock()

while running:
    num = 0

    deltat = clock.tick(10)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
        elif event.type==KEYDOWN:
            if event.key==K_UP: up = True
            if event.key==K_DOWN: down = True
            if event.key==K_LEFT: left = True
            if event.key==K_RIGHT: right = True
            if event.key==K_ESCAPE: esc = True
        elif event.type==KEYUP:
            if event.key==K_UP: up = False
            if event.key==K_DOWN: down = False
            if event.key==K_LEFT: left = False
            if event.key==K_RIGHT: right = False
            if event.key==K_ESCAPE: esc = False

    if up: num += 1
    if down: num += 2
    if left: num += 4
    if right: num += 8
    if esc: num += 16

    if(num != 0):
        sendarr[1] = num
        ser.write(sendarr)
