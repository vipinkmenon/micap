/*
 * icap_ctrl.c
 *
 *  Created on: Jan 22, 2014
 *  Author: Vipin K
 */
#include "zycap.h"
#include <stdlib.h>
#include "xil_cache.h"
#include "xstatus.h"


ZyCap zycap;

ZyCap * Init_Zycap(u32 Zycap_Base_Addr)
{
  zycap.BaseAddress = Zycap_Base_Addr;
  return & zycap;
}


int Config_PR_Bitstream(ZyCap *zycap, int addr, int len, int sync_intr)
{
	int Status;

	Xil_Out32(zycap->BaseAddress+ADDR_REG,addr);  //Write to address register
	Xil_Out32(zycap->BaseAddress+CTRL_REG,WR_MASK);
	Xil_Out32(zycap->BaseAddress+LEN_REG,len);

	if(sync_intr)
	{
		Status = Sync_Zycap(zycap);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}
	}
	return XST_SUCCESS;
}


int Sync_Zycap(ZyCap *zycap)
{
	int Status;
	Status = Xil_In32(zycap->BaseAddress+STAT_REG);
	while(!((Status & 1<<1) || (Status & 1<<5) || (Status & 1<<6))){
		Status = Xil_In32(zycap->BaseAddress+STAT_REG);
	}
	if((Status & 1<<5) || (Status & 1<<6))
	    return XST_FAILURE;
	else
		return XST_SUCCESS;
}
