#pragma once

#include <string>

class CInquireNumber
{
public:
	CInquireNumber(void);
	virtual ~CInquireNumber(void);

	bool GetDeviceNumber(const std::string &strDeviceName, std::string &strDeviceNumber);

protected:
	bool Inquire(HANDLE hDevice, std::string &strDeviceNumber);
};

