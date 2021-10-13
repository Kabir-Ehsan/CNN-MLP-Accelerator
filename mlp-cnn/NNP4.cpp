#include "Controller_net2.h"


void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL2> &out_stream, configuration *Config, int new_net){

    	int    NInputs[MAXNOLAYERS];
    	int    NNeurons[MAXNOLAYERS];
    	int    NPartions[MAXNOLAYERS]; // How many partitions for each layer?
    	int    NExtraPE_Needed[MAXNOLAYERS];
    	fp_type1  weights[M_WEIGHT_SIZE];

    	PE_weights PE_weight;
    	PE_inputs PE_input;
    	PE_outputs PE_output;

    	int    Lcounter=0;
        int    Wcount=0;
    	int    NOutputs=0;
    	fp_type  inputs[INPUT_SIZE];
    	fp_type  OutputArray[OUTPUT_SIZE];
    	int    i,j,k,l,R;
    	int    ws_index=0;
    	fp_type    threshold;
    	float  temp;

    	AXI_VAL1 in;
        AXI_VAL2 out;

    	 threshold=Config->thresholdv;
    	 Lcounter=Config->Nlayers;     // hidden + outputs
    	 NOutputs=Config->Noutputs;
    	 NInputs[0]=Config->Ninputs;
    	 NInputs[1]=Config->NnuronsH1;
    	 NInputs[2]=Config->NnuronsH2;
    	 NInputs[3]=Config->NnuronsH3;
    	 NInputs[4]=Config->NnuronsH4;

    	 NNeurons[0]=Config->NnuronsH1;
    	 NNeurons[1]=Config->NnuronsH2;
    	 NNeurons[2]=Config->NnuronsH3;
    	 NNeurons[3]=Config->NnuronsH4;
    	 NNeurons[Lcounter-1]=NOutputs;


         int Counter1=0;
	     int Counter2;
	     int Counter3=0;
	     int OutCounter=0;

	     int weightcount=0;

			/* PE_input.N0_input[0]= threshold;
			PE_input.N1_input[0]= threshold;
			PE_input.N2_input[0]= threshold;
			PE_input.N3_input[0]= threshold;
			PE_input.N4_input[0]= threshold;
			PE_input.N5_input[0]= threshold;
			PE_input.N6_input[0]= threshold;
			PE_input.N7_input[0]= threshold;
			PE_output.PE0_output[0]= threshold;
			PE_output.PE1_output[0]= threshold;
			PE_output.PE2_output[0]= threshold;
			PE_output.PE3_output[0]= threshold;
			PE_output.PE4_output[0]= threshold;
			PE_output.PE5_output[0]= threshold;
			PE_output.PE6_output[0]= threshold;
			PE_output.PE7_output[0]= threshold; */

 //Storing the streaming input into Input Buffer
            input: for(i=0;i<Config->Ninputs;i++)
            {
                 in_stream >> in;

				 PE_input.N0_input[i]= in.data;
/*#ifndef __SYNTHESIS__
				 cout<<"PE_input.N0_input[" << i << "]="<< PE_input.N0_input[i]<<'\n';
#endif*/
				 PE_input.N1_input[i]= in.data;
				 PE_input.N2_input[i]= in.data;
				 PE_input.N3_input[i]= in.data;
				 PE_input.N4_input[i]= in.data;
				 PE_input.N5_input[i]= in.data;
				 PE_input.N6_input[i]= in.data;
				 PE_input.N7_input[i]= in.data;
			}

	 if(new_net==1)
	 {

		//--------------- configure the net-------------------------
		//-------------------separate inputs from weights--------------

			 int    MaxW=0;
			 MaxWeight: for(j=0;j<Lcounter;j++)
				  MaxW=MaxW+(NInputs[j]*NNeurons[j]);

			 Weight: for(i=0;i<(MaxW);i++)
                     {
					       in_stream.read(in);
					       weights[i] = in.data;
				    }


		 //---------Calculate the Number of Partitions to process each layer--------------------

         PartitionL: for(i=0;i<Lcounter;i++)
    				 {
						NPartions[i]=(int)(NNeurons[i]/ NUM_PE);
						NExtraPE_Needed[i]=NNeurons[i]-(NPartions[i]*NUM_PE);
				     }
		}

//-------------------------------end feeding weights-------------------------------------------------

//------------------------------- start feeding inputs-----------------------------------------------

K1: for(k=0;k<NPartions[0];k++)
	{
		J1:	for(j=0;j<NInputs[0];j++)
			{
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N3_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N4_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N5_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N6_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N7_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

			}

				 PE_output.PE0_output[Counter1]=PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
				 PE_output.PE1_output[Counter1]=PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
				 PE_output.PE2_output[Counter1]=PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
				 PE_output.PE3_output[Counter1]=PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
				 PE_output.PE4_output[Counter1]=PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
				 PE_output.PE5_output[Counter1]=PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
				 PE_output.PE6_output[Counter1]=PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
				 PE_output.PE7_output[Counter1]=PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);

			 Counter1=Counter1+1;
				 PE_output.PE0_output[Counter1]=PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
				 PE_output.PE1_output[Counter1]=PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
				 PE_output.PE2_output[Counter1]=PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
				 PE_output.PE3_output[Counter1]=PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
				 PE_output.PE4_output[Counter1]=PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
				 PE_output.PE5_output[Counter1]=PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
				 PE_output.PE6_output[Counter1]=PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
				 PE_output.PE7_output[Counter1]=PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);

			 Counter1=Counter1+1;
				 PE_output.PE0_output[Counter1]=PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
				 PE_output.PE1_output[Counter1]=PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
				 PE_output.PE2_output[Counter1]=PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
				 PE_output.PE3_output[Counter1]=PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
				 PE_output.PE4_output[Counter1]=PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
				 PE_output.PE5_output[Counter1]=PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
				 PE_output.PE6_output[Counter1]=PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
				 PE_output.PE7_output[Counter1]=PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);

			 Counter1=Counter1+1;
				 PE_output.PE0_output[Counter1]=PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
				 PE_output.PE1_output[Counter1]=PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
				 PE_output.PE2_output[Counter1]=PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
				 PE_output.PE3_output[Counter1]=PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
				 PE_output.PE4_output[Counter1]=PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
				 PE_output.PE5_output[Counter1]=PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
				 PE_output.PE6_output[Counter1]=PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
				 PE_output.PE7_output[Counter1]=PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);

			 Counter1=Counter1+1;
				 PE_output.PE0_output[Counter1]=PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
				 PE_output.PE1_output[Counter1]=PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
				 PE_output.PE2_output[Counter1]=PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
				 PE_output.PE3_output[Counter1]=PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
				 PE_output.PE4_output[Counter1]=PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
				 PE_output.PE5_output[Counter1]=PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
				 PE_output.PE6_output[Counter1]=PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
				 PE_output.PE7_output[Counter1]=PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);

			 Counter1=Counter1+1;
				 PE_output.PE0_output[Counter1]=PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
				 PE_output.PE1_output[Counter1]=PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
				 PE_output.PE2_output[Counter1]=PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
				 PE_output.PE3_output[Counter1]=PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
				 PE_output.PE4_output[Counter1]=PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
				 PE_output.PE5_output[Counter1]=PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
				 PE_output.PE6_output[Counter1]=PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
				 PE_output.PE7_output[Counter1]=PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);

			 Counter1=Counter1+1;
				 PE_output.PE0_output[Counter1]=PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
				 PE_output.PE1_output[Counter1]=PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
				 PE_output.PE2_output[Counter1]=PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
				 PE_output.PE3_output[Counter1]=PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
				 PE_output.PE4_output[Counter1]=PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
				 PE_output.PE5_output[Counter1]=PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
				 PE_output.PE6_output[Counter1]=PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
				 PE_output.PE7_output[Counter1]=PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);

			 Counter1=Counter1+1;
				 PE_output.PE0_output[Counter1]=PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[0]);
				 PE_output.PE1_output[Counter1]=PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[0]);
				 PE_output.PE2_output[Counter1]=PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[0]);
				 PE_output.PE3_output[Counter1]=PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[0]);
				 PE_output.PE4_output[Counter1]=PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[0]);
				 PE_output.PE5_output[Counter1]=PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[0]);
				 PE_output.PE6_output[Counter1]=PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[0]);
				 PE_output.PE7_output[Counter1]=PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[0]);

			 Counter1=Counter1+1;
			}


		 if(NExtraPE_Needed[0]==1)
          {
                 Ex1: for(j=0;j<NInputs[0];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

			 PE_output.PE0_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);

			 Counter1=Counter1+1;
		}


		 if(NExtraPE_Needed[0]==2)
          {
                 Ex2: for(j=0;j<NInputs[0];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

			 PE_output.PE0_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);

			 Counter1=Counter1+1;
		}


		 if(NExtraPE_Needed[0]==3)
          {
                 Ex3: for(j=0;j<NInputs[0];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

			 PE_output.PE0_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);

			 Counter1=Counter1+1;
		}


		 if(NExtraPE_Needed[0]==4)
          {
                 Ex4: for(j=0;j<NInputs[0];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N3_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

			 PE_output.PE0_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);

			 Counter1=Counter1+1;
		}


		 if(NExtraPE_Needed[0]==5)
          {
                 Ex5: for(j=0;j<NInputs[0];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N3_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N4_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

			 PE_output.PE0_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);

			 Counter1=Counter1+1;
		}


		 if(NExtraPE_Needed[0]==6)
          {
                 Ex6: for(j=0;j<NInputs[0];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N3_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N4_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N5_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

			 PE_output.PE0_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);

			 Counter1=Counter1+1;
		}


		 if(NExtraPE_Needed[0]==7)
          {
                 Ex7: for(j=0;j<NInputs[0];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N3_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N4_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N5_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N6_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

			 PE_output.PE0_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[0]);

			 Counter1=Counter1+1;
			 PE_output.PE0_output[Counter1] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
			 PE_output.PE1_output[Counter1] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
			 PE_output.PE2_output[Counter1] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
			 PE_output.PE3_output[Counter1] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
			 PE_output.PE4_output[Counter1] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
			 PE_output.PE5_output[Counter1] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
			 PE_output.PE6_output[Counter1] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);
			 PE_output.PE7_output[Counter1] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[0]);

			 Counter1=Counter1+1;
		}


 //-----------------------------------------------------------------------------------------------------

LyrL: for(i=1;i<Lcounter;i++)
      {
            Counter2 = 0;

K3: for(k=0;k<NPartions[i];k++)
	{
		J3:	for(j=0;j<NInputs[i];j++)
			{
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N3_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N4_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N5_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N6_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

					 PE_weight.N7_weight[j]=weights[weightcount];
					 weightcount=weightcount+1;

			}

if(i%2 == 0)
			   {

			 PE_output.PE0_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE7_Unit(PE_input.N7_input, PE_weight.N7_weight, NInputs[i]);

			 Counter2=Counter2+1;
			}

else
            {
			 PE_input.N0_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE7_Unit(PE_output.PE7_output, PE_weight.N7_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE7_Unit(PE_output.PE7_output, PE_weight.N7_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE7_Unit(PE_output.PE7_output, PE_weight.N7_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE7_Unit(PE_output.PE7_output, PE_weight.N7_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE7_Unit(PE_output.PE7_output, PE_weight.N7_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE7_Unit(PE_output.PE7_output, PE_weight.N7_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE7_Unit(PE_output.PE7_output, PE_weight.N7_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE7_Unit(PE_output.PE7_output, PE_weight.N7_weight, NInputs[i]);

			 Counter2=Counter2+1;
			}
            }


		 if(NExtraPE_Needed[i]==1)
          {
                 Ex8: for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

if(i%2==0)
                {
			 PE_output.PE0_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
else
                {
			 PE_input.N0_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
                }

		 if(NExtraPE_Needed[i]==2)
          {
                 Ex9: for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

if(i%2==0)
                {
			 PE_output.PE0_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
else
                {
			 PE_input.N0_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
                }

		 if(NExtraPE_Needed[i]==3)
          {
                 Ex10: for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

if(i%2==0)
                {
			 PE_output.PE0_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
else
                {
			 PE_input.N0_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
                }

		 if(NExtraPE_Needed[i]==4)
          {
                 Ex11: for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N3_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

if(i%2==0)
                {
			 PE_output.PE0_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
else
                {
			 PE_input.N0_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
                }

		 if(NExtraPE_Needed[i]==5)
          {
                 Ex12: for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N3_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N4_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

if(i%2==0)
                {
			 PE_output.PE0_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
else
                {
			 PE_input.N0_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
                }

		 if(NExtraPE_Needed[i]==6)
          {
                 Ex13: for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N3_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N4_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N5_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

if(i%2==0)
                {
			 PE_output.PE0_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
else
                {
			 PE_input.N0_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
                }

		 if(NExtraPE_Needed[i]==7)
          {
                 Ex14: for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N1_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N2_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N3_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N4_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N5_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;
					 PE_weight.N6_weight[j]=weights[weightcount];
					 weightcount = weightcount+1;

				  }

if(i%2==0)
                {
			 PE_output.PE0_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE0_Unit(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE1_Unit(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE2_Unit(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE3_Unit(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE4_Unit(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE5_Unit(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_output.PE0_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE1_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE2_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE3_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE4_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE5_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE6_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			 PE_output.PE7_output[Counter2] = PE6_Unit(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
else
                {
			 PE_input.N0_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE0_Unit(PE_output.PE0_output, PE_weight.N0_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE1_Unit(PE_output.PE1_output, PE_weight.N1_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE2_Unit(PE_output.PE2_output, PE_weight.N2_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE3_Unit(PE_output.PE3_output, PE_weight.N3_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE4_Unit(PE_output.PE4_output, PE_weight.N4_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE5_Unit(PE_output.PE5_output, PE_weight.N5_weight, NInputs[i]);

			 Counter2=Counter2+1;
			 PE_input.N0_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N1_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N2_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N3_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N4_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N5_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N6_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);
			 PE_input.N7_input[Counter2] = PE6_Unit(PE_output.PE6_output, PE_weight.N6_weight, NInputs[i]);

			 Counter2=Counter2+1;
}
                }
}


 //-----------------------------------------------------------------------------------------------------

if(i==Lcounter)
{                                       // push the outputs to the axi bus


      K4:for(k=0;k<NPartions[i-1];k++)
      {
    	  if(i%2!=0)
                {
				 out.data = PE_output.PE0_output[Counter3];
                out.last = (OutCounter+0==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_output.PE1_output[Counter3+1];
                out.last = (OutCounter+1==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_output.PE2_output[Counter3+2];
                out.last = (OutCounter+2==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_output.PE3_output[Counter3+3];
                out.last = (OutCounter+3==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_output.PE4_output[Counter3+4];
                out.last = (OutCounter+4==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_output.PE5_output[Counter3+5];
                out.last = (OutCounter+5==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_output.PE6_output[Counter3+6];
                out.last = (OutCounter+6==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_output.PE7_output[Counter3+7];
                out.last = (OutCounter+7==NOutputs-1);
				out_stream.write(out);


				OutCounter=OutCounter+8;
				Counter3 = Counter3+8;
}

else
                {
				 out.data = PE_input.N0_input[Counter3];
                out.last = (OutCounter+0==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_input.N1_input[Counter3+1];
                out.last = (OutCounter+1==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_input.N2_input[Counter3+2];
                out.last = (OutCounter+2==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_input.N3_input[Counter3+3];
                out.last = (OutCounter+3==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_input.N4_input[Counter3+4];
                out.last = (OutCounter+4==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_input.N5_input[Counter3+5];
                out.last = (OutCounter+5==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_input.N6_input[Counter3+6];
                out.last = (OutCounter+6==NOutputs-1);
				out_stream.write(out);

				 out.data = PE_input.N7_input[Counter3+7];
                out.last = (OutCounter+7==NOutputs-1);
				out_stream.write(out);


				OutCounter=OutCounter+8;
				Counter3= Counter3+8;
}
		}

		if(NExtraPE_Needed[i-1] !=0)
		{

			 if(NExtraPE_Needed[i-1]==1)
             {
if(i%2!=0)
                {
				 out.data = PE_output.PE0_output[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+1;
				Counter3=Counter3+1;
}
else
                    {
				 out.data = PE_input.N0_input[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+1;
				Counter3=Counter3+1;
}

                    }
	 else if(NExtraPE_Needed[i-1]==2)
             {
if(i%2!=0)
                {
				 //out.data = PE_output.PE0_output[NPartions[i-1]];
				 out.data = PE_output.PE0_output[Counter3];
                 out.last = (OutCounter+0==NOutputs-1);
                 out_stream.write(out);

				 //out.data = PE_output.PE1_output[NPartions[i-1]];
                 out.data = PE_output.PE1_output[Counter3+1];
                 out.last = (OutCounter+1==NOutputs-1);
                 out_stream.write(out);

				OutCounter=OutCounter+2;
				Counter3=Counter3+2;
}
else
                    {
				 out.data = PE_input.N0_input[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N1_input[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+2;
				Counter3=Counter3+2;
}

                    }
	 else if(NExtraPE_Needed[i-1]==3)
             {
if(i%2!=0)
                {
				 out.data = PE_output.PE0_output[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE1_output[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE2_output[Counter3+2];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+3;
				Counter3=Counter3+3;
}
else
                    {
				 out.data = PE_input.N0_input[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N1_input[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N2_input[Counter3+2];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+3;
				Counter3=Counter3+3;
}

                    }
	 else if(NExtraPE_Needed[i-1]==4)
             {
if(i%2!=0)
                {
				 out.data = PE_output.PE0_output[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE1_output[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE2_output[Counter3+2];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE3_output[Counter3+3];
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+4;
				Counter3=Counter3+4;
}
else
                    {
				 out.data = PE_input.N0_input[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N1_input[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N2_input[Counter3+2];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N3_input[Counter3+3];
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+4;
				Counter3=Counter3+4;
}

                    }
	 else if(NExtraPE_Needed[i-1]==5)
             {
if(i%2!=0)
                {
				 out.data = PE_output.PE0_output[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE1_output[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE2_output[Counter3+2];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE3_output[Counter3+3];
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE4_output[Counter3+4];
                    out.last = (OutCounter+4==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+5;
				Counter3=Counter3+5;
}
else
                    {
				 out.data = PE_input.N0_input[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N1_input[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N2_input[Counter3+2];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N3_input[Counter3+3];
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N4_input[Counter3+4];
                    out.last = (OutCounter+4==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+5;
				Counter3=Counter3+5;
}

                    }
	 else if(NExtraPE_Needed[i-1]==6)
             {
if(i%2!=0)
                {
				 out.data = PE_output.PE0_output[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE1_output[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE2_output[Counter3+2];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE3_output[Counter3+3];
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE4_output[Counter3+4];
                    out.last = (OutCounter+4==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE5_output[Counter3+5];
                    out.last = (OutCounter+5==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+6;
				Counter3=Counter3+6;
}
else
                    {
				 out.data = PE_input.N0_input[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N1_input[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N2_input[Counter3+2];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N3_input[Counter3+3];
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N4_input[Counter3+4];
                    out.last = (OutCounter+4==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N5_input[Counter3+5];
                    out.last = (OutCounter+5==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+6;
				Counter3=Counter3+6;
}

                    }
	 else if(NExtraPE_Needed[i-1]==7)
             {
if(i%2!=0)
                {
				 out.data = PE_output.PE0_output[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE1_output[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE2_output[Counter3+2];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE3_output[Counter3+3];
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE4_output[Counter3+4];
                    out.last = (OutCounter+4==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE5_output[Counter3+5];
                    out.last = (OutCounter+5==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE6_output[Counter3+6];
                    out.last = (OutCounter+6==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+7;
				Counter3=Counter3+7;
}
else
                    {
				 out.data = PE_input.N0_input[Counter3];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N1_input[Counter3+1];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N2_input[Counter3+2];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N3_input[Counter3+3];
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N4_input[Counter3+4];
                    out.last = (OutCounter+4==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N5_input[Counter3+5];
                    out.last = (OutCounter+5==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_input.N6_input[Counter3+6];
                    out.last = (OutCounter+6==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+7;
				Counter3=Counter3+7;
}

                    }
}


        }
  return;
}



 //--------------------- PE0_Unit--------------------
fp_type PE0_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
    #pragma HLS INLINE off

    	  fp_type2 sum=0;

    	  fp_type temp1=0;
    	  fp_type temp2=0;

    	  int i;

    		  PE0_Unit_label0:for(i=0;i<Num_operations;i++){
    #pragma HLS UNROLL factor=2
    		      sum=sum+(in_fifo[i]* weight_fifo[i]);
#ifndef __SYNTHESIS__
  		      cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
#endif
    	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
    	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
    		  }
#ifndef __SYNTHESIS__
   	   	     cout<< "PE0-----------:" << sum << "   " << '\n'<<'\n';
#endif
   	   	//return sum;
   	   	   	   	    if(sum<0)
   	   	            {
   	   	                return 0;
   	   	            }
   	   	            else
   	   	            {
   	   	                return sum;
   	   	            }

    		//  temp1=1.0f +expf(-1 * sum);
    	    //  temp2=1.0f/temp1;

    	    //printf("OUT_N0      =%f \n",temp2);
    	    	//printf("PE0---------------------------------------------------------------------sum=%f \n",sum);
    	 //return temp2;
    }



 //------------------------------------------------------------------------------------------

 //--------------------- PE1_Unit--------------------
fp_type PE1_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
    #pragma HLS INLINE off

    	  fp_type sum=0;

    	  fp_type temp1=0;
    	  fp_type temp2=0;

    	  int i;

    		  PE1_Unit_label1:for(i=0;i<Num_operations;i++){
    #pragma HLS UNROLL factor=2
    		      sum=sum+(in_fifo[i]* weight_fifo[i]);
#ifndef __SYNTHESIS__
  		      cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
#endif
    	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
    	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
    		  }
#ifndef __SYNTHESIS__
   	   	     cout<< "PE1-----------:" << sum << "   " << '\n'<<'\n';
#endif
   	   	//return sum;
   	   	   	   	    if(sum<0)
   	   	            {
   	   	                return 0;
   	   	            }
   	   	            else
   	   	            {
   	   	                return sum;
   	   	            }

    		//  temp1=1.0f +expf(-1 * sum);
    	    //  temp2=1.0f/temp1;

    	    //printf("OUT_N1      =%f \n",temp2);
    	    	//printf("PE1---------------------------------------------------------------------sum=%f \n",sum);
    	 //return temp2;
    }



 //------------------------------------------------------------------------------------------

 //--------------------- PE2_Unit--------------------
fp_type PE2_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
    #pragma HLS INLINE off

    	  fp_type2 sum=0;

    	  fp_type temp1=0;
    	  fp_type temp2=0;

    	  int i;

    		  PE2_Unit_label2:for(i=0;i<Num_operations;i++){
    #pragma HLS UNROLL factor=2
    		      sum=sum+(in_fifo[i]* weight_fifo[i]);
#ifndef __SYNTHESIS__
  		      cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
#endif
    	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
    	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
    		  }
#ifndef __SYNTHESIS__
   	   	     cout<< "PE2-----------:" << sum << "   " << '\n'<<'\n';
#endif
   	   	//return sum;
   	   	   	   	    if(sum<0)
   	   	            {
   	   	                return 0;
   	   	            }
   	   	            else
   	   	            {
   	   	                return sum;
   	   	            }

    		//  temp1=1.0f +expf(-1 * sum);
    	    //  temp2=1.0f/temp1;

    	    //printf("OUT_N2      =%f \n",temp2);
    	    	//printf("PE2---------------------------------------------------------------------sum=%f \n",sum);
    	 //return temp2;
    }



 //------------------------------------------------------------------------------------------

 //--------------------- PE3_Unit--------------------
fp_type PE3_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
    #pragma HLS INLINE off

    	  fp_type2 sum=0;

    	  fp_type temp1=0;
    	  fp_type temp2=0;

    	  int i;

    		  PE3_Unit_label3:for(i=0;i<Num_operations;i++){
    #pragma HLS UNROLL factor=2
    		      sum=sum+(in_fifo[i]* weight_fifo[i]);
#ifndef __SYNTHESIS__
  		      cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
#endif
    	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
    	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
    		  }
#ifndef __SYNTHESIS__
   	   	     cout<< "PE3-----------:" << sum << "   " << '\n'<<'\n';
#endif
   	   	//return sum;
   	   	   	   	    if(sum<0)
   	   	            {
   	   	                return 0;
   	   	            }
   	   	            else
   	   	            {
   	   	                return sum;
   	   	            }
    		//  temp1=1.0f +expf(-1 * sum);
    	    //  temp2=1.0f/temp1;

    	    //printf("OUT_N3      =%f \n",temp2);
    	    	//printf("PE3---------------------------------------------------------------------sum=%f \n",sum);
    	 //return temp2;
    }



 //------------------------------------------------------------------------------------------

 //--------------------- PE4_Unit--------------------
fp_type PE4_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
    #pragma HLS INLINE off

    	  fp_type2 sum=0;

    	  fp_type temp1=0;
    	  fp_type temp2=0;

    	  int i;

    		  PE4_Unit_label7:for(i=0;i<Num_operations;i++){
    #pragma HLS UNROLL factor=2
    		      sum=sum+(in_fifo[i]* weight_fifo[i]);
#ifndef __SYNTHESIS__
  		      cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
#endif
    	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
    	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
    		  }
#ifndef __SYNTHESIS__
   	   	     cout<< "PE4-----------:" << sum << "   " << '\n'<<'\n';
#endif
   	   	//return sum;
   	   	   	   	    if(sum<0)
   	   	            {
   	   	                return 0;
   	   	            }
   	   	            else
   	   	            {
   	   	                return sum;
   	   	            }
    		//  temp1=1.0f +expf(-1 * sum);
    	    //  temp2=1.0f/temp1;

    	    //printf("OUT_N4      =%f \n",temp2);
    	    	//printf("PE4---------------------------------------------------------------------sum=%f \n",sum);
    	 //return temp2;
    }



 //------------------------------------------------------------------------------------------

 //--------------------- PE5_Unit--------------------
fp_type PE5_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
    #pragma HLS INLINE off

    	  fp_type2 sum=0;

    	  fp_type temp1=0;
    	  fp_type temp2=0;

    	  int i;

    		  PE5_Unit_label4:for(i=0;i<Num_operations;i++){
    #pragma HLS UNROLL factor=2
    		      sum=sum+(in_fifo[i]* weight_fifo[i]);
#ifndef __SYNTHESIS__
  		      cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
#endif
    	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
    	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
    		  }
#ifndef __SYNTHESIS__
   	   	     cout<< "PE5-----------:" << sum << "   " << '\n'<<'\n';
#endif
   	   	//return sum;
   	   	   	   	    if(sum<0)
   	   	            {
   	   	                return 0;
   	   	            }
   	   	            else
   	   	            {
   	   	                return sum;
   	   	            }
    		//  temp1=1.0f +expf(-1 * sum);
    	    //  temp2=1.0f/temp1;

    	    //printf("OUT_N5      =%f \n",temp2);
    	    	//printf("PE5---------------------------------------------------------------------sum=%f \n",sum);
    	 //return temp2;
    }



 //------------------------------------------------------------------------------------------

 //--------------------- PE6_Unit--------------------
fp_type PE6_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
    #pragma HLS INLINE off

    	  fp_type2 sum=0;

    	  fp_type temp1=0;
    	  fp_type temp2=0;

    	  int i;

    		  PE6_Unit_label5:for(i=0;i<Num_operations;i++){
    #pragma HLS UNROLL factor=2
    		      sum=sum+(in_fifo[i]* weight_fifo[i]);
#ifndef __SYNTHESIS__
  		      cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
#endif
    	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
    	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
    		  }
#ifndef __SYNTHESIS__
   	   	     cout<< "PE6-----------:" << sum << "   " << '\n'<<'\n';
#endif
   	   	//return sum;
   	   	  	   	    if(sum<0)
   	   	            {
   	   	                return 0;
   	   	            }
   	   	            else
   	   	            {
   	   	                return sum;
   	   	            }
    		//  temp1=1.0f +expf(-1 * sum);
    	    //  temp2=1.0f/temp1;

    	    //printf("OUT_N6      =%f \n",temp2);
    	    	//printf("PE6---------------------------------------------------------------------sum=%f \n",sum);
    	 //return temp2;
    }



 //------------------------------------------------------------------------------------------

 //--------------------- PE7_Unit--------------------
fp_type PE7_Unit(fp_type in_fifo[PE_FIFOSIZE], fp_type1 weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
    #pragma HLS INLINE off

    	  fp_type2 sum=0;

    	  fp_type temp1=0;
    	  fp_type temp2=0;

    	  int i;

    		  PE7_Unit_label6:for(i=0;i<Num_operations;i++){
    #pragma HLS UNROLL factor=2
    		      sum=sum+(in_fifo[i]* weight_fifo[i]);
#ifndef __SYNTHESIS__
  		      cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
#endif
    	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
    	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
    		  }
#ifndef __SYNTHESIS__
   	   	     cout<< "PE7-----------:" << sum << "   " << '\n'<<'\n';
#endif

   	   //return sum;
   	   	    if(sum<0)
            {
                return 0;
            }
            else
            {
                return sum;
            }
    		//  temp1=1.0f +expf(-1 * sum);
    	    //  temp2=1.0f/temp1;

    	    //printf("OUT_N7      =%f \n",temp2);
    	    	//printf("PE7---------------------------------------------------------------------sum=%f \n",sum);
    	 //return temp2;
    }



 //------------------------------------------------------------------------------------------
