#include "gfx.h"
#include "debug.h"
#include "gfx_setupdl.h"
#include "controller.h"
#include "printf.h"
#include "array_count.h"
#include "scene.h"
#include "assert.h"
#include "ultra64/convert.h"
#include "romfile.h"
#include "segmented_address.h"
#include "sys_matrix.h"
#include "widescreen.h"
#include "libc64/malloc.h"

static GameState* sGameState = NULL;
static View* sView = NULL;
static OSMesgQueue queue;
static OSMesg msg;
static DmaRequest req;

enum {
    EDIT_MODE_POS,
    EDIT_MODE_ROT,
    EDIT_MODE_SCALE,
};

void Preview_Debug(PreviewState* this) {
    void* vec = NULL;

    if (!this->overrideInput) {
        return;
    }

    // if (CHECK_BTN_ALL(this->state->input[0].press.button, BTN_Z)) {
    //     this->curRoom.num++;
    //     PRINTF("new room requested (%d)\n", this->curRoom.num);
    // }

    // if (CHECK_BTN_ALL(this->state->input[0].press.button, BTN_R)) {
    //     this->curRoom.num--;
    //     PRINTF("new room requested (%d)\n", this->curRoom.num);
    // }

    // if (this->curRoom.num >= this->roomList.count) {
    //     this->curRoom.num = 0;
    // }

    // if (this->curRoom.num < 0) {
    //     this->curRoom.num = 0;
    //     // this->curRoom.num = this->roomList.count - 1;
    // }

    // if (CHECK_BTN_ALL(this->state->input[0].press.button, BTN_Z) || CHECK_BTN_ALL(this->state->input[0].press.button,
    // BTN_R)) {
    //     PRINTF("new room loading (%d)\n", this->curRoom.num);
    //     Preview_FreeRoom(this);
    //     Preview_InitRoom(this);
    // }

    if (CHECK_BTN_ALL(this->state->input[0].press.button, BTN_A)) {
        this->editMode++;
        PRINTF("new edit mode (a): %d\n", this->editMode);
    }

    if (CHECK_BTN_ALL(this->state->input[0].press.button, BTN_B)) {
        this->editMode--;
        PRINTF("new edit mode (b): %d\n", this->editMode);
    }

    switch (this->editMode) {
        case EDIT_MODE_POS:
            vec = &this->matrixPos;
            break;
        case EDIT_MODE_ROT:
            vec = &this->matrixRot;
            break;
        case EDIT_MODE_SCALE:
            vec = &this->matrixScale;
            break;
        default:
            vec = NULL;
            break;
    }

    if (vec == NULL) {
        return;
    }

    if (CHECK_BTN_ALL(this->state->input[0].cur.button, BTN_DLEFT)) {
        if (this->editMode == EDIT_MODE_ROT) {
            ((Vec3s*)vec)->x -= 100;
        } else {
            Vec3f* vecF = ((Vec3f*)vec);

            if (this->editMode == EDIT_MODE_POS) {
                vecF->x--;
            } else {
                vecF->x -= 0.01f;
            }
        }
    }
    if (CHECK_BTN_ALL(this->state->input[0].cur.button, BTN_DRIGHT)) {
        if (this->editMode == EDIT_MODE_ROT) {
            ((Vec3s*)vec)->x += 100;
        } else {
            Vec3f* vecF = ((Vec3f*)vec);

            if (this->editMode == EDIT_MODE_POS) {
                vecF->x++;
            } else {
                vecF->x += 0.01f;
            }
        }
    }

    if (CHECK_BTN_ALL(this->state->input[0].cur.button, BTN_DDOWN)) {
        if (this->editMode == EDIT_MODE_ROT) {
            ((Vec3s*)vec)->y -= 100;
        } else {
            Vec3f* vecF = ((Vec3f*)vec);

            if (this->editMode == EDIT_MODE_POS) {
                vecF->y--;
            } else {
                vecF->y -= 0.01f;
            }
        }
    }
    if (CHECK_BTN_ALL(this->state->input[0].cur.button, BTN_DUP)) {
        if (this->editMode == EDIT_MODE_ROT) {
            ((Vec3s*)vec)->y += 100;
        } else {
            Vec3f* vecF = ((Vec3f*)vec);

            if (this->editMode == EDIT_MODE_POS) {
                vecF->y++;
            } else {
                vecF->y += 0.01f;
            }
        }
    }

    if (CHECK_BTN_ALL(this->state->input[0].cur.button, BTN_CLEFT)) {
        if (this->editMode == EDIT_MODE_ROT) {
            ((Vec3s*)vec)->z -= 100;
        } else {
            Vec3f* vecF = ((Vec3f*)vec);

            if (this->editMode == EDIT_MODE_POS) {
                vecF->z--;
            } else {
                vecF->z -= 0.01f;
            }
        }
    }
    if (CHECK_BTN_ALL(this->state->input[0].cur.button, BTN_CRIGHT)) {
        if (this->editMode == EDIT_MODE_ROT) {
            ((Vec3s*)vec)->z += 100;
        } else {
            Vec3f* vecF = ((Vec3f*)vec);

            if (this->editMode == EDIT_MODE_POS) {
                vecF->z++;
            } else {
                vecF->z += 0.01f;
            }
        }
    }
}

static inline void Preview_BusyLoop(void) {
    while (osRecvMesg(&queue, NULL, OS_MESG_NOBLOCK) != 0) {
        PRINTF("dma is busy\n");
    }
}

void* Preview_LoadFile(GameState* gameState, RomFile* file) {
    u32 size = file->vromEnd - file->vromStart;
    void* allocp = SYSTEM_ARENA_MALLOC(size);

    osCreateMesgQueue(&queue, &msg, 1);
    DMA_REQUEST_ASYNC(&req, allocp, file->vromStart, size, 0, &queue, &msg, __FILE__, __LINE__);
    Preview_BusyLoop();
    return allocp;
}

void Preview_SetSceneId(PreviewState* this, u8 sceneId) {
    this->sceneId = sceneId;
}

void Preview_SetTimer(PreviewState* this, u8 dmaTimer) {
    this->dmaTimer = dmaTimer;
}

void Preview_SetGameState(PreviewState* this, GameState* state) {
    sGameState = state;
}

void Preview_SetView(PreviewState* this, View* view) {
    sView = view;
}

void Preview_FreeScene(PreviewState* this) {
    if (this->sceneSegment != NULL) {
        SYSTEM_ARENA_FREE(this->sceneSegment);
        this->sceneSegment = NULL;
    }
}

void Preview_FreeRoom(PreviewState* this) {
    if (this->roomSegment != NULL) {
        SYSTEM_ARENA_FREE(this->roomSegment);
        this->roomSegment = NULL;
    }
}

void Preview_Reset(PreviewState* this) {
    u8 overrideInput = this->overrideInput;

    Preview_FreeRoom(this);
    Preview_FreeScene(this);

    PRINTF("sGameState: 0x%08X\n", sGameState);
    ASSERT(sGameState != NULL, "[HackerOoT:Error]: GameState unset!", __FILE__, __LINE__);

    memset(this, 0, sizeof(PreviewState));
    this->state = sGameState;
    this->view = sView;
    this->overrideInput = overrideInput;
    this->sceneSegment = NULL;
    this->roomSegment = NULL;
    this->dmaTimer = 0;
    this->doLoad = true;
    this->curRoom.num = 0;

    this->worldAt.x = 0;
    this->worldAt.y = 0.0f;
    this->worldAt.z = 0;

    this->worldEye.x = 100.0f;
    this->worldEye.y = 5.0f;
    this->worldEye.z = -3.0f;

    this->matrixPos.x = -210.0f;
    this->matrixPos.y = 15.0f;
    this->matrixPos.z = 45.0f;

    this->matrixRot.x = 0;
    this->matrixRot.y = -8400;
    this->matrixRot.z = -6800;

    this->matrixScale.x = 0.35f;
    this->matrixScale.y = 0.35f;
    this->matrixScale.z = 0.35f;
}

void Preview_InitScene(PreviewState* this) {
    SceneCmd* sceneCmd;
    u32 cmdCode;

    ASSERT(this->sceneSegment != NULL, "[HackerOoT:Error]: the scene segment is NULL", __FILE__, __LINE__);
    gSegments[2] = OS_K0_TO_PHYSICAL(this->sceneSegment);

    // init scene (the play argument is unused)
    LightContext_Init(NULL, &this->lightCtx);
    this->roomList.romFiles = NULL;

    // execute scene commands
    sceneCmd = this->sceneSegment;
    cmdCode = sceneCmd->base.code;

    while (cmdCode != SCENE_CMD_ID_END) {
        switch (cmdCode) {
            case SCENE_CMD_ID_ROOM_LIST:
                this->roomList.count = sceneCmd->roomList.length;
                this->roomList.romFiles = SEGMENTED_TO_VIRTUAL(sceneCmd->roomList.data);
                break;
            default:
                break;
        }

        sceneCmd++;
        cmdCode = sceneCmd->base.code;
    }
}

void Preview_InitRoom(PreviewState* this) {
    SceneCmd* sceneCmd;
    u32 cmdCode;

    // init room
    ASSERT(this->roomList.romFiles != NULL, "[HackerOoT:Error]: room files not found", __FILE__, __LINE__);
    this->curRoom.num = 0;
    this->curRoom.segment = NULL;
#if ENABLE_F3DEX3
    this->curRoom.occPlaneCount = 0;
#endif

    this->curRoom.segment = this->roomSegment;
    ASSERT(this->roomSegment != NULL, "[HackerOoT:Error]: the room segment is NULL", __FILE__, __LINE__);
    gSegments[3] = OS_K0_TO_PHYSICAL(this->roomSegment);

    // execute room commands
    sceneCmd = this->roomSegment;
    cmdCode = sceneCmd->base.code;

    while (cmdCode != SCENE_CMD_ID_END) {
        switch (cmdCode) {
            case SCENE_CMD_ID_ROOM_SHAPE:
                this->curRoom.roomShape = SEGMENTED_TO_VIRTUAL(sceneCmd->mesh.data);
                break;
            default:
                break;
        }

        sceneCmd++;
        cmdCode = sceneCmd->base.code;
    }
}

void Preview_LoadScene(PreviewState* this) {
    SceneTableEntry* pEntry = &gSceneTable[this->sceneId];
    SceneCmd* sceneCmd;
    u32 cmdCode;

    // first, reset the state so we start clean (this will free the previous allocations)
    Preview_Reset(this);

    // next, initialize the scene so we know where are the rooms
    this->sceneSegment = Preview_LoadFile(this->state, &pEntry->sceneFile);
    Preview_InitScene(this);

    // then, initialize the room (we're always picking room 0, might change in the future)
    this->roomSegment = Preview_LoadFile(this->state, &this->roomList.romFiles[this->curRoom.num]);
    Preview_InitRoom(this);

    PRINTF("load state: sceneSegment=0x%08X, roomSegment=0x%08X, roomShape=0x%08X\n", this->sceneSegment,
           this->roomSegment, this->curRoom.roomShape);
    PRINTF("Loaded scene!\n");
}

void Preview_Init(PreviewState* this) {
    memset(this, 0, sizeof(PreviewState));
    Preview_Reset(this);
}

void Preview_Update(PreviewState* this) {
    // PRINTF("state: dmatimer=%d, doload: %d\n", this->dmaTimer, this->doLoad);
    // PRINTF("x=%.2f, y=%.2f, z=%.2f, x=%d, y=%d, z=%d, x=%.2f, y=%.2f, z=%.2f\n", this->matrixPos.x,
    // this->matrixPos.y, this->matrixPos.z, this->matrixRot.x, this->matrixRot.y, this->matrixRot.z,
    // this->matrixScale.x, this->matrixScale.y, this->matrixScale.z);

    if (this->dmaTimer == 1) {
        this->doLoad = true;
    }

    if (this->dmaTimer > 0) {
        this->dmaTimer--;
        return;
    }

    if (this->doLoad) {
        Preview_LoadScene(this);
        this->doLoad = false;
    }

    if (this->sceneSegment != NULL) {
        gSegments[2] = OS_K0_TO_PHYSICAL(this->sceneSegment);
    }

    if (this->roomSegment != NULL) {
        gSegments[3] = OS_K0_TO_PHYSICAL(this->roomSegment);
    }
}

void Preview_DrawRoomNormal(PreviewState* this) {
    GraphicsContext* gfxCtx = this->state->gfxCtx;
    RoomShapeNormal* roomShape;
    RoomShapeDListsEntry* entry;
    s32 i;
    u32 flags = ROOM_DRAW_OPA /* | ROOM_DRAW_XLU */;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    if (flags & ROOM_DRAW_OPA) {
        gSPSegment(POLY_OPA_DISP++, 0x03, this->roomSegment);
        Gfx_SetupDL_25Opa(gfxCtx);
        Matrix_Translate(this->matrixPos.x, this->matrixPos.y, this->matrixPos.z, MTXMODE_NEW);
        Matrix_Scale(this->matrixScale.x, this->matrixScale.y, this->matrixScale.z, MTXMODE_APPLY);
        Matrix_RotateZYX(this->matrixRot.x, this->matrixRot.y, this->matrixRot.z, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, MATRIX_FINALIZE(gfxCtx, __FILE__, __LINE__),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }

    if (flags & ROOM_DRAW_XLU) {
        gSPSegment(POLY_XLU_DISP++, 0x03, this->roomSegment);
        Gfx_SetupDL_25Xlu(gfxCtx);
        Matrix_Translate(this->matrixPos.x, this->matrixPos.y, this->matrixPos.z, MTXMODE_NEW);
        Matrix_Scale(this->matrixScale.x, this->matrixScale.y, this->matrixScale.z, MTXMODE_APPLY);
        Matrix_RotateZYX(this->matrixRot.x, this->matrixRot.y, this->matrixRot.z, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, MATRIX_FINALIZE(gfxCtx, __FILE__, __LINE__),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }

    roomShape = &this->curRoom.roomShape->normal;
    entry = SEGMENTED_TO_VIRTUAL(roomShape->entries);
    for (i = 0; i < roomShape->numEntries; i++) {
        if ((flags & ROOM_DRAW_OPA) && entry->opa != NULL) {
            gSPDisplayList(POLY_OPA_DISP++, entry->opa);
        }

        if ((flags & ROOM_DRAW_XLU) && entry->xlu != NULL) {
            gSPDisplayList(POLY_XLU_DISP++, entry->xlu);
        }

        entry++;
    }

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);
}

void Preview_Draw(PreviewState* this) {
    GraphicsContext* gfxCtx = this->state->gfxCtx;

    ASSERT(this->view != NULL, "[HackerOoT:Error]: View unset!", __FILE__, __LINE__);

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    gSPSegment(POLY_XLU_DISP++, 0x00, NULL);
    gSPSegment(OVERLAY_DISP++, 0x00, NULL);

    Gfx_SetupFrame(gfxCtx, true, 0, 0, 0);
    Gfx_ClearZBuffer(gfxCtx);

    View_LookAt(this->view, &this->worldEye, &this->worldAt, &this->view->up);
    View_SetPerspective(this->view, this->view->fovy, this->view->zNear, this->lightCtx.zFar);
    View_Apply(this->view, VIEW_VIEWING | VIEW_VIEWPORT | VIEW_PROJECTION_PERSPECTIVE);

    Lights* sp228 = LightContext_NewLights(&this->lightCtx, gfxCtx);
    Lights_BindAll(sp228, this->lightCtx.listHead, NULL);
    Lights_Draw(sp228, gfxCtx);

    if (this->dmaTimer == 0 && !this->doLoad && this->sceneSegment != NULL) {
        gSegments[2] = OS_K0_TO_PHYSICAL(this->sceneSegment);
        gSPSegment(POLY_OPA_DISP++, 0x02, this->sceneSegment);
        gSPSegment(POLY_XLU_DISP++, 0x02, this->sceneSegment);
        gSPSegment(OVERLAY_DISP++, 0x02, this->sceneSegment);

        if (this->roomSegment != NULL) {
            gSegments[3] = OS_K0_TO_PHYSICAL(this->roomSegment);

            if (this->curRoom.roomShape->base.type == ROOM_SHAPE_TYPE_NORMAL) {
                Preview_DrawRoomNormal(this);
            }
        } else {
            PRINTF("draw aborted because the room isn't ready\n");
        }
    } else {
        PRINTF("draw aborted because a new scene is loading\n");
    }

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);
}

void Preview_Main(PreviewState* this) {
    Preview_Debug(this);
    Preview_Update(this);
    Preview_Draw(this);
}

void Preview_Destroy(PreviewState* this) {
    Preview_FreeRoom(this);
    Preview_FreeScene(this);
    memset(this, 0, sizeof(PreviewState));
    sGameState = NULL;
    sView = NULL;
}
