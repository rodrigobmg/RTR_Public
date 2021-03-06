#include <Windows.h>
#include<gl/GL.h>
#include<Math.h>
#include<GL/glu.h>
#define PI 3.1416
#pragma comment(lib,"glu32.lib")
#pragma comment(lib, "opengl32.lib")
#define WIN_WIDTH  800
#define WIN_HEIGHT  600

bool gbIsGameDone = false;
bool gbIsFullScreen = false;
bool gbIsActiveWindow = false;
bool gbIsEscapeKeyPressed = false;

float angleCubeInDegree = 0.0f;


GLfloat idetentiyMatirx[16];
GLfloat transformationMatrix[16];
GLfloat scaleMatrix[16];
GLfloat xRotationMatirx[16];
GLfloat yRotationMatrix[16];
GLfloat zRotationMatrix[16];


LONG glPreviousStyle;
WINDOWPLACEMENT  gwndplacement;
MONITORINFO mi;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{

	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	void IntializeOpenGL(HWND, HDC*, HGLRC*);

	void Update(void);

	void Render(HDC hdc);

	void UnIntializeOpenGL(HWND, HDC*, HGLRC*);



	MSG msg;
	WNDCLASSEX wndclassex;
	TCHAR szAppName[] = TEXT("3D CUBE ROTATION WiTHOUT TRANSFORMATION FUNCTION APPLCATION USING OPENGL");
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

	void IntializeIdentityMatrix();

	void  IntializeTransformationMatrix(GLfloat x, GLfloat y, GLfloat z);

	void IntializeScaleMatrix(GLfloat x, GLfloat y, GLfloat z);


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


	IntializeIdentityMatrix();
	IntializeTransformationMatrix(0.0f, 0.0f, -6.0f);
    IntializeScaleMatrix(0.75f , 0.75f ,0.75f);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	

	Resize(WIN_WIDTH, WIN_HEIGHT);
}

void Render(HDC hdc)
{
	void DrawMultiColor3dCubeRotating();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawMultiColor3dCubeRotating();

	SwapBuffers(hdc);

}
void UnIntializeOpenGL(HWND hwnd, HDC* hdc, HGLRC* hglrc)
{
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
	if (angleCubeInDegree >= 360.0f) angleCubeInDegree = 0.0f;

	angleCubeInDegree = angleCubeInDegree + 0.025f;
}

void DrawMultiColor3dCubeRotating()
{
	void IntializeXRotationMatrix(GLfloat);
	void IntializeYRotationMatrix(GLfloat);
	void IntializeZRotationMatrix(GLfloat);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(idetentiyMatirx);
	glMultMatrixf(transformationMatrix);
	glMultMatrixf(scaleMatrix);

	GLfloat angleRadian = angleCubeInDegree * (PI / 180.0f);

	IntializeXRotationMatrix(angleRadian);
	glMultMatrixf(xRotationMatirx);

	IntializeYRotationMatrix(angleRadian);
	glMultMatrixf(yRotationMatrix);

	IntializeZRotationMatrix(angleRadian);
	glMultMatrixf(zRotationMatrix);


	glBegin(GL_QUADS);

	//Front face
	glColor3f(0.0f, 0.0f, 1.0f); //BLUE
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	//


	//Back face
	glColor3f(0.0f, 1.0f, 1.0f); //CYAN
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	//


	//Left face
	glColor3f(1.0f, 1.0f, 0.0f); // YELLOW
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	//

	//Right face
	glColor3f(1.0f, 0.0f, 1.0f); // MAGENTA
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	//


	//Top face
	glColor3f(1.0f, 0.0f, 0.0f); //RED
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	//


	//Bottom face
	glColor3f(0.0f, 1.0f, 0.0f); //GREEN
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	//
	glEnd();
}




void IntializeIdentityMatrix()
{
	idetentiyMatirx[0] = 1;
	idetentiyMatirx[1] = 0;
	idetentiyMatirx[2] = 0;
	idetentiyMatirx[3] = 0;
	idetentiyMatirx[4] = 0;
	idetentiyMatirx[5] = 1;
	idetentiyMatirx[6] = 0;
	idetentiyMatirx[7] = 0;
	idetentiyMatirx[8] = 0;
	idetentiyMatirx[9] = 0;
	idetentiyMatirx[10] = 1;
	idetentiyMatirx[11] = 0;
	idetentiyMatirx[12] = 0;
	idetentiyMatirx[13] = 0;
	idetentiyMatirx[14] = 0;
	idetentiyMatirx[15] = 1;
}
void IntializeTransformationMatrix(GLfloat x, GLfloat y, GLfloat z)
{
	transformationMatrix[0] = 1.0f;
	transformationMatrix[1] = 0.0f;
	transformationMatrix[2] = 0.0f;
	transformationMatrix[3] = 0.0f;
	transformationMatrix[4] = 0.0f;
	transformationMatrix[5] = 1.0f;
	transformationMatrix[6] = 0.0f;
	transformationMatrix[7] = 0.0f;
	transformationMatrix[8] = 0.0f;
	transformationMatrix[9] = 0.0f;
	transformationMatrix[10] = 1.0f;
	transformationMatrix[11] = 0.0f;
	transformationMatrix[12] = x;
	transformationMatrix[13] = y;
	transformationMatrix[14] = z;
	transformationMatrix[15] = 1.0f;
}
void IntializeScaleMatrix(GLfloat x, GLfloat y, GLfloat z)
{
	scaleMatrix[0] = x;
	scaleMatrix[1] = 0.0f;
	scaleMatrix[2] = 0.0f;
	scaleMatrix[3] = 0.0f;
	scaleMatrix[4] = 0.0f;
	scaleMatrix[5] = y;
	scaleMatrix[6] = 0.0f;
	scaleMatrix[7] = 0.0f;
	scaleMatrix[8] = 0.0f;
	scaleMatrix[9] = 0.0f;
	scaleMatrix[10] = z;
	scaleMatrix[11] = 0.0f;
	scaleMatrix[12] = 0.0f;
	scaleMatrix[13] = 0.0f;
	scaleMatrix[14] = 0.0f;
	scaleMatrix[15] = 1.0f;
}
void IntializeXRotationMatrix(GLfloat theta)
{
	xRotationMatirx[0] = 1.0f;
	xRotationMatirx[1] = 0.0f;
	xRotationMatirx[2] = 0.0f;
	xRotationMatirx[3] = 0.0f;
	xRotationMatirx[4] = 0.0f;
	xRotationMatirx[5] = cos(theta);
	xRotationMatirx[6] = sin(theta);
	xRotationMatirx[7] = 0.0f;
	xRotationMatirx[8] = 0.0f;
	xRotationMatirx[9] = -sin(theta);
	xRotationMatirx[10] = cos(theta);
	xRotationMatirx[11] = 0.0f;
	xRotationMatirx[12] = 0.0f;
	xRotationMatirx[13] = 0.0f;
	xRotationMatirx[14] = 0.0f;
	xRotationMatirx[15] = 1.0f;
}
void IntializeYRotationMatrix(GLfloat thetha)
{
	yRotationMatrix[0] = cos(thetha);
	yRotationMatrix[1] = 0.0f;
	yRotationMatrix[2] = -sin(thetha);
	yRotationMatrix[3] = 0.0f;
	yRotationMatrix[4] = 0.0f;
	yRotationMatrix[5] = 1.0f;
	yRotationMatrix[6] = 0.0f;
	yRotationMatrix[7] = 0.0f;
	yRotationMatrix[8] = sin(thetha);
	yRotationMatrix[9] = 0.0f;
	yRotationMatrix[10] = cos(thetha);
	yRotationMatrix[11] = 0.0f;
	yRotationMatrix[12] = 0.0f;
	yRotationMatrix[13] = 0.0f;
	yRotationMatrix[14] = 0.0f;
	yRotationMatrix[15] = 1.0f;
}
void IntializeZRotationMatrix(GLfloat thetha)
{
	zRotationMatrix[0] = cos(thetha);
	zRotationMatrix[1] = sin(thetha);
	zRotationMatrix[2] = 0.0f;
	zRotationMatrix[3] = 0.0f;
	zRotationMatrix[4] = -sin(thetha);
	zRotationMatrix[5] = cos(thetha);
	zRotationMatrix[6] = 0.0f;
	zRotationMatrix[7] = 0.0f;
	zRotationMatrix[8] = 0.0f;
	zRotationMatrix[9] = 0.0f;
	zRotationMatrix[10] = 1.0f;
	zRotationMatrix[11] = 0.0f;
	zRotationMatrix[12] = 0.0f;
	zRotationMatrix[13] = 0.0f;
	zRotationMatrix[14] = 0.0f;
	zRotationMatrix[15] = 1.0f;
}