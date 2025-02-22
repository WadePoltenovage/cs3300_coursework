import pygame
pygame.init()

#BASIC MOVEMENTS AND KEY PRESSES

win = pygame.display.set_mode((500, 500)) #window

pygame.display.set_caption("First Game")    #name of game/window

x = 50 
y = 50
width = 40
height = 60
vel = 5 #how fast yr character move

run = True
while run:
    pygame.time.delay(100) #time delay, like a clock

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    keys = pygame.key.get_pressed()

    if keys[pygame.K_LEFT]:
        x -= vel
    if keys[pygame.K_RIGHT]:
        x += vel
    if keys[pygame.K_UP]:
        y -= vel
    if keys[pygame.K_DOWN]:
        y += vel

    win.fill((0,0,0)) #without this line, you will end up drawing on screen
    pygame.draw.rect(win, (255, 0, 0), (x, y, width, height))
    pygame.display.update() #must do this, else nothing on screen/is updated

pygame.quit()
