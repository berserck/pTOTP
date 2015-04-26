#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
static TextLayer *text_layer_header;
static int error_code;

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  text_layer = text_layer_create(GRect(bounds.origin.x, bounds.origin.y + 3, bounds.size.w, bounds.size.h));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);

  text_layer_set_text(text_layer, "\n\n\nThere was an error storing your TOTP credentials.\n\nTry reinstalling the app or resetting your Pebble.");

  text_layer_header = text_layer_create(bounds);
  text_layer_set_text_alignment(text_layer_header, GTextAlignmentCenter);

  static char* err_title = "Persist Err -999";
  snprintf(err_title, strlen(err_title), "Persist Err %d", error_code);
  text_layer_set_background_color(text_layer_header, GColorClear);
  text_layer_set_font(text_layer_header, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text(text_layer_header, err_title);

  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  layer_add_child(window_layer, text_layer_get_layer(text_layer_header));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  text_layer_destroy(text_layer_header);
  window_destroy(window);
}

void persist_error_push(int code) {
  error_code = code;
  window = window_create();
  window_set_fullscreen(window, true);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true);
}
