//#include "Controller_net3.h"
#include "Controller.h"
//#include "weights_LSTM.h"
//#include "InWeights_LSTM.h"
//#include "ReWeights_LSTM.h"
//#include "FCWeights_LSTM.h"
//#include "bias_LSTM.h"
#include "allParam2.h"
//#include "inputs.h"
#include "inputsLSTM2.h"
#include "labelLSTM2.h"
//#include "hls_stream.h"
//#include <math.h>


int main()
{

	//hls::stream<AXI_VAL1> in_stream;
	//hls::stream<AXI_VAL2> out_stream;

	//AXI_VAL1 d1,d2,d3,d4,d5,d6;
	//AXI_VAL2 out;



	float Output[50];
	int i,j,k,l,m;
	int retval=0;
	int new_net=1;

	int h;
	float correct = 0;

	fp1 in_stream[50];
	fp2 inputWeight[gateInWsize];
	fp2 forgetWeight[gateInWsize];
	fp2 cellWeight[gateInWsize];
	fp2 outputWeight[gateInWsize];
	fp2 inputReWeight[gateReWsize];
	fp2 forgetReWeight[gateReWsize];
	fp2 cellReWeight[gateReWsize];
	fp2 outputReWeight[gateReWsize];
	fp2  weights[hiddenUnitsLayer1];
	fp2 inputBias[hiddenUnitsLayer1];
	fp2 forgetBias[hiddenUnitsLayer1];
	fp2 cellBias[hiddenUnitsLayer1];
	fp2 outputBias[hiddenUnitsLayer1];
	fp2 fcbias[1];
	fp2 out_stream[50]; //use BRAM port


//-----------------------------Configure registers ----------------------------------
	 FILE *f2;


	 int MIW1 = 4*timeSteps*hiddenUnitsLayer1;
	 //int MIW2 = 4*hiddenUnitsLayer1*hiddenUnitsLayer2;

	 cout<<"MaxW_input1:"<< MIW1<<'\n';

	 int MRW1 = 4*hiddenUnitsLayer1*hiddenUnitsLayer1;
	 //int MRW2 = 4*hiddenUnitsLayer1*hiddenUnitsLayer2;

	 cout<<"MaxW_recur1:"<< MRW1<<'\n';

	 int sizeBias = 4*hiddenUnitsLayer1+1;
	 int fcWsize = hiddenUnitsLayer1;

	 int total = MIW1+MRW1+sizeBias+fcWsize;

	 cout<<"total parameters:"<< total<<'\n';

     int P;
	// printf("---------start-------------------------------- \n");
	 for(P=0;P<970;P++)
	 {
		//f1=fopen("result.dat","a");
		if(P>0)
		{
			new_net=0;
			f2=fopen("./labelLSTM1.txt", "a");


			 if (!f2)
			 {
				cout << "File not created!\n\n";
			 }
			 else
			 {
				cout << "File created successfully!\n\n";
				//f2.close();
			 }

			in_stream[0] = input_arr[P];

			for(i=0;i<2*hiddenUnitsLayer1;i++)
			{

				in_stream[i+1] = Output[i];
				//cout<<"inputs["<<i<<"]=  " << d1.data << "\n";
			}

		}
		else
		{

			f2=fopen("./labelLSTM1.txt", "a");

			 if (!f2)
			 {
				cout << "File not created!\n\n";
			 }
			 else
			 {
				cout << "File created successfully!\n\n";
				//f2.close();
			 }

			in_stream[0]=input_arr[P];


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

				inputReWeight[j] = param[j+gateInWsize*4];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<gateReWsize;j++)
			{

				forgetReWeight[j] = param[j+gateInWsize*4+gateReWsize];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<gateReWsize;j++)
			{

				cellReWeight[j] = param[j+gateInWsize*4+gateReWsize*2];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}
			for(j=0;j<gateReWsize;j++)
			{

				outputReWeight[j] = param[j+gateInWsize*4+gateReWsize*3];
				//cout<<"All Param["<<j<<"]=  " << d2.data << " "<< d2.last << "\n";
			}

			for(j=0;j<hiddenUnitsLayer1;j++)
			{

				weights[j] = param[j+gateInWsize*4+gateReWsize*4];
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
			fcbias[0] = param[gateInWsize*4+gateReWsize*4+hiddenUnitsLayer1*5];
		}

    ///Controller(in_stream, weights_stream, out_stream, &Config, new_net);                // call the controller
    ///Controller(in_stream, out_stream, new_net);
    Controller(in_stream, inputWeight, forgetWeight, cellWeight, outputWeight, inputReWeight, forgetReWeight, cellReWeight, outputReWeight, weights, inputBias, forgetBias, cellBias,  outputBias, fcbias, out_stream, new_net); //use BRAM port

	///out_stream.read(out);
	//out_stream>>out;
	Output[0] = out_stream[0] ;
	cout<<"Output["<<P<<"]=  " << Output[0] << "\n";
	fprintf(f2, " Predicted:  %f,   Actual:  %f \n", Output[0], labels[P]);
	fclose(f2);

		for(k=0;k<2*hiddenUnitsLayer1;k++)
		{

				Output[k] = out_stream[k+1] ;
				cout<<"(hidden+cell)["<<k<<"]=  " << Output[k] << "\n";
		}


    }

    /*for(l=0;l<INPUT_SIZE;l++)
    {
    	fprintf(f2, " Predicted:  %f,   Actual:  %f \n", Output[l], labels[l]);
        //cout<<"Output["<<i<<"]=  " << Output[i] << "\n";
    }*/

    //cout<< '\n' <<'\n'<<"tanh(2):" <<tan<fp1,fp1>(2)<< '\n';



	// fclose(f1);
	//fprintf(f2, "Actual: %f \n", labels[P]);

	/* if (get_label(Output,10)==labels[P])
	 {
		 printf(" Matched \n");
		 correct=correct+1;
	 }
	 else
		 printf(" UnMatched \n");
	}*/
	//printf(" Accuracy Final: %f\n", correct);
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
//	 }
   		return 0;
}


