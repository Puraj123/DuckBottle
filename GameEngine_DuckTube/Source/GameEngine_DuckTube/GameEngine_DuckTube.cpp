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
bool instancesRunning(LPCTSTR classname);
void CheckDiskCapacity();
void CheckMemoryCapacity();
void CheckCPUStats();

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
bool instancesRunning(LPCTSTR classname) {

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
	//int msgboxID = MessageBox(g_hWnd,text, _T("Cpu Specs"),NULL);
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, PWSTR pCmdLine, int nCmdShow)
{
	instancesRunning("Hello World");
	CheckDiskCapacity();
	CheckMemoryCapacity();
	CheckCPUStats();
	return 0;
}
