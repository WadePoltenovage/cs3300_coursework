import pygame
pygame.init()

#Jumping and Boundaries

win = pygame.display.set_mode((500, 500)) #window

pygame.display.set_caption("First Game")    #name of game/window

screenWidth = 500

x = 50 
y = 50
width = 40 #character width
height = 60
vel = 5 #how fast yr character move

isJump = False
jumpCount = 10

run = True
while run:
    pygame.time.delay(100) #time delay, like a clock

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    keys = pygame.key.get_pressed()

    if keys[pygame.K_LEFT] and x > vel:
        x -= vel
    if keys[pygame.K_RIGHT] and x < screenWidth - width:
        x += vel
    if not (isJump):
        if keys[pygame.K_UP] and y > vel:
            y -= vel
        if keys[pygame.K_DOWN] and y < screenWidth - height - vel:
            y += vel
        if keys[pygame.K_SPACE]:
            isJump = True
    else:
        if jumpCount >= -10:
            neg = 1
            if jumpCount < 0:
                neg = -1
            y -= (jumpCount ** 2) * 0.5 * neg
            jumpCount -= 1
        else:
            isJump = False
            jumpCount = 10


    win.fill((0,0,0)) #without this line, you will end up drawing on screen
    pygame.draw.rect(win, (255, 0, 0), (x, y, width, height))
    pygame.display.update() #must do this, else nothing on screen/is updated

pygame.quit()
