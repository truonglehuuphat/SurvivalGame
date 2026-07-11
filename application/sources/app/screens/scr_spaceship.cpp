#include "scr_spaceship.h"

#include <stdlib.h>

/*****************************************************************************/
/* scr_spaceship
 *
 * Controls:
 *   - UP button   short press  -> move BACKWARD (ship moves left)
 *   - DOWN button short press  -> move FORWARD   (ship moves right)
 *   - UP button   long press   -> move UP        (ship moves up)
 *   - DOWN button long press   -> move DOWN      (ship moves down)
 *   - MODE button short press  -> re-center the ship
 */
/*****************************************************************************/

#define SHIP_HALF_SIZE			(6)
#define SHIP_STEP_X				(10)	/* forward / backward step */
#define SHIP_STEP_Y				(10)	/* up / down step */

#define SHIP_MIN_X				(SHIP_HALF_SIZE)
#define SHIP_MAX_X				((LCD_WIDTH - 1)  - SHIP_HALF_SIZE)
#define SHIP_MIN_Y				(SHIP_HALF_SIZE)
#define SHIP_MAX_Y				((LCD_HEIGHT - 1) - SHIP_HALF_SIZE)

#define STAR_COUNT				(10)

static void view_scr_spaceship();

view_dynamic_t dyn_view_spaceship = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_spaceship
};

view_screen_t scr_spaceship = {
	&dyn_view_spaceship,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

/* spaceship current position (center point) */
static int16_t ship_x;
static int16_t ship_y;

/* simple star field used to give a visual feedback of forward/backward motion */
static int16_t star_x[STAR_COUNT];
static int16_t star_y[STAR_COUNT];

static void spaceship_reset() {
	ship_x = LCD_WIDTH / 2;
	ship_y = LCD_HEIGHT / 2;

	for (uint8_t i = 0; i < STAR_COUNT; i++) {
		star_x[i] = rand() % LCD_WIDTH;
		star_y[i] = rand() % LCD_HEIGHT;
	}
}

/* shift the star field horizontally to simulate the ship moving through space
 * dx > 0 -> ship moves forward (stars flow backward/left)
 * dx < 0 -> ship moves backward (stars flow forward/right)
 */
static void spaceship_scroll_stars(int16_t dx) {
	for (uint8_t i = 0; i < STAR_COUNT; i++) {
		star_x[i] -= dx;

		if (star_x[i] < 0) {
			star_x[i] += LCD_WIDTH;
			star_y[i] = rand() % LCD_HEIGHT;
		}
		else if (star_x[i] >= LCD_WIDTH) {
			star_x[i] -= LCD_WIDTH;
			star_y[i] = rand() % LCD_HEIGHT;
		}
	}
}

/* move the ship, clamping to the screen bounds.
 * returns true if the ship actually moved, false if it was blocked by an edge.
 */
static bool spaceship_move(int16_t dx, int16_t dy) {
	int16_t new_x = ship_x + dx;
	int16_t new_y = ship_y + dy;
	bool moved = true;

	if (new_x < SHIP_MIN_X) {
		new_x = SHIP_MIN_X;
		moved = false;
	}
	else if (new_x > SHIP_MAX_X) {
		new_x = SHIP_MAX_X;
		moved = false;
	}

	if (new_y < SHIP_MIN_Y) {
		new_y = SHIP_MIN_Y;
		moved = false;
	}
	else if (new_y > SHIP_MAX_Y) {
		new_y = SHIP_MAX_Y;
		moved = false;
	}

	ship_x = new_x;
	ship_y = new_y;

	return moved;
}

static void view_scr_spaceship() {
	view_render.clear();

	/* star field background */
	for (uint8_t i = 0; i < STAR_COUNT; i++) {
		view_render.drawPixel(star_x[i], star_y[i], WHITE);
	}

	/* viewport border */
	view_render.drawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, WHITE);

	/* spaceship body: triangle nose pointing up */
	view_render.fillTriangle(	ship_x,				ship_y - SHIP_HALF_SIZE,	\
								ship_x - SHIP_HALF_SIZE,	ship_y + SHIP_HALF_SIZE,	\
								ship_x + SHIP_HALF_SIZE,	ship_y + SHIP_HALF_SIZE,	\
								WHITE);

	/* wings */
	view_render.drawLine(ship_x - SHIP_HALF_SIZE, ship_y + SHIP_HALF_SIZE, ship_x - SHIP_HALF_SIZE - 3, ship_y + SHIP_HALF_SIZE + 3, WHITE);
	view_render.drawLine(ship_x + SHIP_HALF_SIZE, ship_y + SHIP_HALF_SIZE, ship_x + SHIP_HALF_SIZE + 3, ship_y + SHIP_HALF_SIZE + 3, WHITE);

	/* engine flame */
	view_render.drawLine(ship_x - 2, ship_y + SHIP_HALF_SIZE, ship_x - 2, ship_y + SHIP_HALF_SIZE + 4, WHITE);
	view_render.drawLine(ship_x + 2, ship_y + SHIP_HALF_SIZE, ship_x + 2, ship_y + SHIP_HALF_SIZE + 4, WHITE);
}

void scr_spaceship_handle(ak_msg_t *msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		spaceship_reset();
	} break;

	/* short press UP -> move backward */
	case AC_DISPLAY_BUTON_UP_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_UP_PRESSED -> move backward\n");
		if (spaceship_move(-SHIP_STEP_X, 0)) {
			spaceship_scroll_stars(-SHIP_STEP_X);
			BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		}
		else {
			BUZZER_PlaySound(BUZZER_SOUND_BANG);
		}
	} break;

	/* short press DOWN -> move forward */
	case AC_DISPLAY_BUTON_DOWN_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_DOWN_PRESSED -> move forward\n");
		if (spaceship_move(SHIP_STEP_X, 0)) {
			spaceship_scroll_stars(SHIP_STEP_X);
			BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		}
		else {
			BUZZER_PlaySound(BUZZER_SOUND_BANG);
		}
	} break;

	/* long press UP -> move up */
	case AC_DISPLAY_BUTON_UP_LONG_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_UP_LONG_PRESSED -> move up\n");
		if (spaceship_move(0, -SHIP_STEP_Y)) {
			BUZZER_PlaySound(BUZZER_SOUND_TONE_1);
		}
		else {
			BUZZER_PlaySound(BUZZER_SOUND_BANG);
		}
	} break;

	/* long press DOWN -> move down */
	case AC_DISPLAY_BUTON_DOWN_LONG_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_DOWN_LONG_PRESSED -> move down\n");
		if (spaceship_move(0, SHIP_STEP_Y)) {
			BUZZER_PlaySound(BUZZER_SOUND_TONE_1);
		}
		else {
			BUZZER_PlaySound(BUZZER_SOUND_BANG);
		}
	} break;

	/* MODE -> re-center the ship */
	case AC_DISPLAY_BUTON_MODE_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_PRESSED -> reset\n");
		spaceship_reset();
		BUZZER_PlaySound(BUZZER_SOUND_3BEEP);
	} break;

	default:
		break;
	}
}
