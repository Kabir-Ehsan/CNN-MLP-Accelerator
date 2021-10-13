//#include "Controller_net3.h"
#include "Controller_net_upgrade1.h"
#include "weights_mnist2.h"
//#include "weights_mnist.h"
#include "inputs.h"
//#include "inputs3.h"
#include "weights_kernel.h"
#include "hls_stream.h"
#include <math.h>


int main()
{

	hls::stream<AXI_VAL1> in_stream;
	hls::stream<AXI_VAL2> out_stream;

	AXI_VAL1 d1,d2,d3, in;
	AXI_VAL2 out;



	fp6 Output[OUTPUT_SIZE];
//	int start=1;
//	one_bit last=0;
	int i,j,k;
	int retval=0;
	int new_net=1;
	int conv=1;
	int last_layer;
	int h;
	float correct = 0;


//-----------------------------Configure registers ----------------------------------
	FILE        *f1, *f2;
	//fp1=fopen("result.dat","a");
	//fp2=fopen("C:\\Users\\ekabir\\Desktop\\FPGA\\MLP_HLS\\mlp_PE02_With_HLS_Stream\\HLS_PE08_withScript\\mlp2\\labels.txt", "a");

	configuration Config;

	Config.Nlayers=2;   //hidden + output
///	Config.Nlayers=3;
	Config.Ninputs=784;
	Config.Noutputs=10;
	Config.NnuronsH1=40;
///	Config.NnuronsH1=64;
	Config.NnuronsH2=0;
///	Config.NnuronsH2=64;
	Config.NnuronsH3=0;
	Config.NnuronsH4=0;
	Config.imageSize=28;
	Config.convOut=441;
	Config.KernelSize=8;
	Config.channels=1;
	Config.stride=1;
	Config.padding=0;
	Config.KSIZE=64;
	Config.convOut1D=21;
	Config.convLayer=1;
	Config.filter=1;
	//Config.thresholdv=-1.0;

	//****************************Inputs**************************************************
    int MaxW=0;
		// for(i=0;i<config.Ninputs+1;i++)           // put the inputs in the beginning of the stream
			// in_stream[i]=1;//input[i];
	//------------------------------------------------------------------------------------
	if(Config.Nlayers==2)
		last_layer=	Config.NnuronsH1;
	else if(Config.Nlayers==3)
		last_layer=	Config.NnuronsH2;
	else if(Config.Nlayers==4)
		last_layer=	Config.NnuronsH3;
	else if(Config.Nlayers==5)
			last_layer=	Config.NnuronsH4;
	//****************************Weights**************************************************
	 //// MaxW=((Config.Ninputs)* Config.NnuronsH1)+((Config.NnuronsH1)* Config.NnuronsH2)+((Config.NnuronsH2)* Config.NnuronsH3)+((Config.NnuronsH3)* Config.NnuronsH4)+ ((last_layer)* Config.Noutputs); // no of weights
	 MaxW=((Config.convOut)* Config.NnuronsH1)+((Config.NnuronsH1)* Config.NnuronsH2)+((Config.NnuronsH2)* Config.NnuronsH3)+((Config.NnuronsH3)* Config.NnuronsH4)+ ((last_layer)* Config.Noutputs);
	   printf("MaxW=%d \n",MaxW);

//------------------------------------------------------------------------------------
     int P;
	// printf("---------start-------------------------------- \n");
	 for(P=0;P<100;P++)
	 {
		//f1=fopen("result.dat","a");
		if(P>0)
		{
			new_net=0;
			//f2=fopen("C:\\Users\\ekabir\\Desktop\\FPGA\\hw\\HLS_PE08_2\\labelTestFixWithRelu64.txt", "a");
			for(i=0;i<Config.Ninputs;i++)
			{
				d1.data = input_arr[i+784*P];
	//			cout<<"d1.data["<<i<<"]=  " << d1.data << "\n";
				d1.last = (i==Config.Ninputs-1);
				in_stream.write(d1);
			}
		}
		else
		{

			for(i=0;i<Config.Ninputs;i++)
			{
				d1.data = input_arr[i+784*P];
				cout<<"d1.data["<<i<<"]=  " << d1.data << "\n";
				d1.last = (i==Config.Ninputs-1);
				in_stream.write(d1);
			}

			for(i=0;i<MaxW;i++)
			{

				d2.data = weights[i];
//				cout<<"d2.data["<<i<<"]=  " << d2.data << "\n";
				d2.last = (i==MaxW-1);
				in_stream.write(d2);
			}
			if(conv==1)
			{
				for(i=0;i<(Config.KernelSize)*(Config.KernelSize);i++)
				{

					d3.data = Kernels[i];
//					cout<<"d3.data["<<i<<"]=  " << d3.data << "\n";
					d3.last = (i==(Config.KernelSize)*(Config.KernelSize)-1);
					in_stream.write(d3);
				}
			}
		}

    ///Controller(in_stream, weights_stream, out_stream, &Config, new_net);                // call the controller
    Controller(in_stream, out_stream, &Config, new_net, conv);
    for(i=0;i<Config.Noutputs;i++)
    {
    	//out_stream[i].read(out);
    	out_stream.read(out);
    	//Output[i] = fix2float(out.data, 16) ;
    	Output[i] = out.data ;
    }


	 for(i=0;i<Config.Noutputs;i++)
	 {
		  cout<<"Output["<<i<<"]=  " << Output[i] << "\n";
		  //printf(" Output[%d]=  %f	 \n", i,Output[i]); //print the outputs
		 // fprintf(f1, " %d   %f \n", i, Output[i]);
	 }
	// fclose(f1);
	 //fprintf(f2, "\n label: %d,  Actual: %d \n", get_label(Output,10), labels[P]);
	 //fclose(f2);
	 if (get_label(Output,10)==labels[P])
	 {
		 printf(" Matched \n");
		 correct=correct+1;
	 }
	 else
		 printf(" UnMatched \n");
	}
	printf(" Accuracy Final: %f\n", correct);
   	    /*
   		// Compare the results file with the golden results
   		retval = system("diff --brief -w result.dat result.golden.dat");
   		if (retval != 0)
   		{
   			printf("Test failed  !!!\n");
   			retval=1;
   		}
   		else
   		{
   			printf("Test passed !\n");
   		}
    // printf("last=%d \n",last);

   	     */
   		return 0;
}


/*

//fixed_point_t float2fix(float n)
//int16_t float2fix(float n)*/
int8 float2fix1(float n, int FIXED_BIT)
{
	//fixed_point_t int_part = 0, frac_part = 0;
	//int16_t int_part = 0, frac_part = 0;
	int8 int_part = 0, frac_part = 0;
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

uint8 float2fix2(float n, int FIXED_BIT)
{
	//fixed_point_t int_part = 0, frac_part = 0;
	//int16_t int_part = 0, frac_part = 0;
	uint8 int_part = 0, frac_part = 0;
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

//float fix2float(fixed_point_t n)
//float fix2float(int16_t n)
/*float fix2float(int16 n, int FIXED_BIT)
{
    float ret = 0.0;
    int integer_part;
    int i;

    integer_part = (n >> FIXED_BIT);
    ret += integer_part;
    for (i = 0; i < FIXED_BIT; i++) {
        ret += ((n >> (FIXED_BIT - i - 1)) & (0x01)) * (1.0 / pow(2, i+1));
    }

    return ret;
};*/

int get_label(fp6 *Output, int n)
{
	if(n<=0) return -1;
	int k=0, max_k=0;
	fp6 max = Output[0];
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

