
#ifndef _SWCONTROL_H
#define _SWCONTROL_H

#include "hwcontrol.h"

// DEBUGGING을 선택적으로 적용하기 위해 COMPILE OPTION 부여
// COMPILE시 아래 Value를 0으로 바꾸면 DEBUGGING용 print 실행되지 않는다.
#define _MAJOR_REV1 1
#define _MAJOR_REV2 3
#define _MINOR_REV 4

#define _LIB_DEBUG_PRINT	0

#define SI(a, b) \
    ((((a) & 0xff) << 8 | ((b) & 0xff)) << 16)

#define GET_BIT(a, b) \
    (bool) ((a[(b) / 8] >> ((b) % 8)) & 1)

#define SET_BIT(a, b, c) \
    (c ? (a[(b) / 8] |= (1 << ((b) % 8))) : (a[(b) / 8] &= ~(1 << ((b) % 8))))

#define GET_BIT2(a, b) \
    (bool) (((a) >> (b)) & 1)

#define SET_BIT2(a, b, c) \
    ((c) ? ((a) |= (1 << (b))) : ((a) &= ~(1 << (b))))

#define SET_ELOOP(a) \
    setExtLoop(a)

#define RESET_DRIVER(a) \
    Write(0x6, 1)

#define RUNMODE \
    !m_all_skip && !m_skip_status

#define LOAD(a, b) \
    a##_##b(#b)

#define GET_MUX \
    getMux

#define WRITE_CONSOLE \
    printf

#define SET_PIN	\
    setPDS

#define RESET_LATCH \
    sysResetLatch

#define DELAY_TIME \
    sysDelay

#define START \
    START_MPAT

#define WAIT_PATTERN \
    WAIT_MPAT

#define SET_FM_MODE(a) \
    sysFMMode(a)

#define SET_SCAN_LOOP(a) \
    setScanLoop(a)

#define SET_READY(a) \
    m_ready_bit = a

#define SCAN_INV \
    scanInv

#define SET_FAULT \
    setFault

#define SET_SCANMODE \
    setScanMode2

#define SET_FMSEL \
    setFMSel

#define CLOCK_INV \
    setClkInv

#define READ_CHIP_DATA \
    getHICInfo

#define PRINT(a) \
{ \
    if (!m_all_skip) \
    Send(BC_PRINTF, (char *) a, strlen(a) + 1); \
    }

#define SET_ECC(a, b, c) \
    do { \
    m_ecc_page = b; \
    m_ecc_io = a; \
    m_ecc_block = c; \
    int tmp[3] = { a, b, c}; \
    Send(BC_ECC, tmp, sizeof(tmp)); \
    } while (0)

#define SET_DISPLAY_IO(a) \
    do { \
    int i = a; \
    Send(BC_DISPLAY_IO, &i, 4); \
    } while (0)

#define SET_DRAM_FAILBLOCK(a) \
    do { \
    int f = a; \
    Send(BC_DRAM_FBLOCK, &f, 4); \
    } while (0)

#define SET_MONITOR(a, b, c) \
    do { \
    int t[3] = { a, b, c }; \
    Send(BC_SETFAULT, &t, 12); \
    } while (0)

#define SET_BITIME(a) \
    do { \
    int t = a; \
    Send(BC_BITIME, &t, 4); \
    } while (0)

#define GET_MEMORYDATA(a) \
    do { \
    int m[2] = { MEMORY, a }; \
    Send(BC_GET_FMDATA, &m, sizeof(m)); \
    Recv((char *) m_failblock, sizeof(FailBlock) * m_bib_x * m_bib_y * NSLOT); \
    } while (0)

#define SET_FAILMODE(a) \
    do { \
    m_fail_mode = a; \
    if (m_fail_mode == INIT_FAIL) { \
    memset(m_init_fail_block, 0, m_bib_x * m_bib_y * NSLOT * (m_block_limit / 8 + 1)); \
    } \
    Send(BC_SETFAILMODE, &m_fail_mode, sizeof(int)); \
    } while (0)

#define SET_JUDGEMODE(a) \
    do { \
    m_judge_mode = a; \
    Send(BC_SETJUDGEMODE, &m_judge_mode, sizeof(int)); \
    } while (0)

/*
#define SEND_FAIL_DATA() \
do { \
        if (m_fail_index) { \
                Send(BC_BLOCKDATA, m_block_fail, sizeof(int) * m_fail_index); \
                m_fail_index = 0; \
        } \
        if (m_ecc_index) { \
                Send(BC_ECCDATA, m_ecc_fail, sizeof(int) * m_ecc_index); \
                m_ecc_index = 0; \
        } \
} while (0)
*/

#define SEND_FAIL_DATA() \
    do { \
    } while (0)

#define SET_TEMP(a, b, c) \
do { \
	float temp[3] = { a, b, c }; \
	if(m_all_skip == true)\
	{\
		if(m_tempinfo_mode == true) \
		{\
			Send(BC_SETTEMPINFO, temp, sizeof(temp));\
		}\
		else\
		{\
			continue;\
		}\
	}\
	else \
	{ \
	Send(BC_SETTEMP, temp, sizeof(temp)); \
	} \
	if (b) \
	{ \
		recv(0, temp, 4, 0); \
	} \
} while (0)

#define SET_TEMP2(a, b, c, d) \
    do { \
    int m = d; \
    Send(BC_SETTEMP2, &m, 4); \
    SET_TEMP(a, b, c); \
    } while (0)

#define SET_FAIL_SKIP(a) \
    do { \
    if (a) \
    m_app_value = true; \
    else \
    m_app_value = false; \
    } while (0)

#define GET_ITEM_YIELD(a, b) \
    do { \
    if (!m_all_skip) { \
    float y = 0; \
    Send(BC_GET_ITEMYIELD, a, strlen(a) + 1); \
    recv(0, (char *) &y, 4, 0); \
    b = y; \
    } \
    } while (0)

#define SAVE_HICHIP(a) \
    do { \
    int ddp[3] = { m_hic_count, m_hic_length, m_hic_size }; \
    Send(BC_SETDDP, &ddp, sizeof(ddp)); \
    Send(BC_SENDCAL, (char *) m_failblock, sizeof(FailBlock) * m_bib_x * m_bib_y * NSLOT); \
    Send(BC_SAVE_HICHIP, a, strlen(a) + 1); \
    } while (0)

#define SET_STOPMODE(a, b) \
    do { \
    int temp[2] = { a, b }; \
    Send(BC_STOPMODE, (char *) temp, sizeof(temp)); \
    } while (0)

#define SET_STOP(a) \
    do { \
    Send(BC_STOP, a, strlen(a)); \
    return; \
    } while (0);

#define SET_INITFAIL(a) \
    do { \
    SET_FAILMODE(INIT_FAIL); \
    } while (0);

#define SET_ADDFAIL(a) \
    do { \
    SET_FAILMODE(ADD_FAIL); \
    } while (0);

#define STOP_FAIL_RATE(a, b, c) \
    do { \
    if (RUNMODE) { \
    float frate = 0;\
    GET_ITEM_YIELD(a, frate); \
    frate = 100.0 - frate; \
    if (frate > b) \
    SET_STOP(c); \
    } \
    } while (0)

#define SET_FLS(a, b) \
    do { \
    int tb = b; \
    char sname[64]; \
    memset(sname, 0x00, sizeof(sname)); \
    memcpy(sname, (char *) &tb, sizeof(tb)); \
    strcpy(sname + sizeof(tb), a); \
    Send(BC_SET_FLS, sname, sizeof(sname)); \
    } while (0)

#define SET_DQ_DISPLAY(a) \
    m_dq_mode = (a) ? true : false

#define CHAMBER_MODE(a, b) \
    do { \
    unsigned int temp[2] = { a, b }; \
    Send(BC_CHAMBER_MODE, temp, sizeof(temp)); \
    recv(0, temp, 4, 0); \
    } while (0)

#define SET_BIST_ERROR_NO(a) \
    do { \
    m_bist_errno_no = a; \
    Send(BC_BIST_ERROR_NO, (char *) &m_bist_errno_no, sizeof(m_bist_errno_no)); \
    } while (0)

#define SET_BIST_EXCEPTION_CATE(a) \
    do { \
    m_bist_exception_cate = a; \
    Send(BC_BIST_EXCEPTION_CATE, (char *) &m_bist_exception_cate, sizeof(m_bist_exception_cate)); \
    } while (0)

#define setCAT_REV(a) \
    do { \
    char __cat_rev[32]; \
    memset(__cat_rev, 0, sizeof(__cat_rev)); \
    snprintf(__cat_rev, sizeof(__cat_rev), "%s", a); \
    Send(BC_CAT_REV, (char *) __cat_rev, strlen(__cat_rev)); \
    } while (0)

#define setFW_REV(a) \
    do { \
    char __fw_rev[32]; \
    memset(__fw_rev, 0, sizeof(__fw_rev)); \
    snprintf(__fw_rev, sizeof(__fw_rev), "%s", a); \
    Send(BC_FW_REV, (char *) __fw_rev, strlen(__fw_rev)); \
    } while (0)

#define setC_Vendor(a) \
    do { \
    char __c_vendor[32]; \
    memset(__c_vendor, 0, sizeof(__c_vendor)); \
    snprintf(__c_vendor, sizeof(__c_vendor), "%s", a); \
    Send(BC_C_VENDOR, (char *) __c_vendor, strlen(__c_vendor)); \
    } while (0)

#define SET_ADD_LIMIT_STEP(a) \
    m_block_add_limit_step = a

#define GET_CUMFAIL(a) \
    do { \
    if (RUNMODE) { \
    Send(BC_GET_CUMFAIL_COUNT, 0, 0); \
    recv(0, (char *) &a, 4, 0); \
    } \
    } while (0)

#define GET_CUMYIELD(a) \
    do { \
    if (RUNMODE) { \
    unsigned int _yield; \
    Send(BC_GET_CUMFAIL_YIELD, 0, 0); \
    recv(0, (char *) &_yield, 4, 0); \
    a = _yield / 100.0; \
    } \
    } while (0)

#define SET_MASK(a) \
    do { \
    unsigned int temp = a; \
    m_use_mask = a; \
    if (m_use_mask == false && m_init_fail && m_test_fail) \
    memcpy(m_test_fail, m_init_fail, m_bib_x * m_bib_y * NSLOT); \
    Send(BC_SETMASK, &temp, sizeof(unsigned int)); \
    } while (0)

#define SET_EXCL_MASK(a) \
    do { \
    unsigned int temp = a; \
    m_use_excl_mask = a ; \
    Send(BC_SETEXCLMASK, &temp, sizeof(unsigned int)); \
    } while (0)

#define SET_STEPCNT(a, b) \
    do { \
    m_set_step_count = a; \
    m_step_merge_type = b; \
    } while (0)

/*------- CAT -------*/
#define CAT_CHIPID_VER(a) \
    m_cat_chipid_ver = a

#define CDEC_ITEM(a, b) \
    m_cat_cdec[a] = b

#define READ_CCHIP_DATA \
    CAT_CHIP_DATA
/*-------------------*/

#define SET_INITMAP(a) \
    do { \
    m_use_setmap = true; \
    unsigned int temp = a; \
    Send(BC_PREMAP_CNT, &temp, sizeof(unsigned int)); \
    } while (0)

#define DIAG_SYNC() \
    do { \
    unsigned int    r = 0; \
    Send(BC_DIAGSYNC, &r, sizeof(r)); \
    recv(0, &r, sizeof(r), 0); \
    } while (0)

#define SET_TEMP_AMBIENT(a, b, c)	{ \
					float	temp[3] = { a, b, c }; \
					Send(BC_SETTEMP_AMBIENT, temp, sizeof(temp)); \
					if (b) \
						recv(0, temp, 4, 0); \
				}

#define SET_DOOR_OPEN()		{	\
					int	temp = 1; \
					Send(BC_SETDOOR, &temp, 4); \
					recv(0, (char*)&temp, 4, 0); \
				}

#define SET_DOOR_CLOSE()	{	\
					int	temp = 0; \
					Send(BC_SETDOOR, &temp, 4); \
					recv(0, (char*)&temp, 4, 0); \
				}

#define SET_CMB_STOP()        {       \
	                                int     temp = 0; \
                                        Send(BC_CMBSTOP, &temp, 4); \
                                        recv(0, (char*)&temp, 4, 0); \
                                }

#define SET_RATE(a) \
do { \
	int	t = a; \
	Send(BC_SETRATE, (char *) &t, sizeof(t)); \
} while (0)

#define INITIAL()	m_hottemp = true

#define INIT_ACTION() \
do { \
	int temp[16] = { 0 }, i; \
	confirm_fail(); \
	for (i = 0; i < 8; i++) { \
		temp[i] = m_icount[i]; \
		temp[8 + i] = m_tcount[i]; \
	} \
	Send(BC_INITTEMP, temp, sizeof(temp)); \
	recv(0, (char *) &i, sizeof(int), 0); \
} while (0)

#define SET_SOAKTIME(a)		\
do { \
	int t = a;	\
	Send(BC_SET_SOAKTIME, (char *)&t, sizeof(t));   \
} while (0)

#define SET_ITEM_TEST_TIME(a) \
do { \
	int	t = a; \
	Send(BC_TESTTIME, (char *) &t, sizeof(t)); \
} while (0)

#define SET_ITEM_STEP_TIME(a) \
do { \
	int	t = a; \
	Send(BC_TESTSTEPTIME, (char *) &t, sizeof(t)); \
} while (0)

#define SET_TIME_DISABLE()	Send(BC_TESTTIME_STATE, 0, 0);

#define SET_AMBIENT()		Send(BC_AMBIENT_START, 0, 0); 


#define EXEC(a)			do { \
					char cmd[128] = a; \
					Send(BC_EXEC, cmd, sizeof(cmd)); \
				} while (0);


const unsigned int ODD = 1;
const unsigned int EVEN = 2;

const unsigned int _OR = 0;
const unsigned int _AND = 1;
const unsigned int _NOT = 2;

const unsigned int FAIL = 1;
const unsigned int PASS = 2;
const unsigned int BLOCK = 3;
const unsigned int F_MASK = 4;
const unsigned int F_FAIL = 5;
const unsigned int F_PASS = 6;
const unsigned int L_MASK = 8;
const unsigned int L_FAIL = 9;
const unsigned int L_PASS = 10;
const unsigned int D_MASK = 16;
const unsigned int G_MASK = 32;

const unsigned int MAX_CHIP = 480;
const unsigned int MAX_NAND_CHIP = 240;
const unsigned int MAX_BIST_SIZE = 4096;
const unsigned int MAX_STEP = 510;
const unsigned int MAX_STEP_BIT_L = (((MAX_STEP) + 2) / 8);
const unsigned int MAX_CATEGORY = 1000;
const unsigned int MAX_CATEGORY_BIT_L = MAX_CATEGORY / 8;
const unsigned int MAX_NBYTE = 4080;
const unsigned int MAX_DQ_VALUE = 256;

/*------- DEV OS TYPE -------*/
const unsigned int ALL_TYPE = 0;
const unsigned int T33_ONLY = 1;
const unsigned int T10_ONLY = 2;
/*---------------------------*/

/*------- CAT -------*/
const unsigned int FW_2_0_76 = 0;
const unsigned int FW_2_0_80 = 1;
/*-------------------*/

const unsigned int CUM_FAIL = 0x00000;
const unsigned int ITEM_FAIL = 0x10000;

const unsigned int CD_WRITE = 0;
const unsigned int IMPRINT = 1;
const unsigned int MEMORY = 2;

const unsigned int ALL_FAIL = 1;
const unsigned int ALL_BLOCK_FAIL = 2;

const unsigned int NORMAL = 0x0;
const unsigned int INIT_FAIL = 0x1;
const unsigned int ADD_FAIL = 0x2;
const unsigned int ECC_FAIL = 0x3;
const unsigned int MUST_FAIL = 0x4;
const unsigned int AGING = 0x5;
const unsigned int GRADE_TEST = 0x6;

const unsigned int JUD_NORMAL = 0;
const unsigned int JUD_CLEAN = 1;
const unsigned int JUD_64M1 = 2;
const unsigned int JUD_64M2 = 3;
const unsigned int JUD_BCAU = 4;

const unsigned int FAIL_MASK = 1;
const unsigned int PASS_MASK = 2;
const unsigned int BLOCK_DATA = 4;

const unsigned int MULTI_AND = 0;
const unsigned int MULTI_OR = 1;

const unsigned int SCAN_23 = 0x01000060;
const unsigned int SCAN_45 = 0x01000050;
const unsigned int SCAN_67 = 0x01000040;
const unsigned int SCAN_89 = 0x01000030;
const unsigned int SCAN_1011 = 0x01000020;
const unsigned int SCAN_1213 = 0x01000010;
const unsigned int SCAN_1415 = 0x00000000;
const unsigned int SCAN_1617 = 0x00000010;
const unsigned int SCAN_1819 = 0x00000020;
const unsigned int SCAN_2021 = 0x00000030;
const unsigned int SCAN_2223 = 0x00000040;
const unsigned int SCAN_2425 = 0x00000050;
const unsigned int SCAN_2627 = 0x00000060;
const unsigned int SCAN_2829 = 0x00000070;

const unsigned int CONSTANT = 0;
const unsigned int AMBIENT = 1;
const unsigned int CMBSTOP = 3;
const unsigned int DAMPER_OPEN = 2;
const unsigned int DAMPER_CLOSE = 0;

const unsigned int IO_DEFINE_BUFFER_LENGTH = 8;

class SWControl : public HWControl
{
public:
    SWControl();
    virtual ~SWControl();

    PGCompiler *getPgCompiler() { return m_patternCompiler; }
    TGCompiler *getTgCompiler() { return m_timeCompiler; }
    timesetGroup *getTgGroup() { return &m_tgGroup; }
    patternGroup *getPgGroup() { return &m_pgGroup; }
    unsigned short DEV_TESTMODE() { return m_test_mode; }
    unsigned short STEP_TYPE() { return m_step_type; }
    unsigned int BLOCK_LIMIT() { return m_block_limit; }
    unsigned int BIB_X() { return m_bib_x; }
    unsigned int BIB_Y() { return m_bib_y; }
    unsigned int BIB_SCAN() { return m_bib_scan; }
    unsigned int BIB_IO_SIZE() { return m_bib_io_size; }
    unsigned int BIB_IO() { return m_bib_io_count_per_1scan; }
    unsigned short MCP() { return m_mcp; }
    unsigned short NCHIP() { return m_nchip; }
    unsigned short SCAN_SET() { return m_scan_set; }
    unsigned short EMMC_CE() { return m_emmc_ce; }
    unsigned int FM_READ_COUNT() { return m_fm_read_buf_index; }

    void SET_SCAN_SET(unsigned short v) { m_scan_set = v; }
    void SET_EMMC_CE(unsigned short v) { m_emmc_ce = v; }
    void SET_DEV_TESTMODE(unsigned short v) { m_test_mode = v; }
    void SET_STEP_TYPE(unsigned short v) { m_step_type = v; }
    void SET_DEV_OS_TYPE(unsigned int type) { m_dev_os_type = type; }
    void SET_PAT_LIMIT_TIME(long time) { m_mpat_limit_time = time; }
    void SET_FM_READ_COUNT(unsigned int v) { m_fm_read_buf_index = v; }
    //	void SET_FM_READ_SCAN_COUNT(unsigned int v) { m_fm_read_scan_count = v; }
    void SET_FM_READ_DATA_INDEX(unsigned int v) { m_fm_read_data_index = v; }
    void SET_TEST_CE(unsigned short start, unsigned short end)
    {
        m_start_mcp = start;
        m_end_mcp = end > m_mcp ? m_mcp : end;
    }
    void SET_FM_16IOMODE(bool v) { m_16io_mode = v; }
    void SET_RDQ_COUNT(unsigned short v) { m_set_rdq_count = v; }
    void SET_RDQ_INDEX(unsigned short v) { m_rdq_index = v; }

    BIB_FIND *FIND_BOARD(int bib = 0);
    String GET_PART_NAME();
    String GET_LOT_NAME();
    int GET_LOT_NAME(int &no, String *name);
    String  GET_PGM_NAME();
    String  GET_SYSTEM_NAME();
    String  GET_START_TIME();
    String  GET_END_TIME();
    char*   GET_OWNER();

    /*------- EMMC -------*/
    unsigned char EMMC_BIT_VALUE() { return m_emmc_bit_value; }
    unsigned char EMMC_INIT_DISPLAY_COUNT() { return m_emmc_init_display_count; }
    unsigned char EMMC_TEST_DISPLAY_COUNT() { return m_emmc_test_display_count; }
    unsigned short EMMC_CHIPID_UNIT_COUNT() { return m_emmc_chipid_unit_count; }
    unsigned short EMMC_FM_DISPLAY_COUNT() { return m_emmc_fm_display_count; }
    void SET_EMMC_BIT_VALUE(unsigned char v) { m_emmc_bit_value = v; }
    void SET_EMMC_INIT_DISPLAY_COUNT(unsigned char v) { m_emmc_init_display_count = v; }
    void SET_EMMC_TEST_DISPLAY_COUNT(unsigned char v) { m_emmc_test_display_count = v; }
    void SET_EMMC_CHIPID_UNIT_COUNT(unsigned short v) { m_emmc_chipid_unit_count = v; }
    void SET_EMMC_FM_DISPLAY_COUNT(unsigned short v) { m_emmc_fm_display_count = v; }
    /*--------------------*/

    /*------- coordinate -------*/
    int mapindex_to_si_raw(int index, unsigned int &scan, unsigned int &io);
    int index_to_si(int index, unsigned int &scan, unsigned int &io);
    int index_to_xy(int index, unsigned int &x, unsigned int &y);
    int si_to_index(unsigned int scan, unsigned int io, unsigned int &index);
    int xy_to_index(unsigned int x, unsigned int y, unsigned int &index);
    int si_to_xy(unsigned int scan, unsigned int io, unsigned int &x, unsigned int &y);
    int xy_to_si(unsigned int x, unsigned int y, unsigned int &scan, unsigned int &io);
    /*--------------------------*/

    /*------- GET / SET DATA -------*/
    bool IS_PRE_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos);
    bool IS_PRE_FAIL(unsigned int board, unsigned int dut);
    bool IS_CUM_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos);
    bool IS_CUM_FAIL(unsigned int board, unsigned int dut);
    bool IS_CUM_FAIL_MASK(unsigned int board, unsigned int x_pos, unsigned int y_pos);
    bool IS_CUM_FAIL_MASK(unsigned int board, unsigned int dut);
    bool IS_STEP_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos);
    bool IS_STEP_FAIL(unsigned int board, unsigned int dut);
    bool IS_STEP_IO_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int io);
    bool IS_STEP_IO_FAIL(unsigned int board, unsigned int dut, unsigned int io);
    unsigned int  GET_STEP_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos);
    unsigned int  GET_STEP_FAIL(unsigned int board, unsigned int dut);
    bool IS_CUM_FAIL_CHIP(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index);
    bool IS_CUM_FAIL_CHIP(unsigned int board, unsigned int dut, unsigned int chip_index);
    bool IS_STEP_FAIL_CHIP(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index);
    bool IS_STEP_FAIL_CHIP(unsigned int board, unsigned int dut, unsigned int chip_index);
    bool IS_TEST_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos);
    bool IS_TEST_FAIL(unsigned int board, unsigned int dut);
    bool IS_CUM_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block);
    bool IS_CUM_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block);
    char IS_CUM_BLOCK_FAIL8(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block);
    char IS_CUM_BLOCK_FAIL8(unsigned int board, unsigned int dut, unsigned int block);

    bool IS_STEP_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block);
    bool IS_STEP_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block);
    char IS_STEP_BLOCK_FAIL8(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block);
    char IS_STEP_BLOCK_FAIL8(unsigned int board, unsigned int dut, unsigned int block);

    bool IS_GT_STEP_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block);
    bool IS_GT_STEP_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block);
    bool IS_INIT_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block);
    bool IS_INIT_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block);

    void SET_PRE_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, bool value);
    void SET_PRE_FAIL(unsigned int board, unsigned int dut, bool value);
    void SET_CUM_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, bool value);
    void SET_CUM_FAIL(unsigned int board, unsigned int dut, bool value);
    void SET_CUM_FAIL_MASK(unsigned int board, unsigned int x_pos, unsigned int y_pos, bool value);
    void SET_CUM_FAIL_MASK(unsigned int board, unsigned int dut, bool value);
    void SET_STEP_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, bool value);
    void SET_STEP_FAIL(unsigned int board, unsigned int dut, bool value);
    void SET_STEP_IO_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int value);
    void SET_STEP_IO_FAIL(unsigned int board, unsigned int dut, unsigned int value);
    void SET_CUM_FAIL_CHIP(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, bool value);
    void SET_CUM_FAIL_CHIP(unsigned int board, unsigned int dut, unsigned int chip_index, bool value);
    void SET_STEP_FAIL_CHIP(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, bool value);
    void SET_STEP_FAIL_CHIP(unsigned int board, unsigned int dut, unsigned int chip_index, bool value);
    void SET_TEST_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, bool value);
    void SET_TEST_FAIL(unsigned int board, unsigned int dut, bool value);
    void SET_CUM_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block, bool value);
    void SET_CUM_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block, bool value);
    void SET_STEP_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block, bool value);
    void SET_STEP_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block, bool value);
    void SET_GT_STEP_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block, bool value);
    void SET_GT_STEP_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block, bool value);
    void SET_INIT_BLOCK_FAIL(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int block, bool value);
    void SET_INIT_BLOCK_FAIL(unsigned int board, unsigned int dut, unsigned int block, bool value);

    int CUM_BLOCK_FAIL_COUNT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int mcp);
    int CUM_BLOCK_FAIL_COUNT(unsigned int board, unsigned int dut, unsigned int mcp);
    int STEP_BLOCK_FAIL_COUNT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int mcp);
    int STEP_BLOCK_FAIL_COUNT(unsigned int board, unsigned int dut, unsigned int mcp);

    int CUM_BLOCK_FAIL_ADDRESS(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int mcp,
                               unsigned int index);
    int CUM_BLOCK_FAIL_ADDRESS(unsigned int board, unsigned int dut, unsigned int mcp, unsigned int index);
    int STEP_BLOCK_FAIL_ADDRESS(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int mcp,
                                unsigned int index);
    int STEP_BLOCK_FAIL_ADDRESS(unsigned int board, unsigned int dut, unsigned int mcp, unsigned int index);
    /*------------------------------*/

    /*------- READ FM DATA -------*/
    unsigned char TMP_FM_DATA(unsigned int board, unsigned int io, unsigned int index);
    unsigned char FM_DATA(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index,
                          unsigned int byte_index);
    unsigned char FM_DATA(unsigned int board, unsigned int dut, unsigned int chip_index, unsigned int byte_index);
    void SET_FM_DATA(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index,
                     unsigned int byte_index, unsigned char value);
    void SET_FM_DATA(unsigned int board, unsigned int dut, unsigned int chip_index,
                     unsigned int byte_index, unsigned char value);
    /*----------------------------*/

    /*------- READ BIST DATA -------*/
    char *BIST_DATA(unsigned int board, unsigned int x_pos, unsigned int y_pos);
    char *BIST_DATA(unsigned int board, unsigned int dut);
    /*------------------------------*/

    /*------- READ DQ DATA -------*/
    int DQ_DATA(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int dq_index);
    int DQ_DATA(unsigned int board, unsigned int dut, unsigned int dq_index);
    /*----------------------------*/

    /*------- GET / SET CHIPID -------*/
    char *CHIPID_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index);
    char *CHIPID_V(unsigned int board, unsigned int dut, unsigned int chip_index);
    int WAFERID_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index);
    int WAFERID_V(unsigned int board, unsigned int dut, unsigned int chip_index);
    int XPOS_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index);
    int XPOS_V(unsigned int board, unsigned int dut, unsigned int chip_index);
    int YPOS_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index);
    int YPOS_V(unsigned int board, unsigned int dut, unsigned int chip_index);

    void SET_CHIPID_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, char *value);
    void SET_CHIPID_V(unsigned int board, unsigned int dut, unsigned int chip_index, char *value);
    void SET_WAFERID_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, unsigned int value);
    void SET_WAFERID_V(unsigned int board, unsigned int dut, unsigned int chip_index, unsigned int value);
    void SET_XPOS_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, unsigned int value);
    void SET_XPOS_V(unsigned int board, unsigned int dut, unsigned int chip_index, unsigned int value);
    void SET_YPOS_V(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int chip_index, unsigned int value);
    void SET_YPOS_V(unsigned int board, unsigned int dut, unsigned int chip_index, unsigned int value);
    /*--------------------------------*/

    /*------- CATEGORY & SORT -------*/
    void JUDGE_FORCE(unsigned int mode, unsigned int bin);
    bool FCAT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int fail_cate);
    bool FCAT(unsigned int board, unsigned int dut, unsigned int fail_cate);
    bool PCAT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int pass_cate);
    bool PCAT(unsigned int board, unsigned int dut, unsigned int pass_cate);
    unsigned int BIN(unsigned int board, unsigned int x_pos, unsigned int y_pos);
    unsigned int BIN(unsigned int board, unsigned int dut);
    unsigned int BIN_MARK(unsigned int board, unsigned int x_pos, unsigned int y_pos);
    unsigned int BIN_MARK(unsigned int board, unsigned int dut);
    void SET_FCAT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int fail_cate);
    void SET_FCAT(unsigned int board, unsigned int dut, unsigned int fail_cate);
    void SET_PCAT(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int pass_cate);
    void SET_PCAT(unsigned int board, unsigned int dut, unsigned int pass_cate);
    void SET_BIN(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int bin);
    void SET_BIN(unsigned int board, unsigned int dut, unsigned int bin);
    void SET_BIN_MARK(unsigned int board, unsigned int x_pos, unsigned int y_pos, unsigned int value);
    void SET_BIN_MARK(unsigned int board, unsigned int dut, unsigned int value);
    void SET_FAULTBIN(unsigned int bin);
    void SET_STEP_FAULTBIN(unsigned int bin);
    /*-------------------------------*/

    /*------- PPN (CAT) Chip -------*/
    void CAT_ACTION_CATE(int c, ...);
    void CAT_LIMIT(int l, ...);
    void CAT_DECODE(unsigned int v, unsigned char bit, char *s, unsigned int len);
    void CAT_CHIP_DATA(unsigned int libno, unsigned int length, const char *data_type);
    virtual int CID_X_CAL(int x) { return x; }
    virtual int CID_Y_CAL(int y) { return y; }
    virtual int CID_WAFER_CAL(int wafer) { return wafer; }
    virtual String CID_LOTID_CAL(String lotid) { return lotid; }
    /*------------------------------*/

    /*------- IO_DEFINE -------*/
    unsigned int IO_DEFINE_FAIL_DEVICE(unsigned int fail_device);
    unsigned int IO_DEFINE_FAIL_DEVICE_REVERSE(unsigned int fail_device);
    unsigned long long IO_DEFINE_FAIL_LATCH(unsigned long long fail_dq);
    void IO_DEFINE_FAIL_DATA(unsigned char *result, unsigned char *fail_data);
    void IO_DEFINE_FAIL_DATA_ADDR(unsigned char *result_data, unsigned char *fail_data,
                                  unsigned short *result_addr, unsigned short *fail_addr);
    void IO_DEFINE_FAIL_COUNTER(unsigned int *result, unsigned int *fail_counter);
    void SET_IO_DEFINE(unsigned int io_group0, unsigned int io_group1, unsigned int io_group2, unsigned int io_group3,
                       unsigned int io_group4, unsigned int io_group5, unsigned int io_group6, unsigned int io_group7);
    /*-------------------------*/

    void BIB(int d, ...);
    void SET_BIB(unsigned int scan, unsigned int io, unsigned int bibx = 0, unsigned int biby = 0, bool f_16mode = 0);
    void SET_BIB_EX(unsigned int scan, unsigned int io, unsigned int io_count_per_1scan,
                    unsigned int bibx, unsigned int biby, bool f_16mode = 0);
    void SET_IOMASK(long long a, long long b = -1);
    void SET_LIMIT(unsigned int bl, unsigned int bfl, unsigned int bal);
    void SET_ADD_SIZE(unsigned int block_shift, unsigned int page_shift, unsigned int page_div,
                      unsigned int mcp = 1, unsigned int nchip = 1);
    void SET_JUDGE(int r);
    void SET_DEVICE(int mode, unsigned short device_code = 0);
    bool SET_BOARD(int board = -1, bool keep_level = false);
    void SET_VLOOP_COUNT(unsigned int index, unsigned int v, ...);
    void SET_VLOOP_COUNT_DATA(unsigned int *s, unsigned int size, unsigned int index = 0);
    void SET_VECTOR(unsigned int index, unsigned int v, ...);
    void SET_VECTOR_DATA(unsigned int *s, unsigned int size, unsigned int index = 0);
    void SET_VECTOR_OFF();
    void SET_VECTOR_EMUL(unsigned int v, ...);
    void SET_VECTOR_EMUL_DATA(unsigned int *s, unsigned int size);
    void SET_VECTOR_EMUL_OFF();
    void SET_SCAN(int b, ...);
    void SET_PS(int ch, double voltage, double delay, double high = 0, double low = 0, double c_limit = 0);
    void write_i2cid(int ps, bool Merge);
    void i2c_init();
    //void set_i2c(double voltage, bool is_up, int ps);
    void set_i2c(double voltage, bool is_up, int ps, double high = 0, double low = 0, double c_limit = 0);
    //void verify_i2c(int ps);
    void sort_channel(int ch, double voltage, double high, double low, double c_limit);
    void readPSCommon(bool &bcommon, bool &bcommon2);
    int check_psflag(bool bcommon, bool bcommon2, int &enbit);
    void SET_PS3(double voltage, double delay1, double delay2, double high = 0, double low = 0, double c_limit = 0);
    void SET_PSA(VOLTINFO *vi, unsigned int length = 0);
    void set_da();
    void SET_VIH(int ch, double voltage, double delay);
    void SET_VIL(int ch, double voltage, double delay);
    void SET_VTH(double voltage, double delay);
    void MEAS_MPAT(const char *name, double wait = 0, short xa = 0, short ya = 0, int da = 0, char sa = 0);
    bool START_MPAT(const char *pat, double wait = 0, short xa = 0, short ya = 0, int da = 0, char sa = 0);
    void STOP_MPAT();
    void WAIT_MPAT(double wait = 0);
    int WAIT_PAUSE(double wait = 0);
    void CONTINUE_MPAT();
    void RESET_TESTER();
    void SET_TIME(const char *group = "internel");
    void SET_MPAT(const char *group);
    void SET_SCR(const char *group, Scramble &scramble);
    void SET_AGING(int time);
    void SET_EMMC_COMPARE(unsigned int bit);
    void SET_EMMC_COMPARE(unsigned int bit, unsigned long long value);
    void SET_EMMC_EXPECT(unsigned int bit, unsigned long long value = 0llu);
    void SET_READ_NAND_CHIPID();

    void RATE(double r0 = 0, double r1 = 0, double r2 = 0, double r3 = 0,
              double  r4 = 0, double  r5 = 0, double  r6 = 0, double  r7 = 0,
              double  r8 = 0, double  r9 = 0, double r10 = 0, double r11 = 0,
              double r12 = 0, double r13 = 0, double r14 = 0, double r15 = 0,
              double r16 = 0, double r17 = 0, double r18 = 0, double r19 = 0,
              double r20 = 0, double r21 = 0, double r22 = 0, double r23 = 0,
              double r24 = 0, double r25 = 0, double r26 = 0, double r27 = 0,
              double r28 = 0, double r29 = 0, double r30 = 0, double r31 = 0);
    void CLK(int pin, char *c0 = 0, char *c1 = 0, char *c2 = 0, char *c3 = 0,
             char  *c4 = 0, char  *c5 = 0, char  *c6 = 0, char  *c7 = 0,
             char  *c8 = 0, char  *c9 = 0, char *c10 = 0, char *c11 = 0,
             char *c12 = 0, char *c13 = 0, char *c14 = 0, char *c15 = 0,
             char *c16 = 0, char *c17 = 0, char *c18 = 0, char *c19 = 0,
             char *c20 = 0, char *c21 = 0, char *c22 = 0, char *c23 = 0,
             char *c24 = 0, char *c25 = 0, char *c26 = 0, char *c27 = 0,
             char *c28 = 0, char *c29 = 0, char *c30 = 0, char *c31 = 0);
    void TS(char *n0 = 0, char *n1 = 0, char *n2 = 0, char *n3 = 0,
            char  *n4 = 0, char  *n5 = 0, char  *n6 = 0, char  *n7 = 0,
            char  *n8 = 0, char  *n9 = 0, char *n10 = 0, char *n11 = 0,
            char *n12 = 0, char *n13 = 0, char *n14 = 0, char *n15 = 0,
            char *n16 = 0, char *n17 = 0, char *n18 = 0, char *n19 = 0,
            char *n20 = 0, char *n21 = 0, char *n22 = 0, char *n23 = 0,
            char *n24 = 0, char *n25 = 0, char *n26 = 0, char *n27 = 0,
            char *n28 = 0, char *n29 = 0, char *n30 = 0, char *n31 = 0);

    //	int SET_PG(const char *name, int offset, int data, int which);
    //	bool SET_TG(const char *name, const char *pin, int edge, ...);
    int FM_READ_RESULT();
    int GET_BLOCK_DATA(char *step_name);
    int GET_NBYTE_MATCH(double &result, char *base, unsigned int chip_index, unsigned int byte_index, unsigned int max_byte);
    int getPC(const char *name);
    void tsCompile(const char *group, const char *name, const char *source);
    void subCompile(const char *group, const char *name, const char *source);
    int Recv(char *buf, unsigned int length);
    int Sync();
    int confirm_fail();
    void GET_LOT_YIELD( char *item_name, float lot_yield[32] );
    void GET_CUM_LOT_YIELD( float lot_yield[32] );
    void SET_LOTBINSET( int select_lot, int select_bin );
    void SET_BOARD_DISABLE( int select_lot, bool state );
    void SET_BOARD_ENABLE();

    void MULTIBIN_SET(int bin10, int bin11, int bin12, int bin13, int bin14, int bin15);
    void MULTIBIN_SET(int bin1, int bin2, int bin3, int bin4, int bin5, int bin6, int bin7, int bin8, int bin9, int bin10, int bin11, int bin12, int bin13, int bin14, int bin15);
    void SET_WAFER_PDA(char *RUNNAME, int QTY_CNT, bool PDA_STATE = true);
	void SET_BIN_REF1(double binref1, double binref2 = 0, double binref3 = 0, double binref4 = 0, double binref5 = 0, double binref6 = 0, double binref7 = 0, double binref8 = 0, double binref9 = 0, double binref10 = 0, double binref11 = 0, double binref12 = 0, double binref13 = 0, double binref14 = 0, double binref15 = 0);
	void SET_BIN_REF2(double binref1, double binref2 = 0, double binref3 = 0, double binref4 = 0, double binref5 = 0, double binref6 = 0, double binref7 = 0, double binref8 = 0, double binref9 = 0, double binref10 = 0, double binref11 = 0, double binref12 = 0, double binref13 = 0, double binref14 = 0, double binref15 = 0);
	void SET_BIN_REF3(double binref1, double binref2 = 0, double binref3 = 0, double binref4 = 0, double binref5 = 0, double binref6 = 0, double binref7 = 0, double binref8 = 0, double binref9 = 0, double binref10 = 0, double binref11 = 0, double binref12 = 0, double binref13 = 0, double binref14 = 0, double binref15 = 0);

	void InterLock_Time1(int onoff, int time = 0);
	void InterLock_Time2(int onoff, int time = 0);
	void InterLock_Time3(int onoff, int time = 0);
	void InterLock_Time4(int onoff, int time = 0);
	void InterLock_Time_AllStop();
	void INTERLOCK_TIME_MAX1(int onoff, int time = 0);
	void INTERLOCK_TIME_MAX2(int onoff, int time = 0);
	void INTERLOCK_TIME_MAX3(int onoff, int time = 0);
	void INTERLOCK_TIME_MAX4(int onoff, int time = 0);
	void INTERLOCK_TIME_MIN1(int onoff, int time = 0);
	void INTERLOCK_TIME_MIN2(int onoff, int time = 0);
	void INTERLOCK_TIME_MIN3(int onoff, int time = 0);
	void INTERLOCK_TIME_MIN4(int onoff, int time = 0);
	void INTERLOCK_TIME_ALLSTOP();

	void SET_USERALARM();
	void SET_USERSYSTEMDOWN();
	int  SET_USERPOPUP(char *str);
	void PAUSE();
	void SET_USERSENDMONITORING( const char *strMESSAGE, const char *strCOMMENT );

    void readPSRev();
    void readBDRev();
    void SET_READ_FAULT_TIMER(unsigned long long sec);
    void SET_REV();

    STEPINFO m_cur_step;
    bool m_shmoo_init;
    bool m_shmoo_test;
    int  m_shmoo_save_ch[2];
    float m_shmoo_save_data[2];
    int m_tsCount;
    int m_icount[8];
    int m_tcount[8];
    int m_ocount[8];
    char ts_name[1000];
    BIB_FIND _bi_find[MAX_NSLOT];
    unsigned long long m_fault_timer;

protected:
    bool _APP(unsigned int value);
    virtual bool APP(int value);
    virtual int SET_PF(unsigned int mode);
    virtual int SET_FM_READ_DATA(unsigned int base, unsigned int i_block, unsigned int index = 0);
    virtual int SET_FM(int board, unsigned int fm_mode = 0, int s_scan = -1, int e_scan = -1);
    virtual int SET_EMMC_READ_DATA(unsigned int index = 0);
    virtual int SET_DRAM_READ_CHIPID(unsigned int index = 0);
    virtual void SET_BLOCK(int addr, int s, int &address, int &scan);
    virtual bool RUNTIME_ERROR(const char *);
    virtual bool BURST(const char *pattern_name, int burst);
    virtual bool PAUSE(const char *pattern_name);
    virtual int OPTION_CALL(int command, int wparam = 0, int lparam = 0);
    void INIT_READ_CHIPID();
    void getHICInfo(unsigned short chip_count, unsigned short data_length, unsigned short chip_size, const char *data_type);
    void DRAM_READ_CHIP_DATA(unsigned short chip_count, const char *data_type);
    void EMMC_READ_CHIP_DATA(unsigned short chip_count, const char *data_type);
    void CALC_HICHIP();
    unsigned char find_max_value(unsigned char *buf, unsigned int len);

    int do_bistdata_fmt_1(unsigned int b, unsigned int x, unsigned int y);
    int do_bistdata_fmt_2(unsigned int b, unsigned int x, unsigned int y);
    int do_emmcdata_fmt_0(unsigned int b, unsigned int x, unsigned int y);
    int do_emmcdata_fmt_1(unsigned int b, unsigned int x, unsigned int y);
    int do_emmcdata_fmt_2(unsigned int b, unsigned int x, unsigned int y);
    int do_emmcdata_fmt_3(unsigned int b, unsigned int x, unsigned int y);
    int do_emmcdata_comp_expect(char *buf);
    int do_get_cum_data(unsigned int mode);
    int do_get_cate_bin_data(unsigned int mode);

    int STEP_START();
    int STEP_END();
    int RESULT_CHIPID_DATA(unsigned int mode);

    bool m_init_test;
    bool m_eng_mode;	// manual mode 이면 true
    bool m_dq_mode;	// dq data display
    bool m_dq_use;
    bool m_app_value;
    bool m_hichp_use;
    bool m_read_nand_chipid;
    bool m_aging_use;
    bool m_aging_end;
    bool m_skip_status;
    bool m_all_skip;
    bool m_use_mask;
    bool m_use_excl_mask;
    bool m_use_setmap;
    bool m_pf_state;
    unsigned char m_emmc_bit_value;
    unsigned char m_emmc_init_display_count;
    unsigned char m_emmc_test_display_count;
    unsigned char m_init_count;
    unsigned char m_test_count;
    unsigned char m_cat_chipid_ver;
    unsigned short m_mcp;
    unsigned short m_nchip;
    unsigned short m_start_mcp;
    unsigned short m_end_mcp;
    unsigned short m_scan_set;
    unsigned short m_emmc_ce;
    unsigned short m_emmc_chipid_unit_count;
    unsigned short m_emmc_fm_display_count;
    unsigned short m_device_mode;
    unsigned short m_device_code;
    unsigned short m_test_mode;
    unsigned short m_ready_bit;
    unsigned short m_read_rdq_count;
    unsigned short m_set_rdq_count;
    unsigned short m_rdq_index;
    unsigned short m_hic_length;
    unsigned short m_hic_count;
    unsigned short m_hic_size;
    unsigned short m_dram_read_chipid_count;
    unsigned short m_step_type;
    unsigned short m_use_io_count;
    unsigned int m_dev_os_type;
    unsigned int m_block_limit;
    unsigned int m_block_fail_limit;
    unsigned int m_block_shift;
    unsigned int m_page_shift;
    unsigned int m_page_div;
    unsigned int m_test_dut_count;
    unsigned int m_block_add_limit;
    int m_block_add_limit_step;
    //	int m_fail_index;
    unsigned int m_fail_mode;
    unsigned int m_judge_mode;
    int m_ecc_index;
    int m_ecc_page;
    int m_ecc_io;
    int m_ecc_block;
    unsigned int m_emmc_compare_bit;
    unsigned int m_emmc_expect_bit;
    int _test_mode;
    int m_aging_stime;
    int m_aging_etime;
    unsigned int m_fm_read_buf_index;
    //	unsigned int m_fm_read_scan_count;
    unsigned int m_fm_read_data_index;
    int m_board_index;
    int m_step_no;
    int m_old_step_no;
    unsigned int m_now_step_count;
    unsigned int m_set_step_count;
    unsigned int m_step_merge_type;
    unsigned int m_bib_scan;
    unsigned int m_bib_io_size;
    unsigned int m_bib_io_count_per_1scan;
    unsigned int m_bib_x;
    unsigned int m_bib_y;
    int m_scan;
    int m_count;
    bool m_hottemp;
	bool m_tempinfo_mode;
    unsigned int m_sync_index;
    /*
     * m_use_mode
     *     0 : NONE DATA Send & Recv STEP (ex : TEMPERATURE ONLY)
     *     1 : TEST DATA STEP
     *     2 : BLOCK DATA STEP
     *     3 : PPN STEP
     *  0xff : After TEST END
         */
    unsigned int m_use_mode;
    unsigned int m_base_cat_step;
    unsigned int m_bist_errno_no;
    unsigned int m_bist_exception_cate;
    long m_mpat_limit_time;

    unsigned long long m_emmc_compare_value;
    unsigned long long m_emmc_expect_value;
    char m_hic_type[100];

    char *m_init_fail;	// INITIAL (PRETEST) FAIL [CUM]
    char *m_pre_init_fail;
    char *m_cum_fail;	// TEST FAIL [CUM]
    char *m_mask_fail;	// BLOCK FAIL COUNT MASK
    char *m_step_fail;	// TEST FAIL [NOW STEP]
    unsigned int *m_step_io_fail;
    char *m_cum_fail_chip;
    char *m_step_fail_chip;
    char *m_test_fail;
    char *m_test_data;
    unsigned int *m_test_io_data;
    char *m_cum_fail_block;
    char *m_now_step_fail_block;
    char *m_get_step_fail_block;
    char *m_init_fail_block;
    unsigned char *m_read_fm_data;
    char *m_chipid;
    unsigned int *m_waferid;
    unsigned int *m_xpos;
    unsigned int *m_ypos;
    unsigned int *m_emmc_offset;
    int *m_index_map;
    int *m_index_si_map;
    int *m_si_index_map;
    //	int *m_bib_map;
    //	int *m_scan_map;
    //	int *m_index_bib_map;
    //	int *m_index_scan_map;
    //	unsigned int *m_block_fail;
    unsigned int *m_ecc_fail;
    unsigned int *m_cat_limit;
    unsigned int *m_cat_action_cate;
    unsigned int *m_dq_value;
    unsigned long long *m_dq_data;
    FailBlock *m_failblock;
    PGCompiler *m_patternCompiler;
    TGCompiler *m_timeCompiler;
    timesetGroup m_tgGroup;
    patternGroup m_pgGroup;
    IOS m_comp_io_mask;
    unsigned char m_tmp_fm_data[MAX_NSLOT][8][4080];
    //	char m_skip_step[MAX_STEP_BIT_L];
    char *m_bist[MAX_NSLOT][MAX_NAND_CHIP];
    Map<unsigned char, unsigned char, char, char> m_cat_cdec;

    /*------- CATEGORY & SORT -------*/
    char m_fail_cate[MAX_NSLOT][MAX_CHIP][MAX_CATEGORY_BIT_L];
    char m_pass_cate[MAX_NSLOT][MAX_CHIP][MAX_CATEGORY_BIT_L];
    bool m_force_bin[MAX_NSLOT][MAX_CHIP];
    unsigned int m_bin[MAX_NSLOT][MAX_CHIP];
    unsigned int m_bin_mark[MAX_NSLOT][MAX_CHIP];
    /*-------------------------------*/

private:
    int app_id_ecc();
    int app_id_init();
    int app_id_data();
    int app_id_read_id();
    int app_id_status();
    int app_id_read_scan();
    int app_id_read_block();
    int app_id_read_blocks();
    int app_id_l1_result();
    int app_id_read_ecc();
    int app_id_read_io();
    int app_id_read_display();
    int app_id_pretest_data();
    int app_id_pretest_result();
    int app_id_bitest_data();
    int app_id_bitest_result();
    int app_id_fm_read_data();
    int app_id_dq_data();
    int app_id_dq_result();
    int app_id_bist_io_latch();
    int app_id_bist_data();
    int app_id_bist_result();
    int app_id_bist2_result();
    int app_id_emmc_io_latch();
    int app_id_emmc_data();
    int app_id_emmc_init();
    int app_id_emmc_result();
    int app_id_emmc_block_result();
    int app_id_emmc_chip_id();
    int app_id_emmc_read_data();
    bool runtimeError(const char *code);
    int DoPgrunWork(double time = 0, bool pause = false);

    bool m_16io_mode;
    bool m_ps1;
    bool m_ps2;
    bool m_ps4;
    bool m_ps5;
    int m_fm_page;
    int m_imprint_fail_count;
    int m_display_count;
    int m_page_mask;
    int m_ecc_fly_count;
    double m_ps1_voltage;
    double m_ps2_voltage;
    double m_ps4_voltage;
    double m_ps5_voltage;
    double m_ps1_high;
    double m_ps2_high;
    double m_ps4_high;
    double m_ps5_high;
    double m_ps1_low;
    double m_ps2_low;
    double m_ps4_low;
    double m_ps5_low;
    double m_ps1_limit;
    double m_ps2_limit;
    double m_ps3_limit;
    double m_ps4_limit;
    double m_ps5_limit;
    char *m_ecc_block_fail;
    unsigned int *m_ecc_fly_fail;
    unsigned int m_io_define[IO_DEFINE_BUFFER_LENGTH];
    String m_cur_pattern;
    double m_ps3_volt_save;
};

#endif
