#include "rom_header.h"
#include "region.h"
#include "versions.h"

/* 0x00 */ ENDIAN_IDENTIFIER
/* 0x01 */ PI_DOMAIN_1_CFG(64, 18, 7, 3)
/* 0x04 */ SYSTEM_CLOCK_RATE_SETTING(0xF)
/* 0x08 */ ENTRYPOINT(0x80000400)
#if PLATFORM_N64
/* 0x0C */ LIBULTRA_VERSION(2, 0, I)
#else
/* 0x0C */ LIBULTRA_VERSION(2, 0, L)
#endif
/* 0x10 */ CHECKSUM()
/* 0x18 */ PADDING(8)
/* 0x20 */ ROM_NAME("THE LEGEND OF ZELDA")
/* 0x34 */ PADDING(7)

#if OOT_NTSC || ((defined CONSOLE_WIIVC) || (defined CONSOLE_GC))
/* 0x3B */ MEDIUM(CARTRIDGE_EXPANDABLE)
#else
/* 0x3B */ MEDIUM(CARTRIDGE)
#endif

/* 0x3C */ GAME_ID("ZL")

#if (defined CONSOLE_WIIVC) || (defined CONSOLE_GC)
/* 0x3E */ REGION(US)
#else
#if OOT_REGION == REGION_NULL
/* 0x3E */ REGION(FREE)
#elif OOT_REGION == REGION_US
/* 0x3E */ REGION(US)
#elif OOT_REGION == REGION_JP
/* 0x3E */ REGION(JP)
#elif OOT_REGION == REGION_EU
/* 0x3E */ REGION(PAL)
#endif
#endif

#if (defined CONSOLE_WIIVC) || (defined CONSOLE_GC)
/* 0x3F */ GAME_REVISION(0)
#else

#if OOT_VERSION == NTSC_1_0 || OOT_VERSION == PAL_1_0
/* 0x3F */ GAME_REVISION(0)
#elif OOT_VERSION == NTSC_1_1 || OOT_VERSION == PAL_1_1
/* 0x3F */ GAME_REVISION(1)
#elif OOT_VERSION == NTSC_1_2
/* 0x3F */ GAME_REVISION(2)
#else
/* 0x3F */ GAME_REVISION(15)
#endif

#endif
