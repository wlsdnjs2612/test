
#include <stdlib.h>
#include "syscompiler.h"

SysCompiler::SysCompiler()
{
	option = 0;
	total_line = 0;
	error_count = 0;
//	sysModel = 0;
}

SysCompiler::~SysCompiler()
{
	macro_map.RemoveAll();
	syntex_map.RemoveAll();
	error_string = "";
	source_string = "";
}
/*
int SysCompiler::getModel()
{
	return sysModel;
}

void SysCompiler::setModel(int model)
{
	sysModel = model;
}
*/
String SysCompiler::name()
{
	return compile_name;
}

void SysCompiler::setName(const char *name)
{
	compile_name = name;
}

int SysCompiler::line()
{
	return total_line;
}

int SysCompiler::error()
{
	return error_count;
}

void SysCompiler::setOption(int o)
{
	option = o;
}

void SysCompiler::setMacro(char *m, char *arg)
{
	macro_map[m] = arg;
}

String SysCompiler::source()
{
	return source_string;
}

int SysCompiler::compTime()
{
	return comp_time;
}

void SysCompiler::setCompTime(int time)
{
	comp_time = time;
}

String SysCompiler::getError()
{
	return error_string;
}

void SysCompiler::init()
{
	total_line = 0;
	error_count = 0;
	source_string = "";
	error_string = "";
}

void SysCompiler::setSource(const char *source)
{
	source_string = source;
}

void SysCompiler::addError(int no, const char *msg, const char *syntex, bool warning)
{
	char buf[300];

	if (warning) {
		sprintf(buf, "%s:%d: warning %03d: %s : '%s'\r\n", (const char *) logo_string, total_line + 1, no, msg, syntex);
	}
	else {
		error_count++;
		sprintf(buf, "%s:%d: error %03d: %s : '%s'\r\n", (const char *) logo_string, total_line + 1, no, msg, syntex);
	}

	error_string += buf;
}

char *SysCompiler::macro(char *word, const char *token)
{
	String define;
	static char macro[80] = { 0, };

	if (word == 0) {
		word = strtok(0, token);
	}

	if (word && macro_map.Lookup(word, define)) {
		char *s = (char *) define;
		strcpy(macro, s);
		return macro;
	}

	return word;
}

void SysCompiler::setLogo(const char *logo_name)
{
	if (logo_name == 0 && logo_string.isEmpty()) {
		logo_string = "Line";
	}
	else if (logo_string.isEmpty()) {
		logo_string = logo_name;
	}
}

int SysCompiler::compile(char *source, int line, const char *logo_name)
{
	String strSource;
	setLogo(logo_name);

	if (source == NULL) {
		strSource = source_string;
		splitSyntex((char *) (const char *) source_string);
		source_string = strSource;
	}
	else {
		init();
		strSource = source;
		splitSyntex(source, line);
		source_string = strSource;
	}

	return error_count;
}
