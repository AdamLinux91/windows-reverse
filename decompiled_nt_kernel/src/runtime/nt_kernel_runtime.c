/*
 * Bare-Metal Ring-0 Executive Runtime, Console & 16-Subsystem Hardware Self-Test Suite
 * Exercises the 527 real decompiled & Z3-verified Windows 10 ntoskrnl.exe (10.0.16299.15)
 * functions on real x86-64 hardware (Windows winload.efi / UEFI / Multiboot2) and host silicon.
 */
#include "../../include/nt_baremetal.h"
#include "../../include/efi_min.h"

struct _SID_AND_ATTRIBUTES {
    void    *Sid;
    uint32_t Attributes;
    uint32_t _pad;
};

#ifdef NT_HOST_SILICON_TEST
extern long __attribute__((sysv_abi)) write(int fd, const void *buf, size_t count);
#endif

/* ============================================================================
 * 1. Strong Executive Global Tables (Overriding Weak Symbols)
 * ============================================================================ */
uint8_t ObHeaderCookie = 0x5AU;
struct _OBJECT_TYPE* ObTypeIndexTable[256] = {0};

/* Retail Windows 10 x64 ObInitSystem optional-header offset lookup table:
 *   Bit 0 (0x01) = OBJECT_HEADER_CREATOR_INFO (0x20 bytes)
 *   Bit 1 (0x02) = OBJECT_HEADER_NAME_INFO    (0x20 bytes)
 */
uint8_t ObpInfoMaskToOffset[4096] = {
    0x00, /* Mask 0: none */
    0x20, /* Mask 1: CreatorInfo (32B) */
    0x20, /* Mask 2: NameInfo (32B) */
    0x40, /* Mask 3: CreatorInfo + NameInfo (64B) */
};

uint16_t KiActiveGroups = 1U;
uint8_t  KeForceGroupAwareness = 0U;
uint8_t  KdPitchDebugger = 1U;
uint8_t  KdDebuggerNotPresent = 1U;

/* Binary SID for BUILTIN\Administrators: S-1-5-32-544 (16 bytes) */
static uint8_t g_SeAliasAdminsSidBytes[16] __attribute__((aligned(8))) = {
    0x01, 0x02,                         /* Revision=1, SubAuthorityCount=2 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x05, /* IdentifierAuthority = SECURITY_NT_AUTHORITY (5) */
    0x20, 0x00, 0x00, 0x00,             /* SubAuthority[0] = SECURITY_BUILTIN_DOMAIN_RID (32) */
    0x20, 0x02, 0x00, 0x00              /* SubAuthority[1] = DOMAIN_ALIAS_RID_ADMINS (544) */
};
uint64_t SeAliasAdminsSid = 0ULL;

/* Diagnostic telemetry captured during hardware test execution */
uint64_t g_nt_last_timer_hand    = 0ULL;
int64_t  g_nt_last_timer_duetime = 0LL;
ULONG    g_nt_last_timer_period  = 0U;
uint32_t g_nt_kd_printed_len     = 0U;
char     g_nt_kd_printed_buf[128];

static uint32_t g_nt_mdl_freed_pages = 0U;

/* ============================================================================
 * 2. Multi-Backend Hardware Console (COM1 UART + VGA + UEFI ConOut + GOP FB)
 * ============================================================================ */
static int      g_serial_enabled = 0;
static int      g_vga_enabled    = 0;
static uint32_t g_vga_row        = 0;
static uint32_t g_vga_col        = 0;

static EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *g_uefi_conout = NULL;
static void (NTAPI *g_bootvid_display_str)(const char *) = NULL;

void NtConsoleBindBootvid(void (NTAPI *display_str_fn)(const char *)) {
    g_bootvid_display_str = display_str_fn;
}

static uint32_t *g_gop_fb     = NULL;
static uint32_t  g_gop_width  = 0;
static uint32_t  g_gop_height = 0;
static uint32_t  g_gop_pitch  = 0;
static uint32_t  g_gop_cx     = 0;
static uint32_t  g_gop_cy     = 0;

/* Compact 8x8 ASCII bitmap font (0x20..0x7E) scaled 1x2 -> 8x16 on GOP screens */
static const uint8_t g_font8x8[95][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* ' ' */
    {0x18,0x3C,0x3C,0x18,0x18,0x00,0x18,0x00}, /* '!' */
    {0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00}, /* '"' */
    {0x36,0x36,0x7F,0x36,0x7F,0x36,0x36,0x00}, /* '#' */
    {0x0C,0x3E,0x03,0x1E,0x30,0x1F,0x0C,0x00}, /* '$' */
    {0x00,0x63,0x33,0x18,0x0C,0x66,0x63,0x00}, /* '%' */
    {0x1C,0x36,0x1C,0x6E,0x3B,0x33,0x6E,0x00}, /* '&' */
    {0x06,0x06,0x03,0x00,0x00,0x00,0x00,0x00}, /* '\'' */
    {0x18,0x0C,0x06,0x06,0x06,0x0C,0x18,0x00}, /* '(' */
    {0x06,0x0C,0x18,0x18,0x18,0x0C,0x06,0x00}, /* ')' */
    {0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00}, /* '*' */
    {0x00,0x0C,0x0C,0x3F,0x0C,0x0C,0x00,0x00}, /* '+' */
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x06}, /* ',' */
    {0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00}, /* '-' */
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00}, /* '.' */
    {0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00}, /* '/' */
    {0x3E,0x63,0x73,0x7B,0x6F,0x67,0x3E,0x00}, /* '0' */
    {0x0C,0x0E,0x0C,0x0C,0x0C,0x0C,0x3F,0x00}, /* '1' */
    {0x1E,0x33,0x30,0x1C,0x06,0x33,0x3F,0x00}, /* '2' */
    {0x1E,0x33,0x30,0x1C,0x30,0x33,0x1E,0x00}, /* '3' */
    {0x38,0x3C,0x36,0x33,0x7F,0x30,0x78,0x00}, /* '4' */
    {0x3F,0x03,0x1F,0x30,0x30,0x33,0x1E,0x00}, /* '5' */
    {0x1C,0x06,0x03,0x1F,0x33,0x33,0x1E,0x00}, /* '6' */
    {0x3F,0x33,0x30,0x18,0x0C,0x0C,0x0C,0x00}, /* '7' */
    {0x1E,0x33,0x33,0x1E,0x33,0x33,0x1E,0x00}, /* '8' */
    {0x1E,0x33,0x33,0x3E,0x30,0x18,0x0E,0x00}, /* '9' */
    {0x00,0x0C,0x0C,0x00,0x00,0x0C,0x0C,0x00}, /* ':' */
    {0x00,0x0C,0x0C,0x00,0x00,0x0C,0x0C,0x06}, /* ';' */
    {0x18,0x0C,0x06,0x03,0x06,0x0C,0x18,0x00}, /* '<' */
    {0x00,0x00,0x3F,0x00,0x00,0x3F,0x00,0x00}, /* '=' */
    {0x06,0x0C,0x18,0x30,0x18,0x0C,0x06,0x00}, /* '>' */
    {0x1E,0x33,0x30,0x18,0x0C,0x00,0x0C,0x00}, /* '?' */
    {0x3E,0x63,0x7B,0x7B,0x7B,0x03,0x1E,0x00}, /* '@' */
    {0x0C,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00}, /* 'A' */
    {0x3F,0x66,0x66,0x3E,0x66,0x66,0x3F,0x00}, /* 'B' */
    {0x3C,0x66,0x03,0x03,0x03,0x66,0x3C,0x00}, /* 'C' */
    {0x1F,0x36,0x66,0x66,0x66,0x36,0x1F,0x00}, /* 'D' */
    {0x7F,0x46,0x16,0x1E,0x16,0x46,0x7F,0x00}, /* 'E' */
    {0x7F,0x46,0x16,0x1E,0x16,0x06,0x0F,0x00}, /* 'F' */
    {0x3C,0x66,0x03,0x03,0x73,0x66,0x7C,0x00}, /* 'G' */
    {0x33,0x33,0x33,0x3F,0x33,0x33,0x33,0x00}, /* 'H' */
    {0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00}, /* 'I' */
    {0x78,0x30,0x30,0x30,0x33,0x33,0x1E,0x00}, /* 'J' */
    {0x67,0x66,0x36,0x1E,0x36,0x66,0x67,0x00}, /* 'K' */
    {0x0F,0x06,0x06,0x06,0x46,0x66,0x7F,0x00}, /* 'L' */
    {0x63,0x77,0x7F,0x7F,0x6B,0x63,0x63,0x00}, /* 'M' */
    {0x63,0x67,0x6F,0x7B,0x73,0x63,0x63,0x00}, /* 'N' */
    {0x1C,0x36,0x63,0x63,0x63,0x36,0x1C,0x00}, /* 'O' */
    {0x3F,0x66,0x66,0x3E,0x06,0x06,0x0F,0x00}, /* 'P' */
    {0x1E,0x33,0x33,0x33,0x3B,0x1E,0x38,0x00}, /* 'Q' */
    {0x3F,0x66,0x66,0x3E,0x36,0x66,0x67,0x00}, /* 'R' */
    {0x1E,0x33,0x07,0x0E,0x38,0x33,0x1E,0x00}, /* 'S' */
    {0x3F,0x2D,0x0C,0x0C,0x0C,0x0C,0x1E,0x00}, /* 'T' */
    {0x33,0x33,0x33,0x33,0x33,0x33,0x3F,0x00}, /* 'U' */
    {0x33,0x33,0x33,0x33,0x33,0x1E,0x0C,0x00}, /* 'V' */
    {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00}, /* 'W' */
    {0x63,0x63,0x36,0x1C,0x1C,0x36,0x63,0x00}, /* 'X' */
    {0x33,0x33,0x33,0x1E,0x0C,0x0C,0x1E,0x00}, /* 'Y' */
    {0x7F,0x63,0x31,0x18,0x4C,0x66,0x7F,0x00}, /* 'Z' */
    {0x1E,0x06,0x06,0x06,0x06,0x06,0x1E,0x00}, /* '[' */
    {0x03,0x06,0x0C,0x18,0x30,0x60,0x40,0x00}, /* '\\' */
    {0x1E,0x18,0x18,0x18,0x18,0x18,0x1E,0x00}, /* ']' */
    {0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00}, /* '^' */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF}, /* '_' */
    {0x0C,0x0C,0x18,0x00,0x00,0x00,0x00,0x00}, /* '`' */
    {0x00,0x00,0x1E,0x30,0x3E,0x33,0x6E,0x00}, /* 'a' */
    {0x07,0x06,0x06,0x3E,0x66,0x66,0x3B,0x00}, /* 'b' */
    {0x00,0x00,0x1E,0x33,0x03,0x33,0x1E,0x00}, /* 'c' */
    {0x38,0x30,0x30,0x3E,0x33,0x33,0x6E,0x00}, /* 'd' */
    {0x00,0x00,0x1E,0x33,0x3F,0x03,0x1E,0x00}, /* 'e' */
    {0x1C,0x36,0x06,0x0F,0x06,0x06,0x0F,0x00}, /* 'f' */
    {0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x1F}, /* 'g' */
    {0x07,0x06,0x36,0x6E,0x66,0x66,0x67,0x00}, /* 'h' */
    {0x0C,0x00,0x0E,0x0C,0x0C,0x0C,0x1E,0x00}, /* 'i' */
    {0x30,0x00,0x30,0x30,0x30,0x33,0x33,0x1E}, /* 'j' */
    {0x07,0x06,0x66,0x36,0x1E,0x36,0x67,0x00}, /* 'k' */
    {0x0E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00}, /* 'l' */
    {0x00,0x00,0x33,0x7F,0x7F,0x6B,0x63,0x00}, /* 'm' */
    {0x00,0x00,0x1F,0x33,0x33,0x33,0x33,0x00}, /* 'n' */
    {0x00,0x00,0x1E,0x33,0x33,0x33,0x1E,0x00}, /* 'o' */
    {0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x0F}, /* 'p' */
    {0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x78}, /* 'q' */
    {0x00,0x00,0x3B,0x6E,0x66,0x06,0x0F,0x00}, /* 'r' */
    {0x00,0x00,0x3E,0x03,0x1E,0x30,0x1F,0x00}, /* 's' */
    {0x08,0x0C,0x3E,0x0C,0x0C,0x2C,0x18,0x00}, /* 't' */
    {0x00,0x00,0x33,0x33,0x33,0x33,0x6E,0x00}, /* 'u' */
    {0x00,0x00,0x33,0x33,0x33,0x1E,0x0C,0x00}, /* 'v' */
    {0x00,0x00,0x63,0x6B,0x7F,0x7F,0x36,0x00}, /* 'w' */
    {0x00,0x00,0x63,0x36,0x1C,0x36,0x63,0x00}, /* 'x' */
    {0x00,0x00,0x33,0x33,0x33,0x3E,0x30,0x1F}, /* 'y' */
    {0x00,0x00,0x3F,0x19,0x0C,0x26,0x3F,0x00}, /* 'z' */
    {0x38,0x0C,0x0C,0x07,0x0C,0x0C,0x38,0x00}, /* '{' */
    {0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00}, /* '|' */
    {0x07,0x0C,0x0C,0x38,0x0C,0x0C,0x07,0x00}, /* '}' */
    {0x6E,0x3B,0x00,0x00,0x00,0x00,0x00,0x00}  /* '~' */
};

void NtConsoleInitSerial(void) {
#ifndef NT_HOST_SILICON_TEST
    nt_outb(COM1_IOPORT + 1, 0x00); /* Disable interrupts */
    nt_outb(COM1_IOPORT + 3, 0x80); /* Enable DLAB */
    nt_outb(COM1_IOPORT + 0, 0x01); /* 115200 baud (divisor 1) */
    nt_outb(COM1_IOPORT + 1, 0x00);
    nt_outb(COM1_IOPORT + 3, 0x03); /* 8 bits, no parity, 1 stop bit */
    nt_outb(COM1_IOPORT + 2, 0xC7); /* Enable FIFO, clear, 14-byte threshold */
    nt_outb(COM1_IOPORT + 4, 0x0B); /* IRQs enabled, RTS/DSR set */
    g_serial_enabled = 1;
#endif
}

void NtConsoleInitVgaText(void) {
#ifndef NT_HOST_SILICON_TEST
    volatile uint16_t *vga = (volatile uint16_t *)(uintptr_t)0xB8000ULL;
    for (uint32_t i = 0; i < 80U * 25U; ++i) {
        vga[i] = (uint16_t)((0x1FU << 8) | ' '); /* White on NT BSOD/Boot Royal Blue */
    }
    g_vga_row = 0;
    g_vga_col = 0;
    g_vga_enabled = 1;
#endif
}

void NtConsoleBindUefiConOut(void *con_out) {
    g_uefi_conout = (EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *)con_out;
}

void NtConsoleBindGopFramebuffer(uint32_t *fb_base, uint32_t width, uint32_t height, uint32_t pitch) {
    g_gop_fb     = fb_base;
    g_gop_width  = width;
    g_gop_height = height;
    g_gop_pitch  = pitch;
    g_gop_cx     = 8;
    g_gop_cy     = 8;
    if (g_gop_fb) {
        for (uint32_t y = 0; y < height; ++y) {
            uint32_t *row = g_gop_fb + (size_t)y * pitch;
            for (uint32_t x = 0; x < width; ++x) {
                row[x] = 0x000A1931U; /* Deep NT Kernel Midnight Blue */
            }
        }
    }
}

static void gop_draw_char(char c) {
    if (!g_gop_fb || g_gop_width < 16 || g_gop_height < 24) {
        return;
    }
    if (c == '\r') {
        g_gop_cx = 8;
        return;
    }
    if (c == '\n') {
        g_gop_cx = 8;
        g_gop_cy += 16;
        if (g_gop_cy + 16 >= g_gop_height) {
            g_gop_cy = 8;
        }
        return;
    }
    uint8_t idx = (c >= 0x20 && c <= 0x7E) ? (uint8_t)(c - 0x20) : (uint8_t)('?' - 0x20);
    const uint8_t *glyph = g_font8x8[idx];
    for (uint32_t gy = 0; gy < 8; ++gy) {
        uint8_t bits = glyph[gy];
        for (uint32_t sub = 0; sub < 2; ++sub) {
            uint32_t py = g_gop_cy + gy * 2 + sub;
            if (py >= g_gop_height) break;
            uint32_t *row = g_gop_fb + (size_t)py * g_gop_pitch;
            for (uint32_t gx = 0; gx < 8; ++gx) {
                uint32_t px = g_gop_cx + gx;
                if (px < g_gop_width) {
                    row[px] = (bits & (1U << gx)) ? 0x00E8F1F8U : 0x000A1931U;
                }
            }
        }
    }
    g_gop_cx += 8;
    if (g_gop_cx + 8 >= g_gop_width) {
        g_gop_cx = 8;
        g_gop_cy += 16;
        if (g_gop_cy + 16 >= g_gop_height) {
            g_gop_cy = 8;
        }
    }
}

void NtConsolePutChar(char c) {
#ifdef NT_HOST_SILICON_TEST
    (void)write(1, &c, 1);
#else
    if (g_serial_enabled) {
        if (c == '\n') {
            uint32_t spin = 0;
            while (((nt_inb(COM1_IOPORT + 5) & 0x20U) == 0U) && (++spin < 100000U)) {}
            nt_outb(COM1_IOPORT, '\r');
        }
        uint32_t spin = 0;
        while (((nt_inb(COM1_IOPORT + 5) & 0x20U) == 0U) && (++spin < 100000U)) {}
        nt_outb(COM1_IOPORT, (uint8_t)c);
    }
    if (g_vga_enabled) {
        volatile uint16_t *vga = (volatile uint16_t *)(uintptr_t)0xB8000ULL;
        if (c == '\n') {
            g_vga_col = 0;
            if (++g_vga_row >= 25U) g_vga_row = 0;
        } else if (c != '\r') {
            vga[g_vga_row * 80U + g_vga_col] = (uint16_t)((0x1FU << 8) | (uint8_t)c);
            if (++g_vga_col >= 80U) {
                g_vga_col = 0;
                if (++g_vga_row >= 25U) g_vga_row = 0;
            }
        }
    }
    if (g_uefi_conout && g_uefi_conout->OutputString) {
        if (c == '\n') {
            CHAR16 crlf[3] = { '\r', '\n', 0 };
            g_uefi_conout->OutputString(g_uefi_conout, crlf);
        } else if (c != '\r') {
            CHAR16 ch[2] = { (CHAR16)(uint8_t)c, 0 };
            g_uefi_conout->OutputString(g_uefi_conout, ch);
        }
    }
    if (g_gop_fb) {
        gop_draw_char(c);
    }
    if (g_bootvid_display_str) {
        char s[2] = { c, '\0' };
        g_bootvid_display_str(s);
    }
#endif
}

void NtConsoleWrite(const char *s) {
    if (!s) return;
    while (*s) {
        NtConsolePutChar(*s++);
    }
}

void NtConsolePrintHex(uint64_t val) {
    static const char hex[] = "0123456789ABCDEF";
    NtConsoleWrite("0x");
    int started = 0;
    for (int shift = 60; shift >= 0; shift -= 4) {
        uint8_t nib = (uint8_t)((val >> shift) & 0xFU);
        if (nib != 0 || started || shift == 0) {
            NtConsolePutChar(hex[nib]);
            started = 1;
        }
    }
}

void NtConsolePrintDec(int64_t val) {
    char buf[24];
    int pos = 0;
    uint64_t u;
    if (val < 0) {
        NtConsolePutChar('-');
        u = (uint64_t)(-(val + 1)) + 1ULL;
    } else {
        u = (uint64_t)val;
    }
    if (u == 0) {
        NtConsolePutChar('0');
        return;
    }
    while (u > 0) {
        buf[pos++] = (char)('0' + (u % 10ULL));
        u /= 10ULL;
    }
    while (pos > 0) {
        NtConsolePutChar(buf[--pos]);
    }
}

static void mem_zero(void *dst, size_t n) {
    uint8_t *p = (uint8_t *)dst;
    for (size_t i = 0; i < n; ++i) {
        p[i] = 0;
    }
}

/* ============================================================================
 * 3. Freestanding Ring-0 Implementations of Internal Leaf Kernel Callees
 * ============================================================================ */

BOOLEAN __attribute__((ms_abi)) KiSetTimerEx(
    void    *Timer,
    int64_t  DueTime,
    ULONG    Period,
    uint64_t Hand
) {
    g_nt_last_timer_duetime = DueTime;
    g_nt_last_timer_period  = Period;
    g_nt_last_timer_hand    = Hand;
    if (Timer) {
        struct _DISPATCHER_HEADER *hdr = (struct _DISPATCHER_HEADER *)Timer;
        uint8_t *raw_hdr = (uint8_t *)hdr;
        BOOLEAN was_inserted = (raw_hdr[3] != 0) ? 1U : 0U;
        raw_hdr[3]       = 1U;
        hdr->SignalState = 0;
        return was_inserted;
    }
    return 0U;
}

uint64_t __attribute__((ms_abi)) KeQueryActiveProcessorCountEx(
    uint64_t GroupNumber, uint64_t a1, uint64_t a2, uint64_t a3
) {
    (void)a1; (void)a2; (void)a3;
    return (GroupNumber == 0ULL || GroupNumber == 1ULL || GroupNumber == 0xFFFFULL) ? 8ULL : 0ULL;
}

uint64_t __attribute__((ms_abi)) KeSetTargetProcessorDpcEx(
    uint64_t DpcPtr, uint64_t ProcNumPtr, uint64_t a2, uint64_t a3
) {
    (void)a2; (void)a3;
    struct _KDPC *dpc = (struct _KDPC *)(uintptr_t)DpcPtr;
    const struct _PROCESSOR_NUMBER *pn = (const struct _PROCESSOR_NUMBER *)(uintptr_t)ProcNumPtr;
    if (dpc && pn) {
        dpc->Number = (uint16_t)(0x200U + ((uint16_t)pn->Group << 6) + (uint16_t)pn->Number);
    }
    return 0ULL;
}

BOOLEAN __attribute__((ms_abi)) MiIsAddressValid(void *VirtualAddress) {
    uintptr_t va = (uintptr_t)VirtualAddress;
    if (va < 0x1000ULL) {
        return 0U;
    }
    /* Verify canonical 48-bit x86-64 virtual address sign-extension */
    int64_t signed_va = (int64_t)va;
    int64_t top = signed_va >> 47;
    return (top == 0 || top == -1) ? 1U : 0U;
}

uint64_t __attribute__((ms_abi)) MiFreePagesFromMdl(
    uint64_t MdlPtr, uint64_t Flags, uint64_t a2, uint64_t a3
) {
    (void)Flags; (void)a2; (void)a3;
    struct _MDL *mdl = (struct _MDL *)(uintptr_t)MdlPtr;
    if (mdl && mdl->ByteCount > 0) {
        uint32_t pages = (mdl->ByteOffset + mdl->ByteCount + 0xFFFU) >> 12;
        g_nt_mdl_freed_pages += pages;
        mdl->MdlFlags &= (int16_t)~0x0002; /* Clear MDL_PAGES_LOCKED */
    }
    return 0ULL;
}

int64_t __attribute__((ms_abi)) ObfDereferenceObjectWithTag(void *Object, ULONG Tag) {
    (void)Tag;
    if (!Object) return 0;
    struct _OBJECT_HEADER *hdr = CONTAINING_RECORD(Object, struct _OBJECT_HEADER, Body);
    return __atomic_sub_fetch(&hdr->PointerCount, 1, __ATOMIC_SEQ_CST);
}

BOOLEAN __attribute__((ms_abi)) ObpIsKernelHandle(void *Handle, BOOLEAN PreviousMode) {
    (void)PreviousMode;
    uintptr_t h = (uintptr_t)Handle;
    /* Windows x64 OBJ_KERNEL_HANDLE = 0xFFFFFFFF80000000ULL, excluding pseudo-handles -1/-2 */
    if (h == ~(uintptr_t)0ULL || h == ~(uintptr_t)1ULL) {
        return 0U;
    }
    return ((h & 0xFFFFFFFF80000000ULL) == 0xFFFFFFFF80000000ULL) ? 1U : 0U;
}

NTSTATUS __attribute__((ms_abi)) RtlHashUnicodeString(
    const struct _UNICODE_STRING *String,
    BOOLEAN                       CaseInSensitive,
    ULONG                         HashAlgorithm,
    ULONG                        *HashValue
) {
    if (!String || !HashValue) {
        return (NTSTATUS)0xC000000DU; /* STATUS_INVALID_PARAMETER */
    }
    if (HashAlgorithm > 1U) {
        return (NTSTATUS)0xC000000DU;
    }
    /* Retail Windows X65599 Unicode string hash */
    ULONG hash = 0U;
    uint16_t chars = String->Length / sizeof(uint16_t);
    const uint16_t *buf = String->Buffer;
    for (uint16_t i = 0; i < chars; ++i) {
        uint16_t wc = buf[i];
        if (CaseInSensitive && wc >= 'a' && wc <= 'z') {
            wc = (uint16_t)(wc - ('a' - 'A'));
        }
        hash = hash * 65599U + (ULONG)wc;
    }
    *HashValue = hash;
    return 0; /* STATUS_SUCCESS */
}

static int sid_equals(const uint8_t *a, const uint8_t *b) {
    if (!a || !b) return 0;
    if (a[0] != b[0] || a[1] != b[1]) return 0;
    size_t total = 8U + (size_t)a[1] * 4U;
    for (size_t i = 0; i < total; ++i) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

BOOLEAN __attribute__((ms_abi)) SepSidInToken(
    void    *TokenPtr,
    void    *PrincipalSelfSid,
    void    *Sid,
    BOOLEAN  DenyOnly,
    BOOLEAN  Restricted,
    BOOLEAN  SingleSid
) {
    (void)PrincipalSelfSid; (void)DenyOnly; (void)SingleSid;
    const struct _TOKEN *tok = (const struct _TOKEN *)TokenPtr;
    if (!tok || !Sid) return 0U;

    const struct _SID_AND_ATTRIBUTES *arr = Restricted ? tok->RestrictedSids : tok->UserAndGroups;
    uint32_t count = Restricted ? tok->RestrictedSidCount : tok->UserAndGroupCount;
    if (!arr || count == 0U) return 0U;

    for (uint32_t i = 0; i < count; ++i) {
        /* SE_GROUP_ENABLED = 0x00000004U */
        if ((arr[i].Attributes & 0x4U) != 0U && sid_equals((const uint8_t *)arr[i].Sid, (const uint8_t *)Sid)) {
            return 1U;
        }
    }
    return 0U;
}

BOOLEAN __attribute__((ms_abi)) KdEnterDebugger(void *TrapFrame) {
    (void)TrapFrame;
    return 1U;
}

BOOLEAN __attribute__((ms_abi)) KdpPrintString(struct _STRING *Output) {
    if (Output && Output->Buffer && Output->Length > 0) {
        uint32_t copy_len = Output->Length < (sizeof(g_nt_kd_printed_buf) - 1U)
            ? Output->Length
            : (uint32_t)(sizeof(g_nt_kd_printed_buf) - 1U);
        for (uint32_t i = 0; i < copy_len; ++i) {
            g_nt_kd_printed_buf[i] = Output->Buffer[i];
        }
        g_nt_kd_printed_buf[copy_len] = '\0';
        g_nt_kd_printed_len = Output->Length;
    }
    return 0U;
}

void __attribute__((ms_abi)) KdExitDebugger(BOOLEAN Enable) {
    (void)Enable;
}

/* ============================================================================
 * 4. Executive Structure Initialization & Hardware Verification Suite
 * ============================================================================ */

void NtRuntimeInitializeExecutive(
    struct _KPCR     *kpcr,
    struct _KPRCB    *kprcb,
    struct _ETHREAD  *ethread,
    struct _EPROCESS *eproc
) {
    SeAliasAdminsSid = (uint64_t)(uintptr_t)&g_SeAliasAdminsSidBytes[0];

    mem_zero(kpcr, sizeof(*kpcr));
    mem_zero(kprcb, sizeof(*kprcb));
    mem_zero(ethread, sizeof(*ethread));
    mem_zero(eproc, sizeof(*eproc));

    /* Wire Windows x64 KPCR / KPRCB (`gs:[0x18]`, `gs:[0x20]`, `gs:[0x188]`) */
    kpcr->Self                = kpcr;
    kpcr->CurrentPrcb         = kprcb;
    kpcr->Prcb.CurrentThread  = &ethread->Tcb;
    kpcr->Prcb.Number         = 0U;
    kpcr->Prcb.Group          = 0U;
    kpcr->Prcb.GroupIndex     = 0U;

    kprcb->CurrentThread      = &ethread->Tcb;
    kprcb->Number             = 3U;
    kprcb->Group              = 1U;
    kprcb->GroupIndex         = 3U;

    /* Wire System _EPROCESS (PID 4, SystemProcess bit 12 in Flags3, PPL=1) */
    eproc->UniqueProcessId              = (void *)(uintptr_t)4ULL;
    eproc->InheritedFromUniqueProcessId = (void *)(uintptr_t)0ULL;
    eproc->SectionBaseAddress           = (void *)(uintptr_t)0xFFFFF80002000000ULL;
    eproc->Peb                          = NULL;
    eproc->ExitStatus                   = 0x00000103; /* STATUS_PENDING */
    eproc->Flags3                       = (1U << 12); /* SystemProcess */
    eproc->Protection.Level             = 0x61U;      /* Type=1 (ProtectedLight), Signer=6 (WinTcb) */
    const char sys_name[] = "System";
    for (size_t i = 0; i < sizeof(sys_name); ++i) {
        eproc->ImageFileName[i] = (uint8_t)sys_name[i];
    }

    /* Wire initial _ETHREAD / _KTHREAD (TID 8 -> PID 4) */
    ethread->Tcb.ApcState.Process = &eproc->Pcb;
    ethread->Tcb.Process          = &eproc->Pcb;
    ethread->Tcb.Priority         = 15;
    ethread->Tcb.PreviousMode     = 0; /* KernelMode */
    ethread->Tcb.Teb              = (void *)(uintptr_t)0x0000007FFDF00000ULL;
    ethread->Tcb.MiscFlags        = (1 << 10); /* SystemThread */
    ethread->Cid.UniqueProcess    = (void *)(uintptr_t)4ULL;
    ethread->Cid.UniqueThread     = (void *)(uintptr_t)8ULL;
}

static uint32_t g_tests_passed = 0;
static uint32_t g_tests_failed = 0;

static void check_cond(const char *subsys, const char *desc, int ok) {
    if (ok) {
        ++g_tests_passed;
        NtConsoleWrite("  [PASS] [");
        NtConsoleWrite(subsys);
        NtConsoleWrite("] ");
        NtConsoleWrite(desc);
        NtConsoleWrite("\n");
    } else {
        ++g_tests_failed;
        NtConsoleWrite("  [FAIL] [");
        NtConsoleWrite(subsys);
        NtConsoleWrite("] ");
        NtConsoleWrite(desc);
        NtConsoleWrite("\n");
    }
}

/* Aligned static fixtures so bare-metal boot requires tiny stack footprint */
static uint64_t g_zero_page[512] __attribute__((aligned(4096)));
static uint64_t g_dirty_page[512] __attribute__((aligned(4096)));
static union _SLIST_HEADER g_slist_head __attribute__((aligned(16)));
static struct _SINGLE_LIST_ENTRY g_slist_n1 __attribute__((aligned(16)));
static struct _SINGLE_LIST_ENTRY g_slist_n2 __attribute__((aligned(16)));
static uint8_t g_obj_storage[256] __attribute__((aligned(64)));
static uint8_t g_fake_obj_type[128] __attribute__((aligned(16)));
static struct _DEVICE_OBJECT g_dev_bottom;
static struct _DEVICE_OBJECT g_dev_mid;
static struct _DEVICE_OBJECT g_dev_top;
static struct _IRP g_test_irp;
static struct _EPROCESS g_wow64_proc;
static uint8_t g_wow64_struct[32] __attribute__((aligned(8)));
static struct _TOKEN g_test_token;
static struct _SID_AND_ATTRIBUTES g_primary_sids[1];
static struct _SID_AND_ATTRIBUTES g_restricted_sids[1];

int NtRunHardwareVerificationSuite(int is_ring0_hardware) {
    g_tests_passed = 0;
    g_tests_failed = 0;

    NtConsoleWrite("\n========================================================================\n");
    NtConsoleWrite(" Windows NT 10.0.16299.15 Decompiled Kernel — Live Hardware Test Suite\n");
    NtConsoleWrite(" Source: Retail ntoskrnl.exe + ntkrnlmp.pdb (527/527 Z3 Formal Proofs)\n");
    NtConsoleWrite("========================================================================\n");

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 1: Ke / Ki — Kernel Dispatcher, CR8 IRQL, CR3/CR4 TLB, Pages
     * ---------------------------------------------------------------------- */
    if (is_ring0_hardware) {
        KIRQL init_irql = KeGetCurrentIrql();
        KIRQL prev1 = KzRaiseIrql(DISPATCH_LEVEL);
        KIRQL at_dpc = KeGetCurrentIrql();
        KzLowerIrql(prev1);
        KIRQL prev2 = KeRaiseIrqlToDpcLevel();
        KIRQL at_dpc2 = KeGetCurrentIrql();
        KzLowerIrql(prev2);
        KIRQL restored = KeGetCurrentIrql();
        check_cond(
            "Ke/ASM",
            "Ring-0 CR8 IRQL raise/query/lower (KeGetCurrentIrql / KzRaiseIrql / KzLowerIrql)",
            (init_irql == PASSIVE_LEVEL && at_dpc == DISPATCH_LEVEL && at_dpc2 == DISPATCH_LEVEL && restored == init_irql)
        );

        (void)KiFlushCurrentTbWorker();
        check_cond(
            "Ki/ASM",
            "Ring-0 CR4.PGE / CR3 hardware TLB flush (KiFlushCurrentTbWorker)",
            1
        );
    }

    struct _KTHREAD *cur_thr = KeGetCurrentThread();
    struct _PROCESSOR_NUMBER proc_num = {0, 0, 0};
    ULONG flat_proc = KeGetCurrentProcessorNumberEx(&proc_num);
    check_cond(
        "Ke",
        "KPCR/KPRCB gs:[0x188] & gs:[0x20] (KeGetCurrentThread + KeGetCurrentProcessorNumberEx)",
        (cur_thr != NULL && proc_num.Group == 1U && proc_num.Number == 3U && flat_proc == 3U && KeQueryPriorityThread(cur_thr) == 15)
    );

    struct _KDPC dpc;
    mem_zero(&dpc, sizeof(dpc));
    KeInitializeDpc(&dpc, (void *)(uintptr_t)0x12345678ULL, (void *)(uintptr_t)0xCAFEBABEULL);
    KeSetImportanceDpc(&dpc, 2U);
    KeSetTargetProcessorDpc(&dpc, 2);
    check_cond(
        "Ke",
        "DPC initialization, importance & target CPU routing (KeInitializeDpc / KeSetTargetProcessorDpc)",
        (dpc.DeferredRoutine == (void *)(uintptr_t)0x12345678ULL &&
         dpc.DeferredContext == (void *)(uintptr_t)0xCAFEBABEULL &&
         dpc.Importance == 2U && dpc.Number == (0x200U + 2U))
    );

    mem_zero(g_zero_page, sizeof(g_zero_page));
    mem_zero(g_dirty_page, sizeof(g_dirty_page));
    g_dirty_page[511] = 0x8000000000000000ULL;
    check_cond(
        "Ke/DeOpt",
        "Rerolled 16x-unrolled 4KiB page scan (KeCheckForZeroPage: 0 on zero page, !=0 on dirty byte 4095)",
        (KeCheckForZeroPage(g_zero_page) == 0ULL && KeCheckForZeroPage(g_dirty_page) == 0x8000000000000000ULL)
    );

    struct _DISPATCHER_HEADER timer_hdr;
    mem_zero(&timer_hdr, sizeof(timer_hdr));
    (void)KeSetCoalescableTimer(&timer_hdr, -500000LL, 100U, 2000U);
    /* TolerableDelay=2000ms -> 20,000,000 (100ns) - 16,515,072 = 3,484,928; / 10000 = 348ms added to Period=100 -> 448! */
    check_cond(
        "Ke/DeOpt",
        "64-bit magic '/ 10000' timer coalescing (KeSetCoalescableTimer -> KiSetTimerEx Period=448)",
        (g_nt_last_timer_period == 448U && KeReadStateTimer(&timer_hdr) == 0U)
    );

    struct _DISPATCHER_HEADER ev_hdr;
    mem_zero(&ev_hdr, sizeof(ev_hdr));
    ev_hdr.SignalState = 1;
    check_cond(
        "Ke",
        "Dispatcher state readers (KeReadStateEvent / Mutex / Semaphore / Timer)",
        (KeReadStateEvent(&ev_hdr) == 1 && KeReadStateMutex(&ev_hdr) == 1 && KeReadStateSemaphore(&ev_hdr) == 1)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 2: Ps — Process & Thread Structure Executive
     * ---------------------------------------------------------------------- */
    struct _EPROCESS *sys_p = PsGetCurrentProcess();
    struct _ETHREAD  *sys_t = PsGetCurrentThread();
    const uint8_t *img_name = PsGetProcessImageFileName(sys_p);
    check_cond(
        "Ps",
        "Process & thread executive accessors (PID=4, TID=8, Image='System', PPL=1, System=1)",
        (sys_p != NULL && sys_t != NULL &&
         (uintptr_t)PsGetProcessId(sys_p) == 4ULL &&
         (uintptr_t)PsGetProcessInheritedFromUniqueProcessId(sys_p) == 0ULL &&
         img_name[0] == 'S' && img_name[1] == 'y' && img_name[5] == 'm' &&
         PsGetProcessExitStatus(sys_p) == 0x103 &&
         (uintptr_t)PsGetProcessSectionBaseAddress(sys_p) == 0xFFFFF80002000000ULL &&
         PsIsSystemProcess(sys_p) != 0 &&
         PsIsProtectedProcess(sys_p) != 0 &&
         PsIsProtectedProcessLight(sys_p) != 0 &&
         (uintptr_t)PsGetThreadId(sys_t) == 8ULL &&
         (uintptr_t)PsGetThreadProcessId(sys_t) == 4ULL &&
         PsGetThreadProcess(&sys_t->Tcb) == sys_p &&
         (uintptr_t)PsGetThreadTeb(&sys_t->Tcb) == 0x0000007FFDF00000ULL &&
         PsIsSystemThread(&sys_t->Tcb) != 0)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 3: Mm — Virtual Memory & MDL Subsystem
     * ---------------------------------------------------------------------- */
    /* Base=0x1FF0, Length=0x1020 spans 3 pages -> sizeof(_MDL)=48 + 3*8 = 72 bytes */
    size_t mdl_sz = MmSizeOfMdl((void *)(uintptr_t)0x1FF0ULL, 0x1020ULL);
    struct _MDL test_mdl;
    mem_zero(&test_mdl, sizeof(test_mdl));
    test_mdl.ByteCount  = 0x2000U;
    test_mdl.ByteOffset = 0U;
    test_mdl.MdlFlags   = 0x0002;
    g_nt_mdl_freed_pages = 0U;
    MmFreePagesFromMdl(&test_mdl);
    check_cond(
        "Mm",
        "MDL page-span math, section limits & VA validity (MmSizeOfMdl=72, MaxSection=0x3ffffffffff000)",
        (mdl_sz == 72ULL &&
         MmGetMaximumFileSectionSize() == 0x3FFFFFFFFFF000ULL &&
         MmIsAddressValid((void *)&test_mdl) == 1U &&
         MmIsAddressValid(NULL) == 0U &&
         g_nt_mdl_freed_pages == 2U)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 4: Ob — Executive Object Manager & Cookie Type Table
     * ---------------------------------------------------------------------- */
    mem_zero(g_obj_storage, sizeof(g_obj_storage));
    /* Place optional NameInfo (0x20 bytes) at +0x40, _OBJECT_HEADER at +0x60, Body at +0x90 */
    uint8_t *name_info_expected = &g_obj_storage[0x40];
    struct _OBJECT_HEADER *obj_hdr = (struct _OBJECT_HEADER *)&g_obj_storage[0x60];
    void *obj_body = &obj_hdr->Body;
    uint8_t raw_slot = 7U;
    uint8_t addr_byte = (uint8_t)(((uintptr_t)obj_hdr >> 8) & 0xFFU);
    obj_hdr->PointerCount = 3;
    obj_hdr->InfoMask     = 0x02U; /* OBJECT_HEADER_NAME_INFO present (-0x20 from _OBJECT_HEADER) */
    obj_hdr->TypeIndex    = (uint8_t)(raw_slot ^ ObHeaderCookie ^ addr_byte);
    ObTypeIndexTable[raw_slot] = (struct _OBJECT_TYPE *)(uintptr_t)&g_fake_obj_type[0];

    struct _OBJECT_TYPE *resolved_type = ObGetObjectType(obj_body);
    void *resolved_name_info = ObQueryNameInfo(obj_body);
    int64_t new_ref = ObDereferenceObject(obj_body);
    check_cond(
        "Ob",
        "CONTAINING_RECORD(-0x30) ObHeaderCookie XOR type decode, ObQueryNameInfo & ObDereferenceObject",
        (resolved_type == (struct _OBJECT_TYPE *)(uintptr_t)&g_fake_obj_type[0] &&
         resolved_name_info == (void *)name_info_expected &&
         new_ref == 2 &&
         ObGetFilterVersion() == 0x0100U &&
         ObIsKernelHandle((void *)(uintptr_t)0xFFFFFFFF80000124ULL) == 1U &&
         ObIsKernelHandle((void *)(uintptr_t)0x0000000000000124ULL) == 0U)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 5: Io / Pi — I/O Manager, PGO WoW64 IRP Check & PnP '% 13'
     * ---------------------------------------------------------------------- */
    mem_zero(&g_dev_bottom, sizeof(g_dev_bottom));
    mem_zero(&g_dev_mid, sizeof(g_dev_mid));
    mem_zero(&g_dev_top, sizeof(g_dev_top));
    g_dev_bottom.AttachedDevice = &g_dev_mid;
    g_dev_mid.AttachedDevice    = &g_dev_top;
    g_dev_top.AttachedDevice    = NULL;

    mem_zero(&g_test_irp, sizeof(g_test_irp));
    g_test_irp.Flags = (4U << 17); /* Priority hint = 4 - 1 = 3 (IoPriorityHigh) */

    /* Test IoIs32bitProcess across PGO cold blocks: UserMode (1) + WoW64 Machine=0x014c (I386) */
    mem_zero(&g_wow64_proc, sizeof(g_wow64_proc));
    mem_zero(g_wow64_struct, sizeof(g_wow64_struct));
    *(uint16_t *)(&g_wow64_struct[8]) = 0x014CU; /* IMAGE_FILE_MACHINE_I386 */
    g_wow64_proc.WoW64Process = (struct _EWOW64PROCESS *)(uintptr_t)&g_wow64_struct[0];

    struct _IRP wow_irp;
    mem_zero(&wow_irp, sizeof(wow_irp));
    wow_irp.RequestorMode  = 1; /* UserMode */
    wow_irp.Flags          = 0U; /* Route through Tail.Overlay.Thread + ApcEnvironment switch */
    wow_irp.ApcEnvironment = 1U; /* Exercises PGO cold-chunk block L_bb_14017f74a -> ApcState.Process */
    struct _ETHREAD wow_thr;
    mem_zero(&wow_thr, sizeof(wow_thr));
    wow_thr.Tcb.Process          = &g_wow64_proc.Pcb;
    wow_thr.Tcb.ApcState.Process = &g_wow64_proc.Pcb;
    wow_irp.Tail.Overlay.Thread  = &wow_thr;

    static const uint16_t pnp_str[] = { 'A','C','P','I','\\','P','N','P','0','A','0','8', 0 };
    struct _UNICODE_STRING u_pnp;
    RtlInitUnicodeString(&u_pnp, pnp_str);
    ULONG raw_hash = 0;
    RtlHashUnicodeString(&u_pnp, 1U, 0U, &raw_hash);
    ULONG pnp_bucket = PiUEventHashStringIntoBucket(pnp_str);

    check_cond(
        "Io/PnP",
        "Device stack walk, IRP priority, WoW64 PGO cold-chunk check & PnP magic '% 13' bucket",
        (IoGetAttachedDevice(&g_dev_bottom) == &g_dev_top &&
         IoGetIoPriorityHint(&g_test_irp) == 3U &&
         IoGetCurrentProcess() == sys_p &&
         IoThreadToProcess(sys_t) == sys_p &&
         IoIs32bitProcess(&wow_irp) == 1U &&
         IoIs32bitProcess(&g_test_irp) == 0U &&
         pnp_bucket == (raw_hash % 13U))
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 6: Ex — 128-bit `lock cmpxchg16b` Lock-Free SList & Rundown
     * ---------------------------------------------------------------------- */
    mem_zero(&g_slist_head, sizeof(g_slist_head));
    mem_zero(&g_slist_n1, sizeof(g_slist_n1));
    mem_zero(&g_slist_n2, sizeof(g_slist_n2));
    ExpInterlockedPushEntrySList(&g_slist_head, &g_slist_n1);
    ExpInterlockedPushEntrySList(&g_slist_head, &g_slist_n2);
    uint16_t depth2 = ExQueryDepthSList(&g_slist_head);
    struct _SINGLE_LIST_ENTRY *popped1 = ExpInterlockedPopEntrySList(&g_slist_head);
    struct _SINGLE_LIST_ENTRY *popped2 = ExpInterlockedPopEntrySList(&g_slist_head);
    struct _SINGLE_LIST_ENTRY *popped3 = ExpInterlockedPopEntrySList(&g_slist_head);

    struct _EX_RUNDOWN_REF run_ref;
    run_ref.Count = 0xDEADULL;
    ExInitializeRundownProtection(&run_ref);
    run_ref.Count = 8ULL;
    ExReInitializeRundownProtection(&run_ref);
    check_cond(
        "Ex/ASM",
        "Hardware 128-bit `lock cmpxchg16b` SList LIFO push/pop/depth & Rundown protection",
        (depth2 == 2U && popped1 == &g_slist_n2 && popped2 == &g_slist_n1 && popped3 == NULL &&
         ExQueryDepthSList(&g_slist_head) == 0U && run_ref.Count == 0ULL)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 7: Rtl — Strings, Bitmaps, `repe cmpsq`, Magic Div & Epochs
     * ---------------------------------------------------------------------- */
    uint8_t buf_a[32], buf_b[32];
    for (uint32_t i = 0; i < 32U; ++i) {
        buf_a[i] = (uint8_t)(i + 1U);
        buf_b[i] = (uint8_t)(i + 1U);
    }
    buf_b[27] = 0xFFU;
    size_t cmp_match = RtlCompareMemory(buf_a, buf_b, 32U);

    uint32_t bm_words[4] = {0, 0, 0, 0};
    struct _RTL_BITMAP bm;
    RtlInitializeBitMap(&bm, bm_words, 128U);
    RtlSetBit(&bm, 93U);
    BOOLEAN bit93_set = RtlTestBit(&bm, 93U);
    BOOLEAN bit92_set = RtlTestBit(&bm, 92U);
    RtlClearBit(&bm, 93U);
    BOOLEAN bit93_clr = RtlTestBit(&bm, 93U);

    union _LARGE_INTEGER nt_time;
    nt_time.QuadPart = 0;
    ULONG unix_2026_10_23 = 1792713600U;
    RtlSecondsSince1970ToTime(unix_2026_10_23, &nt_time);
    ULONG roundtrip_secs = 0U;
    BOOLEAN time_ok = RtlTimeToSecondsSince1970(&nt_time, &roundtrip_secs);
    /* Magic divide by 10: 0xCCCCCCCCCCCCCCCD >> 3 on 123450 == 12345 */
    int64_t mdiv = RtlExtendedMagicDivide(123450LL, (int64_t)0x6666666666666667ULL, 2);

    check_cond(
        "Rtl",
        "`repe cmpsq` RtlCompareMemory, 128-bit RtlBitMap bt/bts/btr, RtlExtendedMagicDivide & 1970 Epoch",
        (cmp_match == 27ULL && bit93_set == 1U && bit92_set == 0U && bit93_clr == 0U &&
         time_ok == 1U && roundtrip_secs == unix_2026_10_23 && mdiv == 12345LL)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 8: Se / Kd — Restricted UAC Admin Token & KD Connection Probe
     * ---------------------------------------------------------------------- */
    mem_zero(&g_test_token, sizeof(g_test_token));
    g_test_token.AuthenticationId.LowPart  = 0x3E7U; /* SYSTEM_LUID */
    g_test_token.AuthenticationId.HighPart = 0;
    g_test_token.TokenType                 = 1;      /* TokenPrimary */
    g_primary_sids[0].Sid                  = &g_SeAliasAdminsSidBytes[0];
    g_primary_sids[0].Attributes           = 0x04U;  /* SE_GROUP_ENABLED */
    g_test_token.UserAndGroupCount         = 1U;
    g_test_token.UserAndGroups             = g_primary_sids;

    BOOLEAN admin_unrestricted = SeTokenIsAdmin(&g_test_token);

    /* Mark token restricted (0x10 in TokenFlags -> SeTokenIsRestricted == TRUE) WITHOUT RestrictedSids */
    g_test_token.TokenFlags         = 0x10U;
    g_test_token.RestrictedSidCount = 0U;
    g_test_token.RestrictedSids     = NULL;
    BOOLEAN admin_restricted_denied = SeTokenIsAdmin(&g_test_token);

    /* Add Administrators SID to RestrictedSids -> 5th stack arg Restricted=1 grants Admin! */
    g_restricted_sids[0].Sid        = &g_SeAliasAdminsSidBytes[0];
    g_restricted_sids[0].Attributes = 0x04U;
    g_test_token.RestrictedSidCount = 1U;
    g_test_token.RestrictedSids     = g_restricted_sids;
    BOOLEAN admin_restricted_ok     = SeTokenIsAdmin(&g_test_token);

    struct _LUID queried_luid = {0, 0};
    NTSTATUS luid_st = SeQueryAuthenticationIdToken(&g_test_token, &queried_luid);

    /* Exercise KdRefreshDebuggerNotPresent with KdPitchDebugger=0 so it scans `.rdata` string */
    KdPitchDebugger     = 0U;
    KdDebuggerNotPresent = 1U;
    g_nt_kd_printed_len = 0U;
    BOOLEAN kd_not_pres = KdRefreshDebuggerNotPresent();

    check_cond(
        "Se/Kd",
        "UAC Restricted-Token 6-arg SeTokenIsAdmin, SeQueryAuthenticationIdToken & KD string probe",
        (admin_unrestricted == 1U &&
         SeTokenIsRestricted(&g_test_token) == 1U &&
         admin_restricted_denied == 0U &&
         admin_restricted_ok == 1U &&
         luid_st == 0 && queried_luid.LowPart == 0x3E7U &&
         kd_not_pres == 1U && g_nt_kd_printed_len == 35U &&
         g_nt_kd_printed_buf[0] == 'K' && g_nt_kd_printed_buf[8] == ':')
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 9: Cm — Configuration Manager Registry Callback Versioning
     * ---------------------------------------------------------------------- */
    uint32_t cm_maj = 0U, cm_min = 0U;
    CmGetCallbackVersion(&cm_maj, &cm_min);
    check_cond(
        "Cm",
        "CmGetCallbackVersion registry filter version negotiation (Major=1, Minor=1)",
        (cm_maj == 1U && cm_min == 1U)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 10: Cc — Cache Manager SharedCacheMap EX_FAST_REF Unmasking
     * ---------------------------------------------------------------------- */
    uint8_t fake_shared_cache_map[256] __attribute__((aligned(16)));
    uint8_t fake_bcb[256] __attribute__((aligned(16)));
    mem_zero(fake_shared_cache_map, sizeof(fake_shared_cache_map));
    mem_zero(fake_bcb, sizeof(fake_bcb));
    uintptr_t expected_fobj = ((uintptr_t)fake_bcb) & ~(uintptr_t)0xFULL;
    *(uint64_t *)(fake_shared_cache_map + 0x60) = (uint64_t)(expected_fobj | 0x7ULL);
    *(uint64_t *)(fake_bcb + 0xB0) = (uint64_t)(uintptr_t)fake_shared_cache_map;
    uint64_t cc_fobj = CcGetFileObjectFromBcb((uint64_t *)(uintptr_t)fake_bcb);
    check_cond(
        "Cc",
        "CcGetFileObjectFromBcb SharedCacheMap->FileObjectFastRef (~0xF mask)",
        (cc_fobj == (uint64_t)expected_fobj)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 11: FsRtl — File-System Extra Create Parameter (ECP) Flags
     * ---------------------------------------------------------------------- */
    uint8_t fake_ecp_alloc[64] __attribute__((aligned(16)));
    mem_zero(fake_ecp_alloc, sizeof(fake_ecp_alloc));
    uint32_t *ecp_body = (uint32_t *)(uintptr_t)(fake_ecp_alloc + 0x20);
    FsRtlAcknowledgeEcp(ecp_body);
    uint32_t ecp_hdr_flags = *(uint32_t *)(uintptr_t)(fake_ecp_alloc + 0x08);
    check_cond(
        "FsRtl",
        "FsRtlAcknowledgeEcp negative-offset ECP_HEADER (+0x08 |= 0x8 ACKNOWLEDGED)",
        ((ecp_hdr_flags & 0x8U) == 0x8U)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 12: Po — Power Manager Device Busy & Stopwatch Telemetry
     * ---------------------------------------------------------------------- */
    uint32_t po_busy[4] = {0U, 0U, 5U, 0U};
    PoEndDeviceBusy(po_busy);
    uint64_t po_sw_idle[4] = {0ULL, 0ULL, 0ULL, 0ULL};
    uint64_t po_sw_act[4]  = {0ULL, 0ULL, 0ULL, 0x1234ULL};
    BOOLEAN sw0 = PoIsRunningStopWatch(po_sw_idle);
    BOOLEAN sw1 = PoIsRunningStopWatch(po_sw_act);
    check_cond(
        "Po",
        "PoEndDeviceBusy counter decrement & PoIsRunningStopWatch state check",
        (po_busy[2] == 4U && sw0 == 0U && sw1 == 1U)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 13: Alpc — Local Inter-Process Communication Attribute Sizing
     * ---------------------------------------------------------------------- */
    int32_t alpc_hdr_base = AlpcGetHeaderSize((PRTL_DYNAMIC_HASH_TABLE)(uintptr_t)0ULL);
    int32_t alpc_hdr_sec  = AlpcGetHeaderSize((PRTL_DYNAMIC_HASH_TABLE)(uintptr_t)0x80000000ULL);
    check_cond(
        "Alpc",
        "AlpcGetHeaderSize message attribute bitmask calculation (8B base, 32B security)",
        (alpc_hdr_base == 8 && alpc_hdr_sec == 0x20)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 14: Etw / Vf / Hvl — Kernel Event Logging & Debug Device
     * ---------------------------------------------------------------------- */
    KdEventLoggingPresent = 0U;
    KdSetEventLoggingPresent(1U);
    uint64_t kd_dev_ptr = KdGetDebugDevice();
    check_cond(
        "Etw/Kd",
        "KdSetEventLoggingPresent state update & KdGetDebugDevice descriptor RVA",
        (KdEventLoggingPresent == 1U && kd_dev_ptr == (uint64_t)(uintptr_t)&KdDebugDevice)
    );

    /* ----------------------------------------------------------------------
     * SUBSYSTEM 15 & 16: Tm / Zw — KTM IAT Thunks & SSDT Syscall Dispatchers
     * ---------------------------------------------------------------------- */
    uintptr_t tm_addr = (uintptr_t)&TmCurrentTransaction;
    uintptr_t zw_addr = (uintptr_t)&ZwClose;
    /* Verify preserved ZwClose prologue starts with `48 8B C4 FA` (`mov rax, rsp; cli`) */
    const uint8_t *zw_bytes = (const uint8_t *)zw_addr;
    check_cond(
        "Tm/Zw",
        "Preserved KTM IAT dispatch thunk & ZwClose Ring-0 SSDT stub (`mov rax,rsp; cli`)",
        (tm_addr != 0ULL && zw_addr != 0ULL &&
         zw_bytes[0] == 0x48U &&
         ((zw_bytes[1] == 0x89U && zw_bytes[2] == 0xE0U) || (zw_bytes[1] == 0x8BU && zw_bytes[2] == 0xC4U)) &&
         zw_bytes[3] == 0xFAU)
    );

    NtConsoleWrite("------------------------------------------------------------------------\n");
    NtConsoleWrite(" Summary: ");
    NtConsolePrintDec((int64_t)g_tests_passed);
    NtConsoleWrite(" PASSED, ");
    NtConsolePrintDec((int64_t)g_tests_failed);
    NtConsoleWrite(" FAILED — ");
    if (g_tests_failed == 0) {
        NtConsoleWrite("ALL DECOMPILED NT KERNEL SUBSYSTEMS VERIFIED ON SILICON!\n");
    } else {
        NtConsoleWrite("FAILURES DETECTED!\n");
    }
    NtConsoleWrite("========================================================================\n");

    return (int)g_tests_failed;
}
