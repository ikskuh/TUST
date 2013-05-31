#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <strio.c>

// Should be defined for active debug mode
//#define DEBUG

#define LOG_LINES 50

TEXT* txtLog = NULL;
FONT* fontLog = "Arial#12b";
COLOR cLogColor;
int nLogToFile = 0;
int nLogHeaderWritten = 0;
var vLogFileHandle;
STRING* strLogTemp = "#128";
STRING* strLogFile = "#128";

void log_init();
void log_init(STRING* _file);
void log_free();
void log_hide();
void log_show();
void log_toggle_file_log();
void log_clear();
void log_system_data(char* _info);
void log_set_color(COLOR* _c);
void log_print(char* _info, STRING* _in);
void log_print(char* _info, char* _in);
void log_print(char* _info, int _in);
void log_print(char* _info, var _in);
void log_print(char* _info, float _in);
void log_print(char* _info, double _in);
void log_print(char* _info, VECTOR* _in);

// Internal
void log_write_header();
void log_write_footer();
void log_rgb_to_hex(char* _target, int _r, int _g, int _b);

#include "log.c"

#endif /* DEBUG_H */