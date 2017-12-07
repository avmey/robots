import pygame, serial, time, sys
from pygame import *
from time import sleep

ser = serial.Serial('/dev/cu.linvor-DevB',9600)
pygame.init()
screen = pygame.display.set_mode((200,200))
sendarr = bytearray(8)
sendarr[0] = 13
clock = pygame.time.Clock()
i = 1

while True:
    deltat = clock.tick(10)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                sendarr[i] = 0
                ser.write(sendarr)
                sys.exit()
            elif event.key == K_1: sendarr[i] = 1
            elif event.key == K_2: sendarr[i] = 2
            elif event.key == K_3: sendarr[i] = 3
            elif event.key == K_4: sendarr[i] = 4
            elif event.key == K_5: sendarr[i] = 5
            elif event.key == K_6: sendarr[i] = 6
            i += 1
        
