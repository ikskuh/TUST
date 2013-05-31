#ifndef _DEBUG_C_
#define _DEBUG_C_

// Creating an HTML header
void log_write_header() {
	// Open "LOG_FILE" (see log.h) to write
	vLogFileHandle = file_open_write(strLogFile);
	file_str_write(vLogFileHandle, "<html>\n\t<head>\n\t\t<title>Log file</title>\n\t</head>\n\t<body>\n\t\t<h1>Log file</h1>\n\t\t");

	char cbuffer[128];
	// Log date and time
	sprintf(cbuffer, "%d.%d.%d %d:%d:%d\n\t\t<table width=\"600\" style=\"border-style: solid; border-width: 1px; \">",
		(long)sys_day, (long)sys_month, (long)sys_year, (long)sys_hours, (long)sys_minutes, (long)sys_seconds);
	file_str_write(vLogFileHandle, cbuffer);

	// Write table header
	file_str_write(vLogFileHandle, "\n\t\t\t<tr><td><b>Frame</b></td><td><b>Title</b></td><td><b>Message</b></td></tr>");
	file_close(vLogFileHandle);
	nLogHeaderWritten = 1;
}

void log_init() {
	log_init(NULL);
}

// Initialize log
void log_init(STRING* _logFile) {
	txtLog = txt_create(LOG_LINES,999);
	txtLog.font = fontLog;
	txtLog.size_x = screen_size.x;
	txtLog.size_y = 100;
	set(txtLog, SHOW | OUTLINE);
	int i;
	for(i=0; i<txtLog.strings; i++) {
		(txtLog.pstring)[i] = str_create("#256");
	}

	// Color for log text
	vec_set(cLogColor, vector(255,255,255));

	// Log to file, too?
	if (_logFile != NULL) {
		nLogToFile = 1;
		str_cpy(strLogFile, _logFile);
	}
	
	// If logging to file, initialize file IO
	if (nLogToFile == 1) {
		log_write_header();
	}
}

// Close HTML file
void log_write_footer() {
	vLogFileHandle = file_open_append(strLogFile);
	file_str_write(vLogFileHandle, "\n\t\t</table>\n\t</body>\n</html>");
	file_close(vLogFileHandle);
}

// Close logging lib
void log_free() {
	if (nLogToFile == 1) log_write_footer();
	ptr_remove(txtLog);
	txtLog = NULL;
	nLogHeaderWritten = 0;
}

void logHide() {
	if (txtLog != NULL) {
		reset(txtLog, SHOW);
	}
}

void logShow() {
	if (txtLog != NULL) {
		set(txtLog, SHOW);
	}
}

// Toggle logging to file
void log_toggle_file_log() {
	if (txtLog == NULL) return;
	nLogToFile = 1 - nLogToFile;
	if (nLogToFile == 1) {
		if (nLogHeaderWritten == 0) {
			log_write_header();
		}
	}
}

void log_clear() {
	if (txtLog == NULL) return;
	int i;
	for(i=0; i<txtLog.strings; i++) {
		str_cpy((txtLog.pstring)[i], "");
	}
}

// Logging of char arrays
void log_print(char* _info, char* _in) {
	if (txtLog == NULL) return;

	// Move lines on step down
	int i;
	for (i=LOG_LINES-1; i > 0; i--) {
		str_cpy((txtLog.pstring)[i], (txtLog.pstring)[i-1]);
	}

	char cbuffer[128];
	sprintf(cbuffer, "%d: %s %s", total_frames, _info, _in);
	str_cpy((txtLog.pstring)[0], cbuffer);

	// Print to file?
	if (nLogToFile == 1) {
		vLogFileHandle = file_open_append(strLogFile);
		char cbuffer2[10];

		// Change color to hex for HTML coding
		log_rgb_to_hex(cbuffer2,(long)cLogColor.red,(long)cLogColor.green,(long)cLogColor.blue);
		sprintf(cbuffer, "\n\t\t\t<tr BGCOLOR=\"%s\"><td>%d</td><td>%s</td><td>%s</td></tr>", cbuffer2, total_frames, _info, _in);
		file_str_write(vLogFileHandle, cbuffer);
		file_close(vLogFileHandle);
	}
}

// Logging of Strings
void log_print(char* _info, STRING* _in) {
	if (_in != NULL) {
		log_print(_info, _in.chars);
	} else {
		log_print(_info, "NULL");
	}
}

// Logging of integers
void log_print(char* _info, int _in) {
	str_for_num(strLogTemp, _in);
	log_print(_info, strLogTemp.chars);
}

// Logging of vars
void log_print(char* _info, var _in) {
	str_for_num(strLogTemp, _in);
	log_print(_info, strLogTemp.chars);
}

// Logging of floats
void log_print(char* _info, float _in) {
	str_for_num(strLogTemp, _in);
	log_print(_info, strLogTemp.chars);	
}

// Logging of doubles
void log_print(char* _info, double _in) {
	str_for_num(strLogTemp, _in);
	log_print(_info, strLogTemp.chars);
}

// Logging of vectors
void log_print(char* _info, VECTOR* _in) {
	if (_in != NULL) {
		char buffer[128];
		sprintf(buffer, "%d %d %d", (long)(_in.x), (long)(_in.y), (long)(_in.z));
		log_print(_info, buffer);
	} else {
		log_print(_info, "NULL");
	}
}

// Logging system information
void log_system_data(char* _info) {
	char buffer[256];
	sprintf(buffer, "Engine memory: %d Windows version: %d Textur memory: %d Max. texture size: %d Shader version: %d",
		(long)sys_memory, (long)sys_winversion, (long)d3d_texfree, (long)d3d_texlimit, (long)d3d_shaderversion);
	log_print(_info, buffer);
}

// RGB colors to hexadecimal representation
void log_rgb_to_hex(char* _target, int _r, int _g, int _b) {
	sprintf(_target, "%.2X%.2X%.2X", _r, _g, _b);
}

void log_set_color(COLOR* _c) {
	vec_set(cLogColor, _c);
}

#endif /* DEBUG_C */