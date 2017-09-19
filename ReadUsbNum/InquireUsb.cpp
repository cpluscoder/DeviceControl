#include "StdAfx.h"
#include "scsi_struct.h"
#include "InquireUsb.h"


//ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
//ON_CBN_SELCHANGE(IDC_DRIVES_COMBO, OnSelchangeDrivesCombo)

/////////////////////////////////////////////////////////////////////////////
// CMyregionDlg message handlers

//BOOL CMyregionDlg::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	// Set the icon for this dialog.  The framework does this automatically
//	//  when the application's main window is not a dialog
//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon
//
//	PopulateDrivesList();
//
//	UpdateDeviceInfos();
//
//	return TRUE;  // return TRUE  unless you set the focus to a control
//}


//void CMyregionDlg::OnBtnRefresh()
//{
//	PopulateDrivesList();
//}

bool carry_cdb(HANDLE device, void *cdb, UCHAR cdb_length, void *buffer, DWORD buffer_length, int data_in = SCSI_IOCTL_DATA_IN)
{
	DWORD returned;

	// size of SCSI_PASS_THROUGH + 96 bytes for sense data
	unsigned char cmd[sizeof(SCSI_PASS_THROUGH_DIRECT) + 96] = {0};

	// shortcut to the buffer
	SCSI_PASS_THROUGH_DIRECT *pcmd = (SCSI_PASS_THROUGH_DIRECT *)cmd;

	// Copy the CDB to the SCSI_PASS_THROUGH structure
	memcpy(pcmd->Cdb, cdb, cdb_length);

	pcmd->Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
	//pcmd->CdbLength = cdb_length;
	pcmd->CdbLength = sizeof(pcmd->Cdb);//0x10;

	pcmd->TargetId = 0x01;

	pcmd->SenseInfoLength = sizeof(cmd) - sizeof(SCSI_PASS_THROUGH_DIRECT);
	pcmd->DataIn = data_in;
	pcmd->DataTransferLength = buffer_length;

	//pcmd->TimeOutValue = 6000;
	pcmd->TimeOutValue = 0x258;

	pcmd->DataBuffer = buffer;
	pcmd->SenseInfoOffset = sizeof(SCSI_PASS_THROUGH_DIRECT);

	/// CMD
	pcmd->Cdb[0] = 0xfe;
	pcmd->Cdb[1] = 0x6f;
	pcmd->Cdb[2] = 0x01;

	TRACE("sizeof(cmd):%d\n", sizeof(cmd));

	BOOL bRet = DeviceIoControl(
		device,
		IOCTL_SCSI_PASS_THROUGH_DIRECT,
		(LPVOID)&cmd,
		sizeof(cmd),
		(LPVOID)&cmd,
		sizeof(cmd),
		&returned,
		NULL);

#ifdef _DEBUG
	DWORD err = ::GetLastError();
	if(err != ERROR_SUCCESS)
	{
		printf("carry_cdb(): last error = %08X\n", err);
	}
#endif

	return bRet ? true : false;
}


bool UpdateDeviceInfos()
{
	char devname[20];
	HANDLE device;

	//sprintf(devname, "\\\\.\\%c:", m_cbDrives.GetItemData(m_cbDrives.GetCurSel()) + 'A');

	device = ::CreateFile(/*devname,*/
		"\\\\?\\USBSTOR#Disk&Ven_NISEC&Prod_TCG-01&Rev_5.00#7&2dca610a&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		0);

	CString strInquiry = "Could not retrieve Inquiry information!";
	CString strRegion = "Could not retrieve region info!";

	//m_edtInquiry.SetWindowText(strInquiry);
	//m_edtRegion.SetWindowText(strRegion);
	
	if(device == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("Cannot access the drive!");
		return false;
	}

	// Get Inquiry information
	do
	{
		CDB_INQUIRY6 inquiry = {0};
		SCSI_INQUIRY_STD_DATA data = {0};

		//inquiry.AllocationLength = sizeof(data);
		inquiry.OperationCode6 = SCSIOP_INQUIRY;

		if(!carry_cdb(device, &inquiry, sizeof(inquiry), &data, sizeof(data)))
		{
			break;
		}

		strInquiry = "";

		CString s;

/*
		char snip[100] = {0};
		int i;
		strncpy(snip, data.vendor_id, i = sizeof(data.vendor_id));
		snip[i] = 0;
		s.Format("Vendor id: %s\r\n", snip);

		strInquiry += s;

		strncpy(snip, data.product_id , i = sizeof(data.product_id));
		snip[i] = 0;
		s.Format("Product id: %s\r\n", snip);

		strInquiry += s;

		strncpy(snip, data.product_revision_level , i = sizeof(data.product_revision_level));
		snip[i] = 0;
		s.Format("Product rev level: %s\r\n", snip);
*/

		strInquiry += s;
	} while (false);


	//m_edtInquiry.SetWindowText(strInquiry);
	//m_edtRegion.SetWindowText(strRegion);

	::CloseHandle(device);

	return true;
}

//void CMyregionDlg::OnSelchangeDrivesCombo()
//{
//	UpdateDeviceInfos();
//}
