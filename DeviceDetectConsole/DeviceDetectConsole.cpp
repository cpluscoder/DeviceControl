// AutodetectConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DeviceDetectLog.h"
#include "TestDeviceWatcherObserver.h"
#include "DeviceWatcher.h"


int main(int argc, char* argv[])
{
	LogMessage("");
	LogMessage("Start detecting...");

	CTestDeviceWatcherObserver crier;
	DeviceDetectLibrary::CDeviceWatcher::Pointer pDeviceWatcher = DeviceDetectLibrary::CDeviceWatcher::Create(&crier);

	//pDeviceWatcher.reset(new DeviceDetectLibrary::CDeviceWatcher(&crier));

	pDeviceWatcher->Start();

	getchar();

	LogMessage("End detecting...");
	return 0;
}

