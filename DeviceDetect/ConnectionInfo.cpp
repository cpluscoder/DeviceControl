#include "StdAfx.h"
#include "ConnectionInfo.h"

bool DeviceDetectLibrary::operator < (const DeviceDetectLibrary::CConnectionInfo & left, const DeviceDetectLibrary::CConnectionInfo & right)
{
	return (left.FriendlyName < right.FriendlyName);
}

bool DeviceDetectLibrary::operator == (const DeviceDetectLibrary::CConnectionInfo & left, const DeviceDetectLibrary::CConnectionInfo & right)
{
	return (left.FriendlyName == right.FriendlyName) && (left.PhysicalDeviceName == right.PhysicalDeviceName);
}
