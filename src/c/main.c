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
static GFont //FontHour,
FontDate,
FontDate2,
//FontAMPM,
//FontBattery,
//FontDivider,
//FontIcon,
FontIcon2,
FontIcon3
;

FFont* time_font;

//char  settings.ALIEN[4];

static Window * s_window;

static Layer * s_canvas;
static Layer * s_canvas_bt_icon;
static Layer * s_canvas_qt_icon;
static Layer * s_picture_bitmap_layer;
static GBitmap *s_background_picture;

Layer * time_area_layer;

static int s_hours, s_minutes, s_weekday, s_day, s_month;


//////Init Configuration///
//Init Clay
ClaySettings settings;
// Initialize the default settings
static void prv_default_settings(){
//settings.Back1Color = GColorBlack;
  settings.FrameColor1 = GColorWhite;
  settings.Text1Color = GColorBlack;
  settings.Text2Color = GColorBlack;
  settings.Text3Color = GColorBlack;
  settings.Text4Color = GColorBlack;
  settings.Text5Color = GColorBlack;
  settings.Text6Color = GColorBlack;
  settings.ALIEN = false;
//  settings.Text7Color = GColorWhite;
  settings.HourColor = GColorBlack;

//  settings.MinColor = GColorWhite;
 }

bool BTOn=true;

//////End Configuration///
///////////////////////////

// Callback for js request
void request_watchjs(){
  //Starting loop at 0
//  s_loop = 0;
  // Begin dictionary
  DictionaryIterator * iter;
  app_message_outbox_begin( & iter);
  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);
  // Send the message!
  app_message_outbox_send();
}


///BT Connection
static void bluetooth_callback(bool connected){
  BTOn = connected;
}

static void update_background_picture() {
  if (!settings.ALIEN){
          s_background_picture = gbitmap_create_with_resource (RESOURCE_ID_IMAGE_ALIENHEAD);
        //  APP_LOG(APP_LOG_LEVEL_DEBUG, "ALIEN %s", settings.ALIEN);
          APP_LOG(APP_LOG_LEVEL_DEBUG, "should be white");
  //  } else if ((settings.Invert == true) && (IsNightNow == false)){
  //    IsInvertedNow = true;
  //          s_background_picture = gbitmap_create_with_resource (RESOURCE_ID_IMAGE_BACKGROUNDINV);
    } else {
          s_background_picture = gbitmap_create_with_resource (RESOURCE_ID_IMAGE_ALIENBLUE);
        //  APP_LOG(APP_LOG_LEVEL_DEBUG, "ALIEN %s", settings.ALIEN);
          APP_LOG(APP_LOG_LEVEL_DEBUG, "should be black & blue");
    }
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

#ifdef PBL_MICROPHONE
void layer_update_proc_background (Layer * back_layer, GContext * ctx){

  gbitmap_destroy(s_background_picture);
  update_background_picture();
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Alien in layer update proc %s", settings.ALIEN);
  GRect bitmap_bounds = gbitmap_get_bounds(s_background_picture);
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, s_background_picture, bitmap_bounds);

}

#else

void layer_update_proc_background (Layer * back_layer, GContext * ctx){
  //GRect bounds = layer_get_bounds(back_layer);
  //GRect Backgroundrect = GRect(0, 0, bounds.size.w, bounds.size.h);

  gbitmap_destroy(s_background_picture);
  update_background_picture();
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Alien in layer update proc %s", settings.ALIEN);

  /*  if ((settings.ALIEN == 49)){
    graphics_context_set_fill_color(ctx,GColorWhite);
    graphics_fill_rect(ctx, Backgroundrect,0,GCornersAll);
    }
    else {
    graphics_context_set_fill_color(ctx,GColorBlack);
    graphics_fill_rect(ctx, Backgroundrect,0,GCornersAll);
  }*/

  GRect bitmap_bounds = gbitmap_get_bounds(s_background_picture);
  GRect Bitmaprect = GRect(34, 2, bitmap_bounds.size.w, bitmap_bounds.size.h);
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, s_background_picture, Bitmaprect);

}
#endif

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
//    v_adjust = ROUND_VERTICAL_PADDING;
    v_adjust = 0;

  #else
    // for rectangular watches, adjust X position based on sidebar position
  //    h_adjust -= ACTION_BAR_WIDTH / 2 + 1;
   h_adjust = 0;
  #endif

  FPoint time_pos;
  fctx_begin_fill(&fctx);
  fctx_set_text_em_height(&fctx, time_font, font_size);
  fctx_set_color_bias(&fctx,0);
//  fctx_set_text_em_height(&fctx, minutes_font, font_size);

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
  time_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(128, 128)  + v_adjust);

  fctx_set_offset(&fctx, time_pos);
  fctx_draw_string(&fctx, timedraw, time_font, GTextAlignmentCenter, FTextAnchorMiddle);
  fctx_end_fill(&fctx);

  fctx_deinit_context(&fctx);
}

//Update main layer

static void layer_update_proc(Layer * layer1, GContext * ctx){
  // Create Rects
  //GRect bounds1 = layer_get_bounds(layer1);

  /*GRect MediumBand =
    PBL_IF_ROUND_ELSE(
    GRect(8, 75, 36, 30),
    GRect(102, 68, 36, 32));


   //Build display
//  graphics_context_set_fill_color(ctx, settings.Back1Color);
//  graphics_fill_rect(ctx, bounds1, 0, GCornerNone);
  graphics_context_set_fill_color(ctx, settings.FrameColor1);
  graphics_fill_rect(ctx, MediumBand,4,GCornersAll);*/

  GRect VerticalDivierRect =
      (PBL_IF_ROUND_ELSE(
        GRect(102,144+2,2,180-144),
        GRect(72,144+2,2,168-144)));


 GRect DayofWeekRect =
  (PBL_IF_ROUND_ELSE(
      GRect(0, 140, 100, 16),
      GRect(0, 140, 72, 16)));

  /*GRect DateRect =
    (PBL_IF_ROUND_ELSE(
      GRect(52, 140, 40, 16),
      GRect(52, 140, 40, 16)));


  GRect MonthNameRect =
    (PBL_IF_ROUND_ELSE(
      GRect(96, 140, 40, 16),
      GRect(96, 140, 48, 16)));
*/
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
  graphics_context_set_fill_color(ctx, settings.FrameColor1);// GColorBlack);
  graphics_fill_rect(ctx, BatteryRect, 0, GCornerNone);
  graphics_fill_rect(ctx, VerticalDivierRect, 0, GCornerNone);

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

/*  char monthdraw[10];
  fetchmonth (s_month, sys_locale, monthdraw);
  char monthnow[10];
  snprintf(monthnow, sizeof(monthnow), "%s", monthdraw);
*/
  int daydraw;
  daydraw = s_day;
  char daynow[8];
  snprintf(daynow, sizeof(daynow), "%02d", daydraw);

  char daydate[9];
  snprintf(daydate, sizeof (daydate), "%s %02d", datedraw,daydraw);

  // Concatenate date
  //strcat(/*datenow,*/ convertday);


  //Battery
//  int battery_level = battery_state_service_peek().charge_percent;
//  char battperc[20];
//  snprintf(battperc, sizeof(battperc), "%d", battery_level);
//  strcat(battperc, "%");

  //dates band
//  graphics_context_set_text_color(ctx, settings.Text2Color);
//  graphics_draw_text(ctx, battperc, FontBattery, BatteryRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentRight), NULL);

  //graphics_context_set_text_color(ctx, settings.Text1Color);
  //graphics_draw_text(ctx, ampm, FontAMPM, ampmRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentRight), NULL);

  graphics_context_set_text_color(ctx, settings.Text3Color);
  //graphics_draw_text(ctx, datenow, FontDate, DayofWeekRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentRight), NULL);
  graphics_draw_text(ctx, daydate, FontDate, DayofWeekRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentRight,GTextAlignmentCenter), NULL);



  //graphics_context_set_text_color(ctx, settings.Text6Color);
  //graphics_draw_text(ctx, daynow, FontDate2, DateRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);

  //graphics_context_set_text_color(ctx, settings.Text5Color);
  //graphics_draw_text(ctx, monthnow, FontDate, MonthNameRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft), NULL);

  }


static void layer_update_proc_bt(Layer * layer3, GContext * ctx3){
   // Create Rects

  GRect BTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(0,140,40,20),
      GRect(0,140,38,20)));


 bluetooth_callback(connection_service_peek_pebble_app_connection());

 graphics_context_set_text_color(ctx3, settings.Text4Color);
 graphics_draw_text(ctx3, "z", FontIcon2, BTIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft), NULL);
  }

static void layer_update_proc_qt(Layer * layer4, GContext * ctx4){
   // Create Rects
 GRect QTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(104,140,40,20),
      GRect(104,140,38,20)));

 quiet_time_icon();

 graphics_context_set_text_color(ctx4, settings.Text4Color);
 graphics_draw_text(ctx4, "\U0000E061", FontIcon2, QTIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentRight), NULL);

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

  // Background Color
  /*Tuple * bg1_color_t = dict_find(iter, MESSAGE_KEY_Back1Color);
  if (bg1_color_t){
    settings.Back1Color = GColorFromHEX(bg1_color_t-> value -> int32);
  }*/

  /*Tuple * alien_t = dict_find(iter, MESSAGE_KEY_ALIEN);
  if (alien_t){
    settings.ALIEN = (int) alien_t -> value -> int32;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Alien tuple %d", settings.ALIEN);
  }*/

  Tuple * alien_t = dict_find(iter, MESSAGE_KEY_ALIEN);
  if (alien_t){
    if (alien_t -> value -> int32 == 0){
      settings.ALIEN = false;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "White Alien");
      } else {
      settings.ALIEN = true;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Black&Blue Alien");
      }
  }


  Tuple * fr1_color_t = dict_find(iter, MESSAGE_KEY_FrameColor1);
  if (fr1_color_t){
    settings.FrameColor1 = GColorFromHEX(fr1_color_t-> value -> int32);
  }

  Tuple * tx1_color_t = dict_find(iter, MESSAGE_KEY_Text1Color);
  if (tx1_color_t){
    settings.Text1Color = GColorFromHEX(tx1_color_t-> value -> int32);
  }

  ///////////////////////////////
  Tuple * hr_color_t = dict_find(iter, MESSAGE_KEY_HourColor);
  if (hr_color_t){
    settings.HourColor = GColorFromHEX(hr_color_t-> value -> int32);
  }

  ///Tuple * min_color_t = dict_find(iter, MESSAGE_KEY_MinColor);
  //if (min_color_t){
  //  settings.MinColor = GColorFromHEX(min_color_t-> value -> int32);
  //}
 ///////////////////////////////
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

  Tuple * tx5_color_t = dict_find(iter,MESSAGE_KEY_Text5Color);
  if (tx5_color_t){
    settings.Text5Color = GColorFromHEX(tx5_color_t-> value -> int32);
    }

   Tuple * tx6_color_t = dict_find(iter,MESSAGE_KEY_Text6Color);
  if (tx6_color_t){
    settings.Text6Color = GColorFromHEX(tx6_color_t-> value -> int32);
    }

  //  Tuple * tx7_color_t = dict_find(iter,MESSAGE_KEY_Text7Color);
  // if (tx7_color_t){
  //   settings.Text7Color = GColorFromHEX(tx7_color_t-> value -> int32);
  //   }


  //End data gathered
  // Get display handlers

/*  Tuple * disntheme_t = dict_find(iter, MESSAGE_KEY_NightTheme);
  if (disntheme_t){
    if (disntheme_t -> value -> int32 == 0){
      settings.NightTheme = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "NTHeme off");
    } else settings.NightTheme = true;
  } */


  //Update colors
  layer_mark_dirty(s_picture_bitmap_layer);
  layer_mark_dirty(s_canvas);
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

  s_picture_bitmap_layer = layer_create(bounds4);
    layer_set_update_proc (s_picture_bitmap_layer, layer_update_proc_background);
    layer_add_child(window_layer,s_picture_bitmap_layer);

  s_canvas = layer_create(bounds4);
    layer_set_update_proc(s_canvas, layer_update_proc);
    layer_add_child(window_layer, s_canvas);

  s_canvas_bt_icon = layer_create(bounds4);
    layer_set_update_proc (s_canvas_bt_icon, layer_update_proc_bt);
    layer_add_child(window_layer, s_canvas_bt_icon);

  s_canvas_qt_icon = layer_create(bounds4);
    layer_set_update_proc (s_canvas_qt_icon, layer_update_proc_qt);
    layer_add_child(window_layer, s_canvas_qt_icon);

  time_area_layer = layer_create(bounds4);
    layer_add_child(window_get_root_layer(s_window), time_area_layer);
    layer_set_update_proc(time_area_layer, update_time_area_layer);


}


static void window_unload(Window * window){
  layer_destroy(s_picture_bitmap_layer);
  gbitmap_destroy(s_background_picture);
  layer_destroy(s_canvas);
  layer_destroy(time_area_layer);
  layer_destroy(s_canvas_bt_icon);
  layer_destroy(s_canvas_qt_icon);
  window_destroy(s_window);
//  fonts_unload_custom_font(FontIcon);
  fonts_unload_custom_font(FontIcon2);
  fonts_unload_custom_font(FontIcon3);
  ffont_destroy(time_font);
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

  layer_mark_dirty(s_picture_bitmap_layer);
  layer_mark_dirty(s_canvas);
  layer_mark_dirty(s_canvas_bt_icon);
  layer_mark_dirty(s_canvas_qt_icon);
  layer_mark_dirty(time_area_layer);


 /////// update_rotation(); ///use this one

}

static void tick_handler(struct tm * time_now, TimeUnits changed){

  main_window_update(time_now -> tm_hour, time_now -> tm_min, time_now -> tm_wday, time_now -> tm_mday, time_now -> tm_mon);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick at %d", time_now -> tm_min);

  }


static void init(){
//set background color to match bitmap
//  window_set_background_color(s_window, GColorBlack);

  prv_load_settings();
  // Listen for AppMessages

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  s_hours=t->tm_hour;
  s_minutes=t->tm_min;
  s_day=t->tm_mday;
  s_weekday=t->tm_wday;
  s_month=t->tm_mon;
  //Register and open
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(512, 512);
  // Load Fonts
  update_background_picture();
  time_font =  ffont_create_from_resource(RESOURCE_ID_FFONT_GRAM);
 //  time_font =  ffont_create_from_resource(RESOURCE_ID_FONT_STEELFISH);
 // time_font = ffont_create_from_resource(RESOURCE_ID_FONT_DINCONBOLD);
  FontDate = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  FontDate2 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  //FontAMPM = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  //FontDivider = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
  //FontBattery= fonts_get_system_font(PBL_IF_ROUND_ELSE(FONT_KEY_GOTHIC_14,FONT_KEY_GOTHIC_18_BOLD));
  //FontIcon = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_WEATHERICONS_20));
  FontIcon2 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_16));
  FontIcon3 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_18));

  main_window_push();
  // Register with Event Services
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  connection_service_subscribe((ConnectionHandlers){
    .pebble_app_connection_handler = bluetooth_vibe_icon
  });
  bluetooth_vibe_icon(connection_service_peek_pebble_app_connection());
 // handle_battery(battery_state_service_peek());
}
static void deinit(){
  tick_timer_service_unsubscribe();
  app_message_deregister_callbacks(); //Destroy the callbacks for clean up
//   tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();

}
int main(){
  init();
  app_event_loop();
  deinit();
}
