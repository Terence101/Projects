import pygame
import random

pygame.init()

white = (255,255,255)
black = (0,0,0)
red = (255,0,0)
green = (0,155,0)
yellow = (255,255,0)
blue = (0,0,200)

display_width = 600
display_height  = 500

gameDisplay = pygame.display.set_mode((display_width,display_height))
pygame.display.set_caption('Slither')

icon = pygame.image.load('apple2.png')
pygame.display.set_icon(icon)

img = pygame.image.load('snakehead.png')
appleimg = pygame.image.load('apple.png')

clock = pygame.time.Clock()

AppleThickness = 16
block_size = 15
FPS = 10

direction = "right"

smallfont = pygame.font.SysFont("comicsansms", 25)
medfont = pygame.font.SysFont("comicsansms", 50)
largefont = pygame.font.SysFont("comicsansms", 80)

def controls():

    control = True

    while control:


        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        gameDisplay.fill(white)

        message_to_screen("Controls",green,-100,"medium")

        message_to_screen("RIGHT: -->   LEFT: <--",black,-30)

        message_to_screen("UP: (up key)   DOWN: (down key)",black,20)

        message_to_screen("PAUSE GAME: (P) ",black,80)

        buttons("play",green, 70,400,100,50)
        #buttons("Main menu", yellow, 230,400,150,50)
        buttons("quit", red, 430,400,100,50)
        clock.tick(5)

        pygame.display.update()


def pause():

    paused = True
    message_to_screen("Paused",
                      black,
                      -100,
                      size="medium")

    message_to_screen("Press C to continue or Q to quit.",
                      black,
                      25)
    pygame.display.update()

    while paused:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_c:
                    paused = False

                elif event.key == pygame.K_q:
                    pygame.quit()
                    quit()

        #gameDisplay.fill(white)
        
        clock.tick(5)

def gameOver():
    over = True

    while over:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        gameDisplay.fill(white)
        message_to_screen("Game over",red,y_displace=-50,size="medium")
        message_to_screen("YOU LOSE",green,y_displace=0)
        
        buttons("replay",green, 70,400,100,50)
        buttons("controls", yellow, 230,400,120,50)
        buttons("quit", red, 430,400,100,50)

        pygame.display.update()
              

def score(score):
    text = smallfont.render("Score: "+str(score), True, black)
    gameDisplay.blit(text, [0,0])

def randAppleGen():
    randAppleX = round((random.randrange(0, display_width-AppleThickness))/10.0)*10.0
    randAppleY = round((random.randrange(0, display_height-AppleThickness))/10.0)*10.0

    return randAppleX,randAppleY



def game_intro():

    intro = True

    while intro:

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

   
        gameDisplay.fill(white)
        message_to_screen("Welcome to Snake",
                          green,
                          -100,
                          "medium")
        message_to_screen("The objective of the game is to eat red apples",
                          black,
                          -30)

        message_to_screen("The more apples you eat, the longer you get",
                          black,
                          10)

        message_to_screen("If you run into yourself, or the edges, you lose!",
                          black,
                          50)
        buttons("play",green, 70,400,100,50)
        buttons("controls", yellow, 230,400,120,50)
        buttons("quit", red, 430,400,100,50)
    
        pygame.display.update()
        clock.tick(15)
        
        


def snake(block_size, snakelist):

    if direction == "right":
        head = pygame.transform.rotate(img, 270)
        gameDisplay.blit(head, (snakelist[-1][0]+15, snakelist[-1][1]))

    if direction == "left":
        head = pygame.transform.rotate(img, 90)
        gameDisplay.blit(head, (snakelist[-1][0]-15, snakelist[-1][1]))

    if direction == "up":
        head = img
        gameDisplay.blit(head, (snakelist[-1][0], snakelist[-1][1]-15))

    if direction == "down":
        head = pygame.transform.rotate(img, 180)
        gameDisplay.blit(head, (snakelist[-1][0], snakelist[-1][1]+15))
        
    
    
    for XnY in snakelist:
        pygame.draw.rect(gameDisplay, green, [XnY[0],XnY[1],block_size,block_size])

def text_objects(text,color,size):
    if size == "small":
        textSurface = smallfont.render(text, True, color)
    elif size == "medium":
        textSurface = medfont.render(text, True, color)
    elif size == "large":
        textSurface = largefont.render(text, True, color)

    
    return textSurface, textSurface.get_rect()
    

def buttons(text,color,x,y,width,height):
    current = pygame.mouse.get_pos()
    click = pygame.mouse.get_pressed()

    if x+width > current[0] > x and y+height > current[1] > y:
        #pygame.draw.rect(gameDisplay, blue, (x,y,width,height))

        if click[0] == 1:

            if text == "play" or text == "replay":
                gameLoop()
            elif text == "controls":
                controls()
            elif text == "quit":
                pygame.quit()
                quit()

    #else:
    pygame.draw.rect(gameDisplay, color, (x,y,width,height))
    
    textSurf, textRect = text_objects(text,black,"small")
    textRect.center = x+50, y+20
    gameDisplay.blit(textSurf,textRect)


def message_to_screen(msg,color, y_displace=0, size = "small"):
    textSurf, textRect = text_objects(msg,color, size)
    textRect.center = (display_width / 2), (display_height / 2)+y_displace
    gameDisplay.blit(textSurf, textRect)


def gameLoop():
    global direction

    direction = 'right'
    gameExit = False

    lead_x = display_width/2
    lead_y = display_height/2

    lead_x_change = 10
    lead_y_change = 0

    snakeList = []
    snakeLength = 1


    randAppleX,randAppleY = randAppleGen()
    
    while not gameExit:
 
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                gameExit = True
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    direction = "left"
                    lead_x_change = -block_size
                    lead_y_change = 0
                elif event.key == pygame.K_RIGHT:
                    direction = "right"
                    lead_x_change = block_size
                    lead_y_change = 0
                elif event.key == pygame.K_UP:
                    direction = "up"
                    lead_y_change = -block_size
                    lead_x_change = 0
                elif event.key == pygame.K_DOWN:
                    direction = "down"
                    lead_y_change = block_size
                    lead_x_change = 0

                elif event.key == pygame.K_p:
                    pause()

        if lead_x >= display_width or lead_x < 0 or lead_y >= display_height or lead_y < 0:
            gameOver()
      

        lead_x += lead_x_change
        lead_y += lead_y_change

        gameDisplay.fill(white)
        
        gameDisplay.blit(appleimg, (randAppleX, randAppleY))


        snakeHead = []
        snakeHead.append(lead_x)
        snakeHead.append(lead_y)
        snakeList.append(snakeHead)

        if len(snakeList) > snakeLength:
            del snakeList[0]

        for eachSegment in snakeList[:-1]:
            if eachSegment == snakeHead:
                gameOver()

        
        snake(block_size, snakeList)

        score(snakeLength-1)

        
        pygame.display.update()

        if lead_x > randAppleX and lead_x < randAppleX + AppleThickness or lead_x + block_size > randAppleX and lead_x + block_size < randAppleX + AppleThickness:

            if lead_y > randAppleY and lead_y < randAppleY + AppleThickness:

                randAppleX,randAppleY = randAppleGen()
                snakeLength += 1

            elif lead_y + block_size > randAppleY and lead_y + block_size < randAppleY + AppleThickness:

                randAppleX,randAppleY = randAppleGen()
                snakeLength += 1


        clock.tick(FPS)
        
    pygame.quit()
    quit()

game_intro()
gameLoop()
