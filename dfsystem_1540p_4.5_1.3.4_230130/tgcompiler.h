
#ifndef TGCOMPILER_H
#define TGCOMPILER_H

#include "syscompiler.h"

#define SET_TS(a, b, c)	\
do { \
	char *s = "ts=" #a " { width=" #b " "  #c " } "; \
	tsCompile(name, #a, s); \
} while (0)

#define TIMESET(a) \
	void TIMESET_##a(const char *name)

#define SET_TIMESET() \
	SET_TIME(name)

class TGCompiler : public SysCompiler
{
public:
	TGCompiler();
	~TGCompiler();

public:
	TTIMESET *ts() { return &timeset; }
	String timeString();
	void init();
	void splitSyntex(char *source, int line = 0);

protected:
	TTIMESET timeset;

	void createSyntex();
	void clkInv(char *word);
	void pinAssign(char *word);
	int subCompile(char *source, int line);
};

typedef Map<String, char *, TGCompiler *, TGCompiler *> timesetMap;
typedef List<TGCompiler *, TGCompiler *> timesetGroup;

#endif
