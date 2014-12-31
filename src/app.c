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
   char*  minutes[] = {"null", "zehn", "zwanzig", "dreißig", "vierzig", "fünfzig"};
   static char   buffer[20];
      
   // get current time
   time_t now = time(NULL);
   struct tm * currentTime = localtime(&now);

   memset(buffer, sizeof(buffer), 0);
   snprintf (buffer, sizeof(buffer), "+%d", currentTime->tm_min % 10);

   // some calculations
   if (currentTime->tm_hour > 12) {
      text_layer_set_text(_hour, hours[currentTime->tm_hour - 12]);
   } else {
      text_layer_set_text(_hour, hours[currentTime->tm_hour]);
   }
   text_layer_set_text(_minute, minutes[currentTime->tm_min / 10]);
   text_layer_set_text(_offset, buffer);
   
   APP_LOG(APP_LOG_LEVEL_DEBUG, "%d:%d (%s)", currentTime->tm_hour, currentTime->tm_min, buffer);
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
   _minute = createLayer(60, 45, FONT_KEY_BITHAM_42_LIGHT);
   _offset = createLayer(110, 30, FONT_KEY_GOTHIC_28);

   layer_add_child(window_get_root_layer(_window), text_layer_get_layer(_hour));
   layer_add_child(window_get_root_layer(_window), text_layer_get_layer(_minute));   
   layer_add_child(window_get_root_layer(_window), text_layer_get_layer(_offset));
   
   // Ensures time is displayed immediately (will break if NULL tick event accessed).
   // (This is why it's a good idea to have a separate routine to do the update itself.)
   handle_minute_tick(NULL, 0);
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