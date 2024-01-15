
#include "pgcc.h"

string deviceDefault(char *source, char *def_number)
{
	char *start, *end;
	string defSrc = "";

	for (;;) {
		if ((source = strstr(source, "void")) == NULL) {
			break;
		}

		source = source + strlen("void");

		while (*source == ' ' || *source == '\t' || *source == '\n') {
			source++;
		}

		if (strncmp(source, "device_default", strlen("device_default")) != 0) {
			continue;
		}

		source = source + strlen("device_default");

		if (def_number[0] != '@') {
			bool b = false;
			unsigned int i = 0;

			for (i = 0; i < strlen(def_number); i++) {
				if (def_number[i] != source[i]) {
					b = true;
					break;
				}
			}

			if (b == true) {
				continue;
			}

			source += i;
		}

		while (*source == ' ' || *source == '\t' || *source == '\n') {
			source++;
		}

		if (*source != '(') {
			continue;
		}

		if ((start = strchr(source, '{')) && ((end = strchr(source, '}')))) {
			*end = 0;
			defSrc = start;
			*end = '}';
			break;
		}
	}

	return defSrc;
}

void eatComment(char *source)
{
	char *s;
	int start, end;

	s = source;

	while (s && (s = strstr(s, "/*"))) {
		start = s - source + 2;

		if (s && (s = strstr(s + 2, "*/"))) {
			end = s - source - 2;
			memset(source + start, ' ', end - start);
		}
	}

	s = source;

	while (s && (s = strstr(s,"//"))) {
		start = s - source;

		if (s && (s = strchr(s + 2, '\n'))) {
			end = s - source;
			memset(source + start, ' ', end - start);
		}
	}
}

void getWord(char *buf, char *buf1)
{
	char *s, *e;
	char tmp[132] = { 0, };

	if ((s = strchr(buf, '(')) && (e = strchr(buf, ')'))) {
		strncpy(tmp, s + 1, e - s - 1);
		sscanf(tmp, " %s ", buf1);
	}
}

int countLine(int start, int end, const char *buf)
{
	int line = 0;

	for (int i = start; i < end; i++) {
		if (buf[i] == '\n')
			line++;
	}

	return line;
}

bool TimesetCompile(const char *buffer)
{
	char group[32] = "dfinc", name[32] = { 0 }, width[10] = { 0 }, name2[32] = { 0 }, width2[10] = { 0 };
	char *group_end, *sub_end, *s;
	char *buf = (char *) buffer;

	while ((buf = strstr(buf, "\nTIMESET("))) {
		if ((group_end = strchr(buf, '}'))) {
			*group_end = '\0';
		}
		else {
			break;
		}

		getWord(buf, group);

		if (!tgGroup.Lookup(group, timeCompiler)) {
			timeCompiler = new TGCompiler;
			tgGroup[group] = timeCompiler;
		}

		while ((buf = strstr(buf, "SET_TS"))) {
			if ((s = strchr(buf, '('))) {
				sscanf(s + 1, "%[^,],%[^,]", name2, width2);
				sscanf(name2, "%s", name);
				sscanf(width2, "%s", width);
				s = strchr(s, ',');

				if ((s = strchr(s, '\n'))) {
					s += 1;

					if ((sub_end = strchr(s, ')'))) {
						*sub_end = '\0';
						buf = sub_end + 1;
					}

					while (strchr(s,'\n')) {
						*(strchr(s, '\n')) = ' ';
					}

					String str_tsn;
					str_tsn	= "ts=";
					str_tsn += name;
					str_tsn += " {  width=";
					str_tsn += width;
					str_tsn += " { ";
					str_tsn += s;
					str_tsn += " } ";

					int line = countLine(0, s - buffer, src_org);
					timeCompiler->setLogo(strSrcFile.c_str());
					timeCompiler->splitSyntex((char *) str_tsn, line + 1);

					if (timeCompiler->error()) {
						printf(timeCompiler->getError());
						return false;
					}
				}
			}
		}

		buf = group_end + 1;
	}

	if (timeCompiler == NULL) {
		char *p;
		buf = (char *) buffer;

		timeCompiler = new TGCompiler;
		tgGroup[group] = timeCompiler;

		while ((buf = strstr(buf, "SET_TS("))) {
			if ((group_end = strchr(buf, '\n'))) {
				*group_end = '\0';
			}
			else {
				break;
			}

			p = strtok(buf + 3, "\" ,);");

			while (p) {
				TTGDATA	*tg = new TTGDATA;
				strcpy(tg->name, p);
				timeCompiler->ts()->list.AddTail(tg);
				p = strtok(NULL, "\" ,);");
			}

			buf = group_end + 1;
		}
	}

	return true;
}

bool PatternSET_SUB(char *buf, const char *all_buf, char *strSubName, PGCompiler *patternCompiler)
{
	char *sub_end;
	char *s;
	char name[64] = { 0 };
	int line;

	while ((buf = strstr(buf, "SET_SUB"))) {
		sub_end = 0;

		if ((s = strchr(buf, '('))) {
			sscanf(s + 1, " %s", name);

			if (name[strlen(name) - 1] == ',') {
				name[strlen(name) - 1] = 0;
			}

			strcat(strSubName, name);
			strcat(strSubName, " ");

			if ((s = strchr(s, ','))) {
				s += 1;

				if ((sub_end = strchr(s, ')'))) {
					*sub_end = '\0';
					buf = sub_end + 1;
				}

				if (strchr(s, ',')) {
					*(strchr(s, ',')) = ' ';
				}

				String str_sub;
				str_sub	= "sub=";
				str_sub += name;
				str_sub += " {  ";
				str_sub += s;
				str_sub += " } ";
				line = countLine(0, s - all_buf, src_org);
				patternCompiler->setLogo(strSrcFile.c_str());
				patternCompiler->splitSyntex((char *) (const char *) str_sub, line + 1);

				if (patternCompiler->error()) {
					printf(patternCompiler->getError());
					return false;
				}
				else if (patternCompiler->pc() > 2047) {
					printf("%s:%d:too large pattern line '%s'\n",
							(char *) strSrcFile.c_str(), line + 1, name);
					return false;
				}
			}
		}

		if (sub_end) {
			*sub_end = ')';
		}
	}

	return true;
}

bool PatternDeviceDefault(char *buf, const char *all_buf, char *strSubName, PGCompiler *patternCompiler)
{
	bool b;
	unsigned int n;
	char *s;
	char *dbuf;
	const char *dbuffer;
	char def_number[1024];
	string def_src;

	for (;;) {
		if ((s = strstr(buf, "device_default")) == NULL) {
			break;
		}

		buf = s + strlen("device_default");
		memset(def_number, 0, sizeof(def_number));

		for (n = 0; *buf >= '0' && *buf <= '9'; n++) {
			def_number[n] = *buf;
			buf++;
		}

		if (strlen(def_number) == 0) {
			snprintf(def_number, sizeof(def_number), "@");
		}

		for (b = false, n = 0; n < def_parse.size(); n++) {
			if (strcmp(def_parse[n].c_str(), def_number) == 0) {
				b = true;
				break;
			}
		}

		if (b == false) {
			dbuf = (char *) all_buf;
			def_src	= deviceDefault(dbuf, def_number);
			def_parse.push_back(def_number);
			def_str.push_back(def_src);
			dbuf = (char *) def_src.c_str();
			dbuffer = dbuf;
		}
		else {
			dbuf = (char *) def_str[n].c_str();
			dbuffer = dbuf;
		}

		if (PatternSET_SUB(dbuf, dbuffer, strSubName, patternCompiler) == false) {
			return false;
		}
	}

	return true;
}

bool PatternCompile(const char *buffer)
{
	patternMap patternGroup;
	PGCompiler *patternCompiler = NULL;
	String str, strPC;
	bool nosub = false, nogroup = false;
	//char strSubName[102400] = "";
	char strSubName[409600] = "";
	char group[32] = { 0 }, name[64] = { 0 }, ts_group[20] = "dfinc";
	char *group_end, *s;
	char *buf = (char *) buffer;

	while ((buf = strstr(buf, "\nPATTERN("))) {
		if (patternCompiler) {
			patternCompiler->init();
			delete patternCompiler;
		}

		group_end = 0;
		s = buf;

		if ((s = strstr(s, "SET_PATTERN")) == 0) {
			s = buf;
		}

		if ((group_end = strchr(s, '}'))) {
			*group_end = '\0';
		}
		else {
			break;
		}

		getWord(buf, group);
		strcat(strSubName, "_group_start");
		strcat(strSubName, group);
		strcat(strSubName, " ");

		if (!patternGroup.Lookup(group, patternCompiler)) {
			patternCompiler = new PGCompiler;
			patternGroup[group] = patternCompiler;
		}

		if ((s = strstr(buf, "LOAD"))) {
			if ((s = strstr(s, "TIMESET"))) {
				if ((s = strchr(s, ','))) {
					sscanf(s + 1, " %s ", ts_group);
				}
			}
		}

		if (tgGroup.Lookup(ts_group, timeCompiler)) {
			int timeset = timeCompiler->ts()->timeset;

			for (POSITION pos = timeCompiler->ts()->list.GetHeadPosition(); pos; timeset++) {
				TTGDATA	*tg = timeCompiler->ts()->list.GetNext(pos);
				patternCompiler->addTsGroup(tg->name, timeset);
			}
		}
		else {
			int line = countLine(0, s - buffer, src_org);
			printf("%s:%d: '%s' undeclared (first use this timeset)\n",
					(char *) strSrcFile.c_str(), line + 1, ts_group);
			return false;
		}

		if (PatternDeviceDefault(buf, buffer, strSubName, patternCompiler) == false) {
			return false;
		}

		if (PatternSET_SUB(buf, buffer, strSubName, patternCompiler) == false) {
			return false;
		}

		if (pc_line && !patternCompiler->error()) {
			str.sprintf("'%s'=%d ", group, patternCompiler->pc());
			strPC += str;
		}

		if (group_end) {
			*group_end = '}';
		}

		buf = group_end + 1;
		strcat(strSubName, "_group_end ");
	}

	bool pre_process;
	char *x, *next = 0;
	bool find;

	buf = (char *) buffer;
	char groupname[100] = "";

	while ((buf = strstr(buf + 4, "LOAD( PATTERN"))) {
		sscanf(buf, "LOAD( PATTERN , %s", name);

		if ((x = strchr(name, ')'))) {
			*x = 0;
		}

		strcpy(groupname, "_group_start");
		strcat(groupname, name);
		strcat(groupname, " ");

		if (!(s = strstr(strSubName, groupname))) {
			printf("%s:%d:no group '%s'\n",
					(char *) strSrcFile.c_str(),
					countLine(0, buf - buffer, src_org) + 1, name);
			buf += 20;
			nogroup = true;
			continue;
		}

		group_end = strstr(s, "_group_end");

		if (group_end) {
			*group_end = 0;
		}

		if ((next = strstr(buf + 20, "LOAD( PATTERN"))) {
			*next = 0;
		}

		if ((x = strstr(buf, "MPAT"))) {
			pre_process = false;
			buf = x;
			find = false;

			while (x--) {
				if (*x == '\n' || *x == ' ') {
					break;
				}
				else if (*x == '#') {
					pre_process = true;
					break;
				}
			}

			sscanf(x, " %[^(]", name);

			if (strcmp(name, "START_MPAT") && strcmp(name, "MEAS_MPAT")) {
				buf += 4;

				if (next) {
					*next = 'L';
				}

				continue;
			}

			if (!pre_process && sscanf(buf, "%[^\"]\"%[^\"]\"", group, name) == 2) {
				find = false;

				while ((s = strstr(s, name))) {
					if ((x = strchr(s, ' ' ))) {
						*x = 0;

						if (strcmp(s, name) == 0) {
							if (isspace(*(s - 1))) {
								find = true;
								*x = ' ';
								break;
							}
						}

						*x = ' ';
					}

					s += 1;
				}

				if (!find) {
					nosub = true;
					printf("%s:%d:no sub '%s'\n",
							(char *) strSrcFile.c_str(),
							countLine(0, buf - buffer, src_org) + 1, name);
				}
			}
		}

		buf += 4;

		if (next) {
			*next = 'L';
		}

		if (group_end) {
			*group_end = '_';
		}
	}

	if (nogroup || nosub) {
		return false;
	}

	if (!strPC.isEmpty()) {
		printf("%s\n", (char *) strPC);
	}

	return true;
}

int write_source(char *srcfname)
{
	unsigned int total;
	off_t len;
	int fd;
	int ret;
	char buf[1024] = { 0 };
	char *p;
	string dir;
	string fname;
	string::size_type idx;
	vector<char> src_str;

	dir = srcfname;
	idx = dir.rfind("/");

	if ((idx = dir.rfind("/")) == string::npos) {
		snprintf(buf, sizeof(buf), "tar cfj %s.src_tmp %s > /dev/null 2>&1",
				srcfname, srcfname);
	}
	else {
		fname = dir.substr(idx + 1);
		dir = dir.substr(0, idx);
		snprintf(buf, sizeof(buf), "cd %s && tar cfj %s.src_tmp %s > /dev/null 2>&1",
				dir.c_str(), fname.c_str(), fname.c_str());
	}

	if (system(buf) != 0) {
		return -1;
	}

	snprintf(buf, sizeof(buf), "%s.src_tmp", srcfname);

	if ((fd = open(buf, O_RDONLY)) < 0) {
		return -1;
	}

	if ((len = lseek(fd, 0, SEEK_END)) < 0) {
		close(fd);
		return -1;
	}

	if (lseek(fd, 0, SEEK_SET) < 0) {
		close(fd);
		return -1;
	}

	src_str.resize(strlen(START_CODE) + strlen(END_CODE) + len + 1);
	memset(&src_str[0], 0, src_str.size());
	p = &src_str[0] + strlen(START_CODE);

	for (total = 0; total < (unsigned int) len;) {
		if ((ret = read(fd, p + total, len - total)) < 0) {
			if (errno == EAGAIN || errno == EINTR) {
				continue;
			}

			close(fd);
			return -1;
		}

		total += ret;
	}

	close(fd);
	snprintf(buf, sizeof(buf), "rm -f %s.src_tmp", srcfname);

	if (system(buf) != 0) {
		return -1;
	}

	memcpy(&src_str[0], START_CODE, strlen(START_CODE));
	memcpy(p + total, END_CODE, strlen(END_CODE));

	snprintf(buf, sizeof(buf), "%s.src_tmp", srcfname);

	if ((fd = open(buf, O_CREAT | O_TRUNC | O_WRONLY, 0644)) < 0) {
		return -1;
	}

	for (total = 0; total < src_str.size();) {
		if ((ret = write(fd, &src_str[0] + total, src_str.size() - total)) < 0) {
			if (errno == EAGAIN || errno == EINTR) {
				continue;
			}

			return -1;
		}

		total += ret;
	}

	close(fd);
	return 0;
}

int write_outfile(char *outfname, char *srcfname)
{
	unsigned int total;
	int ret;
	int fd;
	char buf[1024] = { 0 };

	if (srcfname != NULL) {
		if (write_source(srcfname) < 0) {
			return -1;
		}
	}

	if ((fd = open(outfname, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
		return -1;
	}

	snprintf(buf, sizeof(buf), "extern \"C\" int getTime() {\n\treturn %d;\n}\n", (int) time(0));

	for (total = 0; total < strlen(buf);) {
		if ((ret = write(fd, buf + total, strlen(buf) - total)) < 0) {
			if (errno == EAGAIN || errno == EINTR) {
				continue;
			}

			return -1;
		}

		total += ret;
	}

	close(fd);
	return 0;
}

int main(int argc, char* argv[])
{
	string strOutFile;
	string strFile;
	char *file_buffer;
	string strPattern;
	string strTimeset;
	unsigned int total;
	int ret;
	int fd;
	bool help = false;
	bool src = false;

	for (int i = 1; i < argc; i++) {
		if (strstr(argv[i], "-c")) {
			strSrcFile = argv[++i];
		}
		else if(strstr(argv[i], "-o")) {
			strOutFile = argv[++i];
		}
		else if(strstr(argv[i], "-s")) {
			src = true;
		}
		else if(strstr(argv[i], "-n")) {
			pc_line = true;
		}
		else if(strstr(argv[i], "-t33")) {
			sysModel = 33;
		}
		else if(strstr(argv[i], "-t10")) {
			sysModel = 10;;
		}
		else if (strstr(argv[i], "-h")) {
			help = true;
		}
	}

	if (help == true) {
		printf("%s VERSION : %s\n", argv[0], _VERSION_);
		return 0;
	}

	if (strSrcFile.empty()) {
		printf("Command line error.\n");
		return 1;
	}

	if ((fd = open(strSrcFile.c_str(), O_RDONLY)) < 0) {
		printf("File could not be opened\n");
		return 1;
	}

	off_t size = lseek(fd, 0, SEEK_END);

	if (size < 0) {
		return 1;
	}

	if (lseek(fd, 0, SEEK_SET) < 0) {
		return 1;
	}

	file_buffer = new char[size + 1];

	for (total = 0; total < (unsigned int) size;) {
		if ((ret = read(fd, file_buffer + total, size - total)) < 0) {
			if (errno == EAGAIN || errno == EINTR) {
				continue;
			}

			return 1;
		}

		total += ret;
	}

	file_buffer[size] = 0;
	strFile = file_buffer;
	src_org = (char *) strFile.c_str();
	eatComment(file_buffer);
	strPattern = file_buffer;
	strTimeset = file_buffer;
	delete[] file_buffer;
	close(fd);

	if (TimesetCompile(strTimeset.c_str()) == false || PatternCompile(strPattern.c_str()) == false) {
		return 1;
	}

	if (strOutFile.empty()) {
		return 0;
	}

	if (write_outfile((char *) strOutFile.c_str(), src == true ? (char *) strSrcFile.c_str() : NULL) < 0) {
		return 1;
	}

	return 0;
}
