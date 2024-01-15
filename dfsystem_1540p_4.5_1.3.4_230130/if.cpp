
#include "ladybird.h"
#include "if.h"

CIF::CIF():
	NRACK(4),
	NSLOT(8),
	m_fd(0)
{
	set_port_t set_port;

	if ((m_fd = dev_open(DEV_FILE, FOX_MAJOR, 0)) < 0) {
		return;
	}

	set_port.addr_phy = 0x10000000; // nCS2 start addr
	set_port.size = 0x4000000;	// nCS2
	set_port.bus_wdt = 2 ;
	ioctl(m_fd, IOCTL_SET_PORT, &set_port);
}

CIF::~CIF()
{
	close(m_fd);
}

int CIF::dev_open(char *fname, unsigned char major, unsigned char minor)
{
	int dev;

	if ((dev = open(fname, O_RDWR | O_NDELAY)) < 0) {
		mknod(fname, (S_IRWXU | S_IRWXG | S_IFCHR), (major << 8) | (minor));

		if ((dev = open(fname, O_RDWR | O_NDELAY)) < 0) {
			printf("Device OPEN FAIL %s\n", fname);
			return -1;
		}
	}

	return dev;
}

unsigned long CIF::Read(unsigned long addr)
{
	fpga_access_t fpga;
	fpga.addr = addr << 1;
	fpga.data = 0;
	ioctl(m_fd, IOCTL_IN_FPGA, &fpga);
	return fpga.data;
}

void CIF::Write(unsigned long addr, unsigned long data)
{
	fpga_access_t fpga;
	fpga.addr = addr << 1;
	fpga.data = data;
	ioctl(m_fd, IOCTL_OUT_FPGA, &fpga);
}

void CIF::delay_1us()
{
	volatile int i;

	for (i = 0; i < 15; i++) {
		;
	}
}

unsigned long long CIF::Counter()
{
	struct ntptimeval currTime;
	ntp_gettime(&currTime);
	return (unsigned long long) currTime.time.tv_sec * 1000000 + currTime.time.tv_usec;
}

unsigned long long CIF::Delay(unsigned long long delay)
{
	struct ntptimeval start;
	struct ntptimeval end;
	unsigned long long start_time, end_time;

	ntp_gettime(&start);
	start_time = (unsigned long long) start.time.tv_sec * 1000000 + start.time.tv_usec;

	if (delay == 0) {
		return start_time / 1000;
	}

	do {
		ntp_gettime(&end);
		end_time = (unsigned long long) end.time.tv_sec * 1000000 + end.time.tv_usec;
	} while (end_time - start_time < delay);

	return end_time / 1000;
}

unsigned long CIF::wait_int(int fd, unsigned long out_addr, unsigned long out_data, unsigned long in_addr, int time_out)
{
	fpga_out_wait_t fpga;

	fpga.wait_msec = time_out;
	fpga.out.addr = out_addr;
	fpga.out.data = out_data;
	fpga.in.addr = in_addr;
	fpga.in.data = 0xffffffff;

	if (ioctl(fd, IOCTL_OUT_WAIT_INT, &fpga) < 0) {
		printf(" ...time out\n");
	}

	return fpga.in.data;
}
