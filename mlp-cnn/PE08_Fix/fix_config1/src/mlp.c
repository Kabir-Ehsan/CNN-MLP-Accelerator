#include <stdio.h>
#include "platform.h"
#include <stdlib.h>  // Standard C functions, e.g. exit()
#include <stdbool.h> // Provides a Boolean data type for ANSI/ISO-C
#include "xparameters.h" // Parameter definitions for processor peripherals
#include "xcontroller.h"   // Device driver for HLS HW block
#include "xaxidma.h" // Device driver API for AXI DMA
#include "xil_types.h"
#include "xtmrctr.h"
#include "xstatus.h"
#include "xtmrctr_l.h"
#include "xil_io.h"

#include "inputs4.h"
//#include "weights_mnist3.h"
//#include "kernel3.h"

#include "xil_printf.h"
#include "xaxidma_hw.h"
#include "xil_cache.h"
#include "math.h"

#define NLAYERS 2//3
#define INPUTS 784
#define convOut 243//441//300  //441;//300;
#define HIDDEN1 64//40//128
#define HIDDEN2 0//64
#define HIDDEN3 0
#define HIDDEN4  0
#define HIDDEN5  0
#define OUTPUTS 10

#define imageSize1 28
#define imageSize2 26//21//26
#define imageSize3 11
#define imageSize4 0
#define imageSize5 0
#define imageSize6 0



#define KernelSize1 3
#define KernelSize2 6//3
#define KernelSize3 3
#define KernelSize4 0
#define KernelSize5 0
#define KernelSize6 0

#define channels1 1
#define channels2 3//1
#define channels3 8
#define channels4 0
#define channels5 0
#define channels6 0

#define stride1 1
#define stride2 2
#define stride3 1
#define stride4 0
#define stride5 0
#define stride6 0


#define KSIZE1 9//64
#define KSIZE2 36//9
#define KSIZE3 9//0
#define KSIZE4 0
#define KSIZE5 0
#define KSIZE6 0

#define convOut1D1 26//21
#define convOut1D2 11//10
#define convOut1D3 9
#define convOut1D4 0
#define convOut1D5 0
#define convOut1D6 0

#define convLayer 3
#define KsizeTotal 1107//64//91

#define filter1 3
#define filter2 8//3
#define filter3 3
#define filter4 0
#define filter5 0
#define filter6 0

#define filterTotal1 27//64
#define filterTotal2 864//27
#define filterTotal3 216//0
#define filterTotal4 0
#define filterTotal5 0
#define filterTotal6 0

#define LEN  65536

#define FIXED_BIT 12

#define Fixed_Point_Frac_Bits 14;

typedef int16_t fixed_point_t;

//#define LEN  307

float fixed_to_float(fixed_point_t input);

fixed_point_t float_to_fixed(float input);

int get_label(volatile int16_t *Output, int n);

XController Controller;

//XController_Config_r configData;



// timer
XTmrCtr timer_dev;

//dma
XAxiDma axiDma;

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

int init_dma(XAxiDma *axiDmaPtr);

//void init_dma();

int main()
{

	print("\nProgram Begins\n\r");

	print("*************************************************************\n\r");
	print("************Neural Network Processor on FPGA*****************\n\r");
	print("***************University of Arkansas************************\n\r");
	unsigned long int i, j, k, p, r;
	int status;

/*	configData.word_0 = NLAYERS;
	configData.word_1 = INPUTS;
	configData.word_2 = OUTPUTS;
	configData.word_3 = HIDDEN1;
	configData.word_4 = HIDDEN2;
	configData.word_5 = HIDDEN3;
	configData.word_6 = HIDDEN4;*/
	//configData.word_7 =-1;



	//XAxiDma *axiDma;


	unsigned int init_time, curr_time, calibration;
	unsigned int begin_time, begin_time1, begin_time2, begin_time3, begin_time4;
	unsigned int end_time, end_time1, end_time2, end_time3, end_time4;
	unsigned int run_time_sw = 0;
	unsigned int run_time_hw1 = 0;
	unsigned int run_time_hw2 = 0;
	unsigned int run_time_hw3 = 0;
	unsigned int run_time_hw4 = 0;

	float correct = 0;

	int last_layer;
	 //------------------------------------------------------------------------------------
	if(NLAYERS==2)
		last_layer=	HIDDEN1;
	else if(NLAYERS==3)
		last_layer=	HIDDEN2;
	else if(NLAYERS==4)
		last_layer=	HIDDEN3;
	else if(NLAYERS==5)
			last_layer=HIDDEN4;
	//****************************Weights**************************************************

	unsigned long int MaxW=0;
	//MaxW=((INPUTS)* HIDDEN1)+((HIDDEN1)* HIDDEN2)+((HIDDEN2)* HIDDEN3)+((HIDDEN3)* HIDDEN4)+ ((last_layer)* OUTPUTS); // no of weights

	MaxW=((convOut)* HIDDEN1)+((HIDDEN1)* HIDDEN2)+((HIDDEN2)* HIDDEN3)+((HIDDEN3)* HIDDEN4)+ ((last_layer)* OUTPUTS);

	xil_printf("MaxW=%d\n\r",MaxW);

	static int16_t din[LEN];
	static int16_t din2[INPUTS];
	//static float din2[LEN];
	volatile static int16_t dout[OUTPUTS];
	volatile static int16_t dout2[OUTPUTS];

	/*static u32 din[LEN];
	static u32 din2[INPUTS];
	volatile static u32 dout[OUTPUTS];
	volatile static u32 dout2[OUTPUTS];*/

	for(i=0;i<INPUTS;i++)
	{
	   din[i]= 0;//input_arr[i];
	 // printf("din[%d]= %f  \n\r", i, din[i]);

    }

	for(i=0;i<MaxW;i++)
	{
	   din[i+INPUTS]=0;//weights[i];
	}

	for(i=0;i<KsizeTotal;i++)
	{
		din[i+INPUTS+MaxW]=0;//Kernels[i];
	}
	printf("din[%d]= %f  \n\r", i, din[i+INPUTS+MaxW-1]);

	 unsigned int indma_size = (MaxW+INPUTS+KsizeTotal) * sizeof(int16_t);

//	 unsigned int indma_size = (MaxW+INPUTS) * sizeof(float);
	 unsigned int indma_size2 = (INPUTS) * sizeof(int16_t);
	 unsigned int outdma_size = OUTPUTS * sizeof(int16_t);

	/* unsigned int indma_size = (MaxW+INPUTS) * sizeof(u32);
	 unsigned int indma_size2 = (INPUTS) * sizeof(u32);
	 unsigned int outdma_size = OUTPUTS * sizeof(u32);*/


	  init_platform();

	  status = TmrCtrLowLevelExample(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);
	  if (status != XST_SUCCESS)
	  {
		  print("\rFailed to initialize timer\r\n");
		  //return XST_FAILURE;
	  }

	  status = init_dma(&axiDma);
	  if (status != XST_SUCCESS)
	  {
	  	  exit(-1);
	  }
	  print("\rDMA Init done\n\r");

	  // Calibrate timer
	  init_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

	  curr_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

	  calibration = curr_time - init_time;

	  xil_printf("\rCalibration report:\r\n");
	  xil_printf("\rinit_time: %d cycles.\r\n", init_time);
	  xil_printf("\rcurr_time: %d cycles.\r\n", curr_time);
	  xil_printf("\rcalibration: %d cycles.\r\n", calibration);

	  // Loop measurement
	  begin_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

	  for (i = 0; i< 1000; i++);

	  end_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

	  run_time_sw = end_time - begin_time - calibration;

	  xil_printf("\rLoop time for 1000 iterations is %d cycles.\r\n", run_time_sw);

	 //-----------------------------------------------------------------------


	    status = Controller_init(&Controller);

		if(status != XST_SUCCESS)
		{
			print("Error: example setup failed\n");
			//return XST_FAILURE;
		}



			////XController_Set_Config_thresholdv(&Controller, -1);

			//-----------------------------------



			XController_Set_Config_Ninputs_V(&Controller, INPUTS);
			XController_Set_Config_Noutputs_V(&Controller, OUTPUTS);
			XController_Set_Config_NnuronsH1_V(&Controller, HIDDEN1);
			XController_Set_Config_NnuronsH2_V(&Controller, HIDDEN2);
			XController_Set_Config_NnuronsH3_V(&Controller, HIDDEN3);
			XController_Set_Config_NnuronsH4_V(&Controller, HIDDEN4);
			XController_Set_Config_convOut_V(&Controller, convOut);
			XController_Set_Config_filter1_V(&Controller, filter1);
			XController_Set_Config_filter2_V(&Controller, filter2);
			XController_Set_Config_filter3_V(&Controller, filter3);
			XController_Set_Config_filter4_V(&Controller, filter4);
			XController_Set_Config_filter5_V(&Controller, filter5);
			XController_Set_Config_filter6_V(&Controller, filter6);
			XController_Set_Config_Nlayers_V(&Controller, NLAYERS);
			XController_Set_Config_imageSize1_V(&Controller,imageSize1);
			XController_Set_Config_imageSize2_V(&Controller, imageSize2);
			XController_Set_Config_imageSize3_V(&Controller, imageSize3);
			XController_Set_Config_imageSize4_V(&Controller, imageSize4);
			XController_Set_Config_imageSize5_V(&Controller, imageSize5);
			XController_Set_Config_imageSize6_V(&Controller, imageSize6);
			XController_Set_Config_KernelSize1_V(&Controller, KernelSize1);
			XController_Set_Config_KernelSize2_V(&Controller, KernelSize2);
			XController_Set_Config_KernelSize3_V(&Controller, KernelSize3);
			XController_Set_Config_KernelSize4_V(&Controller, KernelSize4);
			XController_Set_Config_KernelSize5_V(&Controller, KernelSize5);
			XController_Set_Config_KernelSize6_V(&Controller, KernelSize6);
			XController_Set_Config_channels1_V(&Controller, channels1);
			XController_Set_Config_channels2_V(&Controller, channels2);
			XController_Set_Config_channels3_V(&Controller, channels3);
			XController_Set_Config_channels4_V(&Controller, channels4);
			XController_Set_Config_channels5_V(&Controller, channels5);
			XController_Set_Config_channels6_V(&Controller, channels6);
			XController_Set_Config_stride1_V(&Controller, stride1);
			XController_Set_Config_stride2_V(&Controller, stride2);
			XController_Set_Config_stride3_V(&Controller, stride3);
			XController_Set_Config_stride4_V(&Controller, stride4);
			XController_Set_Config_stride5_V(&Controller, stride5);
			XController_Set_Config_stride6_V(&Controller, stride6);
			XController_Set_Config_convOut1D1_V(&Controller, convOut1D1);
			XController_Set_Config_convOut1D2_V(&Controller, convOut1D2);
			XController_Set_Config_convOut1D3_V(&Controller, convOut1D3);
			XController_Set_Config_convOut1D4_V(&Controller, convOut1D4);
			XController_Set_Config_convOut1D5_V(&Controller, convOut1D5);
			XController_Set_Config_convOut1D6_V(&Controller, convOut1D6);
			XController_Set_Config_KSIZE1_V(&Controller, KSIZE1);
			XController_Set_Config_KSIZE2_V(&Controller, KSIZE2);
			XController_Set_Config_KSIZE3_V(&Controller, KSIZE3);
			XController_Set_Config_KSIZE4_V(&Controller, KSIZE4);
			XController_Set_Config_KSIZE5_V(&Controller, KSIZE5);
			XController_Set_Config_KSIZE6_V(&Controller, KSIZE6);
			XController_Set_Config_convLayer_V(&Controller, convLayer);
			XController_Set_Config_KsizeTotal(&Controller, KsizeTotal);
			XController_Set_Config_filterTotal1(&Controller, filterTotal1);
			XController_Set_Config_filterTotal2(&Controller, filterTotal2);
			XController_Set_Config_filterTotal3(&Controller, filterTotal3);
			XController_Set_Config_filterTotal4(&Controller, filterTotal4);
			XController_Set_Config_filterTotal5(&Controller, filterTotal5);
			XController_Set_Config_filterTotal6(&Controller, filterTotal6);
			XController_Set_new_net(&Controller, 1);
			XController_Set_conv(&Controller, 1);


			////XController_Set_Config_r(&Controller, configData);


			 if (XController_IsReady(&Controller))
				  print("\n HLS peripheral is ready.  Starting...\n\r ");
			 else
			 {
				  print("!!! HLS peripheral is not ready! Exiting...\n\r");
				  exit(-1);
			 }

			//flush the cache
			// Xil_DCacheInvalidateRange((u32)dout, outdma_size);
			// Xil_DCacheInvalidateRange((u32)din, outdma_size);
			 Xil_DCacheFlushRange((UINTPTR)din, indma_size);
			 Xil_DCacheFlushRange((UINTPTR)dout, outdma_size);

			 //Xil_DCacheFlushRange((unsigned int)res_hw,dma_size);
			 print("\r Cache cleared successfully\n\r");
			 //----------------------------------------------------------------------------

			 //************************************************* HW************************************************************************
			 ////Controller_start(&Controller);
			///Controller_isr(&Controller);

			XController_Start(&Controller);

			print("\r mlp started \n\r");
				//begin_time3 =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

				/*XController_Set_new_net(&Controller, 1);
				//flush the cache
				Xil_DCacheInvalidateRange((u32)dout, outdma_size);
				Xil_DCacheFlushRange((u32)din, indma_size);
				Xil_DCacheFlushRange((u32)dout, outdma_size);

				print("\rCache cleared successfully\n\r");

				XController_Start(&Controller);
				print("\r mlp started \n\r");*/




		begin_time4 =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);

		//status =
		XAxiDma_SimpleTransfer(&axiDma, (UINTPTR)din, indma_size, XAXIDMA_DMA_TO_DEVICE);

//		if (status != XST_SUCCESS) {
		//print("Error: DMA transfer to Vivado HLS block failed\n");
//		return XST_FAILURE;
//		}

//		while (XAxiDma_Busy(&axiDma,  XAXIDMA_DMA_TO_DEVICE)) ;
		//begin_time =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
		//get results from the Vivado HLS block
//		status =

		XAxiDma_SimpleTransfer(&axiDma, (UINTPTR) dout,outdma_size, XAXIDMA_DEVICE_TO_DMA);

//		if (status != XST_SUCCESS) {
		//print("Error: DMA transfer from Vivado HLS block failed\n");
//		return XST_FAILURE;
//		}
		/* Wait for transfer to be done */
		while (XAxiDma_Busy(&axiDma,  XAXIDMA_DEVICE_TO_DMA)) ;

		end_time4 = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);

		run_time_hw4 = end_time4 - begin_time4 - calibration;

		for(k=0;k<OUTPUTS;k++)
			printf("HW_out[%d]= %d  \n\r", k, dout[k]);

		Xil_DCacheInvalidateRange((UINTPTR)dout, outdma_size);


		for(j=0;j<10;j++)
		{
			for(i=0;i<INPUTS;i++)
			{      //inputs
					//commented//
				  //// din2[i]=float_to_fixed(0.1);//InputPatterens[row][i];
				   //din[i]=InputPatterens[row][i];
					din2[i]=0;//input_arr[i+784*j];
			}
		/*	for(i=0;i<MaxW;i++)
			{
				  //commented//
				   din2[i+INPUTS]=0.2;//weight_Ann[i];
				   //din[i+INPUTS]=weight_Ann[i];
			}*/
				   xil_printf("---------- Pattren[%d] ---------\n\r", j);
				   xil_printf("                 **********                    \n\r");
				   xil_printf("Number of Layers =    %d\n\r",NLAYERS);
				   xil_printf("Number of Inputs =    %d\n\r",INPUTS);
				   xil_printf("Number of Outputs =    %d\n\r",OUTPUTS);
				   xil_printf("Number of Hidden1=    %d\n\r",HIDDEN1);
				   xil_printf("Number of Hidden2=    %d\n\r",HIDDEN2);
				   xil_printf("Number of Hidden3=    %d\n\r",HIDDEN3);
				   xil_printf("------------------------------------------\n\r");

				   xil_printf("HW_Cycles4= %d     \n\r", run_time_hw4);
				   printf("HW_time4(us)= %f     \n\r", (run_time_hw4*10.0)/1000);

				////

		/*		   status = Controller_init(&Controller);

					if(status != XST_SUCCESS)
					{
						print("Error: example setup failed\n");
						//return XST_FAILURE;
					}*/

				   XController_Set_new_net(&Controller, 0);
				  // XController_Set_conv(&Controller, 1);

				   Xil_DCacheInvalidateRange((UINTPTR)dout2, outdma_size);
				   Xil_DCacheFlushRange((UINTPTR)dout2, outdma_size);
				   Xil_DCacheFlushRange((UINTPTR)din2, indma_size2);

		//		   print("\r Cache cleared successfully\n\r");

				   XController_Start(&Controller);

			//	   print("\r mlp started \n\r");



			    begin_time4 =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);

///				status =
			    XAxiDma_SimpleTransfer(&axiDma, (UINTPTR)din2, indma_size2, XAXIDMA_DMA_TO_DEVICE);

		//		print("No Error\n");

//				if (status != XST_SUCCESS)
//				{
//					print("Error: DMA transfer to Vivado HLS block failed\n");
//					return XST_FAILURE;
//				}

//				while (XAxiDma_Busy(&axiDma,  XAXIDMA_DMA_TO_DEVICE)) ;
				//begin_time =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
				//get results from the Vivado HLS block
//				status =

				XAxiDma_SimpleTransfer(&axiDma, (UINTPTR) dout2, outdma_size, XAXIDMA_DEVICE_TO_DMA);

//				if (status != XST_SUCCESS)
//				{
//					print("Error: DMA transfer from Vivado HLS block failed\n");
//					return XST_FAILURE;
//				}

		//		print("\rNo Error\n");
				/* Wait for transfer to be done */
				while (XAxiDma_Busy(&axiDma,  XAXIDMA_DEVICE_TO_DMA)) ;

				//print("\rNo Error\n");
				end_time4 = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

				run_time_hw4 = end_time4 - begin_time4 - calibration;

				for(k=0;k<OUTPUTS;k++)
					printf("HW_out[%d]= %d  \n\r", k, dout2[k]);


				  /* xil_printf("HW_Cycles1= %d     \n\r", run_time_hw1);
				   printf("HW_time1(us)= %f      \n\r", (run_time_hw1*10.0)/1000);

				   xil_printf("HW_Cycles2= %d     \n\r", run_time_hw2);
				   printf("HW_time2(us)= %f      \n\r", (run_time_hw2*10.0)/1000);

				   xil_printf("HW_Cycles3= %d     \n\r", run_time_hw3);
				   printf("HW_time3(us)= %f      \n\r", (run_time_hw3*10.0)/1000);*/
				if (get_label(dout2,10)==labels[j])
				{
					printf(" Matched \n");
					correct=correct+1;
				}
				else
					printf(" UnMatched \n");

		//##################################################################################
				   //Xil_DCacheInvalidateRange((u32)dout, outdma_size);
				   //Xil_DCacheInvalidateRange((u32)din, outdma_size);
				   //Xil_DCacheFlushRange((u32)din, indma_size);
				   //Xil_DCacheFlushRange((u32)dout, outdma_size);
			}
		    printf(" Accuracy Final: %f\n", correct);

/*		  // r=0;
		   for(j=0;j<10;j++)
		   {
			   xil_printf("---------- Pattren[%d] ---------\n\r", j);
			   for(i=0;i<INPUTS;i++)      //inputs
						//commented//
				   din2[i]=0.1;//InputPatterens[p][i];
				   //din2[i]=InputPatterens[p][i];
			   for(i=0;i<MaxW;i++)
				   din2[i+INPUTS]=.2;
			   /*p=0; k=0;
			   for(;;)
			   {
			   	 	for(i=0;i<INPUTS/4;i++)
			   	 	{
			   	 		din2[k]=InputPatterens[r][i];
			   	 		k++;
			   	 	}
			   	 	p++;
			   	 	r++;
			   	 	if(p>3)
			   	 	{
			   	 		break;
			   	 	}
			   }

			   XController_Set_new_net(&Controller,1);
			   Xil_DCacheInvalidateRange((u32)dout2,outdma_size);
			   //Xil_DCacheInvalidateRange((u32)din2,outdma_size);
			   Xil_DCacheFlushRange((u32)dout2,outdma_size);
			  // Xil_DCacheFlushRange((u32)din2,indma_size2);
			   Xil_DCacheFlushRange((u32)din2,indma_size);

				XController_Start(&Controller);

				begin_time =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, XPAR_TMRCTR_0_DEVICE_ID);

				//status = XAxiDma_SimpleTransfer(&axiDma, (u32)din2, indma_size2, XAXIDMA_DMA_TO_DEVICE);
				status = XAxiDma_SimpleTransfer(&axiDma, (u32)din2, indma_size, XAXIDMA_DMA_TO_DEVICE);

				status = XAxiDma_SimpleTransfer(&axiDma, (u32) dout2, outdma_size, XAXIDMA_DEVICE_TO_DMA);

				if (status != XST_SUCCESS)
				{
					print("Error: DMA transfer to Vivado HLS block failed\n\r");
					return XST_FAILURE;
				}
				/*end_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
				run_time_hw = end_time - begin_time - calibration;
				xil_printf("HW_Cycles-01    =%d     \n\r",run_time_hw);
				printf("HW_time(us)-01  =%f      \n\r",(run_time_hw *10.0)/1000);*/


				//test code
				//begin_time =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
				//while(!XController_IsDone(&Controller));
				/*end_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
				run_time_hw = end_time - begin_time - calibration;
				xil_printf("HW_Cycles-02    =%d     \n\r",run_time_hw);
				printf("HW_time(us)-02  =%f      \n\r",(run_time_hw *10.0)/1000);*/
				//test code

				//while (XAxiDma_Busy(&axiDma,  XAXIDMA_DMA_TO_DEVICE)) ;
				//begin_time =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
				//get results from the Vivado HLS block
				/*status = XAxiDma_SimpleTransfer(&axiDma, (u32) dout2, outdma_size, XAXIDMA_DEVICE_TO_DMA);

				if (status != XST_SUCCESS)
				{
					print("Error: DMA transfer from Vivado HLS block failed\n\r");
					//return XST_FAILURE;
				}

				//end_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
				//run_time_hw = end_time - begin_time - calibration;
				//xil_printf("HW_Cycles    =%d     \n\r",run_time_hw);
				//printf("HW_time(us)  =%f      \n\r",(run_time_hw *10.0)/1000);


				//begin_time =XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
				// Wait for transfer to be done

				//while (XAxiDma_Busy(&axiDma,  XAXIDMA_DMA_TO_DEVICE));
				while (XAxiDma_Busy(&axiDma,  XAXIDMA_DEVICE_TO_DMA)) ;

				end_time = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR,XPAR_TMRCTR_0_DEVICE_ID);
				run_time_hw = end_time - begin_time - calibration;

				//--------

				for(k=0;k<OUTPUTS;k++)
					  printf("HW2_out[%d]= %f   \n\r", k, dout2[k]);


				xil_printf("HW_Cycles= %d     \n\r",run_time_hw);
				printf("HW_time(us)= %f      \n\r",(run_time_hw *10.0)/1000);

		   }*/

//###################################################################################
		   fflush(stdout);
		   cleanup_platform();
    return 0;
}



int Controller_init(XController *ControllerPtr)
{
  // XController_Config *cfgPtr;
	XController_Config cconfig;
	cconfig.DeviceId = 0;
	cconfig.Bus_bundle_configur_BaseAddress = XPAR_XCONTROLLER_0_S_AXI_BUS_BUNDLE_CONFIGUR_BASEADDR;
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
};
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

/*inline float fixed_to_float(fixed_point_t input)
{
	float a = 1<<Fixed_Point_Frac_Bits;
	return (float)input/a;
};

inline fixed_point_t float_to_fixed(float input)
{
	float b = 1 << Fixed_Point_Frac_Bits;
	return (fixed_point_t)(round(input*b));
};*/

// float 2 fix
//#include <stdio.h>

//#include <math.h>
//#define FIXED_BIT 12

unsigned short int float2fix(float n)
{
    unsigned short int int_part = 0, frac_part = 0;
    int i;
    float t;

    //int_part = (int)floor(n) << FIXED_BIT;
    //n -= (int)floor(n);

    if (n > 0)
    {
        int_part = ((int)floor(n)) << FIXED_BIT;
    }
    else
    {
        int_part = ((int)ceil(n)) << FIXED_BIT; // need to round towards 0
    }
      // now that the whole part has been translated and set in 2s compliment, dont need to worry about sign
      n = fabs(n) - floor(fabs(n));

    t = 0.5;
    for (i = 0; i < FIXED_BIT; i++) {
        if ((n - t) >= 0) {
            n -= t;
            frac_part += (1 << (FIXED_BIT - 1 - i));
        }
        t = t /2;
    }

    return int_part + frac_part;
};

/*int main()
{
    float n;
    n = 2.5; // 0d10240
    printf("%f => %d\n", n, float2fix(n));
    n = 2.625; // 0d10752
    printf("%f => %d\n", n, float2fix(n));
    n = 0.375; // 0d1536
    printf("%f => %d\n", n, float2fix(n));
    return 0;
}*/

//#include <stdio.h>
//#include <math.h>



float fix2float(unsigned short int n)
{
    float ret = 0.0;
    int integer_part;
    int i;

    integer_part = (n >> 12);
    ret += integer_part;
    for (i = 0; i < FIXED_BIT; i++) {
        ret += ((n >> (FIXED_BIT - i - 1)) & (0x01)) * (1.0 / pow(2, i+1));
    }

    return ret;
};

int get_label(volatile int16_t *Output, int n)
{
	if(n<=0) return -1;
	int k=0, max_k=0;
	int16_t max = Output[0];
	//int max = abs((int)Output[0]);
	for (k=1;k<n;++k)
	{
		if(Output[k]>max)
		{
			max = Output[k];
			max_k = k;
		}
	}
	return max_k;

};
