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
} games[] = {
    { 1411414200, "St Louis " }, // 9/22/2014 7:30:00 PM St Louis at Dallas
    { 1412019000, "Florida " }, // 9/29/2014 7:30:00 PM Florida at Dallas
    { 1412105400, "Tampa Bay " }, // 9/30/2014 7:30:00 PM Tampa Bay at Dallas
    { 1412883000, "Chicago " }, // 10/9/2014 7:30:00 PM Chicago at Dallas
    { 1413658800, "Philadelphia " }, // 10/18/2014 7:00:00 PM Philadelphia at Dallas
    { 1413919800, "Vancouver " }, // 10/21/2014 7:30:00 PM Vancouver at Dallas
    { 1414524600, "St Louis " }, // 10/28/2014 7:30:00 PM St Louis at Dallas
    { 1414783800, "Anaheim " }, // 10/31/2014 7:30:00 PM Anaheim at Dallas
    { 1415129400, "Los Angeles " }, // 11/4/2014 7:30:00 PM Los Angeles at Dallas
    { 1415302200, "Nashville " }, // 11/6/2014 7:30:00 PM Nashville at Dallas
    { 1415473200, "San Jose " }, // 11/8/2014 7:00:00 PM San Jose at Dallas
    { 1416056400, "Minnesota " }, // 11/15/2014 1:00:00 PM Minnesota at Dallas
    { 1416339000, "Carolina " }, // 11/18/2014 7:30:00 PM Carolina at Dallas
    { 1416511800, "Arizona " }, // 11/20/2014 7:30:00 PM Arizona at Dallas
    { 1416682800, "Los Angeles " }, // 11/22/2014 7:00:00 PM Los Angeles at Dallas
    { 1416943800, "Edmonton " }, // 11/25/2014 7:30:00 PM Edmonton at Dallas
    { 1417203000, "Minnesota " }, // 11/28/2014 7:30:00 PM Minnesota at Dallas
    { 1417888800, "Montreal " }, // 12/6/2014 6:00:00 PM Montreal at Dallas
    { 1418153400, "Winnipeg " }, // 12/9/2014 7:30:00 PM Winnipeg at Dallas
    { 1418499000, "New Jersey " }, // 12/13/2014 7:30:00 PM New Jersey at Dallas
    { 1419357600, "Toronto " }, // 12/23/2014 6:00:00 PM Toronto at Dallas
    { 1419881400, "Ny Rangers " }, // 12/29/2014 7:30:00 PM Ny Rangers at Dallas
    { 1420054200, "Arizona " }, // 12/31/2014 7:30:00 PM Arizona at Dallas
    { 1420311600, "Minnesota " }, // 1/3/2015 7:00:00 PM Minnesota at Dallas
    { 1420572600, "Columbus " }, // 1/6/2015 7:30:00 PM Columbus at Dallas
    { 1421177400, "Ottawa " }, // 1/13/2015 7:30:00 PM Ottawa at Dallas
    { 1421350200, "Winnipeg " }, // 1/15/2015 7:30:00 PM Winnipeg at Dallas
    { 1421521200, "Washington " }, // 1/17/2015 7:00:00 PM Washington at Dallas
    { 1421782200, "Boston " }, // 1/20/2015 7:30:00 PM Boston at Dallas
    { 1422991800, "Colorado " }, // 2/3/2015 7:30:00 PM Colorado at Dallas
    { 1423164600, "Tampa Bay " }, // 2/5/2015 7:30:00 PM Tampa Bay at Dallas
    { 1423855800, "Florida " }, // 2/13/2015 7:30:00 PM Florida at Dallas
    { 1424374200, "San Jose " }, // 2/19/2015 7:30:00 PM San Jose at Dallas
    { 1424545200, "Detroit " }, // 2/21/2015 7:00:00 PM Detroit at Dallas
    { 1425065400, "Colorado " }, // 2/27/2015 7:30:00 PM Colorado at Dallas
    { 1425236400, "Anaheim " }, // 3/1/2015 7:00:00 PM Anaheim at Dallas
    { 1425411000, "Ny Islanders " }, // 3/3/2015 7:30:00 PM Ny Islanders at Dallas
    { 1426438800, "St Louis " }, // 3/15/2015 5:00:00 PM St Louis at Dallas
    { 1426793400, "Pittsburgh " }, // 3/19/2015 7:30:00 PM Pittsburgh at Dallas
    { 1426964400, "Chicago " }, // 3/21/2015 7:00:00 PM Chicago at Dallas
    { 1427139000, "Buffalo " }, // 3/23/2015 7:30:00 PM Buffalo at Dallas
    { 1427743800, "Calgary " }, // 3/30/2015 7:30:00 PM Calgary at Dallas
    { 1428089400, "St Louis " }, // 4/3/2015 7:30:00 PM St Louis at Dallas
    { 1428778800, "Nashville " }, // 4/11/2015 7:00:00 PM Nashville at Dallas
};
static int nGames = (int)(sizeof(games) / sizeof(games[0]));
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
static int nEBWkTnPTrains = (int)(sizeof(ebWeekdayTnP) / sizeof(ebWeekdayTnP)[0]);
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
static int nEBWkVSTrains = (int)(sizeof(ebWeekdayVS) / sizeof(ebWeekdayVS)[0]);
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
static int nEBSatTnPTrains = (int)(sizeof(ebSaturdayTnP) / sizeof(ebSaturdayTnP)[0]);
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
static int nEBSatVSTrains = (int)(sizeof(ebSaturdayVS) / sizeof(ebSaturdayVS)[0]);
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
static int nWBWkVSTrains = (int)(sizeof(wbWeekdayVS) / sizeof(wbWeekdayVS)[0]);
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
static int nWBWkTnPTrains = (int)(sizeof(wbWeekdayTnP) / sizeof(wbWeekdayTnP)[0]);
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
static int nWBSatVSTrains = (int)(sizeof(wbSaturdayVS) / sizeof(wbSaturdayVS)[0]);
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
static int nWBSatTnPTrains = (int)(sizeof(wbSaturdayTnP) / sizeof(wbSaturdayTnP)[0]);

static int nActive = 0;
static unsigned nSelect = ewk;
static int nGame = 0;

static time_t Seconds(struct tm *pTime){
    return 60 * (pTime->tm_hour * 60 + pTime->tm_min);
}
static int GetHour(struct tm *pTime){
    switch (pTime->tm_hour){
    case 0:     return 12;
    case 1:     return 1;
    case 2:     return 2;
    case 3:     return 3;
    case 4:     return 4;
    case 5:     return 5;
    case 6:     return 6;
    case 7:     return 7;
    case 8:     return 8;
    case 9:     return 9;
    case 10:    return 10;
    case 11:    return 11;
    case 12:    return 12;
    case 13:    return 1;
    case 14:    return 2;
    case 15:    return 3;
    case 16:    return 4;
    case 17:    return 5;
    case 18:    return 6;
    case 19:    return 7;
    case 20:    return 8;
    case 21:    return 9;
    case 22:    return 10;
    case 23:    return 11;
    }
    return ((pTime->tm_hour - 1) % 12) + 1;
}
static void ShowIt(){
    time_t target = 0;
    time_t arrival = 0;
    char *arrow = "->";
    switch (nSelect){
    case ewk:
        if (nActive > (int)(sizeof(ebWeekdayTnP) / sizeof(ebWeekdayTnP[0]))) nActive = 0;
        target = ebWeekdayTnP[nActive];
        arrival = ebWeekdayVS[nActive];
        arrow = "->";
        text_layer_set_text(title, "T&P Weekday VS");
        break;
    case esat:
        if (nActive > (int)(sizeof(ebSaturdayTnP) / sizeof(ebSaturdayTnP[0]))) nActive = 0;
        target = ebSaturdayTnP[nActive];
        arrival = ebSaturdayVS[nActive];
        arrow = "->";
        text_layer_set_text(title, "T&P Saturday VS");
        break;
    case wwk:
        if (nActive > (int)(sizeof(wbWeekdayVS) / sizeof(wbWeekdayVS[0]))) nActive = 0;
        target = wbWeekdayVS[nActive];
        arrival = wbWeekdayTnP[nActive];
        arrow = "<-";
        text_layer_set_text(title, "VS Weekday T&P");
        break;
    case wsat:
        if (nActive > (int)(sizeof(wbSaturdayVS) / sizeof(wbSaturdayVS[0]))) nActive = 0;
        target = wbSaturdayVS[nActive];
        arrival = wbSaturdayTnP[nActive];
        arrow = "<-";
        text_layer_set_text(title, "VS Saturday T&P");
        break;
    }
    struct tm *pTime = localtime(&target);
    static char szTimeOut[30];
    char szFrom[10];
    snprintf(szFrom, sizeof(szFrom), "%d:%2.2d", GetHour(pTime), pTime->tm_min);
    char szTo[10];
    pTime = localtime(&arrival);
    snprintf(szTo, sizeof(szTo), "%d:%2.2d", GetHour(pTime), pTime->tm_min);
    snprintf(szTimeOut, sizeof(szTimeOut), "%s %s (%s)", szFrom, arrow, szTo);
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
    strftime(szTime, sizeof(szTime), "%H:%M", pNow);
    text_layer_set_text(clock_layer, szTime);

    if (pNow->tm_wday == 6){
        nSelect = esat;
    }
    else{
        nSelect = ewk;
    }
}
static void ticktock(struct tm *tick_time, TimeUnits units_changed)
{
    snprintf(szTime, sizeof(szTime), "%d:%2.2d %s", GetHour(tick_time), tick_time->tm_min, tick_time->tm_hour > 11 ? "pm" : "am");
    text_layer_set_text(clock_layer, szTime);
}
static void ShowGame()
{
    struct tm *pTime = localtime(&games[nGame].startTime);
    snprintf(szJunk, sizeof(szJunk), "%d/%d %d:%2.2d %s\n%s", pTime->tm_mon + 1, pTime->tm_mday, GetHour(pTime), pTime->tm_min, "pm",
        games[nGame].opponent);
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
    int nEBTrains = 0;
    int nWBTrains = 0;
    time_t *schedule = NULL;
    time_t *arrivals = NULL;
    switch (nSelect)
    {
    case ewk:
        schedule = ebWeekdayTnP;
        arrivals = wbWeekdayVS;
        nEBTrains = nEBWkTnPTrains;
        nWBTrains = nWBWkVSTrains;
        break;
    case esat:
        schedule = ebSaturdayTnP;
        nEBTrains = nEBSatTnPTrains;
        arrivals = wbSaturdayVS;
        nWBTrains = nWBSatVSTrains;
        break;
    }
    for (int i = 0; i < nEBTrains; ++i){
        struct tm test = *localtime(&schedule[i]);
        if ((test.tm_hour >= today.tm_hour) && (test.tm_min > today.tm_min)){
            nActive = i;
            break;
        }
    }
    // at this point we've selected the next east bound train
    // if we're game day, after the puck drops, look for the next westbound train
    struct gameInfo *todaysGame = NULL;
    struct tm test;

    // find a game in the future
    for (int i = nGames - 1; i > 0; --i){
        if (now < games[i].startTime){
            nGame = i;
        }
    }
    for (int i = 0; i < nGames; ++i){
        test = *localtime(&games[i].startTime);
        if ((today.tm_mon == test.tm_mon) && (today.tm_mday == test.tm_mday)){
            todaysGame = &games[i];
            nGame = i;
            break;
        }
    }
    if (todaysGame){
        // Game day!
        // check to see if it's before the puck drops
        time_t nSecsToday = Seconds(&today);
        if (nSecsToday < Seconds(&test)){
            for (int i = 0; i < nEBTrains; ++i){
                if (nSecsToday < schedule[i]){
                    nActive = i;
                    break;
                }
            }
        }
        else{
            // get here after the puck drops
            // find the next west bound train
            for (int i = 0; i < nWBTrains; ++i){
                if (nSecsToday < arrivals[i]){
                    nSelect++;  // change from east bound to westbound
                    nActive = i;
                    break;
                }
            }
        }
    }
    else{
        // no game today, show next train    
        time_t nSecsToday = Seconds(&today);
        for (int i = 0; i < nEBTrains; ++i){
            if (nSecsToday < schedule[i]){
                nActive = i;
                break;
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
    snprintf(szTime, sizeof(szTime), "00:00");
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
    tick_timer_service_subscribe(MINUTE_UNIT, ticktock);
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