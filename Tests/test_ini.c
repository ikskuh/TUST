#include <acknex.h>
#include "..\\Source\\tust.h"

function main()
{
	STRING *inifile = "#256";
	str_cpy(inifile, "test.ini");
	path_make_absolute(inifile);
	
	error(inifile);
	
	ini_write(inifile, "Section A", "Entry A", "Hello World!");
	ini_write(inifile, "Section A", "Filename", inifile);
	ini_write_int(inifile, "Section B", "int", 7);
	ini_write_float(inifile, "Section B", "float", 0.1234567);
	ini_write_var(inifile, "Section B", "var", 0.1234567);
	
	STRING *tmp = "#128";
	ini_read(tmp, inifile, "Section A", "Filename", "");
	error(tmp);
	error(str_for_int(NULL, ini_read_int(inifile, "Section B", "int", 0)));
	error(str_for_float(NULL, ini_read_float(inifile, "Section B", "float", 0)));
	error(str_for_num(NULL, ini_read_var(inifile, "Section B", "var", 0)));
}