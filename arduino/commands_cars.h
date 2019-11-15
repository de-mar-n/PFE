#pragma once

enum update_dir {
	ZERO_DIR = 0x20,
	INC_DIR = 0x40,
	DEC_DIR = 0x80,
};

#define DO_NOTHING	0
#define FORWARD		1
#define LEFT		2
#define RIGHT		3
#define LEFT_SLOW	4
#define RIGHT_SLOW	5
#define FORWARD_SLOW	6
#define BACK		7
#define BACK_SLOW	8

#define DUMP_VARS 9

#define HIGH_SPEED	0xa
#define SLOW_SPEED	0xb
#define TURN_SPEED	0xc
#define SLOW_TURN_SPEED 0xd
#define BIG_ANGLE	0xe
#define SMALL_ANGLE	0xf
#define BACK_HIGH_SPEED	0x10
#define BACK_SLOW_SPEED	0x11

static inline void update_var_from_command(int *var, unsigned command)
{
	if (command & ZERO_DIR)
		*var = 0;
	else if (command & INC_DIR)
		*var += 10;
	else if (command & DEC_DIR)
		*var -= 10;
}
