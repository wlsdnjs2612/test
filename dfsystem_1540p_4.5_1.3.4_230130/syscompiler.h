
#ifndef SYSCOMPILER_H
#define SYSCOMPILER_H

#include "struct.h"

class SysCompiler
{
public:
	SysCompiler();
	virtual ~SysCompiler();

	String name();
	void setName(const char *name);
	int compile(char *source = 0, int line = 0, const char *logo = 0);
	void setLogo(const char *logo_name);
	int line();
	int error();
	String source();
	int compTime();
	void setCompTime(int time);
	void setOption(int option);
	void setMacro(char *m, char *arg);
//	int getModel();
//	void setModel(int);
	void setSource(const char *source);
	String getError();

protected:
//	int sysModel;
	String compile_name;
	String logo_string;
	str2WordMap syntex_map;
	str2StrMap macro_map;

	int option;
	int comp_time;
	int total_line;
	int error_count;
	String error_string;
	String source_string;

	char *macro(char *word, const char *token = " ");
	virtual void init();
//	virtual void splitSyntex(char *source, int line = 0) {};
	virtual void splitSyntex(char *source, int line = 0) = 0;
	virtual void addError(int no, const char *msg, const char *syntex, bool warning = false);
};

#endif
