/*
 *
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 *
 */

#include <zephyr/kernel.h>
#include "util.h"

LV_IMG_DECLARE(bolt);

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[], int angle) {

    static lv_color_t cbuf_tmp[CONFIG_DISP_CANVAS * CONFIG_DISP_CANVAS];
    memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));
    lv_image_dsc_t img;
    img.data = (void *)cbuf_tmp;
    img.header.cf = LV_COLOR_FORMAT_ARGB8888;
    img.header.w = CONFIG_DISP_CANVAS;
    img.header.h = CONFIG_DISP_CANVAS;

    lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &img;
    img_dsc.rotation = angle;
    img_dsc.pivot.x = CONFIG_DISP_CANVAS / 2;
    img_dsc.pivot.y = CONFIG_DISP_CANVAS / 2;

    lv_area_t area = {0, 0, CONFIG_DISP_CANVAS - 1, CONFIG_DISP_CANVAS - 1};
    lv_draw_image(&layer, &img_dsc, &area);

    lv_canvas_finish_layer(canvas, &layer);
}

void draw_battery(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_black_dsc;
    init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_area_t a;

    // Outer border
    a = (lv_area_t){0, 2, 28, 13};
    lv_draw_rect(&layer, &rect_white_dsc, &a);
    // Inner fill (black)
    a = (lv_area_t){1, 3, 27, 12};
    lv_draw_rect(&layer, &rect_black_dsc, &a);
    // Charge level
    a = (lv_area_t){2, 4, 2 + (state->battery + 2) / 4 - 1, 11};
    lv_draw_rect(&layer, &rect_white_dsc, &a);
    // Battery nub outer
    a = (lv_area_t){30, 5, 32, 10};
    lv_draw_rect(&layer, &rect_white_dsc, &a);
    // Battery nub inner
    a = (lv_area_t){31, 6, 31, 9};
    lv_draw_rect(&layer, &rect_black_dsc, &a);

    if (state->charging) {
        lv_draw_image_dsc_t img_dsc;
        lv_draw_image_dsc_init(&img_dsc);
        img_dsc.src = &bolt;
        lv_area_t bolt_area = {9, 0, 9 + bolt.header.w - 1, bolt.header.h - 1};
        lv_draw_image(&layer, &img_dsc, &bolt_area);
    }

    lv_canvas_finish_layer(canvas, &layer);
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color, const lv_font_t *font,
                    lv_text_align_t align) {
    lv_draw_label_dsc_init(label_dsc);
    label_dsc->color = color;
    label_dsc->font = font;
    label_dsc->align = align;
}

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
    lv_draw_rect_dsc_init(rect_dsc);
    rect_dsc->bg_color = bg_color;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width) {
    lv_draw_line_dsc_init(line_dsc);
    line_dsc->color = color;
    line_dsc->width = width;
}

void init_arc_dsc(lv_draw_arc_dsc_t *arc_dsc, lv_color_t color, uint8_t width) {
    lv_draw_arc_dsc_init(arc_dsc);
    arc_dsc->color = color;
    arc_dsc->width = width;
}