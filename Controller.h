#include"stdio.h"
#include <iostream>

using namespace std;

#include "hls_math.h"
#include "ap_fixed.h"
#include "math.h"
#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include "assert.h"
#include "ap_int.h"


#define INPUT_SIZE 1//970//1000
#define timeSteps 1
//#define OUTPUT_SIZE 64
#define M_WEIGHT_SIZE 65536 //850000//60000// 300000 //
#define hiddenUnitsLayer1 40 ///20 //
#define hiddenUnitsLayer2 30
#define maxHidden 40
#define MaxW_input1 160 // 80 //
#define MaxW_recur1 6400 //1600 //
#define biasSize1 161 //81 //
#define gateInWsize 40 //20 //
#define gateReWsize 1600 //400 //

//#define MAXNOLAYERS 6// input + hidden + output
//#define MAXConvLAYERS 6
//#define MAXFilter 32
//#define MAXChannel 32
//#define NUM_PE 8

//#define kernelSize 8
//#define kernelNo 1
//#define channels 1

//#define PE_WEIGHT_SIZE 1024
//#define MAX_PE_OUT     256
//#define PE_FIFOSIZE    1024 //power of 2 values

//#define isize 1
//#define hsize 20

//#define shift1 0
//#define shift2 0

//const int ksize = kernelSize*kernelSize;
//typedef ap_fixed<8,2> fp1;
//typedef ap_fixed<8,4> fp2;
//typedef ap_fixed<16,6> fp3;
/*typedef ap_fixed<8,4, AP_RND, AP_SAT> fp1;
typedef ap_fixed<8,4, AP_RND, AP_SAT> fp2;
typedef ap_fixed<12,8, AP_TRN, AP_SAT> fp5;//12,8
typedef ap_fixed<12,8, AP_RND, AP_SAT> fp6;//16,12*/

/*typedef ap_fixed<12,6, AP_RND, AP_SAT> fp1;
typedef ap_fixed<12,6, AP_RND, AP_SAT> fp2;
//typedef ap_fixed<12,8, AP_RND, AP_SAT> fp3;
//typedef ap_fixed<12,8, AP_TRN, AP_SAT> fp5;
typedef ap_fixed<16,8, AP_RND, AP_SAT> fp6;*/

/*typedef ap_fixed<10,6, AP_RND, AP_SAT> fp1;
typedef ap_fixed<10,6, AP_RND, AP_SAT> fp2;
//typedef ap_fixed<12,8, AP_RND, AP_SAT> fp3;
//typedef ap_fixed<12,8, AP_TRN, AP_SAT> fp5;
typedef ap_fixed<16,8, AP_RND, AP_SAT> fp6;*/

typedef ap_fixed<8,8, AP_RND, AP_SAT>convert;
/*typedef ap_fixed<8,4> fp2;
typedef ap_fixed<12,8> fp3;
typedef ap_fixed<12,8> fp5;
typedef ap_fixed<12,8> fp6;*/


typedef float fp1;
typedef float fp2;
typedef float fp3;
typedef float fp4;
typedef float fp6;
typedef float fp5;

//typedef float fp1;
//typedef float fp2;
//typedef float fp3;

typedef ap_int<3> int3;
typedef ap_int<4> int4;
typedef ap_int<6> int6;
typedef ap_int<8> int8;
typedef ap_int<10> int10;
typedef ap_int<12> int12;

/////typedef ap_int<14> int14;
typedef ap_int<24> int24;
typedef ap_int<28> int28;

typedef ap_int<16> int16;

typedef ap_int<14> int18;

typedef ap_int<20> int20;
typedef ap_int<22> int22;
typedef ap_int<30> int30;
typedef ap_int<32> int32;

typedef ap_uint<8> uint8;
typedef ap_uint<4> uint4;
typedef ap_uint<3> uint3;
typedef ap_uint<10> uint10;
typedef ap_uint<12> uint12;
typedef ap_uint<16> uint16;
typedef ap_uint<18> uint18;
typedef ap_uint<20> uint20;
typedef ap_uint<22> uint22;
typedef ap_uint<30> uint30;
typedef ap_uint<28> uint28;
typedef ap_uint<38> uint38;

///typedef ap_axis<8,0,0,0> AXI_VAL1;
///typedef ap_axis<16,0,0,0> AXI_VAL2;



typedef struct ap_hls1{
	float data;
//	fp1 data;
	//uint8 data;
//	ap_uint<2> keep;
    ap_uint<1> last;
}AXI_VAL1;

typedef struct ap_hls3{
	float data;
	//fp2 data;
	//int8 data;

    ap_uint<1> last;
}AXI_VAL3;

typedef struct ap_hls2{
	float data;
//	fp6 data;
	//int12 data;
//	ap_uint<2> keep;
    ap_uint<1> last;
}AXI_VAL2;
/*
typedef struct{
	fp2 N0_weight[PE_WEIGHT_SIZE];
	fp2 N1_weight[PE_WEIGHT_SIZE];
	fp2 N2_weight[PE_WEIGHT_SIZE];
	fp2 N3_weight[PE_WEIGHT_SIZE];
	fp2 N4_weight[PE_WEIGHT_SIZE];
	fp2 N5_weight[PE_WEIGHT_SIZE];
	fp2 N6_weight[PE_WEIGHT_SIZE];
	fp2 N7_weight[PE_WEIGHT_SIZE];
	//fp2 N8_weight[PE_WEIGHT_SIZE];
	//fp2 N9_weight[PE_WEIGHT_SIZE];
	//fp2 N10_weight[PE_WEIGHT_SIZE];
	//fp2 N11_weight[PE_WEIGHT_SIZE];

}PE_weights;

template<typename T>
struct PE_inputs{
	T N0_input[PE_FIFOSIZE];
	T N1_input[PE_FIFOSIZE];
	T N2_input[PE_FIFOSIZE];
	T N3_input[PE_FIFOSIZE];
	T N4_input[PE_FIFOSIZE];
	T N5_input[PE_FIFOSIZE];
	T N6_input[PE_FIFOSIZE];
	T N7_input[PE_FIFOSIZE];
	//T N8_input[PE_FIFOSIZE];
	//T N9_input[PE_FIFOSIZE];
	//T N10_input[PE_FIFOSIZE];
	//T N11_input[PE_FIFOSIZE];

};

template<typename T>
struct PE_outputs{
	  T PE0_output[MAX_PE_OUT];
	  T PE1_output[MAX_PE_OUT];
	  T PE2_output[MAX_PE_OUT];
	  T PE3_output[MAX_PE_OUT];
	  T PE4_output[MAX_PE_OUT];
	  T PE5_output[MAX_PE_OUT];
	  T PE6_output[MAX_PE_OUT];
	  T PE7_output[MAX_PE_OUT];
	//T PE8_output[MAX_PE_OUT];
	//T PE9_output[MAX_PE_OUT];
	//T PE10_output[MAX_PE_OUT];
	//T PE11_output[MAX_PE_OUT];
};

typedef struct{

	uint16 Ninputs;
	uint16 Noutputs;
	uint16 NnuronsH1;
	uint16 NnuronsH2;
	uint16 NnuronsH3;
	uint16 NnuronsH4;
	uint16 convOut;

	uint8 filter1;
	uint8 filter2;
	uint8 filter3;
	uint8 filter4;
	uint8 filter5;
	uint8 filter6;

	uint8  Nlayers;
	uint8 imageSize1;
	uint8 imageSize2;
	uint8 imageSize3;
	uint8 imageSize4;
	uint8 imageSize5;
	uint8 imageSize6;

	uint8 KernelSize1;
	uint8 KernelSize2;
	uint8 KernelSize3;
	uint8 KernelSize4;
	uint8 KernelSize5;
	uint8 KernelSize6;

	uint8 channels1;
	uint8 channels2;
	uint8 channels3;
	uint8 channels4;
	uint8 channels5;
	uint8 channels6;

	uint8 stride1;
	uint8 stride2;
	uint8 stride3;
	uint8 stride4;
	uint8 stride5;
	uint8 stride6;

	uint8 convOut1D1;
	uint8 convOut1D2;
	uint8 convOut1D3;
	uint8 convOut1D4;
	uint8 convOut1D5;
	uint8 convOut1D6;

	uint8	KSIZE1;
	uint8	KSIZE2;
	uint8	KSIZE3;
	uint8	KSIZE4;
	uint8	KSIZE5;
	uint8	KSIZE6;
	uint8 convLayer;

	int KsizeTotal;
	int filterTotal1;
	int filterTotal2;
	int filterTotal3;
	int filterTotal4;
	int filterTotal5;
	int filterTotal6;

}configuration;
*/


//void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL3> &weight_stream, hls::stream<AXI_VAL2> &out_stream, configuration *Config, int new_net);
//void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL2> &out_stream, configuration *Config, int new_net, int conv);

void Controller(fp1 in_stream[90], fp2 inputWeight[gateInWsize], fp2 forgetWeight[gateInWsize], fp2 cellWeight[gateInWsize], fp2 outputWeight[gateInWsize], fp2 inputReWeight[gateReWsize], fp2 forgetReWeight[gateReWsize], fp2 cellReWeight[gateReWsize], fp2 outputReWeight[gateReWsize], fp2  weights[hiddenUnitsLayer1], fp2 inputBias[hiddenUnitsLayer1], fp2 forgetBias[hiddenUnitsLayer1], fp2 cellBias[hiddenUnitsLayer1],  fp2 outputBias[hiddenUnitsLayer1], fp2 fcbias[1], fp2 out_stream[90], int new_net);
//void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL2> &out_stream, int new_net);

/*
template<class T, class U, class V>
T PE0_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations);

template<class T, class U, class V>
T PE1_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations);

template<class T, class U, class V>
T PE2_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations);

template<class T, class U, class V>
T PE3_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations);

template<class T, class U, class V>
T PE4_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations);

template<class T, class U, class V>
T PE5_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations);

template<class T, class U, class V>
T PE6_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations);

template<class T, class U, class V>
T PE7_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations);

//template<class T, class U, class V>
//T PE8_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations);

*/

/*
fp_type PE1_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE2_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE3_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE4_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE5_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE6_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE7_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE8_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE9_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE10_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE11_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
*/
//void gates();
void forgetGate(fp1  *In1, fp1 *ot1, int numInputs1, fp2  *InW1, fp2  *ReW1, fp2  *hid1, fp2 *b1, int isize1, int hsize1);
void inputGate(fp1  *In2, fp1 *ot2, int numInputs2, fp2  *InW2, fp2  *ReW2, fp2  *hid2, fp2 *b2, int isize2, int hsize2);
void outputGate(fp1  *In3, fp1 *ot3, int numInputs3, fp2  *InW3, fp2  *ReW3, fp2  *hid3, fp2 *b3, int isize3, int hsize3);
void cellGate(fp1  *In4, fp1 *ot4, int numInputs4, fp2  *InW4, fp2  *ReW4, fp2  *hid4, fp2 *b4, int isize4, int hsize4);


/*template<class T, class U>
T RELU(U x);

template<class T, class U>
T sig(U x);

template<class T, class U>
T tan(U x);*/

template<class T, class U>
T sig(U x);

template<class T, class U>
T tan(U x);

int get_label(fp6 *Output, int n);
int8 float2fix1(float n, int FIXED_BIT);
uint8 float2fix2(float n, int FIXED_BIT);
float fix2float(uint8 n, int FIXED_BIT);
