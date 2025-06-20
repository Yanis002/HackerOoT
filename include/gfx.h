#ifndef GFX_H
#define GFX_H

#include "ultra64.h"
#include "ultra64/gbi.h"
#include "alignment.h"
#include "sched.h"
#include "thga.h"
#include "config.h"
#include "versions.h"

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

// Texture memory size, 4 KiB
#define TMEM_SIZE 0x1000

typedef struct GfxPool {
    /* 0x00000 */ u16 headMagic; // GFXPOOL_HEAD_MAGIC
    /* 0x00008 */ Gfx polyOpaBuffer[POLY_OPA_BUFFER_SIZE];
    /* 0x0BF08 */ Gfx polyXluBuffer[POLY_XLU_BUFFER_SIZE];
    /* 0x0FF08 */ Gfx overlayBuffer[OVERLAY_BUFFER_SIZE];
    /* 0x11F08 */ Gfx workBuffer[WORK_BUFFER_SIZE];
    /* 0x11308 */ Gfx debugBuffer[DEBUG_BUFFER_SIZE];
    /* 0x12408 */ u16 tailMagic; // GFXPOOL_TAIL_MAGIC
} GfxPool; // size = 0x12410

typedef struct GraphicsContext {
    /* 0x0000 */ Gfx* polyOpaBuffer; // Pointer to "Zelda 0"
    /* 0x0004 */ Gfx* polyXluBuffer; // Pointer to "Zelda 1"
    /* 0x0008 */ char unk_008[0x08]; // Unused, could this be pointers to "Zelda 2" / "Zelda 3"
    /* 0x0010 */ Gfx* overlayBuffer; // Pointer to "Zelda 4"
    /* 0x0014 */ u32 unk_014;
    /* 0x0018 */ char unk_018[0x20];
    /* 0x0038 */ OSMesg msgBuff[0x08];
    /* 0x0058 */ OSMesgQueue* schedMsgQueue;
    /* 0x005C */ OSMesgQueue queue;
    /* 0x0078 */ OSScTask task;
    /* 0x00E0 */ char unk_0E0[0xD0];
    /* 0x01B0 */ Gfx* workBuffer;
    /* 0x01B4 */ TwoHeadGfxArena work;
    /* 0x01C4 */ char unk_01C4[0xC0];
    /* 0x0284 */ OSViMode* viMode;
    /* 0x0288 */ char unk_0288[0x20]; // Unused, could this be Zelda 2/3 ?
    /* 0x02A8 */ TwoHeadGfxArena overlay; // "Zelda 4"
    /* 0x02B8 */ TwoHeadGfxArena polyOpa; // "Zelda 0"
    /* 0x02C8 */ TwoHeadGfxArena polyXlu; // "Zelda 1"
    /* 0x02D8 */ u32 gfxPoolIdx;
    /* 0x02DC */ u16* curFrameBuffer;
    /* 0x02E0 */ char unk_2E0[0x04];
    /* 0x02E4 */ u32 viFeatures;
    /* 0x02E8 */ s32 fbIdx;
    /* 0x02EC */ void (*callback)(struct GraphicsContext*, void*);
    /* 0x02F0 */ void* callbackParam;
#if OOT_VERSION >= PAL_1_0
    /* 0x02F4 */ f32 xScale;
    /* 0x02F8 */ f32 yScale;
#endif
#if DEBUG_FEATURES
    /* 0x02FC */ Gfx* debugBuffer;
    TwoHeadGfxArena debug;
#endif
} GraphicsContext; // size = 0x300

extern Gfx gEmptyDL[];

Gfx* Gfx_SetFog(Gfx* gfx, s32 r, s32 g, s32 b, s32 a, s32 near, s32 far);
Gfx* Gfx_SetFogWithSync(Gfx* gfx, s32 r, s32 g, s32 b, s32 a, s32 near, s32 far);
Gfx* Gfx_SetFog2(Gfx* gfx, s32 r, s32 g, s32 b, s32 a, s32 near, s32 far);

Gfx* Gfx_BranchTexScroll(Gfx** gfxP, u32 x, u32 y, s32 width, s32 height);
Gfx* func_80094E78(GraphicsContext* gfxCtx, u32 x, u32 y);
Gfx* Gfx_TexScroll(GraphicsContext* gfxCtx, u32 x, u32 y, s32 width, s32 height);
Gfx* Gfx_TwoTexScroll(GraphicsContext* gfxCtx, s32 tile1, u32 x1, u32 y1, s32 width1, s32 height1, s32 tile2, u32 x2,
                      u32 y2, s32 width2, s32 height2);
Gfx* Gfx_TwoTexScrollEnvColor(GraphicsContext* gfxCtx, s32 tile1, u32 x1, u32 y1, s32 width1, s32 height1, s32 tile2,
                              u32 x2, u32 y2, s32 width2, s32 height2, s32 r, s32 g, s32 b, s32 a);
Gfx* Gfx_EnvColor(GraphicsContext* gfxCtx, s32 r, s32 g, s32 b, s32 a);
void Gfx_SetupFrame(GraphicsContext* gfxCtx, s32 clearFB, u8 r, u8 g, u8 b);
void Gfx_ClearZBuffer(GraphicsContext* gfxCtx);
void func_80095974(GraphicsContext* gfxCtx);

void* Graph_Alloc(GraphicsContext* gfxCtx, size_t size);
void* Graph_Alloc2(GraphicsContext* gfxCtx, size_t size);

#define WORK_DISP       __gfxCtx->work.p
#define POLY_OPA_DISP   __gfxCtx->polyOpa.p
#define POLY_XLU_DISP   __gfxCtx->polyXlu.p
#define OVERLAY_DISP    __gfxCtx->overlay.p
#define DEBUG_DISP      __gfxCtx->debug.p

#if DEBUG_FEATURES

void Graph_OpenDisps(Gfx** dispRefs, GraphicsContext* gfxCtx, const char* file, int line);
void Graph_CloseDisps(Gfx** dispRefs, GraphicsContext* gfxCtx, const char* file, int line);

// __gfxCtx shouldn't be used directly.
// Use the DISP macros defined above when writing to display buffers.
#define OPEN_DISPS(gfxCtx, ...) \
    {                                  \
        GraphicsContext* __gfxCtx;     \
        Gfx* dispRefs[4];              \
        __gfxCtx = gfxCtx;             \
        (void)__gfxCtx;                \
        Graph_OpenDisps(dispRefs, gfxCtx, __FILE__, __LINE__)

#define CLOSE_DISPS(gfxCtx, ...)                     \
        do {                                                \
            Graph_CloseDisps(dispRefs, gfxCtx, __FILE__, __LINE__); \
        } while (0);                                        \
    }                                                       \
    (void)0

#define GRAPH_ALLOC(gfxCtx, size) Graph_Alloc(gfxCtx, size)

#else

#define OPEN_DISPS(gfxCtx, ...)      \
    {                                       \
        GraphicsContext* __gfxCtx = gfxCtx; \
        s32 __dispPad

#define CLOSE_DISPS(gfxCtx, ...) \
        do {} while (0);                \
    }                                   \
    (void)0

#define GRAPH_ALLOC(gfxCtx, size) ((void*)((gfxCtx)->polyOpa.d = (Gfx*)((u8*)(gfxCtx)->polyOpa.d - ALIGN16(size))))

#endif

void Graph_ThreadEntry(void*);

extern u64 gMojiFontTLUTs[4][4]; // original name: "moji_tlut"
extern u64 gMojiFontTex[]; // original name: "font_ff"

/**
 * `x` vertex x
 * `y` vertex y
 * `z` vertex z
 * `s` texture s coordinate
 * `t` texture t coordinate
 * `crnx` red component of color vertex, or x component of normal vertex
 * `cgny` green component of color vertex, or y component of normal vertex
 * `cbnz` blue component of color vertex, or z component of normal vertex
 * `a` alpha
 */
#define VTX(x,y,z,s,t,crnx,cgny,cbnz,a) { { { x, y, z }, 0, { s, t }, { crnx, cgny, cbnz, a } } }

#define VTX_T(x,y,z,s,t,cr,cg,cb,a) { { x, y, z }, 0, { s, t }, { cr, cg, cb, a } }

#define gDPSetTileCustom(pkt, fmt, siz, uls, ult, lrs, lrt, pal,        \
                         cms, cmt, masks, maskt, shifts, shiftt)        \
_DW({                                                                   \
    gDPPipeSync(pkt);                                                   \
    gDPTileSync(pkt);                                                   \
    gDPSetTile(pkt, fmt, siz,                                           \
        (((((lrs) - (uls) + 1) * siz##_TILE_BYTES) + 7) >> 3), 0,       \
        G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, masks,               \
        shifts);                                                        \
    gDPTileSync(pkt);                                                   \
    gDPSetTile(pkt, fmt, siz,                                           \
        (((((lrs) - (uls) + 1) * siz##_LINE_BYTES) + 7) >> 3), 0,       \
        G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts);  \
    gDPSetTileSize(pkt, G_TX_RENDERTILE,                                \
        (uls) << G_TEXTURE_IMAGE_FRAC,                                  \
        (ult) << G_TEXTURE_IMAGE_FRAC,                                  \
        (lrs) << G_TEXTURE_IMAGE_FRAC,                                  \
        (lrt) << G_TEXTURE_IMAGE_FRAC);                                 \
})

// HackerOoT

// System for inserting SPDontSkipTexLoadsAcross for actors/effects which
// manipulate segments to select texture indices. Note that this only needs to
// be done for things which have a single material and which can appear multiple
// times consecutively in the scene, such as Rupees and effects.
#if ENABLE_F3DEX3

// It might seem that we'd need to ensure this is reset every frame. But we
// actually only care about when this changes within a frame, as the texture
// loads would only ever be skipped between two or more rupees drawn
// consecutively. This is s32 so it can hold an actual texture pointer in case
// an "index" is not available.
#define IF_F3DEX3_DONT_SKIP_TEX_INIT() \
    static s32 _lastTexIndex = -1; \
    (void)0

// If we have consecutive items rendering with different textures, F3DEX3's
// optimizer will incorrectly believe the texture loads can be skipped, so this
// command tells it not to skip them. However, if the texture really is the same
// as last time, then we can let the optimizer skip the load.
#define IF_F3DEX3_DONT_SKIP_TEX_HERE(pkt, texIndex) \
    if ((s32)(texIndex) != _lastTexIndex) { \
        gSPDontSkipTexLoadsAcross(pkt); \
        _lastTexIndex = (s32)(texIndex); \
    } \
    (void)0

// In some cases we are sure things are rendered consecutively with different
// textures, e.g. in Fire Temple fire objects.
#define IF_F3DEX3_ALWAYS_DONT_SKIP_TEX(pkt) \
    gSPDontSkipTexLoadsAcross(pkt)

#else
#define IF_F3DEX3_DONT_SKIP_TEX_INIT() (void)0
#define IF_F3DEX3_DONT_SKIP_TEX_HERE(pkt, texIndex) (void)0
#define IF_F3DEX3_ALWAYS_DONT_SKIP_TEX(pkt) (void)0
#endif

#endif
