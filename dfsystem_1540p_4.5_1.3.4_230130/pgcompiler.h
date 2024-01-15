
#ifndef PGCOMPILER_H
#define PGCOMPILER_H

#include "syscompiler.h"

#define DUP_SUB	0x1

#define SET_SUB(a, b) \
do { \
	char *s = "sub=" #a " { " #b " } "; \
	subCompile(name, #a, s); \
} while (0)

#define PATTERN(a) \
	void PATTERN_##a(const char *name)

#define SET_PATTERN() \
	SET_MPAT(name)

#define SEND_MPAT(a) \
	PATTERN_##a(#a)

#define MEAS_PATTERN(a, b) \
	MEAS_MPAT(#a, b)

#define START_PATTERN(a) \
	START_MPAT(#a)

class PGCompiler : public SysCompiler
{
public:
	PGCompiler();
	~PGCompiler();

	int pc() { return global_pc; }
	void setPC(int pc = 1) { global_pc = pc; }
	String attach();
	String ramString();
	patternList *patternData() { return &pattern; }
	void addTsGroup(const char *timeset, int ts);
	void changeJump(const char *name, int jump);
	void init();
	void splitSyntex(char *strSource, int line = 0);

protected:
	int global_pc;
	patternList pattern;
	str2WordMap tsTimeGroup;
	noLabelList noSub;
	str2WordMap pauseMap;

	void createSyntex();
	int subCompile(char *source, int pc, int line);
	String ram2String(ramList *ramlist, int pc);
};

typedef Map<String, char *, PGCompiler *, PGCompiler *> patternMap;
typedef List<PGCompiler *, PGCompiler *> patternGroup;

#endif
