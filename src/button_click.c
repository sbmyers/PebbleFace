#include <pebble.h>

enum { ewk, esat, wwk, wsat, max_Selects };
static Window *window;
static TextLayer *text_layer;
#if 01
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
#else
//struct tm {
//	int tm_sec;
//	int tm_min;
//	int tm_hour;
//	int tm_mday;
//	int tm_mon;
//	int tm_year;
//	int tm_wday;
//	int tm_yday;
//	int tm_isdst;
//};

static struct tm eastboundWeekday[] = {
	{ 0, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0 },      // 5:00
	{ 0, 5, 39, 1, 0, 0, 0, 0, 0, 0, 0 },     // 5:39
	{ 0, 6, 9, 1, 0, 0, 0, 0, 0, 0, 0 },      // 6:09
	{ 0, 6, 29, 1, 0, 0, 0, 0, 0, 0, 0 },     // 6:29
	{ 0, 6, 50, 1, 0, 0, 0, 0, 0, 0, 0 },     // 6:50
	{ 0, 7, 20, 1, 0, 0, 0, 0, 0, 0, 0 },     // 7:20
	{ 0, 7, 42, 1, 0, 0, 0, 0, 0, 0, 0 },     // 7:42
	{ 0, 8, 29, 1, 0, 0, 0, 0, 0, 0, 0 },     // 8:29
	{ 0, 9, 42, 1, 0, 0, 0, 0, 0, 0, 0 },     // 9:42
	{ 0, 10, 42, 1, 0, 0, 0, 0, 0, 0, 0 },    // 10:42
	{ 0, 12, 12, 1, 0, 0, 0, 0, 0, 0, 0 },    // 12:12
	{ 0, 13, 42, 1, 0, 0, 0, 0, 0, 0, 0 },    // 13:42
	{ 0, 15, 42, 1, 0, 0, 0, 0, 0, 0, 0 },    // 15:42
	{ 0, 16, 42, 1, 0, 0, 0, 0, 0, 0, 0 },    // 16:42
	{ 0, 17, 12, 1, 0, 0, 0, 0, 0, 0, 0 },    // 17:12
	{ 0, 17, 42, 1, 0, 0, 0, 0, 0, 0, 0 },    // 17:42
	{ 0, 18, 12, 1, 0, 0, 0, 0, 0, 0, 0 },    // 18:12
	{ 0, 19, 42, 1, 0, 0, 0, 0, 0, 0, 0 },    // 19:42
	{ 0, 21, 42, 1, 0, 0, 0, 0, 0, 0, 0 },    // 21:42
};
static struct tm eastboundSaturday[] = {
	{ 0, 8, 50, 1, 0, 0, 0, 0, 0, 0, 0 },     // 8:50
	{ 0, 10, 20, 1, 0, 0, 0, 0, 0, 0, 0 },    // 10:20
	{ 0, 11, 50, 1, 0, 0, 0, 0, 0, 0, 0 },    // 11:50
	{ 0, 13, 50, 1, 0, 0, 0, 0, 0, 0, 0 },    // 13:50
	{ 0, 15, 20, 1, 0, 0, 0, 0, 0, 0, 0 },    // 15:20
	{ 0, 17, 20, 1, 0, 0, 0, 0, 0, 0, 0 },    // 17:20
	{ 0, 18, 20, 1, 0, 0, 0, 0, 0, 0, 0 },    // 18:20
	{ 0, 20, 20, 1, 0, 0, 0, 0, 0, 0, 0 },    // 20:20
	{ 0, 21, 50, 1, 0, 0, 0, 0, 0, 0, 0 },    // 21:50
};
static struct tm westboundWeekday[] = {
	{ 0, 5, 54, 1, 0, 0, 0, 0, 0, 0, 0 },     // 5:54
	{ 0, 6, 25, 1, 0, 0, 0, 0, 0, 0, 0 },     // 6:25
	{ 0, 6, 54, 1, 0, 0, 0, 0, 0, 0, 0 },     // 6:54
	{ 0, 7, 24, 1, 0, 0, 0, 0, 0, 0, 0 },     // 7:24
	{ 0, 7, 49, 1, 0, 0, 0, 0, 0, 0, 0 },     // 7:49
	{ 0, 8, 10, 1, 0, 0, 0, 0, 0, 0, 0 },     // 8:10
	{ 0, 8, 33, 1, 0, 0, 0, 0, 0, 0, 0 },     // 8:33
	{ 0, 9, 25, 1, 0, 0, 0, 0, 0, 0, 0 },     // 9:25
	{ 0, 9, 55, 1, 0, 0, 0, 0, 0, 0, 0 },     // 9:55
	{ 0, 10, 55, 1, 0, 0, 0, 0, 0, 0, 0 },    // 10:55
	{ 0, 12, 25, 1, 0, 0, 0, 0, 0, 0, 0 },    // 12:25
	{ 0, 13, 55, 1, 0, 0, 0, 0, 0, 0, 0 },    // 13:55
	{ 0, 15, 25, 1, 0, 0, 0, 0, 0, 0, 0 },    // 15:25
	{ 0, 15, 55, 1, 0, 0, 0, 0, 0, 0, 0 },    // 15:55
	{ 0, 16, 25, 1, 0, 0, 0, 0, 0, 0, 0 },    // 16:25
	{ 0, 16, 45, 1, 0, 0, 0, 0, 0, 0, 0 },    // 16:45
	{ 0, 17, 9, 1, 0, 0, 0, 0, 0, 0, 0 },     // 17:09
	{ 0, 17, 24, 1, 0, 0, 0, 0, 0, 0, 0 },    // 17:24
	{ 0, 17, 55, 1, 0, 0, 0, 0, 0, 0, 0 },    // 17:55
	{ 0, 18, 25, 1, 0, 0, 0, 0, 0, 0, 0 },    // 18:25
	{ 0, 19, 9, 1, 0, 0, 0, 0, 0, 0, 0 },     // 19:09
	{ 0, 20, 25, 1, 0, 0, 0, 0, 0, 0, 0 },    // 20:25
	{ 0, 21, 55, 1, 0, 0, 0, 0, 0, 0, 0 },    // 21:55
};
static struct tm westboundSaturday[] = {
	{ 0, 8, 55, 1, 0, 0, 0, 0, 0, 0, 0 },     // 8:55
	{ 0, 10, 25, 1, 0, 0, 0, 0, 0, 0, 0 },    // 10:25
	{ 0, 12, 25, 1, 0, 0, 0, 0, 0, 0, 0 },    // 12:25
	{ 0, 13, 55, 1, 0, 0, 0, 0, 0, 0, 0 },    // 13:55
	{ 0, 15, 25, 1, 0, 0, 0, 0, 0, 0, 0 },    // 15:25
	{ 0, 16, 55, 1, 0, 0, 0, 0, 0, 0, 0 },    // 16:55
	{ 0, 18, 55, 1, 0, 0, 0, 0, 0, 0, 0 },    // 18:55
	{ 0, 20, 25, 1, 0, 0, 0, 0, 0, 0, 0 },    // 20:25
	{ 0, 21, 55, 1, 0, 0, 0, 0, 0, 0, 0 },    // 21:55
	{ 0, 23, 11, 1, 0, 0, 0, 0, 0, 0, 0 },    // 23:11
};
#endif
//static char *eastboundWeekday[] = {
//  "=> 5:00", "=> 5:39", "=> 6:09", "=> 6:29", "=> 6:50", "=> 7:20", "=> 7:42", "=> 8:29", "=> 9:42", "=> 10:42",
//  "=> 12:12", "=> 13:42", "=> 15:42", "=> 16:42", "=> 17:12", "=> 17:42", "=> 18:12", "=> 19:42", "=> 21:42",
//};
//static char *eastboundSaturday[] = {
//	">> 8:50", ">> 10:20", ">> 11:50", ">> 1:50", ">> 15:20", ">> 17:20", ">> 18:20", ">> 20:20", ">> 21:50",
//};
//
//static char *westboundWeekday[] = {
//	"<- 5:00", "<- 5:39", "<- 6:09", "<- 6:29", "<- 6:50", "<- 7:20", "<- 7:42", "<- 8:29", "<- 9:42", "<- 10:42",
//	"<- 12:12", "<- 13:42", "<- 15:42", "<- 16:42", "<- 17:12", "<- 17:42", "<- 18:12", "<- 19:42", "<- 21:42",
//};
//static char *westboundSaturday[] = {
//	"<< 5:00", "<< 5:39", "<< 6:09", "<< 6:29", "<< 6:50", "<< 7:20", "<< 7:42", "<< 8:29", "<< 9:42", "<< 10:42",
//	"<< 12:12", "<< 13:42", "<< 15:42", "<< 16:42", "<< 17:12", "<< 17:42", "<< 18:12", "<< 19:42", "<< 21:42",
//};

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
  static char szTimeOut[20]={"abcdefghijklmnop"};
  strftime(szTimeOut, sizeof(szTimeOut), "%H:%M", pTime);
  //snprintf(szTimeOut, sizeof(szTimeOut), "%d:%d:%d",pTime.tm_hour, pTime.tm_min, pTime.tm_sec);
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
	//text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
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