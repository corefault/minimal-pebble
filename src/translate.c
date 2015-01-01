#include <pebble.h>
#include "translate.h"

// global map of german words
static char*   _map[] = {"null", "eins", "zwei", "drei", "view", "fünf", "sechs", "sieben", "acht", "neun", "zehn", "elf", "zwölf"};

// global configuration structure
struct t_info info[] = {
   {.pre = "", .mid = "", .post = "$0 Uhr"},
   {.pre = "fünf nach", .mid = "", .post = "$0 Uhr"},
   {.pre = "zehn nach", .mid = "", .post = "$0 Uhr"},
   {.pre = "viertel nach", .mid = "", .post = "$0 Uhr"},
   {.pre = "zwanzig nach", .mid = "", .post = "$0 Uhr"},
   {.pre = "fünf vor", .mid = "halb", .post = "$1"},
   {.pre = "", .mid = "halb", .post = "$1"},
   {.pre = "fünf nach", .mid = "halb", .post = "$1"},
   {.pre = "zwangig vor", .mid = "", .post = "$1"},
   {.pre = "viertel vor", .mid = "", .post = "$1"},
   {.pre = "zehn vor", .mid = "", .post = "$1"},
   {.pre = "fünf vor", .mid = "", .post = "$1"}
};
   
/**
 * get 5 minute remainig dots
 */
char* translate_remaining(int minute) {
   static char   str[6];
   
   int i;
   minute = minute % 5;
   
   memset(str, 0, 6);
   for (i = 0; i < minute; i++) {
      strcat(str, ".");
   }
   return str;
}

/**
 * return the line based on configuration
 * @param mode see defines (PRE,MIDDLE,POST)
 * @param hour the hour
 * @param minute the minute
 * @return full line
 */
char* translate_line(int mode, int hour, int minute) {
   static char   str[20];

   int   index = minute / 5;
   if (hour > 12) {
      hour = hour - 12;
   }
   
   if (mode == PRE) {
      return info[index].pre;
   }
   if (mode == MIDDLE) {
      return info[index].mid;
   }
   if (mode == POST) {
      char   *begin = strchr(info[index].post, '$');
      if (begin == NULL) {
         return info[index].post;
      }
      begin++;

      // add the value to the hour
      if (*begin=='1') {
         hour++;
      }
      if (hour > 12) {
         hour = hour - 12;
      }
      begin+=1;
      memset(str, 0, 20);
      snprintf (str, 20, "%s%s", _map[hour], begin);
   }
   return str;
}
