/*
 * File: z_opening.c
 * Overlay: ovl_opening
 * Description: Initializes the game into the title screen
 */

#include "gfx.h"
#include "regs.h"
#include "sys_matrix.h"
#include "title_setup_state.h"
#include "letterbox.h"
#include "game.h"
#include "play_state.h"
#include "save.h"
#include "sram.h"
#include "view.h"

void TitleSetup_SetupTitleScreen(TitleSetupState* this) {
    gSaveContext.gameMode = GAMEMODE_TITLE_SCREEN;
    this->state.running = false;
    gSaveContext.save.linkAge = LINK_AGE_ADULT;
    Sram_InitDebugSave();
    gSaveContext.save.cutsceneIndex = CS_INDEX_3;
    // assigning scene layer here is redundant, as Play_Init sets it right away
    gSaveContext.sceneLayer = GET_CUTSCENE_LAYER(CS_INDEX_3);
    SET_NEXT_GAMESTATE(&this->state, Play_Init, PlayState);
}

void func_80803C5C(TitleSetupState* this) {
}

void TitleSetup_Main(GameState* thisx) {
    TitleSetupState* this = (TitleSetupState*)thisx;

    Gfx_SetupFrame(this->state.gfxCtx, true, 0, 0, 0);
    Gfx_ClearZBuffer(this->state.gfxCtx);
    TitleSetup_SetupTitleScreen(this);
    func_80803C5C(this);
}

void TitleSetup_Destroy(GameState* thisx) {
#if ENABLE_NEW_LETTERBOX
    ShrinkWindow_Destroy();
#endif
}

void TitleSetup_Init(GameState* thisx) {
    TitleSetupState* this = (TitleSetupState*)thisx;

    R_UPDATE_RATE = 1;
    Matrix_Init(&this->state);
#if ENABLE_NEW_LETTERBOX
    ShrinkWindow_Init();
#endif
    View_Init(&this->view, this->state.gfxCtx);
    this->state.main = TitleSetup_Main;
    this->state.destroy = TitleSetup_Destroy;
}
