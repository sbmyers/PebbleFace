#include <pebble.h>

enum { ewk, esat, wwk, wsat, max_Selects };
static Window *window;
static TextLayer *text_layer;
static char *eastboundWeekday[] = {
  "=> 5:00", "=> 5:39", "=> 6:09", "=> 6:29", "=> 6:50", "=> 7:20", "=> 7:42", "=> 8:29", "=> 9:42", "=> 10:42",
  "=> 12:12", "=> 13:42", "=> 15:42", "=> 16:42", "=> 17:12", "=> 17:42", "=> 18:12", "=> 19:42", "=> 21:42",
};
static char *eastboundSaturday[] = {
  ">> 8:50", ">> 10:20", ">> 11:50", ">> 1:50", ">> 15:20", ">> 17:20", ">> 18:20", ">> 20:20", ">> 21:50",
};

static char *westboundWeekday[] = {
  "<- 5:00", "<- 5:39", "<- 6:09", "<- 6:29", "<- 6:50", "<- 7:20", "<- 7:42", "<- 8:29", "<- 9:42", "<- 10:42",
  "<- 12:12", "<- 13:42", "<- 15:42", "<- 16:42", "<- 17:12", "<- 17:42", "<- 18:12", "<- 19:42", "<- 21:42",
};
static char *westboundSaturday[] = {
  "<< 5:00", "<< 5:39", "<< 6:09", "<< 6:29", "<< 6:50", "<< 7:20", "<< 7:42", "<< 8:29", "<< 9:42", "<< 10:42",
  "<< 12:12", "<< 13:42", "<< 15:42", "<< 16:42", "<< 17:12", "<< 17:42", "<< 18:12", "<< 19:42", "<< 21:42",
};

static int nActive = 0;
static unsigned nSelect = ewk;

static void ShowIt(){
  switch (nSelect){
  case ewk:
    if (nActive > (int)(sizeof(eastboundWeekday) / sizeof(eastboundWeekday[0]))) nActive = 0;
    text_layer_set_text(text_layer, eastboundWeekday[nActive]);
    break;
  case esat:
    if (nActive > (int)(sizeof(eastboundSaturday) / sizeof(eastboundSaturday[0]))) nActive = 0;
    text_layer_set_text(text_layer, eastboundSaturday[nActive]);
    break;
  case wwk:
    if (nActive > (int)(sizeof(westboundWeekday) / sizeof(westboundWeekday[0]))) nActive = 0;
    text_layer_set_text(text_layer, westboundWeekday[nActive]);
    break;
  case wsat:
    if (nActive > (int)(sizeof(westboundSaturday) / sizeof(westboundSaturday[0]))) nActive = 0;
    text_layer_set_text(text_layer, westboundSaturday[nActive]);
    break;
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  nSelect = (nSelect + 1) % max_Selects;
  ShowIt();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  ++nActive;
  ShowIt();
  text_layer_set_text(text_layer, eastboundWeekday[nActive]);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 40} });
  text_layer_set_font(text_layer, fonts_get_system_font( FONT_KEY_GOTHIC_24_BOLD));
  ShowIt();
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
      .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}