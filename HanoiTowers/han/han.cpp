#include <iostream>
#include <vector>
#include <GL/freeglut.h>

using namespace std;

struct State{
	int n;
	int start;
	int temp;
	int dest;
	int step;
};
vector<State> stack;
vector<int> rings[3];
int N;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	for (int i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < rings[i].size(); ++j)
		{
			const int RingCenter = i * 852 / 3 + 852 / 3 / 2;
			const int RingRadius = (852 / 3 / 2 - rings[i][j] * 852 / 3 / 2 / N);
			glColor3f(0.5, 1.0, 0.5);
			glVertex2f(RingCenter - RingRadius, 480 - j * 20);
			glVertex2f(RingCenter - RingRadius, 480 - j * 20 - 18);
			glVertex2f(RingCenter + RingRadius, 480 - j * 20 - 18);
			glVertex2f(RingCenter + RingRadius, 480 - j * 20);
		}
	}
	glEnd();
	glutSwapBuffers();
}

void timer(int = 0)
{
	if (stack.size() > 0)
	{
		State &state = stack.back();
		switch (state.step)
		{
		case 0:
			if (state.n == 0)
				stack.pop_back();
			else
			{
				++state.step;
				State newState;
				newState.n = state.n - 1;
				newState.start = state.start;
				newState.dest = state.temp;
				newState.temp = state.dest;
				newState.step = 0;
				stack.push_back(newState);
			}
			break;
		case 1:
			cout << state.start << " -> " << state.dest << endl;
			rings[state.dest].push_back(*rings[state.start].rbegin());
			rings[state.start].pop_back();
			++state.step;

			State newState;
			newState.n = state.n - 1;
			newState.start = state.temp;
			newState.dest = state.dest;
			newState.temp = state.start;
			newState.step = 0;
			stack.push_back(newState);
			break;
		case 2:
			stack.pop_back();
			break;
		}
		glutPostRedisplay();
		glutTimerFunc(10, timer, 0); //change first arg to make slower/faster
	}
}

void init(int n, int start, int temp, int dest)
{
	State state;
	state.n = n;
	state.start = start;
	state.temp = temp;
	state.dest = dest;
	state.step = 0;
	stack.push_back(state);
	N = n;
	for (int i = 0; i < n; ++i)
		rings[start].push_back(i);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(852, 480);
	glutInitWindowPosition(0, 86);
	glutCreateWindow("Tower");
	glClearColor(0, 0, 0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 852, 480, 0, -1, 1);
	glutDisplayFunc(display);

	init(3, 0, 1, 2); // 1st arg - number of blocks, 2 - starting point, 3 - temp, 4 - finish
	timer();
	glutMainLoop();

	system("pause");
	return 0;
}