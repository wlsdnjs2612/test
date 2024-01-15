
#ifndef _PGCC_H
#define _PGCC_H

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include <string>
#include <vector>
using namespace std;

#include <tgcompiler.h>
#include <pgcompiler.h>

#define START_CODE	"TRACDES"
#define END_CODE	"TRACDEQ"

bool pc_line;
char *src_org;
int sysModel;
string strSrcFile;
timesetMap tgGroup;
TGCompiler *timeCompiler = NULL;
vector<string> def_parse;
vector<string> def_str;

#endif
