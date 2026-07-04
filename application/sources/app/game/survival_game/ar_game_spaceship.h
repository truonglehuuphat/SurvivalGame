#ifndef __AR_GAME_SPACESHIP_H__
#define __AR_GAME_SPACESHIP_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#include "scr_survival_game.h"

#define STEP_ARCHERY_AXIS_Y     (10)
#define SIZE_BITMAP_ARCHERY_X   (15)
#define SIZE_BITMAP_ARCHERY_Y   (15)
#define AXIS_X_ARCHERY          (0)
#define AXIS_Y_ARCHERY          (30)

typedef struct {
    bool visible;
    uint32_t x, y;
    uint8_t action_image;
} ar_game_spaceship_t;

extern ar_game_spaceship_t archery;

/* Khoi tao phi thuyen tai vi tri trung tam man hinh */
void ar_game_spaceship_init(void);

/* Di chuyen phi thuyen theo huong, tu dong ghim (clamp) trong bien 128x64 */
void ar_game_spaceship_move(spaceship_dir_t dir);

/* Ve phi thuyen len man hinh (goi trong vong lap render / task_display) */
void ar_game_spaceship_draw(void);

#endif //__AR_GAME_SPACESHIP_H__
