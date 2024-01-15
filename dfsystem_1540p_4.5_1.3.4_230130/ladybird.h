
#ifndef _LINUX_FOX_H
#define _LINUX_FOX_H

#define INT_GPIO7	7
#define INT_GPIO7_IRQ	IRQ_GPIO(INT_GPIO7)

#define FOX_MAJOR	240
#define DEV_NAME	"DI Corp"
#define DEV_VERSION	DEV_NAME" v1.0"
#define DEV_FILE	"/dev/fox"

#define MAGIC_DEV		's'
#define IOCTL_SET_PORT		_IOW(MAGIC_DEV, 20, set_port_t)
#define IOCTL_OUT_FPGA		_IOW(MAGIC_DEV, 21, fpga_access_t)
#define IOCTL_IN_FPGA		_IOW(MAGIC_DEV, 22, fpga_access_t)
#define IOCTL_OUT_WAIT_INT	_IOW(MAGIC_DEV, 23, fpga_out_wait_t)
#define IOCTL_OUT_FPGA_HIGH	_IOW(MAGIC_DEV, 24, fpga_access_t)
#define IOCTL_OUT_FPGA_LOW	_IOW(MAGIC_DEV, 25, fpga_access_t)
#define IOCTL_IN_FPGA_HIGH	_IOW(MAGIC_DEV, 26, fpga_access_t)
#define IOCTL_IN_FPGA_LOW	_IOW(MAGIC_DEV, 27, fpga_access_t)
#define IOCTL_OUT_FPGA_STREAM	_IOW(MAGIC_DEV, 28, fpga_access_data_t)
/*
typedef union {
	unsigned char   byte;
	unsigned short  word;
	unsigned long   dword;
} IOData;

typedef struct {
	unsigned long   add;
	IOData          data;
} FOXData;

typedef struct {
	unsigned long   add;
	unsigned long   count;
	unsigned long   *data;
} FOXDatas;

// fos
//-----------------------------------------------------
// di
*/
typedef struct
{
	unsigned long	addr_phy;
	unsigned long	size;
	char		bus_wdt;
} set_port_t;

typedef struct
{
	unsigned long	addr;
	unsigned long	data;
} fpga_access_t;

typedef struct
{
	int		wait_msec;
	fpga_access_t	out;
	fpga_access_t	in;

} fpga_out_wait_t;

typedef struct
{
	unsigned long	IOadd;
	void		*vaddr;
	unsigned long	cnt;
} fpga_access_data_t;

typedef struct
{
	unsigned long	vport;
	unsigned long	size;
	char		bs;
	unsigned long	pos;
} dev_port_t;

#endif
