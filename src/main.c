#include <pebble.h>

static Window *s_main_window;
// Time variables
static TextLayer *s_time_layer;
static GFont s_time_font;
// OVO Background variables
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
// Date variables
static TextLayer *s_date_layer;
static GFont s_date_font;
// // Battery variables
// static TextLayer *s_battery_layer;
// static GFont s_battery_font;
// // Battery Emoji Variables
// static BitmapLayer *s_emoji_layer;
// static GBitmap *s_emoji_bitmap;


static void main_window_load(Window *window) {
  //Create OVO GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  //Create Emoji GBitmap, then set to created BitmapLayer
//   s_emoji_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_EMOJI);
//   s_emoji_layer = bitmap_layer_create(GRect(0, 74, 32, 14));
//   bitmap_layer_set_bitmap(s_emoji_layer, s_emoji_bitmap);
//   layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_emoji_layer));
  
  
  //Create the Time Textlayer
  s_time_layer = text_layer_create(GRect (5, 130, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  
  // Create Time GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TOO_LATE_25));
  
  // Improve Time Layout
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // Add Time Layer as a Child Layer to the Window's Root Layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  
  //Create the Date Textlayer
  s_date_layer = text_layer_create(GRect(5, 10, 139, 50));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  
  //Create Date Gfont
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TOO_LATE_15));
  
  //Improve Date Layout
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  //Add Date Layer as a Child Layer to the Window's Root Layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
  
  //Create the Battery Textlayer
//   s_battery_layer = text_layer_create(GRect(5, 74, 28, 20));
//   text_layer_set_background_color(s_battery_layer, GColorClear);
//   text_layer_set_text_color(s_battery_layer, GColorWhite);
  
  //Create Battery Gfont
//   s_battery_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TOO_LATE_10));
  
  //Improve Battery Layout
//   text_layer_set_font(s_battery_layer, s_battery_font);
//   text_layer_set_text_alignment(s_battery_layer, GTextAlignmentLeft);
  
  //Add Battery Layer as a Child Layer to the Window's Root Layer
//   layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));
}

static void main_window_unload(Window *window){
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
//   text_layer_destroy(s_battery_layer);
  
  //Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
//   text_layer_destroy(s_battery_font);
  
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);
//   gbitmap_destroy(s_emoji_bitmap);
  
  //Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
//   bitmap_layer_destroy(s_emoji_layer);
}

// static void battery_handler(BatteryChargeState new_state) {
//   // Write to a buffer and display
//   static char s_battery_buffer[16];
  
//   if (new_state.is_charging) {
//     snprintf(s_battery_buffer, sizeof(s_battery_buffer), "+ +");
//   } else {
//     snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", new_state.charge_percent);
//   }
//   text_layer_set_text(s_battery_layer, s_battery_buffer);
  
// }

// Refresh the Date and Time every minute
void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  
  static char time_text[] = "00:00";
  static char date_text[] = "Xxxxxxxxx 00";

  char *time_format;

  // Fetch the Date Info
  strftime(date_text, sizeof(date_text), "%B %e", tick_time);
  
  // Set Date to layer
  text_layer_set_text(s_date_layer, date_text);

  // Fetch the Time Info
  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);

  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  //Set time to the layer 
  text_layer_set_text(s_time_layer, time_text);
  
  //Fetch battery Info
//   battery_handler(battery_state_service_peek());
}


static void init() {
  //Create main window element and assign to pointer
  s_main_window = window_create();
  
  //Set handlers to manage elements inside window
  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  //Show the window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  //Register with TicketTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  
  //Register with Battery State Service
//   battery_state_service_subscribe(battery_handler);
  
}

static void deinit() {
  //Destroy the Window
  window_destroy(s_main_window);
  
}

int main(void){
  init();
  app_event_loop();
  deinit();
}
