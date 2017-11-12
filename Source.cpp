#include "includes.h"

#define getColour(colour) colour.aa/255.0f, colour.bb/255.0f, colour.cc/255.0f
#define MOVING 0
#define FALLING 1
#define GAP 10.0f

float yOffset = 0.0f;
float height = 20.0f;
bool gameOver = false;
float X_SPEED = 2.0f;
float Y_SPEED = 0.0f;
float Y_ACC = -0.4f;
float rotateY = 45.0f;
int streak = 0;
int score = 0;
float increment = 10.0f;
float scale = 1.0f;
float ysoff = 100.0f;

class Colour {
public:
	//the rgb or hsv components
	GLfloat aa, bb, cc;
	//0 if hsv, 1 if rgb
	GLboolean rgb;

	Colour() {
		aa = bb = cc = 0.0f;
		rgb = 1;
	}

	Colour(GLfloat a, GLfloat b, GLfloat c, GLboolean rgb=1) {
		this->aa = a;
		this->bb = b;
		this->cc = c;
		this->rgb = rgb;
	}
	Colour getRGB() {
		if (rgb == 0) {
			GLfloat r, g, b;
			HSVtoRGB(&r, &g, &b, aa, bb, cc);
			Colour c(r, g, b, 1);
			return c;
		}
		return (*this);
	}

	Colour getHSV() {
		if (rgb == 1) {
			GLfloat h, s, v;
			RGBtoHSV(aa, bb, cc, &h, &s, &v);
			Colour c(h, s, v, 0);
			return c;
		}
		return (*this);
	}
};

class Block {
public:
	Block() {
		x = y = z = width = depth = 0.0f;
	}

	GLfloat width, depth;
	GLfloat x, y, z;
	Colour top, left, right;

	void setPos(GLfloat x, GLfloat y, GLfloat z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void setDim(GLfloat w, GLfloat d) {
		width = w;
		depth = d;
	}

	void setColour(Colour c) {
		top = c;
		Colour hsv = c.getHSV();
		hsv.cc -= 40.0f;
		left = hsv.getRGB();
		hsv = c.getHSV();
		hsv.cc -= 80.0f;
		right = hsv.getRGB();
	}

	void draw() {
		glLoadIdentity();
		//glRotatef(35.264f, 1.0f, 0.0f, 0.0f);
		//glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
		glTranslatef(x, y+yOffset, z);
		//glTranslatef(-width/2.0f, 0.0f, -depth/2.0f);
		glScalef(width, 1.0f, depth);
		glBegin(GL_QUADS);
			glColor3f(getColour(top));
			glVertex3f(1.0f, 0, 0.0f);
			glVertex3f(0.0f, 0, 0.0f);
			glVertex3f(0.0f, 0, 1.0f);
			glVertex3f(1.0f, 0, 1.0f);

			glColor3f(getColour(right));
			glVertex3f(1.0f, 0, 1.0f);
			glVertex3f(0.0f, 0, 1.0f);
			glVertex3f(0.0f, -height, 1.0f);
			glVertex3f(1.0f, -height, 1.0f);

			glColor3f(getColour(left));
			glVertex3f(0.0f, 0, 1.0f);
			glVertex3f(0.0f, 0, 0.0f);
			glVertex3f(0.0f, -height, 0.0f);
			glVertex3f(0.0f, -height, 1.0f);

			glColor3f(getColour(top));
			glVertex3f(1.0f, -height, 0.0f);
			glVertex3f(0.0f, -height, 0.0f);
			glVertex3f(0.0f, -height, 1.0f);
			glVertex3f(1.0f, -height, 1.0f);

			glColor3f(getColour(right));
			glVertex3f(1.0f, 0, 0.0f);
			glVertex3f(0.0f, 0, 0.0f);
			glVertex3f(0.0f, -height, 0.0f);
			glVertex3f(1.0f, -height, 0.0f);

			glColor3f(getColour(left));
			glVertex3f(1.0f, 0, 1.0f);
			glVertex3f(1.0f, 0, 0.0f);
			glVertex3f(1.0f, -height, 0.0f);
			glVertex3f(1.0f, -height, 1.0f);
		glEnd();
	}
};

vector<Block> blocks(10);
void update();
int dir = 0;
int current = 1;
int state = MOVING;
int flag = 0;

void initGL() {
	srand(time(NULL));
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	blocks[0].setPos(0.0f, 0.0f, 0.0f);
	blocks[0].setDim(100.0f, 100.0f);
	blocks[0].setColour(Colour(rand() % 125 + 100.0f, rand() % 125 + 100.0f, rand() % 125 + 100.0f));
	blocks[1].setPos(0.0f, height, 0.0f);
	blocks[1].setDim(100.0f, 100.0f);
	Colour nextColour = Colour(blocks[current - 1].top.getHSV());
	if (flag == 0)
		nextColour.cc -= GAP;
	else
		nextColour.cc += GAP;
	if ((nextColour.cc < 50.0f && flag == 0) || (nextColour.cc > 200.0f && flag == 1)) {
		flag = 1 - flag;
		nextColour.aa += 20.0f;
		nextColour.aa = (float)(((int)nextColour.aa) % 360);
	}
	blocks[current].setColour(nextColour.getRGB());

}
void timerFunc(int value)
{
	update();
	glutPostRedisplay();
	glutTimerFunc(17, timerFunc, 1);
}
void update() {
    if(kbd.keyPressed(RIGHT)) {
        rotateY += 3.5f;
        rotateY = (float)((int)rotateY%360);
    } else if(kbd.keyPressed(LEFT)) {
        rotateY -= 2.5f;
        rotateY = (float)((int)(rotateY+360)%360);
    }
	if (kbd.keyTapped(DOWN))
		state = FALLING;

	if (state != FALLING) {
		if (dir == 0) {
			if (current % 2 == 0) {
				blocks[current].x += X_SPEED;
				if (blocks[current].x > 100.0f)
					dir = 1;
			}
			else {
				blocks[current].z += X_SPEED;
				if (blocks[current].z > 150.0f)
					dir = 1;
			}
		}
		else {
			if (current % 2 == 0) {
				blocks[current].x -= X_SPEED;
				if (blocks[current].x < -150.0f)
					dir = 0;
			}
			else {
				blocks[current].z -= X_SPEED;
				if (blocks[current].z < -100.0f)
					dir = 0;
			}
		}
	}
	else if(blocks[current].y > -10000.0f) {
		blocks[current].y += Y_SPEED;
		Y_SPEED += Y_ACC;
		if(current % 2 == 0) {
            if(blocks[current].x + blocks[current].width < blocks[current-1].x || blocks[current].x > blocks[current-1].x + blocks[current-1].width) {
                gameOver = true;
            }
		} else {
            if(blocks[current].z + blocks[current].depth < blocks[current-1].z || blocks[current].z > blocks[current-1].z + blocks[current-1].depth) {
                gameOver = true;
            }
		}
		if (blocks[current].y - height <= blocks[current - 1].y && !gameOver) {
			blocks[current].y = blocks[current - 1].y + height;
            X_SPEED += 0.025f;
            score++;
			if(current %2 == 0) {
                if (blocks[current].x < blocks[current - 1].x) {
                    //printf("OK : landed successfully left\n");
                    if(blocks[current].x < blocks[current - 1].x-5) {
                        blocks[current].width -= (blocks[current-1].x- blocks[current].x);
                        blocks[current].depth = blocks[current-1].depth;
                        blocks[current].x = blocks[current - 1].x;
                        streak=0;
                    }
                    else {
                        streak++;
                        score++;
                        if(streak >= 5) {
                            blocks[current].width = blocks[current-1].width + increment;
                            blocks[current].depth = blocks[current-1].depth + increment;
                            blocks[current].x = blocks[current - 1].x - increment/2;
                            blocks[current].z = blocks[current - 1].z - increment/2;
                        } else {
                            blocks[current].width -= (blocks[current-1].x- blocks[current].x);
                            blocks[current].depth = blocks[current-1].depth;
                            blocks[current].x = blocks[current - 1].x;
                        }
                    }
                }
                else {
                    //printf("OK : landed successfully right\n");
                    if(blocks[current].x + blocks[current].width > blocks[current-1].x + blocks[current-1].width + 5) {
                        blocks[current].width += (blocks[current-1].x- blocks[current].x);
                        blocks[current].x = blocks[current-1].x+(blocks[current-1].width - blocks[current].width);
                        blocks[current].depth = blocks[current-1].depth;
                        streak=0;
                    }
                    else {
                        streak++;
                        score++;
                        if(streak >= 5) {
                            blocks[current].width = blocks[current-1].width + increment;
                            blocks[current].depth = blocks[current-1].depth + increment;
                            blocks[current].x = blocks[current - 1].x - increment/2;
                            blocks[current].z = blocks[current - 1].z - increment/2;
                        } else {
                            blocks[current].width += (blocks[current-1].x- blocks[current].x);
                            blocks[current].x = blocks[current-1].x+(blocks[current-1].width - blocks[current].width);
                            blocks[current].depth = blocks[current-1].depth;
                        }
                    }
                }
			} else {
                if (blocks[current].z < blocks[current - 1].z) {
                    //printf("OK : landed successfully back\n");
                    if(blocks[current].z < blocks[current - 1].z-5) {
                        blocks[current].depth -= (blocks[current-1].z- blocks[current].z);
                        blocks[current].width = blocks[current-1].width;
                        blocks[current].z = blocks[current - 1].z;
                        streak=0;
                    }
                    else {
                        streak++;
                        score++;
                        if(streak >= 5) {
                            blocks[current].width = blocks[current-1].width + increment;
                            blocks[current].depth = blocks[current-1].depth + increment;
                            blocks[current].z = blocks[current - 1].z - increment/2;
                            blocks[current].x = blocks[current - 1].x - increment/2;
                        } else {
                            blocks[current].depth -= (blocks[current-1].z- blocks[current].z);
                            blocks[current].width = blocks[current-1].width;
                            blocks[current].z = blocks[current - 1].z;
                        }
                    }
                }
                else {
                    //printf("OK : landed successfully front\n");
                    if(blocks[current].z + blocks[current].depth > blocks[current-1].z + blocks[current-1].depth + 5) {
                        blocks[current].depth += (blocks[current-1].z- blocks[current].z);
                        blocks[current].z = blocks[current-1].z+(blocks[current-1].depth - blocks[current].depth);
                        blocks[current].width = blocks[current-1].width;
                        streak=0;
                    }
                    else {
                        streak++;
                        score++;
                        if(streak >= 5) {
                            blocks[current].width = blocks[current-1].width + increment;
                            blocks[current].depth = blocks[current-1].depth + increment;
                            blocks[current].z = blocks[current - 1].z - increment/2;
                            blocks[current].x = blocks[current - 1].x - increment/2;
                        }
                        else {
                            blocks[current].depth += (blocks[current-1].z- blocks[current].z);
                            blocks[current].z = blocks[current-1].z+(blocks[current-1].depth - blocks[current].depth);
                            blocks[current].width = blocks[current-1].width;
                        }
                    }
                }
			}
            system("clear");
            printf("Score : %d\n", score);

			if (current+5 > (int)blocks.size())
				blocks.resize(2 * current);

			current++;
			if(current % 2 == 0) {
                blocks[current].setPos((rand() % 300) - 150, blocks[current - 1].y + height, blocks[current - 1].z);
			}
            else {
                blocks[current].setPos(blocks[current - 1].x, blocks[current - 1].y + height, (rand() % 300) - 150);
            }

            blocks[current].setDim(blocks[current-1].width, blocks[current-1].depth);

			//set colour
			Colour nextColour = Colour(blocks[current - 1].top.getHSV());
			//printf("%f %f %f\n", nextColour.aa, nextColour.bb, nextColour.cc);
			if (flag == 0)
				nextColour.cc -= GAP;
			else
				nextColour.cc += GAP;
			if ((nextColour.cc < 130.0f && flag == 0) || (nextColour.cc > 200.0f && flag == 1)) {
				flag = 1 - flag;
				nextColour.aa += 20.0f;
				nextColour.aa = (float)(((int)nextColour.aa) % 360);
			}
			//printf("%f %f %f\n", nextColour.aa, nextColour.bb, nextColour.cc);
			blocks[current].setColour(nextColour.getRGB());
			state = MOVING;
			Y_SPEED = 0.0f;
		}
	}
	if (blocks[current].y <= -100.0f) {
        rotateY+=0.5f;
	}

	if (blocks[current].y + yOffset > 350.0f)
		yOffset-=1.5f;

    if(gameOver) {
        if(scale >= 400.0f / blocks[current-1].y) {
            scale *= 0.99f;
            ysoff+=0.35f;
        }
    }
}

void drawText(char *text, int x, int y){
    glColor3f(1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    double matrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    glScalef(0.5f, 0.5f, 0.5f);
    for(int i=0; text[i] != '\0'; i++){
        glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
    }

    glScalef(2.0f, 2.0f, 2.0f);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}
char scoreMsg[50];
void render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    sprintf(scoreMsg, "Score : %d", score);
    drawText(scoreMsg, 20, 570);

	for (int i = 0; i <= current; i++) {
		blocks[i].draw();
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-rotateY, 0.0f, 1.0f, 0.0f);
	glRotatef(-35.264f, 1.0f, 0.0f, 0.0f);
	glScalef(1/scale, 1/scale, 1/scale);
    glColor3f(0.0, 0.0, 0.0);
    Colour c = blocks[current].top.getHSV();
    //c.cc = 20.0f;
    c.aa += 120.0f;
    c.bb = 0.8f;
    c.cc = 100.0f;
    glBegin(GL_QUADS);
        glColor3f(getColour(c.getRGB()));
        glVertex3f(-800.0, -300.0, -500.0f);
        glVertex3f(800.0, -300.0, -500.0f);
    c.cc = 255.0f;
    c.bb = 0.0f;
        glColor3f(getColour(c.getRGB()));
        glVertex3f(800.0, 400.0, -500.0f);
        glVertex3f(-800.0, 400.0, -500.0f);
    glEnd();

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-400.0f, 400.0f, -300.0f, 300.0f, -1000.0f, 1000.0f);
	glScalef(scale, scale, scale);
	glTranslatef(-50.0f, -200.0f+ysoff, -50.0f);
	glRotatef(35.264f, 1.0f, 0.0f, 0.0f);
	glTranslatef(50.0f, 0.0f, 50.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glTranslatef(-50.0f, 0.0f, -50.0f);


	glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;
	//float aspect = (float)width / (float)height;

	glViewport(0, 0, width, height);


}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_DOUBLE | GLUT_DEPTH);
	glDepthFunc(GL_LESS);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Stack 3D");

	//callback methods
	glutReshapeFunc(reshape);
	glutDisplayFunc(render);
	glutSpecialFunc(keyboardDown);
	glutSpecialUpFunc(keyboardUp);
	glutTimerFunc(17, timerFunc, 1);
	initGL();
	glutMainLoop();
	return 0;
}
