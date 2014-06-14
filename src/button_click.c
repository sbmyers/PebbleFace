#include <pebble.h>

enum { ewk, esat, wwk, wsat, max_Selects };
static Window *window;
static TextLayer *text_layer;

static time_t eastboundWeekday[] = {
	18000,  // 5:00
	20340,  // 5:39
	22140,  // 6:09
	23340,  // 6:29
	24600,  // 6:50
	26400,  // 7:20
	27720,  // 7:42
	30540,  // 8:29
	34920,  // 9:42
	38520,  // 10:42
	43920,  // 12:12
	49320,  // 13:42
	56520,  // 15:42
	60120,  // 16:42
	61920,  // 17:12
	63720,  // 17:42
	65520,  // 18:12
	70920,  // 19:42
	78120,  // 21:42  
};
static time_t eastboundSaturday[] = {
	31800,  // 8:50
	37200,  // 10:20
	42600,  // 11:50
	49800,  // 13:50
	55200,  // 15:20
	62400,  // 17:20
	66000,  // 18:20
	73200,  // 20:20
	78600,  // 21:50
};
static time_t westboundWeekday[] = {
	21240,  // 5:54
	23100,  // 6:25
	24840,  // 6:54
	26640,  // 7:24
	28140,  // 7:49
	29400,  // 8:10
	30780,  // 8:33
	33900,  // 9:25
	35700,  // 9:55
	39300,  // 10:55
	44700,  // 12:25
	50100,  // 13:55
	55500,  // 15:25
	57300,  // 15:55
	59100,  // 16:25
	60300,  // 16:45
	61740,  // 17:09
	62640,  // 17:24
	64500,  // 17:55
	66300,  // 18:25
	68940,  // 19:09
	73500,  // 20:25
	78900,  // 21:55
};
static time_t westboundSaturday[] = {
	32100,  // 8:55
	37500,  // 10:25
	44700,  // 12:25
	50100,  // 13:55
	55500,  // 15:25
	60900,  // 16:55
	68100,  // 18:55
	73500,  // 20:25
	78900,  // 21:55
	83460,  // 23:11
};

static int nActive = 0;
static unsigned nSelect = ewk;

static void ShowIt(){
  time_t target = 0;
	switch (nSelect){
	case ewk:
		if (nActive > (int)(sizeof(eastboundWeekday) / sizeof(eastboundWeekday[0]))) nActive = 0;
		target = eastboundWeekday[nActive];
		break;
	case esat:
		if (nActive > (int)(sizeof(eastboundSaturday) / sizeof(eastboundSaturday[0]))) nActive = 0;
		target = eastboundSaturday[nActive];
		//text_layer_set_text(text_layer, eastboundSaturday[nActive]);
		break;
	case wwk:
		if (nActive > (int)(sizeof(westboundWeekday) / sizeof(westboundWeekday[0]))) nActive = 0;
		target = westboundWeekday[nActive];
		//text_layer_set_text(text_layer, westboundWeekday[nActive]);
		break;
	case wsat:
		if (nActive > (int)(sizeof(westboundSaturday) / sizeof(westboundSaturday[0]))) nActive = 0;
		target = westboundSaturday[nActive];
		//text_layer_set_text(text_layer, westboundSaturday[nActive]);
		break;
	}
  struct tm *pTime = localtime(&target);
  static char szTimeOut[20];
  strftime(szTimeOut, sizeof(szTimeOut), "%H:%M", pTime);
	text_layer_set_text(text_layer,szTimeOut);
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
}

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 40 } });
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
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