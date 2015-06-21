/*  Author   :   Vipin.K
 *  Project  :   Zycap
 *  Dcpr.    :   Example application for the custom ICAP controller
 */


#include "xparameters.h"
#include "xil_io.h"
#include "xstatus.h"
#include "xtmrctr.h"
#include "zycap.h"
#include "xintc.h"
#include "xil_exception.h"


#define PARTIAL_BITFILE1_LEN 0x6A1C0   /*Partial bitstream size. Not needed by the ICAP, used only to calculate the performance*/
#define PARTIAL_BITFILE2_LEN 0x30108   /*Partial bitstream size. Not needed by the ICAP, used only to calculate the performance*/
#define XPAR_REG_PERIPHERAL_0_BASEADDR 0xFF00


int main()
{
	int Status;
	u32 delay;
	ZyCap *my_zycap;
	XTmrCtr TimerCounterInst;        //The instance of the Timer Counter. Used to measure the performance of the ICAP controller
	// Initialize timer counter
	Status = XTmrCtr_Initialize(&TimerCounterInst, XPAR_AXI_TIMER_0_DEVICE_ID);
	if (Status != XST_SUCCESS){
		xil_printf("Timer initialisation failed\r\n",Status);
		return XST_FAILURE;
	}
	XTmrCtr_SetOptions(&TimerCounterInst, 0, XTC_AUTO_RELOAD_OPTION);

	print("Starting Reconfiguration\n\r");
	//Initialise the ICAP controller
	my_zycap = Init_Zycap(XPAR_ZYCAP_0_BASEADDR);
	print("Zycap Initialisation done..\n\r");
	//Reset the Timer and start it
	XTmrCtr_Reset(&TimerCounterInst, 0);
	XTmrCtr_Start(&TimerCounterInst, 0);
	//Send config2 partial bitstream to the ICAP with reset sync bit set

	Status = Config_PR_Bitstream(my_zycap,0x90000000,PARTIAL_BITFILE2_LEN,1);
	if (Status != XST_SUCCESS){
		xil_printf("Reconfiguration failed\r\n",Status);
		return XST_FAILURE;
	}
	//Read the content of the timer and check the performance
	delay = XTmrCtr_GetValue(&TimerCounterInst, 0);
	xil_printf("Reconfiguration speed: %d MBytes/sec\r\n", PARTIAL_BITFILE2_LEN*100/delay);

	//rtn = Xil_In32(XPAR_AXI_DMA_0_BASEADDR);
	//xil_printf("Control Reg %0x\n\r",rtn);
	//rtn = Xil_In32(XPAR_AXI_DMA_0_BASEADDR+0x4);
	//xil_printf("Status Reg %0x\n\r",rtn);
	//print("Hellow\n\r");
	return 0;
}
