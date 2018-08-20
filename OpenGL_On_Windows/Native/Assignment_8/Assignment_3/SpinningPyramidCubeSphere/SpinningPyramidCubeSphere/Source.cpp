#include <Windows.h>
#include<gl/GL.h>
#include<Math.h>
#include<GL/glu.h>

#pragma comment(lib,"glu32.lib")
#pragma comment(lib, "opengl32.lib")
#define WIN_WIDTH  800
#define WIN_HEIGHT  600

bool gbIsGameDone = false;
bool gbIsFullScreen = false;
bool gbIsActiveWindow = false;
bool gbIsEscapeKeyPressed = false;



LONG glPreviousStyle;
WINDOWPLACEMENT  gwndplacement;
MONITORINFO mi;


float anglePyramid = 0.0f;
float angleCube = 0.0f;
float angleSphere = 0.0f;


GLboolean bPyramid = GL_TRUE;
GLboolean bCube = GL_FALSE;
GLboolean bSphere = GL_FALSE;
GLboolean IsLkeyPressed = GL_FALSE;

GLUquadric *quadric = NULL;

GLfloat light0_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light0_difuse[] = { 1.0f,0.0f,0.0f,0.0f };
GLfloat light0_specular[] = { 1.0f,0.0f,0.0f,0.0f };
GLfloat light0_position[] = { 2.0f,1.0f,1.0f,0.0f };


GLfloat light1_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat light1_difuse[] = { 0.0f,0.0f,1.0f,0.0f };
GLfloat light1_specular[] = { 0.0f,0.0f,1.0f,0.0f };
GLfloat light1_position[] = { -2.0f,1.0f,1.0f,0.0f };


GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat material_difuse[] = { 1.0f,1.0f,1.0f,0.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,0.0f };
GLfloat material_shinyness = 50.0f;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{

	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	void IntializeOpenGL(HWND, HDC*, HGLRC*);

	void Update(void);

	void Render(HDC hdc);

	void UnIntializeOpenGL(HWND, HDC*, HGLRC*);



	MSG msg;
	WNDCLASSEX wndclassex;
	TCHAR szAppName[] = TEXT("(LIGHTS glNormal) 3D PYRAMID ROTATION APPLCATION USING OPENGL");
	HDC hdc = NULL;
	HGLRC hglrc = NULL;

	wndclassex.cbSize = sizeof(WNDCLASSEX);
	wndclassex.cbClsExtra = 0;
	wndclassex.cbWndExtra = 0;
	wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclassex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclassex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclassex.hInstance = hInstance;
	wndclassex.lpfnWndProc = WndProc;
	wndclassex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclassex.lpszClassName = szAppName;
	wndclassex.lpszMenuName = NULL;
	wndclassex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	if (!RegisterClassEx(&wndclassex))
	{
		MessageBox(NULL, TEXT("ERROR IN REGISTERING CLASS"), TEXT("ERROR"), MB_ICONERROR);
		return 0;
	}



	HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);


	ShowWindow(hWnd, iCmdShow);

	SetForegroundWindow(hWnd);

	SetFocus(hWnd);

	IntializeOpenGL(hWnd, &hdc, &hglrc);


	while (gbIsGameDone == false)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				gbIsGameDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		}
		else
		{
			if (gbIsActiveWindow == true)
			{
				if (gbIsEscapeKeyPressed == true)
					gbIsGameDone = true;

				Update();
				Render(hdc);
			}
		}


	}

	UnIntializeOpenGL(hWnd, &hdc, &hglrc);

	return msg.wParam;
}
void IntializeOpenGL(HWND hwnd, HDC* hdc, HGLRC* hglrc)
{
	void Resize(int, int);

	PIXELFORMATDESCRIPTOR pfd;
	RECT rect;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cBlueBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;


	*hdc = GetDC(hwnd);

	int iIndexPixelFormatDescriptor = ChoosePixelFormat(*hdc, &pfd);

	if (iIndexPixelFormatDescriptor == 0)
	{
		MessageBox(hwnd, TEXT("ERROR TO CHOOSE PIXEL FORMAT DESCRIPTOR"), TEXT("ERROR"), MB_ICONERROR);
		return;
	}


	if (SetPixelFormat(*hdc, iIndexPixelFormatDescriptor, &pfd) == FALSE)
	{
		MessageBox(hwnd, TEXT("ERROR TO SET PIXEL FORMAT"), TEXT("ERROR"), MB_ICONERROR);
		ReleaseDC(hwnd, *hdc);

		*hdc = NULL;
		return;
	}

	*hglrc = wglCreateContext(*hdc);

	if (*hglrc == NULL)
	{
		MessageBox(hwnd, TEXT("ERROR TO SET PIXEL FORMAT"), TEXT("ERROR"), MB_ICONERROR);

		wglDeleteContext(*hglrc);
		ReleaseDC(hwnd, *hdc);

		*hglrc = NULL;
		*hdc = NULL;
		return;
	}

	if (wglMakeCurrent(*hdc, *hglrc) == FALSE)
	{
		MessageBox(hwnd, TEXT("ERROR TO wgl Make Current"), TEXT("ERROR"), MB_ICONERROR);

		wglDeleteContext(*hglrc);
		ReleaseDC(hwnd, *hdc);

		*hglrc = NULL;
		*hdc = NULL;
		return;

	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Black background
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // Less than or equal
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_difuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glEnable(GL_LIGHT0);


	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_difuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glEnable(GL_LIGHT1);


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_difuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_shinyness);

	Resize(WIN_WIDTH, WIN_HEIGHT);
}

void Render(HDC hdc)
{
	void Draw3DRotatingPyramid();
	void Draw3DRotatingCube();
	void Draw3DRotatingSphere();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (bPyramid == GL_TRUE)
	{
		Draw3DRotatingPyramid();
	}


	if (bCube == GL_TRUE)
	{
		Draw3DRotatingCube();
	}

	if (bSphere == GL_TRUE)
	{
		Draw3DRotatingSphere();
	}

	SwapBuffers(hdc);

}
void UnIntializeOpenGL(HWND hwnd, HDC* hdc, HGLRC* hglrc)
{

	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}

	wglDeleteContext(*hglrc);
	*hglrc = NULL;

	ReleaseDC(hwnd, *hdc);
	*hdc = NULL;

	DestroyWindow(hwnd);
	hwnd = NULL;

	PostQuitMessage(0);

	return;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	void Resize(int, int);

	void ToggleScreen(HWND);


	void Resize(int, int);

	void ToggleScreen(HWND);


	switch (message)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case   WM_ACTIVATE:
		if (HIWORD(wParam) == 0)
			gbIsActiveWindow = true;
		else
			gbIsActiveWindow = false;
		break;

	case WM_SIZE:
		Resize(LOWORD(lParam), HIWORD(lParam));
		break;


	case WM_KEYDOWN:

		switch (wParam)
		{

		case 76:
			if (IsLkeyPressed == GL_TRUE)
			{
				glDisable(GL_LIGHTING);
				IsLkeyPressed = GL_FALSE;
			}
			else
			{
				glEnable(GL_LIGHTING);
				IsLkeyPressed = GL_TRUE;
			}
			break;


		case 80:

			if (bPyramid == GL_TRUE)
			{
				bPyramid = GL_FALSE;
			}
			else
			{
				bPyramid = GL_TRUE;
				bCube = GL_FALSE;
				bSphere = GL_FALSE;
			}
			break;

		case 67:
			if (bCube == GL_TRUE)
			{
				bCube = GL_FALSE;
			}
			else
			{
				bCube = GL_TRUE;
				bSphere = GL_FALSE;
				bPyramid = GL_FALSE;
			}
			break;

		case 83:
			if (bSphere == GL_TRUE)
			{
				bSphere = GL_FALSE;
			}
			else
			{
				bSphere = GL_TRUE;
				bPyramid = GL_FALSE;
				bCube = GL_FALSE;
			}
			break;


		case VK_ESCAPE: gbIsEscapeKeyPressed = true;
			break;

		case 0x46:
			ToggleScreen(hWnd);
			break;

		default: break;
		}


	default: break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}
void Resize(int width, int height)
{
	if (height == 0) height = 1;


	glViewport(0, 0, GLsizei(width), GLsizei(height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}
void ToggleScreen(HWND hwnd)
{
	if (gbIsFullScreen)
	{
		SetWindowLong(hwnd, GWL_STYLE, glPreviousStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hwnd, &gwndplacement);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		gbIsFullScreen = false;
		return;
	}

	glPreviousStyle = GetWindowLong(hwnd, GWL_STYLE);

	if (glPreviousStyle & WS_OVERLAPPEDWINDOW)
	{
		mi = { sizeof(MONITORINFO) };

		if (GetWindowPlacement(hwnd, &gwndplacement) && GetMonitorInfo(MonitorFromWindow(hwnd, MONITORINFOF_PRIMARY), &mi))
		{
			SetWindowLong(hwnd, GWL_STYLE, glPreviousStyle & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
		}

	}
	ShowCursor(FALSE);
	gbIsFullScreen = true;

}

void Update()
{
	if (bPyramid == GL_TRUE && anglePyramid >= 360.0f)
	{
		anglePyramid = 0.0f;
	}
	if (bPyramid == GL_TRUE)
	{
		anglePyramid = anglePyramid + 0.025f;
	}


	if (bCube == GL_TRUE && angleCube >= 360.0f)
	{
		angleCube = 0.0f;
	}
	if (bCube == GL_TRUE)
	{
		angleCube = angleCube + 0.040f;
	}


	if (bSphere == GL_TRUE && angleSphere >= 360.0f)
	{
		angleSphere = 0.0f;
	}
	if (bSphere == GL_TRUE)
	{
		angleSphere = angleSphere + 0.030f;
	}

}

void Draw3DRotatingSphere()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -3.0f);
	glRotatef(angleSphere, 0.0f, 1.0f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GLU_FILL);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.75f, 30.0f, 30.0f);
}


void  Draw3DRotatingCube()
{

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -6.0f);
	glScalef(0.75f, 0.75f, 0.75f);
	glRotatef(angleCube, 0.0f, 1.0f, 0.0f);

	glBegin(GL_QUADS);

	//Front face
	glNormal3f(0.0f, 0.0f, 1.0f);
	//glColor3f(1.0f, 0.0f, 0.0f); // red
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	//


	//Back face
	glNormal3f(0.0f, 0.0f, -1.0f);
	//glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	//


	//Left face
	glNormal3f(1.0f, 0.0f, 0.0f);
	//glColor3f(0.0f, 0.0f, 1.0f); // blue
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	//
	
	//Right face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	//glColor3f(1.0f, 1.0f, .0f); // yellow
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	//

	
	//Top face
	glNormal3f(0.0f, 1.0f, 0.0f);
	//glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	//


	//Bottom face
	glNormal3f(0.0f, -1.0f, 0.0f);
	//glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	//
	glEnd();
}

void Draw3DRotatingPyramid()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(anglePyramid, 0.0f, 1.0f, 0.0f);
	
	glBegin(GL_TRIANGLES);

	// Front face
	glNormal3f(0.0f, 0.447214f, 0.894427f);
	//glColor3f(1.0f, 0.0f, 0.0f); // red
	glVertex3f(0.0f, 1.0f, 0.0f);

	//glColor3f(0.0f, 1.0f, 0.0f); //blue
	glVertex3f(-1.0f, -1.0f, 1.0f);

	//glColor3f(0.0f, 0.0f, 1.0f); // green
	glVertex3f(1.0f, -1.0f, 1.0f);
	//

	//Back Face
	glNormal3f(0.0f, 0.4472141f, -0.894427f);
	//glColor3f(1.0f, 0.0f, 0.0f); //red
	glVertex3f(0.0f, 1.0f, 0.0f);

	//glColor3f(0.0f, 1.0f, 0.0f);// blue
	glVertex3f(1.0f, -1.0f, -1.0f);

	//glColor3f(0.0f, 0.0f, 1.0f);// green
	glVertex3f(-1.0f, -1.0f, -1.0f);
	//

	//Right face
	glNormal3f(0.894427f, 0.4472141f, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f); //red
	glVertex3f(0.0f, 1.0f, 0.0f);

	//glColor3f(0.0f, 0.0f, 1.0f); // blue
	glVertex3f(1.0f, -1.0f, 1.0f);

	//glColor3f(0.0f, 1.0f, 0.0f); //green
	glVertex3f(1.0f, -1.0f, -1.0f);
	//

	//Left face
	glNormal3f(-0.894427f, 0.4472141f, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f); // red
	glVertex3f(0.0f, 1.0f, 0.0f);

	//glColor3f(0.0f, 0.0f, 1.0f); //green
	glVertex3f(-1.0f, -1.0f, -1.0f);

	//glColor3f(0.0f, 1.0f, 0.0f); // blue
	glVertex3f(-1.0f, -1.0f, 1.0f);
	//
	glEnd();
}