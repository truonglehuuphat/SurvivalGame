#include "scr_game_space_ship.h"

#include "screens.h"

/*****************************************************************************/
/* Variable Declaration - Space ship screen */
/*****************************************************************************/
uint8_t spaceship_game_state;

static const char* SHIP_GAME_OVER_TEXT = "GAME OVER";

/*****************************************************************************/
/* View - Space ship screen */
/*****************************************************************************/
void spaceship_frame_display() {
	view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);
}

void spaceship_display() {
	if (spaceship.visible == WHITE) {
		if (spaceship.action_image == AR_GAME_SPACESHIP_ACTION_IMAGE_1) {
			view_render.drawBitmap(	spaceship.x, \
					spaceship.y, \
					bitmap_spaceship, \
					SIZE_BITMAP_SPACESHIP_X, \
					SIZE_BITMAP_SPACESHIP_Y, \
					WHITE);
		}
	}
}

void spaceship_ship_meteoroid_display() {
	for (uint8_t i = 0; i < NUM_SHIP_METEOROIDS; i++) {
		if (ship_meteoroid[i].visible == WHITE) {
			if (ship_meteoroid[i].action_image == AR_GAME_SHIP_METEOROID_ACTION_IMAGE_1) {
				view_render.drawBitmap(	ship_meteoroid[i].x, \
										ship_meteoroid[i].y, \
										bitmap_meteoroid_I, \
										SIZE_BITMAP_SHIP_METEOROID_X, \
										SIZE_BITMAP_SHIP_METEOROID_Y, \
										WHITE);
			}
			else if (ship_meteoroid[i].action_image == AR_GAME_SHIP_METEOROID_ACTION_IMAGE_2) {
				view_render.drawBitmap(	ship_meteoroid[i].x, \
										ship_meteoroid[i].y, \
										bitmap_meteoroid_II, \
										SIZE_BITMAP_SHIP_METEOROID_X, \
										SIZE_BITMAP_SHIP_METEOROID_Y, \
										WHITE);
			}
			else if (ship_meteoroid[i].action_image == AR_GAME_SHIP_METEOROID_ACTION_IMAGE_3) {
				view_render.drawBitmap(	ship_meteoroid[i].x, \
										ship_meteoroid[i].y, \
										bitmap_meteoroid_III, \
										SIZE_BITMAP_SHIP_METEOROID_X, \
										SIZE_BITMAP_SHIP_METEOROID_Y, \
										WHITE);
			}
		}
	}
}

void spaceship_ship_bang_display() {
	if (ship_bang.visible == WHITE) {
		if (ship_bang.action_image == AR_GAME_SHIP_BANG_ACTION_IMAGE_1) {
			view_render.drawBitmap(	ship_bang.x - 3, \
					ship_bang.y - 4, \
					bitmap_bang_I, \
					SIZE_BITMAP_SHIP_BANG_I_X, \
					SIZE_BITMAP_SHIP_BANG_I_Y, \
					WHITE);
		}
		else if (ship_bang.action_image == AR_GAME_SHIP_BANG_ACTION_IMAGE_2) {
			view_render.drawBitmap(	ship_bang.x - 3, \
					ship_bang.y - 4, \
					bitmap_bang_II, \
					SIZE_BITMAP_SHIP_BANG_I_X, \
					SIZE_BITMAP_SHIP_BANG_I_Y, \
					WHITE);
		}
		else if (ship_bang.action_image == AR_GAME_SHIP_BANG_ACTION_IMAGE_3) {
			view_render.drawBitmap(	ship_bang.x - 1, \
					ship_bang.y - 1, \
					bitmap_bang_III, \
					SIZE_BITMAP_SHIP_BANG_II_X, \
					SIZE_BITMAP_SHIP_BANG_II_Y, \
					WHITE);
		}
	}
}

void spaceship_game_over_display() {
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(38, 28);
	view_render.print(SHIP_GAME_OVER_TEXT);
}

static void view_scr_game_space_ship();

view_dynamic_t dyn_view_item_space_ship = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_space_ship
};

view_screen_t scr_game_space_ship = {
	&dyn_view_item_space_ship,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_game_space_ship() {
	if (spaceship_game_state == SPACESHIP_GAME_PLAY ||
		spaceship_game_state == SPACESHIP_GAME_EXPLODING)
	{
		spaceship_frame_display();
		spaceship_ship_meteoroid_display();
		spaceship_display();
		spaceship_ship_bang_display();
	}
	else if (spaceship_game_state == SPACESHIP_GAME_OVER) {
		view_render.clear();
		spaceship_game_over_display();
	}
}

/*****************************************************************************/
/* Handle - Space ship screen */
/*****************************************************************************/
void scr_game_space_ship_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");

		// Setup game Object
		task_post_pure_msg(AR_GAME_SPACESHIP_ID,		AR_GAME_SPACESHIP_SETUP);
		task_post_pure_msg(AR_GAME_SHIP_METEOROID_ID,	AR_GAME_SHIP_METEOROID_SETUP);
		task_post_pure_msg(AR_GAME_SHIP_BANG_ID,		AR_GAME_SHIP_BANG_SETUP);

		// Set state 'SPACESHIP_GAME_PLAY' & remove idle screen timer
		spaceship_game_state = SPACESHIP_GAME_PLAY;
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);

		// Setup timer
		timer_set(	AC_TASK_DISPLAY_ID, \
					AR_GAME_SPACESHIP_SCREEN_TIME_TICK, \
					AR_GAME_SPACESHIP_TIME_TICK_INTERVAL, \
					TIMER_PERIODIC);
	} break;

	case AR_GAME_SPACESHIP_SCREEN_TIME_TICK: {
		APP_DBG_SIG("AR_GAME_SPACESHIP_SCREEN_TIME_TICK\n");

		if (spaceship_game_state == SPACESHIP_GAME_PLAY) {
			// Dong bo vi tri phi thuyen sau khi cac nut da post lenh di chuyen
			task_post_pure_msg(AR_GAME_SPACESHIP_ID, AR_GAME_SPACESHIP_UPDATE);

			// Thien thach tiep tuc roi va kiem tra va cham voi phi thuyen
			task_post_pure_msg(AR_GAME_SHIP_METEOROID_ID, AR_GAME_SHIP_METEOROID_RUN);
			task_post_pure_msg(AR_GAME_SHIP_METEOROID_ID, AR_GAME_SHIP_METEOROID_CHECK_COLLISION);
		}
		else if (spaceship_game_state == SPACESHIP_GAME_EXPLODING) {
			// Dong bang thien thach, chi chay hoat hinh vu no
			task_post_pure_msg(AR_GAME_SHIP_BANG_ID, AR_GAME_SHIP_BANG_UPDATE);
		}
	} break;

	case AR_GAME_SPACESHIP_SCREEN_HIT: {
		APP_DBG_SIG("AR_GAME_SPACESHIP_SCREEN_HIT\n");
		// Phi thuyen vua va cham thien thach (da duoc an va bat vu no
		// trong ar_game_ship_meteoroid_handle). Chuyen sang trang thai
		// dang no, van con chay tick de vu no duoc hoat hinh.
		spaceship_game_state = SPACESHIP_GAME_EXPLODING;
	} break;

	case AR_GAME_SPACESHIP_SCREEN_RESET: {
		APP_DBG_SIG("AR_GAME_SPACESHIP_SCREEN_RESET\n");
		// Duoc ar_game_ship_bang_handle goi khi hoat hinh no da chay xong

		// Stop timer tick
		timer_remove_attr(AC_TASK_DISPLAY_ID, AR_GAME_SPACESHIP_SCREEN_TIME_TICK);

		// Reset toan bo game Object
		task_post_pure_msg(AR_GAME_SPACESHIP_ID,		AR_GAME_SPACESHIP_RESET);
		task_post_pure_msg(AR_GAME_SHIP_METEOROID_ID,	AR_GAME_SHIP_METEOROID_RESET);
		task_post_pure_msg(AR_GAME_SHIP_BANG_ID,		AR_GAME_SHIP_BANG_RESET);

		// Hien chu "GAME OVER"
		spaceship_game_state = SPACESHIP_GAME_OVER;

		// Timer thoat man hinh, dung chung khoang thoi gian voi archery game
		timer_set(	AC_TASK_DISPLAY_ID, \
					AR_GAME_SPACESHIP_SCREEN_EXIT_GAME, \
					AR_GAME_SPACESHIP_TIME_EXIT_INTERVAL, \
					TIMER_ONE_SHOT);

		BUZZER_PlaySound(BUZZER_SOUND_LOWSCORE);
	} break;

	// case AR_GAME_SPACESHIP_SCREEN_EXIT_GAME: {
	// 	APP_DBG_SIG("AR_GAME_SPACESHIP_SCREEN_EXIT_GAME\n");

	// 	// State off & set timer show idle screen
	// 	spaceship_game_state = SPACESHIP_GAME_OFF;
	// 	timer_set(	AC_TASK_DISPLAY_ID, \
	// 		AC_DISPLAY_SHOW_IDLE, \
	// 		AC_DISPLAY_IDLE_INTERVAL, \
	// 		TIMER_ONE_SHOT);

	// 	// Quay ve menu
	// 	SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	// } break;

	default:
		break;
	}
}
