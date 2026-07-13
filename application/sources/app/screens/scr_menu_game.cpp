#include "scr_menu_game.h"

#include "scr_idle.h"
#include "scr_game_space_ship.h"

/*****************************************************************************/
/* Menu don gian: chi dung chu, khong can icon bitmap                        */
/* Muc 0: Space Ship game                                                    */
/* Muc 1: Exit -> ve man hinh nghi (scr_idle)                                */
/*****************************************************************************/
#define NUMBER_MENU_ITEMS      (2)
#define MENU_ITEM_SPACE_SHIP   (0)
#define MENU_ITEM_EXIT         (1)

static const char* menu_items_name[NUMBER_MENU_ITEMS] = {
	"> Space Ship",
	"> Exit",
};

static uint8_t menu_selected_item = 0;

/*****************************************************************************/
/* View - Menu game */
/*****************************************************************************/
static void view_scr_menu_game();

view_dynamic_t dyn_view_item_menu_game = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_menu_game
};

view_screen_t scr_menu_game = {
	&dyn_view_item_menu_game,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_menu_game() {
	view_render.setTextSize(1);

	for (uint8_t i = 0; i < NUMBER_MENU_ITEMS; i++) {
		uint8_t y = 10 + (i * 14);

		if (i == menu_selected_item) {
			// Muc dang chon: ve khung tren nen trang, chu den
			view_render.fillRect(0, y - 2, 128, 12, WHITE);
			view_render.setTextColor(BLACK);
		}
		else {
			view_render.setTextColor(WHITE);
		}

		view_render.setCursor(8, y);
		view_render.print(menu_items_name[i]);
	}
}

/*****************************************************************************/
/* Handle - Menu game */
/*****************************************************************************/
static void screen_tran_menu() {
	switch (menu_selected_item) {
	case MENU_ITEM_SPACE_SHIP: {
		SCREEN_TRAN(scr_game_space_ship_handle, &scr_game_space_ship);
	} break;

	case MENU_ITEM_EXIT: {
		SCREEN_TRAN(scr_idle_handle, &scr_idle);
	} break;

	default:
		break;
	}
}

void scr_menu_game_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		menu_selected_item = 0;

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
		screen_tran_menu();
	} break;

	case AC_DISPLAY_BUTTON_UP_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_PRESSED\n");
		if (menu_selected_item > 0) {
			menu_selected_item--;
		}
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);
	} break;

	case AC_DISPLAY_BUTTON_DOWN_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_PRESSED\n");
		if (menu_selected_item < NUMBER_MENU_ITEMS - 1) {
			menu_selected_item++;
		}
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE, AC_DISPLAY_IDLE_INTERVAL, TIMER_ONE_SHOT);
	} break;

	default:
		break;
	}
}