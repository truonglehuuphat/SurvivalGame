#include "ar_game_spaceship.h"
#include "screens_bitmap.h"
#include "view_render.h"

ar_game_spaceship_t spaceship;

/*****************************************************************************/
/* ar_game_spaceship_init                                                    */
/* Dat phi thuyen vao giua man hinh 128x64                                   */
/*****************************************************************************/
void ar_game_spaceship_init(void)
{
    spaceship.visible      = true;
    spaceship.action_image = AR_GAME_SPACESHIP_ACTION_IMAGE_1;

    spaceship.x = (LCD_WIDTH  - SIZE_BITMAP_SPACESHIP_X) / 2;
    spaceship.y = (LCD_HEIGHT - SIZE_BITMAP_SPACESHIP_Y) / 2;
}

/*****************************************************************************/
/* ar_game_spaceship_move                                                    */
/* Di chuyen phi thuyen, khong cho phep di ra ngoai vung 128x64.             */
/* Neu da cham bien, phi thuyen dung lai tai bien (khong wrap-around).       */
/*****************************************************************************/
void ar_game_spaceship_move(spaceship_dir_t dir)
{
    switch (dir)
    {
        case SPACESHIP_DIR_UP:
            if (spaceship.y > SPACESHIP_MIN_Y + SPACESHIP_MOVE_STEP)
                spaceship.y -= SPACESHIP_MOVE_STEP;
            else
                spaceship.y = SPACESHIP_MIN_Y;
            break;

        case SPACESHIP_DIR_DOWN:
            if (spaceship.y + SPACESHIP_MOVE_STEP < SPACESHIP_MAX_Y)
                spaceship.y += SPACESHIP_MOVE_STEP;
            else
                spaceship.y = SPACESHIP_MAX_Y;
            break;

        case SPACESHIP_DIR_LEFT:
            if (spaceship.x > SPACESHIP_MIN_X + SPACESHIP_MOVE_STEP)
                spaceship.x -= SPACESHIP_MOVE_STEP;
            else
                spaceship.x = SPACESHIP_MIN_X;
            break;

        case SPACESHIP_DIR_RIGHT:
            if (spaceship.x + SPACESHIP_MOVE_STEP < SPACESHIP_MAX_X)
                spaceship.x += SPACESHIP_MOVE_STEP;
            else
                spaceship.x = SPACESHIP_MAX_X;
            break;

        default:
            break;
    }
}

/*****************************************************************************/
/* ar_game_spaceship_draw                                                    */
/* Goi trong ham render cua man hinh game (vd scr_archery_game render loop)  */
/*****************************************************************************/
void ar_game_spaceship_draw(void)
{
    if (!spaceship.visible)
        return;

    view_render.drawBitmap(spaceship.x, spaceship.y,
                            bitmap_spaceship,
                            SIZE_BITMAP_SPACESHIP_X,
                            SIZE_BITMAP_SPACESHIP_Y,
                            WHITE);
}
