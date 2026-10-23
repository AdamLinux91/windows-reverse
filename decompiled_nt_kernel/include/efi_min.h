/*
 * Minimal Zero-Dependency UEFI 2.x Specification Header (x86-64 Microsoft ABI)
 * Self-contained definitions for booting decompiled Windows NT kernel modules
 * directly from real UEFI motherboards (\EFI\BOOT\BOOTX64.EFI).
 */
#ifndef NT_EFI_MIN_H
#define NT_EFI_MIN_H

#include <stdint.h>
#include <stddef.h>

#define EFIAPI __attribute__((ms_abi))

typedef uint64_t UINTN;
typedef uint64_t EFI_STATUS;
typedef void*    EFI_HANDLE;
typedef uint16_t CHAR16;
typedef uint64_t EFI_PHYSICAL_ADDRESS;

#define EFI_SUCCESS 0ULL

typedef struct {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} EFI_GUID;

typedef struct {
    uint64_t Signature;
    uint32_t Revision;
    uint32_t HeaderSize;
    uint32_t CRC32;
    uint32_t Reserved;
} EFI_TABLE_HEADER;

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET)(
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    uint8_t ExtendedVerification
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING)(
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    const CHAR16 *String
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    UINTN Attribute
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_CLEAR_SCREEN)(
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
);

struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET         Reset;
    EFI_TEXT_STRING        OutputString;
    void                  *TestString;
    void                  *QueryMode;
    void                  *SetMode;
    EFI_TEXT_SET_ATTRIBUTE SetAttribute;
    EFI_TEXT_CLEAR_SCREEN  ClearScreen;
    void                  *SetCursorPosition;
    void                  *EnableCursor;
    void                  *Mode;
};

/* UEFI Graphics Output Protocol (GOP) for 32-bpp bare-metal framebuffer output */
typedef enum {
    PixelRedGreenBlueReserved8BitPerColor = 0,
    PixelBlueGreenRedReserved8BitPerColor = 1,
    PixelBitMask                          = 2,
    PixelBltOnly                          = 3
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
    uint32_t                  Version;
    uint32_t                  HorizontalResolution;
    uint32_t                  VerticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
    uint32_t                  PixelInformation[4];
    uint32_t                  PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct {
    uint32_t                              MaxMode;
    uint32_t                              Mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    UINTN                                 SizeOfInfo;
    EFI_PHYSICAL_ADDRESS                  FrameBufferBase;
    UINTN                                 FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct {
    void                              *QueryMode;
    void                              *SetMode;
    void                              *Blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_STALL)(UINTN Microseconds);
typedef EFI_STATUS (EFIAPI *EFI_LOCATE_PROTOCOL)(
    const EFI_GUID *Protocol,
    void           *Registration,
    void          **Interface
);
typedef EFI_STATUS (EFIAPI *EFI_SET_WATCHDOG_TIMER)(
    UINTN    Timeout,
    uint64_t WatchdogCode,
    UINTN    DataSize,
    CHAR16  *WatchdogData
);

typedef struct {
    EFI_TABLE_HEADER       Hdr;
    void                  *RaiseTPL;
    void                  *RestoreTPL;
    void                  *AllocatePages;
    void                  *FreePages;
    void                  *GetMemoryMap;
    void                  *AllocatePool;
    void                  *FreePool;
    void                  *CreateEvent;
    void                  *SetTimer;
    void                  *WaitForEvent;
    void                  *SignalEvent;
    void                  *CloseEvent;
    void                  *CheckEvent;
    void                  *InstallProtocolInterface;
    void                  *ReinstallProtocolInterface;
    void                  *UninstallProtocolInterface;
    void                  *HandleProtocol;
    void                  *Reserved;
    void                  *RegisterProtocolNotify;
    void                  *LocateHandle;
    void                  *LocateDevicePath;
    void                  *InstallConfigurationTable;
    void                  *LoadImage;
    void                  *StartImage;
    void                  *Exit;
    void                  *UnloadImage;
    void                  *ExitBootServices;
    void                  *GetNextMonotonicCount;
    EFI_STALL              Stall;
    EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;
    void                  *ConnectController;
    void                  *DisconnectController;
    void                  *OpenProtocol;
    void                  *CloseProtocol;
    void                  *OpenProtocolInformation;
    void                  *ProtocolsPerHandle;
    void                  *LocateHandleBuffer;
    EFI_LOCATE_PROTOCOL    LocateProtocol;
} EFI_BOOT_SERVICES;

typedef struct {
    EFI_TABLE_HEADER                 Hdr;
    CHAR16                          *FirmwareVendor;
    uint32_t                         FirmwareRevision;
    EFI_HANDLE                       ConsoleInHandle;
    void                            *ConIn;
    EFI_HANDLE                       ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE                       StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
    void                            *RuntimeServices;
    EFI_BOOT_SERVICES               *BootServices;
    UINTN                            NumberOfTableEntries;
    void                            *ConfigurationTable;
} EFI_SYSTEM_TABLE;

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID \
    { 0x9042a9de, 0x23dc, 0x4a38, { 0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a } }

#endif /* NT_EFI_MIN_H */
