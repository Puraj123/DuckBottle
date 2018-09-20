//John Paul Valdez 101055854
//Minxu Huang 101074668
//Puraj Desai 101093244

#include "stdafx.h"
#include <atlhandler.h>
#include <iostream>
#include <winnt.h>



HWND g_hWnd;

using namespace std;

//Initializing Function
bool instancesRunning(LPCSTR classname, LPCWSTR windowTitle);
void CheckDiskCapacity();
void CheckMemoryCapacity();
void CheckCPUStats();
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateWindowInstance(const HINSTANCE &hInstance, const wchar_t  CLASS_NAME[14]);

//initializing Struct for Processor Information Function
typedef struct _PROCESSOR_POWER_INFORMATION {
	ULONG Number;
	ULONG MaxMhz;
	ULONG CurrentMhz;
	ULONG MhzLimit;
	ULONG MaxIdleState;
	ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

//Checks if an instance of the application is already running or not.
bool instancesRunning(LPCSTR classname, LPCWSTR windowTitle) {

	HANDLE handles = CreateMutex(NULL, TRUE, classname);
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		cout << "more than one instances is running" << endl;
		return false;
	}
	else
	{
		cout << "no multiple instances is running" << endl;
	}
	return true;
	CloseHandle(handles);
}

//Checks if device has required disk space needed.
void CheckDiskCapacity()
{
	wchar_t text[200];
	ULARGE_INTEGER ulFreeSpaceAvailable;
	ULARGE_INTEGER ulTotalSpace;
	ULARGE_INTEGER ulTotalFreeSpace;

	::GetDiskFreeSpaceEx(_T("c:\\"),
		&ulFreeSpaceAvailable,
		&ulTotalSpace,
		&ulTotalFreeSpace);

	if (300 > (ulTotalFreeSpace.QuadPart / 1024 * 1024))
	{ 
		swprintf(text, 200, L"Insufficient disk space \nDisk space required: 300 \nDisk space available: %llu MB.", ulTotalFreeSpace.QuadPart / (1024 * 1024));
	}
	else
	{
		swprintf(text, 200, L"Sufficient disk space \nDisk space required: 300 \nDisk space available: %llu MB.", ulTotalFreeSpace.QuadPart / (1024 * 1024));
	}
	cout <<"Disk Capacity:"<< text<<endl;
	//int msgboxID = MessageBox(g_hWnd, text, _T("Disk Space Available"), NULL);
}

//Displays the size of physical and virtual memory available to use.
void CheckMemoryCapacity()
{
	wchar_t text[200];
	MEMORYSTATUSEX memoryStruct = {};
	memoryStruct.dwLength = sizeof(memoryStruct);

	GlobalMemoryStatusEx(&memoryStruct);

	int bufferSize = swprintf(text, 200, L"Physical Memory Available: %I64d MB \nVirtual Memory Available: %I64d MB", memoryStruct.ullAvailPhys / (1024 * 1024), memoryStruct.ullAvailVirtual / (1024 * 1024));
	cout <<"Memory Capacity:"<< text << endl;
	//int msgboxID = MessageBox(g_hWnd, text, _T("Memory Available"), NULL);

}

//Displays the CPU speed and architecture of the device
void CheckCPUStats()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	// allocate buffer to get info for each processor
	const int size = sysInfo.dwNumberOfProcessors * sizeof(PROCESSOR_POWER_INFORMATION);
	LPBYTE pBuffer = new BYTE[size];
	//CallNtPowerInformation(ProcessorInformation, NULL, 0, pBuffer, size);
	PPROCESSOR_POWER_INFORMATION ppi = (PPROCESSOR_POWER_INFORMATION)pBuffer;

	wchar_t text[200];
	swprintf(text, 200, L"Proccesor Speed: %d MHz \nCPU Architecture Value: %u", ppi->MaxMhz, sysInfo.wProcessorArchitecture);
	cout <<"CPU:"<< text << endl;
	int msgboxID = MessageBox(g_hWnd,(LPCSTR)text, _T("Cpu Specs"),NULL);
}


int main(HINSTANCE hInstance, HINSTANCE hPrevInstace, PWSTR pCmdLine, int nCmdShow)
{

	const wchar_t CLASS_NAME[] = L"Editor Window";
	const wchar_t WINDOW_TITLE[] = L"Tube Engine";

	// check if window is already open
	if (instancesRunning((LPCSTR)CLASS_NAME,WINDOW_TITLE))
	{
		return 0;
	}

	// create wc instance
	g_hWnd = CreateWindowInstance(hInstance, CLASS_NAME);

	if (g_hWnd == NULL)
		return 0; //create window failed

	ShowWindow(g_hWnd, nCmdShow);
	//instancesRunning("Hello World");
	CheckDiskCapacity();
	CheckMemoryCapacity();
	CheckCPUStats();

	// MESSAGE LOOP
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

HWND CreateWindowInstance(const HINSTANCE &hInstance, const wchar_t  CLASS_NAME[14])
{
	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = (LPCSTR)CLASS_NAME;

	RegisterClass(&wc);

	// creating window
	return CreateWindowEx(
		0,
		(LPCSTR)CLASS_NAME,
		"Tube Engine",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
}


//Handling window messages.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY: //calling quit function
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));
		EndPaint(hwnd, &ps);
		return 0;
	}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
