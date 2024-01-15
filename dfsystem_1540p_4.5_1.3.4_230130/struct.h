
#ifndef _STRUCT_H_
#define _STRUCT_H_

#include "template.h"

typedef List<int, int> intList;
typedef List<String, char *> StringList;

typedef Map<String, char *, unsigned int, unsigned int> str2WordMap;
typedef Map<String, char *, String, char *> str2StrMap;
typedef Map<int, int, String, char *> int2StrMap;

typedef struct tagIOS
{
	unsigned long long da;
	unsigned long long db;
} IOS;

#define MAX_TS		128
#define MAX_LENGTH	10240
#define MIN_RATE	10
#define USE_BIT		1024
#define RATE_BIT	0

typedef union tagPGCMD
{
	unsigned int value;
	struct
	{
		unsigned int bit : 16;
		unsigned int opt : 8;
		unsigned int command : 8;
	} c;
} TPGCMD;

typedef struct tagSUB
{
	int pos;
	char *p;
	unsigned int flag;
} TSUB;

typedef List<TSUB *, TSUB *> subList;

typedef struct tagNOLABEL
{
	char label[32];
	int pc;
} TNOLABEL;

typedef List<TNOLABEL *, TNOLABEL *> noLabelList;

typedef struct tagTGDATA
{
	char name[32];
	int rate;
	int width;
	unsigned short data[0x60];
	unsigned short data2[0x20];
} TTGDATA;

typedef List<TTGDATA *, TTGDATA *> timeDataList;

typedef struct tagCHANNEL
{
	char name[32];
	unsigned int sys;
} TCHANNEL;

typedef List<TCHANNEL *, TCHANNEL *> channelList;

typedef struct tagTIMESET
{
	unsigned int clk_inv;
	unsigned int clk_inv2;
	char order[32];
	timeDataList list;
	channelList chlist;
	int timeset;
	int magic[32];
} TTIMESET;

typedef struct tagListData
{
	unsigned short start;
	unsigned short end;
	int bit;
} TLISTDATA;

typedef struct tagRAMDATA
{
	union
	{
		unsigned short opcode_ts;
		struct
		{
			unsigned short ts : 7;
			unsigned short ref_mask : 1;
			unsigned short opcode : 5;
			unsigned short ref_hold : 1;
			unsigned short dont_use : 1;
			unsigned short use : 1;
		} o;
	};

	union
	{
		unsigned short opcode_ts33;
		struct
		{
			unsigned short ts : 8;
			unsigned short opcode : 5;
			unsigned short ref_hold : 1;
			unsigned short ref_mask : 1;
			unsigned short use : 1;
		} pg33;
	};

	union
	{
		unsigned short jump;
		struct
		{
			unsigned short jump_add : 11;
			unsigned short dont_use : 4;
			unsigned short dbg : 1;
		} j;
	};

	union
	{
		unsigned int loop_count;
		struct
		{
			unsigned short low_count;
			unsigned short high_count;
			unsigned short spare;
		} l;
	};

	union
	{
		unsigned short x;
		struct
		{
			unsigned short	x_opcode : 3;
			unsigned short	x_src : 3;
			unsigned short	x_des : 3;
			unsigned short	x_out : 3;
			unsigned short	x_dir : 2; // 0:y=ya 1:y=ya&0xffff 2:y=ya|0xffff 3:y=0xffff
			unsigned short	x_inv : 1;
			unsigned short	x_xscr : 1;
		} ax;
	};

	unsigned short x_operand;
	unsigned short x_operdir;
	unsigned short x_direct;

	union
	{
		unsigned short x_fm;
		struct
		{
			unsigned short x_addr : 1;
		};
	};

	union
	{
		unsigned short y;
		struct
		{
			unsigned short y_opcode : 3;
			unsigned short y_src : 3;
			unsigned short y_des : 3;
			unsigned short y_out : 3;
			unsigned short y_dir : 2;	// 0:y=ya 1:y=ya&0xffff 2:y=ya|0xffff 3:y=0xffff
			unsigned short y_inv : 1;
			unsigned short y_xscr : 1;
		} ay;
	};

	unsigned short y_operand;
	unsigned short y_operdir;
	unsigned short y_direct;

	union
	{
		unsigned short y_fm;
		struct
		{
			unsigned short y_addr : 1;
		};
	};

	union
	{
		unsigned int data;
		struct
		{
			unsigned int d_operand : 18;
			unsigned int d_opcode : 2;
			unsigned int start : 1;
			unsigned int stop : 1;
			unsigned int d_src : 2;
			unsigned int d_des : 2;
			unsigned int d_out : 2;
			unsigned int d_dir : 1;
			unsigned int d_inv : 1;
			unsigned int d_expinv : 1;
			unsigned int d_xscr : 1;
		} d;

		struct
		{
			unsigned short d_oper;
			unsigned short d_op;
		} dop;
	};

	union
	{
		unsigned int scan_bank;
		struct
		{
			unsigned int b_operand : 4;
			unsigned int b_opcode : 2;
			unsigned int b_src : 2;
			unsigned int b_des : 2;
			unsigned int b_out : 2;
			unsigned int b_dir : 1;
			unsigned int b_inv : 1;
			unsigned int s_dir : 1;
			unsigned int s_all : 1;
			unsigned int s_operand : 8;
			unsigned int s_opcode : 2;
			unsigned int s_src : 2;
			unsigned int s_des : 2;
			unsigned int s_out : 2;
		} sb;

		struct
		{
			unsigned short bank;
			unsigned short scan;
		};
	};

	union
	{
		unsigned short vector;
		struct
		{
			unsigned short v_opcode : 2;
			unsigned short v_src : 2;
			unsigned short v_des : 2;
			unsigned short v_out : 2;
			unsigned short v_dir : 1;
			unsigned short v_nu : 7;
		} v;
	};

	unsigned short v_source;
	unsigned short v_srcdir;
	unsigned short v_direct;

	struct
	{
			unsigned short x_use : 1;
			unsigned short y_use : 1;
			unsigned short d_use : 1;
			unsigned short b_use : 1;
			unsigned short s_use : 1;
			unsigned short v_use : 1;
			unsigned short x_aluuse : 1;
			unsigned short y_aluuse : 1;
			unsigned short d_aluuse : 1;
			unsigned short b_aluuse : 1;
			unsigned short s_aluuse : 1;
			unsigned short v_aluuse : 1;
	} u;

	union
	{
		unsigned short s_fm;
		struct
		{
			unsigned short s_addr : 1;
			unsigned short s_next : 1;
			unsigned short s_zero : 1;
		};
	};
} TRAMDATA;

typedef List<TRAMDATA *, TRAMDATA *> ramList;

typedef struct tagMINMAX
{
	unsigned int xmin;
	unsigned int xmax;
	unsigned int ymin;
	unsigned int ymax;
	unsigned int dmin : 18;
	unsigned int bmin : 4;
	unsigned int smin : 8;
	unsigned int vmin;
	unsigned int vmax;
	unsigned int xmin_use : 1;
	unsigned int xmax_use : 1;
	unsigned int ymin_use : 1;
	unsigned int ymax_use : 1;
	unsigned int dmin_use : 1;
	unsigned int dmax_use : 1;
	unsigned int bmin_use : 1;
	unsigned int bmax_use : 1;
	unsigned int smin_use : 1;
	unsigned int smax_use : 1;
	unsigned int vmin_use : 1;
	unsigned int vmax_use : 1;
	unsigned int dmax : 18;
	unsigned int bmax : 4;
	unsigned int smax : 8;
} TMINMAX;

typedef struct tagPGDATA
{
	char name[32];
	TMINMAX minmax;
	int pc;
	unsigned int dbg_pc : 16;
	unsigned int dbg_op : 5;
	unsigned int scan_use : 1;
	unsigned int catch_use : 1;
	ramList *pram;
} TPGDATA;

typedef List<TPGDATA *, TPGDATA *> patternList;

typedef struct tagBIB_INFO
{
	unsigned int fault : 1;
	unsigned int find_drv : 1;
	unsigned int read : 1;
	unsigned int error : 1;
	unsigned int tested : 1;
	char name[10];
} BIB_FIND;

typedef struct tagGTSTEP
{
	char name[60];
	int step_no;
} GETSTEP;

typedef List<GETSTEP, GETSTEP> StepList;

typedef struct tagSTEP
{
	char name[60];
	int step_no;
	char tcs[32];
	float mpat_time;
	float ps[5];
	float vih[8];
	float vil[7];
	float vth;
} STEPINFO;

typedef struct tagVOLT
{
	int ch;
	float voltage;
	float delay;
	float high;
	float low;
	float c_limit;
} VOLTINFO;

//FAULT BIN
typedef struct tagFAULTBIN
{
        bool            occur;  
        unsigned int    step;   
        unsigned int    bin;    
        bool            slot[32];
} TFAULTBIN;

typedef struct tagSENDMONITORING
{
	char MESSAGE[10];
	char COMMENT[250];
} TSENDMONITORING;



#define FAIL_BLOCK_LIMIT	1920

// Original
/*
typedef unsigned short FailBlock[FAIL_BLOCK_LIMIT + 3];	// FAIL_BLOCK_LIMIT     = fail count
							// FAIL_BLOCK_LIMIT + 1 = add count
							// FAIL_BLOCK_LIMIT + 2 = mcp top
*/
// 4CE -> 6CE
typedef unsigned short FailBlock[FAIL_BLOCK_LIMIT + 6];	// FAIL_BLOCK_LIMIT     = fail count
							// FAIL_BLOCK_LIMIT + 1 = add count
							// FAIL_BLOCK_LIMIT + 2 = mcp top
/*
typedef short Judge[200];
*/
typedef char Judge[200];

#define BC_BLOCKDATA_OLD	-1

enum BC_COMMAND
{
	BC_GETSTEP = 0xff00ff00,
	BC_GETBIB = 0xff00ff01,
	BC_DEVICEID = 0xff00ff02,
	BC_INITDATA = 0xff00ff03,
	BC_DUT_PF_DATA_CUM = 0xff00ff04,
	BC_DUT_PF_DATA_STEP = 0xff00ff05,
	BC_STEPINFO = 0xff00ff06,
	BC_STEPSTART = 0xff00ff07,
	BC_STEPEND = 0xff00ff08,
	BC_SETTEMP = 0xff00ff09,
	BC_GETTEMP = 0xff00ff0a,
	BC_TESTEND = 0xff00ff0b,
	BC_INITEND = 0xff00ff0c,
	BC_FAILLIMIT = 0xff00ff0d,
	BC_SETDEVICE = 0xff00ff0e,
	BC_BIBFIND = 0xff00ff0f,
	BC_WD_FAULT = 0xff00ff10,
	BC_BLOCK_PF_DATA_CUM = 0xff00ff11,
	BC_BLOCK_PF_DATA_STEP = 0xff00ff12,
	BC_VERSION = 0xff00ff13,
	BC_SETFAILMODE = 0xff00ff14,
	BC_ADDSIZE = 0xff00ff15,
	BC_SETJUDGEMODE = 0xff00ff16,
	BC_GET_FMDATA = 0xff00ff17,
	BC_GET_BLOCK_DATA = 0xff00ff18,
	BC_SYNC = 0xff00ff19,
	BC_INDEX_SI_MAP = 0xff00ff1a,
	BC_GET_DUT_PF_DATA_CUM = 0xff00ff1b,
	BC_GET_BLOCK_PF_DATA_CUM = 0xff00ff1c,
	BC_GET_CHIP_PF_DATA_CUM = 0xff00ff1d,
	BC_DRAM_FBLOCK = 0xff00ff1e,
	BC_TESTRESULT = 0xff00ff1f,
	BC_ECCDATA = 0xff00ff20,
	BC_BITIME = 0xff00ff22,
	BC_NETMON = 0xff00ff23,
	BC_SETFAULT = 0xff00ff24,
	BC_KILLED = 0xff00ff25,
	BC_DISPDATA = 0xff00ff26,
	BC_DISPLAY_END = 0xff00ff27,
	BC_DISPLAY_IO = 0xff00ff28,
	BC_SETVOLT = 0xff00ff29,
	BC_ECC = 0xff00ff2a,
	BC_SCAN_SET = 0xff00ff2b,
	BC_PRINTF = 0xff00ff2c,
	BC_PRINTF2 = 0xff00ff2d,
	BC_DEVICEFAULT = 0xff00ff2e,
	BC_SETMEMORY = 0xff00ff2f,
	BC_GET_ITEMYIELD = 0xff00ff30,
	BC_SAVE_HICHIP = 0xff00ff31,
	BC_RUNTIMEERROR = 0xff00ff32,
	BC_COMPILEERROR = 0xff00ff33,
	BC_TESTSTART = 0xff00ff34,
	BC_SETTEMP2 = 0xff00ff35,
	BC_SENDCAL = 0xff00ff36,
	BC_SETDDP = 0xff00ff37,
	BC_DRV_FAULT = 0xff00ff38,
	BC_STOPMODE = 0xff00ff39,
	BC_CHIP_PF_DATA_CUM = 0xff00ff3a,
	BC_CHIP_PF_DATA_STEP = 0xff00ff3b,
	BC_INIT_PF_DATA = 0xff00ff3c,
	BC_STEP_TYPE = 0xff00ff3d,
	BC_DQDATA = 0xff00ff3f,
	BC_STOP = 0xff00ff40,
	BC_SET_FLS = 0xff00ff41,
	BC_BISTDATA = 0xff00ff42,
	BC_BISTDATA2 = 0xff00ff43,
	BC_CHAMBER_MODE = 0xff00ff45,
	BC_FCATE_DATA = 0xff00ff46,
	BC_PCATE_DATA = 0xff00ff47,
	BC_BIN_DATA = 0xff00ff48,
	BC_GET_FCATE_DATA = 0xff00ff49,
	BC_BIST_ERROR_NO = 0xff00ff4a,
	BC_GET_PCATE_DATA = 0xff00ff4b,
	BC_GET_BIN_DATA = 0xff00ff4c,
	BC_CAT_LIMIT = 0xff00ff4d,
	BC_BIST_EXCEPTION_CATE = 0xff00ff4e,
	BC_CAT_ACTION_CATE = 0xff00ff4f,
	BC_BISTDATA3 = 0xff00ff50,
	BC_CAT_REV = 0xff00ff51,
	BC_FW_REV = 0xff00ff52,
	BC_C_VENDOR = 0xff00ff53,
	BC_MPAT_TIME_LIMIT_OVER = 0xff00ff54,
	BC_DEV_OS_TYPE = 0xff00ff55,
	BC_LIB_TYPE = 0xff00ff56,
	BC_CHIPID_DATA = 0xff00ff57,
	BC_SETMASK = 0xff00ff58,
	BC_SETEXCLMASK = 0xff00ff59,
	BC_GET_BIB_ID_TO_SLOT = 0xff00ff5a,
	BC_CALL_CHIPID_MEM = 0xff00ff5b,
	BC_GET_PART_NAME = 0xff00ff5c,
	BC_GET_LOT_NO = 0xff00ff5d,
	BC_GET_PGM_NAME = 0xff00ff5e,
	BC_GET_SYSTEM_NAME = 0xff00ff5f,
	BC_GET_START_TIME = 0xff00ff60,
	BC_GET_END_TIME = 0xff00ff61,
	BC_PRINT_CHIPID = 0xff00ff62,
	BC_GET_IS_MASK = 0xff00ff63,
	BC_GET_BIB_LOT_TO_SLOT = 0xff00ff64,
	BC_SENDFTP = 0xff00ff65,
	BC_RDCHIP_START = 0xff00ff66,
	BC_RDCHIP_END = 0xff00ff67,
	BC_SET_AGING_TIME = 0xff00ff68,
	BC_SI_INDEX_MAP = 0xff00ff69,
	BC_EXEC = 0xff00ff6a,
	BC_SCAN_VECTOR = 0xff00ff6b,
	BC_FM_DATA = 0xff00ff6d,
	BC_GET_LOT_NAME = 0xff00ff6e,
	BC_GET_CUMFAIL_COUNT = 0xff00ff71,
	BC_GET_CUMFAIL_YIELD = 0xff00ff72,
	BC_GET_NBYTE_MATCH = 0xff00ff73,
	BC_CHIPID = 0xff00ff80,
	BC_WAFERID = 0xff00ff81,
	BC_XPOS = 0xff00ff82,
	BC_YPOS = 0xff00ff83,
	BC_CHIPID_DRAM = 0xff00ff84,
	BC_WAFERID_DRAM = 0xff00ff85,
	BC_XPOS_DRAM = 0xff00ff86,
	BC_YPOS_DRAM = 0xff00ff87,
	BC_EMMC_CHIPID_DATA = 0xff00ff88,
	BC_DRAM_CHIPID_DATA = 0xff00ff89,
	BC_PREMAP_CNT = 0xff00ff8a,
	BC_PREMAP_DATA = 0xff00ff8b,
	BC_GET_LOTYIELD = 0xff00ff8c,
	BC_GET_CUMLOTYIELD = 0xff00ff8d,
	BC_DIAGSYNC = 0xff00ff8e,
	BC_SET_LOTFAIL = 0xff00ff8f,
	BC_SET_BOARD_DISABLE = 0xff00ff90,
	BC_SET_BOARD_ENABLE = 0xff00ff91,
	BC_WAFERPDASET = 0xff00ff95,
	BC_SETBIN_REF1 = 0xff00ff96,
	BC_SETBIN_REF2 = 0xff00ff97,
	BC_SETBIN_REF3 = 0xff00ff98,
	BC_TIME_INTERLOCK = 0xff00ff99,
	BC_TIME_INTERLOCK_OFF = 0xff00ff9a,
	BC_MULTIBIN_SET = 0xff00ff9b,
	BC_MULTIBIN_SET1 = 0xff00ff9c,
	BC_SHMOO_ENDINFO = 0xff00ff9d,
	BC_SHMOO_TIMESET_CNT = 0xff00ff9e,
	BC_SHMOO_TIMESET_LIST = 0xff00ff9f,
	BC_SETFAULTBIN = 0xff00ffa0,
	BC_SETSTEPFAULTBIN = 0xff00ffa1,
	BC_SETPSCOMMON = 0xff00ffa2,
	BC_BD_INTERLOCK = 0xff00ffa3,
	BC_SETRATE = 0xff00ffb2,
	BC_INITTEMP = 0xff00ffc1,
	BC_AMBIENT_START = 0xff00ffc2,
	BC_TESTTIME = 0xff00ffc5,
	BC_TESTSTEPTIME	= 0xff00ffc6,
	BC_TESTTIME_STATE = 0xff00ffc7,
	BC_SET_SOAKTIME	= 0xff00ffc9,
	BC_SETTEMP_AMBIENT = 0xff00ffca,
	BC_SETDOOR = 0xff00ffcb,
	BC_CMBSTOP = 0xff00ffcc,
	BC_GET_OWNER = 0xff00ffcf,

	BC_TIME_INTERLOCK_MAX1 = 0xff00ffd0,
	BC_TIME_INTERLOCK_MAX2 = 0xff00ffd1,
	BC_TIME_INTERLOCK_MAX3 = 0xff00ffd2,
	BC_TIME_INTERLOCK_MAX4 = 0xff00ffd3,
	BC_TIME_INTERLOCK_MIN1 = 0xff00ffd4,
	BC_TIME_INTERLOCK_MIN2 = 0xff00ffd5,
	BC_TIME_INTERLOCK_MIN3 = 0xff00ffd6,
	BC_TIME_INTERLOCK_MIN4 = 0xff00ffd7,

	BC_SETTEMPINFO = 0xff00ffd9,
	BC_USERALARM = 0xff00ffda,
	BC_USERSYSTEMDOWN = 0xff00ffdb,
	BC_USERWAITTEST = 0xff00ffdc,
	BC_PAUSE = 0xff00ffdd,
	BC_SENDMONITORING = 0xff00ffde,

   
	//BC_EXEC = 0xff00ffc0,


//	BC_SETFAILLIMIT_SEP = 0xff00ff04,	// FAIL LIMIT 을 STEP 별로 지정
//	BC_BLOCKDATA = 0xff00ff05,
//	BC_JUDGE = 0xff00ff11,
//	BC_DECISION = 0xff00ff12,
//	BC_CLEAR_FMDATA = 0xff00ff16,
//	BC_GET_JUDGE = 0xff00ff18,
//	BC_64_4 = 0xff00ff1b,
//	BC_CLEAN_BLOCK = 0xff00ff1c,
//	BC_TESTDATA = 0xff00ff1d,
//	BC_DEVICELOAD = 0xff00ff1f,
//	BC_SETFAILBIN = 0xff00ff21,
//	BC_GET_FMDATA2 = 0xff00ff2d,
//	BC_SETFAILBIN_SEP = 0xff00ff3a,		// LINE FAIL 을 STEP 별로 지정
//	BC_REMOVEFAILBIN_SEP = 0xff00ff3b,	// LINE FAIL 을 STEP 별로 지정
//	BC_SEND_CATE = 0xff00ff3c,
//	BC_SEND_SORT = 0xff00ff3d,
//	BC_GET_BIN = 0xff00ff3e,
//	BC_COMPARE_CUM = 0xff00ff44,
//	BC_SETTEMP_SYNC = 0xff00ff46,
//	BC_LINEFAIL_DISABLE = 0xff00ff47,
//	BC_SETGROUPFAILBIN = 0xff00ff48,
//	BC_GR_LINEFAIL_DISABLE = 0xff00ff49,
//	BC_FAILBLK_ADDR_SCAN = 0xff00ff4b,
//	BC_FAILBLK_ADDR_COMP = 0xff00ff4c,
//	BC_JUDGE_BCAU = 0xff00ff6c,
//	BC_INITFAIL = 0xff00ff6f,
//	BC_ADDFAIL = 0xff00ff70,
};

#endif
