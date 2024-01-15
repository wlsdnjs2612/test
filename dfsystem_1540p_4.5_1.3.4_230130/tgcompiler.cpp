
#include <ctype.h>
#include <time.h>

#include "tgcompiler.h"

enum
{
	TSCM_NOP,
	TSCM_LINE,
	TSCM_MAX_LENGTH,
	TSCM_RATE,
	TSCM_CLK,
	TSCM_BIT,
	TSCM_ORDER,
	TSCM_TIMESET,
	TSCM_BASE,
	TSCM_CLKS,
	TSCM_CLKS2,
	TSCM_PIN_ASSIGN,
	TSCM_TS_RATE,
	TSCM_CLK_INV
};

TGCompiler::TGCompiler() : SysCompiler()
{
	timeset.timeset = 0;
	timeset.clk_inv = 0;
	timeset.clk_inv2 = 0;

	for (unsigned int i = 0; i < 32; i++) {
		timeset.magic[i] = 0;
	}

	createSyntex();
}

TGCompiler::~TGCompiler()
{
	for (POSITION pos = timeset.chlist.GetHeadPosition(); pos;) {
		delete timeset.chlist.GetNext(pos);
	}

	for (POSITION pos = timeset.list.GetHeadPosition(); pos;) {
		delete timeset.list.GetNext(pos);
	}

	timeset.chlist.RemoveAll();
	timeset.list.RemoveAll();
}

#define COMMAND(a, b, c)	((a << 24) | (b << 16) | c)

void TGCompiler::createSyntex()
{
	syntex_map["assign"] = COMMAND(TSCM_PIN_ASSIGN, 0, 0);
	syntex_map["ts="] = COMMAND(TSCM_TIMESET, 0, 0);
	syntex_map["width="] = COMMAND(TSCM_BASE, 0, 0);
	syntex_map["\n"] = COMMAND(TSCM_LINE, 0, 0);
	syntex_map[";"] = COMMAND(TSCM_NOP, 0, 0);
	syntex_map["clock_inv"] = COMMAND(TSCM_CLK_INV, 0, 0);

	syntex_map["tri"] = COMMAND(TSCM_CLKS, 12, 1);
	syntex_map["strobe"] = COMMAND(TSCM_CLKS, 8, 2);
	syntex_map["strobe1"] = COMMAND(TSCM_CLKS, 10, 3);
	syntex_map["amux"] = COMMAND(TSCM_CLKS, 0, 4);
	syntex_map["dmux"] = COMMAND(TSCM_CLKS, 2, 5);
	syntex_map["addh"] = COMMAND(TSCM_CLKS, 16, 6);
	syntex_map["addm"] = COMMAND(TSCM_CLKS, 14, 0);
	syntex_map["16addr"] = COMMAND(TSCM_CLKS, 18, 7);
	syntex_map["scan0"] = COMMAND(TSCM_CLKS, 4, 8);
	syntex_map["dinv"] = COMMAND(TSCM_CLKS, 6, 9); //scan1->dinv
	syntex_map["scan2"] = COMMAND(TSCM_CLKS, 20, 10);
	syntex_map["scan3"] = COMMAND(TSCM_CLKS, 22, 11);
	syntex_map["vmux"] = COMMAND(TSCM_CLKS, 89, 12);

	char szClk[10] = { 0 };

	for (unsigned int i = 0; i < 16; i++) {
		sprintf(szClk, "c%d", i);
		syntex_map[szClk] = COMMAND(TSCM_CLKS, 24 + i * 4, i + 16);
	}

	for (unsigned int i = 0; i < 16; i++) {
		sprintf(szClk, "c%d", i + 16);
		syntex_map[szClk] = COMMAND(TSCM_CLKS2, i * 2, i + 16);
	}
}

void TGCompiler::init()
{
	SysCompiler::init();

	for (POSITION pos = timeset.chlist.GetHeadPosition(); pos;) {
		delete timeset.chlist.GetNext(pos);
	}

	for (POSITION pos = timeset.list.GetHeadPosition(); pos;) {
		delete timeset.list.GetNext(pos);
	}

	timeset.chlist.RemoveAll();
	timeset.list.RemoveAll();
	timeset.clk_inv = 0;
	timeset.clk_inv2 = 0;
}

String TGCompiler::timeString()
{
	int i = 0;
	char msg[100];
	String strTime;
	char clks[32][10] = { "c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9", "c10", "c11", "c12", "c13", "c14", "c15",
			"amux", "dmux", "scan0", "dinv", "strobe", "strobe1", "tri", "addm", "addh", "16addr", "scan2", "scan3",
			"vmux", "", "" };
	char clks2[16][10] = { "c16", "c17", "c18", "c19", "c20", "c21", "c22", "c23",
			"c24", "c25", "c26", "c27", "c28", "c29", "c30", "c31" };

	for (POSITION pos = timeset.list.GetHeadPosition(); pos;) {
		TTGDATA *tg = timeset.list.GetNext(pos);
		sprintf(msg, "ts=%s\n{\n\twidth=%d\n", tg->name, tg->width);
		strTime += msg;

		if (!i && (timeset.clk_inv || timeset.clk_inv2)) {
			strTime += "\tclock_inv   ";

			for (i = 0; i < 29; i++) {
				if ((timeset.clk_inv & (1 << i))) {
					sprintf(msg, "%s ", clks[i]);
					strTime += msg;
				}
			}

			for (i = 0; i < 16; i++) {
				if ((timeset.clk_inv2 & (1 << i))) {
					sprintf(msg, "%s ", clks2[i]);
					strTime += msg;
				}
			}

			strTime += "\n";
		}

		for (POSITION p = timeset.chlist.GetHeadPosition(); p;) {
			TCHANNEL *ch = timeset.chlist.GetNext(p);
			TPGCMD cmd;

			cmd.value = ch->sys;
			sprintf(msg, "\t%-8s:\t", ch->name);
			strTime += msg;

			if (cmd.c.command == TSCM_CLKS2) {
				if ((tg->data2[cmd.c.opt] & 0x8000) &&
						(tg->data2[cmd.c.opt] & 0x7fff) &&
						(tg->data2[cmd.c.opt] & 0x7fff) < tg->width - 2) {
					sprintf(msg, "%d ", tg->data2[cmd.c.opt] & 0x7fff);
					strTime += msg;
				}

				if ((tg->data2[cmd.c.opt + 1] & 0x8000) &&
						(tg->data2[cmd.c.opt + 1] & 0x7fff) &&
						(tg->data2[cmd.c.opt + 1] & 0x7fff) < tg->width - 2) {
					sprintf(msg, "%d ", tg->data2[cmd.c.opt + 1] & 0x7fff);
					strTime += msg;
				}
			}
			else {
				if ((tg->data[cmd.c.opt] & 0x8000) &&
						(tg->data[cmd.c.opt] & 0x7fff) &&
						(tg->data[cmd.c.opt] & 0x7fff) < tg->width - 2) {
					sprintf(msg, "%d ", tg->data[cmd.c.opt] & 0x7fff);
					strTime += msg;
				}

				if ((tg->data[cmd.c.opt + 1] & 0x8000) &&
						(tg->data[cmd.c.opt + 1] & 0x7fff) &&
						(tg->data[cmd.c.opt + 1] & 0x7fff) < tg->width - 2) {
					sprintf(msg, "%d ", tg->data[cmd.c.opt + 1] & 0x7fff);
					strTime += msg;
				}

				if (cmd.c.opt >= 24) {
					if (tg->data[cmd.c.opt + 2] & 0x8000 &&
							(tg->data[cmd.c.opt + 2] & 0x7fff) &&
							(tg->data[cmd.c.opt + 2] & 0x7fff) < tg->width - 2) {
						sprintf(msg, "%d ", tg->data[cmd.c.opt + 2] & 0x7fff);
						strTime += msg;

						if ((tg->data[cmd.c.opt + 3] & 0x8000) &&
								(tg->data[cmd.c.opt + 3] & 0x7fff) &&
								(tg->data[cmd.c.opt + 3] & 0x7fff) < tg->width - 2) {
							sprintf(msg, "%d ", tg->data[cmd.c.opt + 3] & 0x7fff);
							strTime += msg;
						}
					}
				}
			}

			strTime += "\n";
		}

		strTime += "}\n";
	}

	return strTime;
}

void TGCompiler::splitSyntex(char *source, int line)
{
	char	*word;
	char	p1 = 0, p, p2 = 0, p3;
	int	commant = 0;
	int	i = 0;
	int	nline = line;
	int	nstart = 0;
	char	*new_source = new char[327670];
	subList	subString;

	total_line = line;

	if (strlen(source) == 0)
		return;

	i = 0;
	word = strtok(source, " \t\r");

	while (word) {
		p1 = 0;

		while ((p = *word)) {
			word++;

			if (commant == 1) {
				if (p == '\n') {
					commant = 0;
					nline++;
					new_source[i++] = p;
					new_source[i++] = ' ';
				}

				p1 = p;
				continue;
			}
			else if (commant == 2) {
				if (p == '\n') {
					nline++;
					new_source[i++] = p;
					new_source[i++] = ' ';
				}
				else if (p1 == '*' && p == '/')
					commant = 0;

				p1 = p;
				continue;
			}

			if (p == '\n') {
				new_source[i++] = ' ';
				new_source[i++] = p;
				new_source[i++] = ' ';
				p1 = p;
				continue;
			}
			else if (p == ';') {
				new_source[i++] = ' ';
				new_source[i++] = ';';
				new_source[i++] = ' ';
				p1 = p;
				continue;
			}
			else if (p == ':') {
				new_source[i++] = ' ';
				new_source[i++] = ':';
				new_source[i++] = ' ';
				p1 = p;
				continue;
			}
			else if (p == '=') {
				if (p2 == 't' && p1 == 's') {
					i -= 3;

					if (i > 1) {
						new_source[i++] = 0;

						if (nstart)
							nstart++;

						char *pSub = new char[i];
						memcpy(pSub, new_source, i);
						TSUB *sub = new TSUB;
						sub->pos = nstart;
						sub->p = pSub;
						sub->flag = 1;
						subString.AddTail(sub);
					}

					i = 0;
					new_source[i++] = 't';
					new_source[i++] = 's';
					new_source[i++] = p;
					new_source[i++] = ' ';
					p1 = p;
					nstart = nline;
				}
				else {
					new_source[i++] = p;
					new_source[i++] = ' ';
					p1 = p;
				}

				continue;
			}
			else if (p == ' ' && i && new_source[i - 1] == ' ')
				continue;

			switch (p) {
				case '{' :
					break;
				case '}' : {
					new_source[i++] = 0;
					char *pSub = new char[i];
					memcpy(pSub, new_source, i);
					TSUB *sub = new TSUB;
					sub->pos = nstart;
					sub->p = pSub;
					sub->flag = 0;
					subString.AddTail(sub);
					i = 0;
					break;
				}
				case '/' :
					if (p1 == '/') {
						commant = 1;
						i--;
					}
					else
						new_source[i++] = p;
					break;
				case '*' :
					if (p1 == '/') {
						commant = 2;
						i--;
					}
					else
						new_source[i++] = p;
					break;
				default :
					new_source[i++] = p;
			}

			p3 = p2;
			p2 = p1;
			p1 = p;
		}

		new_source[i++] = ' ';
		word = strtok(0, " \t\r");
	}

	if (nstart)
		nstart++;

	new_source[i - 1] = 0;
	char *pSub = new char[i];
	memcpy(pSub, new_source, i);
	TSUB *sub = new TSUB;
	sub->pos = nstart;
	sub->p = pSub;
	sub->flag = 1;
	subString.AddTail(sub);
	delete new_source;

	for (POSITION pos = subString.GetHeadPosition(); pos;) {
		TSUB *sub = subString.GetNext(pos);

		if (sub->flag != 0x1)
			subCompile(sub->p, sub->pos);

		delete sub->p;
		delete sub;
	}

	subString.RemoveAll();
	comp_time = ::time(0);
}

void TGCompiler::clkInv(char *word)
{
	int bit, mask;
	TPGCMD cmd;

	word = macro(0);

	while (word && *word != ';' && *word != '\n') {
		if (word == 0) {
			addError(226, "clock_inv syntex error", word);
		}
		else if (!syntex_map.Lookup(word, cmd.value)) {
			addError(227, "undefined", word);
		}
		else {
			if (cmd.c.command == TSCM_CLKS2) {
				bit = cmd.c.opt / 2;
				mask = 1 << bit;
				timeset.clk_inv2 |= mask;
			}
			else {
				if (cmd.c.opt > 22 && cmd.c.opt < 89) {
					bit = (cmd.c.opt - 24) / 4;
				}
				else {
					bit = ((cmd.c.opt >= 89 ? cmd.c.opt - 65 : cmd.c.opt) / 2) + 16;
				}

				mask = 1 << bit;
				timeset.clk_inv |= mask;
			}
		}

		word = macro(0);
	}
}

void TGCompiler::pinAssign(char *word)
{
	TPGCMD cmd;
	char *pMacro = strtok(0, " ");

	if (pMacro == 0) {
		addError(201, "assign syntex error", word);
	}

	word = macro(0);

	if (pMacro == 0) {
		addError(202, "assign syntex error", pMacro);
	}

	String strDefine;

	if (macro_map.Lookup(pMacro, strDefine)) {
		addError(203, "redefined", pMacro);
	}
	else if (!syntex_map.Lookup(word, cmd.value)) {
		addError(204, "undefined", word);
	}
	else {
		macro_map[pMacro] = word;
		TCHANNEL *ch = new TCHANNEL;
		ch->sys = cmd.value;
		strcpy(ch->name, pMacro);
		timeset.chlist.AddTail(ch);
	}
}

int TGCompiler::subCompile(char *source, int /*nline*/)
{
	char		*endptr;
	unsigned int	value;
	TTGDATA		time;
	int 		order = 0;
	unsigned int 	rates[] = { 10, 20, 50, 100, 200, 500, 1000, 2000 };

	memset(&time, 0, sizeof(time));
	time.rate = 2;

	char *word = strtok(source, " ");

	while (word) {
		TPGCMD cmd;

		if (!syntex_map.Lookup(word, cmd.value)) {
			addError(205, "syntex error", word);
			word = macro(0);
			continue;
		}

		switch (cmd.c.command) {
			case TSCM_NOP :
				break;
			case TSCM_PIN_ASSIGN :
				pinAssign(word);
				break;
			case TSCM_CLK_INV :
				clkInv(word);
				break;
			case TSCM_LINE :
				total_line++;
				break;

			case TSCM_TIMESET : {	// timeset = hfhjfdhjd
				word = macro(0);

				if (time.name[0])
					addError(206, "timeset redefined", word);
				else {
					strcpy(time.name, word);

					for (POSITION pos = timeset.list.GetHeadPosition(); pos;) {
						TTGDATA *tg = timeset.list.GetNext(pos);

						if (strcmp(time.name, tg->name) == 0) {
							addError(207, "timeset redefinition", word);
							break;
						}
					}
				}

				break;
			}
			case TSCM_TS_RATE : {
				word = macro(0);	// rate=100
				value = strtoul(word, &endptr, 0);

				if (*endptr != 0 && *endptr != ' ')
					addError(208, "bad suffix on number", word);
				else if (time.rate)
					addError(209, " rate redefined", word);
				else {
					int i;

					for (i = 0; i < 8; i++)
						if (value == rates[i])
							break;

					if (i == 8)
						addError(210, "bad rate number", word);
					else {
						time.rate = value;
					}
				}

				break;
			}
			case TSCM_BASE : {	// width= 100
				word = macro(0);
				value = strtoul(word, &endptr, 0);

				unsigned int max_width = 60000;
				time.rate = 2;

				if (*endptr != 0 && *endptr != ' ')
					addError(211, "bad suffix on number", word);
				else if (time.rate == 0)
					addError(212, "rate undefined", word);
				else if (time.width)
					addError(213, "width redefined", word);
				else if (value > max_width)
					addError(214, "large width on rate", word);
				else {
					time.width = value;
					time.data[0x58] = (value - 30);
				}

				break;
			}
			case TSCM_CLKS : {
				if (time.name[0] == 0) {
					addError(215, "timeset undefined", word);
				}
				else if (time.rate == 0) {
					addError(216, "rate undefined", word);
				}
				else if (time.width == 0) {
					addError(217, "width undefined", word);
				}
				else {
					int old = -1;
					int bit = cmd.c.opt;
					int clk2 = false;

					if (!timeset.list.GetCount()) {
						int i = 0;

						for (POSITION pos = timeset.chlist.GetHeadPosition(); pos; i++) {
							TCHANNEL *ch = timeset.chlist.GetNext(pos);

							if (ch->sys == cmd.value) {
								timeset.order[order++] = i;
								break;
							}
						}
					}

					word = macro(0);

					if (*word != ':') {
						addError(218, "clock set error ", word);
						break;
					}

					word = macro(0);

					while (isdigit(*word) /* != '\n'*/) {
						int from;
						int to = time.width;

						value = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ') {
							addError(219, "bad suffix on number", word);
							break;
						}
						else {
							from = value;
						}

						if (from == 0) {
							int mask;

							if (bit < 24 || bit >= 89) {
								mask = 1 << (((bit == 89 ? bit - 65 : bit) / 2) + 16);
							}
							else {
								mask = 1 << ((bit - 24) / 4);
							}

							if (timeset.clk_inv & mask) {
								word = strtok(0, " \t\r,");
								value = strtoul(word, &endptr, 0);

								if (*endptr != 0 && *endptr != ' ') {
									continue;
								}
								else {
									from = value;
								}
							}
						}

						if (from > time.width) {
							addError(220, "large number on clock", word);
							break;
						}
						else if (from <= old) {
							addError(221, "small number on clock", word);
							break;
						}

						old = from;
						word = strtok(0, " \t\r,");

						if (word && isdigit(*word) /*!= '\n'*/) {
							value = strtoul(word, &endptr, 0);

							if (*endptr != 0 && *endptr != ' ') {
								addError(222, "bad suffix on number", word);
								break;
							}
							else {
								to = value;
							}

							if (to > time.width) {
								addError(223, "large number on clock", word);
								break;
							}
							else if (to <= old) {
								addError(224, "small number on clock", word);
								break;
							}
							else {
								old = to;
							}

							word = strtok(0, " \t\r,");
						}

						time.data[bit] = from | 0x8000;

						if (to < time.width) {
							time.data[bit + 1] = to | 0x8000;
						}

						if ((bit < 24 || bit >= 89 || clk2) && isdigit(*word)) {
							addError(225, "bad suffix on number", word);
							break;
						}
						else {
							bit += 2;
							clk2 = true;
						}
					}

					if (*word == '\n') {
						total_line++;
					}
					else if (*word != ';') {
						continue;
					}
				}

				break;
			}
			case TSCM_CLKS2 : {
				if (time.name[0] == 0) {
					addError(215, "timeset undefined", word);
				}
				else if (time.rate == 0) {
					addError(216, "rate undefined", word);
				}
				else if (time.width == 0) {
					addError(217, "width undefined", word);
				}
				else {
					int old = -1;
					int bit = cmd.c.opt;
					int clk2 = false;

					if (!timeset.list.GetCount()) {
						int i = 0;

						for (POSITION pos = timeset.chlist.GetHeadPosition(); pos; i++) {
							TCHANNEL *ch = timeset.chlist.GetNext(pos);

							if (ch->sys == cmd.value) {
								timeset.order[order++] = i;
								break;
							}
						}
					}

					word = macro(0);

					if (*word != ':') {
						addError(218, "clock set error ", word);
						break;
					}

					word = macro(0);

					while (isdigit(*word) /* != '\n'*/) {
						int from;
						int to = time.width;

						value = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ') {
							addError(219, "bad suffix on number", word);
							break;
						}
						else {
							from = value;
						}

						if (from == 0) {
							unsigned int mask = 1 << (bit / 2);

							if (timeset.clk_inv2 & mask) {
								word = strtok(0, " \t\r,");
								value = strtoul(word, &endptr, 0);

								if (*endptr != 0 && *endptr != ' ') {
									continue;
								}
								else {
									from = value;
								}
							}
						}

						if (from > time.width) {
							addError(220, "large number on clock", word);
							break;
						}
						else if (from <= old) {
							addError(221, "small number on clock", word);
							break;
						}

						old = from;
						word = strtok(0, " \t\r,");

						if (word && isdigit(*word) /*!= '\n'*/) {
							value = strtoul(word, &endptr, 0);

							if (*endptr != 0 && *endptr != ' ') {
								addError(222, "bad suffix on number", word);
								break;
							}
							else {
								to = value;
							}

							if (to > time.width) {
								addError(223, "large number on clock", word);
								break;
							}
							else if (to <= old) {
								addError(224, "small number on clock", word);
								break;
							}
							else {
								old = to;
							}

							word = strtok(0, " \t\r,");
						}

						time.data2[bit] = from | 0x8000;

						if (to < time.width) {
							time.data2[bit + 1] = to | 0x8000;
						}

						if (clk2 && isdigit(*word)) {
							addError(225, "bad suffix on number", word);
							break;
						}
						else {
							clk2 = true;
						}
					}

					if (*word == '\n') {
						total_line++;
					}
					else if (*word != ';') {
						continue;
					}
				}

				break;
			}
		}

		word = macro(0);
	}

	if (time.name[0] == 0) {
		addError(226, "timeset undefined", "");
		return 0;
	}

	TTGDATA *tg = new TTGDATA;
	memcpy(tg, &time, sizeof(TTGDATA));
	timeset.list.AddTail(tg);

	return 1;
}
