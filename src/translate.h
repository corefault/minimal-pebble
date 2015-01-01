#pragma once

#define PRE 0
#define MIDDLE 1
#define POST 2
   
struct t_info {
   char*   pre;
   char*   mid;
   char*   post;
};

char* translate_remaining(int minute);
char* translate_line(int mode, int hour, int minute);