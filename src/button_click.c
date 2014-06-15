#include <pebble.h>

enum { ewk, wwk, esat, wsat, max_Selects };
static Window *window;
static TextLayer *text_layer;
static TextLayer *title;
static TextLayer *clock_layer;
static TextLayer *junk_layer;
static char szTime[10];
static char szJunk[50];
static struct gameInfo{
  time_t startTime;
  char *opponent;
} games[] ={
  {1379268000, "St Louis "}, // 9/15/2013 6:00:00 PM St Louis 6 - Dallas 5
  {1379532600, "Florida "}, // 9/18/2013 7:30:00 PM Florida 2 - Dallas 3
  {1380223800, "Colorado "}, // 9/26/2013 7:30:00 PM Colorado 1 - Dallas 5
  {1380828600, "Florida "}, // 10/3/2013 7:30:00 PM Florida 4 - Dallas 2
  {1380999600, "Washington "}, // 10/5/2013 7:00:00 PM Washington 1 - Dallas 2
  {1382038200, "San Jose "}, // 10/17/2013 7:30:00 PM San Jose 3 - Dallas 4
  {1382643000, "Calgary "}, // 10/24/2013 7:30:00 PM Calgary 1 - Dallas 5
  {1382810400, "Winnipeg "}, // 10/26/2013 6:00:00 PM Winnipeg 2 - Dallas 1
  {1383334200, "Colorado "}, // 11/1/2013 7:30:00 PM Colorado 3 - Dallas 2
  {1384023600, "Chicago "}, // 11/9/2013 7:00:00 PM Chicago 5 - Dallas 2
  {1385062200, "Ny Rangers "}, // 11/21/2013 7:30:00 PM Ny Rangers 3 - Dallas 2
  {1385492400, "Anaheim "}, // 11/26/2013 7:00:00 PM Anaheim 3 - Dallas 6
  {1385753400, "Chicago "}, // 11/29/2013 7:30:00 PM Chicago 2 - Dallas 1
  {1385917200, "Edmonton "}, // 12/1/2013 5:00:00 PM Edmonton 3 - Dallas 2
  {1386421200, "Philadelphia "}, // 12/7/2013 1:00:00 PM Philadelphia 1 - Dallas 5
  {1386703800, "Chicago "}, // 12/10/2013 7:30:00 PM Chicago 6 - Dallas 2
  {1387308600, "Colorado "}, // 12/17/2013 7:30:00 PM Colorado 2 - Dallas 3
  {1387481400, "Vancouver "}, // 12/19/2013 7:30:00 PM Vancouver 1 - Dallas 4
  {1388172600, "Nashville "}, // 12/27/2013 7:30:00 PM Nashville 1 - Dallas 4
  {1388336400, "St Louis "}, // 12/29/2013 5:00:00 PM St Louis 3 - Dallas 2
  {1388516400, "Los Angeles "}, // 12/31/2013 7:00:00 PM Los Angeles 2 - Dallas 3
  {1388691000, "Montreal "}, // 1/2/2014 7:30:00 PM Montreal 6 - Dallas 4
  {1388862000, "Detroit "}, // 1/4/2014 7:00:00 PM Detroit 5 - Dallas 1
  {1389546000, "Ny Islanders "}, // 1/12/2014 5:00:00 PM Ny Islanders 4 - Dallas 2
  {1389727800, "Edmonton "}, // 1/14/2014 7:30:00 PM Edmonton 2 - Dallas 5
  {1389900600, "Boston "}, // 1/16/2014 7:30:00 PM Boston 4 - Dallas 2
  {1390330800, "Minnesota "}, // 1/21/2014 7:00:00 PM Minnesota 0 - Dallas 4
  {1390503600, "Toronto "}, // 1/23/2014 7:00:00 PM Toronto 1 - Dallas 7
  {1390676400, "Pittsburgh "}, // 1/25/2014 7:00:00 PM Pittsburgh 0 - Dallas 3
  {1390851000, "Colorado "}, // 1/27/2014 7:30:00 PM Colorado 4 - Dallas 3
  {1391110200, "New Jersey "}, // 1/30/2014 7:30:00 PM New Jersey 3 - Dallas 2
  {1391886000, "Phoenix "}, // 2/8/2014 7:00:00 PM Phoenix 1 - Dallas 2
  {1393529400, "Carolina "}, // 2/27/2014 7:30:00 PM Carolina 1 - Dallas 4
  {1393682400, "Tampa Bay "}, // 3/1/2014 2:00:00 PM Tampa Bay 4 - Dallas 2
  {1393873200, "Buffalo "}, // 3/3/2014 7:00:00 PM Buffalo 2 - Dallas 3
  {1394134200, "Vancouver "}, // 3/6/2014 7:30:00 PM Vancouver 1 - Dallas 6
  {1394307000, "Minnesota "}, // 3/8/2014 7:30:00 PM Minnesota 3 - Dallas 4
  {1394825400, "Calgary "}, // 3/14/2014 7:30:00 PM Calgary 4 - Dallas 3
  {1395496800, "Ottawa "}, // 3/22/2014 2:00:00 PM Ottawa 1 - Dallas 3
  {1395689400, "Winnipeg "}, // 3/24/2014 7:30:00 PM Winnipeg 1 - Dallas 2
  {1396035000, "Nashville "}, // 3/28/2014 7:30:00 PM Nashville 3 - Dallas 7
  {1396985400, "Nashville "}, // 4/8/2014 7:30:00 PM Nashville 2 - Dallas 3
  {1397071800, "Columbus "}, // 4/9/2014 7:30:00 PM Columbus 3 - Dallas 1
  {1397244600, "St Louis "}, // 4/11/2014 7:30:00 PM St Louis 0 - Dallas 3
  {1398112200, "Anaheim "}, // 4/21/2014 8:30:00 PM Anaheim 0 - Dallas 3
  {1398279600, "Anaheim "}, // 4/23/2014 7:00:00 PM Anaheim 2 - Dallas 4
  {1398625200, "Anaheim "}, // 4/27/2014 7:00:00 PM Anaheim 5 - Dallas 4
};
static int nGames = (int)(sizeof(games)/sizeof(games[0]));
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
static int nEBWkTnPTrains = (int)(sizeof(ebWeekdayTnP)/sizeof(ebWeekdayTnP)[0]);
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
static int nEBWkVSTrains = (int)(sizeof(ebWeekdayVS)/sizeof(ebWeekdayVS)[0]);
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
static int nEBSatTnPTrains = (int)(sizeof(ebSaturdayTnP)/sizeof(ebSaturdayTnP)[0]);
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
static int nEBSatVSTrains = (int)(sizeof(ebSaturdayVS)/sizeof(ebSaturdayVS)[0]);
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
static int nWBWkVSTrains = (int)(sizeof(wbWeekdayVS)/sizeof(wbWeekdayVS)[0]);
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
static int nWBWkTnPTrains = (int)(sizeof(wbWeekdayTnP)/sizeof(wbWeekdayTnP)[0]);
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
static int nWBSatVSTrains = (int)(sizeof(wbSaturdayVS)/sizeof(wbSaturdayVS)[0]);
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
static int nWBSatTnPTrains = (int)(sizeof(wbSaturdayTnP)/sizeof(wbSaturdayTnP)[0]);

static int nActive = 0;
static unsigned nSelect = ewk;
static int nGame = 0;

static time_t Seconds(struct tm *pTime){
  return 60 * (pTime->tm_hour * 60 + pTime->tm_min);
}
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
static void ShowGame()
{
  struct tm *pTime = localtime(&games[nGame].startTime);
  //snprintf(szJunk, sizeof(szJunk),"%s", games[nGame].opponent);
  char szTemp[30];
  strftime(szTemp, sizeof(szTemp),"%m/%d %H:%M",pTime);
  snprintf(szJunk,sizeof(szJunk),"%s\n%s", szTemp, games[nGame].opponent);
  text_layer_set_text(junk_layer, szJunk);
}
void tapHandler(AccelAxisType axis, int32_t direction)
{
  nGame = (nGame + 1) % nGames;
  ShowGame();
}
static void CheckForGameDay()
{
  time_t now = time(NULL);
  struct tm today = *localtime(&now);
  // InitClock has set things to eastbound,
  // set to the next departure from TnP
  int nTrains = 0;
  time_t *schedule = NULL;
  switch(nSelect)
  {
    case ewk:
      schedule = ebWeekdayTnP;
      nTrains = nEBWkTnPTrains;
      break;
    case esat:
      schedule = ebSaturdayTnP;
      nTrains = nEBSatTnPTrains;
      break;
  }
  for(int i = 0; i < nTrains; ++i){
    struct tm test = *localtime(&schedule[i]);
    if((test.tm_hour >= today.tm_hour) && (test.tm_min > today.tm_min)){
      nActive = i;
      break;
    }    
  }
  // at this point we've selected the next east bound train
  // if we're game day, after the puck drops, look for the next westbound train
  struct gameInfo *todaysGame = NULL;
  struct tm test;
  for(int i = 0; i < nGames; ++i){
    test = *localtime(&games[i].startTime);
    if((today.tm_mon == test.tm_mon) && (today.tm_mday == test.tm_mday)){
      todaysGame = &games[i];
      nGame = i;
      break;
    }
  }
  if(todaysGame){
    // Game day!
    // check to see if it's before the puck drops
    time_t nSecsToday = Seconds(&today);
    if(nSecsToday < Seconds(&test)){
      for(int i = 0; i < nTrains; ++i){
        if(nSecsToday < schedule[i]){
          nActive = i;
          break;
        }
        
      }
    }
    else{
      if(true){
        
      }
      
    }
    
  }
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

  title = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 26 } });
  text_layer_set_font(title, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(title, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(title));
  
  clock_layer = text_layer_create((GRect) { .origin = { 0, 26 }, .size = { bounds.size.w, 32 } });
  text_layer_set_font(clock_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(clock_layer, GTextAlignmentCenter);
  snprintf(szTime, sizeof(szTime),"00:00");
  text_layer_set_text(clock_layer, szTime);
  layer_add_child(window_layer, text_layer_get_layer(clock_layer));

  text_layer = text_layer_create((GRect) { .origin = { 0, 60 }, .size = { bounds.size.w, 32 } });
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_background_color(text_layer, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  junk_layer = text_layer_create((GRect) { .origin = { 0, 90 }, .size = { bounds.size.w, 50 } });
  text_layer_set_font(junk_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(junk_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(junk_layer));
  
  text_layer_set_text(junk_layer, szJunk);

  InitClock();
  CheckForGameDay();
  ShowIt();
  ShowGame();
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