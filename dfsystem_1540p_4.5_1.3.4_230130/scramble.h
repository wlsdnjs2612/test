
#ifndef _SCRAMBLE_H
#define _SCRAMBLE_H

extern int ii;
extern int _scr_line_index;
extern int _xy_count;

class BitOperator
{
public:
	BitOperator(int b = 6, int size = 8);
	~BitOperator();

	void set(int index, int value);
	int get(int index);

	int bit;
	int mask;
	unsigned short *data;
};

class Scramble;
class ay;

class ax
{
public:
	ax(int b, Scramble *s = 0) : bit(b), result(false), scr(s) {}

	ax operator !();
	ax operator &&(ax);
	ax operator ||(ax);
	ax operator ^(ax);
	ax &operator =(ax &);
	ax operator &&(ay);
	ax operator ||(ay);
	ax operator ^(ay);
	ax &operator =(ay &);

	int bit;
	bool result;
	Scramble *scr;
};

class ay
{
public:
	ay(int b, Scramble *s = 0) : bit(b), result(false), scr(s) {}

	ay operator !();
	ay operator &&(ay);
	ay operator ||(ay);
	ay operator ^(ay);
	ay &operator =(ay &);
	ay operator &&(ax);
	ay operator ||(ax);
	ay operator ^(ax);
	ay &operator =(ax &);

	int bit;
	bool result;
	Scramble *scr;
};

class add
{
public:
	add(int b = 0, Scramble *s = 0, bool y = false) : bit(b), mask(0), scr(s), yaddr(y) {}

	add &operator &&(add &);
	add &operator ||(add &);
	add &operator ^(add &);
	add &operator !();

	int bit;
	int mask;
	Scramble *scr;
	bool result;
	bool yaddr;
};

class yadd;

class xadd : public add
{
public:
	xadd(int b, Scramble *s) : add(b, s) {}

	void operator =(int);
	void operator =(xadd);
	void operator =(yadd);
	void operator =(ax);
	void operator =(ay);
};

class yadd : public add
{
public:
	yadd(int b, Scramble *s) : add(b, s, true) {}

	void operator =(int);
	void operator =(xadd);
	void operator =(yadd);
	void operator =(ax);
	void operator =(ay);
};

class data
{
public:
	data(int b, Scramble *s) : bit(b), scr(s) {}

	void operator =(add &);

	int bit;
	Scramble *scr;
};

class Scramble
{
public:
	Scramble();

	unsigned short xscr[0x100];
	unsigned short yscr[0x100];
	unsigned int dscr[0x100];
	unsigned char xy_order[256][8];
	unsigned char xscr_add33[16][8];
	unsigned char yscr_add33[16][8];
	unsigned char dscr_add33[8];

	BitOperator xlink, xlink33;
	BitOperator ylink, ylink33;
	BitOperator dscr_add;

	unsigned short x_mode;
	unsigned short y_mode;
	int d_set;
	int d_set33;
	int comp_time;

	xadd x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23;
	yadd y0, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10, y11, y12, y13, y14, y15, y16, y17, y18, y19, y20, y21, y22, y23;
	ax ax0, ax1, ax2, ax3, ax4, ax5, ax6, ax7, ax8, ax9, ax10, ax11, ax12, ax13, ax14, ax15;
	ax ax16, ax17, ax18, ax19, ax20, ax21, ax22, ax23;
	ay ay0, ay1, ay2, ay3, ay4, ay5, ay6, ay7, ay8, ay9, ay10, ay11, ay12, ay13, ay14, ay15;
	ay ay16, ay17, ay18, ay19, ay20, ay21, ay22, ay23;
	data d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, d17;
	add new_add;

	int compTime() { return comp_time; }
	bool set_order(int bit, int &mask, bool yaddr);
	void link_set(int add, int value, bool adr23 = false);
	void set_xy_order(int bit1, int bit2);
	void set_xscr_add33(int index);
	void set_yscr_add33(int index);
	void set_dscr_add33(int index1, bool yaddr1, int index2, bool yaddr2);
//	int xy_index(int bit);
	void set_xy_default();
};

#define SET_SCRAMBLE(a)	\
			_scr_line_index = 0; \
		} \
	} \
}; \
scr_##a a##_scr; \
void SCRAMBLE_##a(const char *name) { \
	SET_SCR(name, a##_scr);

#define SCRAMBLE(a) \
class scr_##a : public Scramble \
{ \
public: \
	scr_##a() : Scramble() \
	{ \
		for (ii = 0; ii < 0x100; ii++)

#endif
