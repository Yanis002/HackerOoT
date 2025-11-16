#ifndef HACKEROOT_PREVIEW_H
#define HACKEROOT_PREVIEW_H

#include "ultra64.h"
#include "config.h"
#include "game.h"
#include "light.h"
#include "room.h"
#include "view.h"
#include "environment.h"
#include "object.h"
#include "actor.h"
#include "bgcheck.h"
#include "scene.h"
#include "animated_materials.h"

#define PREVIEW_TIMER 30

typedef struct PreviewState {
    GameState* state;
    View* view;
    void* sceneSegment;
    void* roomSegment;
    u8 sceneId;
    u8 drawConfig;
    u8 dmaTimer;
    u8 doLoad;
    u8 editMode;
    u8 overrideInput;
    u8 framesTimer;
    u32 gameplayFrames;
    CollisionHeader colHeader;
    EnvironmentContext envCtx;
    u8 skyboxId;
    LightContext lightCtx;
    TransitionActorList transitionActors;
#if ENABLE_ANIMATED_MATERIALS
    AnimatedMaterial* sceneMaterialAnims;
#endif
    RoomList roomList;
    Room curRoom;
    ObjectContext objectCtx;
    ActorContext actorCtx;
    u8 numActorEntries;
    ActorEntry* actorEntryList;
    Spawn* spawnList;
    Vec3f worldEye;
    Vec3f worldAt;
    Vec3f matrixPos;
    Vec3f matrixScale;
    Vec3s matrixRot;
} PreviewState;

void* Preview_LoadFile(GameState* gameState, RomFile* file);
void Preview_SetSceneId(PreviewState* this, u8 sceneId);
void Preview_SetTimer(PreviewState* this, u8 dmaTimer);
void Preview_SetGameState(PreviewState* this, GameState* state);
void Preview_SetView(PreviewState* this, View* view);
void Preview_FreeScene(PreviewState* this);
void Preview_FreeRoom(PreviewState* this);
void Preview_FreeObjectAlloc(PreviewState* this);
void Preview_Reset(PreviewState* this);
void Preview_InitScene(PreviewState* this);
void Preview_InitRoom(PreviewState* this);
void Preview_LoadScene(PreviewState* this);
void Preview_Init(PreviewState* this);
void Preview_Debug(PreviewState* this);
void Preview_Update(PreviewState* this);
void Preview_DrawRoomNormal(PreviewState* this);
void Preview_Draw(PreviewState* this);
void Preview_Main(PreviewState* this);
void Preview_Destroy(PreviewState* this);

#endif
