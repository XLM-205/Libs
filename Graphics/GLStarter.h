/*-------Moon Wiz Graphical Library Starter-------
*	--Starts-up and make available GL context with DirectX or OpenGL
*
*	Copyright (c) 2015 Moon Wiz Studios
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version --.--
*/

#ifndef _H_MWGLSTARTER_
#define _H_MWGLSTARTER_
//Moon Wiz GL Starter -> A.K.A MWGLS

//#include "..\Control\Igniter.h"
#include "..\Control\MessageBoxHandler.h"
#include "..\Control\InputHandler.h"
#include "..\Maths\Vec2d.h"

#define MWGLS_DEMO_LINE_COORDS	//IF defined, display lines going from the center to each corner in faint orange
//#define MWGLS_DEMO_TEST		//If defined, display a triangle on the screen, to test the rendering window
#define MWGLS_GL_BASIC

#ifdef MWGLS_DIRECTX
	#pragma message ("[MWGLS] -> Starting Up with DirectX configuration")
	//Code for DirectX intialization and procedures
#else
	#define MWGLS_OPENGL
	#pragma message ("[MWGLS] -> Starting Up with OpenGL configuration (default)")
		#ifndef MWGLS_GL_BASIC
		#pragma comment(lib, "glew32.lib")		//Extension Wangler
		#endif
	//#pragma comment(lib, "opengl32.lib")		//OpenGL default library
	
	#include <gl\GL.h>
	#include <gl\GLU.h>
#endif

//#error [MWGLS][INIT] -> No Graphical Mode Defined! Define either 'MWGLS_OPENGL' or 'MWGLS_DIRECTX'

//Copy & Paste Shortcut
//#ifdef MWGLS_OPENGL
//
//#elif defined MWGLS_DIRECTX
//
//#else
//#endif

#define MWGLS_FLAG_FULLSCREEN		1	//Initialization Flag --> Fullscreen context
#define MWGLS_FLAG_BORDERLESS		2	//Initialization Flag --> Borderless window (if supported). If flag 'MWGLS_FLAG_FULLSCREEN' was set, this flag will not be set
#define MWGLS_FLAG_ALLOW_RESIZE		4	//Initialization Flag --> Window CAN be resized
#define MWGLS_FLAG_SHOW_CURSOR		8	//Initialization Flag --> Show Cursor on window

//Default Initialization Flag 
#define MWGLS_FLAG_DEFAULT	MWGLS_FLAG_ALLOW_RESIZE | MWGLS_FLAG_SHOW_CURSOR
#define MWGLS_COLOR_DEFAULT 32			//Default color bits

enum MWGLSErrorCodes
{
	MWGLS_ERROR_NONE,						//No error
	MWGLS_ERROR_FEATURE_UNAVAILABLE,		//Error trying to use a feature that is NOT supported by the Graphics Card / OS
	MWGLS_ERROR_CONTEXT_ALREADY_BUILT,		//Error trying to build a context that is already built!
	MWGLS_ERROR_INIT						//Error initializing context. Call 'getLastError()' for error description
};

enum MWGLSViewLimits
{
	MWGLS_VIEW_LEFT_OR_FOV,					//Left view (Ortho) or FOV (Perspective)
	MWGLS_VIEW_RIGHT_OR_ASPECT,				//Right view (Ortho) or Aspect (Perspective)
	MWGLS_VIEW_BOTTOM_OR_PzNEAR,			//Bottom view (Ortho) or Perspective zNear
	MWGLS_VIEW_TOP_OR_PzFAR,				//Top view (Ortho) or Perspectie zFar
	MWGLS_VIEW_ORTHO_zNEAR,					//Ortho zNear
	MWGLS_VIEW_ORTHO_ZFAR					//Ortho zFar
};

class MWGLContext
{
protected:
	MWI_UNVSAL_WINDOW_HANDLE m_wHandle;
	MWI_UNVSAL_APP_INSTANCE m_instance;
	uint8 m_errorCode = 0;				//Becomes a value (see 'MWGLSErrorCodes') if an error was detected
	bool m_contextSuccess = false;		//Becomes true if the context was initialized successfully
	bool m_isFullScreen = false;		//If true, the context is in fullscreen mode (default FALSE)
	bool m_isBorderLess = false;		//If true, the context's window is borderless (if supported) (default FALSE)
	bool m_isResizeAllowed = false;		//If true, the context's window CAN be resized (default FALSE)
	bool m_isCursorVisible = false;		//If true, the context window will show the mouse cursor (default FALSE)
	bool m_isDisplayOrtho = true;		//If true, the context's window uses a Ortho view. If false, a perspective view (default TRUE)
	bool m_keepRatio = true;			//If true, the context will keep the ratio when resizing (default TRUE)
	uint16 m_wWidth;					//Window Width
	uint16 m_wHeigth;					//Window Height
	Vec2<uint16> m_wPos;				//Window Position
	uint8 m_colorBits;					//The amount of bits this window can use (for color)
	//Holds data about the display (either Ortho or Perspective View)
	//If Ortho:			[0] - Left | [1] - Right  | [2] - Bottom  | [3] - Top  | [4] - zNear    | [5] - zFar
	//If Perspective:	[0] - FOV  | [1] - Aspect | [2] -  zNear  | [3] - zFar | [4] - Reserved | [5] - Reserved
	//Defaults to:		[0]  -100  | [1]    100   | [2]    -100   | [3]   100  | [4]    -100    | [5]     100
	GLfloat m_displayLimits[6];
	double m_Ratio = 0;					//Window Ratio
	double m_deltaOrthoX;				//Holds the total ortho of the X Axis (Right - Left)
	double m_deltaOrthoY;				//Holds the total ortho of the Y axis (Top - Bottom)
	String64 m_wTitle;					//Window Title;
	String64 m_errorDesc;				//Error description

	void setError(MWGLSErrorCodes Code, const char* Description)
	{
		m_errorCode = Code;
		m_errorDesc = Description;
	}

	void initMWGL(const char *title, uint16 width, uint16 height, uint16 posx, uint16 posy, uint8 colorbits, uint8 flags)
	{
		Flag8 tempFlag(flags);
		m_isFullScreen = tempFlag[0];
		if (!m_isFullScreen)
		{
			m_isBorderLess = tempFlag[1];
		}
		m_isResizeAllowed = tempFlag[2];
		m_isCursorVisible = tempFlag[3];
		m_wWidth = width;
		m_wHeigth = height;
		m_colorBits = colorbits;
		m_wTitle = title;
		m_wPos.setValues(posx, posy);
		setDisplayViewLimits(-100, 100, -100, 100, -100, 100);
	}

	//Specific variables for each OS
	#ifdef MWI_OS_WINDOWS
		HDC m_wHDC;		//GDI (Graphics Device Interface) Device Context
		HGLRC m_wRC;	//Rendering Context (permanent)
	#elif defined MWI_OS_LINUX
	
	#elif defined MWI_OS_OSX
	
	#endif


public:
	MWGLContext(const char *Title)
	{
		initMWGL(Title, 600, 600, 0, 0, 32, MWGLS_FLAG_DEFAULT);
	}
	MWGLContext(const char *Title, uint16 Width, uint16 Height)
	{
		initMWGL(Title, Width, Height, 0, 0, 32, MWGLS_FLAG_DEFAULT);
	}
	MWGLContext(const char *Title, uint16 Width, uint16 Height, uint8 Flags)
	{
		initMWGL(Title, Width, Height, 0, 0, 32, Flags);
	}
	MWGLContext(const char* Title, uint16 Width, uint16 Height, uint8 ColorBits, uint8 Flags)
	{
		initMWGL(Title, Width, Height, 0, 0, ColorBits, Flags);
	}
	MWGLContext(const char *Title, uint16 Width, uint16 Height, uint16 PosX, uint16 PosY, uint8 ColorBits, uint8 Flags)
	{
		initMWGL(Title, Width, Height, PosX, PosY, ColorBits, Flags);
	}
	MWGLContext(const char* Title, uint16 Width, uint16 Height, Vec2<uint16> &Pos, uint8 ColorBits, uint8 Flags)
	{
		initMWGL(Title, Width, Height, Pos._X, Pos._Y, ColorBits, Flags);
	}
	~MWGLContext(void)
	{
		if (m_contextSuccess)
		{
			destroyContext();
		}
	}

	void clearScene(void)
	{
#ifdef MWGLS_OPENGL
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
#elif defined MWGLS_DIRECTX

#endif
	}
	double getDeltaOrthoX(void)
	{
		return m_deltaOrthoX;
	}
	double getDeltaOrthoY(void)
	{
		return m_deltaOrthoY;
	}
	double getDisplayViewLimits(MWGLSViewLimits Limit)
	{
		return m_displayLimits[Limit];
	}
	uint8 getLastErrorCode(void)
	{
		return m_errorCode;
	}
	char* getLastError(void)
	{
		return m_errorDesc.getString();
	}
	double getRatio(void)
	{
		return m_Ratio;
	}
	uint16 getWindowWidth(void)
	{
		return m_wWidth;
	}
	uint16 getWindowHeight(void)
	{
		return m_wHeigth;
	}
	bool initializeWindow(void)
	{
#ifdef MWGLS_OPENGL											//OpenGL window Initializations
		glShadeModel(GL_SMOOTH);							//Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				//Black Background
		glClearDepth(1.0f);									//Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							//Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								//The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//Really Nice Perspective Calculations
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	#ifdef MWGLS_DEMO_TEST
		glBegin(GL_TRIANGLES);
			if (m_isDisplayOrtho)
			{
				//If Ortho:			[0] - Left | [1] - Right | [2] - Bottom  | [3] - Top | [4] - zNear | [5] - zFar
				glVertex3f(m_displayLimits[1] * 0.9, 0, 0);
				glVertex3f(0, m_displayLimits[3] * 0.9, 0);
				glVertex3f(m_displayLimits[0] * 0.9, 0, 0);
			}
			else
			{
				glVertex3f(10, 0, 0);
				glVertex3f(0, 10, 0);
				glVertex3f(-10, 0, 0);
			}
		glEnd();
	#elif defined MWGLS_DEMO_LINE_COORDS
		glBegin(GL_LINES);
		glColor3ub(128, 64, 0);
			if (m_isDisplayOrtho)
			{
				//If Ortho:			[0] - Left | [1] - Right | [2] - Bottom  | [3] - Top | [4] - zNear | [5] - zFar
				glVertex3f(m_displayLimits[0], 0, 0);
				glVertex3f(m_displayLimits[1], 0, 0);
				glVertex3f(0, m_displayLimits[2], 0);
				glVertex3f(0, m_displayLimits[3], 0);
				glVertex3f(m_displayLimits[0], m_displayLimits[2], 0);
				glVertex3f(m_displayLimits[1], m_displayLimits[3], 0);
				glVertex3f(m_displayLimits[1], m_displayLimits[2], 0);
				glVertex3f(m_displayLimits[0], m_displayLimits[3], 0);
			}
			else
			{
				glVertex3f(-10, 0, 0);
				glVertex3f(10, 0, 0);
				glVertex3f(0, -10, 0);
				glVertex3f(0, 10, 0);
				glVertex3f(-10, -10, 0);
				glVertex3f(10, 10, 0);
				glVertex3f(10, -10, 0);
				glVertex3f(-10, 10, 0);
			}
		glColor3ub(255, 255, 255);
		glEnd();
#endif
		return true;
#elif defined MWGLS_DIRECTX		//DirectX window Initializations
		return true;
#endif
	}
	bool isRatioKept(void)
	{
		return m_keepRatio;
	}
	void keepAspectRatio(bool KeepRatio)
	{
		if (m_keepRatio != KeepRatio)
		{
			m_keepRatio = KeepRatio;
			resizeContext(m_wWidth, m_wHeigth);
		}
	}
	void swapBuffers(void)
	{
	#ifdef MWI_OS_WINDOWS
		#ifdef MWGLS_OPENGL
			SwapBuffers(m_wHDC);
		#elif defined MWGLS_DIRECTX

		#endif
	#elif defined MWI_OS_LINUX
		#ifdef MWGLS_OPENGL

		#elif defined MWGLS_DIRECTX

		#endif
	#elif defined MWI_OS_OSX
		#ifdef MWGLS_OPENGL

		#elif defined MWGLS_DIRECTX

		#endif
	#endif
	}
	void setDisplayModeViewOrtho(void)
	{
		if (!m_isDisplayOrtho)
		{
			m_isDisplayOrtho = true;
			resizeContext(m_wWidth, m_wHeigth);
		}
	}
	void setDisplayModeViewPerspective(void)
	{
		if (m_isDisplayOrtho)
		{
			m_isDisplayOrtho = false;
			resizeContext(m_wWidth, m_wHeigth);
		}
	}
	void setDisplayViewLimits(double FOV, double Aspect, double zNear, double zFar)
	{
		m_displayLimits[0] = (GLfloat)FOV;
		m_displayLimits[1] = (GLfloat)Aspect;
		m_displayLimits[2] = (GLfloat)zNear;
		m_displayLimits[3] = (GLfloat)zFar;
		if (m_isDisplayOrtho)
		{
			setDisplayModeViewPerspective();
		}
	}
	void setDisplayViewLimits(double OrthoSquare)
	{
		m_displayLimits[0] = (GLfloat)-OrthoSquare;
		m_displayLimits[1] = (GLfloat)OrthoSquare;
		m_displayLimits[2] = (GLfloat)-OrthoSquare;
		m_displayLimits[3] = (GLfloat)OrthoSquare;
		m_displayLimits[4] = (GLfloat)-OrthoSquare;
		m_displayLimits[5] = (GLfloat)OrthoSquare;
		m_deltaOrthoY = m_deltaOrthoX = OrthoSquare + OrthoSquare;
		if (!m_isDisplayOrtho)
		{
			setDisplayModeViewOrtho();
		}
	}
	void setDisplayViewLimits(double Left, double Right, double Bottom, double Top, double zNear, double zFar)
	{
		m_displayLimits[0] = (GLfloat)Left;
		m_displayLimits[1] = (GLfloat)Right;
		m_displayLimits[2] = (GLfloat)Bottom;
		m_displayLimits[3] = (GLfloat)Top;
		m_displayLimits[4] = (GLfloat)zNear;
		m_displayLimits[5] = (GLfloat)zFar;
		m_deltaOrthoX = Right - Left;
		m_deltaOrthoY = Top - Bottom;
		if (!m_isDisplayOrtho)
		{
			setDisplayModeViewOrtho();
		}
	}
	void setDisplayViewLimits(double *Limits)
	{
		m_displayLimits[0] = (GLfloat)Limits[0];
		m_displayLimits[1] = (GLfloat)Limits[1];
		m_displayLimits[2] = (GLfloat)Limits[2];
		m_displayLimits[3] = (GLfloat)Limits[3];
		if (m_isDisplayOrtho)
		{
			m_displayLimits[4] = (GLfloat)Limits[4];
			m_displayLimits[5] = (GLfloat)Limits[5];
			m_deltaOrthoX = Limits[0] - Limits[1];
			m_deltaOrthoY = Limits[2] - Limits[3];
		}
	}
	void setTitle(char* NewTitle)
	{
		m_wTitle = NewTitle;
		SetWindowText(m_wHandle, m_wTitle.getString());
	}
	void toogleDisplayModeView(void)
	{
		m_isDisplayOrtho = !m_isDisplayOrtho;
		resizeContext(m_wWidth, m_wHeigth);
	}

	MWI_UNVSAL_APP_INSTANCE getInstance(void)
	{
		return m_instance;
	}
	MWI_UNVSAL_WINDOW_HANDLE getWindowHandle(void)
	{
		return m_wHandle;
	}



#ifdef MWI_OS_WINDOWS

	HDC getHDC(void)
	{
		return m_wHDC;
	}
	bool buildContext(void)
	{
		return buildContext(nullptr, MWInputHandler::InputInterceptor);
	}
	bool buildContext(MWI_UNVSAL_APP_INSTANCE hInst, LRESULT(__stdcall *MessageHandler)(HWND, UINT, WPARAM, LPARAM))
	{
		if (!m_contextSuccess)	//Only build if the current context wasn't built yet
		{
			const char* MBoxTitle;
	#ifdef MWGLS_OPENGL
			MBoxTitle = "[OpenGL ] - MWGLS";
	#elif defined MWGLS_DIRECTX
			MBoxTitle = "[DirectX] - MWGLS";
	#else 
			MBoxTitle = "[UNKNOWN] - MWGLS";
	#endif
			uint32 PFormat = NULL;			//Pixel Format for PFD validation
			uint32 WExStyle = NULL;			//Window Extended Style
			uint32 WStyle = NULL;			//Window Style
			WNDCLASS WindowClass;			//Windows Class Structure
			RECT WindowRect;				//Grabs Rectangle Upper Left / Lower Right Values. Used to create the window
			WindowRect.left= 0;				//Set Left Value To 0
			WindowRect.right= m_wWidth;		//Set Right Value To Requested Width
			WindowRect.top = 0;				//Set Top Value To 0
			WindowRect.bottom = m_wHeigth;	//Set Bottom Value To Requested Height
			
			if (hInst)
			{
				m_instance = hInst;			//Saving our instance
			}
			else
			{
				m_instance = GetModuleHandle(NULL);	//User don't supplied an App Instance. Catch it
			}

			WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
			WindowClass.lpfnWndProc = (WNDPROC)MessageHandler;		// WndProc Handles Messages
			WindowClass.cbClsExtra = 0;								// No Extra Window Data
			WindowClass.cbWndExtra = 0;								// No Extra Window Data
			WindowClass.hInstance = m_instance;						// Set The Instance
			WindowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);		// Load The Default Icon
			WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// Load The Arrow Pointer
			WindowClass.hbrBackground = NULL;						// No Background Required For GL
			WindowClass.lpszMenuName = NULL;						// We Don't Want A Menu
			WindowClass.lpszClassName = m_wTitle.getString();		// Set The Class Name

			if (!RegisterClass(&WindowClass))						// Attempt To Register The Window Class
			{
				MWMessageBoxHandler::Message("Failed to Register Window Class!", MBoxTitle, MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_WARNING);
				setError(MWGLS_ERROR_INIT, "Failed to Register WC");
				return false;											// Return FALSE
			}

			//Testing fullscreen compability		// ------------------------------------------------------------------------------------------------------
			if (m_isFullScreen)
			{
				DEVMODE ScreenSettings;
				memset(&ScreenSettings, 0, sizeof(ScreenSettings));		//Setting it all to 0, for safety...
				ScreenSettings.dmSize = sizeof(ScreenSettings);			//'DEVMODEA' size
				ScreenSettings.dmPelsWidth = m_wWidth;
				ScreenSettings.dmPelsHeight = m_wHeigth;
				ScreenSettings.dmBitsPerPel = m_colorBits;
				ScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
				//Trying to change the display to fullscreen mode
				if (ChangeDisplaySettings(&ScreenSettings, CDS_FULLSCREEN))
				{
					//If we fall here, this means we failed to set the window to fullscreen, since 'ChangeDisplaySeetingsA' returns == 0 if it was successful
					//Ask the user if it wants another window instead...
					if (MWMessageBoxHandler::Reponse("Could not set window to Fullscreen Mode.\n\nChange to Windowed instead?", MBoxTitle, MessageBoxButtons::MBB_YES_NO, MessageBoxIcons::MBI_WARNING, MessageBoxResponses::MBR_YES))
					{
						//The user wants to change to windowed
						m_isFullScreen = false;
						WExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
						WStyle = WS_OVERLAPPEDWINDOW;
					}
					else
					{
						//Looks like the user only will use this application on fullscreen... Abort context building
						setError(MWGLS_ERROR_FEATURE_UNAVAILABLE, "Could not set the window to fullscreen mode!");
						return false;
					}
				}
				else
				{
					//Fullscreen mode ready!
					WExStyle = WS_EX_APPWINDOW;
					WStyle = WS_POPUP;
				}
			}
			else
			{
				WExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
				WStyle = WS_OVERLAPPEDWINDOW;
			}
			ShowCursor(m_isCursorVisible);

			AdjustWindowRectEx(&WindowRect, WStyle, false, WExStyle);	//NOTE: That 'false' there defines that we DON'T WANT a menu

			//Now, creating the window (finally)	// ------------------------------------------------------------------------------------------------------
			if (!(m_wHandle = CreateWindowEx(WExStyle,			//The extented style
				m_wTitle.getString(),							//Class name
				m_wTitle.getString(),							//Window Title
				WStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,		//Our defined window style | the required window style
				m_wPos._X,										//Window position X
				m_wPos._Y,										//Window position Y
				WindowRect.right - WindowRect.left,				//Compute Window Width
				WindowRect.bottom - WindowRect.top,				//Compute Window Heigth
				NULL,											//We have any parent window?
				NULL,											//We have menus?
				m_instance,										//The application instance (required)
				NULL											//Do we pass anything to 'WM_CREATE' flag?
				)))
			{
				//If we fall here, we failed to create the window!
				setError(MWGLS_ERROR_INIT, "Could not create window!");
				MWMessageBoxHandler::Message("Error Creating Window!\nContext creation aborted", MBoxTitle, MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_ERROR);
				destroyContext();
				return false;
			}
			//Continuing creating window...			// ------------------------------------------------------------------------------------------------------
			PIXELFORMATDESCRIPTOR PixelFormat = {	sizeof(PIXELFORMATDESCRIPTOR),				//The size of this structure
													1,											//Version Number
													PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	//Format MUST: support window, OpenGL and double buffering
													PFD_TYPE_RGBA,								//Support RGBA
													m_colorBits,								//Our color depth
													0, 0, 0, 0, 0, 0,							//Ignored Color Bits
													0,											//No Alpha Buffer
													0,											//Shift Bit Ignored
													0,											//No Accumulation Buffer
													0, 0, 0, 0,									//Accumulation Bits Ignored
													16,											//16Bit Z-Buffer (Depth Buffer)  
													0,											//No Stencil Buffer
													0,											//No Auxiliary Buffer
													PFD_MAIN_PLANE,								//Main Drawing Layer
													0,											//Reserved
													0, 0, 0										//Layer Masks Ignored
												};

			//Error checking and final steps		// ------------------------------------------------------------------------------------------------------
			if (!(m_wHDC = GetDC(m_wHandle)))							//Failed to create a device context
			{
				setError(MWGLS_ERROR_INIT, "Failed to create device context (HDC)");
				MWMessageBoxHandler::Message("Error Creating a valid GL Device Context!\nContext creation aborted", MBoxTitle, MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_ERROR);
				destroyContext();
				return false;
			}
			if (!(PFormat = ChoosePixelFormat(m_wHDC, &PixelFormat)))	//Failed to find a matching pixel format
			{
				setError(MWGLS_ERROR_INIT, "Failed to FIND a valid Pixel Format");
				MWMessageBoxHandler::Message("Error finding a matching Pixel Format!\nContext creation aborted", MBoxTitle, MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_ERROR);
				destroyContext();
				return false;
			}
			if (!SetPixelFormat(m_wHDC, PFormat, &PixelFormat))			//Failed to set this pixel format
			{
				setError(MWGLS_ERROR_INIT, "Failed to SET this valid Pixel Format");
				MWMessageBoxHandler::Message("Error setting the Pixel Format!\nContext creation aborted", MBoxTitle, MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_ERROR);
				destroyContext();
				return false;
			}
	#ifdef MWGLS_OPENGL
			if (!(m_wRC = wglCreateContext(m_wHDC)))					//Failed to get a valid Rendering Context
			{
				setError(MWGLS_ERROR_INIT, "Failed to CREATE Rendering Context (HGLRC)");
				MWMessageBoxHandler::Message("Error creating a valid Rendering Context!\nContext creation aborted", MBoxTitle, MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_ERROR);
				destroyContext();
				return false;
			}
			if (!wglMakeCurrent(m_wHDC, m_wRC))							//Failed to activate the Rendering Context!
			{
				setError(MWGLS_ERROR_INIT, "Failed to ACTIVATE Rendering Context (HGLRC)");
				MWMessageBoxHandler::Message("Error activating the Rendering Context!\nContext creation aborted", MBoxTitle, MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_ERROR);
				destroyContext();
				return false;
			}
	#elif defined MWGLS_DIRECTX

	#endif
			ShowWindow(m_wHandle, SW_SHOW);			//Show our magnificent window...
			SetForegroundWindow(m_wHandle);			//...give it the main role in this scene...
			SetFocus(m_wHandle);					//...point the spotlight towards it...
			resizeContext(m_wWidth, m_wHeigth);		//...ask mom to get the camera...

			if (!(m_contextSuccess = initializeWindow()))
			{
				//...and we fucked up.........
				setError(MWGLS_ERROR_INIT, "Failed to Initialize Window (HGLRC)");
				MWMessageBoxHandler::Message("Error Initializing OpenGL Window!\nContext creation aborted", MBoxTitle, MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_ERROR);
				destroyContext();
				return false;
			}
			//...and now start the show!
			setError(MWGLS_ERROR_NONE, "No errors recorded!");
			return true;
		}
		setError(MWGLS_ERROR_CONTEXT_ALREADY_BUILT, "Context was already built!");
		return false;
	}

	void destroyContext(void)
	{
		if (m_isFullScreen)			//If we are in fullscreen, we need to switch to the desktop first
		{
			ChangeDisplaySettings(NULL, 0);
			if (!m_isCursorVisible)	//Bring the cursor back!
			{
				ShowCursor(true);
			}
		}
		if (m_wRC)												//Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL, NULL))					//Are We Able To Release The DC And RC Contexts?
			{
				MWMessageBoxHandler::Message("Failed to release Device and Rendering Contexts!", "[MWGLS] - Fatal Error", MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_WARNING);
			}
			if (!wglDeleteContext(m_wRC))						//Are We Able To Delete The RC?
			{
				MWMessageBoxHandler::Message("Failed to release Rendering Contexts!", "[MWGLS] - Fatal Error", MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_WARNING);
			}
			m_wRC = NULL;										//Set RC To NULL
		}

		if (m_wHDC && !ReleaseDC(m_wHandle, m_wHDC))			//Are We Able To Release The DC
		{
			MWMessageBoxHandler::Message("Failed to release Device Contexts!", "[MWGLS] - Fatal Error", MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_WARNING);
			m_wHDC = NULL;										//Set DC To NULL
		}

		if (m_wHDC && !DestroyWindow(m_wHandle))				//Are We Able To Destroy The Window?
		{
			MWMessageBoxHandler::Message("Failed to release Window Handle!", "[MWGLS] - Fatal Error", MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_WARNING);
			m_wHandle = NULL;									//Set wHandle To NULL
		}

		if (!UnregisterClass(m_wTitle.getString(), m_instance))				//Are We Able To Unregister Class?
		{
			MWMessageBoxHandler::Message("Failed to Unregister Window Class!", "[MWGLS] - Fatal Error", MessageBoxButtons::MBB_OK, MessageBoxIcons::MBI_WARNING);
			m_instance  = NULL;									// Set app instance To NULL
		}
		m_contextSuccess = false;
	}
	void resizeContext(uint16 Width, uint16 Height)
	{
		if (Height == 0)
		{
			Height = 1;
		}
		m_wHeigth = Height;
		m_wWidth = Width;
	#ifdef MWGLS_OPENGL
			glViewport(0, 0, m_wWidth, m_wHeigth);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			mwVec2d PreOrthoLR(m_displayLimits[0], m_displayLimits[1]);
			double PreOrthoTop = m_displayLimits[3];
			if (m_keepRatio)
			{
				m_Ratio = (double)m_wWidth / (double)m_wHeigth;
				if (m_wWidth >= m_wHeigth)
				{
					PreOrthoLR *= m_Ratio;
				}
				else
				{
					PreOrthoTop = PreOrthoTop / m_Ratio;
				}
			}
			if (m_isDisplayOrtho)
			{
				glOrtho(PreOrthoLR._X, PreOrthoLR._Y, m_displayLimits[2], m_displayLimits[3], m_displayLimits[4], PreOrthoTop);
			}
			else
			{
				//gluPerspective(m_displayLimits[0], m_displayLimits[1], m_displayLimits[2], m_displayLimits[3]);
			}
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
	#elif defined MWGLS_DIRECTX
			
	#endif
	}
#elif defined MWI_OS_LINUX

#elif defined MWI_OS_OSX

#endif
};

#endif;	//Moon Wiz GL Starter