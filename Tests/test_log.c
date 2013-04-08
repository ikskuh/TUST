#include <acknex.h>
#include "..\\Source\\log.h"

void main() {
	level_load("");
	log_init(1);
	log_print("INFO", "This is a simple log message!");
	log_print("An integer", 123);
	log_set_color(vector(0,0,255));
	log_system_data("Some system info");
}