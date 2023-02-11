
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL.h>

#include "lib/lvgl/lvgl.h"

#include "lib/lv_drivers/display/monitor.h"
#include "lib/lv_drivers/indev/mouse.h"
#include "lib/lv_drivers/indev/keyboard.h"
#include "lib/lv_drivers/indev/mousewheel.h"

#include "lib/lvgl/examples/lv_examples.h"
#include "lib/lvgl/demos/lv_demos.h"

#include "lib/ui/ui.h"
#include "lib/ui/ui_helpers.h"
#include "lib/ui/ui_comp.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int  tick_thread(void *data);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/


void my_lvgl_fs_test(void)
{
#if 0 /* 测试读写 */
    lv_fs_file_t f;
    lv_fs_res_t  res;
    char        *dirName = "/Track";
    lv_fs_dir_t  dir;
    char         dirNameTemp[512] = {0};
    char         temp[512]        = {0};
    strcat(dirNameTemp, dirName);
    strcat(dirNameTemp, "/");
    // sprintf(temp, "%s-%03d.txt", esp_log_system_timestamp(), esp_random() % 100);
    sprintf(temp, "%03d.txt", esp_random() % 100);

    strcat(dirNameTemp, temp);
    // strcat(dirNameTemp, "/");
    ESP_LOGI("fs_test", "dirNameTemp: %s", dirNameTemp);

    /* 创建文件并写入名字 */
    res = lv_fs_open(&f, dirNameTemp, LV_FS_MODE_RD | LV_FS_MODE_WR);
    if (res != LV_FS_RES_OK) {
        ESP_LOGW("fs_test", "lv_fs_open failed: %d", res);
    } else {
        ESP_LOGW("fs_test", "lv_fs_open succeed");
        ESP_LOGI("fs_test", "temp: %s", temp);
        res = lv_fs_write(&f, temp, strlen(temp), NULL);
        if (res == LV_FS_RES_OK) {
            ESP_LOGI("fs_test", "lv_fs_write ok");
        } else {
            ESP_LOGE("fs_test", "lv_fs_write failed: %d", res);
        }

        memset(temp, 0, 512);
        uint32_t actual_read_byte = 0;

        res = lv_fs_read(&f, temp, 512, &actual_read_byte);

        if (res == LV_FS_RES_OK) {
            ESP_LOGI("fs_test", "lv_fs_read ok, byte: %d", actual_read_byte);
            ESP_LOGI("fs_test", "temp: %s", temp);
        } else {
            ESP_LOGE("fs_test", "lv_fs_read failed: %d", res);
        }
    }

    /* 扫描其中文件 */
    if (lv_fs_dir_open(&dir, dirName) == LV_FS_RES_OK) {

        LV_LOG_USER("%s open success", dirName);

        char name[512];
        while (1) {

            /* 用于防止遍历文件时文件过多耗时过多导致看门狗触发 */
            if ((esp_log_timestamp() + 1) % 200 == 0) {
                vTaskDelay(pdMS_TO_TICKS(1));
            }

            lv_fs_res_t res = lv_fs_dir_read(&dir, name);

            if (name[0] == '\0' || res != LV_FS_RES_OK) {
                // LV_LOG_ERROR("name = %s, res = %d", name, res);
                break;
            }

            LV_LOG_USER("name = %s", name); /* TODO: 记得注释掉 */
        }
        lv_fs_dir_close(&dir);
    } else {
        LV_LOG_ERROR("%s open faild", dirName);
    }

    // uint32_t t0 = esp_log_timestamp();
    // for (uint32_t i = 0; i < 9999999999999; i++) {
    //     lv_fs_open(&f, "/:foo.txt", LV_FS_MODE_RD);
    //     if (esp_log_timestamp() - t0 >= 20) {
    //         t0 = esp_log_timestamp();
    //         /* 用于防止遍历文件时文件过多耗时过多导致看门狗触发 */
    //         vTaskDelay(pdMS_TO_TICKS(10));
    //     }
    // }

    // uint32_t read_num;
    // uint8_t  buf[8];
    // res = lv_fs_read(&f, buf, 8, &read_num);
    // if (res != LV_FS_RES_OK || read_num != 8) my_error_handling();

    lv_fs_close(&f);
#else /* 扫描文件 */
    bool        retval  = false;
    char       *dirName = "/";
    lv_fs_dir_t dir;

    if (lv_fs_dir_open(&dir, dirName) == LV_FS_RES_OK) {

        printf("%s open success\n", dirName); /* LV_LOG_USER */

        char name[128];
        while (1) {

            /* 用于防止遍历文件时文件过多耗时过多导致看门狗触发 */
            if ((esp_log_timestamp() + 1) % 200 == 0) {
                vTaskDelay(pdMS_TO_TICKS(1));
            }

            lv_fs_res_t res = lv_fs_dir_read(&dir, name);

            if (name[0] == '\0' || res != LV_FS_RES_OK) {
                printf("error: name = %s, res = %d\n", name, res); /* LV_LOG_ERROR */
                break;
            }

            printf("name = %s\n", name); /* TODO: 记得注释掉 */
        }
        lv_fs_dir_close(&dir);
    } else {
        printf("error: %s open faild\n", dirName);
    }
#endif
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char **argv)
{
    (void)argc; /*Unused*/
    (void)argv; /*Unused*/

    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_init();

    // ui_init();

    lv_example_gif_1();
    // lv_example_style_1();
    // lv_example_switch_1();
    //  lv_example_calendar_1();
    //  lv_example_btnmatrix_2();
    //  lv_example_checkbox_1();
    //  lv_example_colorwheel_1();
    //  lv_example_chart_6();
    //  lv_example_table_2();
    //  lv_example_scroll_2();
    //  lv_example_textarea_1();
    //  lv_example_msgbox_1();
    //  lv_example_dropdown_2();
    //  lv_example_btn_1();
    //  lv_example_scroll_1();
    //  lv_example_tabview_1();
    //  lv_example_tabview_1();
    //  lv_example_flex_3();
    //  lv_example_label_1();

    //  lv_demo_widgets();
    //  lv_demo_keypad_encoder();
    //  lv_demo_benchmark();
    //  lv_demo_stress();
    //  lv_demo_music();

    while (1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_timer_handler();
        usleep(5 * 1000);
    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static void hal_init(void)
{
    /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    monitor_init();
    /* Tick init.
     * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about
     * how much time were elapsed Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);

    /*Create a display buffer*/
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t         buf1_1[MONITOR_HOR_RES * 100];
    static lv_color_t         buf1_2[MONITOR_HOR_RES * 100];
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, MONITOR_HOR_RES * 100);

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.draw_buf     = &disp_buf1;
    disp_drv.flush_cb     = monitor_flush;
    disp_drv.hor_res      = MONITOR_HOR_RES;
    disp_drv.ver_res      = MONITOR_VER_RES;
    disp_drv.antialiasing = 1;

    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    lv_theme_t *th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t *g = lv_group_create();
    lv_group_set_default(g);

    /* Add the mouse as input device
     * Use the 'mouse' driver which reads the PC's mouse*/
    mouse_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb     = mouse_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    keyboard_init();
    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2); /*Basic initialization*/
    indev_drv_2.type     = LV_INDEV_TYPE_KEYPAD;
    indev_drv_2.read_cb  = keyboard_read;
    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
    lv_indev_set_group(kb_indev, g);
    mousewheel_init();
    static lv_indev_drv_t indev_drv_3;
    lv_indev_drv_init(&indev_drv_3); /*Basic initialization*/
    indev_drv_3.type    = LV_INDEV_TYPE_ENCODER;
    indev_drv_3.read_cb = mousewheel_read;

    lv_indev_t *enc_indev = lv_indev_drv_register(&indev_drv_3);
    lv_indev_set_group(enc_indev, g);

    /*Set a cursor for the mouse*/
    LV_IMG_DECLARE(mouse_cursor_icon);                  /*Declare the image file.*/
    lv_obj_t *cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);     /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);       /*Connect the image  object to the driver*/
}

/**
 * A task to measure the elapsed time for LVGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void *data)
{
    (void)data;

    while (1) {
        SDL_Delay(5);
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}
