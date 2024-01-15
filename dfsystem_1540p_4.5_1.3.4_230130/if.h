
#ifndef _IF_H
#define _IF_H

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/timex.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
//#include "../common.h"

const unsigned int MAX_NSLOT = 12;

class CIF
{
public:
	CIF();
	~CIF();

	int dev_open(char *, unsigned char, unsigned char);
	unsigned long wait_int(int, unsigned long, unsigned long, unsigned long, int);
	unsigned long Read(unsigned long add);
	unsigned char ReadByte(unsigned long add);
	void Write(unsigned long add, unsigned long data);
	unsigned long long Delay(unsigned long long delay);
	void delay_1us();
	unsigned long long Counter();

protected:
	unsigned int NRACK;
	unsigned int NSLOT;
	int m_fd;
};

#endif
