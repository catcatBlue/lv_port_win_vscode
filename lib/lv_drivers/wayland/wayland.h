/**
 * @file wayland
 *
 */

#ifndef WAYLAND_H
#define WAYLAND_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../config/lv_drv_conf.h"
#endif
#endif

#if USE_WAYLAND

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void wayland_init(void);
void wayland_deinit(void);
void wayland_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
bool wayland_pointer_read(lv_indev_drv_t * drv, lv_indev_data_t * data);
bool wayland_pointeraxis_read(lv_indev_drv_t * drv, lv_indev_data_t * data);
bool wayland_keyboard_read(lv_indev_drv_t * drv, lv_indev_data_t * data);
bool wayland_touch_read(lv_indev_drv_t * drv, lv_indev_data_t * data);

/**********************
 *      MACROS
 **********************/

#endif /* USE_WAYLAND */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WAYLAND_H */
