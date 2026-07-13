#ifndef __SCR_GAME_SPACE_SHIP_H__
#define __SCR_GAME_SPACE_SHIP_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "view_render.h"

#include "screens.h"
#include "screens_bitmap.h"

#include "ar_game_ship_meteoroid.h"
#include "ar_game_ship_bang.h"
#include "ar_game_spaceship.h"

#define SPACESHIP_GAME_OFF			(0)
#define SPACESHIP_GAME_PLAY			(1)
#define SPACESHIP_GAME_EXPLODING		(2)  /* phi thuyen vua va cham, dang phat hoat hinh no */
#define SPACESHIP_GAME_OVER			(3)  /* hien chu "GAME OVER" truoc khi thoat man hinh */

extern uint8_t spaceship_game_state;

extern view_dynamic_t dyn_view_item_space_ship;
extern view_screen_t scr_game_space_ship;
extern void scr_game_space_ship_handle(ak_msg_t* msg);

#endif //__SCR_GAME_SPACE_SHIP_H__
