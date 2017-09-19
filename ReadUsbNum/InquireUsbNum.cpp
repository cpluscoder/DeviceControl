#include "StdAfx.h"
#include "InquireUsbNum.h"

#ifndef IOCTL_SCSI_PASS_THROUGH_DIRECT
#define IOCTL_SCSI_PASS_THROUGH_DIRECT	0x4D014
#define SCSI_IOCTL_DATA_IN				1
typedef struct _SCSI_PASS_THROUGH_DIRECT 
{
	USHORT Length;
	UCHAR ScsiStatus;
	UCHAR PathId;
	UCHAR TargetId;
	UCHAR Lun;
	UCHAR CdbLength;
	UCHAR SenseInfoLength;
	UCHAR DataIn;
	ULONG DataTransferLength;
	ULONG TimeOutValue;
	PVOID DataBuffer;
	ULONG SenseInfoOffset;
	UCHAR Cdb[16];
} SCSI_PASS_THROUGH_DIRECT, *PSCSI_PASS_THROUGH_DIRECT;
#endif

using namespace std;

CInquireUsbNum::CInquireUsbNum(void)
{
}


CInquireUsbNum::~CInquireUsbNum(void)
{
}

bool CInquireUsbNum::Inquire(HANDLE hDevice, void *pCdb, size_t stCdbLength, void *szBuffer, DWORD dwBufLength, ULONG &nResultLength)
{
	unsigned char cmd[sizeof(SCSI_PASS_THROUGH_DIRECT) + 96] = {0};
	SCSI_PASS_THROUGH_DIRECT *pCmd = (SCSI_PASS_THROUGH_DIRECT *)cmd;

	memcpy(pCmd->Cdb, pCdb, stCdbLength);
	pCmd->Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
	pCmd->TargetId = 0x01;
	pCmd->CdbLength = sizeof(pCmd->Cdb);
	pCmd->SenseInfoLength = sizeof(cmd) - sizeof(SCSI_PASS_THROUGH_DIRECT);
	pCmd->DataIn = SCSI_IOCTL_DATA_IN;
	pCmd->DataTransferLength = dwBufLength;
	pCmd->TimeOutValue = 600;
	pCmd->DataBuffer = szBuffer;
	pCmd->SenseInfoOffset = sizeof(SCSI_PASS_THROUGH_DIRECT);

	TRACE("sizeof(cmd):%d\n", sizeof(cmd));
	DWORD dwBytesReturned = 0;

	BOOL bSuccess = ::DeviceIoControl(
		hDevice,
		IOCTL_SCSI_PASS_THROUGH_DIRECT,
		(LPVOID)&cmd,
		sizeof(cmd),
		(LPVOID)&cmd,
		sizeof(cmd),
		&dwBytesReturned,
		NULL);

#ifdef _DEBUG
	DWORD err = ::GetLastError();
	if(err != ERROR_SUCCESS)
	{
		TRACE("carry_cdb(): last error = %08X\n", err);
	}
#endif

	nResultLength = pCmd->DataTransferLength;

	return bSuccess ? true : false;
}

bool CInquireUsbNum::GetUsbNum(const std::string &strDeviceName, std::string &strDeviceNumber)
{
	//char szDeviceName[128] = "\\\\?\\USBSTOR#Disk&Ven_NISEC&Prod_TCG-01&Rev_5.00#7&2dca610a&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}";
	
	HANDLE hDevice = ::CreateFile(
		strDeviceName.c_str(), //szDeviceName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_DEVICE,
		NULL);
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		TRACE("Cannot access the drive");
		return false;
	}
	
	ULONG nResultLength = 0;
	unsigned char szData[0x10000];
	ZeroMemory(szData, sizeof(szData));
	unsigned char szInquiry[3] = {254, 111, 1};
	bool bSuccess = Inquire(hDevice, szInquiry, sizeof(szInquiry), &szData, sizeof(szData), nResultLength);
	if(bSuccess && nResultLength > 0)
	{
		strDeviceNumber.assign((char *)szData, nResultLength);
		TRACE("к╟©ьел╠Ю╨е:%s\n", strDeviceNumber.c_str());
	}

	::CloseHandle(hDevice);

	return bSuccess;
}
