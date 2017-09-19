#pragma once

#include <string>

class CInquireUsbNum
{
public:
	CInquireUsbNum(void);
	virtual ~CInquireUsbNum(void);

	bool GetUsbNum(const std::string &strDeviceName, std::string &strDeviceNumber);

protected:
	bool Inquire(HANDLE hDevice, void *pCdb, size_t stCdbLength, void *szBuffer, DWORD dwBufLength, ULONG &nResultLength);
};

