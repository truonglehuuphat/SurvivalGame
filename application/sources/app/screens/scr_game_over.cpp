#include "scr_game_over.h"

#include "screens.h"
#include "scr_idle.h"
#include "scr_game_space_ship.h"
#include "scr_menu_game.h"

/*****************************************************************************/
/* View - Game over */
/*****************************************************************************/
static void view_scr_game_over();

view_dynamic_t dyn_view_item_game_over = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_over
};

view_screen_t scr_game_over = {
	&dyn_view_item_game_over,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_game_over() {
	view_render.clear();

	view_render.setTextSize(2);
	view_render.setTextColor(WHITE);
	view_render.setCursor(14, 18);
	view_render.print("GAME OVER");

	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(4, 42);
	view_render.print("MODE : choi lai");
	view_render.setCursor(4, 52);
	view_render.print("UP/DOWN : ve menu");
}

/*****************************************************************************/
/* Handle - Game over */
/*****************************************************************************/
void scr_game_over_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		BUZZER_PlaySound(BUZZER_SOUND_LOWSCORE);

		// Timer switch to scr_idle neu khong bam gi
		timer_set(	AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_SHOW_IDLE, \
					AC_DISPLAY_IDLE_INTERVAL, \
					TIMER_ONE_SHOT);
	} break;

	case AC_DISPLAY_SHOW_IDLE: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_IDLE\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
		SCREEN_TRAN(scr_idle_handle, &scr_idle);
	} break;

	case AC_DISPLAY_BUTTON_MODE_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_PRESSED\n");
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		// Choi lai tu dau
		SCREEN_TRAN(scr_game_space_ship_handle, &scr_game_space_ship);
	} break;

	case AC_DISPLAY_BUTTON_UP_PRESSED:
	case AC_DISPLAY_BUTTON_DOWN_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP/DOWN_PRESSED\n");
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		// Ve lai menu chinh
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	} break;

	default:
		break;
	}
}