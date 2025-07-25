/*****************************************************************************/
/*                                                                           */
/*                                 target.c                                  */
/*                                                                           */
/*                           Target specification                            */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2000-2015, Ullrich von Bassewitz                                      */
/*                Roemerstrasse 52                                           */
/*                D-70794 Filderstadt                                        */
/* EMail:         uz@cc65.org                                                */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/



#include <stdlib.h>
#include <string.h>

/* common */
#include "attrib.h"
#include "chartype.h"
#include "check.h"
#include "target.h"



/*****************************************************************************/
/*                                   Data                                    */
/*****************************************************************************/



/* Translation table with direct (no) translation */
static const unsigned char CTNone[256] = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
};

/* Translation table ISO-8859-1 -> AtASCII */
static const unsigned char CTAtari[256] = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0xFD,0x08,0x7F,0x9B,0x0B,0x7D,0x0D,0x0E,0x0F,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
};

/* Translation table ISO-8859-1 -> OSASCII */
static const unsigned char CTOSI[256] = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7D,0x7C,0x7F,0x7E,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
};

/* Translation table ISO-8859-1 -> PetSCII */
static const unsigned char CTPET[256] = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x14,0x09,0x0D,0x11,0x93,0x0A,0x0E,0x0F,
    0x10,0x0B,0x12,0x13,0x08,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x40,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0x5B,0xBF,0x5D,0x5E,0xA4,
    0xAD,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0xB3,0xDD,0xAB,0xB1,0xDF,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
    0x90,0x91,0x92,0x0C,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
};

/* Translation table KOI8-R -> Agat-9 */
static unsigned char CTAgat[256] = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0xA0,
    0x1B,0x5C,0x10,0x12,0x1D,0x1F,0x13,0x1C,0x11,0x1E,0x14,0xA0,0x02,0x5F,0xA0,0xA0,
    0xA0,0xA0,0xA0,0xA0,0xA0,0x9E,0x04,0xA0,0x3C,0x3E,0xA0,0xA0,0x30,0x32,0xA0,0x2F,
    0xA0,0xA0,0xA0,0x0F,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,
    0xA0,0xA0,0xA0,0x9F,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,0xA0,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
};

/* One entry in the target map */
typedef struct TargetEntry TargetEntry;
struct TargetEntry {
    char        Name[13];               /* Target name */
    target_t    Id;                     /* Target ID */
};

/* Table that maps target names to IDs.
** Allows multiple entries for one target ID (target name aliases).
** CAUTION: must be alphabetically for bsearch().
*/
static const TargetEntry TargetMap[] = {
/* BEGIN SORTED.SH */
    {   "agat",         TGT_AGAT         },
    {   "apple2",       TGT_APPLE2       },
    {   "apple2enh",    TGT_APPLE2ENH    },
    {   "atari",        TGT_ATARI        },
    {   "atari2600",    TGT_ATARI2600    },
    {   "atari5200",    TGT_ATARI5200    },
    {   "atari7800",    TGT_ATARI7800    },
    {   "atarixl",      TGT_ATARIXL      },
    {   "atmos",        TGT_ATMOS        },
    {   "bbc",          TGT_BBC          },
    {   "c128",         TGT_C128         },
    {   "c16",          TGT_C16          },
    {   "c64",          TGT_C64          },
    {   "c65",          TGT_C65          },
    {   "cbm510",       TGT_CBM510       },
    {   "cbm610",       TGT_CBM610       },
    {   "creativision", TGT_CREATIVISION },
    {   "cx16",         TGT_CX16         },
    {   "gamate",       TGT_GAMATE       },
    {   "geos",         TGT_GEOS_CBM     },
    {   "geos-apple",   TGT_GEOS_APPLE   },
    {   "geos-cbm",     TGT_GEOS_CBM     },
    {   "kim1",         TGT_KIM1         },
    {   "lunix",        TGT_LUNIX        },
    {   "lynx",         TGT_LYNX         },
    {   "mega65",       TGT_MEGA65,      },
    {   "module",       TGT_MODULE       },
    {   "nes",          TGT_NES          },
    {   "none",         TGT_NONE         },
    {   "osic1p",       TGT_OSIC1P       },
    {   "pce",          TGT_PCENGINE     },
    {   "pet",          TGT_PET          },
    {   "plus4",        TGT_PLUS4        },
    {   "rp6502",       TGT_RP6502       },
    {   "sim6502",      TGT_SIM6502      },
    {   "sim65c02",     TGT_SIM65C02     },
    {   "supervision",  TGT_SUPERVISION  },
    {   "sym1",         TGT_SYM1         },
    {   "telestrat",    TGT_TELESTRAT    },
    {   "vic20",        TGT_VIC20        },
/* END SORTED.SH */
};
#define MAP_ENTRY_COUNT         (sizeof (TargetMap) / sizeof (TargetMap[0]))


/* Table with target properties by target ID */
static const TargetProperties PropertyTable[TGT_COUNT] = {
    { "none",           CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "module",         CPU_6502,       BINFMT_O65,         CTNone  },
    { "atari",          CPU_6502,       BINFMT_BINARY,      CTAtari },
    { "atari2600",      CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "atari5200",      CPU_6502,       BINFMT_BINARY,      CTAtari },
    { "atari7800",      CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "atarixl",        CPU_6502,       BINFMT_BINARY,      CTAtari },
    { "vic20",          CPU_6502,       BINFMT_BINARY,      CTPET   },
    { "c16",            CPU_6502,       BINFMT_BINARY,      CTPET   },
    { "c64",            CPU_6502,       BINFMT_BINARY,      CTPET   },
    { "c128",           CPU_6502,       BINFMT_BINARY,      CTPET   },
    { "plus4",          CPU_6502,       BINFMT_BINARY,      CTPET   },
    { "cbm510",         CPU_6502,       BINFMT_BINARY,      CTPET   },
    { "cbm610",         CPU_6502,       BINFMT_BINARY,      CTPET   },
    { "osic1p",         CPU_6502,       BINFMT_BINARY,      CTOSI   },
    { "pet",            CPU_6502,       BINFMT_BINARY,      CTPET   },
    { "bbc",            CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "apple2",         CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "apple2enh",      CPU_65C02,      BINFMT_BINARY,      CTNone  },
    { "geos-cbm",       CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "creativision",   CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "geos-apple",     CPU_65C02,      BINFMT_BINARY,      CTNone  },
    { "lunix",          CPU_6502,       BINFMT_O65,         CTNone  },
    { "atmos",          CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "telestrat",      CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "nes",            CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "supervision",    CPU_65SC02,     BINFMT_BINARY,      CTNone  },
    { "lynx",           CPU_65SC02,     BINFMT_BINARY,      CTNone  },
    { "sim6502",        CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "sim65c02",       CPU_65C02,      BINFMT_BINARY,      CTNone  },
    { "pce",            CPU_HUC6280,    BINFMT_BINARY,      CTNone  },
    { "gamate",         CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "c65",            CPU_4510,       BINFMT_BINARY,      CTPET   },
    { "cx16",           CPU_W65C02,     BINFMT_BINARY,      CTPET   },
    { "sym1",           CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "mega65",         CPU_45GS02,     BINFMT_BINARY,      CTPET   },
    { "kim1",           CPU_6502,       BINFMT_BINARY,      CTNone  },
    { "rp6502",         CPU_W65C02,     BINFMT_BINARY,      CTNone  },
    { "agat",           CPU_6502,       BINFMT_BINARY,      CTAgat  },
};

/* Target system */
target_t Target = TGT_NONE;



/*****************************************************************************/
/*                                   Code                                    */
/*****************************************************************************/



static int Compare (const void* Key, const void* Entry)
/* Compare function for bsearch */
{
    return strcmp ((const char*) Key, ((const TargetEntry*)Entry)->Name);
}



target_t FindTarget (const char* Name)
/* Find a target by name and return the target ID. TGT_UNKNOWN is returned if
** the given name is no valid target.
*/
{
    /* Search for the name in the map */
    const TargetEntry* T;
    T = bsearch (Name, TargetMap, MAP_ENTRY_COUNT, sizeof (TargetMap[0]), Compare);

    /* Return the target ID */
    return (T == 0)? TGT_UNKNOWN : T->Id;
}



const TargetProperties* GetTargetProperties (target_t Target)
/* Return the properties for a target */
{
    /* Must have a valid target ID */
    PRECONDITION (Target >= 0 && Target < TGT_COUNT);

    /* Return the array entry */
    return &PropertyTable[Target];
}



const char* GetTargetName (target_t Target)
/* Return the name of a target */
{
    /* Return the array entry */
    return GetTargetProperties (Target)->Name;
}



int TargetHasCap (capability_t Cap attribute((unused)))
/* Check if the current target has the given capability */
{
    /* Currently unused */
    return 0;
}
