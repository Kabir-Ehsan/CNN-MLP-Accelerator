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

#define INPUT_SIZE  1024
#define OUTPUT_SIZE 64
#define M_WEIGHT_SIZE 65536 //850000//60000// 300000 //
#define ksize 4096
#define MaxConvOut1D 32
#define NPc 4
#define K 32
#define MAXPARTITIONS 4
#define MAXNOLAYERS 6  // input + hidden + output
#define NUM_PE 8

//#define kernelSize 8
//#define kernelNo 1
//#define channels 1

#define PE_WEIGHT_SIZE 1024
#define MAX_PE_OUT     256
#define PE_FIFOSIZE    1024 //power of 2 values

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


/*typedef ap_ufixed<8,4> fp1;
typedef ap_fixed<8,4> fp2;
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

/*
typedef union
{
	float f;
	int8 i;
}conv1;

typedef union
{
	float f;
	int8 i;
}conv2;*/
/*
typedef float int3;
typedef float int4;
typedef float int8;
typedef float int10;
typedef float int12;

typedef float int14;
typedef float int24;
typedef float int28;

typedef float int16;
typedef float int18;
typedef float int20;
typedef float int22;
typedef float int30;
typedef float int32;
*/

/*typedef float uint8;
typedef float uint18;
typedef float uint28;
typedef float uint38;*/


//typedef uint8 fp_type;
//typedef uint8 fp_type1;
//typedef uint38 fp_type2;

typedef struct ap_hls1{
	//float data;
	fp1 data;
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
	//float data;
	fp6 data;
	//int12 data;
//	ap_uint<2> keep;
    ap_uint<1> last;
}AXI_VAL2;

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
	uint8  Nlayers;
	uint8 imageSize;
	uint8 KernelSize;
	uint8 channels;
	uint8 stride;
	uint8 padding;
	uint8 KSIZE;
	uint8 convOut1D;

}configuration;



//void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL3> &weight_stream, hls::stream<AXI_VAL2> &out_stream, configuration *Config, int new_net);
void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL2> &out_stream, configuration *Config, int new_net, int conv);

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

template<class T, class U>
T RELU(U x);

int get_label(fp6 *Output, int n);
int8 float2fix1(float n, int FIXED_BIT);
uint8 float2fix2(float n, int FIXED_BIT);
float fix2float(uint8 n, int FIXED_BIT);
