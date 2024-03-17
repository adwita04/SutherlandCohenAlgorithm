/*
Question:
To implement Sutherland-Cohen Clipping Algorithm.

Name: Adwita Deshpande
Roll No.: 2203303
Branch: Mathematics and Computing
*/


#include <cstdlib>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define NUMBERPRIMITIVES 3

static int width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int rectCount = 0; //Number of rectangles
static int tempX, tempY; // Co-ordinates of clicked point.
static bool Clip = false;
float xMax,xMin,yMax,yMin; //define the extremes of the clipping window


class Point
{
    public:
        float x, y; // x and y co-ordinates of point
        bool isClipped; //to check if point is clipped
        float size; // size of point
        Point(float xVal, float yVal, float sizeVal)
        {
            x = xVal; y = yVal; size = sizeVal;
            isClipped =false;
        }
        Point() {};
        void drawPoint(void); // Function to draw a point.
};

// Function to draw a point.
void Point::drawPoint()
{
    if (isClipped == true)
        glColor3f(0.0,0.7,0.7); //if point is clipped change color
    else
        glColor3f(0.0,0.0,0.0);
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 0.0);
	glEnd();
}

// Vector of points.
std::vector<Point> points;

// Function to draw all points in the points array.
void drawPoints(void)
{
	// Loop through the points array drawing each point.
	for (auto point : points) { point.drawPoint(); }
}

// Line class.
class Line
{
public:
    float x1, y1, x2, y2; // x and y co-ordinates of endpoints.
    bool isClipped; //to check if line is clipped
	Line(float x1Val, float y1Val, float x2Val, float y2Val)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
		isClipped = false;
	}
	Line() {};
	void drawLine();
};


// Function to draw a line.
void Line::drawLine()
{
    if (isClipped == true)
        glColor3f(0.0,0.7,0.7);
    else
        glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

// Vector of lines.
std::vector<Line> lines;

//Vector to iterate through lines vector.
//std::vector<Line>::iterator it;

// Function to draw all lines in the lines array.
void drawLines(void)
{
	// Loop through the lines array drawing each line.
	for (auto line : lines) { line.drawLine(); }
}

// Rectangle class.
class Rect
{
public:
    float x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices
	Rect(float x1Val, float y1Val, float x2Val, float y2Val)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
	}
	Rect() {};
	void drawRectangle();

};

// Function to draw a rectangle.
void Rect::drawRectangle()
{
    glColor3f(0.0,0.0,0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
std::vector<Rect> rectangles;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
	// Loop through the rectangles array drawing each rectangle.
	for (auto rectangle : rectangles) { rectangle.drawRectangle(); }
}

// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{
	if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.9*height, 0.1*width, height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.9*height, 0.1*width, height);

	// Draw point icon.
	glPointSize(pointSize);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex3f(0.05*width, 0.95*height, 0.0);
	glEnd();
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{
	if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

	// Draw line icon.
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.025*width, 0.825*height, 0.0);
	glVertex3f(0.075*width, 0.875*height, 0.0);
	glEnd();
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{
	if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

	// Draw rectangle icon.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
	glEnd();
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
	glColor3f(0.6, 0.6, 0.6);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glVertex3f(tempX, tempY, 0.0);
	glEnd();
}


// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	drawPointSelectionBox();
	drawLineSelectionBox();
	drawRectangleSelectionBox();
	drawInactiveArea();

	drawPoints();
    drawLines();
	drawRectangles();

	// Draw shape according to primitive selected.
	if (((primitive == LINE) || (primitive == RECTANGLE)) &&
		(pointCount == 1)) drawTempPoint();

	glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
	if (y < (1 - NUMBERPRIMITIVES*0.1)*height) primitive = INACTIVE;
	else if (y < (1 - 2 * 0.1)*height) primitive = RECTANGLE;
	else if (y < (1 - 1 * 0.1)*height) primitive = LINE;
	else primitive = POINT;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		y = height - y; // Correct from mouse to OpenGL co-ordinates.

		if (x < 0 || x > width || y < 0 || y > height); // Click outside canvas - do nothing.

		// Click in left selection area.
		else if (x < 0.1*width)
		{
			pickPrimitive(y);
			pointCount = 0;
		}

		// Click in canvas.
		else
		{
			if (primitive == POINT) points.push_back(Point(x, y, pointSize)); //push point in the points vector
			else if (primitive == LINE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					lines.push_back(Line(tempX, tempY, x, y)); // push line in the points vector
					pointCount = 0;
				}
			}
			else if (primitive == RECTANGLE)
			{
			    if(rectCount == 0) // to make sure you have only one clipping window
                {
                    if (pointCount == 0 )
                    {
                        tempX = x; tempY = y;
                        pointCount++;
                    }
                    else
                    {
                        rectangles.push_back(Rect(tempX, tempY, x, y)); //push rectangle in the points vector
                        pointCount = 0;
                        rectCount++;
                    }
                }
			}
		}
	}
	glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set viewing box dimensions equal to window dimensions.
	glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

	// Pass the size of the OpenGL window to globals.
	width = w;
	height = h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
	points.clear();
	lines.clear();
	rectangles.clear();
	primitive = INACTIVE;
	pointCount = 0;
	rectCount =0;
	xMax=0;xMin=0;yMax=0;yMin=0;
}

// Clipping Algorithm.

// Define codes for clipping window edges.
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// To calculate the code
int computeCode(float x, float y)
{
    int code = INSIDE; // Initialize as inside
    if (x < xMin) // to the left of clipping window
    {
        code = code | LEFT;
    }
    else if (x > xMax) // to the right of clipping window
    {
        code  = code |  RIGHT;
    }
    if (y < yMin) // below clipping window
    {
        code = code |  BOTTOM;
    }
    else if (y > yMax) // above clipping window
    {
         code  = code |  TOP;
    }
    return code;
}

//Sutherland Cohen Clipping algorithm
void clipLineCohenSutherland(void)
{
    //Calculate the extremes of the clipping window
    for (auto rectangle : rectangles)
    {
        xMax = (rectangle.x1 > rectangle.x2) ? rectangle.x1 : rectangle.x2;
        xMin = (rectangle.x1 < rectangle.x2) ? rectangle.x1 : rectangle.x2;
        yMax = (rectangle.y1 > rectangle.y2) ? rectangle.y1 : rectangle.y2;
        yMin = (rectangle.y1 < rectangle.y2) ? rectangle.y1 : rectangle.y2;
    }
    for (auto& line: lines)
    {
        int startCode = computeCode(line.x1,line.y1);
        int endCode = computeCode(line.x2,line.y2);
        bool accept = false;
        while (true)
        {
            if (!(startCode | endCode))
            { // Both endpoints inside window.
                accept = true;
                break;
            }
            else if (startCode & endCode)
            { // Both endpoints outside same edge.
                break;
            }
            else
            {
                float x, y;
                int outsideCode = startCode ? startCode : endCode;

                // Calculate intersection point
                if (outsideCode & TOP) {
                    x = line.x1 + (line.x2 - line.x1) * (yMax - line.y1) / (line.y2 - line.y1);
                    y = yMax;
                } else if (outsideCode & BOTTOM) {
                    x = line.x1 + (line.x2 - line.x1) * (yMin - line.y1) / (line.y2 - line.y1);
                    y = yMin;
                } else if (outsideCode & RIGHT) {
                    y = line.y1 + (line.y2 - line.y1) * (xMax - line.x1) / (line.x2 - line.x1);
                    x = xMax;
                } else if (outsideCode & LEFT) {
                    y = line.y1 + (line.y2 - line.y1) * (xMin - line.x1) / (line.x2 - line.x1);
                    x = xMin;
                }

                // Update the point and region code
                if (outsideCode == startCode)
                {
                    line.x1 = x;
                    line.y1 = y;
                    startCode = computeCode(line.x1, line.y1);
                }
                else
                {
                    line.x2 = x;
                    line.y2 = y;
                    endCode = computeCode(line.x2, line.y2);
                }
            }
        }
        // Line is completely outside.
        if (!accept)
            line.isClipped= false;
        else
            line.isClipped= true;
    }

    //Algorithm for points
    for (auto& point : points)
    {
        int Pointcode = computeCode(point.x ,point.y);

        //If code is non-zero, its outside the clipping window
        if(Pointcode!= 0)
            point.isClipped=false;
        else
            point.isClipped= true;
    }

    //Remove lines from vector which are completely outside the clipping window
    auto itL = lines.begin();
    while (itL != lines.end()) {
        // Check if the line is not clipped and erase it if necessary
        if (!itL ->isClipped) {
            itL = lines.erase(itL); // Erase the current element and update the iterator
        } else {
            ++itL; // Move to the next element
        }
    }

    //Remove points from vector which are completely outside the clipping window
    auto itP = points.begin();
    while (itP != points.end()) {
        // Check if the point is not clipped and erase it if necessary
        if (!itP ->isClipped) {
            itP = points.erase(itP); // Erase the current element and update the iterator
        } else {
            ++itP; // Move to the next element
        }
    }
}

// The right button menu callback function.
void rightMenu(int id)
{
    if (id == 1)
    {
        Clip = true;
        clipLineCohenSutherland();
        glutPostRedisplay();
    }
	if (id == 2)
	{
		clearAll();
		glutPostRedisplay();
	}
	if (id == 3) exit(0);
}

// Function to create menu.
void makeMenu(void)
{
	glutCreateMenu(rightMenu);
	glutAddMenuEntry("Clip", 1);
	glutAddMenuEntry("Clear", 2);
	glutAddMenuEntry("Quit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	makeMenu(); // Create menu.
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Left click on a box on the left to select a primitive." << std::endl
		<< "Then left click on the drawing area: once for point, twice for line or rectangle." << std::endl
		<< "Draw a rectangle to define the clipping window." << std::endl
		<< "Right click for menu options." << std::endl
		<< "Click on Clip in the menu to Clip the image" << std::endl;
}

// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("canvas.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseControl);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
