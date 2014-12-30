#include <pebble.h>

Window    *_window; 
TextLayer *_hour;
TextLayer *_minute;
TextLayer *_offset;

const int WIDTH = 144;
const int HEIGHT = 168;

/**
 * helper function to create text layer
 */
TextLayer* createLayer(int y, int height, const char* fontName) {
   TextLayer* layer;
   layer = text_layer_create(GRect(0, y, WIDTH, height));
   text_layer_set_text_color(layer, GColorWhite);
   text_layer_set_background_color(layer, GColorClear);
   text_layer_set_font(layer, fonts_get_system_font(fontName));
   text_layer_set_text_alignment(layer, GTextAlignmentLeft);
   return layer;
}

/**
 * callback for minute tickhandler
 */
void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {

   char*  hours[] = {"zwölf", "eins", "zwei", "drei", "view", "fünf", "sechs", "sieben", "acht", "neun", "zehn", "elf"};
   char   offset[5];
      
   // get current time
   time_t now = time(NULL);
   struct tm * currentTime = localtime(&now);

   // some calculations
   if (currentTime->tm_hour > 12) {
      text_layer_set_text(_hour, hours[currentTime->tm_hour - 12]);
   } else {
      text_layer_set_text(_hour, hours[currentTime->tm_hour]);
   }
   
   if (currentTime->tm_min >= 0 && currentTime->tm_min < 15) {
      text_layer_set_text(_minute, "null");
      snprintf (offset, 5, "+%d", currentTime->tm_min);
   } else if (currentTime->tm_min >= 15 && currentTime->tm_min < 30) {
      text_layer_set_text(_minute, "fünfzehn");
      snprintf (offset, 5, "+%d", currentTime->tm_min - 15);
   } else if (currentTime->tm_min >= 30 && currentTime->tm_min < 45) {
      text_layer_set_text(_minute, "dreißig");
      snprintf (offset, 5, "+%d", currentTime->tm_min - 30);
   } else if (currentTime->tm_min >= 45) {
      text_layer_set_text(_minute, "fünfundvierzig");
      snprintf (offset, 5, "+%d", currentTime->tm_min - 45);
   }
   
   text_layer_set_text(_offset, offset);
}

// ---- now the application code --------------------

/**
 * initialization of application
 */
static void init() {
   // Create our app's base window
   _window = window_create();
   window_stack_push(_window, true);
   window_set_background_color(_window, GColorBlack);

   // create the text layers
   _hour   = createLayer(10, 45, FONT_KEY_BITHAM_42_BOLD);
   _minute = createLayer(60, 35, FONT_KEY_GOTHIC_28);
   _offset = createLayer(100, 32, FONT_KEY_BITHAM_30_BLACK);
   
   // Ensures time is displayed immediately (will break if NULL tick event accessed).
   // (This is why it's a good idea to have a separate routine to do the update itself.)
   handle_minute_tick(NULL, 0);

   layer_add_child(window_get_root_layer(_window), text_layer_get_layer(_hour));
   layer_add_child(window_get_root_layer(_window), text_layer_get_layer(_minute));   
   layer_add_child(window_get_root_layer(_window), text_layer_get_layer(_offset));

   tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

/**
 * application cleanup
 */
static void deinit() {
   window_destroy(_window);
   text_layer_destroy(_hour);
   text_layer_destroy(_minute);
   text_layer_destroy(_offset);
}


/**
 * main entry point
 */
int main(void) {
  init();
  app_event_loop();
  deinit();
}