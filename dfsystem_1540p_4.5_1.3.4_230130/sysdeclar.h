
#ifndef _SYSDECLAR_H
#define _SYSDECLAR_H

#define A0	0
#define A1	1
#define A2	2
#define A3	3
#define A4	4
#define A5	5
#define A6	6
#define A7	7
#define A8	8
#define A9	9
#define A10	10
#define A11	11
#define A12	12
#define A13	13
#define A14	14
#define A15	15
#define A16	16
#define A17	17
#define A18	18
#define A19	19
#define A20	20
#define A21	21
#define A22	22
#define A23	23
#define A24	24
#define A25	25
#define A26	26
#define A27	27
#define A28	28
#define A29	29
#define A30	30
#define A31	31

#define FIXL	0
#define FIXH	1
#define AX0	2
#define AX1	3
#define AX2	4
#define AX3	5
#define AX4	6
#define AX5	7
#define AX6	8
#define AX7	9
#define AX8	10
#define AX9	11
#define AX10	12
#define AX11	13
#define AX12	14
#define AX13	15
#define AX14	16
#define AX15	17
#define AX16	18
#define AX17	19
#define AX18	20
#define AX19	21
#define AX20	22
#define AX21	23
#define AX22	24
#define AX23	25
#define AY0	26
#define AY1	27
#define AY2	28
#define AY3	29
#define AY4	30
#define AY5	31
#define AY6	32
#define AY7	33
#define AY8	34
#define AY9	35
#define AY10	36
#define AY11	37
#define AY12	38
#define AY13	39
#define AY14	40
#define AY15	41
#define AY16	42
#define AY17	43
#define AY18	44
#define AY19	45
#define AY20	46
#define AY21	47
#define AY22	48
#define AY23	49
#define CLK8	50
#define CLK9	51
#define CLK10	52
#define CLK11	53
#define CLK12	54
#define CLK13	55
#define CLK14	56
#define CLK15	57
#define AZ0	58
#define AZ1	59
#define AZ2	60
#define AZ3	61
#define CLK16	62
#define CLK17	63
#define CLK18	64
#define CLK19	65
#define CLK20	66
#define CLK21	67
#define CLK22	68
#define CLK23	69
#define CLK24	70
#define CLK25	71
#define CLK26	72
#define CLK27	73
#define CLK28	74
#define CLK29	75
#define CLK30	76
#define CLK31	77

#define NA	* 1e-9 // 0.000000001
#define UA	* 1e-6 // 0.000001
#define MA	* 1e-3 // 0.001
#define A	* 1
#define UV	* 1e-6 // 0.000001
#define MV	* 1e-3 // 0.001
#define V	* 1

#define NS	* 1e-9 // 0.000000001
#define US	* 1e-6 // 0.000001
#define MS	* 1e-3 // 0.001
#define S	* 1
#define MIN	* 60
#define HOUR	* 3600

#define H	"0"
#define L	""

#define SRAM	0
#define DRAM	2
#define RAMBUS	6
#define FLASH_DEVICE	8
#define EMMC	24

#define IO_SAVE		1
#define IO_FAIL		0
#define ON	1
#define OFF	0

#define PS1	0
#define PS2	1
#define PS3	2
#define PS4	3
#define PS5	4

#define VIH		0
#define VIH_CLK		1
#define VIH_CLK6	2
#define VIH_CLK7	3
#define VIH_ADD8	4
#define VIH_ADD9	5
#define VIH_SCAN0	6
#define VIH_SCAN1	7

#define VIL_CLK		0
#define VIL_CLK6	1
#define VIL_CLK7	2
#define VIL_ADD8	3
#define VIL_ADD9	4
#define VIL_SCAN0	5
#define VIL_SCAN1	6

//////////// PC_INDEX ///////////
#define PC_OPCODE	0x20000
#define PC_JUMP	0x20200
#define PC_LOOP	0x20400
#define PC_XOPCODE	0x21000
#define PC_XVALUE	0x21200
#define PC_XDIRECT	0x21400
#define PC_YOPCODE	0x22000
#define PC_YVALUE	0x22200
#define PC_YDIRECT	0x22400
#define PC_DVALUE	0x23000
#define PC_DOPCODE	0x23200
#define PC_BOPCODE	0x23400

#define NAND_TYPE_16M	0x73
#define NAND_TYPE_32M	0x75
#define NAND_TYPE_64M	0x76
#define NAND_TYPE_128M	0x79
#define NAND_TYPE_256M	0
#define NAND_TYPE_512M	0
#define NAND_TYPE_1G	0
#define NAND_TYPE_2G	0
#define NAND_TYPE_4G	0
#define NAND_TYPE_8G	0

#define F16M		0x1000000
#define F32M		0x2000000
#define F64M		0x4000000
#define F128M		0x8000000
#define F256M		0x10000000
#define F512M		0x20000000
#define F1G		0x40000000
#define F2G		0x80000000
#define F4G		0x100000000L
#define F8G		0x200000000L

#define BC_SYSTEM_START		0
#define BC_TIME_CHECK		1
#define BC_ITEM_START		2
#define BC_ITEM_END		3
#define BC_MPAT_WORK		4
#define BC_SET_TEMP		5
#define BC_TEMP_WAIT		6
#define BC_TEMP_ERROR		7
#define BC_INIT_END		8
#define BC_TEST_END		9
#define BC_VOLT_SET		10
#define BC_JOB_START		11
#define BC_JOB_END		12
#define BC_RUNTIME_ERROR	13
#define BC_BIB_FUSE		14
#define BC_SET_SCAN		15
#define BC_SET_MODE		16
#define BC_SET_BIN_GROUP	17
#define BC_READ_COMP		18
#define BC_CALL_NAME		19
#define BC_PROC_END		20
#define BC_START_MPAT		21
#define BC_IN_STEP		22
#define BC_SET_IOMASK		23
#define BC_DISABLE_SCAN		24
#define BC_TEST_START		25
#define BC_SYSTEM_OFF		27
#define BC_POWER_FAULT		28
#define BC_USER			100

#endif
