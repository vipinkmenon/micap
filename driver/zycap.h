/*
 * icap_ctrl.h
 *
 *  Created on: Jan 22, 2014
 *  Author: Vipin K
 */

#include "xparameters.h"
#include <stdio.h>
#include "xil_printf.h"
#include "xil_io.h"
#include <stdlib.h>


typedef struct {
	u32 BaseAddress;	 /**< Base address of registers */
} ZyCap;

#define CTRL_REG 0x0
#define STAT_REG 0x4
#define ADDR_REG 0x18
#define LEN_REG  0x28

#define WR_MASK  0x00000001

ZyCap * Init_Zycap(u32 Zycap_Base_Addr);
int Config_PR_Bitstream(ZyCap *zycap, int addr, int len, int sync_intr);
int Sync_Zycap(ZyCap *zycap);
