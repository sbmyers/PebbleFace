#include <pebble.h>

enum { ewk, wwk, esat, wsat, max_Selects };
static Window *window;
static TextLayer *text_layer;
static TextLayer *title;
static TextLayer *clock_layer;
static TextLayer *junk_layer;
static char szTime[10];
static char szJunk[20];

static char *Teams[] = {
    "Boston",
    "Tampa Bay",
    "Montreal",
    "Detroit",
    "Ottawa",
    "Toronto",
    "Florida",
    "Buffalo",
    "Pittsburgh",
    "NY Rangers",
    "Philadelphia",
    "Columbus",
    "Washington",
    "New Jersey",
    "Carolina",
    "NY Islanders",
    "Colorado",
    "St. Louis",
    "Chicago",
    "Minnesota",
    "Nashville",
    "Winnipeg",
    "Anaheim",
    "San Jose",
    "Los Angeles",
    "Phoenix",
    "Vancouver",
    "Calgary",
    "Edmonton",  
};
static time_t ebWeekdayTnP[] = {
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
static time_t ebWeekdayVS[] = {
    21000,  // 5:50
    23460,  // 6:31
    25200,  // 7:00
    26400,  // 7:20
    27900,  // 7:45
    29580,  // 8:13
    30840,  // 8:34
    33660,  // 9:21
    38040,  // 10:34
    41640,  // 11:34
    47040,  // 13:04
    52440,  // 14:34
    59640,  // 16:34
    63240,  // 17:34
    65040,  // 18:04
    66900,  // 18:35
    68640,  // 19:04
    74100,  // 20:35
    81240,  // 22:34
};
static time_t ebSaturdayTnP[] = {
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
static time_t ebSaturdayVS[] = {
    34920,  // 9:42
    40320,  // 11:12
    45720,  // 12:42
    52920,  // 14:42
    58320,  // 16:12
    65520,  // 18:12
    69120,  // 19:12
    76320,  // 21:12
    81720,  // 22:42
};
static time_t wbWeekdayVS[] = {
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
static time_t wbWeekdayTnP[] = {
    24480,  // 6:48
    26340,  // 7:19
    28140,  // 7:49
    29940,  // 8:19
    31740,  // 8:49
    34020,  // 9:27
    37140,  // 10:19
    42540,  // 11:49
    47940,  // 13:19
    53340,  // 14:49
    58740,  // 16:19
    60600,  // 16:50
    62400,  // 17:20
    63660,  // 17:41
    64920,  // 18:02
    66000,  // 18:20
    67740,  // 18:49
    69540,  // 19:19
    72120,  // 20:02
    76740,  // 21:19
    82140,  // 22:49
};
static time_t wbSaturdayVS[] = {
  32100,  // 8:55
  37500,  // 10:25
  44700,  // 12:25
  50100,  // 13:55
  55500,  // 15:25
  60900,  // 16:55
  68100,  // 18:55
  73500,  // 20:25
  78900,  // 21:55
};
static time_t wbSaturdayTnP[] = {
    24420,  // 6:47
    28080,  // 7:48
    35400,  // 9:50
    40800,  // 11:20
    48000,  // 13:20
    53400,  // 14:50
    58800,  // 16:20
    64200,  // 17:50
    71400,  // 19:50
    76800,  // 21:20
    82200,  // 22:50
};

static int nActive = 0;
static unsigned nSelect = ewk;
static int nGame = 0;

static void ShowIt(){
  time_t target = 0;
  time_t arrival = 0;
  switch (nSelect){
  case ewk:
    if (nActive > (int)(sizeof(ebWeekdayTnP) / sizeof(ebWeekdayTnP[0]))) nActive = 0;
    target = ebWeekdayTnP[nActive];
    arrival = ebWeekdayVS[nActive];
    text_layer_set_text(title, "T&P Weekday VS");
    break;
  case esat:
    if (nActive > (int)(sizeof(ebSaturdayTnP) / sizeof(ebSaturdayTnP[0]))) nActive = 0;
    target = ebSaturdayTnP[nActive];
    arrival = ebSaturdayVS[nActive];
    text_layer_set_text(title, "T&P Saturday VS");
    break;
  case wwk:
    if (nActive > (int)(sizeof(wbWeekdayVS) / sizeof(wbWeekdayVS[0]))) nActive = 0;
    target = wbWeekdayVS[nActive];
    arrival = wbWeekdayTnP[nActive];
    text_layer_set_text(title, "VS Weekday T&P");
    break;
  case wsat:
    if (nActive > (int)(sizeof(wbSaturdayVS) / sizeof(wbSaturdayVS[0]))) nActive = 0;
    target = wbSaturdayVS[nActive];
    arrival = wbSaturdayTnP[nActive];
    text_layer_set_text(title, "VS Saturday T&P");
    break;
  }
  struct tm *pTime = localtime(&target);
  static char szTimeOut[30];
  char szFrom[10];
  strftime(szFrom, sizeof(szFrom), "%H:%M", pTime);
  char szTo[10];
  pTime = localtime(&arrival);
  strftime(szTo, sizeof(szTo), "%H:%M", pTime);
  snprintf(szTimeOut, sizeof(szTimeOut), "%s -> (%s)", szFrom, szTo);
  text_layer_set_text(text_layer, szTimeOut);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  nSelect = (nSelect + 1) % max_Selects;
  ShowIt();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Up");
 
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
static void InitClock()
{
  time_t now = time(NULL);
  struct tm *pNow = localtime(&now);
  strftime(szTime,sizeof(szTime),"%H:%M",pNow);
  text_layer_set_text(clock_layer, szTime);

  if(pNow->tm_wday == 6){
    nSelect = esat;
  }
  else{
    nSelect = ewk;    
  }
}
static void ticktock(struct tm *tick_time, TimeUnits units_changed)
{
  strftime(szTime,sizeof(szTime),"%H:%M",tick_time);
  text_layer_set_text(clock_layer, szTime);
}

void tapHandler(AccelAxisType axis, int32_t direction)
{
  snprintf(szJunk, sizeof(szJunk),"%s", Teams[nGame]);
  text_layer_set_text(junk_layer, szJunk);
  nGame = (nGame + 1) % (sizeof(Teams)/sizeof(Teams[0]));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 32 } });
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_background_color(text_layer, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  title = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 25 } });
  text_layer_set_font(title, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(title, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(title));
  
  clock_layer = text_layer_create((GRect) { .origin = { 0, 28 }, .size = { bounds.size.w, 40 } });
  text_layer_set_font(clock_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(clock_layer, GTextAlignmentCenter);
  snprintf(szTime, sizeof(szTime),"00:00");
  text_layer_set_text(clock_layer, szTime);
  layer_add_child(window_layer, text_layer_get_layer(clock_layer));

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 32 } });
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_background_color(text_layer, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  junk_layer = text_layer_create((GRect) { .origin = { 0, 110 }, .size = { bounds.size.w, 30 } });
  text_layer_set_font(junk_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(junk_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(junk_layer));
  
  text_layer_set_text(junk_layer, szJunk);

  InitClock();
  ShowIt();
  tick_timer_service_subscribe(MINUTE_UNIT,ticktock);
  accel_tap_service_subscribe(tapHandler);

  light_enable_interaction();
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);  
  text_layer_destroy(title);
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