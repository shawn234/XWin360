#include "DaisyWin.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <Wincodec.h>
#include <atlbase.h>
#include <atlimage.h>
#include "resource.h"
#pragma comment(lib, "d2d1.lib")

int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 500;
int WINDOW_X = 0; 
int WINDOW_Y = 0; 

void DaisyWin::drawWin(HINSTANCE hI){
    /*
    HWND hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED, L"DaisyWindow",  \
                                         L"daisywin", WS_POPUP ,  \
                                         WINDOW_X, WINDOW_Y,  \
                                         WINDOW_WIDTH, WINDOW_HEIGHT,  \
                                         NULL, NULL, GetModuleHandle(0), NULL); 
	HDC hdc = GetDC(hwnd);
	RECT rect;
	GetClientRect(hwnd, &rect);
	char * text = "this Window cannot be used";
	DrawTextA(hdc, text, strlen(text), &rect, DT_CENTER | DT_VCENTER);
	ReleaseDC(hwnd, hdc);
    */
	//HWND hWnd = CreateWindowEx(WS_EX_LAYERED, L"DaisyWheel", L"wheel", 0,
	//	CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hI, NULL);
	// Load our PNG image
	//CImage img;
	//img.Load(L"C:\\Users\\Shawn\\Documents\\Visual Studio 2012\\Projects\\XBOX360Test\\DaisyWheel_Background.bmp");
	//img.LoadFromResource(hI,L"IDB_BITMAP1");
	//img.LoadFromResource(hI,MAKEINTRESOURCE(IDB_BITMAP1));
	//img.Load(L"DaisyWheel_Background.bmp");
	HWND hWnd = CreateWindowEx(WS_EX_LAYERED, L"DaisyWin", L"fwoieg", 0,
     CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hI, NULL);

// Load PNG
CImage img;
img.LoadFromResource(hI,MAKEINTRESOURCE(IDB_BITMAP10));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);

// Call UpdateLayeredWindow
BLENDFUNCTION blend = {0};
blend.BlendOp = AC_SRC_OVER;
blend.SourceConstantAlpha = 0;// half transparent
blend.AlphaFormat = AC_SRC_ALPHA;
POINT ptLocation = {0, 0};
SIZE szWnd = {nWidth, nHeight};
POINT ptSrc = {0, 0};
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
ShowWindow(hWnd, SW_SHOW);

SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
}
/*
// Creates a stream object initialized with the data from an executable resource.
IStream * CreateStreamOnResource(LPCTSTR lpName, LPCTSTR lpType)
{
// initialize return value
IStream * ipStream = NULL;

// find the resource
HRSRC hrsrc = FindResource(NULL, lpName, lpType);
if (hrsrc == NULL)
goto Return;

// load the resource
DWORD dwResourceSize = SizeofResource(NULL, hrsrc);
HGLOBAL hglbImage = LoadResource(NULL, hrsrc);
if (hglbImage == NULL)
goto Return;

// lock the resource, getting a pointer to its data
LPVOID pvSourceResourceData = LockResource(hglbImage);
if (pvSourceResourceData == NULL)
goto Return;

// allocate memory to hold the resource data
HGLOBAL hgblResourceData = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
if (hgblResourceData == NULL)
goto Return;

// get a pointer to the allocated memory
LPVOID pvResourceData = GlobalLock(hgblResourceData);
if (pvResourceData == NULL)
goto FreeData;

// copy the data from the resource to the new memory block
CopyMemory(pvResourceData, pvSourceResourceData, dwResourceSize);
GlobalUnlock(hgblResourceData);

// create a stream on the HGLOBAL containing the data
if (SUCCEEDED(CreateStreamOnHGlobal(hgblResourceData, TRUE, &ipStream)))
goto Return;

FreeData:
// couldn't create stream; free the memory
GlobalFree(hgblResourceData);

Return:
// no need to unlock or free the resource
return ipStream;
}
// Loads a PNG image from the specified stream (using Windows Imaging Component).
IWICBitmapSource * LoadBitmapFromStream(IStream * ipImageStream)
{
// initialize return value
IWICBitmapSource * ipBitmap = NULL;

// load WIC's PNG decoder
IWICBitmapDecoder * ipDecoder = NULL;
if (FAILED(CoCreateInstance(CLSID_WICPngDecoder, NULL, CLSCTX_INPROC_SERVER, __uuidof(ipDecoder), reinterpret_cast<void**>(&ipDecoder))))
goto Return;

// load the PNG
if (FAILED(ipDecoder->Initialize(ipImageStream, WICDecodeMetadataCacheOnLoad)))
goto ReleaseDecoder;

// check for the presence of the first frame in the bitmap
UINT nFrameCount = 0;
if (FAILED(ipDecoder->GetFrameCount(&nFrameCount)) || nFrameCount != 1)
goto ReleaseDecoder;

// load the first frame (i.e., the image)
IWICBitmapFrameDecode * ipFrame = NULL;
if (FAILED(ipDecoder->GetFrame(0, &ipFrame)))
goto ReleaseDecoder;

// convert the image to 32bpp BGRA format with pre-multiplied alpha
//   (it may not be stored in that format natively in the PNG resource,
//   but we need this format to create the DIB to use on-screen)
WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, ipFrame, &ipBitmap);
ipFrame->Release();

ReleaseDecoder:
ipDecoder->Release();
Return:
return ipBitmap;
}

// Creates a 32-bit DIB from the specified WIC bitmap.
HBITMAP CreateHBITMAP(IWICBitmapSource * ipBitmap)
{
// initialize return value
HBITMAP hbmp = NULL;

// get image attributes and check for valid image
UINT width = 0;
UINT height = 0;
if (FAILED(ipBitmap->GetSize(&width, &height)) || width == 0 || height == 0)
goto Return;

// prepare structure giving bitmap information (negative height indicates a top-down DIB)
BITMAPINFO bminfo;
ZeroMemory(&bminfo, sizeof(bminfo));
bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
bminfo.bmiHeader.biWidth = width;
bminfo.bmiHeader.biHeight = -((LONG) height);
bminfo.bmiHeader.biPlanes = 1;
bminfo.bmiHeader.biBitCount = 32;
bminfo.bmiHeader.biCompression = BI_RGB;

// create a DIB section that can hold the image
void * pvImageBits = NULL;
HDC hdcScreen = GetDC(NULL);
hbmp = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pvImageBits, NULL, 0);
ReleaseDC(NULL, hdcScreen);
if (hbmp == NULL)
goto Return;

// extract the image into the HBITMAP
const UINT cbStride = width * 4;
const UINT cbImage = cbStride * height;
if (FAILED(ipBitmap->CopyPixels(NULL, cbStride, cbImage, static_cast<BYTE *>(pvImageBits))))
{
// couldn't extract image; delete HBITMAP
DeleteObject(hbmp);
hbmp = NULL;
}

Return:
return hbmp;
}

// Loads the PNG containing the splash image into a HBITMAP.
HBITMAP LoadSplashImage()
{
HBITMAP hbmpSplash = NULL;

// load the PNG image data into a stream
IStream * ipImageStream = CreateStreamOnResource(L"DaisyWheel_Background.png", _T("PNG"));
if (ipImageStream == NULL)
goto Return;

// load the bitmap with WIC
IWICBitmapSource * ipBitmap = LoadBitmapFromStream(ipImageStream);
if (ipBitmap == NULL)
goto ReleaseStream;

// create a HBITMAP containing the image
hbmpSplash = CreateHBITMAP(ipBitmap);
ipBitmap->Release();

ReleaseStream:
ipImageStream->Release();
Return:
return hbmpSplash;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
LPCTSTR szWindowClass = L"TransparentClass";

// Register class
WNDCLASSEX wcex = {0};

wcex.cbSize = sizeof(WNDCLASSEX);
wcex.lpfnWndProc    = DefWindowProc;
wcex.hInstance      = hInstance;
wcex.lpszClassName  = szWindowClass;

RegisterClassEx(&wcex);

HWND hWnd = CreateWindowEx(WS_EX_LAYERED, szWindowClass, 0, WS_OVERLAPPEDWINDOW,
CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

int width = 512;
int height = 512;
HBITMAP hbmp = LoadSplashImage();

HDC hdcScreen = GetDC(0);
HDC hdc = CreateCompatibleDC(hdcScreen);
ReleaseDC(0, hdcScreen);
HBITMAP hbmpold = (HBITMAP)SelectObject(hdc, hbmp);

POINT dcOffset = {0, 0};
SIZE size = {width, height};
BLENDFUNCTION bf;
bf.BlendOp = AC_SRC_OVER;
bf.BlendFlags = 0;
bf.SourceConstantAlpha = 255;
bf.AlphaFormat = AC_SRC_ALPHA;
UpdateLayeredWindow(hWnd, 0, 0, &size, hdc, &dcOffset, 0, &bf, ULW_ALPHA);
SelectObject(hdc, hbmpold);
DeleteDC(hdc);
DeleteObject(hbmp);

ShowWindow(hWnd, SW_SHOW);

MSG msg;

// Main message loop:
while (GetMessage(&msg, NULL, 0, 0))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}

return (int)msg.wParam;
}
*/