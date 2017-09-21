#include "StdAfx.h"
#include "InquireNumber.h"

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

CInquireNumber::CInquireNumber(void)
{
}


CInquireNumber::~CInquireNumber(void)
{
}

bool CInquireNumber::Inquire(HANDLE hDevice, string &strDeviceNumber)
{
	unsigned char cmd[sizeof(SCSI_PASS_THROUGH_DIRECT) + 96] = {0};
	SCSI_PASS_THROUGH_DIRECT *pCmd = (SCSI_PASS_THROUGH_DIRECT *)cmd;

	unsigned char szInquiry[3] = {254, 111, 1};
	memcpy(pCmd->Cdb, szInquiry, sizeof(szInquiry));

	unsigned char szData[0x10000];
	ZeroMemory(szData, sizeof(szData));

	pCmd->Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
	pCmd->TargetId = 0x01;
	pCmd->CdbLength = sizeof(pCmd->Cdb);
	pCmd->SenseInfoLength = sizeof(cmd) - sizeof(SCSI_PASS_THROUGH_DIRECT);
	pCmd->DataIn = SCSI_IOCTL_DATA_IN;
	pCmd->DataTransferLength = sizeof(szData);
	pCmd->TimeOutValue = 600;
	pCmd->DataBuffer = szData;
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

	if(szData != nullptr && pCmd->DataTransferLength > 0)
	{
		strDeviceNumber.assign((char *)szData, pCmd->DataTransferLength);
	}

	return !strDeviceNumber.empty();
}

bool CInquireNumber::GetDeviceNumber(const string &strDeviceName, string &strDeviceNumber)
{
	strDeviceNumber.clear();

	HANDLE hDevice = ::CreateFile(
		strDeviceName.c_str(), 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_DEVICE,
		NULL);
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		OutputDebugString("Cannot access the drive");
		return false;
	}
	
	//bool bSuccess = Inquire(hDevice, szInquiry, sizeof(szInquiry), &szData, sizeof(szData), strDeviceNumber);
	Inquire(hDevice, strDeviceNumber);

	::CloseHandle(hDevice);

	return !strDeviceNumber.empty();
}
