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


/* #define INPUT_SIZE  64
#define OUTPUT_SIZE 64
#define M_WIEGHT_SIZE 4096
#define MAXPARTITIONS 4
#define MAXNOLAYERS 6  // input + hidden + output
#define NUM_PE 8 */


#define INPUT_SIZE  784
#define OUTPUT_SIZE 64
//#define M_WIEGHT_SIZE 500000*/

#define M_WEIGHT_SIZE 60000



#define MAXPARTITIONS 4
#define MAXNOLAYERS 6  // input + hidden + output
#define NUM_PE 8

#define PE_WEIGHT_SIZE 784
#define MAX_PE_OUT     200
#define PE_FIFOSIZE    784

//const unsigned long long M_WIEGHT_SIZE = 850000;

//const ap_uint<64> M_WIEGHT_SIZE = 50000;

/*
typedef ap_uint<20> int20;

typedef ap_int<8> int8;

typedef ap_int<4> int4;

typedef ap_int<3> int3;

typedef ap_uint<8> uint8;

typedef ap_uint<4> uint4;

typedef ap_uint<3> uint3;

typedef ap_int<16> int16;

typedef ap_uint<16> uint16;

typedef ap_int<32> int32;
*/
//#define FIXED_BIT 3

/*
#define INPUT_SIZE  64
#define OUTPUT_SIZE 64
#define M_WEIGHT_SIZE 4096
#define MAXPARTITIONS 4
#define MAXNOLAYERS 6  // input + hidden + output
#define NUM_PE 8

#define PE_WEIGHT_SIZE 64
#define MAX_PE_OUT     64
#define PE_FIFOSIZE    64
*/


typedef ap_uint<16> fixed_point_t;

//typedef ap_ufixed<3,1>fp_type1;
//typedef ap_ufixed<3,1>fp_type2;

//typedef ap_int<32> fp_type;
//typedef ap_int<32> fp_type1;

typedef float fp_type;
typedef float fp_type1;
typedef float fp_type2;

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

//typedef uint16 fp_type;
//typedef uint4 fp_type1;
//typedef uint16 fp_type2;

//typedef ap_axiu<32,4,5,5> AXI_VAL;

/*typedef union
{
	float i;
	fp_type o;
};*/

typedef struct ap_hls1{
	//float data;
	fp_type1 data;
	//uint8 data;
    ap_uint<1> last;
}AXI_VAL1;

typedef struct ap_hls2{
	//float data;
	fp_type data;
	//int16_t data;
    ap_uint<1> last;
}AXI_VAL2;



/*typedef struct{
	 float N0_weight[PE_WEIGHT_SIZE];
	 float N1_weight[PE_WEIGHT_SIZE];
	 float N2_weight[PE_WEIGHT_SIZE];
	 float N3_weight[PE_WEIGHT_SIZE];
	 float N4_weight[PE_WEIGHT_SIZE];
	 float N5_weight[PE_WEIGHT_SIZE];
	 float N6_weight[PE_WEIGHT_SIZE];
	 float N7_weight[PE_WEIGHT_SIZE];
}PE_weights;*/

typedef struct{
	fp_type1 N0_weight[PE_WEIGHT_SIZE];
	fp_type1 N1_weight[PE_WEIGHT_SIZE];
	fp_type1 N2_weight[PE_WEIGHT_SIZE];
	fp_type1 N3_weight[PE_WEIGHT_SIZE];
	fp_type1 N4_weight[PE_WEIGHT_SIZE];
	fp_type1 N5_weight[PE_WEIGHT_SIZE];
	fp_type1 N6_weight[PE_WEIGHT_SIZE];
	fp_type1 N7_weight[PE_WEIGHT_SIZE];
	
	
}PE_weights;
         
/*typedef struct{
	 float N0_input[PE_FIFOSIZE];
	 float N1_input[PE_FIFOSIZE];
	 float N2_input[PE_FIFOSIZE];
	 float N3_input[PE_FIFOSIZE];
	 float N4_input[PE_FIFOSIZE];
	 float N5_input[PE_FIFOSIZE];
	 float N6_input[PE_FIFOSIZE];
	 float N7_input[PE_FIFOSIZE];
}PE_inputs;*/
         
typedef struct{
	fp_type N0_input[PE_FIFOSIZE];
	fp_type N1_input[PE_FIFOSIZE];
	fp_type N2_input[PE_FIFOSIZE];
	fp_type N3_input[PE_FIFOSIZE];
	fp_type N4_input[PE_FIFOSIZE];
	fp_type N5_input[PE_FIFOSIZE];
	fp_type N6_input[PE_FIFOSIZE];
	fp_type N7_input[PE_FIFOSIZE];
	
	
}PE_inputs;

/*typedef struct{
	 float PE0_output[MAX_PE_OUT];
	 float PE1_output[MAX_PE_OUT];
	 float PE2_output[MAX_PE_OUT];
	 float PE3_output[MAX_PE_OUT];
	 float PE4_output[MAX_PE_OUT];
	 float PE5_output[MAX_PE_OUT];
	 float PE6_output[MAX_PE_OUT];
	 float PE7_output[MAX_PE_OUT];
}PE_outputs;*/

typedef struct{
	fp_type PE0_output[MAX_PE_OUT];
	fp_type PE1_output[MAX_PE_OUT];
	fp_type PE2_output[MAX_PE_OUT];
	fp_type PE3_output[MAX_PE_OUT];
	fp_type PE4_output[MAX_PE_OUT];
	fp_type PE5_output[MAX_PE_OUT];
	fp_type PE6_output[MAX_PE_OUT];
	fp_type PE7_output[MAX_PE_OUT];
	
	
}PE_outputs;

typedef struct{
                        
            int Nlayers;
            int Ninputs;
            int Noutputs;
            int NnuronsH1;
            int NnuronsH2;
            int NnuronsH3;
            int NnuronsH4;
            int thresholdv;
}configuration;

typedef int one_bit;
typedef int TwoBits;
typedef int OneByte;

void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL2> &out_stream, configuration *Config, int new_net);

/*float PE0_Unit(float in_fifo[PE_FIFOSIZE], float weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
float PE1_Unit(float in_fifo[PE_FIFOSIZE], float weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
float PE2_Unit(float in_fifo[PE_FIFOSIZE], float weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
float PE3_Unit(float in_fifo[PE_FIFOSIZE], float weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
float PE4_Unit(float in_fifo[PE_FIFOSIZE], float weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
float PE5_Unit(float in_fifo[PE_FIFOSIZE], float weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
float PE6_Unit(float in_fifo[PE_FIFOSIZE], float weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
float PE7_Unit(float in_fifo[PE_FIFOSIZE], float weight_fifo[PE_WEIGHT_SIZE], int Num_operations);*/

fp_type PE0_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE1_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE2_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE3_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE4_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE5_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE6_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);
fp_type PE7_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations);

/*fixed_point_t float2fix(float n);
float fix2float(fixed_point_t n);*/
int get_label(fp_type2 *Output, int n);

//int16_t float2fix(float n);
//float fix2float(int16_t n);

int8 float2fix1(float n, int FIXED_BIT);
float fix2float(int8 n, int FIXED_BIT);
int8 float2fix2(float n, int FIXED_BIT);
