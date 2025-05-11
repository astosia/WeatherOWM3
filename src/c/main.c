#include <pebble.h>
#include "main.h"
#include "weekday.h"
#include <pebble-fctx/fctx.h>
#include <pebble-fctx/fpath.h>
#include <pebble-fctx/ffont.h>

#define ROUND_VERTICAL_PADDING 15
#define ROTATION_SETTING_DEFAULT 0
#define ROTATION_SETTING_LEFT    0
#define ROTATION_SETTING_RIGHT   1

//Static and initial vars
static GFont
FontDate,
FontTemp,
FontTempFore,
FontIcon2
;

FFont* time_font;
FFont* weather_font;

static Window * s_window;

static Layer * s_canvas_background;
static Layer * s_canvas;
static Layer * s_canvas_bt_icon;
static Layer * s_canvas_qt_icon;
static Layer * s_canvas_weather; //weather layer
//static Layer  *s_background_picture;

Layer * time_area_layer;
Layer * weather_area_layer;

static int s_hours, s_minutes, s_weekday, s_day, s_month;

static char* weather_conditions[] = {
  "\U0000F07B", // 'unknown': 0,
  "\U0000F00D", // 'clear': 1,
  "\U0000F00C", // 'fewclouds': 2,
  "\U0000F041", // 'scattered clouds': 3,
  "\U0000F013", // 'brokenclouds': 4,
  "\U0000F019", // 'shower rain': 5,
  "\U0000F008", // 'rain': 6,
  "\U0000F076", // 'snow': 7,
  "\U0000F016", // 'tstorms': 8,
  "\U0000F021", // 'mist': 9,
  "\U0000F02E", // 'nt_clear': 10,
  "\U0000F081", // 'nt_few clouds': 11,
  "\U0000F086", // 'nt_scattered clouds': 12,
  "\U0000F013", // 'nt_broken clouds' : 13
  "\U0000F019", // 'nt_shower rain': 14,
  "\U0000F036", // 'nt_rain': 15,
  "\U0000F076", // 'nt_snow': 16,
  "\U0000F016", // 'nt_tstorms': 17,
  "\U0000F021", // 'nt_mist': 18,
};
//////Init Configuration///
//Init Clay
ClaySettings settings;
// Initialize the default settings
static void prv_default_settings(){
//settings.Back1Color = GColorBlack;
  settings.Back1Color = GColorBlack;
  settings.FrameColor1 = GColorWhite; //divider bar color
  settings.Text2Color = GColorBlack; //Battery Bar Colour
  settings.Text3Color = GColorWhite; //Date & Temperature Colour
  settings.Text4Color = GColorWhite; //Quiet Time & BT icon colour
  settings.Text7Color = GColorWhite; //weathericon colour
  settings.HourColor = GColorWhite; //time colour
  settings.WeatherUnit = 0;
  settings.UpSlider = 30;
  settings.UseForecast = false;
 }

bool BTOn=true;
bool showForecastWeather = false;
int s_countdown = 0;
//int s_countdown_images = 30;
int s_loop = 0;

//////End Configuration///
///////////////////////////

// Callback for js request
/*void request_watchjs(){
    // Begin dictionary
  DictionaryIterator * iter;
  app_message_outbox_begin( & iter);
  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);
  // Send the message!
  app_message_outbox_send();
}*/


///BT Connection
static void bluetooth_callback(bool connected){
  BTOn = connected;
}

static void bluetooth_vibe_icon (bool connected) {

  layer_set_hidden(s_canvas_bt_icon, connected);

  if(!connected && !quiet_time_is_active()) {
    // Issue a vibrating alert
    vibes_double_pulse();
  }
}

static void quiet_time_icon () {
  if(!quiet_time_is_active()) {
  layer_set_hidden(s_canvas_qt_icon,true);
  }
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  // A tap event occured
  showForecastWeather = !showForecastWeather;
  layer_mark_dirty (s_canvas_weather);
}

void layer_update_proc_background (Layer * back_layer, GContext * ctx){
  GRect bounds = layer_get_bounds(back_layer);

  GRect BackgroundRect =
  PBL_IF_ROUND_ELSE(
    GRect(0, 0, bounds.size.w, bounds.size.h),
    GRect(0, 0, bounds.size.w, bounds.size.h));

   graphics_context_set_fill_color(ctx, settings.Back1Color);
   graphics_fill_rect(ctx, BackgroundRect,0,GCornersAll);

}

void update_time_area_layer(Layer *l, GContext* ctx7) {
  // check layer bounds
  GRect bounds = layer_get_unobstructed_bounds(l);

  #ifdef PBL_ROUND
 //   bounds = GRect(0, ROUND_VERTICAL_PADDING, bounds.size.w, bounds.size.h - ROUND_VERTICAL_PADDING * 2);
     bounds = GRect(0, 0,bounds.size.w, bounds.size.h);
  #else
     bounds = GRect(0,0,bounds.size.w, bounds.size.h);
  #endif

  // initialize FCTX, the fancy 3rd party drawing library that all the cool kids use
  FContext fctx;

  fctx_init_context(&fctx, ctx7);
  fctx_set_color_bias(&fctx, 0);
  fctx_set_fill_color(&fctx, settings.HourColor);

#ifdef PBL_ROUND
  //int font_size = bounds.size.h * 0.55;
  int font_size = 34;
 #elif PBL_PLATFORM_APLITE
  //int font_size = bounds.size.h * 0.65;
  int font_size = 34;
 #else
  int font_size = 34;
  #endif
  int h_adjust = 0;
  int v_adjust = 0;

    #ifdef PBL_COLOR
      fctx_enable_aa(true);
    #endif

  // if it's a round watch, EVERYTHING CHANGES
  #ifdef PBL_ROUND
    v_adjust = 0;

  #else
    h_adjust = 0;
  #endif

  FPoint time_pos;
  fctx_begin_fill(&fctx);
  fctx_set_text_em_height(&fctx, time_font, font_size);
  fctx_set_color_bias(&fctx,0);

  int hourdraw;
  char hournow[3];
  if (clock_is_24h_style()){
    hourdraw=s_hours;
    snprintf(hournow,sizeof(hournow),"%02d",hourdraw);
    }
  else {
    if (s_hours==0 || s_hours==12){
      hourdraw=12;
    }
    else hourdraw=s_hours%12;
  snprintf(hournow, sizeof(hournow), "%d", hourdraw);
   }

  int mindraw;
  mindraw = s_minutes;
  char minnow[3];
  snprintf(minnow, sizeof(minnow), "%02d", mindraw);

  char timedraw[6];
  snprintf(timedraw, sizeof(timedraw), "%s:%s",hournow,minnow);

  time_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(90, 72) + h_adjust);
  time_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(127, 127)  + v_adjust);

  fctx_set_offset(&fctx, time_pos);
  fctx_draw_string(&fctx, timedraw, time_font, GTextAlignmentCenter, FTextAnchorMiddle);
  fctx_end_fill(&fctx);

  fctx_deinit_context(&fctx);
}

//Update main layer
void update_weather_area_layer(Layer *l, GContext* ctx) {
  // check layer bounds

  #ifdef PBL_ROUND
     GRect bounds = layer_get_unobstructed_bounds(l);
     bounds = GRect(0, 0,bounds.size.w, bounds.size.h);
  #else
     GRect bounds = GRect (0,0,144,144);
     bounds = GRect(0,0,bounds.size.w,bounds.size.h);
  #endif

  FContext fctx;

  fctx_init_context(&fctx, ctx);
  fctx_set_color_bias(&fctx, 0);

    #ifdef PBL_ROUND
    //int font_size = bounds.size.h * 0.55;
    int font_size = 116;//81
   #elif PBL_PLATFORM_APLITE
    //int font_size = bounds.size.h * 0.65;
    int font_size = 100;
   #else
    int font_size = 116;
    #endif
    int h_adjust = 0;
    int v_adjust = 0;

      #ifdef PBL_COLOR
        fctx_enable_aa(true);
      #endif

    // if it's a round watch, EVERYTHING CHANGES
    #ifdef PBL_ROUND
      v_adjust = 0;

    #else
      h_adjust = 0;
    #endif

    FPoint icon_pos;
    fctx_set_fill_color(&fctx, settings.Text7Color);
    fctx_begin_fill(&fctx);
    fctx_set_text_em_height(&fctx, weather_font, font_size);
    fctx_set_color_bias(&fctx,0);
  //  fctx_set_fill_color(&fctx, ColorSelect(settings.HourColor, settings.HourColorN));
    char CondToDraw[4];
if(!settings.UseForecast){
  snprintf(CondToDraw, sizeof(CondToDraw), "%s",settings.iconnowstring);//"\U0000F06B");
  }
  else
{
  snprintf(CondToDraw, sizeof(CondToDraw), "%s",settings.iconforestring);//"\U0000F06B");
}
    icon_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(90+8, 72+8) + h_adjust);
    icon_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(30+12, 30+12)  + v_adjust);

    fctx_set_offset(&fctx, icon_pos);
    fctx_draw_string(&fctx, CondToDraw, weather_font, GTextAlignmentCenter, FTextAnchorTop);

    fctx_end_fill(&fctx);

    fctx_deinit_context(&fctx);
  }

static void layer_update_proc(Layer * layer1, GContext * ctx){
  GRect VerticalDividerRect =
      (PBL_IF_ROUND_ELSE(
        GRect(102,144+2,2,180-144),
        GRect(72,144+2,2,168-144)));

  GRect DayofWeekRect =
  (PBL_IF_ROUND_ELSE(
      GRect(0, 142, 98, 16),
      GRect(0, 140, 72, 16)));

  GRect BatteryRect =
    (PBL_IF_ROUND_ELSE(
      GRect(0,144,180,2),
      GRect(0,144,144,2)));

      //Battery
  int s_battery_level = battery_state_service_peek().charge_percent;

  #ifdef PBL_ROUND
    int width_round = (s_battery_level * 144) / 100;
  #else
    int width_rect = (s_battery_level * 144) / 100;
  #endif

  GRect BatteryFillRect =
      (PBL_IF_ROUND_ELSE(
        GRect(18,144,width_round,2),
        GRect(0,144,width_rect,2)));

    char battperc[6];
    snprintf(battperc, sizeof(battperc), "%d", s_battery_level);
    strcat(battperc, "%");

  // Draw the battery bar & divider bar backgrounds
  graphics_context_set_fill_color(ctx, settings.Back1Color);// GColorBlack);
  graphics_fill_rect(ctx, BatteryRect, 0, GCornerNone);

  graphics_context_set_fill_color(ctx, settings.FrameColor1);// GColorBlack);
  graphics_fill_rect(ctx, VerticalDividerRect, 0, GCornerNone);

  // Draw the battery bar
  graphics_context_set_fill_color(ctx, settings.Text2Color);
  graphics_fill_rect(ctx,BatteryFillRect, 0, GCornerNone);

  //Date
  // Local language
  const char * sys_locale = i18n_get_system_locale();
  char datedraw[10];
  fetchwday(s_weekday, sys_locale, datedraw);
  char datenow[10];
  snprintf(datenow, sizeof(datenow), "%s", datedraw);

  int daydraw;
  daydraw = s_day;
  char daynow[8];
  snprintf(daynow, sizeof(daynow), "%02d", daydraw);

  char daydate[9];
  snprintf(daydate, sizeof (daydate), "%s %02d", datedraw,daydraw);

  graphics_context_set_text_color(ctx, settings.Text3Color);
  graphics_draw_text(ctx, daydate, FontDate, DayofWeekRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentRight,GTextAlignmentCenter), NULL);

  }

  static void layer_update_proc_weather(Layer * layer1, GContext * ctx){
    char TempToDraw[20];

    graphics_context_set_text_color(ctx,settings.Text3Color);

    if (!showForecastWeather)
        {
    GRect TempRect =  //shows current temp
        (PBL_IF_ROUND_ELSE(
          GRect(108, 142, 180-100, 16),
          GRect(74, 140, 72-2, 16)));
     snprintf(TempToDraw, sizeof(TempToDraw), "%s",settings.tempstring);
     graphics_draw_text(ctx, TempToDraw, FontTemp,TempRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentLeft,GTextAlignmentCenter), NULL);

        }
    else
        {
    GRect TempRect =  //shows forecast temp hi|low
        (PBL_IF_ROUND_ELSE(
          GRect(106, 145, 180-100, 16),
          GRect(74, 140, 72-2, 16)));
    snprintf(TempToDraw, sizeof(TempToDraw), "%s",settings.temphistring);
    graphics_draw_text(ctx, TempToDraw, FontTempFore, TempRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentLeft,GTextAlignmentCenter), NULL);

        }
    }


static void layer_update_proc_bt(Layer * layer3, GContext * ctx3){
   // Create Rects

  GRect BTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(128,17,40,20),
      GRect(144-40,5,38,20)));

 bluetooth_callback(connection_service_peek_pebble_app_connection());

 graphics_context_set_text_color(ctx3, settings.Text4Color);
 graphics_draw_text(ctx3, "z", FontIcon2, BTIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentLeft,GTextAlignmentRight), NULL);
  }

static void layer_update_proc_qt(Layer * layer4, GContext * ctx4){
   // Create Rects
 GRect QTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(13,17,40,20),
      GRect(2,5,38,20)));

 quiet_time_icon();

 graphics_context_set_text_color(ctx4, settings.Text4Color);
 graphics_draw_text(ctx4, "\U0000E061", FontIcon2, QTIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentRight,GTextAlignmentLeft), NULL);

}


/////////////////////////////////////////
////Init: Handle Settings////
/////////////////////////////////////////
// Read settings from persistent storage
static void prv_load_settings(){
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, & settings, sizeof(settings));
}
// Save the settings to persistent storage
static void prv_save_settings(){
  persist_write_data(SETTINGS_KEY, & settings, sizeof(settings));
 }

// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator * iter, void * context){
    s_loop = s_loop + 1;
    // Weather conditions
    Tuple * wtemp_t = dict_find(iter, MESSAGE_KEY_WeatherTemp);
    if (wtemp_t){
    snprintf(settings.tempstring, sizeof(settings.tempstring), "%s", wtemp_t -> value -> cstring);
    }

    Tuple * wforetemp_t = dict_find(iter, MESSAGE_KEY_TempFore);
    if (wforetemp_t){
      snprintf(settings.temphistring, sizeof(settings.temphistring), "%s", wforetemp_t -> value -> cstring);
    }

    Tuple * iconnow_tuple = dict_find(iter, MESSAGE_KEY_IconNow);
      if (iconnow_tuple){
        snprintf(settings.iconnowstring,sizeof(settings.iconnowstring),"%s",weather_conditions[(int)iconnow_tuple->value->int32]);
    }

    Tuple * iconnumbernow_tuple = dict_find(iter, MESSAGE_KEY_IconNow);
      if (iconnumbernow_tuple){
      settings.iconnumbernow = (int)iconnumbernow_tuple->value->int32;
    }

    Tuple * iconnumberfore_tuple = dict_find(iter, MESSAGE_KEY_IconFore);
      if (iconnumberfore_tuple){
      settings.iconnumberfore = (int)iconnumberfore_tuple->value->int32;
    }

    Tuple * iconfore_tuple = dict_find(iter, MESSAGE_KEY_IconFore);
    if (iconfore_tuple){
      snprintf(settings.iconforestring,sizeof(settings.iconforestring),"%s",weather_conditions[(int)iconfore_tuple->value->int32]);
    }

    Tuple * frequpdate = dict_find(iter, MESSAGE_KEY_UpSlider);
    if (frequpdate){
      settings.UpSlider = (int) frequpdate -> value -> int32;
      //Restart the counter
      s_countdown = settings.UpSlider;
      //s_countdown_images = settings.UpSlider + 1;
    }

    Tuple * currfore_t = dict_find(iter, MESSAGE_KEY_CurrentOrFore);
    if (currfore_t){
      if (currfore_t -> value -> int32 == 0){
        settings.UseForecast = false;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Use current for images");
      } else {
        settings.UseForecast = true;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Use forecast for images");
      }
    }

//Colours
Tuple * bk1_color_t = dict_find(iter, MESSAGE_KEY_Back1Color);
if (bk1_color_t){
  settings.Back1Color = GColorFromHEX(bk1_color_t-> value -> int32);
}


  Tuple * fr1_color_t = dict_find(iter, MESSAGE_KEY_FrameColor1);
  if (fr1_color_t){
    settings.FrameColor1 = GColorFromHEX(fr1_color_t-> value -> int32);
  }


  Tuple * hr_color_t = dict_find(iter, MESSAGE_KEY_HourColor);
  if (hr_color_t){
    settings.HourColor = GColorFromHEX(hr_color_t-> value -> int32);
  }

  Tuple * tx2_color_t = dict_find(iter, MESSAGE_KEY_Text2Color);
  if (tx2_color_t){
    settings.Text2Color = GColorFromHEX(tx2_color_t-> value -> int32);
  }

   Tuple * tx3_color_t = dict_find(iter, MESSAGE_KEY_Text3Color);
  if (tx3_color_t){
    settings.Text3Color = GColorFromHEX(tx3_color_t-> value -> int32);
  }

  Tuple * tx4_color_t = dict_find(iter,MESSAGE_KEY_Text4Color);
  if (tx4_color_t){
    settings.Text4Color = GColorFromHEX(tx4_color_t-> value -> int32);
    }

    Tuple * tx7_color_t = dict_find(iter,MESSAGE_KEY_Text7Color);
    if (tx7_color_t){
      settings.Text7Color = GColorFromHEX(tx7_color_t-> value -> int32);
      }

  //Update for config changes

  layer_mark_dirty(s_canvas_background);
  layer_mark_dirty(s_canvas);
  layer_mark_dirty(s_canvas_weather);
  layer_mark_dirty(s_canvas_bt_icon);
  layer_mark_dirty(s_canvas_qt_icon);
  layer_mark_dirty(time_area_layer);


  // Save the new settings to persistent storage
  prv_save_settings();
}

//Load main layer
static void window_load(Window * window){
  Layer * window_layer = window_get_root_layer(window);
  GRect bounds4 = layer_get_bounds(window_layer);

//add background picture

  s_canvas_background = layer_create(bounds4);
    layer_set_update_proc (s_canvas_background, layer_update_proc_background);
    layer_add_child(window_layer,s_canvas_background);

  weather_area_layer = layer_create(bounds4);
    layer_set_update_proc(weather_area_layer, update_weather_area_layer);
    layer_add_child(window_layer, weather_area_layer);

    // add time layer
  time_area_layer = layer_create(bounds4);
    layer_set_update_proc(time_area_layer, update_time_area_layer);
    layer_add_child(window_layer, time_area_layer);

//add day date, battery and dividers
  s_canvas = layer_create(bounds4);
    layer_set_update_proc(s_canvas, layer_update_proc);
    layer_add_child(window_layer, s_canvas);

//add bluetooth icon
  s_canvas_bt_icon = layer_create(bounds4);
    layer_set_update_proc (s_canvas_bt_icon, layer_update_proc_bt);
    layer_add_child(window_layer, s_canvas_bt_icon);

// add quiet time icon
  s_canvas_qt_icon = layer_create(bounds4);
    layer_set_update_proc (s_canvas_qt_icon, layer_update_proc_qt);
    layer_add_child(window_layer, s_canvas_qt_icon);





//add weather (temperature) info layer
  s_canvas_weather = layer_create(bounds4);
      layer_set_update_proc(s_canvas_weather, layer_update_proc_weather);
      layer_add_child(window_layer, s_canvas_weather);

}


static void window_unload(Window * window){
  layer_destroy(s_canvas_background);
//  gbitmap_destroy(s_background_picture);
  layer_destroy(s_canvas);
  layer_destroy(time_area_layer);
  layer_destroy(weather_area_layer);
  layer_destroy(s_canvas_bt_icon);
  layer_destroy(s_canvas_qt_icon);
  layer_destroy(s_canvas_weather);
  window_destroy(s_window);
  fonts_unload_custom_font(FontIcon2);
  ffont_destroy(time_font);
  ffont_destroy(weather_font);
}

void main_window_push(){
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
  }

void main_window_update(int hours, int minutes, int weekday, int day, int month){
  s_hours = hours;
  s_minutes = minutes;
  s_day = day;
  s_weekday = weekday;
  s_month = month;

  layer_mark_dirty(s_canvas_background);
  layer_mark_dirty(s_canvas);
  //layer_mark_dirty(s_canvas_weather);
  layer_mark_dirty(s_canvas_bt_icon);
  layer_mark_dirty(s_canvas_qt_icon);
  layer_mark_dirty(time_area_layer);
  layer_mark_dirty(weather_area_layer);

}

static void tick_handler(struct tm * time_now, TimeUnits changed){

  main_window_update(time_now -> tm_hour, time_now -> tm_min, time_now -> tm_wday, time_now -> tm_mday, time_now -> tm_mon);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick at %d", time_now -> tm_min);

  if (s_countdown == 0){
    //Reset
    s_countdown = settings.UpSlider;
  //  s_countdown_images = settings.UpSlider + 1;
  } else{
    s_countdown = s_countdown - 1;
  //  s_countdown_images = s_countdown_images -1;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Countdown to update %d loop is %d", s_countdown, s_loop);

  if (s_countdown == 0){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "countdown is 0, updated weather at %d", time_now -> tm_min);
      s_loop = 0;
      // Begin dictionary
      DictionaryIterator * iter;
      app_message_outbox_begin( & iter);
      // Add a key-value pair
      dict_write_uint8(iter, 0, 0);
      // Send the message!
      app_message_outbox_send();

  //    layer_mark_dirty(s_canvas_background);
  //    update_background_picture();
  }

    //onreconnection(BTOn, connection_service_peek_pebble_app_connection());
    if (!BTOn && connection_service_peek_pebble_app_connection()){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "BT reconnected, requesting weather at %d", time_now -> tm_min);
      s_loop = 0;
      // Begin dictionary
      DictionaryIterator * iter;
      app_message_outbox_begin( & iter);
      // Add a key-value pair
      dict_write_uint8(iter, 0, 0);
      // Send the message!
      app_message_outbox_send();

    //  layer_mark_dirty(s_picture_bitmap_layer);
    //  update_background_picture();
    }

    bluetooth_callback(connection_service_peek_pebble_app_connection());


}


static void init(){

  prv_load_settings();
  // Listen for AppMessages
  //s_countdown_images = settings.UpSlider + 1;
  s_countdown = settings.UpSlider;
  s_loop = 0;
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  s_hours=t->tm_hour;
  s_minutes=t->tm_min;
  s_day=t->tm_mday;
  s_weekday=t->tm_wday;
  s_month=t->tm_mon;
  //Register and open
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(1536, 1536);
  // Load Fonts
//  update_background_picture();
  time_font =  ffont_create_from_resource(RESOURCE_ID_FFONT_GRAM);
  weather_font= ffont_create_from_resource(RESOURCE_ID_FFONT_ERET);
 //  time_font =  ffont_create_from_resource(RESOURCE_ID_FONT_STEELFISH);
 // time_font = ffont_create_from_resource(RESOURCE_ID_FONT_DINCONBOLD);
  FontDate = PBL_IF_ROUND_ELSE(fonts_get_system_font(FONT_KEY_GOTHIC_24),fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  FontTemp = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  FontTempFore = PBL_IF_ROUND_ELSE(fonts_get_system_font(FONT_KEY_GOTHIC_18),fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  FontIcon2 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_16));

  main_window_push();
  // Register with Event Services
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  connection_service_subscribe((ConnectionHandlers){
    .pebble_app_connection_handler = bluetooth_vibe_icon
  });
  bluetooth_vibe_icon(connection_service_peek_pebble_app_connection());
  accel_tap_service_subscribe(accel_tap_handler);

}
static void deinit(){
  tick_timer_service_unsubscribe();
  app_message_deregister_callbacks(); //Destroy the callbacks for clean up
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();
  accel_tap_service_unsubscribe();
}
int main(){
  init();
  app_event_loop();
  deinit();
}
