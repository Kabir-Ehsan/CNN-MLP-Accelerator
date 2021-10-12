#include <stdio.h>
//#include <iostream>
//#include <fstream>

#include "platform.h"
#include <stdlib.h>  // Standard C functions, e.g. exit()
#include <stdbool.h> // Provides a Boolean data type for ANSI/ISO-C
#include "xparameters.h" // Parameter definitions for processor peripherals
#include "xcontroller.h"   // Device driver for HLS HW block
//#include "xaxidma.h" // Device driver API for AXI DMA
#include "xil_types.h"

#include "xtmrctr.h"
///#include "AxiTimerHelper.h"

#include "xstatus.h"
#include "xtmrctr_l.h"
#include "xil_io.h"

#include "inputsLSTM2.h"
#include "allParam2.h"
#include "labelLSTM2.h"

#include "xil_printf.h"
//#include "xaxidma_hw.h"
#include "xil_cache.h"
#include "math.h"

//using namespace std;

#define INPUTS 970
#define INPUT_SIZE 1//1000
#define timeSteps 1

#define hiddenUnitsLayer1 20 ///30
#define hiddenUnitsLayer2 30

#define maxHidden 60
#define MaxW_input1 80
#define MaxW_recur1 1600

#define biasSize1 81
#define gateInWsize 20
#define gateReWsize 400

#define LEN 3000


XController Controller;

//XController_Config_r configData;



// timer
XTmrCtr timer_dev;
///AxiTimerHelper axiTimer;

//dma
//XAxiDma axiDma;

// Global variable definitions - used by ISR
volatile static int RunController = 0;
volatile static int ResultAvailController = 0;

// Setup and helper functions
int setup_interrupt();
int Controller_init(XController *ControllerPtr);
void Controller_start(void *InstancePtr);

int TmrCtrLowLevelExample(u32 TmrCtrBaseAddress, u8 TmrCtrNumber);
//// The ISR prototype
void Controller_isr(void *InstancePtr);



int main()
{
	//init_platform();

	print("\nProgram Begins\n\r");

	print("*************************************************************\n\r");
	print("************    Neural Network Processor on FPGA    *****************\n\r");
	print("***************   University of Arkansas   ************************\n\n\r");
	unsigned long int i, j, k, l, p, r, P;
    int status;

	unsigned int init_time, curr_time, calibration;
	unsigned int begin_time, begin_time1, begin_time2, begin_time3, begin_time4;
	unsigned int end_time, end_time1, end_time2, end_time3, end_time4;

	unsigned int run_time_sw = 0;
	unsigned int run_time_hw1 = 0;
	unsigned int run_time_hw2 = 0;
	unsigned int run_time_hw3 = 0;
	unsigned int run_time_hw4 = 0;

	//double run_time_hw4;
	//double run_time_sw;


	//MaxW=((CONVOUT)* HIDDEN1)+((HIDDEN1)* HIDDEN2)+((HIDDEN2)* HIDDEN3)+((HIDDEN3)* HIDDEN4)+ ((last_layer)* OUTPUTS);

	float *inputStream1 = (float*)XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR;
	float *inputStream2 = (float*)XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR;

	float *inputWeight = (float*)XPAR_AXI_BRAM_CTRL_4_S_AXI_BASEADDR;
	float *forgetWeight = (float*)XPAR_AXI_BRAM_CTRL_8_S_AXI_BASEADDR;
	float *cellWeight = (float*)XPAR_AXI_BRAM_CTRL_9_S_AXI_BASEADDR;
	float *outputWeight = (float*)XPAR_AXI_BRAM_CTRL_10_S_AXI_BASEADDR;

	float *inputReWeight1 = (float*)XPAR_AXI_BRAM_CTRL_11_S_AXI_BASEADDR;
	float *inputReWeight2 = (float*)XPAR_AXI_BRAM_CTRL_18_S_AXI_BASEADDR;

	float *forgetReWeight1 = (float*)XPAR_AXI_BRAM_CTRL_13_S_AXI_BASEADDR;
	float *forgetReWeight2 = (float*)XPAR_AXI_BRAM_CTRL_19_S_AXI_BASEADDR;

	float *cellReWeight1 = (float*)XPAR_AXI_BRAM_CTRL_7_S_AXI_BASEADDR;
	float *cellReWeight2 = (float*)XPAR_AXI_BRAM_CTRL_6_S_AXI_BASEADDR;

	float *outputReWeight1 = (float*)XPAR_AXI_BRAM_CTRL_16_S_AXI_BASEADDR;
	float *outputReWeight2 = (float*)XPAR_AXI_BRAM_CTRL_15_S_AXI_BASEADDR;

	float *fcWeights = (float*)XPAR_AXI_BRAM_CTRL_14_S_AXI_BASEADDR;

	float *inputBias = (float*)XPAR_AXI_BRAM_CTRL_20_S_AXI_BASEADDR;
	float *forgetBias = (float*)XPAR_AXI_BRAM_CTRL_12_S_AXI_BASEADDR;
	float *cellBias = (float*)XPAR_AXI_BRAM_CTRL_5_S_AXI_BASEADDR;
	float *outputBias = (float*)XPAR_AXI_BRAM_CTRL_21_S_AXI_BASEADDR;

	float *fcBias = (float*)XPAR_AXI_BRAM_CTRL_17_S_AXI_BASEADDR;

	float *outputStream1 = (float*)XPAR_AXI_BRAM_CTRL_3_S_AXI_BASEADDR;
	float *outputStream2 = (float*)XPAR_AXI_BRAM_CTRL_2_S_AXI_BASEADDR;


//	unsigned int indma_size1 = (INPUT_SIZE+total) * sizeof(float);
//	unsigned int indma_size2 = (INPUT_SIZE+2*hiddenUnitsLayer1) * sizeof(float);
//	unsigned int outdma_size = (INPUT_SIZE+2*hiddenUnitsLayer1) * sizeof(float);

//	static float din[LEN];
//	static float din2[INPUTS];
	//static float din2[LEN];
//	volatile static float dout[INPUTS];
//	volatile static float dout2[INPUTS];

	int MIW1 = 4*timeSteps*hiddenUnitsLayer1;
	printf("\nMaxW_input1: %d\n", MIW1);
		//cout<<"\nMaxW_input1:"<< MIW1<<'\n';

	int MRW1 = 4*hiddenUnitsLayer1*hiddenUnitsLayer1;
	printf("\nMaxW_recur1: %d\n", MRW1);
		//cout<<"\nMaxW_recur1:"<< MRW1<<'\n';

	int sizeBias = 4*hiddenUnitsLayer1+1;
	int fcWsize = hiddenUnitsLayer1;

	int total = MIW1+MRW1+sizeBias+fcWsize;
	printf("\ntotalParameter: %d\n", total);
		//cout<<"\ntotalParameter:" << total << '\n' ;





	  status = TmrCtrLowLevelExample(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

	  if (status != XST_SUCCESS)
	  {
		  print("\rFailed to initialize timer\r\n");
		  //return XST_FAILURE;
	  }

	  // Calibrate timer
	  init_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

	  curr_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

	  calibration = curr_time - init_time;

	  xil_printf("\n\rCalibration report:\r\n");
	  xil_printf("\n\rinit_time: %d cycles.\r\n", init_time);
	  xil_printf("\n\rcurr_time: %d cycles.\r\n", curr_time);
	  xil_printf("\n\rcalibration: %d cycles.\r\n", calibration);

	  // Loop measurement
	  begin_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

	  //axiTimer.startTimer();

	  for (i = 0; i<1000; i++);

	  //axiTimer.stopTimer();

	  //double run_time_sw = axiTimer.getElapsedTimerInSeconds();

	  end_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

	  run_time_sw = end_time - begin_time - calibration;

	  // printf("\n\rLoop time for 1000 iterations is %f seconds.\r\n", run_time_sw);

	  xil_printf("\n\rLoop time for 1000 iterations is %d cycles.\r\n", run_time_sw);

	 //-----------------------------------------------------------------------

	    status = Controller_init(&Controller);

		if(status != XST_SUCCESS)
		{
			print("Error: example setup failed\n");
			//return XST_FAILURE;
		}
		//XController_DisableAutoRestart(&Controller);

	 for(P=0;P<3;P++)
	 {
		//f1=fopen("result.dat","a");
		if(P>0)
		{
			   print("\n\n\r Start from second sample \n\n\r");
			   XController_Set_new_net(&Controller, 0);

//			   Xil_DCacheInvalidateRange((UINTPTR)dout2, outdma_size);
//			   Xil_DCacheFlushRange((UINTPTR)dout2, outdma_size);
//			   Xil_DCacheFlushRange((UINTPTR)din2, indma_size2);

//			   print("\n\r Cache cleared successfully\n\r");


			   inputStream1[0]= input_arr[P];
			   inputStream2[0]= input_arr[P];

			for(i=0;i<2*hiddenUnitsLayer1;i++)
			{

				inputStream1[i+1]= outputStream1[i+1];
				inputStream2[i+1]= outputStream2[i+1];

				printf("\ninputStream1[%ld] = %f\n", i, inputStream1[i]);
				printf("\ninputStream2[%ld] = %f\n", i, inputStream2[i]);
				//cout <<"inputStream1["<<i<<"] =" << inputStream1[i] << '\n';
				//cout <<"inputStream2["<<i<<"] =" << inputStream2[i] << '\n\n';

			}

			print("\n\n\r ACCELERATOR STARTS \n\r");

			begin_time4 =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
			//axiTimer.startTimer();
			XController_Start(&Controller);

			while(!(XController_IsDone(&Controller) && XController_IsIdle(&Controller)));
//			while(!XController_IsDone(&Controller))
//			{
//				printf("output while not done: %f\n", outputStream1[0]);
//			}
//			while(!outputStream1[0]);

			end_time4 = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
			run_time_hw4 = end_time4 - begin_time4 - calibration;

			//axiTimer.stopTimer();
			//double run_time_hw4 = axiTimer.getElapsedTimerInSeconds();

			print("\n\r ACCELERATOR ENDS \n\n\r");

			printf("\nSample: %ld\n\n", P);

			//xil_printf("\nHW_Cycles4= %d     \n\r", run_time_hw4);
			printf("\nHW_time(us)= %f      \n\r", (run_time_hw4*10.0)/1000);

			printf("\nOUTPUT[0]= %f,  LABEL = %f\n\n\r", outputStream1[0],  labels[P]);
			//cout<< "\nOUTPUT["<<0<<"]= " << outputStream1[0] << ", " << "LABEL: " << labels[P] << "\n\r";

		}
		else
		{

			 inputStream1[0]= input_arr[P];
			 inputStream2[0]= input_arr[P];

			for(j=0;j<gateInWsize;j++)
			{

				inputWeight[j] = param[j];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<gateInWsize;j++)
			{

				forgetWeight[j] = param[j+gateInWsize];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<gateInWsize;j++)
			{

				cellWeight[j] = param[j+gateInWsize*2];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}

			for(j=0;j<gateInWsize;j++)
			{

				outputWeight[j] = param[j+gateInWsize*3];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<gateReWsize;j++)
			{

				inputReWeight1[j] = param[j+gateInWsize*4];
				inputReWeight2[j] = param[j+gateInWsize*4];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<gateReWsize;j++)
			{

				forgetReWeight1[j] = param[j+gateInWsize*4+gateReWsize];
				forgetReWeight2[j] = param[j+gateInWsize*4+gateReWsize];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<gateReWsize;j++)
			{

				cellReWeight1[j] = param[j+gateInWsize*4+gateReWsize*2];
				cellReWeight2[j] = param[j+gateInWsize*4+gateReWsize*2];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<gateReWsize;j++)
			{

				outputReWeight1[j] = param[j+gateInWsize*4+gateReWsize*3];
				outputReWeight2[j] = param[j+gateInWsize*4+gateReWsize*3];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}

			for(j=0;j<hiddenUnitsLayer1;j++)
			{

				fcWeights[j] = param[j+gateInWsize*4+gateReWsize*4];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}

			for(j=0;j<hiddenUnitsLayer1;j++)
			{

				inputBias[j] = param[j+gateInWsize*4+gateReWsize*4+hiddenUnitsLayer1];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<hiddenUnitsLayer1;j++)
			{

				forgetBias[j] = param[j+gateInWsize*4+gateReWsize*4+hiddenUnitsLayer1*2];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<hiddenUnitsLayer1;j++)
			{

				cellBias[j] = param[j+gateInWsize*4+gateReWsize*4+hiddenUnitsLayer1*3];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<hiddenUnitsLayer1;j++)
			{

				outputBias[j] = param[j+gateInWsize*4+gateReWsize*4+hiddenUnitsLayer1*4];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			fcBias[0] = param[gateInWsize*4+gateReWsize*4+hiddenUnitsLayer1*5];

			 XController_Set_new_net(&Controller, 1);

			 if (XController_IsReady(&Controller))
				  print("\n HLS peripheral is ready.  Starting...\n\r ");
			 else
			 {
				  print("!!! HLS peripheral is not ready! Exiting...\n\r");
				  exit(-1);
			 }

			 print("\n\n\r Start from First sample \n\n\r");
			//flush the cache
//			 Xil_DCacheInvalidateRange((u32)dout, outdma_size);
//			 Xil_DCacheInvalidateRange((u32)din, outdma_size);
//			 Xil_DCacheFlushRange((UINTPTR)din, indma_size);
//			 Xil_DCacheFlushRange((UINTPTR)dout, outdma_size);

			 //Xil_DCacheFlushRange((unsigned int)res_hw,dma_size);
//			 print("\n\r Cache cleared successfully\n\r");
			 //----------------------------------------------------------------------------

			 //************************************************* HW************************************************************************
			 ////Controller_start(&Controller);
			///Controller_isr(&Controller);
			 print("\n\r ACCELERATOR STARTS \n\r");

			 begin_time3 =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);

			 //axiTimer.startTimer();
			 XController_Start(&Controller);
			 while(!(XController_IsDone(&Controller) && XController_IsIdle(&Controller)));
//			 while(!XController_IsDone(&Controller))
//			 {
//				printf("output while not done: %f\n", outputStream1[0]);
//			 }
//			 while(!outputStream1[0]);
			 //axiTimer.stopTimer();
			 //double run_time_hw4 = axiTimer.getElapsedTimerInSeconds();

			 end_time3 = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

			 run_time_hw3 = end_time3 - begin_time3 - calibration;

			 print("\n\r ACCELERATOR ENDS \n\n\r");


			 printf("\nSample: %ld\n", P);

			 //xil_printf("\nHW_Cycles4= %d     \n\r", run_time_hw4);
			 printf("\nHW_time(us)= %f      \n\r", (run_time_hw3*10.0)/1000);


			 printf("\nOUTPUT[0]= %f,  LABEL = %f\n\r", outputStream1[0],  labels[P]);
			 //cout<< "\nOUTPUT["<<0<<"]= " << outputStream1[0] << ", " << "LABEL: " << labels[P] << "\n\r";


			 for(k=0;k<2*hiddenUnitsLayer1+1;k++)
			 {
				 printf("\nOUTPUT1[%ld]= %f,  OUTPUT2[%ld]= %f,\n\r", k, outputStream1[k],  k, outputStream2[k]);
				//cout<< "OUTPUT1["<<k<<"]= " << outputStream1[k] << "\n\r";
				//cout<< "OUTPUT2["<<k<<"]= " << outputStream2[k] << "\n\n\r";

			 }

			 print("\n\rFinished Process For 1st Sample \r\n");


		}


	 }

			 //************************************************* HW************************************************************************
			 ////Controller_start(&Controller);
			///Controller_isr(&Controller);

//###################################################################################
		   //fflush(stdout);
		   /*		   */
		   //cleanup_platform();

    return 0;
}



int Controller_init(XController *ControllerPtr)
{
  // XController_Config *cfgPtr;
	XController_Config cconfig;
	cconfig.DeviceId = 0;
	cconfig.Bus_bundle_BaseAddress = XPAR_CONTROLLER_0_S_AXI_BUS_BUNDLE_BASEADDR;
    int status;



   status = XController_CfgInitialize(ControllerPtr, &cconfig);
   if (status != XST_SUCCESS) {
      printf("ERROR: Could not initialize accelerator.\n\r");
      return XST_FAILURE;
   }
   else
	   printf("accelerator initialized correctly.\n\r");
   return status;
}

//-----------------------------------------------------


//--------------------------------------------------------
void Controller_start(void *InstancePtr)
{
   XController *pAccelerator = (XController *)InstancePtr;
   XController_InterruptEnable(pAccelerator,1);
   XController_InterruptGlobalEnable(pAccelerator);
   XController_Start(pAccelerator);
};

void Controller_isr(void *InstancePtr)
{
   XController *pAccelerator = (XController *)InstancePtr;

   //Disable the global interrupt
   XController_InterruptGlobalDisable(pAccelerator);
   //Disable the local interrupt
   XController_InterruptDisable(pAccelerator,0xffffffff);

   // clear the local interrupt
   XController_InterruptClear(pAccelerator,1);

   ResultAvailController = 1;
   // restart the core if it should run again
   if(RunController)
   {
      Controller_start(pAccelerator);
   }
};

// A function that wraps all AXI DMA initialization related API calls
//int init_dma(XAxiDma *axiDmaPtr)

/*
void init_dma()
{
	printf("initiated AXI DMA config\n\r");
		//XAxiDma *axiDma;
		XAxiDma_Config *CfgPtr;
		int status;

   // Get pointer to DMA configuration
   CfgPtr = XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);

   if(!CfgPtr)
   {
      printf("Error looking for AXI DMA config\n\r");
      //return XST_FAILURE;
   }
   // Initialize the DMA handle
   status = XAxiDma_CfgInitialize(&axiDma, CfgPtr);

   if(status != XST_SUCCESS)
   {
      printf("Error initializing DMA\n\r");
      //return XST_FAILURE;
   }
   else
	   printf("DMA Initiation Done\n\r");
   /*  Did not enabled Scatter Gather
   //check for scatter gather mode - this example must have simple mode only
   if(XAxiDma_HasSg(axiDmaPtr))
   {
      printf("Error DMA configured in SG mode\n\r");
      return XST_FAILURE;
   }*/
   //disable the interrupts
   //XAxiDma_IntrDisable(&axiDma, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
   //XAxiDma_IntrDisable(&axiDma, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);


   //XAxiDma_IntrEnable(&axiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

   //XAxiDma_IntrEnable(&axiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

   //return XST_SUCCESS;
//};

/*
// A function that wraps all AXI DMA initialization related API calls
int init_dma(XAxiDma *axiDmaPtr)
{
	 //printf("z init dma  AXI DMA config\n\r");
   XAxiDma_Config *CfgPtr;
   int status;

   // Get pointer to DMA configuration
   CfgPtr = XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);

   if(!CfgPtr)
   {
      printf("Error looking for AXI DMA config\n\r");
      return XST_FAILURE;
   }
   // Initialize the DMA handle
   status = XAxiDma_CfgInitialize(axiDmaPtr,CfgPtr);
   if(status != XST_SUCCESS)
   {
      printf("Error initializing DMA\n\r");
      return XST_FAILURE;
   }

   //check for scatter gather mode - this example must have simple mode only
   if(XAxiDma_HasSg(axiDmaPtr))
   {
       printf("Error DMA configured in SG mode\n\r");
       return XST_FAILURE;
   }

   //disable the interrupts
  XAxiDma_IntrDisable(axiDmaPtr, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
  XAxiDma_IntrDisable(axiDmaPtr, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);


////   XAxiDma_IntrEnable(axiDmaPtr, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

////   XAxiDma_IntrEnable(axiDmaPtr, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

   return XST_SUCCESS;
};*/


//-------------------------------------------------------------------------
int TmrCtrLowLevelExample(u32 TmrCtrBaseAddress, u8 TmrCtrNumber)
{
	//printf("Initializing timer\n\r");
	u32 ControlStatus;
	/*
	* Clear the Control Status Register
	*/
	XTmrCtr_SetControlStatusReg(TmrCtrBaseAddress, TmrCtrNumber, 0x0);
	/*
	* Set the value that is loaded into the timer counter and cause it to
	* be loaded into the timer counter
	*/
	XTmrCtr_SetLoadReg(TmrCtrBaseAddress, TmrCtrNumber, 0x0);
	XTmrCtr_LoadTimerCounterReg(TmrCtrBaseAddress, TmrCtrNumber);
	/*
	* Clear the Load Timer bit in the Control Status Register
	*/
	ControlStatus = XTmrCtr_GetControlStatusReg(TmrCtrBaseAddress, TmrCtrNumber);
	ControlStatus = ControlStatus | XTC_CSR_AUTO_RELOAD_MASK;
	ControlStatus = ControlStatus & (~XTC_CSR_LOAD_MASK);
	XTmrCtr_SetControlStatusReg(TmrCtrBaseAddress, TmrCtrNumber, ControlStatus);
	/*
	* Get a snapshot of the timer counter value before it's started
	* to compare against later
	*/
	//Value1 = XTmrCtr_GetTimerCounterReg(TmrCtrBaseAddress, TmrCtrNumber);
	/*
	* Start the timer counter such that it's incrementing by default
	*/
	XTmrCtr_Enable(TmrCtrBaseAddress, TmrCtrNumber);
	/*
	* Read the value of the timer counter and wait for it to change,
	* since it's incrementing it should change, if the hardware is not
	* working for some reason, this loop could be infinite such that the
	* * function does not return
	*/
	/*
	* Disable the timer counter such that it stops incrementing
	*/
	//XTmrCtr_Disable(TmrCtrBaseAddress, TmrCtrNumber);
	return XST_SUCCESS;
};

