
#include <ctype.h>
#include <time.h>

#include "pgcompiler.h"

enum {
	PTCM_LINE,
	PTCM_OPLINE,
	PTCM_DEFINE,
	PTCM_MCC_OPCODE,
	PTCM_OPTION,
	PTCM_MCC_BIT,
	PTCM_MCC_DBG,
	PTCM_LABEL,
	PTCM_INCLUDE,
	PTCM_FUNCTION,
	PTCM_TIMESET,
	PTCM_MINMAX,
	PTCM_X,
	PTCM_X_FM,
	PTCM_XADD,
	PTCM_Y,
	PTCM_Y_FM,
	PTCM_YADD,
	PTCM_D,
	PTCM_DADD,
	PTCM_B,
	PTCM_BADD,
	PTCM_S,
	PTCM_BS,
	PTCM_S_FM,
	PTCM_SADD,
	PTCM_V,
	PTCM_VADD,
	PTCM_NOSUPPORT
};

PGCompiler::PGCompiler() : SysCompiler()
{
	global_pc = 1;
	createSyntex();
}

PGCompiler::~PGCompiler()
{
	for (POSITION pos = pattern.GetHeadPosition(); pos;) {
		TPGDATA *pt = pattern.GetNext(pos);

		if (pt->pram) {
			for (POSITION pos2 = pt->pram->GetHeadPosition(); pos2;)
				delete pt->pram->GetNext(pos2);

			pt->pram->RemoveAll();
			delete pt->pram;
		}

		delete pt;
	}

	pattern.RemoveAll();
	tsTimeGroup.RemoveAll();

	for (POSITION pos = noSub.GetHeadPosition(); pos;)
		delete noSub.GetNext(pos);

	noSub.RemoveAll();
}

void PGCompiler::init()
{
	SysCompiler::init();
	global_pc = 1;

	for (POSITION pos = pattern.GetHeadPosition(); pos;) {
		TPGDATA *pt = pattern.GetNext(pos);

		if (pt->pram) {
			for (POSITION pos2 = pt->pram->GetHeadPosition(); pos2;)
				delete pt->pram->GetNext(pos2);

			pt->pram->RemoveAll();
			delete pt->pram;
			pt->pram = 0;
		}

		delete pt;
	}

	pattern.RemoveAll();
	tsTimeGroup.RemoveAll();

	for (POSITION pos = noSub.GetHeadPosition(); pos;)
		delete noSub.GetNext(pos);

	noSub.RemoveAll();
}

void PGCompiler::addTsGroup(const char *timeset, int ts)
{
	if (ts == 0) {
		tsTimeGroup.RemoveAll();
	}

	tsTimeGroup[(char *) timeset] = (unsigned int) ts;
}

void PGCompiler::changeJump(const char *name, int pc)
{
	for (POSITION pos = pattern.GetHeadPosition(); pos;) {
		TPGDATA *pt = pattern.GetNext(pos);

		if (name && strcmp(name, pt->name) != 0)
			continue;

		pt->pc -= pc;

		for (POSITION rampos = pt->pram->GetHeadPosition(); rampos;) {
			TRAMDATA *ram = pt->pram->GetNext(rampos);
			ram->j.jump_add -= pc;
		}
	}
}

void PGCompiler::createSyntex()
{
#define COMMAND(a, b, c)	((a << 24) | (b << 16) | c)
#define AddPatternSyntex(syntex, command, opt, bit)	syntex_map[syntex] = COMMAND(command, opt, bit);
#include "pattern.stx"

	AddPatternSyntex("%pragma", PTCM_OPTION, 0, 0)

#undef AddPatternSyntex
#undef COMMAND

	macro_map["fm_status"] = "0x8001";
	macro_map["fm_fails"] = "0x8002";
	macro_map["fm_block"] = "0x8003";
	macro_map["IO_1fail"] = "0x8004";
	macro_map["IO_2fail"] = "0x8005";
	macro_map["IO_3fail"] = "0x8006";
	macro_map["IO_4fail"] = "0x8007";
	macro_map["IO_5fail"] = "0x8008";
	macro_map["IO_6fail"] = "0x8009";
	macro_map["IO_7fail"] = "0x800a";
	macro_map["IO_8fail"] = "0x800b";
	macro_map["fm_ecc"] = "0x800c";

	macro_map["pre_result"] = "0x800f";
	macro_map["init_result"] = "0x8010";
	macro_map["id_result"] = "0x8011";
	macro_map["status_result"] = "0x8012";
	macro_map["read_chip_data"] = "0x8012";
	macro_map["scan_result"] = "0x8013";
	macro_map["block_result"] = "0x8014";
	macro_map["128block_result"] = "0x8015";
	macro_map["L1_result"] = "0x8016";
	macro_map["ecc_result"] = "0x8017";
	macro_map["io_result"] = "0x8018";
	macro_map["display_result"] = "0x8019";
	macro_map["pretest_data"] = "0x8020";
	macro_map["pretest_result"] = "0x8021";
	macro_map["bitest_data"] = "0x8022";
	macro_map["bitest_result"] = "0x8023";
	macro_map["latch"] = "0x8024";
	macro_map["read_chip_id"] = "0x8025";
	macro_map["chip_id_result"] = "0x8026";
	macro_map["fm_read_data"] = "0x8027";
	macro_map["dq_data"] = "0x8028";
	macro_map["bist_io_latch"] = "0x8100";
	macro_map["bist_data"] = "0x8101";
	macro_map["bist_result"] = "0x8102";
	macro_map["bist2_result"] = "0x8103";
	macro_map["emmc_io_latch"] = "0x8110";
	macro_map["emmc_data"] = "0x8111";
	macro_map["emmc_init"] = "0x8112";
	macro_map["emmc_result"] = "0x8113";
	macro_map["emmc_block_result"] = "0x8114";
	macro_map["emmc_chip_id"] = "0x8115";
	macro_map["emmc_read_data"] = "0x8116";

	macro_map["aging"] = "0x8201";

	macro_map["scan++"] ="sets sa=sa+1 next_scan";
	macro_map["scan_clear"] ="sa=0 next_page";
}

void PGCompiler::splitSyntex(char *source, int line)
{
	char	*word;
	char	p1 = 0, p, p2 = 0, p3 = 0, p4 = 0, p5 = 0;
	int	commant = 0;
	int	i = 0;
	int	iword = 0;
	int	sub_line = line;
	int	nStart = 0;
	int	current_pc = global_pc;
	int	nSub = 0;
	int	opLine = 0;
	char	arg[132];
	char	buf[132];

	if (strlen(source) < 10)
		return;

	char	*szString = new char[1048576];
	subList	subString;
	String str;
	str = (char *) source;
	String key, value;

	for (POSITION pos = macro_map.GetStartPosition(); pos;) {
		macro_map.GetNextAssoc(pos, key, value);
		str.replace((char *) key, (char *) value);
	}

	source = (char *) str;
	total_line = line;
	i = 0;
	word = strtok(source, " \t\r");

	while (word) {
		p1 = 0;
		iword = 0;

		while ((p = *(word + iword))) {
			iword++;

			if (commant == 1) {
				if (p == '\n') {
					commant = 0;
					sub_line++;
					iword--;
				}

				p1 = p;
				continue;
			}
			else if (commant == 2) {
				if (p == '\n') {
					sub_line++;
					szString[i++] = p;
					szString[i++] = ' ';
				}
				else if (p1 == '*' && p == '/')
					commant = 0;

				p1 = p;
				continue;
			}
			else if (p == '\t')
				p = ' ';

			if (p == '\n') {
				sub_line++;
				szString[i++] = ' ';
				szString[i++] = p;
				szString[i++] = ' ';
				p1 = p;
				word += iword;
				iword = 0;
				continue;
			}
			else if (p == ';') {
				opLine++;
				szString[i++] = ' ';
				szString[i++] = ';';
				szString[i++] = ' ';
				p1 = p;
				continue;
			}
			else if ((p == 'A' && p1 == '=') || (isdigit(p) && (p1 == '=' || p1 == '+' || p1 == '-' || p1 == '|' || p1 == '&'))) {
				szString[i++] = ' ';
				szString[i++] = p;
				p1 = p;
				continue;
			}
			else if (p == '%') {
				char *e;
				int len = strlen(word);
				*(word + len) = ' ';

				if (sscanf((word + iword), "define %s \"%[^\"]", arg, buf) == 2) {
					macro_map[arg] = buf;

					if ((e = strchr((word + iword), ';'))) {
						memset((word + iword) - 1, ' ', e - (word + iword));
						*(word + len) = 0;
						p1 = 0;
						iword = 0;
						word = strtok(NULL, " \t\r");
					}

					continue;
				}
				else
					*(word + len) = 0;
			}
			else if (p == '=') {
				if (p3 == 's' && p2 == 'u' && p1 == 'b') {
					i -= 3;
					szString[i++] = 0;

					if (i > 1) {
						if (nStart)
							nStart++;

						char *pSub = new char[i];
						memcpy(pSub, szString, i);
						TSUB *sub = new TSUB;
						sub->pos = nStart;
						sub->p = pSub;
						sub->flag = 0;
						subString.AddTail(sub);
					}

					i = 0;
					szString[i++] = 's';
					szString[i++] = 'u';
					szString[i++] = 'b';
					szString[i++] = p;
					szString[i++] = ' ';
					p1 = p;
					nStart = sub_line;
					nSub = 1;
					continue;
				}
				else if (p2 == 't' && p1 == 's') {
					szString[i++] = p;
					szString[i++] = ' ';
					p1 = p;
					continue;
				}
				else if (p5 == 'b' && p4 == 'u' && p3 == 'r' && p2 == 's' && p1 == 't') {
					szString[i++] = p;
					szString[i++] = ' ';
					p1 = p;
					continue;
				}
			}
			else if (p == ' ' && i && szString[i - 1] == ' ')
				continue;

			switch (p) {
				case ':' :
					if (!isspace(p1)) {
						i -= iword;
						szString[i++] = ' ';
						szString[i++] = ':';
						szString[i++] = ' ';

						for (int ii = 0; ii < iword - 1; ii++)
							szString[i++] = *(word + ii);

						szString[i++] = ' ';
					}
					else {
						szString[i++] = ' ';
						szString[i++] = p;
						szString[i++] = ' ';
					}

					break;

				case '{' :
					break;

				case '}' : {
					szString[i++] = 0;
					char *pSub = new char[i];
					memcpy(pSub, szString, i);
					TSUB *sub = new TSUB;
					sub->pos = nStart;
					sub->p = pSub;
					sub->flag = nSub;
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
						szString[i++] = p;

					break;

				case '*' :
					if (p1 == '/') {
						commant = 2;
						i--;
					}
					else
						szString[i++] = p;

					break;

				default :
					szString[i++] = p;
			}

			p5 = p4;
			p4 = p3;
			p3 = p2;
			p2 = p1;
			p1 = p;
		}

		szString[i++] = ' ';
		word = strtok(NULL, " \t\r");
	}

	if (nStart)
		nStart++;

	szString[i - 1] = 0;
	char *pSub = new char[i];
	memcpy(pSub, szString, i);
	TSUB *sub = new TSUB;
	sub->pos = nStart;
	sub->p = pSub;
	sub->flag = 0;
	subString.AddTail(sub);
	delete szString;

	for (POSITION pos = subString.GetHeadPosition(); pos;) {
		TSUB *sub = subString.GetNext(pos);

		if (sub->flag == 0x1)
			current_pc = subCompile(sub->p, current_pc, sub->pos);

		delete sub->p;
		delete sub;
	}

	subString.RemoveAll();

	for (POSITION pos = noSub.GetHeadPosition(); pos;) {
		TNOLABEL *no_sub = noSub.GetNext(pos);
		bool bFind = false;
		total_line = no_sub->pc & 0xffff;

		for (POSITION pgpos = pattern.GetHeadPosition(); pgpos;) {
			TPGDATA *patData = pattern.GetNext(pgpos);

			if (strcmp(patData->name, no_sub->label) == 0) {
				int pattern_pos = no_sub->pc >> 24;
				int ram_pos = (no_sub->pc >> 16) & 0xff;
				POSITION patpos = pattern.FindIndex(pattern_pos);

				if (patpos) {
					TPGDATA *pat = pattern.GetAt(patpos);
					POSITION rampos = pat->pram->FindIndex(ram_pos);

					if (rampos) {
						TRAMDATA *ramdata = pat->pram->GetAt(rampos);
						ramdata->j.jump_add = patData->pc;
						bFind = true;
						break;
					}
				}
			}
		}

		if (!bFind)
			addError(171, "sub undefined", no_sub->label);
	}

	global_pc = current_pc;
	comp_time = ::time(0);
}

int PGCompiler::subCompile(char *source, int start_pc, int nline)
{
	TRAMDATA	ram;
	TPGDATA		patData;
	noLabelList	noLabel;
	str2WordMap	labelMap;
	char		*endptr;
	unsigned int	dwValue;
	int		current_pc = 0;

	memset(&patData, 0, sizeof(patData));
	patData.pc = start_pc;
	total_line = nline;
	patData.pram = new ramList;

	memset(&ram, 0, sizeof(TRAMDATA));
	char *word = strtok(source, " ");

	while (word) {
		TPGCMD cmd;

		if (!syntex_map.Lookup(word, cmd.value)) {
			addError(101, "syntex error", word);
			word = macro(NULL);
			continue;
		}

		switch (cmd.c.command) {
		case PTCM_NOSUPPORT :
			addError(102, "syntex error", word, true);
			break;

		case PTCM_LINE :
			total_line++;
			break;

		case PTCM_OPLINE :
			if (ram.o.use || ram.pg33.use) {
				if ((int) patData.dbg_pc == current_pc + start_pc)
					patData.dbg_op = ram.pg33.opcode;

				TRAMDATA *ram_new = new TRAMDATA;
				memcpy(ram_new, &ram, sizeof(TRAMDATA));
				patData.pram->AddTail(ram_new);
				current_pc++;
				memset(&ram, 0, sizeof(TRAMDATA));
			}

			break;

		case PTCM_LABEL :
			word = macro(NULL);

			if (labelMap.Lookup(word,dwValue))
				addError(103, "label redefined", word);
			else
				labelMap[word] = current_pc;

			break;

		case PTCM_MCC_BIT :
			if (cmd.c.bit == 0x8000) {	// dbg
				patData.dbg_pc = current_pc + start_pc;
				ram.j.dbg = cmd.c.bit;
			}
			else {
				ram.opcode_ts33 |= cmd.c.bit;
			}

			break;

		case PTCM_MCC_OPCODE :
			if (ram.o.use || ram.pg33.use)
				addError(104, "opcode redefinition", word);
			else {
				ram.opcode_ts33 |= cmd.c.bit;
				ram.pg33.use = 1;

				if (ram.o.opcode == 1 || ram.o.opcode == 2 || ram.o.opcode == 3 || ram.pg33.opcode == 1 || ram.pg33.opcode == 2 || ram.pg33.opcode == 3) {
					if (isdigit(word[strlen(word) - 1]))
						ram.loop_count = word[strlen(word) - 1] - '0';
				}

				if (cmd.c.opt & 2) {	// need label
					word = macro(NULL);

					if (cmd.c.bit == 0x200 || cmd.c.bit == 0x1e00) {
						ram.j.jump_add = 0;

						for (POSITION pos = pattern.GetHeadPosition(); pos;) {
							TPGDATA *pg = pattern.GetNext(pos);

							if (strcmp(pg->name, word) == 0) {
								ram.j.jump_add = pg->pc;
								patData.scan_use |= pg->scan_use;
								patData.catch_use |= pg->catch_use;

								if (cmd.c.bit == 0x200 && patData.dbg_pc == 0 && pg->dbg_pc) {
									patData.dbg_pc = pg->dbg_pc;
									patData.dbg_op = pg->dbg_op;
								}

								break;
							}
						}

						if (ram.j.jump_add == 0) {
							if (labelMap.Lookup(word,dwValue)) {
								ram.j.jump_add = start_pc + dwValue;
							}
							else {
								int nRam = (pattern.GetCount() << 24) | patData.pram->GetCount() << 16 | total_line;
								TNOLABEL *nolabel = new TNOLABEL;
								nolabel->pc = nRam;
								strcpy(nolabel->label, word);
							//	noSub.AddTail(nolabel);	// global source case
								nolabel->pc = current_pc;
								noLabel.AddTail(nolabel);
							}
						}
					}
					else if (labelMap.Lookup(word, dwValue)) {
						ram.j.jump_add = start_pc + dwValue;
					}
					else {
						TNOLABEL *nolabel = new TNOLABEL;
						nolabel->pc = current_pc;
						strcpy(nolabel->label, word);
						noLabel.AddTail(nolabel);
					}
				}

				if (cmd.c.opt & 1) {
					if (cmd.c.opt == 1)
						ram.j.jump_add = current_pc + start_pc;

					if (ram.loop_count)
						addError(105, "loop count redefinition", word);
					else {
						char *tmp_w = word;
						word = macro(NULL);
						dwValue = strtoul(word, &endptr, 0);

						if (cmd.c.bit == 0x700 || cmd.c.bit == 0x500) {
							unsigned int value = strtoul(word, &endptr, 0);

							if (*endptr != 0 && *endptr != ' ')
								addError(106, "bad suffix on number", word);
							else
								ram.jump = value;

							if (cmd.c.bit == 0x700) {
								word = macro(NULL);
								dwValue = strtoul(word, &endptr, 0);
							}
							else {
								if (strcmp(tmp_w, "catch") == 0 && dwValue != 0x8024)
									patData.catch_use = 1;
								break;
							}
						}

						if (ram.o.opcode == 0x9 || (ram.o.opcode >= 0x1c && ram.o.opcode <= 0x1e) ||
								ram.pg33.opcode == 0x9 || (ram.pg33.opcode >= 0x1c && ram.pg33.opcode <= 0x1e)) {
							ram.loop_count = (unsigned int) (strtod(word, &endptr) * 10000);

							if (*endptr != 0 && *endptr != ' ')
								addError(107, "bad suffix on number", word);
						}
						else if (*endptr != 0 && *endptr != ' ')
							addError(107, "bad suffix on number", word);
						else
							ram.loop_count = dwValue;
					}
				}
			}

			break;

		case PTCM_OPTION :
			word = macro(NULL);
			dwValue = strtoul(word, &endptr, 0);

			if (*endptr != 0 && *endptr != ' ')
				addError(108, "bad suffix on number", word);
			else
				option = dwValue;

			break;

		case PTCM_FUNCTION :
			if (patData.name[0])
				addError(113, "sub redefined", word);
			else {
				word = macro(NULL);
				strcpy(patData.name, word);

				if (!(option & DUP_SUB)) {
					for (POSITION pos = pattern.GetHeadPosition(); pos;) {
						TPGDATA *pat = pattern.GetNext(pos);

						if (strcmp(patData.name, pat->name) == 0) {
							addError(114, "sub function redefinition", word);
							break;
						}
					}
				}
			}

			break;

		case PTCM_TIMESET :
			if (ram.o.ts || ram.pg33.ts)
				addError(115, "timeset redefinition", word);
			else {
				if (cmd.c.opt == 1) {
					ram.pg33.ts = cmd.c.bit;
					break;
				}

				word = macro(NULL);

				if (tsTimeGroup.Lookup(word, dwValue)) {
					ram.pg33.ts = (short) (dwValue & 0xff);
				}
				else {
					dwValue = strtoul(word, &endptr, 0);

					if (*endptr != 0 && *endptr != ' ')
						addError(116, "bad suffix on number", word);
					else if (dwValue > 255)
						addError(117, "too large timeset", word);
					else
						ram.pg33.ts = (short) dwValue;
				}
			}

			break;

		case PTCM_MINMAX :
			if (cmd.c.bit == 0 && patData.minmax.xmax_use)
				addError(118, "xmax redefinition", word);
			else if (cmd.c.bit == 1 && patData.minmax.xmin_use)
				addError(119, "xmin redefinition", word);
			else if (cmd.c.bit == 2 && patData.minmax.ymax_use)
				addError(120, "ymax redefinition", word);
			else if (cmd.c.bit == 3 && patData.minmax.ymin_use)
				addError(121, "ymin redefinition", word);
			else if (cmd.c.bit == 4 && patData.minmax.dmax_use)
				addError(122, "dmax redefinition", word);
			else if (cmd.c.bit == 5 && patData.minmax.dmin_use)
				addError(123, "dmin redefinition", word);
			else if (cmd.c.bit == 6 && patData.minmax.bmax_use)
				addError(124, "bmax redefinition", word);
			else if (cmd.c.bit == 7 && patData.minmax.bmin_use)
				addError(125, "bmin redefinition", word);
			else if (cmd.c.bit == 10 && patData.minmax.smax_use)
				addError(128, "smax redefinition", word);
			else if (cmd.c.bit == 11 && patData.minmax.smin_use)
				addError(129, "smin redefinition", word);
			else if (cmd.c.bit == 10 && patData.minmax.vmax_use)
				addError(130, "vmax redefinition", word);
			else if (cmd.c.bit == 11 && patData.minmax.vmin_use)
				addError(131, "vmin redefinition", word);
			else {
				word = macro(NULL);
				dwValue = strtoul(word, &endptr, 0);
				int mask = 0xffffff;

				if (*endptr != 0 && *endptr != ' ')
					addError(132, "bad suffix on number", word);
				else if (cmd.c.bit == 0) {
					patData.minmax.xmax_use = 1;
					patData.minmax.xmax = dwValue & mask;
				}
				else if (cmd.c.bit == 1) {
					patData.minmax.xmin_use = 1;
					patData.minmax.xmin = dwValue & mask;
				}
				else if (cmd.c.bit == 2) {
					patData.minmax.ymax_use = 1;
					patData.minmax.ymax = dwValue & mask;
				}
				else if (cmd.c.bit == 3) {
					patData.minmax.ymin_use = 1;
					patData.minmax.ymin = dwValue & mask;
				}
				else if (cmd.c.bit == 4) {
					patData.minmax.dmax_use = 1;
					patData.minmax.dmax = dwValue & mask;
				}
				else if (cmd.c.bit == 5) {
					patData.minmax.dmin_use = 1;
					patData.minmax.dmin = dwValue & 0x3ffff;
				}
				else if (cmd.c.bit == 6) {
					patData.minmax.bmax_use = 1;
					patData.minmax.bmax = dwValue & 0xf;
				}
				else if (cmd.c.bit == 7) {
					patData.minmax.bmin_use = 1;
					patData.minmax.bmin = dwValue & 0xf;
				}
				else if (cmd.c.bit == 10) {
					patData.minmax.smax_use = 1;
					patData.minmax.smax = dwValue & 0xff;
				}
				else if (cmd.c.bit == 11) {
					patData.minmax.smin_use = 1;
					patData.minmax.smin = dwValue & 0xff;
				}
				else if (cmd.c.bit == 12) {
					patData.minmax.vmax_use = 1;
					patData.minmax.vmax = dwValue & 0x3ffff;
				}
				else if (cmd.c.bit == 13) {
					patData.minmax.vmin_use = 1;
					patData.minmax.vmin = dwValue & 0x3ffff;
				}
			}

			break;

		case PTCM_X :
			if (ram.ax.x_dir && (cmd.c.bit & 0x2000))
				addError(131, "x direct redefinition", word);
			else if (ram.ax.x_inv && (cmd.c.bit & 0x4000))
				addError(132, "x output invert redefinition", word);
			else if (ram.ax.x_xscr && (cmd.c.bit & 0x8000))
				addError(133, "x scramble disable redefinition", word);
			else {
				ram.u.x_use = 1;
				ram.x |= cmd.c.bit;

				if (cmd.c.opt) {
					word = macro(NULL);
					dwValue = strtoul(word, &endptr, 0);

					if (*endptr != 0 && *endptr != ' ')
						addError(134, "bad suffix on number", word);
					else if (dwValue > 0xffffff)
						addError(135, "too large number", word);
					else {
						ram.x_direct = (short) dwValue;
					//	ram.x_operdir |= (short) ((dwValue >> 16) & 0xff);
						ram.x_operdir |= (short) ((dwValue >> 8) & 0xff00);
					}
				}
			}

			break;

		case PTCM_XADD :
			if (ram.u.x_aluuse)
				addError(136, "x register redefinition", word);
			else {
				ram.u.x_aluuse = 1;
				ram.x |= cmd.c.bit;

				if (cmd.c.opt) {
					if (ram.x_operand)
						addError(137, "x operand redefinition", word);
					else {
						word = macro(NULL);
						dwValue = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ')
							addError(138, "bad suffix on number", word);
						else if (dwValue > 0xffffff)
							addError(139, "too large number", word);
						else {
							ram.x_operand = (short) dwValue;
						//	ram.x_operdir |= (short) ((dwValue >> 8) & 0xff00);
							ram.x_operdir |= (short) ((dwValue >> 16) & 0xff);
						}
					}
				}
			}

			break;

		case PTCM_Y :
			if (ram.ay.y_dir && (cmd.c.bit & 0x2000))
				addError(140, "y direct redefinition", word);
			else if (ram.ay.y_inv && (cmd.c.bit & 0x4000))
				addError(141, "y output invert redefinition", word);
			else if (ram.ay.y_xscr && (cmd.c.bit & 0x8000))
				addError(140, "y scramble disable redefinition", word);
			else {
				ram.u.y_use = 1;
				ram.y |= cmd.c.bit;

				if (cmd.c.opt) {
					word = macro(NULL);
					dwValue = strtoul(word, &endptr, 0);

					if (*endptr != 0 && *endptr != ' ')
						addError(142, "bad suffix on number", word);
					else if (dwValue > 0xffffff)
						addError(143, "too large number", word);
					else {
						ram.y_direct = (short) dwValue;
					//	ram.y_operdir |= (short) ((dwValue >> 16) & 0xff);
						ram.y_operdir |= (short) ((dwValue >> 8) & 0xff00);
					}
				}
			}

			break;

		case PTCM_YADD :
			if (ram.u.y_aluuse)
				addError(144, "x register redefinition", word);
			else {
				ram.u.y_aluuse = 1;
				ram.y |= cmd.c.bit;

				if (cmd.c.opt) {
					if (ram.y_operand)
						addError(145, "y operand redefinition", word);
					else {
						word = macro(NULL);
						dwValue = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ')
							addError(146, "bad suffix on number", word);
						else if (dwValue > 0xffffff)
							addError(147, "too large number", word);
						else {
							ram.y_operand = (short) dwValue;
						//	ram.y_operdir |= (short) ((dwValue >> 8) & 0xff00);
							ram.y_operdir |= (short) ((dwValue >> 16) & 0xff);
						}
					}
				}
			}

			break;

		case PTCM_D :
			if (ram.u.d_use)
				addError(148, "d operand redefinition", word);
			else if (ram.d.d_dir && (cmd.c.bit & 0x1000))
				addError(149, "d direct redefinition", word);
			else if (ram.d.d_inv && (cmd.c.bit & 0x2000))
				addError(150, "d output invert redefinition", word);
			else if (ram.d.d_expinv && (cmd.c.bit & 0x4000))
				addError(151, "d expect invert redefinition", word);
			else if (ram.d.d_xscr && (cmd.c.bit & 0x8000))
				addError(152, "d scramble disable redefinition", word);
			else {
			//	ram.u.d_use = 1;
				ram.dop.d_op |= cmd.c.bit;

				if (cmd.c.opt) {
					if (ram.d.d_operand)
						addError(153, "d operand redefinition", word);
					else {
					//	ram.u.d_use = 1;
						word = macro(NULL);
						dwValue = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ')
							addError(154, "bad suffix on number", word);
						else if (dwValue > 0x3ffff)
							addError(155, "too large number", word);
						else
							ram.d.d_operand = dwValue;
					}
				}
			}

			break;

		case PTCM_DADD :
			if (ram.u.d_use)
				addError(156, "d operand redefinition", word);
			else if (ram.u.d_aluuse)
				addError(157, "d register redefinition", word);
			else {
				ram.u.d_aluuse = 1;
				ram.dop.d_op |= cmd.c.bit;

				if (cmd.c.opt) {
					if (ram.d.d_operand)
						addError(158, "d operand redefinition", word);
					else {
					//	ram.u.d_use = 1;
						word = macro(NULL);
						dwValue = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ')
							addError(159, "bad suffix on number", word);
						else if (dwValue > 0x3ffff)
							addError(160, "too large number", word);
						else
							ram.d.d_operand = dwValue;
					}
				}
			}

			break;

		case PTCM_B :
			if (ram.u.b_use)
				addError(161, "bank output invert redefinition", word);
			else {
				ram.u.b_use = 1;
				ram.bank |= cmd.c.bit;

				if (cmd.c.opt) {
					if (ram.sb.b_operand)
						addError(162, "bank operand redefinition", word);
					else {
						word = macro(NULL);
						dwValue = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ')
							addError(163, "bad suffix on number", word);
						else if (dwValue > 15)
							addError(164, "too large number", word);
						else
							ram.sb.b_operand = (short) dwValue;
					}
				}
			}

			break;

		case PTCM_BADD :
			if (ram.u.b_aluuse)
				addError(165, "bank register redefinition", word);
			else {
				ram.u.b_aluuse = 1;
				ram.bank |= cmd.c.bit;
				if (cmd.c.opt) {
					if (ram.sb.b_operand)
						addError(166, "bank operand redefinition", word);
					else {
						word = macro(NULL);
						dwValue = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ')
							addError(167, "bad suffix on number", word);
						else if (dwValue > 15)
							addError(168, "too large number", word);
						else
							ram.sb.b_operand = (short) dwValue;
					}
				}
			}

			break;

		case PTCM_BS :
		case PTCM_S :
			{
				ram.u.s_use = 1;
				patData.scan_use = 1;

				if (cmd.c.command == PTCM_BS)
					ram.bank |= cmd.c.bit;
				else
					ram.scan |= cmd.c.bit;

				if (cmd.c.opt) {
					if (ram.sb.s_operand)
						addError(173, "scan operand redefinition", word);
					else {
						word = macro(NULL);
						dwValue = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ')
							addError(174, "bad suffix on number", word);
						else if (dwValue > 255)
							addError(175, "too large number", word);
						else
							ram.sb.s_operand = (short) dwValue;
					}
				}
			}

			break;

		case PTCM_SADD :
			if (ram.u.s_aluuse)
				addError(176, "scan register redefinition", word);
			else {
				ram.u.s_aluuse = 1;
				ram.scan |= cmd.c.bit;
				patData.scan_use = 1;

				if (cmd.c.opt) {
					if (ram.sb.s_operand)
						addError(177, "scan operand redefinition", word);
					else {
						word = macro(NULL);
						dwValue = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ')
							addError(178, "bad suffix on number", word);
						else if (dwValue > 255)
							addError(179, "too large number", word);
						else
							ram.sb.s_operand = (short) dwValue;
					}
				}
			}

			break;

		case PTCM_V :
			if (ram.v.v_dir && (cmd.c.bit & 0x100))
				addError(180, "vector direct redefinition", word);
			else {
				ram.u.v_use = 1;
				ram.vector |= cmd.c.bit;

				if (cmd.c.opt) {
					word = macro(NULL);
					dwValue = strtoul(word, &endptr, 0);

					if (*endptr != 0 && *endptr != ' ')
						addError(181, "bad suffix on number", word);
					else if (dwValue > 0x7ffff)
						addError(182, "too large number", word);
					else {
						ram.v_direct = (short) dwValue;
						ram.v_srcdir |= (short) ((dwValue >> 8) & 0xff00);
					}
				}
			}

			break;

		case PTCM_VADD :
			if (ram.u.v_aluuse)
				addError(183, "vector register redefinition", word);
			else {
				ram.u.v_aluuse = 1;
				ram.vector |= cmd.c.bit;

				if (cmd.c.opt) {
					if (ram.v_source)
						addError(184, "vector source address redefinition", word);
					else {
						word = macro(NULL);
						dwValue = strtoul(word, &endptr, 0);

						if (*endptr != 0 && *endptr != ' ')
							addError(185, "bad suffix on number", word);
						else if (dwValue > 0x7ffff)
							addError(186, "too large number", word);
						else {
							ram.v_source = (short) dwValue;
							ram.v_srcdir |= (short) ((dwValue >> 16) & 0xff);
						}
					}
				}
			}

			break;

		case PTCM_X_FM :
			if (ram.x_addr)
				addError(177, "setx redefinition", word);
			else {
				ram.x_addr = cmd.c.bit;
			}

			break;

		case PTCM_Y_FM :
			if (ram.y_addr)
				addError(178, "sety redefinition", word);
			else {
				ram.y_addr = cmd.c.bit;
			}

			break;

		case PTCM_S_FM :
			if (ram.s_addr == cmd.c.bit)
				addError(179, "sets redefinition", word);
			else if (ram.s_next == cmd.c.bit)
				addError(179, "next_scan redefinition", word);
			else if (ram.s_zero == cmd.c.bit)
				addError(179, "next_page redefinition", word);
			else {
				ram.s_fm |= cmd.c.bit;
				patData.scan_use = 1;
			}

			break;
		}

		word = macro(NULL);
	}

	if (current_pc && ! patData.name[0])
		addError(180, "sub undefined", "");

	for (POSITION pos = noLabel.GetHeadPosition(); pos;) {
		TNOLABEL *nolabel = noLabel.GetNext(pos);

		if (labelMap.Lookup(nolabel->label, dwValue)) {
			POSITION rampos = patData.pram->FindIndex(nolabel->pc);

			if (rampos) {
				TRAMDATA *ramdata = patData.pram->GetAt(rampos);
				ramdata->j.jump_add = dwValue + start_pc;
			}
			else {
				addError(181, "internal error", nolabel->label);
				break;
			}
		}
		else {
			addError(182, "label undefined", nolabel->label);
			break;
		}

		delete nolabel;
	}

	noLabel.RemoveAll();
	labelMap.RemoveAll();

	if (error_count) {
		delete patData.pram;
		patData.pram = 0;
	}
	else {
		TPGDATA *pg = new TPGDATA;
		memcpy(pg, &patData, sizeof(TPGDATA));
		pattern.AddTail(pg);
	}

	return start_pc + current_pc;
}

String PGCompiler::ramString()
{
	char msg[300];
	String strMemory;
	int pc = 1;

	for (POSITION pos = pattern.GetHeadPosition(); pos;) {
		TPGDATA *pg = pattern.GetNext(pos);
		pc = pg->pc;
		sprintf(msg, "%s : %d\r\n", pg->name, pg->pc);
		strMemory += msg;

		if (!pg->pram)
			continue;

		for (POSITION rampos = pg->pram->GetHeadPosition(); rampos;) {
			TRAMDATA *ram = pg->pram->GetNext(rampos);

			sprintf(msg, "%02X:%04X %04X %04X %04X  %04X %04X %04X  %04X %04X %04X  %04X %04X %04X\n",
					pc++, ram->opcode_ts33, ram->jump, ram->l.low_count, ram->l.high_count,
					ram->x, ram->x_operand, ram->x_direct,
					ram->y, ram->y_operand, ram->y_direct, ram->data, ram->d.d_operand, ram->bank);
			strMemory += msg;
		}
	}

	return strMemory;
}

String PGCompiler::attach()
{
	char msg[300];
	String strPattern;
	int nfunc = 0;

	source_string = "";

	for (POSITION pos = pattern.GetHeadPosition(); pos; nfunc++) {
		TPGDATA *pg = pattern.GetNext(pos);

		sprintf(msg, "SUB%d", nfunc);
		strcpy(pg->name, msg);
		source_string += "sub=";
		source_string += msg;
		source_string += "\n{\n";

		if (pg->minmax.xmax || pg->minmax.xmin || pg->minmax.ymax || pg->minmax.ymin ||
				pg->minmax.dmax || pg->minmax.dmin || pg->minmax.bmax || pg->minmax.bmin ||
				pg->minmax.vmax || pg->minmax.vmin || pg->minmax.smax || pg->minmax.smin)
			source_string += "\t";

		if (pg->minmax.xmax) { sprintf(msg, "xmax=0x%x ", pg->minmax.xmax); source_string += msg; }
		if (pg->minmax.xmin) { sprintf(msg, "xmin=0x%x ", pg->minmax.xmin); source_string += msg; }
		if (pg->minmax.ymax) { sprintf(msg, "ymax=0x%x ", pg->minmax.ymax); source_string += msg; }
		if (pg->minmax.ymin) { sprintf(msg, "ymin=0x%x ", pg->minmax.ymin); source_string += msg; }
		if (pg->minmax.dmax) { sprintf(msg, "dmax=0x%x ", pg->minmax.dmax); source_string += msg; }
		if (pg->minmax.dmin) { sprintf(msg, "dmin=0x%x ", pg->minmax.dmin); source_string += msg; }
		if (pg->minmax.bmax) { sprintf(msg, "bmax=0x%x ", pg->minmax.bmax); source_string += msg; }
		if (pg->minmax.bmin) { sprintf(msg, "bmin=0x%x ", pg->minmax.bmin); source_string += msg; }
		if (pg->minmax.smax) { sprintf(msg, "smax=0x%x ", pg->minmax.smax); source_string += msg; }
		if (pg->minmax.smin) { sprintf(msg, "smin=0x%x ", pg->minmax.smin); source_string += msg; }
		if (pg->minmax.vmax) { sprintf(msg, "vmax=0x%x ", pg->minmax.vmax); source_string += msg; }
		if (pg->minmax.vmin) { sprintf(msg, "vmin=0x%x ", pg->minmax.vmin); source_string += msg; }

		if (pg->minmax.xmax || pg->minmax.xmin || pg->minmax.ymax || pg->minmax.ymin ||
				pg->minmax.dmax || pg->minmax.dmin || pg->minmax.bmax || pg->minmax.bmin ||
				pg->minmax.smax || pg->minmax.smin || pg->minmax.vmax || pg->minmax.vmin)
			source_string += ";\n\n";

		if (pg->pram)
			source_string += ram2String(pg->pram, pg->pc);

		source_string += "}\n\n";
	}

	return source_string;
}

String PGCompiler::ram2String(ramList *ramlist, int start_pc)
{
	StringList strPatLine;
	intList subLabel;
	intList setLabel;
	int op_level = 0;
	int nLabel = 0, nline = 0;
	String str;
	char msg[100];
	unsigned int opcode;

	if (!ramlist)
		return str;

	for (POSITION pos = ramlist->GetHeadPosition(); pos;) {
		TRAMDATA *ram = ramlist->GetNext(pos);
		str += "\t";

		op_level = 0;
		opcode = ram->pg33.opcode;

		switch (opcode) {
			case 0x0 :
				str += "nop\t";
				break;

			case 0x1 :
				str += "goto";

				if (ram->loop_count) {
					sprintf(msg, "%d\t", ram->loop_count % 5);
					str += msg;
				}
				else
					str += "\t";

				op_level = 1;
				break;

			case 0x2 :
				str += "call";

				if (ram->loop_count) {
					sprintf(msg, "%d\t", ram->loop_count % 5);
					str += msg;
				}
				else
					str += "\t";

				op_level = 4;
				break;

			case 0x3 :
				str += "ret";

				if (ram->loop_count) {
					sprintf(msg, "%d\t", ram->loop_count % 5);
					str += msg;
				}
				else
					str += "\t";

				op_level = 0;
				break;

			case 0x4 :
				str += "iret\t";
				break;

			case 0x6 :
				str += "repeat\t";
				op_level = 2;
				break;

			case 0x7 :
				str += "burst\t";
				sprintf(msg, "%d\t", ram->j.jump_add);
				str += msg;
				op_level = 2;
				break;

			case 0x9 :
				str += "ready\t";
				op_level = 2;
				break;

			case 0xf :
				str += "vloop\t";
				op_level = 1;
				break;

			case 0x10 :
				str += "loop0\t";
				op_level = 3;
				break;

			case 0x11 :
				str += "loop1\t";
				op_level = 3;
				break;

			case 0x12 :
				str += "loop2\t";
				op_level = 3;
				break;

			case 0x13 :
				str += "loop3\t";
				op_level = 3;
				break;

			case 0x14 :
				str += "loop4\t";
				op_level = 3;
				break;

			case 0x15 :
				str += "loop5\t";
				op_level = 3;
				break;

			case 0x16 :
				str += "loop6\t";
				op_level = 3;
				break;

			case 0x17 :
				str += "loop7\t";
				op_level = 3;
				break;

			case 0x18 :
				str += "extloop\t";
				op_level = 1;
				break;

			case 0x19 :
				str += "ifloop0\t";
				op_level = 1;
				break;

			case 0x1a :
				str += "scan\t";
				op_level = 3;
				break;

			case 0x1b :
				str += "sloop\t";
				op_level = 1;
				break;

			case 0x1c :
				str += "timer0\t";
				op_level = 3;
				break;

			case 0x1d :
				str += "timer1\t";
				op_level = 3;
				break;

			case 0x1e :
				str += "refresh\t";
				op_level = 4;
				break;

			case 0x1f :
				str += "stop\t";
				break;

			case 0x5 : {
				if (ram->jump) {
					op_level = 0;
					sprintf(msg, "npause\t0x%x\t", ram->jump);
					str += msg;
				}
				else
					str += "pause\t";

				break;
			}
		}

		if (op_level == 4) {
			int nfunc = 0;
			TPGDATA *pg = 0;

			for (POSITION pos = pattern.GetHeadPosition(); pos; nfunc++) {
				pg = pattern.GetNext(pos);

 				if (ram->j.jump_add == pg->pc) {
					sprintf(msg, "SUB%d ", nfunc);
					str += msg;
					break;
				}
			}

			if (!pg || ram->j.jump_add != pg->pc)
				op_level = 1;

			if (opcode == 0x1e)
				op_level |= 2;
		}

		if (op_level & 1) {
			bool used = false;
			nLabel = 0;

			for (POSITION linepos = subLabel.GetHeadPosition(); linepos;) {
				nLabel++;
				nline = subLabel.GetNext(linepos);

				if (nline == ram->j.jump_add - start_pc) {
					used = true;
					break;
				}
			}

			if (used == false) {
				nLabel++;
				nline = ram->j.jump_add - start_pc;
				subLabel.AddTail(nline);
				setLabel.AddTail((nline << 16) | nLabel);
			}

			sprintf(msg, "label_%d ", nLabel);
			str += msg;
		}

		if (op_level & 2) {
			if (opcode == 0x9 || opcode >= 0x1c && opcode <= 0x1e)
				sprintf(msg, "%5.3f ", ram->loop_count / 10000.0);
				/* ram->loop_count /= 10000; */
			else
				sprintf(msg, "%d ", ram->loop_count);

			str += msg;
		}

		if (ram->o.ref_mask || ram->pg33.ref_mask) {
			str += "ref_enb ";
		}

		if (ram->o.ref_hold || ram->pg33.ref_hold) {
			str += "ref_hold ";
		}

		if (ram->j.dbg) {
		     str += "dbg ";
		}

		if (ram->o.ts || ram->pg33.ts) {
			sprintf(msg, "TS%d ", ram->pg33.ts);
			str += msg;
		}

		if (ram->x != 0xffff) {
			if (ram->ax.x_xscr)
				str += "x_xscr ";

			if (ram->ax.x_inv)
				str += "xinv ";

			if (ram->ax.x_dir) {
				int direct = ram->x_direct;
				direct |= (ram->x_operdir & 0xff) << 16;

				switch (ram->ax.x_dir) {
					case 1 : sprintf(msg, "x=x%c&0x%x ", ram->ax.x_out + 'a', direct); break;
					case 2 : sprintf(msg, "x=x%c|0x%x ", ram->ax.x_out + 'a', direct); break;
					case 3 : sprintf(msg, "x=0x%x ", direct); break;
				}

				str += msg;
			}
			else if (ram->ax.x_out) {
				sprintf(msg, "x=x%c ", ram->ax.x_out + 'a');
				str += msg;
			}

			int operand = ram->x_operand;
			operand |= (ram->x_operdir >> 8) << 16;

			switch (ram->ax.x_opcode) {
				case 0 :
					if (ram->ax.x_des + ram->ax.x_src)
						sprintf(msg, "x%c=x%c ", ram->ax.x_des + 'a', ram->ax.x_src + 'a');
					else
						msg[0] = 0;

					break;

				case 1 :
					sprintf(msg, "x%c=0x%x ", ram->ax.x_des + 'a', operand);
					break;

				case 2 :
					sprintf(msg, "x%c=x%c+0x%x ", ram->ax.x_des + 'a', ram->ax.x_src + 'a', operand);
					break;

				case 3 :
					sprintf(msg, "x%c=x%c-0x%x ", ram->ax.x_des + 'a', ram->ax.x_src + 'a', operand);
					break;

				case 4 :
					sprintf(msg, "x%c=x%c++0x%x ", ram->ax.x_des + 'a', ram->ax.x_src + 'a', operand);
					break;

				case 5 :
					sprintf(msg, "x%c=x%c--0x%x ", ram->ax.x_des + 'a', ram->ax.x_src + 'a', operand);
					break;

				case 6 :
					sprintf(msg, "x%c=~x%c ", ram->ax.x_des + 'a', ram->ax.x_src + 'a');
					break;

				case 7 :
					sprintf(msg, "x%c=fm ", ram->ax.x_des + 'a');
					break;
			}

			str += msg;
		}

		if (ram->x_addr)
			str += "setx ";

		if (ram->y != 0xffff) {
			if (ram->ay.y_xscr)
				str += "y_xscr ";

			if (ram->ay.y_inv)
				str += "yinv ";

			if (ram->ay.y_dir) {
				int direct = ram->y_direct;
				direct |= (ram->y_operdir & 0xff) << 16;

				switch (ram->ay.y_dir) {
					case 1 : sprintf(msg, "y=y%c&0x%x ", ram->ay.y_out + 'a', direct); break;
					case 2 : sprintf(msg, "y=y%c|0x%x ", ram->ay.y_out + 'a', direct); break;
					case 3 : sprintf(msg, "y=0x%x ", direct); break;
				}

				str += msg;
			}
			else if (ram->ay.y_out) {
				sprintf(msg, "y=y%c ", ram->ay.y_out + 'a');
				str += msg;
			}

			int operand = ram->y_operand;
			operand |= (ram->y_operdir >> 8) << 16;

			switch (ram->ay.y_opcode) {
				case 0 :
					if (ram->ay.y_des + ram->ay.y_src)
						sprintf(msg, "y%c=y%c ", ram->ay.y_des + 'a', ram->ay.y_src + 'a');
					else
						msg[0] = 0;

					break;

				case 1 :
					sprintf(msg, "y%c=0x%x ", ram->ay.y_des + 'a', operand);
					break;

				case 2 :
					sprintf(msg, "y%c=y%c+0x%x ", ram->ay.y_des + 'a', ram->ay.y_src + 'a', operand);
					break;

				case 3 :
					sprintf(msg, "y%c=y%c-0x%x ", ram->ay.y_des + 'a', ram->ay.y_src + 'a', operand);
					break;

				case 4 :
					sprintf(msg, "y%c=y%c++0x%x ", ram->ay.y_des + 'a', ram->ay.y_src + 'a', operand);
					break;

				case 5 :
					sprintf(msg, "y%c=y%c--0x%x  ", ram->ay.y_des + 'a', ram->ay.y_src + 'a', operand);
					break;

				case 6 :
					sprintf(msg, "y%c=~y%c ", ram->ay.y_des + 'a', ram->ay.y_src + 'a');
					break;

				case 7 :
					sprintf(msg, "y%c=fm ", ram->ax.x_des + 'a');
					break;
			}

			str += msg;
		}

		if (ram->y_addr)
			str += "sety ";

		if (ram->data != 0xffff) {
			if (ram->d.d_xscr)
				str += "d_xscr ";

			if (ram->d.d_expinv)
				str += "einv ";

			if (ram->d.d_inv)
				str += "dinv ";

			if (ram->d.d_out) {
				sprintf(msg, "d=d%c ", ram->d.d_out + 'a');
				str += msg;
			}

			if (ram->d.d_dir) {
				sprintf(msg, "d=0x%x ", ram->d.d_operand);
				str += msg;
			}

			switch (ram->d.d_opcode) {
				case 0 :
					if (ram->d.d_des + ram->d.d_src)
						sprintf(msg, "d%c=d%c ", ram->d.d_des + 'a', ram->d.d_src + 'a');
					else
						msg[0] = 0;

					break;

				case 1 :
					sprintf(msg, "d%c=0x%x ", ram->d.d_des + 'a', ram->d.d_operand);
					break;

				case 2 :
					sprintf(msg, "d%c=d%c+0x%x ", ram->d.d_des + 'a', ram->d.d_src + 'a', ram->d.d_operand);
					break;

				case 3:
					sprintf(msg, "d%c=~d%c ", ram->d.d_des + 'a', ram->d.d_src + 'a');
					break;
			}

			str += msg;
		}

		if (ram->bank != 0xffff) {
			if (ram->sb.b_inv)
				str += "binv ";

			if (ram->sb.b_out) {
				sprintf(msg, "b=b%c ", ram->sb.b_out + 'a');
				str += msg;
			}

			if (ram->sb.b_dir) {
				sprintf(msg, "b=0x%x ", ram->sb.b_operand);
				str += msg;
			}

			switch (ram->sb.b_opcode) {
				case 0 :
					if (ram->sb.b_des + ram->sb.b_src)
						sprintf(msg, "b%c=b%c ", ram->sb.b_des + 'a', ram->sb.b_src + 'a');
					else
						msg[0] = 0;

					break;

				case 1 :
					sprintf(msg, "b%c=0x%x ", ram->sb.b_des + 'a', ram->sb.b_operand);
					break;

				case 2 :
					sprintf(msg, "b%c=b%c+0x%x ", ram->sb.b_des + 'a', ram->sb.b_src + 'a', ram->sb.b_operand);
					break;

				case 3 :
					sprintf(msg, "b%c=b%c-0x%x ", ram->sb.b_des + 'a', ram->sb.b_src + 'a', ram->sb.b_operand);
					break;
			}

			str += msg;
		}

		if (ram->scan != 0xffff) {
			if (ram->sb.s_all)
				str += "allscan ";

			if (ram->sb.s_out) {
				sprintf(msg, "s=s%c ", ram->sb.s_out + 'a');
				str += msg;
			}

			if (ram->sb.s_dir) {
				sprintf(msg, "s=0x%x ", ram->sb.s_operand);
				str += msg;
			}

			switch (ram->sb.s_opcode) {
				case 0 :
					if (ram->sb.s_des + ram->sb.s_src)
						sprintf(msg, "s%c=s%c ", ram->sb.s_des + 'a', ram->sb.s_src + 'a');
					else
						msg[0] = 0;

					break;

				case 1 :
					sprintf(msg, "s%c=0x%x ", ram->sb.s_des + 'a', ram->sb.s_operand);
					break;

				case 2 :
					sprintf(msg, "s%c=s%c+0x%x ", ram->sb.s_des + 'a', ram->sb.s_src + 'a', ram->sb.s_operand);
					break;

				case 3 :
					sprintf(msg, "s%c=s%c-0x%x ", ram->sb.s_des + 'a', ram->sb.s_src + 'a', ram->sb.s_operand);
					break;
			}

			str += msg;
		}

		if (ram->s_addr)
			str += "sets ";

		if (ram->s_next)
			str += "next_scan ";

		if (ram->s_zero)
			str += "next_page";

		if (ram->vector != 0xffff) {
			if (ram->v.v_out) {
				sprintf(msg, "v=v%c ", ram->v.v_out + 'a');
				str += msg;
			}

			if (ram->v.v_dir) {
				int direct = ram->v_direct;
				direct |= (ram->v_srcdir >> 8) << 16;
				sprintf(msg, "v=0x%x ", direct);
				str += msg;
			}

			int source = ram->v_source;
			source |= (ram->v_srcdir & 0xff) << 16;

			if (ram->v.v_opcode == 0) {
				if (ram->v.v_des + ram->v.v_src)
					sprintf(msg, "v%c=v%c ", ram->v.v_des + 'a', ram->v.v_src + 'a');
				else
					msg[0] = 0;
			}
			else if (ram->v.v_opcode == 1)
				sprintf(msg, "v%c=0x%x ", ram->v.v_des + 'a', source);
			else if (ram->v.v_opcode == 2)
				sprintf(msg, "v%c=v%c+0x%x ", ram->v.v_des + 'a', ram->v.v_src + 'a', source);
			else if (ram->v.v_opcode == 3)
				sprintf(msg, "v%c=v%c-0x%x ", ram->v.v_des + 'a', ram->v.v_src + 'a', source);

			str += msg;
		}

		str += " ;\n";
		strPatLine.AddTail(str);
		str = "";
	}

	for (POSITION pos = setLabel.GetHeadPosition(); pos;) {
		nLabel = setLabel.GetNext(pos);
		POSITION patpos = strPatLine.FindIndex((nLabel >> 16));

		if (patpos) {
			String &strline = strPatLine.GetNext(patpos);
			sprintf(msg, "label_%d:\n", (nLabel & 0xffff));
			str = msg;
			str += strline;
			strline = str;
		}
	}

	str = "";

	for (POSITION pos = strPatLine.GetHeadPosition(); pos;) {
		str += strPatLine.GetNext(pos);
	}

	strPatLine.RemoveAll();
	subLabel.RemoveAll();
	setLabel.RemoveAll();
	return str;
}
