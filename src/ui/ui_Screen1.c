// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: lulu2_menu

#include "ui.h"

void ui_Screen1_screen_init(void)
{
ui_Screen1 = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Screen1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ButtonAction1 = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonAction1, 120);
lv_obj_set_height( ui_ButtonAction1, 100);
lv_obj_set_x( ui_ButtonAction1, 4 );
lv_obj_set_y( ui_ButtonAction1, 4 );
lv_obj_set_align( ui_ButtonAction1, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonAction1, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonAction1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonAction1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonAction1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ButtonAction1, &ui_img_d4_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_color(ui_ButtonAction1, lv_color_hex(0xC0C0C0), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_ButtonAction1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_ButtonAction1, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_color(ui_ButtonAction1, lv_color_hex(0x636363), LV_PART_MAIN | LV_STATE_PRESSED );
lv_obj_set_style_shadow_opa(ui_ButtonAction1, 255, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_width(ui_ButtonAction1, 2, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_spread(ui_ButtonAction1, 2, LV_PART_MAIN| LV_STATE_PRESSED);

ui_ButtonMenu = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonMenu, 90);
lv_obj_set_height( ui_ButtonMenu, 57);
lv_obj_set_x( ui_ButtonMenu, 88 );
lv_obj_set_y( ui_ButtonMenu, -90 );
lv_obj_set_align( ui_ButtonMenu, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonMenu, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonMenu, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonMenu, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonMenu, 55, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_outline_color(ui_ButtonMenu, lv_color_hex(0x675F5F), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_outline_opa(ui_ButtonMenu, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_color(ui_ButtonMenu, lv_color_hex(0x636363), LV_PART_MAIN | LV_STATE_PRESSED );
lv_obj_set_style_shadow_opa(ui_ButtonMenu, 255, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_width(ui_ButtonMenu, 2, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_spread(ui_ButtonMenu, 2, LV_PART_MAIN| LV_STATE_PRESSED);

ui_thredotsLabel = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_thredotsLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_thredotsLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_thredotsLabel, 90 );
lv_obj_set_y( ui_thredotsLabel, -91 );
lv_obj_set_align( ui_thredotsLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_thredotsLabel,"MENU");

ui_ButtonAction3 = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonAction3, 70);
lv_obj_set_height( ui_ButtonAction3, 70);
lv_obj_set_x( ui_ButtonAction3, 105 );
lv_obj_set_y( ui_ButtonAction3, 3 );
lv_obj_set_align( ui_ButtonAction3, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonAction3, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonAction3, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonAction3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonAction3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ButtonAction3, &ui_img_d2_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_color(ui_ButtonAction3, lv_color_hex(0xC0C0C0), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_ButtonAction3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_ButtonAction3, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_outline_color(ui_ButtonAction3, lv_color_hex(0x675F5F), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_outline_opa(ui_ButtonAction3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_color(ui_ButtonAction3, lv_color_hex(0x636363), LV_PART_MAIN | LV_STATE_PRESSED );
lv_obj_set_style_shadow_opa(ui_ButtonAction3, 255, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_width(ui_ButtonAction3, 2, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_spread(ui_ButtonAction3, 2, LV_PART_MAIN| LV_STATE_PRESSED);

ui_ButtonSleep = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonSleep, 100);
lv_obj_set_height( ui_ButtonSleep, 54);
lv_obj_set_x( ui_ButtonSleep, 86 );
lv_obj_set_y( ui_ButtonSleep, 88 );
lv_obj_set_align( ui_ButtonSleep, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonSleep, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonSleep, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonSleep, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonSleep, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ButtonSleep, &ui_img_sleep_dog_100_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_ButtonSleep, lv_color_hex(0x636363), LV_PART_MAIN | LV_STATE_PRESSED );
lv_obj_set_style_shadow_opa(ui_ButtonSleep, 255, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_width(ui_ButtonSleep, 2, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_spread(ui_ButtonSleep, 2, LV_PART_MAIN| LV_STATE_PRESSED);

ui_ButtonAction2 = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonAction2, 70);
lv_obj_set_height( ui_ButtonAction2, 70);
lv_obj_set_x( ui_ButtonAction2, -103 );
lv_obj_set_y( ui_ButtonAction2, 4 );
lv_obj_set_align( ui_ButtonAction2, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonAction2, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonAction2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonAction2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonAction2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ButtonAction2, &ui_img_d1_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_color(ui_ButtonAction2, lv_color_hex(0xC0C0C0), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_ButtonAction2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_ButtonAction2, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_outline_color(ui_ButtonAction2, lv_color_hex(0x675F5F), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_outline_opa(ui_ButtonAction2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_color(ui_ButtonAction2, lv_color_hex(0x636363), LV_PART_MAIN | LV_STATE_CHECKED );
lv_obj_set_style_shadow_opa(ui_ButtonAction2, 255, LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_shadow_width(ui_ButtonAction2, 1, LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_shadow_spread(ui_ButtonAction2, 1, LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_shadow_color(ui_ButtonAction2, lv_color_hex(0x636363), LV_PART_MAIN | LV_STATE_PRESSED );
lv_obj_set_style_shadow_opa(ui_ButtonAction2, 255, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_width(ui_ButtonAction2, 2, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_spread(ui_ButtonAction2, 2, LV_PART_MAIN| LV_STATE_PRESSED);

ui_ButtonBack = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_ButtonBack, 90);
lv_obj_set_height( ui_ButtonBack, 78);
lv_obj_set_x( ui_ButtonBack, -93 );
lv_obj_set_y( ui_ButtonBack, -80 );
lv_obj_set_align( ui_ButtonBack, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_ButtonBack, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_ButtonBack, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ButtonBack, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ButtonBack, 55, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_ButtonBack, &ui_img_back48_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_color(ui_ButtonBack, lv_color_hex(0x636363), LV_PART_MAIN | LV_STATE_PRESSED );
lv_obj_set_style_shadow_opa(ui_ButtonBack, 255, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_width(ui_ButtonBack, 2, LV_PART_MAIN| LV_STATE_PRESSED);
lv_obj_set_style_shadow_spread(ui_ButtonBack, 2, LV_PART_MAIN| LV_STATE_PRESSED);

lv_obj_add_event_cb(ui_ButtonMenu, ui_event_ButtonMenu, LV_EVENT_ALL, NULL);

}
