//
// C++ Interface: PCEnumSerial
//
// Description: 
//
// Author:    xltyt, <xltytlive@hotmail.com>, (C) 2009
// Created:   2009-3-6
//
// Copyright: See COPYING file that comes with this distribution
//

/************************************************************************* 
* Serial port enumeration routines
*
* The EnumSerialPort function will populate an array of SSerInfo structs,
* each of which contains information about one serial port present in
* the system. Note that this code must be linked with setupapi.lib,
* which is included with the Win32 SDK.
*
* by Zach Gorman <gormanjz@hotmail.com>
*
* Copyright (c) 2002 Archetype Auction Software, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following condition is
* met: Redistributions of source code must retain the above copyright
* notice, this condition and the following disclaimer.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL ARCHETYPE AUCTION SOFTWARE OR ITS
* AFFILIATES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************************************/

// The next 3 includes are needed for serial port enumeration

#include "stdafx.h"
#include "../Include.h"
#include <objbase.h>
#include <initguid.h>
#include <Setupapi.h>
#include <Shlwapi.h>
#include <tchar.h>
#include "PCEnumSerial.h"

namespace Device
{
	namespace PCCommEngine
	{
		/*
		// The following define is from ntddser.h in the DDK. It is also
		// needed for serial port enumeration.
#ifndef GUID_CLASS_COMPORT
		DEFINE_GUID(GUID_CLASS_COMPORT, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, \
			0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73);
#endif
		//
		// See Winioctl.h
		// Line 58
		//
		//#define DiskClassGuid               GUID_DEVINTERFACE_DISK
		//#define CdRomClassGuid              GUID_DEVINTERFACE_CDROM
		//#define PartitionClassGuid          GUID_DEVINTERFACE_PARTITION
		//#define TapeClassGuid               GUID_DEVINTERFACE_TAPE
		//#define WriteOnceDiskClassGuid      GUID_DEVINTERFACE_WRITEONCEDISK
		//#define VolumeClassGuid             GUID_DEVINTERFACE_VOLUME
		//#define MediumChangerClassGuid      GUID_DEVINTERFACE_MEDIUMCHANGER
		//#define FloppyClassGuid             GUID_DEVINTERFACE_FLOPPY
		//#define CdChangerClassGuid          GUID_DEVINTERFACE_CDCHANGER
		//#define StoragePortClassGuid        GUID_DEVINTERFACE_STORAGEPORT
		//#define GUID_CLASS_COMPORT          GUID_DEVINTERFACE_COMPORT
		//#define GUID_SERENUM_BUS_ENUMERATOR GUID_DEVINTERFACE_SERENUM_BUS_ENUMERATOR
		*/


		//---------------------------------------------------------------
		// Helpers for enumerating the available serial ports.
		// These throw a CString on failure, describing the nature of
		// the error that occurred.

		// Device Management Functions
		// Helpers for EnumSerialPorts
		//
		// 
		// Virtual Serial Port Work Imperfect
		//

		BOOL EnumPortsWdm(vector<SPCSerInfo>& asi)
		{
			//! Create a device information set that will be the container for 
			//! the device interfaces.
			GUID *guidDev = (GUID*) &GUID_CLASS_COMPORT;
			HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
			hDevInfo = SetupDiGetClassDevs( guidDev,
				NULL,
				NULL,
				DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
				);

			if(hDevInfo == INVALID_HANDLE_VALUE)
			{
				return FALSE;
			}

			DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;
			SP_DEVICE_INTERFACE_DETAIL_DATA *pDetData = NULL;
			pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA*) new char[dwDetDataSize];
			//! This is required, according to the documentation. Yes,
			//! it's weird.
			pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			//! Enumerate the serial ports
			SP_DEVICE_INTERFACE_DATA ifcData;
			ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
			
			BOOL bOK = TRUE;
			BOOL bContinue = TRUE;
			for(DWORD i = 0; bContinue; i++) 
			{
				bContinue = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guidDev, i, &ifcData);
				if(bContinue == FALSE) 
				{
					DWORD err = GetLastError();
					if (err != ERROR_NO_MORE_ITEMS) 
					{
						bOK = FALSE;
					}
					else
					{
						bOK = TRUE;
					}
					break;
				}

				//! Got a device. Get the details.
				SP_DEVINFO_DATA devdata = {sizeof(SP_DEVINFO_DATA)};
				bContinue = SetupDiGetDeviceInterfaceDetail(hDevInfo, &ifcData, pDetData, dwDetDataSize, NULL, &devdata);
				if(bContinue == FALSE) 
				{
					bOK = FALSE;
					break;
				}

				//!
				TCHAR szDevPath[256] = {0};
				lstrcpy(szDevPath, pDetData->DevicePath);
						
				//! Got a path to the device. Try to get some more info.
				TCHAR szFriendName[2560];

				BOOL bSuccess = SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_FRIENDLYNAME, NULL,
					(PBYTE)szFriendName, sizeof(szFriendName), NULL);
				TCHAR szDeviceDesc[256];
				bSuccess = bSuccess && SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_DEVICEDESC, NULL,
					(PBYTE)szDeviceDesc, sizeof(szDeviceDesc), NULL);
				if(bSuccess == FALSE)
				{
					continue;
				}

				BOOL bUsbDevice = FALSE;
				TCHAR szLocateInfo[256];
				if (SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_LOCATION_INFORMATION, NULL,
					(PBYTE)szLocateInfo, sizeof(szLocateInfo), NULL))
				{
					//! Just check the first three characters to determine
					//! if the port is connected to the USB bus. This isn't
					//! an infallible method; it would be better to use the
					//! BUS GUID. Currently, Windows doesn't let you query
					//! that though (SPDRP_BUSTYPEGUID seems to exist in
					//! documentation only).
					bUsbDevice = (StrCmpN(szLocateInfo, _T("USB"), 3)==0);
				}

				//! Add an entry to the array
				SPCSerInfo si;
				si.strDevPath		= szDevPath;
				si.strFriendlyName	= szFriendName;
				si.strPortDesc		= szDeviceDesc;
				si.bUsbDevice		= bUsbDevice;

				LPTSTR pBegin = StrChr(szFriendName, _T('('));
				LPTSTR pEnd = StrChr(szFriendName, _T(')'));
				if(pBegin && pEnd)
				{
					TCHAR szPortName[256] = {0};
					StrCpyN(szPortName, pBegin + 1, pEnd - pBegin);
					si.strPortName = szPortName;
				}

				//! If there is no description, try to make one up from
				//! the friendly name.
				if(lstrlen(szDeviceDesc) == 0) 
				{
					//! If the port name is of the form "ACME Port (COM3)"
					//! then strip off the " (COM3)"
					//!
					si.strPortDesc = si.strFriendlyName;
					//!
					int iFindIndex = si.strPortDesc.find(_T('('));
					if(iFindIndex != string::npos)
					{
						si.strPortDesc = si.strPortDesc.substr(0, iFindIndex - 1);
					}
				}
				asi.push_back(si);
			}

			if(pDetData != NULL)
			{
				delete[] pDetData;
				pDetData = NULL;
			}
			if(hDevInfo != INVALID_HANDLE_VALUE)
			{
				SetupDiDestroyDeviceInfoList(hDevInfo);
				hDevInfo = INVALID_HANDLE_VALUE;
			}
			return bOK;
		}

		//
		//
		//
		BOOL IsNumeric(LPCTSTR pszString, BOOL bIgnoreColon)
		{
			int nLen = _tcslen(pszString);
			if (nLen == 0)
				return FALSE;

			//Assume the best
			BOOL bNumeric = TRUE;

			for (int i=0; i<nLen && bNumeric; i++)
			{
				bNumeric = (_istdigit(pszString[i]) != 0);
				if (bIgnoreColon && (pszString[i] == _T(':')))
					bNumeric = TRUE;
			}

			return bNumeric;
		}

		//
		// Virtual Serial Port Work Perfect
		//
		BOOL EnumPortsSetupAPI(vector<SPCSerInfo>& asi)
		{
			BOOL bSuccess = FALSE;
			
			//First need to convert the name "Ports" to a GUID using SetupDiClassGuidsFromName
			DWORD dwGuids = 0;
			SetupDiClassGuidsFromName(_T("Ports"), NULL, 0, &dwGuids);
			if(dwGuids == 0)
			{
				return FALSE;
			}

			//Allocate the needed memory
			GUID* pGuids = new GUID[dwGuids];

			//Call the function again
			if(!SetupDiClassGuidsFromName(_T("Ports"), pGuids, dwGuids, &dwGuids))
			{
				//Free up the memory before we return
				delete[] pGuids;
				pGuids = NULL;
				
				return FALSE;
			}
			
			//
			//GUID *pGuids = (GUID*)&GUID_CLASS_COMPORT;

			//Now create a "device information set" which is required to enumerate all the ports
			HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
			hDevInfo = SetupDiGetClassDevs(
				pGuids,
				NULL,
				NULL,
				DIGCF_PRESENT// | DIGCF_DEVICEINTERFACE
				);
			if(hDevInfo == INVALID_HANDLE_VALUE)
			{
				//Free up the memory before we return
				delete[] pGuids;
				pGuids = NULL;

				return FALSE;
			}

			//
			DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;
			SP_DEVICE_INTERFACE_DETAIL_DATA *pDetData = NULL;
			pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA*) new char[dwDetDataSize];
			//! This is required, according to the documentation. Yes,
			//! it's weird.
			pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			//! Enumerate the serial ports
			SP_DEVICE_INTERFACE_DATA ifcData;
			ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

			BOOL bOK = TRUE;
			//Finally do the enumeration
			BOOL bMoreItems = TRUE;
			for(DWORD nIndex = 0; bMoreItems; nIndex++)
			{
				SP_DEVINFO_DATA devInfo;
				//Enumerate the current device
				devInfo.cbSize = sizeof(SP_DEVINFO_DATA);
				bMoreItems = SetupDiEnumDeviceInfo(hDevInfo, nIndex, &devInfo);
				if(bMoreItems == FALSE)
				{
					bOK = TRUE;
					break;
				}

				//Get the registry key which stores the ports settings
				HKEY hDeviceKey = SetupDiOpenDevRegKey(hDevInfo, &devInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);
				if(hDeviceKey == INVALID_HANDLE_VALUE)
				{
					continue;
				}

				//Did we find a serial port for this device
				BOOL bAdded = FALSE;
				SPCSerInfo si;

				TCHAR pszPortName[256] = {0};
				DWORD dwSize = sizeof(pszPortName);
				DWORD dwType = 0;
				if((RegQueryValueEx(hDeviceKey, _T("PortName"), NULL, &dwType, (LPBYTE)pszPortName, &dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ))
				{
					//If it looks like "COMX" then
					//add it to the array which will be returned
					int nLen = _tcslen(pszPortName);
					if (nLen > 3)
					{
						if((_tcsnicmp(pszPortName, _T("COM"), 3) == 0) && IsNumeric(&pszPortName[3], FALSE))
						{
							bAdded = TRUE;
						}
					}

					//Close the key now that we are finished with it
					RegCloseKey(hDeviceKey);
				}

				if(bAdded == FALSE)
				{
					continue;
				}

				// Add Port Name
				si.strPortName = pszPortName;

				// Got a path to the device. Try to get some more info.
				TCHAR szFriendName[256] = {0};
				SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfo, SPDRP_FRIENDLYNAME, NULL,
					(PBYTE)szFriendName, sizeof(szFriendName), NULL);
				si.strFriendlyName = szFriendName;

				TCHAR szDeviceDesc[256];
				SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfo, SPDRP_DEVICEDESC, NULL,
					(PBYTE)szDeviceDesc, sizeof(szDeviceDesc), NULL);
				si.strPortDesc = szDeviceDesc;

				BOOL bUsbDevice = FALSE;
				TCHAR szLocateInfo[256] = {0};
				if (SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfo, SPDRP_LOCATION_INFORMATION, NULL,
					(PBYTE)szLocateInfo, sizeof(szLocateInfo), NULL) == TRUE)
				{
					//! Just check the first three characters to determine
					//! if the port is connected to the USB bus. This isn't
					//! an infallible method; it would be better to use the
					//! BUS GUID. Currently, Windows doesn't let you query
					//! that though (SPDRP_BUSTYPEGUID seems to exist in
					//! documentation only).
					bUsbDevice = (StrCmpN(szLocateInfo, _T("USB"), 3)==0);
				}
				si.bUsbDevice = bUsbDevice;

				//
				BOOL bGotDevPath = FALSE;
				BOOL bContinue = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, pGuids, nIndex, &ifcData);
				if(bContinue == TRUE) 
				{
					// Got a device. Get the details.
					SP_DEVINFO_DATA devdata = {sizeof(SP_DEVINFO_DATA)};
					bContinue = SetupDiGetDeviceInterfaceDetail(hDevInfo, &ifcData, pDetData, dwDetDataSize, NULL, &devdata);
					if(bContinue == TRUE) 
					{
						bGotDevPath = TRUE;
					}
				}

				//!
				TCHAR szDevPath[256] = {0};
				if(bGotDevPath == TRUE)
				{
					lstrcpy(szDevPath, pDetData->DevicePath);
				}
				else
				{
					wsprintf(szDevPath, _T("\\\\.\\%s"), pszPortName);
				}
				si.strDevPath = szDevPath;

				//
				asi.push_back(si);
			}

			//Finished with the Guids by this time
			if(pGuids)
			{
				delete[] pGuids;
				pGuids = NULL;
			}
			if(pDetData != NULL)
			{
				delete[] pDetData;
				pDetData = NULL;
			}
			if(hDevInfo != INVALID_HANDLE_VALUE)
			{
				SetupDiDestroyDeviceInfoList(hDevInfo);
				hDevInfo = INVALID_HANDLE_VALUE;
			}
			return bOK;
		}

		BOOL EnumPortsSetupAPIEx(vector<SPCSerInfo>& asi)
		{
			//! Create a device information set that will be the container for 
			//! the device interfaces.
			GUID *guidDev = (GUID*) &GUID_CLASS_COMPORT;
			HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
			hDevInfo = SetupDiGetClassDevs( guidDev,
				NULL,
				NULL,
				DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
				);

			if(hDevInfo == INVALID_HANDLE_VALUE)
			{
				return FALSE;
			}

			DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;
			SP_DEVICE_INTERFACE_DETAIL_DATA *pDetData = NULL;
			pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA*) new char[dwDetDataSize];
			//! This is required, according to the documentation. Yes,
			//! it's weird.
			pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			//! Enumerate the serial ports
			SP_DEVICE_INTERFACE_DATA ifcData;
			ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

			BOOL bOK = TRUE;
			BOOL bContinue = TRUE;
			for(DWORD i = 0; bContinue; i++) 
			{
				bContinue = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guidDev, i, &ifcData);
				if(bContinue == FALSE) 
				{
					DWORD err = GetLastError();
					if (err != ERROR_NO_MORE_ITEMS) 
					{
						bOK = FALSE;
					}
					else
					{
						bOK = TRUE;
					}
					break;
				}

				//! Got a device. Get the details.
				SP_DEVINFO_DATA devdata = {sizeof(SP_DEVINFO_DATA)};
				bContinue = SetupDiGetDeviceInterfaceDetail(hDevInfo, &ifcData, pDetData, dwDetDataSize, NULL, &devdata);
				if(bContinue == FALSE) 
				{
					bOK = FALSE;
					break;
				}

				//Get the registry key which stores the ports settings
				HKEY hDeviceKey = SetupDiOpenDevRegKey(hDevInfo, &devdata, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);
				if(hDeviceKey == INVALID_HANDLE_VALUE)
				{
					continue;
				}

				//Did we find a serial port for this device
				BOOL bAdded = FALSE;
				
				TCHAR pszPortName[256] = {0};
				DWORD dwSize = sizeof(pszPortName);
				DWORD dwType = 0;
				if((RegQueryValueEx(hDeviceKey, _T("PortName"), NULL, &dwType, (LPBYTE)pszPortName, &dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ))
				{
					//If it looks like "COMX" then
					//add it to the array which will be returned
					int nLen = _tcslen(pszPortName);
					if (nLen > 3)
					{
						if((_tcsnicmp(pszPortName, _T("COM"), 3) == 0) && IsNumeric(&pszPortName[3], FALSE))
						{
							bAdded = TRUE;
						}
					}

					//Close the key now that we are finished with it
					RegCloseKey(hDeviceKey);
				}

				if(bAdded == FALSE)
				{
					continue;
				}

				//!
				TCHAR szDevPath[2560] = {0};
				lstrcpy(szDevPath, pDetData->DevicePath);

				//! Got a path to the device. Try to get some more info.
				TCHAR szFriendName[2560];

				BOOL bSuccess = SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_FRIENDLYNAME, NULL,
					(PBYTE)szFriendName, sizeof(szFriendName), NULL);
				TCHAR szDeviceDesc[2560];
				bSuccess = bSuccess && SetupDiGetDeviceRegistryProperty(hDevInfo, &devdata, SPDRP_DEVICEDESC, NULL,
					(PBYTE)szDeviceDesc, sizeof(szDeviceDesc), NULL);
				if(bSuccess == FALSE)
				{
					continue;
				}

				//! Add an entry to the array
				SPCSerInfo si;
				si.strDevPath		= szDevPath;
				si.strFriendlyName	= szFriendName;
				si.strPortDesc		= szDeviceDesc;
				si.bUsbDevice		= FALSE;
				si.strPortName		= pszPortName;
				
				asi.push_back(si);
			}

			if(pDetData != NULL)
			{
				delete[] pDetData;
				pDetData = NULL;
			}
			if(hDevInfo != INVALID_HANDLE_VALUE)
			{
				SetupDiDestroyDeviceInfoList(hDevInfo);
				hDevInfo = INVALID_HANDLE_VALUE;
			}
			return bOK;
		}

		//!---------------------------------------------------------------
		//! Routine for enumerating the available serial ports.
		//! Throws a CString on failure, describing the error that
		//! occurred. If bIgnoreBusyPorts is TRUE, ports that can't
		//! be opened for read/write access are not included.

		BOOL EnumPCSerialPorts(vector<SPCSerInfo> &asi, BOOL bIgnoreBusyPorts)
		{
			//! Clear the output array
			asi.clear();

			//! Win2k and later support a standard API for
			//! enumerating hardware devices.
			if(EnumPortsSetupAPIEx(asi) == FALSE)
			{
				return FALSE;
			}

			for(size_t i = 0; i < asi.size(); i++)
			{
				SPCSerInfo& rsi = asi[i];
				if(bIgnoreBusyPorts)
				{
					//! Only display ports that can be opened for read/write
					HANDLE hCom = CreateFile(
						rsi.strDevPath.c_str(),
						GENERIC_READ | GENERIC_WRITE,
						0,						//! comm devices must be opened w/exclusive-access
						NULL,					//! no security attrs
						OPEN_EXISTING,			//! comm devices must use OPEN_EXISTING
						0,						//! not overlapped I/O
						NULL					//! hTemplate must be NULL for comm devices
						);
					if(hCom == INVALID_HANDLE_VALUE) 
					{
						//! It can't be opened; remove it.
						vector<SPCSerInfo>::iterator pIterator = asi.begin();
						advance(pIterator, i);
						asi.erase(pIterator);
						i--;
						continue;
					}
					else 
					{
						//! It can be opened! Close it and add it to the list
						::CloseHandle(hCom);
					}
				}
			}
			return TRUE;
		}
	}
}
