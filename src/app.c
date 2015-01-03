#include <pebble.h>
#include "translate.h"

static Window    *_window; 
static TextLayer *_lines[5];
static PropertyAnimation *_animation[5] = {0}; 
   
const int WIDTH = 144;

//----------------------------------------------------------------------------
TextLayer* createLine(int y, int height, int align) {
   TextLayer* layer;
   layer = text_layer_create(GRect(0, y, WIDTH, height));
   text_layer_set_text_color(layer, GColorWhite);
   text_layer_set_background_color(layer, GColorClear);
   text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
   text_layer_set_text_alignment(layer, align);
   return layer;
}
//----------------------------------------------------------------------------
static void trigger_animation(int index, int duration) {
   Layer* layer = (Layer*) _lines[index];
   GRect from_frame = layer_get_frame(layer);
   GRect to_frame = from_frame;
   from_frame.origin.x = 160;
   to_frame.origin.x = 0;

   if (_animation[index] != 0) {
      property_animation_destroy (_animation[index]);
   }
   _animation[index] = property_animation_create_layer_frame(layer, &from_frame, &to_frame);
   animation_set_duration ((Animation*)_animation[index], duration);
   animation_set_curve ((Animation*)_animation[index], AnimationCurveEaseOut);
   animation_schedule((Animation*)_animation[index]);
}
//----------------------------------------------------------------------------
void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
   // get current time
   time_t now = time(NULL);
   struct tm * currentTime = localtime(&now);
   int    i;
   
   text_layer_set_text(_lines[0], "Es ist");
   text_layer_set_text(_lines[1], translate_line(PRE, currentTime->tm_hour, currentTime->tm_min));
   text_layer_set_text(_lines[2], translate_line(MIDDLE, currentTime->tm_hour, currentTime->tm_min));
   text_layer_set_text(_lines[3], translate_line(POST, currentTime->tm_hour, currentTime->tm_min));
   text_layer_set_text(_lines[4], translate_remaining(currentTime->tm_min));
   
   for (i = 0; i < 5; i++) {
      trigger_animation(i, 500 + (i*250));
   }
}

// ---- now the application code --------------------

//----------------------------------------------------------------------------
static void init() {
   int i;
   
   // Create our app's base window
   _window = window_create();
   window_stack_push(_window, true);
   window_set_background_color(_window, GColorBlack);
   
   _lines[0] = createLine(15, 32, GTextAlignmentLeft); 
   _lines[1] = createLine(45, 32, GTextAlignmentCenter); 
   _lines[2] = createLine(75, 32, GTextAlignmentCenter); 
   _lines[3] = createLine(105, 32, GTextAlignmentRight);    
   _lines[4] = createLine(135, 32, GTextAlignmentCenter);    

   // create the text layers
   for (i = 0; i < 5; i++) {
      layer_add_child(window_get_root_layer(_window), text_layer_get_layer(_lines[i]));
   }
   
   // Ensures time is displayed immediately (will break if NULL tick event accessed).
   // (This is why it's a good idea to have a separate routine to do the update itself.)
   handle_minute_tick(NULL, 0);
   tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}
//----------------------------------------------------------------------------
static void deinit() {
   int i = 0;
 
   window_destroy(_window);
   for (i = 0; i < 5; i++) {
      text_layer_destroy(_lines[i]);
      property_animation_destroy (_animation[i]);
   }
}
//----------------------------------------------------------------------------
int main(void) {
  init();
  app_event_loop();
  deinit();
}