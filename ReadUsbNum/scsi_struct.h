/*

Some structures are borrowed from other header files and some structures are constructed by lallous from the specification docs

References
--------------
[1] mmc5r03a.pdf from t10.org
[2] MS Windows DDK headers

*/

//#include <windows.h>

// If this was not defined we should also assume that some structs are not defined as well
#ifndef IOCTL_SCSI_PASS_THROUGH_DIRECT

#define IOCTL_SCSI_PASS_THROUGH_DIRECT 0x4D014
#define SCSI_IOCTL_DATA_OUT          0
#define SCSI_IOCTL_DATA_IN           1
#define SCSI_IOCTL_DATA_UNSPECIFIED  2

// Reference [2]
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

#pragma pack(1)

/*!
Reference [1] page 528
*/
enum MMC_KEY_FORMAT_CODE_CLASS0
{
  KEY_FORMAT_AGID_CSS  = 0,
  KEY_FORMAT_CHAL_KEY  = 1,
  KEY_FORMAT_KEY1  = 2,
  KEY_FORMAT_TITLE_KEY  = 4,
  KEY_FORMAT_ASF   = 5,
  KEY_FORMAT_RPC_STATE  = 8,
  KEY_FORMAT_AGID_CPRM  = 0x11,
  KEY_FORMAT_NONE  = 0x3F
};

/*!
The type code field specifies the current state of regionalization process
Reference [1] Table 524, page 533
*/
enum RPC_STATE_TYPE_CODES
{
  //! No drive region
  RPC_STATE_TYPECODE_NONE = 0, 
  //! Drive region is set
  RPC_STATE_TYPECODE_SET = 1,
  //! Drive region is set. Additional restrictions required to make changes
  RPC_STATE_TYPECODE_LAST_CHANCE = 2,
  //! Drive region has been set permamently, but may be reset by vendor
  RPC_STATE_TYPECODE_PERM  = 3
};

/*!
RPC Scheme specifies the type of Region Playback Controls being used by the Drive
*/
enum RPC_STATE_SCHEMES
{
  //!Drive does not enforce Region Playback Controls (RPC)
  RPC_STATE_SCHEMES_UNK = 0,
  /*!
  Drive region shall adhere to this standard and all requirements of the CSS
  license agreement concerning RPC.
  */
  RPC_STATE_SCHEMES_RPC2 = 1
};

/*! 
  This structure is returned when a REPORT_KEY is executed with key type = 8
  Reference [1]p.533
*/
typedef struct _REPORT_KEY_DATA_RPC_STATE
{
  char rsvrd1[4];
  
  //
  // RPC state
  //

  /*!
  # of User Controlled Changes Available is a count down counter that indicates the number of times that the user
  may set the region. This value is initially 5.
  */
  unsigned char nb_user_changes: 3;

  /*!
  # of Vendor Resets Available is a count down counter that indicates the number of times that the vendor may
  reset the region. The manufacturer of the Drive sets this value to 4 and the value is decremented each time the
  vendor clears the Drivers region. When this value is zero, the vendor may no longer clear the Drivers region.
  */
  unsigned char nb_vendor_resets: 3;
  unsigned char type_code: 2;

  /*!
  The Region Mask returns a value that specifies the Drive Region in which the Drive is located. Once the Drive
  Region has been set, the Drive shall be located in only one region. Each bit represents one of eight regions. If a
  bit is cleared in this field, the disc may be played in the corresponding region. If a bit is set in this field, the disc
  may not be played in the corresponding region.
  */
  unsigned char region_mask;

  /*
  RPC Scheme specifies the type of Region Playback Controls being used by the Drive
  */
  unsigned char rpc_scheme;
  char rsvrd2;
} REPORT_KEY_DATA_RPC_STATE;


typedef struct _SCSI_INQUIRY_STD_DATA
{
  UCHAR preipheral_device_type : 5;
  UCHAR peripheral_qualifier: 3;
  UCHAR rsvrd : 7;
  UCHAR rmb: 1;
  UCHAR version;
  UCHAR RESPONSE_DATA_FORMAT;				// 7 = AERC, 6 = Obsolete, 5 = NormACA, 4 = HiSup 3-0 = Response data format.
  // If ANSI Version = 0, this is ATAPI and bits 7-4 = ATAPI version.
  UCHAR		ADDITIONAL_LENGTH;					// Number of additional bytes available in inquiry data
  UCHAR		SCCSReserved;						// SCC-2 device flag and reserved fields
  UCHAR		flags1;								// First byte of support flags
  UCHAR		flags2;								// Second byte of support flags (Byte 7)
  char		vendor_id[8];
  char		product_id[16];
  char		product_revision_level[4];
} SCSI_INQUIRY_STD_DATA;


#ifndef SCSIOP_INQUIRY

// Reference [2]
#define SCSIOP_INQUIRY      0x12
#define SCSIOP_REPORT_KEY   0xA4

// Reference [2]
typedef struct _CDB_INQUIRY 
{
  UCHAR OperationCode6;    // 0x12 - SCSIOP_INQUIRY
  UCHAR Reserved1 : 5;
  UCHAR LogicalUnitNumber : 3;
  UCHAR PageCode;
  UCHAR IReserved;
  UCHAR AllocationLength;
  UCHAR Control;
} CDB_INQUIRY6;


// Reference [2]
typedef struct _CDB_REPORT_KEY 
{
  UCHAR OperationCode;    // 0xA4 - SCSIOP_REPORT_KEY
  UCHAR Reserved1 : 5;
  UCHAR Lun : 3;
  UCHAR LogicalBlockAddress[4];   // for title key
  UCHAR Reserved2[2];
  USHORT AllocationLength;
  UCHAR KeyFormat : 6;
  UCHAR AGID : 2;
  UCHAR Control;
} CDB_REPORT_KEY;

#endif

#pragma pack()