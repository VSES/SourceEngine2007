///////////////////////////////////////////////////////////////////////////////
//
// Microsoft Windows Media Technologies
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// Filename: WmpDevices.h
//
// Structures and constants needed by a device implementer to support
// Windows Media Player device extension for accelerated meta data
// synchronization from a device into the Windows Media Player Media Library.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

// This file defines Windows Media Player support for both traditional WMDM
// service providers (SPs) and the new MTP class of devices.
//
// Please define WMP_WMDM_DEVICE_SUPPORT and/or WMP_MTP_DEVICE_SUPPORT as
// appropriate before including this file into your source.



#if defined(WMP_WMDM_DEVICE_SUPPORT) || defined(WMP_MTP_DEVICE_SUPPORT)

//
// defines used to encode/decode WMP_WMDM_METADATA_ROUND_TRIP_DEVICE2PC::dwFlags and pDevice2PC->Params[2]
//
#define WMP_MDRT_FLAGS_UNREPORTED_DELETED_ITEMS     0x00000001  // bit 0
#define WMP_MDRT_FLAGS_UNREPORTED_ADDED_ITEMS       0x00000002  // bit 1
// all other bits are reserved for future use and should be returned as 0 (zero) for now

#endif // defined(WMP_WMDM_DEVICE_SUPPORT) || defined(WMP_MTP_DEVICE_SUPPORT)



#ifdef WMP_WMDM_DEVICE_SUPPORT

#pragma pack(push, Old, 1)

#define IOCTL_WMP_METADATA_ROUND_TRIP       0x31504d57  // 'W' 'M' 'P' '1'

// Windows Media Player sends and retrieves these structures using code similar to this:
//   IWMDMDevice3 *pDevice3;
//   WMP_WMDM_METADATA_ROUND_TRIP_PC2DEVICE PC2Device = {0,0};
//   DWORD dwSizeDevice2PC = sizeof(WMP_WMDM_METADATA_ROUND_TRIP_DEVICE2PC) + 0x50000;
//   WMP_WMDM_METADATA_ROUND_TRIP_DEVICE2PC *pDevice2PC = (WMP_WMDM_METADATA_ROUND_TRIP_DEVICE2PC *) new BYTE[dwSizeDevice2PC];
//   HRESULT hr = pDevice3->DeviceIoControl(IOCTL_WMP_METADATA_ROUND_TRIP,
//                                  &PC2Device, sizeof(WMP_WMDM_METADATA_ROUND_TRIP_PC2DEVICE),
//                                  pDevice2PC, &dwSizeDevice2PC);

//
// Structure sent by Windows Media Player to WMDM SP
//

typedef struct _WMP_WMDM_METADATA_ROUND_TRIP_PC2DEVICE
{
    DWORD   dwChangesSinceTransactionID;    // 0 on first call ever to device (return all changes),
                                            //   value of WMP_WMDM_METADATA_ROUND_TRIP_DEVICE2PC::dwCurrentTransactionID
                                            //   from last sync session
    DWORD   dwResultSetStartingIndex;       // 0 based starting index into result set that should
                                            //   be returned, always zero on first call

    // WMDM SP should check the passed buffer size is at least sizeof(WMP_WMDM_METADATA_ROUND_TRIP_PC2DEVICE).
    // Future versions of WMP may pass larger structures which enable new functionality but these
    //   first 2 DWORDs would always remain the same.
} WMP_WMDM_METADATA_ROUND_TRIP_PC2DEVICE;

//
// Response structure sent by WMDM SP to Windows Media Player
//

typedef struct _WMP_WMDM_METADATA_ROUND_TRIP_DEVICE2PC
{
    DWORD   dwCurrentTransactionID;         // Device's current transaction ID (WMP will pass this
                                            //   value as WMP_WMDM_METADATA_ROUND_TRIP_PC2DEVICE::dwChangesSinceTransactionID
                                            //   during the next sync session)
    DWORD   dwReturnedObjectCount;          // how many object pathnames are in wsObjectPathnameList
    DWORD   dwUnretrievedObjectCount;       // how many objects have changes that were not returned in
                                            //   this response (non-zero value implies at least one more
                                            //   request must be made by WMP)
    DWORD   dwDeletedObjectStartingOffset;  // wsObjectPathnameList[dwDeletedObjectStartingOffset] is
                                            //   first character of first deleted object pathname
                                            // pass 0 if wsObjectPathnameList only contains deleted
                                            //   objects (contains no updated or added objects)
                                            // pass the character offset of the last nul in
                                            //   wsObjectPathnameList if the list only contains updated
                                            //   or added objects (no deleted objects)
    DWORD   dwFlags;                        // bit 0 (0==false, 1==true) indicating some items were
                                            //   deleted before the first PUOID being reported
                                            //   (normally means device was reformatted)
                                            // bit 1 (0==false, 1==true) indicating some additional items
                                            //   were added that were not returned in the list of PUOIDs
                                            // bits 2-31 - reserved for future use
                                            //   (must be returned as 0 for now)
    WCHAR   wsObjectPathnameList[1];        // list of nul terminated unicode pathname strings, one after
                                            //   another, terminated with an extra nul
                                            // All the objects that have been added or have their
                                            //   playcount, user rating or BuyNow properties changed come
                                            //   first.  Then all the objects that have been deleted come
                                            //   next.  This second set of objects starts at
                                            //   dwDeletedObjectStartingOffset.
                                            // The WMDM SP should return as many object pathnames as will
                                            //   fit in the buffer passed to IMDSPDevice3::DeviceIoControl()
                                            //   who's size is passed in *pnOutBufferSize.  If there isn't
                                            //   enough room return what you can and set the
                                            //   dwUnretrievedObjectCount to a non-zero value appropriately.
} WMP_WMDM_METADATA_ROUND_TRIP_DEVICE2PC;

#define IOCTL_WMP_DEVICE_CAN_SYNC           0x32504d57  // 'W' 'M' 'P' '2'
//
// This IOCTL will pass no parameters and expects a DWORD return value (1 for able to sync, 
// 0 for not able to sync)
//

#pragma pack(pop, Old)

#endif // WMP_WMDM_DEVICE_SUPPORT



#ifdef WMP_MTP_DEVICE_SUPPORT

#include <MtpExt.h> // distributed with the version 10.0 WMDM SDK (Windows Media Device Manager)

const WORD kwWmpMTPmdrtOpCode = 0x9201;
const DWORD kdwWmpMTPmdrtMaxPUOIDCount = 0x10000;   // 65536 * 16 bytes per PUOID (GUID) = 1MB = 1048576 bytes

#define WMP_MTP_DEVICE_EXTENSION_ID "microsoft.com/WMPPD: 10.0"

/* WMP sends the MTP opcode using code similar to the following:

    IWMDMDevice3 *pDevice3;
    MTP_COMMAND_DATA_IN PC2Device = {0,0};
    DWORD dwSizeDevice2PC = sizeof(MTP_COMMAND_DATA_OUT) +
                            kdwWmpMTPmdrtMaxPUOIDCount * sizeof(GUID) +
                            2 * sizeof(DWORD);
    MTP_COMMAND_DATA_OUT *pDevice2PC = (MTP_COMMAND_DATA_OUT *) new BYTE[dwSizeDevice2PC];
    HRESULT hr = pDevice3->DeviceIoControl(IOCTL_MTP_CUSTOM_COMMAND,
                                    &PC2Device, sizeof(MTP_COMMAND_DATA_IN),
                                    pDevice2PC, &dwSizeDevice2PC);

    The buffer being sent from the desktop PC to the MTP device is passed as the 2nd parameter of
    DeviceIoControl().  It is defined by MTP_COMMAND_DATA_IN in MtpExt.h.
    
    WMP populates this buffer with code similar to this:

        PC2Device.OpCode = kwWmpMTPmdrtOpCode;
            
            // device should check that this is at least 3 since future versions of WMP may pass more params.
        PC2Device.NumParams = 3;
            
            // will be zero for the first call ever for this device (return all changes)
        PC2Device.Params[0] = dwLastDeviceTransactionID;

            // starting index, always zero on first call
        PC2Device.Params[1] = 0;

            // the maximum number of array elements that may be returned in one response
        PC2Device.Params[2] = kdwWmpMTPmdrtMaxPUOIDCount;

            // currently unused and passed in as zero values
        PC2Device.Params[3] = 0;
        PC2Device.Params[4] = 0;

            // we expect CommandReadData to be populated in the response from the device
        PC2Device.NextPhase = MTP_NEXTPHASE_READ_DATA;
        
            // there is no data buffer being passed to the device
        PC2Device.CommandWriteDataSize = 0;

    The buffer being retrieved by the desktop PC from the MTP device is passed as the 4th parameter of
    DeviceIoControl().  It is defined by MTP_COMMAND_DATA_OUT in MtpExt.h.
    
    WMP expects this buffer to be populated like this:

            // MTP_RESPONSE_OK or one of the other error response codes
        pDevice2PC->ResponseCode

            // should be 3 indicating Params[0], Params[1], Params[2] are valid and Params[3] and
            //   Params[4] are ignored.  WMP verifies this value is at least 3 to allow for future
            //   enhancements that utilize Params[3] or Params[4]
        pDevice2PC->NumParams

            // Device's current transaction ID.  This will be passed as PC2Device.Params[0] in
            // future sync sessions to this device.
        pDevice2PC->Params[0]       

            // how many PUOIDs still remain to be retrieved in future requests
        pDevice2PC->Params[1]

            // flag bits: bit 0 (0==false, 1==true) indicating some items were deleted before the first
            //                  PUOID being reported - normally means device was reformatted
            //            bit 1 (0==false, 1==true) indicating some additional items were added that
            //                  were not returned in the list of PUOIDs being reported
            //            bits 2-31 - reserved for future use - should be returned as 0 for now
        pDevice2PC->Params[2]

            // number of bytes contained in the CommandReadData buffer
        pDevice2PC->CommandReadDataSize

            // two back to back MTP arrays of PUOIDs (GUIDs).
            // an MTP array is a DWORD with the count of items in the array followed by an array of the elements
            // the first array is the list of PUOIDs that have been added or modified
            // the second array is the list of PUOIDs that have been deleted from the device
            // The total number of elements in both arrays must not exceed the value of PC2Device.Params[2].
        pDevice2PC->CommandReadData
*/

#endif // WMP_MTP_DEVICE_SUPPORT

//
// Macros for notifying WMP that a device has arrived or been removed
//
__inline BOOL WMPNotifyDeviceArrival()
{
    return( ::PostMessage( HWND_BROADCAST, ::RegisterWindowMessageA( "WMPlayer_PluginAddRemove" ), 2, 0 ) );
}

__inline BOOL WMPNotifyDeviceRemoval()
{
    return( ::PostMessage( HWND_BROADCAST, ::RegisterWindowMessageA( "WMPlayer_PluginAddRemove" ), 3, 0 ) );
}
