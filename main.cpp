#ifdef _WIN32
#include<windows.h>
#endif
#include<stdio.h>
#include<stdlib.h>
#include<glut.h>
#include<math.h>
#define GL_SILENCE_DEPRECATION

#define XMAX 1200
#define YMAX 700
#define SPACESHIP_SPEED 20
#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3


GLint m_viewport[4];
bool mButtonPressed = false;
float mouseX, mouseY;
enum view { INTRO, MENU, INSTRUCTIONS, GAME, GAMEOVER };
view viewPage = INTRO; // initial value
bool keyStates[256] = { false };
bool direction[4] = { false };
bool laser1Dir[2] = { false };
bool laser2Dir[2] = { false };

int alienLife1 = 100;
int alienLife2 = 100;
bool gameOver = false;
float xOne = 500, yOne = 0;
float xTwo = 500, yTwo = 0;
bool laser1 = false, laser2 = false;
GLint CI = 0;
GLfloat a[][2] = { 0,-50, 70,-50, 70,70, -70,70 };
GLfloat LightColor[][3] = { 1,1,0,   0,1,1,   0,1,0 };

void displayRasterText(float x, float y, float z, char* stringToDisplay) {
	glRasterPos3f(x, y, z);
	for (char* c = stringToDisplay; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}
void drawBackground() {
	// Draw stars
	glPointSize(2); 
	glColor3f(1.0, 1.0, 1.0); 
	glBegin(GL_POINTS);
	for (int i = 0; i < 100; ++i) {
		float x = rand() % 2400 - 1200; 
		float y = rand() % 1400 - 700;  
		glVertex2f(x, y);
		float brightness = (rand() % 50) / 100.0 + 0.5; 

		glColor3f(brightness, brightness, brightness); 
		glVertex2f(x, y);
	}

	glEnd();
}

void drawBackground1() {
	// Draw stars
	glPointSize(1); 
	glColor3f(1.0, 1.0, 1.0); 
	glBegin(GL_POINTS);
	for (int i = 0; i < 100; ++i) {
		float x = rand() % 2400 - 1200;
		float y = rand() % 1400 - 700; 
		glVertex2f(x, y);
		float brightness = (rand() % 50) / 100.0 + 0.5; 

		glColor3f(brightness, brightness, brightness); 
		glVertex2f(x, y);
	}

	glEnd();
}
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0);
	glColor3f(1.0, 0.0, 0.0);
	drawBackground();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-1200, 1200, -700, 700);                   
	glMatrixMode(GL_MODELVIEW);
}

void DrawSpaceshipBody1()
{
	//Back Stand
	glPushMatrix();
	glColor3f(0.5, 0, 0.5);
	glTranslated(0, 90, 0); 
	glBegin(GL_TRIANGLES);
	glVertex3f(-15.0, -140.0, 0.0);  
	glVertex3f(15.0, -140.0, 0.0); 
	glVertex3f(0, -220.0, 0.0); 
	glEnd();
	glPopMatrix();
	
	//Body
	glColor3f(0.9, 0, 0.9);
	glPushMatrix();
	glScalef(200, 70, 1); 
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//Lights
	glPushMatrix();
	glScalef(8, 8, 1);
	glTranslated(-20, 0, 0);			//1
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//2
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//3
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//4
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//5
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//6
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//7
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//8
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//9
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);

	glScalef(8, 8, 1);
	glTranslated(-20, 0, 0); 
	glPopMatrix();

	//Right stand
	glPushMatrix();
	glColor3f(0.5, 0, 0.5);  
	glTranslated(0, 90, 0); 
	glBegin(GL_TRIANGLES);
	glVertex3f(90.0, -140.0, 0.0);  
	glVertex3f(120.0, -130.0, 0.0); 
	glVertex3f(145.0, -240.0, 0.0); 
	glEnd();
	glPopMatrix();

	//Left stand
	glPushMatrix();
	glColor3f(0.5, 0, 0.5);
	glTranslated(0, 90, 0); 
	glBegin(GL_TRIANGLES);
	glVertex3f(-90.0, -140.0, 0.0); 
	glVertex3f(-120.0, -130.0, 0.0); 
	glVertex3f(-145.0, -240.0, 0.0);
	glEnd();
	glPopMatrix();
}

void DrawSpaceshipDoom1()
{
	//Doom outer outline
  glColor4f(0.9, 1, 1, 0.0011);
	glPushMatrix();
	glTranslated(0, 90, 0); 
	glScalef(100, 150, 1); 
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//Doom inner outline
  glColor4f(0, 0, 1, 0.0011);
	glPushMatrix();
	glTranslated(0, 90, 0); 
	glScalef(90, 140, 1); 
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//Alien face
  glColor4f(0, 1, 0, 0.0011);
	glPushMatrix();
	glTranslated(0, 130, 0); 
	glScalef(25, 40, -1); 
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//Alien body
  glColor4f(0, 1, 0, 0.0011);
	glPushMatrix();
	glTranslated(0, 40, 0); 
	glScalef(35, 50, -1); 
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//Alien's right eye
  glColor4f(0, 0, 0, 0.0011);
	glPushMatrix();
	glTranslated(-8, 135, 0); 
	glScalef(5, 7, 1); 
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//Alien's left eye
  glColor4f(0, 0, 0, 0.0011);
	glPushMatrix();
	glTranslated(8, 135, 0); 
	glScalef(5, 7, 1); 
	glutSolidSphere(1, 50, 50);
	glPopMatrix();
}

void SpaceshipCreate1(int x, int y) {
	glPushMatrix();
	glTranslated(x, y, 0);
	DrawSpaceshipDoom1();
	glPushMatrix();
	glTranslated(4, 69, 0);
	glPopMatrix();
	DrawSpaceshipBody1();
	glPopMatrix();
}
void introScreen() {
	glClear(GL_COLOR_BUFFER_BIT);

	// Background
	glClearColor(0.0, 0.0, 0.0, 1.0); 
	drawBackground();

	// Center coordinates of the screen
	int centerX = 0;
	int centerY = -150;

	SpaceshipCreate1(centerX, centerY); 
	
	
	glColor3f(1.0, 0.5, 0.0); 
	displayRasterText(-350, 400, 0.0, (char*)"MULTIPLAYER SPACE SHOOTER");

	// Created by text
	glColor3f(1.0, 1.0, 1.0); 
	displayRasterText(-1150, -450, 0.0, (char*)"Created by:");
	glColor3f(0.0, 1.0, 1.0); 
	displayRasterText(-1150, -500, 0.0, (char*)"AARUSHI JAIN(102103483)");
	glColor3f(0.0, 1.0, 1.0);
	displayRasterText(-1150, -550, 0.0, (char*)"VAANI DANG(102283010)");

	glColor3f(1.0, 1.0, 1.0);
	displayRasterText(-300, -550, 0.0, (char*)"Press ENTER to start the game");

	glFlush();
	glutSwapBuffers();
}

void startScreenDisplay()
{
	drawBackground();

	glLineWidth(1);

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//START GAME PLOYGON
	glVertex2f(-200, 300);
	glVertex2f(-200, 400);
	glVertex2f(200, 400);
	glVertex2f(200, 300);
	glEnd();

	glBegin(GL_POLYGON);				//INSTRUCTIONS POLYGON
	glVertex2f(-200, 50);
	glVertex2f(-200, 150);
	glVertex2f(200, 150);
	glVertex2f(200, 50);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
	glVertex2f(-200, -200);
	glVertex2f(-200, -100);
	glVertex2f(200, -100);
	glVertex2f(200, -200);
	glEnd();

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= 150 && mouseY <= 200) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			alienLife1 = alienLife2 = 100;
			viewPage = GAME;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);

	displayRasterText(-100, 340, 0.4, (char*)"Start Game");

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= 30 && mouseY <= 80) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			viewPage = INSTRUCTIONS;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0, 0, 0);
	displayRasterText(-120, 80, 0.4, (char*)"Instructions");

	if (mouseX >= -100 && mouseX <= 100 && mouseY >= -90 && mouseY <= -40) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			mButtonPressed = false;
			exit(0);
		}
	}
	else
		glColor3f(0, 0, 0);
	displayRasterText(-100, -170, 0.4, (char*)"    Quit");
	glutPostRedisplay();
}

void backButton() {
	if (mouseX <= -450 && mouseX >= -500 && mouseY >= -275 && mouseY <= -250) {
		glColor3f(0, 0, 1);
		if (mButtonPressed) {
			viewPage = MENU;
			mButtonPressed = false;
			glutPostRedisplay();
		}
	}
	else glColor3f(1, 1, 0);
	displayRasterText(-1000, -550, 0, (char*)"Back");
}
void instructionsScreenDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Background
	glClearColor(0.0, 0.0, 0.0, 1.0); 
	drawBackground(); 

	glColor3f(1, 1, 0);
	displayRasterText(-900, 550, 0.4, (char*)"INSTRUCTIONS");
	glColor3f(1, 0, 0);
	displayRasterText(-1000, 400, 0.4, (char*)"PLAYER 1");
	displayRasterText(200, 400, 0.4, (char*)"PLAYER 2");
	glColor3f(1, 1, 1);
	displayRasterText(-1100, 300, 0.4, (char*)"Key 'w' to move up.");
	displayRasterText(-1100, 200, 0.4, (char*)"Key 's' to move down.");
	displayRasterText(-1100, 100, 0.4, (char*)"Key 'd' to move right.");
	displayRasterText(-1100, 0, 0.4, (char*)"Key 'a' to move left.");
	displayRasterText(100, 300, 0.4, (char*)"Key 'i' to move up.");
	displayRasterText(100, 200, 0.4, (char*)"Key 'k' to move down.");
	displayRasterText(100, 100, 0.4, (char*)"Key 'j' to move right.");
	displayRasterText(100, 0, 0.4, (char*)"Key 'l' to move left.");
	displayRasterText(-1100, -100, 0.4, (char*)"Key 'c' to shoot, Use 'w' and 's' to change direction.");
	displayRasterText(100, -100, 0.4, (char*)"Key 'm' to shoot, Use 'i' and 'k' to change direction.");
	displayRasterText(-1100, -300, 0.4, (char*)"The Objective is to kill your opponent.");
	displayRasterText(-1100, -370, 0.4, (char*)"Each time a player gets shot, LIFE decreases by 5 points.");
	backButton();
}



void DrawSpaceshipBody(bool isPlayer1)
{
	//Back Stand
	glPushMatrix();
	if (isPlayer1)
		glColor3f(0.7, 0, 0);		
	else
		glColor3f(0.5, 0, 0.5);

	glTranslated(3.6, 43, 0);
	glScalef(0.4, 0.4, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(-15.0, -140.0, 0.0);  
	glVertex3f(15.0, -140.0, 0.0); 
	glVertex3f(0, -220.0, 0.0); 

	glEnd();
	glPopMatrix();

	//Body
	if (isPlayer1)
		glColor3f(1, 0, 0);			
	else
		glColor3f(0.9, 0, 0.9);

	glPushMatrix();
	glScalef(70, 20, 1);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//LIGHTS
	glPushMatrix();							
	glScalef(3, 3, 1);
	glTranslated(-20, 0, 0);			    //1
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//2
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//3
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//4
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//5
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//6
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//7
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//8
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glTranslated(5, 0, 0);					//9
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);

	glPopMatrix();

	//Right Stand
	glPushMatrix();
	if (isPlayer1)
		glColor3f(0.7, 0, 0);		
	else
		glColor3f(0.5, 0, 0.5);

	glTranslated(-2, 43, 0);
	glScalef(0.4, 0.4, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(90.0, -140.0, 0.0);  
	glVertex3f(120.0, -130.0, 0.0); 
	glVertex3f(145.0, -240.0, 0.0); 
	
	glEnd();
	glPopMatrix();

	//Left stand
	glPushMatrix();
	if (isPlayer1)
		glColor3f(0.7, 0, 0);			
	else
		glColor3f(0.5, 0, 0.5);

	glTranslated(25.2, 43, 0);
	glScalef(0.4, 0.4, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(-130.0, -140.0, 0.0);  
	glVertex3f(-160.0, -130.0, 0.0); 
	glVertex3f(-185.0, -240.0, 0.0); 
	glEnd();
	glPopMatrix();
	
}

void DrawSpaceshipDoom()
{
	//Doom outer Outline 
  glColor4f(0.9, 1, 1, 0.0011);
	glPushMatrix();
	glTranslated(0, 30, 0);
	glScalef(35, 50, 1);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//Doom inner outline
  glColor4f(0, 0, 1, 0.0011);
	glPushMatrix();
	glTranslated(0, 30, 0); 
	glScalef(32, 47, 1);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//Alien face
  glColor4f(0, 1, 0, 0.0011);
	glPushMatrix();
	glTranslated(0, 46.5, 0);
	glScalef(10, 15, -1); 
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//Alien body
  glColor4f(0, 1, 0, 0.0011);
	glPushMatrix();
	glTranslated(0, 12, 0); 
	glScalef(15, 20, -1);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();
	
	//Alien's right eye
  glColor4f(0, 0, 0, 0.0011);
	glPushMatrix();
	glTranslated(-2.5, 48, 0); 
	glScalef(2, 3, 1); 
	glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//Alien's left eye
  glColor4f(0, 0, 0, 0.0011);
	glPushMatrix();
	glTranslated(2.5, 48, 0); 
	glScalef(2, 3, 1); 
	glutSolidSphere(1, 50, 50);
	glPopMatrix();
}
void DrawLaser(int x, int y, bool dir[]) {
	int xend = -XMAX, yend = y;
	if (dir[0])
		yend = YMAX;
	else if (dir[1])
		yend = -YMAX;

	// length of the laser
	float length = sqrt(pow(xend - x, 2) + pow(yend - y, 2));
	float gap = 40.0f; 

	//direction vector of the laser
	float dirX = (xend - x) / length;
	float dirY = (yend - y) / length;

	float pulsation = 0.5f + 0.5f * sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0f * 2 * 3.14159f);

	glLineWidth(5);
	glBegin(GL_LINES);
	for (float i = 0; i < length; i += gap) {
		glColor3f(1, 0, 0);
		glVertex2f(x + i * dirX, y + i * dirY);
		glColor3f(1, 0, 0); 
		glVertex2f(x + (i + gap * pulsation) * dirX, y + (i + gap * pulsation) * dirY);
	}
	glEnd();
}


void SpaceshipCreate(int x, int y, bool isPlayer1) {
	glPushMatrix();
	glTranslated(x, y, 0);
	
	DrawSpaceshipDoom();
	glPushMatrix();
	glTranslated(4, 19, 0);

	glPopMatrix();

	DrawSpaceshipBody(isPlayer1);
	
	glEnd();
	glPopMatrix();
}

void DisplayHealthBar1() {
	float healthPercentage = static_cast<float>(alienLife1) / static_cast<float>(100); // Calculate the percentage of remaining health
	float barWidth = 200.0f; 
	float barHeight = 20.0f; 
	float barOffsetX = -1100.0f; 
	float barOffsetY = 600.0f; 

	// Draw background of health bar
	glColor3f(0.2f, 0.2f, 0.2f); // Dark gray color
	glBegin(GL_QUADS);
	glVertex2f(barOffsetX, barOffsetY);
	glVertex2f(barOffsetX + barWidth, barOffsetY);
	glVertex2f(barOffsetX + barWidth, barOffsetY - barHeight);
	glVertex2f(barOffsetX, barOffsetY - barHeight);
	glEnd();

	// Draw health remaining
	glColor3f(0.0f, 1.0f, 0.0f); 
	glBegin(GL_QUADS);
	glVertex2f(barOffsetX, barOffsetY);
	glVertex2f(barOffsetX + barWidth * healthPercentage, barOffsetY);
	glVertex2f(barOffsetX + barWidth * healthPercentage, barOffsetY - barHeight);
	glVertex2f(barOffsetX, barOffsetY - barHeight);
	glEnd();
}

void DisplayHealthBar2() {
	float healthPercentage = static_cast<float>(alienLife2) / static_cast<float>(100); 
	float barWidth = 200.0f; 
	float barHeight = 20.0f; 
	float barOffsetX = 800.0f; 
	float barOffsetY = 600.0f; 

	// Draw background of health bar
	glColor3f(0.2f, 0.2f, 0.2f); 
	glBegin(GL_QUADS);
	glVertex2f(barOffsetX, barOffsetY);
	glVertex2f(barOffsetX + barWidth, barOffsetY);
	glVertex2f(barOffsetX + barWidth, barOffsetY - barHeight);
	glVertex2f(barOffsetX, barOffsetY - barHeight);
	glEnd();

	// Draw health remaining
	glColor3f(0.0f, 1.0f, 0.0f); 
	glBegin(GL_QUADS);
	glVertex2f(barOffsetX, barOffsetY);
	glVertex2f(barOffsetX + barWidth * healthPercentage, barOffsetY);
	glVertex2f(barOffsetX + barWidth * healthPercentage, barOffsetY - barHeight);
	glVertex2f(barOffsetX, barOffsetY - barHeight);
	glEnd();
}

void checkLaserContact(int x, int y, bool dir[], int xp, int yp, bool player1) {
	int xend = -XMAX, yend = y;
	xp += 8; yp += 8; 
	if (dir[0])
		yend = YMAX;
	else if (dir[1])
		yend = -YMAX;

	float m = (float)(yend - y) / (float)(xend - x);
	float k = y - m * x;
	int r = 50;

	//calculating value of b, a, and c needed to find discriminant
	float b = 2 * xp - 2 * m * (k - yp);
	float a = 1 + m * m;
	float c = xp * xp + (k - yp) * (k - yp) - r * r;

	float d = (b * b - 4 * a * c); // discriminant for the equation
	printf("\nDisc: %f x: %d, y: %d, xp: %d, yp: %d", d, x, y, xp, yp);
	if (d >= 0) {
		if (player1)
			alienLife1 -= 5;
		else
			alienLife2 -= 5;

		printf("%d %d\n", alienLife1, alienLife2);
	}
}

void gameScreenDisplay()
{
	DisplayHealthBar1();
	DisplayHealthBar2();
	drawBackground1();
	glScalef(2, 2, 0);

	if (alienLife1 > 0) {
		SpaceshipCreate(xOne, yOne, true);
		if (laser1) {
			DrawLaser(xOne, yOne, laser1Dir);
			checkLaserContact(xOne, yOne, laser1Dir, -xTwo, yTwo, true);
		}
	}
	else {
		viewPage = GAMEOVER;
	}

	if (alienLife2 > 0) {
		glPushMatrix();
		glScalef(-1, 1, 1);
		SpaceshipCreate(xTwo, yTwo, false);
		if (laser2) {
			DrawLaser(xTwo, yTwo, laser2Dir);
			checkLaserContact(xTwo, yTwo, laser2Dir, -xOne, yOne, false);
		}
		glPopMatrix();
	}
	else {
		viewPage = GAMEOVER;
	}

	if (viewPage == GAMEOVER) {
		xOne = xTwo = 500;
		yOne = yTwo = 0;
	}
}

void displayGameOverMessage() {
	glColor3f(1, 1, 0);
	char* message;
	if (alienLife1 > 0)
		message = (char*)"Game Over! Player 1 won the game";
	else
		message = (char*)"Game Over! Player 2 won the game";

	displayRasterText(-350, 600, 0.4, message);
}

void keyOperations() {
	if (keyStates[13] == true && viewPage == INTRO) {
		viewPage = MENU;
		printf("view value changed to %d", viewPage);
		printf("enter key pressed\n");
	}
	if (viewPage == GAME) {
		laser1Dir[0] = laser1Dir[1] = false;
		laser2Dir[0] = laser2Dir[1] = false;
		if (keyStates['c'] == true) {
			laser2 = true;
			if (keyStates['w'] == true) 	laser2Dir[0] = true;
			if (keyStates['s'] == true) 	laser2Dir[1] = true;
		}
		else {
			laser2 = false;
			if (keyStates['d'] == true) xTwo -= SPACESHIP_SPEED;
			if (keyStates['a'] == true) xTwo += SPACESHIP_SPEED;
			if (keyStates['w'] == true) yTwo += SPACESHIP_SPEED;
			if (keyStates['s'] == true) yTwo -= SPACESHIP_SPEED;
		}

		if (keyStates['m'] == true) {
			laser1 = true;
			if (keyStates['i'] == true) laser1Dir[0] = true;
			if (keyStates['k'] == true) laser1Dir[1] = true;
		}
		else {
			laser1 = false;
			if (keyStates['l'] == true) xOne += SPACESHIP_SPEED;
			if (keyStates['j'] == true) xOne -= SPACESHIP_SPEED;
			if (keyStates['i'] == true) yOne += SPACESHIP_SPEED;
			if (keyStates['k'] == true) yOne -= SPACESHIP_SPEED;
		}
	}
}

void display()
{
	keyOperations();
	glClear(GL_COLOR_BUFFER_BIT);

	switch (viewPage)
	{
	case INTRO:
		introScreen();
		break;
	case MENU:
		startScreenDisplay();
		break;
	case INSTRUCTIONS:
		instructionsScreenDisplay();
		break;
	case GAME:
		gameScreenDisplay();
		glScalef(1 / 2, 1 / 2, 0);
		break;
	case GAMEOVER:
		displayGameOverMessage();
		startScreenDisplay();
		break;
	}

	glFlush();
	glLoadIdentity();
	glutSwapBuffers();
}


void passiveMotionFunc(int x, int y) {

	//when mouse not clicked
	mouseX = float(x) / (m_viewport[2] / 1200.0) - 600.0;  //converting screen resolution to ortho 2d spec
	mouseY = -(float(y) / (m_viewport[3] / 700.0) - 350.0);

	//Do calculations to find value of LaserAngle
	glutPostRedisplay();
}

void mouseClick(int buttonPressed, int state, int x, int y) {

	if (buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mButtonPressed = true;
	else
		mButtonPressed = false;
	glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
	keyStates[key] = true;
	glutPostRedisplay();
}

void refresh() {
	glutPostRedisplay();
}

void keyReleased(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 600);
	glutCreateWindow("Multi-Player Space Shooter");
	init();
	glutIdleFunc(refresh);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(passiveMotionFunc);
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	glutDisplayFunc(display);
	glutMainLoop();
}
