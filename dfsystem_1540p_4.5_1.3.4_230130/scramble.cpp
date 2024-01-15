
#include <stdio.h>
#include <string.h>

#include "scramble.h"

int ii;
int _scr_line_index;
int _xy_count;

BitOperator::BitOperator(int b, int size) : bit(b), mask(0)
{
	int i, length = bit * size / 16 + 1;

	data = new unsigned short[length];
	memset(data, 0, sizeof(short) * length);

	for (i = 0; i < bit; i++) {
		mask |= (1 << i);
	}
}

BitOperator::~BitOperator()
{
	delete data;
}

int BitOperator::get(int i)
{
	int d = 0;
	int start = bit * i % 16;
	int index = bit * i / 16;

	d = (data[index] >> start) & mask;

	if (start + bit > 16) {
		int div = 16 - start;
		d |= (data[index + 1] << div) & mask;
	}

	return d;
}

void BitOperator::set(int i, int value)
{
	int start = bit * i % 16;
	int index = bit * i / 16;

	data[index] &= ~(mask << start);
	data[index] |= value << start;

	if (start + bit > 16) {
		int div = 16 - start;
		data[index + 1] &= ~(mask >> div);
		data[index + 1] |= value >> div;
	}
}

ax ax::operator !()
{
	ax tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = !result;
	return tmp;
}

ax ax::operator &&(ax oper)
{
	ax tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result && oper.result;
	return tmp;
}

ax ax::operator ||(ax oper)
{
	ax tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result || oper.result;
	return tmp;
}

ax ax::operator ^(ax oper)
{
	ax tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result ^ oper.result;
	return tmp;
}

ax &ax::operator =(ax &oper)
{
	if (this == &oper) {
		return *this;
	}

	_xy_count++;
	result = oper.result;
	return *this;
}

ax ax::operator &&(ay oper)
{
	ax tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result && oper.result;
	return tmp;
}

ax ax::operator ||(ay oper)
{
	ax tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result || oper.result;
	return tmp;
}

ax ax::operator ^(ay oper)
{
	ax tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result ^ oper.result;
	return tmp;
}

ax &ax::operator =(ay &oper)
{
	_xy_count++;
	result = oper.result;
	return *this;
}

ay ay::operator !()
{
	ay tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = !result;
	return tmp;
}

ay ay::operator &&(ay oper)
{
	ay tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result && oper.result;
	return tmp;
}

ay ay::operator ||(ay oper)
{
	ay tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result || oper.result;
	return tmp;
}

ay ay::operator ^(ay oper)
{
	ay tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result ^ oper.result;
	return tmp;
}

ay &ay::operator =(ay &oper)
{
	if (this == &oper) {
		return *this;
	}

	_xy_count++;
	result = oper.result;
	return *this;
}

ay ay::operator &&(ax oper)
{
	ay tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result && oper.result;
	return tmp;
}

ay ay::operator ||(ax oper)
{
	ay tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result || oper.result;
	return tmp;
}

ay ay::operator ^(ax oper)
{
	ay tmp(0, scr);

	if (ii == 0) {
		scr->set_xy_order(bit, oper.bit);
	}

	if (_xy_count == 0) {
		scr->set_xy_default();
	}

	_xy_count++;
	tmp.result = result ^ oper.result;
	return tmp;
}

ay &ay::operator =(ax &oper)
{
	_xy_count++;
	result = oper.result;
	return *this;
}

add &add::operator &&(add &oper)
{
	if (ii == 0) {
		scr->set_dscr_add33(bit, yaddr, oper.bit, oper.yaddr);

		if (scr && !scr->set_order(bit, mask, yaddr)) {
			return scr->new_add;
		}

		if (oper.scr && !scr->set_order(oper.bit, oper.mask, oper.yaddr)) {
			return scr->new_add;
		}
	}

	scr->new_add.result = (bool) (mask ? (ii & mask) : result) && (bool) (oper.mask ? (ii & oper.mask) : oper.result);
	return scr->new_add;
}

add &add::operator ||(add &oper)
{
	if (ii == 0) {
		scr->set_dscr_add33(bit, yaddr, oper.bit, oper.yaddr);

		if (scr && !scr->set_order(bit, mask, yaddr)) {
			return scr->new_add;
		}

		if (oper.scr && !scr->set_order(oper.bit, oper.mask, oper.yaddr)) {
			return scr->new_add;
		}
	}

	scr->new_add.result = (bool) (mask ? (ii & mask) : result) || (bool) (oper.mask ? (ii & oper.mask) : oper.result);
	return scr->new_add;
}

add &add::operator ^(add &oper)
{
	if (ii == 0) {
		scr->set_dscr_add33(bit, yaddr, oper.bit, oper.yaddr);

		if (scr && !scr->set_order(bit, mask, yaddr)) {
			return scr->new_add;
		}

		if (oper.scr && !scr->set_order(oper.bit, oper.mask, oper.yaddr)) {
			return scr->new_add;
		}
	}

	scr->new_add.result = (bool) (mask ? (ii & mask) : result) ^ (bool) (oper.mask ? (ii & oper.mask) : oper.result);
	return scr->new_add;
}

add &add::operator !()
{
	if (ii == 0) {
		scr->set_dscr_add33(bit, yaddr, bit, yaddr);

		if (scr && !scr->set_order(bit, mask, yaddr)) {
			return scr->new_add;
		}
	}

	scr->new_add.result = !(bool) (mask ? (ii & mask) : result);
	return scr->new_add;
}

void xadd::operator =(int value)
{
	if (ii) {
		return;
	}

	scr->link_set(bit + yaddr * 16, value);
	scr->link_set(bit + yaddr * 24, value, true);
}

void xadd::operator =(xadd addr)
{
	if (ii) {
		return;
	}

	scr->link_set(bit + yaddr * 16, addr.bit + addr.yaddr * 16);
	scr->link_set(bit + yaddr * 24, addr.bit + addr.yaddr * 24, true);
}

void xadd::operator =(yadd addr)
{
	if (ii) {
		return;
	}

	scr->link_set(bit + yaddr * 16, addr.bit + addr.yaddr * 16);
	scr->link_set(bit + yaddr * 24, addr.bit + addr.yaddr * 24, true);
}

void xadd::operator =(ax oper)
{
	if (ii == 0) {
		scr->x_mode |= 1;

		if (_xy_count == 0) {
			scr->set_xy_order(oper.bit, oper.bit);
		}

		scr->set_xscr_add33(bit);
	}

	oper.result = (_xy_count == 0) ? ii & 1 : oper.result;
	scr->xscr[ii] |= (oper.result << bit);
	_xy_count = 0;
	_scr_line_index++;
}

void xadd::operator =(ay oper)
{
	if (ii == 0) {
		scr->x_mode |= 1;

		if (_xy_count == 0) {
			scr->set_xy_order(oper.bit, oper.bit);
		}

		scr->set_xscr_add33(bit);
	}

	oper.result = (_xy_count == 0) ? ii & 1 : oper.result;
	scr->xscr[ii] |= (oper.result << bit);
	_xy_count = 0;
	_scr_line_index++;
}

void yadd::operator =(int value)
{
	if (ii) {
		return;
	}

	scr->link_set(bit + yaddr * 16, value);
	scr->link_set(bit + yaddr * 24, value, true);
}

void yadd::operator =(xadd addr)
{
	if (ii) {
		return;
	}

	scr->link_set(bit + yaddr * 16, addr.bit + (!addr.yaddr) * 16);
	scr->link_set(bit + yaddr * 24, addr.bit + (!addr.yaddr) * 24, true);
}

void yadd::operator =(yadd addr)
{
	if (ii) {
		return;
	}

	scr->link_set(bit + yaddr * 16, addr.bit + (!addr.yaddr) * 16);
	scr->link_set(bit + yaddr * 24, addr.bit + (!addr.yaddr) * 24, true);
}

void yadd::operator =(ax oper)
{
	if (ii == 0) {
		scr->y_mode |= 1;

		if (_xy_count == 0) {
			scr->set_xy_order(oper.bit, oper.bit);
		}

		scr->set_yscr_add33(bit);
	}

	oper.result = (_xy_count == 0) ? ii & 1 : oper.result;
	scr->yscr[ii] |= (oper.result << bit);
	_xy_count = 0;
	_scr_line_index++;
}

void yadd::operator =(ay oper)
{
	if (ii == 0) {
		scr->y_mode |= 1;

		if (_xy_count == 0) {
			scr->set_xy_order(oper.bit, oper.bit);
		}

		scr->set_yscr_add33(bit);
	}

	oper.result = (_xy_count == 0) ? ii & 1 : oper.result;
	scr->yscr[ii] |= (oper.result << bit);
	_xy_count = 0;
	_scr_line_index++;
}

void data::operator =(add &oper)
{
	if (ii == 0) {
		scr->set_dscr_add33(oper.bit, oper.yaddr, oper.bit, oper.yaddr);

		if (scr && !scr->set_order(oper.bit, oper.mask, oper.yaddr)) {
			return;
		}
	}

	scr->dscr[ii] |= ((bool) (oper.mask ? (ii & oper.mask) : oper.result)) << bit;
}

Scramble::Scramble() : xlink(5, 16), xlink33(6, 24), ylink(5, 16), ylink33(6, 24),
		dscr_add(6, 8), x_mode(0), y_mode(0), d_set(0),
		 x0( 0, this),  x1( 1, this),  x2( 2, this),  x3( 3, this),  x4( 4, this),  x5( 5, this),  x6( 6, this),
		 x7( 7, this),  x8( 8, this),  x9( 9, this), x10(10, this), x11(11, this), x12(12, this), x13(13, this),
		x14(14, this), x15(15, this), x16(16, this), x17(17, this), x18(18, this), x19(19, this), x20(20, this),
		x21(21, this), x22(22, this), x23(23, this),
		 y0( 0, this),  y1( 1, this),  y2( 2, this),  y3( 3, this),  y4( 4, this),  y5( 5, this),  y6( 6, this),
		 y7( 7, this),  y8( 8, this),  y9( 9, this), y10(10, this), y11(11, this), y12(12, this), y13(13, this),
		y14(14, this), y15(15, this), y16(16, this), y17(17, this), y18(18, this), y19(19, this), y20(20, this),
		y21(21, this), y22(22, this), y23(23, this),
		 ax0( 0x1, this),  ax1( 0x2, this),  ax2( 0x3, this),  ax3( 0x4, this),  ax4( 0x5, this),  ax5( 0x6, this),  ax6( 0x7, this),  ax7( 0x8, this),
		 ax8( 0x9, this),  ax9( 0xa, this), ax10( 0xb, this), ax11( 0xc, this), ax12( 0xd, this), ax13( 0xe, this), ax14( 0xf, this), ax15(0x10, this),
		ax16(0x11, this), ax17(0x12, this), ax18(0x13, this), ax19(0x14, this), ax20(0x15, this), ax21(0x16, this), ax22(0x17, this), ax23(0x18, this),
		 ay0(0x19, this),  ay1(0x1a, this),  ay2(0x1b, this),  ay3(0x1c, this),  ay4(0x1d, this),  ay5(0x1e, this),  ay6(0x1f, this),  ay7(0x20, this),
		 ay8(0x21, this),  ay9(0x22, this), ay10(0x23, this), ay11(0x24, this), ay12(0x25, this), ay13(0x26, this), ay14(0x27, this), ay15(0x28, this),
		ay16(0x29, this), ay17(0x2a, this), ay18(0x2b, this), ay19(0x2c, this), ay20(0x2d, this), ay21(0x2e, this), ay22(0x2f, this), ay23(0x30, this),
		 d0( 0, this),  d1( 1, this),  d2( 2, this),  d3( 3, this),  d4( 4, this),  d5( 5, this),  d6( 6, this),
	 	 d7( 7, this),  d8( 8, this),  d9( 9, this), d10(10, this), d11(11, this), d12(12, this), d13(13, this),
		d14(14, this), d15(15, this), d16(16, this), d17(17, this),
		new_add(-1, this)
{
	int i;

	for (i = 0; i < 16; i++) {
		xlink.set(i, i);
		ylink.set(i, i);
		xlink33.set(i, i);
		ylink33.set(i, i);
	}

	for (; i < 24; i++) {
		xlink33.set(i, i);
		ylink33.set(i, i);
	}

	comp_time = -1;

	memset(xscr, 0, sizeof(xscr));
	memset(yscr, 0, sizeof(yscr));
	memset(dscr, 0, sizeof(dscr));
	memset(xy_order, 0, sizeof(xy_order));
	memset(xscr_add33, 0, sizeof(xscr_add33));
	memset(yscr_add33, 0, sizeof(yscr_add33));
	memset(dscr_add33, 0, sizeof(dscr_add33));
}

bool Scramble::set_order(int bit, int &mask, bool yaddr)
{
	int i;

	if (bit == -1) {
		return true;
	}

	for (i = 0; i < d_set; i++) {
		if (dscr_add.get(i) == 32 + bit + yaddr * 16) {
			break;
		}
	}

	if (i >= 8) {
		return false;
	}

	mask = (1 << i);

	if (i == d_set) {
		dscr_add.set(i, 32 + bit + yaddr * 16);
		d_set = i + 1;
	}

	return true;
}

void Scramble::link_set(int bit, int value, bool add24)
{
	if (!add24) {
		if (bit > 15) {
			ylink.set(bit - 16, value);
			y_mode |= 2;
		}
		else {
			xlink.set(bit, value);
			x_mode |= 2;
		}
	}
	else {
		if (bit > 23) {
			ylink33.set(bit - 24, value);
			y_mode |= 2;
		}
		else {
			xlink33.set(bit, value);
			x_mode |= 2;
		}
	}
}

void Scramble::set_xy_order(int bit1, int bit2)
{
	unsigned int i;

	for (i = 0; i < 8; i++) {
		if (xy_order[_scr_line_index][i] == bit1 || xy_order[_scr_line_index][i] == 0) {
			break;
		}
	}

	xy_order[_scr_line_index][i] = bit1;

	for (i = 0; i < 8; i++) {
		if (xy_order[_scr_line_index][i] == bit2 || xy_order[_scr_line_index][i] == 0) {
			break;
		}
	}

	xy_order[_scr_line_index][i] = bit2;
}

void Scramble::set_xscr_add33(int index)
{
	unsigned int i;

	if (index > 15) {
		return;
	}

	for (i = 0; i < 8; i++) {
		xscr_add33[index][i] = xy_order[_scr_line_index][i];
	}
}

void Scramble::set_yscr_add33(int index)
{
	unsigned int i;

	if (index > 15) {
		return;
	}

	for (i = 0; i < 8; i++) {
		yscr_add33[index][i] = xy_order[_scr_line_index][i];
	}
}

void Scramble::set_dscr_add33(int index1, bool yaddr1, int index2, bool yaddr2)
{
	unsigned int i, j;

	for (i = 0; index1 >= 0 && i < 8; i++) {
		unsigned char v = (yaddr1 == false ? xscr_add33 : yscr_add33)[index1][i];

		for (j = 0; j < 8; j++) {
			if (dscr_add33[j] == v || dscr_add33[j] == 0) {
				break;
			}
		}

		dscr_add33[j] = v;
	}

	for (i = 0; index2 >= 0 && i < 8; i++) {
		unsigned char v = (yaddr2 == false ? xscr_add33 : yscr_add33)[index2][i];

		for (j = 0; j < 8; j++) {
			if (dscr_add33[j] == v || dscr_add33[j] == 0) {
				break;
			}
		}

		dscr_add33[j] = v;
	}
}

/*
int Scramble::xy_index(int bit)
{
	unsigned int i;
	bool bfind = false;

	for (i = 0; i < 8; i++) {
		if (xy_order[_scr_line_index][i] == bit) {
			bfind = true;
			break;
		}
	}

	return bfind == false ? -1 : (int) i;
}
*/
void Scramble::set_xy_default()
{
	unsigned int i;
	ax *pax = NULL;
	ay *pay = NULL;

	for (i = 0; i < 8; i++) {
		switch (xy_order[_scr_line_index][i]) {
			case 0x1: pax = &ax0; break;
			case 0x2: pax = &ax1; break;
			case 0x3: pax = &ax2; break;
			case 0x4: pax = &ax3; break;
			case 0x5: pax = &ax4; break;
			case 0x6: pax = &ax5; break;
			case 0x7: pax = &ax6; break;
			case 0x8: pax = &ax7; break;
			case 0x9: pax = &ax8; break;
			case 0xa: pax = &ax9; break;
			case 0xb: pax = &ax10; break;
			case 0xc: pax = &ax11; break;
			case 0xd: pax = &ax12; break;
			case 0xe: pax = &ax13; break;
			case 0xf: pax = &ax14; break;
			case 0x10: pax = &ax15; break;
			case 0x11: pax = &ax16; break;
			case 0x12: pax = &ax17; break;
			case 0x13: pax = &ax18; break;
			case 0x14: pax = &ax19; break;
			case 0x15: pax = &ax20; break;
			case 0x16: pax = &ax21; break;
			case 0x17: pax = &ax22; break;
			case 0x18: pax = &ax23; break;
			case 0x19: pay = &ay0; break;
			case 0x1a: pay = &ay1; break;
			case 0x1b: pay = &ay2; break;
			case 0x1c: pay = &ay3; break;
			case 0x1d: pay = &ay4; break;
			case 0x1e: pay = &ay5; break;
			case 0x1f: pay = &ay6; break;
			case 0x20: pay = &ay7; break;
			case 0x21: pay = &ay8; break;
			case 0x22: pay = &ay9; break;
			case 0x23: pay = &ay10; break;
			case 0x24: pay = &ay11; break;
			case 0x25: pay = &ay12; break;
			case 0x26: pay = &ay13; break;
			case 0x27: pay = &ay14; break;
			case 0x28: pay = &ay15; break;
			case 0x29: pay = &ay16; break;
			case 0x2a: pay = &ay17; break;
			case 0x2b: pay = &ay18; break;
			case 0x2c: pay = &ay19; break;
			case 0x2d: pay = &ay20; break;
			case 0x2e: pay = &ay21; break;
			case 0x2f: pay = &ay22; break;
			case 0x30: pay = &ay23; break;
			default: pax = NULL; pay = NULL;
		}

		if (pax) {
			pax->result = (ii >> i) & 1;
		}

		if (pay) {
			pay->result = (ii >> i) & 1;
		}

		pax = NULL;
		pay = NULL;
	}
}
