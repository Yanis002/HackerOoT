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
#include "regs.h"

extern s32 Object_SpawnPersistent(ObjectContext* objectCtx, s16 objectId);
extern void Scene_CommandTimeSettingsImpl(EnvironmentContext* envCtx, SceneCmd* cmd);
extern void Scene_CommandObjectListImpl(ObjectContext* objectCtx, ActorContext* actorCtx, SceneCmd* cmd);

static GameState* sGameState = NULL;
static View* sView = NULL;
static OSMesgQueue queue;
static OSMesg msg;
static DmaRequest req;

static Gfx sDefaultDisplayList[] = {
    gsSPSegment(0x08, gEmptyDL),
    gsSPSegment(0x09, gEmptyDL),
    gsSPSegment(0x0A, gEmptyDL),
    gsSPSegment(0x0B, gEmptyDL),
    gsSPSegment(0x0C, gEmptyDL),
    gsSPSegment(0x0D, gEmptyDL),
    gsDPPipeSync(),
    gsDPSetPrimColor(0, 0, 128, 128, 128, 128),
    gsDPSetEnvColor(128, 128, 128, 128),
    gsSPEndDisplayList(),
};

enum {
    EDIT_MODE_MIN,
    EDIT_MODE_POS,
    EDIT_MODE_ROT,
    EDIT_MODE_SCALE,
    EDIT_MODE_EYE,
    EDIT_MODE_AT,
    EDIT_MODE_MAX,
};

void Preview_Debug(PreviewState* this) {
    static const char* modeToStr[] = {
        "EDIT_MODE_MIN", "EDIT_MODE_POS", "EDIT_MODE_ROT", "EDIT_MODE_SCALE",
        "EDIT_MODE_EYE", "EDIT_MODE_AT",  "EDIT_MODE_MAX",
    };

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
        if (this->editMode > EDIT_MODE_MIN && this->editMode < EDIT_MODE_MAX) {
            PRINTF("new edit mode (a): %s (%d)\n", modeToStr[this->editMode], this->editMode);
        }
    }

    if (CHECK_BTN_ALL(this->state->input[0].press.button, BTN_B)) {
        this->editMode--;
        if (this->editMode > EDIT_MODE_MIN && this->editMode < EDIT_MODE_MAX) {
            PRINTF("new edit mode (b): %s (%d)\n", modeToStr[this->editMode], this->editMode);
        }
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
        case EDIT_MODE_EYE:
            vec = &this->worldEye;
            break;
        case EDIT_MODE_AT:
            vec = &this->worldAt;
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

            if (this->editMode != EDIT_MODE_SCALE) {
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

            if (this->editMode != EDIT_MODE_SCALE) {
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

            if (this->editMode != EDIT_MODE_SCALE) {
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

            if (this->editMode != EDIT_MODE_SCALE) {
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

            if (this->editMode != EDIT_MODE_SCALE) {
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

            if (this->editMode != EDIT_MODE_SCALE) {
                vecF->z++;
            } else {
                vecF->z += 0.01f;
            }
        }
    }

    if (this->editMode > EDIT_MODE_MIN && this->editMode < EDIT_MODE_MAX) {
        PRINTF("edit mode (b): %s (%d) - ", modeToStr[this->editMode], this->editMode);
    }

    if (this->editMode == EDIT_MODE_ROT) {
        Vec3s* vecS = ((Vec3s*)vec);
        PRINTF("x=%d, y=%d, z=%d\n", vecS->x, vecS->y, vecS->z);
    } else {
        Vec3f* vecF = ((Vec3f*)vec);
        PRINTF("x=%.2f, y=%.2f, z=%.2f\n", vecF->x, vecF->y, vecF->z);
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

void Preview_FreeObjectAlloc(PreviewState* this) {
    if (this->objectCtx.spaceStart != NULL) {
        SYSTEM_ARENA_FREE(this->objectCtx.spaceStart);
        this->objectCtx.spaceStart = NULL;
    }
}

void Preview_Reset(PreviewState* this) {
    u8 overrideInput = this->overrideInput;

    Preview_FreeObjectAlloc(this);
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
    this->editMode = EDIT_MODE_POS;

    this->worldAt.x = 0.0f;
    this->worldAt.y = 0.0f;
    this->worldAt.z = 0.0f;

    this->worldEye.x = 0.0f;
    this->worldEye.y = 250.0f;
    this->worldEye.z = 0.0f;

    this->matrixPos.x = 0.0f;
    this->matrixPos.y = 0.0f;
    this->matrixPos.z = 0.0f;

    this->matrixRot.x = 0;
    this->matrixRot.y = 0;
    this->matrixRot.z = 0;

    this->matrixScale.x = 0.50f;
    this->matrixScale.y = 0.50f;
    this->matrixScale.z = 0.50f;
}

void Preview_UpdateView(PreviewState* this) {
    Vec3s center;

    center.x = (this->colHeader.minBounds.x + this->colHeader.maxBounds.x) / 2;
    center.y = (this->colHeader.minBounds.y + this->colHeader.maxBounds.y) / 2;
    center.z = (this->colHeader.minBounds.z + this->colHeader.maxBounds.z) / 2;

    // this->matrixPos.x = center.x;
    this->worldEye.y += center.y;
    // this->matrixPos.z = center.z;
}

void Preview_InitScene(PreviewState* this) {
    CollisionHeader* colHeader;
    SceneCmd* cmd;
    u32 cmdCode;

    ASSERT(this->sceneSegment != NULL, "[HackerOoT:Error]: the scene segment is NULL", __FILE__, __LINE__);
    gSegments[2] = OS_K0_TO_PHYSICAL(this->sceneSegment);

    // init scene
    Object_InitContext(this->state, &this->objectCtx, false);
    ASSERT(this->objectCtx.spaceStart != NULL && this->objectCtx.slots[0].segment != NULL,
           "[HackerOoT:Error]: the object segment is NULL", __FILE__, __LINE__);
    LightContext_Init(NULL, &this->lightCtx); // the play argument is unused
    this->roomList.romFiles = NULL;

    // execute scene commands
    cmd = this->sceneSegment;
    cmdCode = cmd->base.code;

    while (cmdCode != SCENE_CMD_ID_END) {
        switch (cmdCode) {
            case SCENE_CMD_ID_COLLISION_HEADER:
                colHeader = SEGMENTED_TO_VIRTUAL(cmd->colHeader.data);
                this->colHeader.minBounds = colHeader->minBounds;
                this->colHeader.maxBounds = colHeader->maxBounds;
                this->colHeader.numVertices = colHeader->numVertices;
                this->colHeader.vtxList = SEGMENTED_TO_VIRTUAL(colHeader->vtxList);
                this->colHeader.numPolygons = colHeader->numPolygons;
                this->colHeader.polyList = SEGMENTED_TO_VIRTUAL(colHeader->polyList);
                this->colHeader.surfaceTypeList = SEGMENTED_TO_VIRTUAL(colHeader->surfaceTypeList);
                this->colHeader.bgCamList = SEGMENTED_TO_VIRTUAL(colHeader->bgCamList);
                this->colHeader.numWaterBoxes = colHeader->numWaterBoxes;
                this->colHeader.waterBoxes = SEGMENTED_TO_VIRTUAL(colHeader->waterBoxes);
                Preview_UpdateView(this);
                PRINTF("colheader: minBounds: %d, %d, %d - maxBounds: %d, %d, %d\n", this->colHeader.minBounds.x,
                       this->colHeader.minBounds.y, this->colHeader.minBounds.z, this->colHeader.maxBounds.x,
                       this->colHeader.maxBounds.y, this->colHeader.maxBounds.z);
                break;
            case SCENE_CMD_ID_ROOM_LIST:
                this->roomList.count = cmd->roomList.length;
                this->roomList.romFiles = SEGMENTED_TO_VIRTUAL(cmd->roomList.data);
                break;
            case SCENE_CMD_ID_SKYBOX_SETTINGS:
                this->skyboxId = cmd->skyboxSettings.skyboxId;
                this->envCtx.skyboxConfig = this->envCtx.changeSkyboxNextConfig = cmd->skyboxSettings.skyboxConfig;
                this->envCtx.lightMode = cmd->skyboxSettings.envLightMode;
                break;
            case SCENE_CMD_ID_LIGHT_SETTINGS_LIST:
                this->envCtx.numLightSettings = cmd->lightSettingList.length;
                this->envCtx.lightSettingsList = SEGMENTED_TO_VIRTUAL(cmd->lightSettingList.data);
                break;
            case SCENE_CMD_ID_TRANSITION_ACTOR_LIST:
                this->transitionActors.count = cmd->transiActorList.length;
                this->transitionActors.list = SEGMENTED_TO_VIRTUAL(cmd->transiActorList.data);
                break;
            case SCENE_CMD_ID_SPECIAL_FILES:
                if (cmd->specialFiles.keepObjectId != OBJECT_INVALID) {
                    this->objectCtx.subKeepSlot =
                        Object_SpawnPersistent(&this->objectCtx, cmd->specialFiles.keepObjectId);
                    gSegments[5] = OS_K0_TO_PHYSICAL(this->objectCtx.slots[this->objectCtx.subKeepSlot].segment);
                }
                break;
            case SCENE_CMD_ID_SPAWN_LIST:
                this->spawnList = SEGMENTED_TO_VIRTUAL(cmd->spawnList.data);
                break;
#if ENABLE_ANIMATED_MATERIALS
            case SCENE_CMD_ID_ANIMATED_MATERIAL_LIST:
                this->sceneMaterialAnims = SEGMENTED_TO_VIRTUAL(cmd->textureAnimations.segment);
                break;
#endif

            default:
                break;
        }

        cmd++;
        cmdCode = cmd->base.code;
    }
}

void Preview_InitRoom(PreviewState* this) {
    SceneCmd* cmd;
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
    cmd = this->roomSegment;
    cmdCode = cmd->base.code;

    while (cmdCode != SCENE_CMD_ID_END) {
        switch (cmdCode) {
            case SCENE_CMD_ID_ROOM_SHAPE:
                this->curRoom.roomShape = SEGMENTED_TO_VIRTUAL(cmd->mesh.data);
                break;
            case SCENE_CMD_ID_ROOM_BEHAVIOR:
                this->curRoom.type = cmd->roomBehavior.gpFlag1;
                this->curRoom.environmentType = cmd->roomBehavior.gpFlag2 & 0xFF;
                this->curRoom.lensMode = (cmd->roomBehavior.gpFlag2 >> 8) & 1;
                break;
            case SCENE_CMD_ID_TIME_SETTINGS:
                Scene_CommandTimeSettingsImpl(&this->envCtx, cmd);
                break;
            case SCENE_CMD_ID_SKYBOX_DISABLES:
                this->envCtx.skyboxDisabled = cmd->skyboxDisables.skyboxDisabled;
                this->envCtx.sunMoonDisabled = cmd->skyboxDisables.sunMoonDisabled;
                break;
            case SCENE_CMD_ID_OBJECT_LIST:
                Scene_CommandObjectListImpl(&this->objectCtx, &this->actorCtx, cmd);
                break;
            case SCENE_CMD_ID_ACTOR_LIST:
                this->numActorEntries = cmd->actorEntryList.length;
                this->actorEntryList = SEGMENTED_TO_VIRTUAL(cmd->actorEntryList.data);
                break;
#if ENABLE_F3DEX3
            case SCENE_CMD_ID_OCC_PLANE_CAND_LIST:
                this->curRoom.occPlaneCount = cmd->occPlaneCandList.count;
                this->curRoom.occPlaneList = SEGMENTED_TO_VIRTUAL(cmd->occPlaneCandList.list);
                break;
#endif
            default:
                break;
        }

        cmd++;
        cmdCode = cmd->base.code;
    }
}

void Preview_LoadScene(PreviewState* this) {
    SceneTableEntry* pEntry = &gSceneTable[this->sceneId];
    SceneCmd* sceneCmd;
    u32 cmdCode;

    // first, reset the state so we start clean (this will free the previous allocations)
    Preview_Reset(this);
    this->drawConfig = pEntry->drawConfig;

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

    gSegments[4] = OS_K0_TO_PHYSICAL(this->objectCtx.slots[this->objectCtx.mainKeepSlot].segment);
    gSegments[5] = OS_K0_TO_PHYSICAL(this->objectCtx.slots[this->objectCtx.subKeepSlot].segment);

    if (this->sceneSegment != NULL) {
        gSegments[2] = OS_K0_TO_PHYSICAL(this->sceneSegment);
    }

    if (this->roomSegment != NULL) {
        gSegments[3] = OS_K0_TO_PHYSICAL(this->roomSegment);
    }

    if (this->framesTimer == 0) {
        this->gameplayFrames++;
        this->framesTimer = R_UPDATE_RATE == 1 ? 3 : R_UPDATE_RATE == 2 ? 2 : R_UPDATE_RATE == 3 ? 0 : R_UPDATE_RATE;
    } else {
        this->framesTimer--;
    }
}

void Preview_DrawRoomNormal(PreviewState* this) {
    GraphicsContext* gfxCtx = this->state->gfxCtx;
    RoomShapeNormal* roomShape;
    RoomShapeDListsEntry* entry;
    s32 i;
    u32 flags = ROOM_DRAW_OPA | ROOM_DRAW_XLU;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    if (flags & ROOM_DRAW_OPA) {
        gSPSegment(POLY_OPA_DISP++, 0x03, this->roomSegment);
        Gfx_SetupDL_25Opa(gfxCtx);
        Matrix_Translate(this->matrixPos.x, this->matrixPos.y, this->matrixPos.z, MTXMODE_NEW);
        Matrix_Scale(this->matrixScale.x, this->matrixScale.y, this->matrixScale.z, MTXMODE_APPLY);
        Matrix_RotateZYX(this->matrixRot.x, this->matrixRot.y, this->matrixRot.z, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx, __FILE__, __LINE__);
    }

    if (flags & ROOM_DRAW_XLU) {
        gSPSegment(POLY_XLU_DISP++, 0x03, this->roomSegment);
        Gfx_SetupDL_25Xlu(gfxCtx);
        Matrix_Translate(this->matrixPos.x, this->matrixPos.y, this->matrixPos.z, MTXMODE_NEW);
        Matrix_Scale(this->matrixScale.x, this->matrixScale.y, this->matrixScale.z, MTXMODE_APPLY);
        Matrix_RotateZYX(this->matrixRot.x, this->matrixRot.y, this->matrixRot.z, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx, __FILE__, __LINE__);
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

void Preview_DrawRoomCullable(PreviewState* this) {
    GraphicsContext* gfxCtx = this->state->gfxCtx;
    RoomShapeCullable* roomShape;
    RoomShapeCullableEntry* entry;
    s32 i;
    u32 flags = ROOM_DRAW_OPA | ROOM_DRAW_XLU;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    if (flags & ROOM_DRAW_OPA) {
        gSPSegment(POLY_OPA_DISP++, 0x03, this->roomSegment);
        Gfx_SetupDL_25Opa(gfxCtx);
        Matrix_Translate(this->matrixPos.x, this->matrixPos.y, this->matrixPos.z, MTXMODE_NEW);
        Matrix_Scale(this->matrixScale.x, this->matrixScale.y, this->matrixScale.z, MTXMODE_APPLY);
        Matrix_RotateZYX(this->matrixRot.x, this->matrixRot.y, this->matrixRot.z, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx, __FILE__, __LINE__);
    }

    if (flags & ROOM_DRAW_XLU) {
        gSPSegment(POLY_XLU_DISP++, 0x03, this->roomSegment);
        Gfx_SetupDL_25Xlu(gfxCtx);
        Matrix_Translate(this->matrixPos.x, this->matrixPos.y, this->matrixPos.z, MTXMODE_NEW);
        Matrix_Scale(this->matrixScale.x, this->matrixScale.y, this->matrixScale.z, MTXMODE_APPLY);
        Matrix_RotateZYX(this->matrixRot.x, this->matrixRot.y, this->matrixRot.z, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx, __FILE__, __LINE__);
    }

    roomShape = &this->curRoom.roomShape->cullable;
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

void Preview_DrawRoomImage(PreviewState* this) {
    GraphicsContext* gfxCtx = this->state->gfxCtx;
    RoomShapeImageBase* roomShape = &this->curRoom.roomShape->image.base;
    RoomShapeImageSingle* roomShapeSingle;
    RoomShapeImageMulti* roomShapeMulti;
    RoomShapeDListsEntry* entry;
    u32 flags = ROOM_DRAW_OPA /* | ROOM_DRAW_XLU */;
    u8 isOpa = flags & ROOM_DRAW_OPA;
    u8 isXlu = flags & ROOM_DRAW_XLU;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    if (isOpa) {
        gSPSegment(POLY_OPA_DISP++, 0x03, this->roomSegment);
        Gfx_SetupDL_25Opa(gfxCtx);
        Matrix_Translate(this->matrixPos.x, this->matrixPos.y, this->matrixPos.z, MTXMODE_NEW);
        Matrix_Scale(this->matrixScale.x, this->matrixScale.y, this->matrixScale.z, MTXMODE_APPLY);
        Matrix_RotateZYX(this->matrixRot.x, this->matrixRot.y, this->matrixRot.z, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx, __FILE__, __LINE__);
    }

    if (isXlu) {
        gSPSegment(POLY_XLU_DISP++, 0x03, this->roomSegment);
        Gfx_SetupDL_25Xlu(gfxCtx);
        Matrix_Translate(this->matrixPos.x, this->matrixPos.y, this->matrixPos.z, MTXMODE_NEW);
        Matrix_Scale(this->matrixScale.x, this->matrixScale.y, this->matrixScale.z, MTXMODE_APPLY);
        Matrix_RotateZYX(this->matrixRot.x, this->matrixRot.y, this->matrixRot.z, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, gfxCtx, __FILE__, __LINE__);
    }

    switch (roomShape->amountType) {
        case ROOM_SHAPE_IMAGE_AMOUNT_SINGLE:
            roomShapeSingle = &this->curRoom.roomShape->image.single;
            entry = SEGMENTED_TO_VIRTUAL(roomShapeSingle->base.entry);

            if (isOpa) {
                gSPDisplayList(POLY_OPA_DISP++, entry->opa);
            }

            if (isXlu) {
                gSPDisplayList(POLY_XLU_DISP++, entry->xlu);
            }
            break;
        case ROOM_SHAPE_IMAGE_AMOUNT_MULTI:
            roomShapeMulti = &this->curRoom.roomShape->image.multi;
            entry = SEGMENTED_TO_VIRTUAL(roomShapeMulti->base.entry);

            if (isOpa) {
                gSPDisplayList(POLY_OPA_DISP++, entry->opa);
            }

            if (isXlu) {
                gSPDisplayList(POLY_XLU_DISP++, entry->xlu);
            }
            break;
        default:
            PRINTF("unknown image amount type: %d\n", roomShape->amountType);
            break;
    }

    CLOSE_DISPS(gfxCtx, __FILE__, __LINE__);
}

void Preview_HandleDrawConfig(PreviewState* this) {
    GraphicsContext* gfxCtx = this->state->gfxCtx;

    OPEN_DISPS(gfxCtx, __FILE__, __LINE__);

    switch (this->drawConfig) {
#if ENABLE_ANIMATED_MATERIALS
        case SDC_MAT_ANIM:
            AnimatedMat_Draw(this->state, this->gameplayFrames, this->sceneMaterialAnims);
            break;
        case SDC_MAT_ANIM_MANUAL_STEP:
            AnimatedMat_DrawStep(this->state, this->sceneMaterialAnims, 1);
            break;
#endif
        default:
            gSPDisplayList(POLY_OPA_DISP++, sDefaultDisplayList);
            gSPDisplayList(POLY_XLU_DISP++, sDefaultDisplayList);
            break;
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

    gSegments[4] = OS_K0_TO_PHYSICAL(this->objectCtx.slots[this->objectCtx.mainKeepSlot].segment);
    gSPSegment(POLY_OPA_DISP++, 0x04, this->objectCtx.slots[this->objectCtx.mainKeepSlot].segment);
    gSPSegment(POLY_XLU_DISP++, 0x04, this->objectCtx.slots[this->objectCtx.mainKeepSlot].segment);
    gSPSegment(OVERLAY_DISP++, 0x04, this->objectCtx.slots[this->objectCtx.mainKeepSlot].segment);

    gSegments[5] = OS_K0_TO_PHYSICAL(this->objectCtx.slots[this->objectCtx.subKeepSlot].segment);
    gSPSegment(POLY_OPA_DISP++, 0x05, this->objectCtx.slots[this->objectCtx.subKeepSlot].segment);
    gSPSegment(POLY_XLU_DISP++, 0x05, this->objectCtx.slots[this->objectCtx.subKeepSlot].segment);
    gSPSegment(OVERLAY_DISP++, 0x05, this->objectCtx.slots[this->objectCtx.subKeepSlot].segment);

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

        Preview_HandleDrawConfig(this);

        if (this->roomSegment != NULL) {
            gSegments[3] = OS_K0_TO_PHYSICAL(this->roomSegment);

            switch (this->curRoom.roomShape->base.type) {
                case ROOM_SHAPE_TYPE_NORMAL:
                    Preview_DrawRoomNormal(this);
                    break;
                case ROOM_SHAPE_TYPE_CULLABLE:
                    Preview_DrawRoomCullable(this);
                    break;
                case ROOM_SHAPE_TYPE_IMAGE:
                    Preview_DrawRoomImage(this);
                    break;
                default:
                    PRINTF("unknown room shape type: %d\n", this->curRoom.roomShape->base.type);
                    break;
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
    Preview_FreeObjectAlloc(this);
    Preview_FreeRoom(this);
    Preview_FreeScene(this);
    memset(this, 0, sizeof(PreviewState));
    sGameState = NULL;
    sView = NULL;
}
