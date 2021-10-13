#include"Controller_net3.h"
//#include "hls_math.h"

	int    NInputs[MAXNOLAYERS];
	int    NNeurons[MAXNOLAYERS];
	int    NPartions[MAXNOLAYERS]; // How many partitions for each layer?
	int    NExtraPE_Needed[MAXNOLAYERS];



	void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL2> &out_stream, configuration *Config, int new_net){

	static fp2  weights[M_WEIGHT_SIZE];
	fp1  inputs[INPUT_SIZE];
	PE_weights PE_weight;
    PE_inputs<fp1> PE_input;
    PE_outputs<fp6> PE_output;

	int    Lcounter=0;
	int    NOutputs=0;
	int    MaxW=0;
	int w;
	int    i,j,k,R;

	AXI_VAL1 in1;
    AXI_VAL2 out;

	 ////threshold=Config->thresholdv;
	 Lcounter=Config->Nlayers;     // hidden +outputs
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



		//--------------- configure the net-------------------------
		//-------------------seperate inputs from weights--------------
	     INPUTS1: for(i=0;i<NInputs[0];i++)
	     {
					 in_stream >> in1;
					 inputs[i]= in1.data;
#ifndef __SYNTHESIS__
    	 cout << "inputs:" << inputs[i] <<'\n';
#endif

	     }
	     if(new_net==1)
	     	 {
		MAXW:for(j=0;j<Lcounter;j++)
				  MaxW=MaxW+(NInputs[j]*NNeurons[j]);


		WEIGHTS:for(i=0;i<(MaxW);i++)
			 {
				 	 	   in_stream>> in1;
						   weights[i] = in1.data;
			 }

		 //---------Calculate the Number of Partitions to process each layer--------------------
		//		 int N=0;
		//		 int temp=0;
		PartitionL:	for(i=0;i<Lcounter;i++)
					{
					 	NPartions[i]=(int)(NNeurons[i]/ NUM_PE);
						NExtraPE_Needed[i]=NNeurons[i]-(NPartions[i]*NUM_PE);
					}


		}



//-------------------------------end feeding weights-------------------------------------------------

//------------------------------- start feeding inputs-----------------------------------------------
int wightcount=0;


LyrL: for(i=0;i<Lcounter;i++)
      {
            int Partions_plus_Extra=0;
            int OutCounter=0;
            int Counter2=0;
            int Counter3=0;
            int Counter1=0;

        if(i==0)
        {
        	 INPUTS2: for(j=0;j<NInputs[0];j++)
        		     {
        						 ///in_stream >> in1;
        						 PE_input.N0_input[j]= inputs[j];
        		#ifndef __SYNTHESIS__
        						 //cout<<"PE_input.N0_input[" << j << "]="<< PE_input.N0_input[j]<<'\n';
        		#endif
        						 PE_input.N1_input[j]= inputs[j];
        						 PE_input.N2_input[j]= inputs[j];
        						 PE_input.N3_input[j]= inputs[j];
        						 PE_input.N4_input[j]= inputs[j];
        						 PE_input.N5_input[j]= inputs[j];
        						 PE_input.N6_input[j]= inputs[j];
        						 PE_input.N7_input[j]= inputs[j];
        		     }



		 if(NExtraPE_Needed[i] !=0)
         {
            Partions_plus_Extra=NPartions[i]+1;
         }
         else
         {
             Partions_plus_Extra=NPartions[i];
         }

        R1: for(R=0;R<Partions_plus_Extra;R++)
         {
             if(R==NPartions[i]-1)
             {
               K1: for(k=0;k<NPartions[i];k++)
                 {
                    J1: for(j=0;j<NInputs[i];j++)
                     {
					 PE_weight.N0_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

					 PE_weight.N1_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

					 PE_weight.N2_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

					 PE_weight.N3_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

					 PE_weight.N4_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

					 PE_weight.N5_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

					 PE_weight.N6_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

					 PE_weight.N7_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

				}

				 PE_output.PE0_output[k]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				 PE_output.PE1_output[k]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
				 PE_output.PE2_output[k]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
				 PE_output.PE3_output[k]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
				 PE_output.PE4_output[k]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
				 PE_output.PE5_output[k]=PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
				 PE_output.PE6_output[k]=PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
				 PE_output.PE7_output[k]=PE7_Unit<fp6, fp1, fp2>(PE_input.N7_input, PE_weight.N7_weight, NInputs[i]);

			}//end k

         }//end if r==  ///if(R==NPartions[i]-1)

         else if(R==NPartions[i] && NExtraPE_Needed[i] !=0)
         {

		 if(NExtraPE_Needed[i]==1)
          {
                 Ex1: for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==1


		 if(NExtraPE_Needed[i]==2)
          {
               Ex2:   for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N1_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==2


		 if(NExtraPE_Needed[i]==3)
          {
			 Ex3:  for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N1_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N2_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
				PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==3


		 if(NExtraPE_Needed[i]==4)
          {
			 Ex4:    for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N1_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N2_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N3_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
				PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
				PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==4


		 if(NExtraPE_Needed[i]==5)
          {
			 Ex5:    for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N1_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N2_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N3_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N4_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
				PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
				PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
				PE_output.PE4_output[NPartions[i]]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==5


		 if(NExtraPE_Needed[i]==6)
          {
			 Ex6:    for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N1_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N2_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N3_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N4_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N5_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
				PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
				PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
				PE_output.PE4_output[NPartions[i]]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
				PE_output.PE5_output[NPartions[i]]=PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==6


		 if(NExtraPE_Needed[i]==7)
          {
			 Ex7:     for(j=0;j<NInputs[i];j++)
                  {
					 PE_weight.N0_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N1_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N2_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N3_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N4_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N5_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;
					 PE_weight.N6_weight[j]=weights[wightcount];
					 wightcount=wightcount+1;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
				PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
				PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
				PE_output.PE4_output[NPartions[i]]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
				PE_output.PE5_output[NPartions[i]]=PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
				PE_output.PE6_output[NPartions[i]]=PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==7


		} // end for else if R
      	} //end of R
   } //end i==0

 // -----------------------------------------------------------------------------------------------------


		 else if(i<Lcounter)
         {
           int Out_index=NPartions[i-1];
           if(NExtraPE_Needed[i-1] !=0)
           {
               Partions_plus_Extra=NPartions[i-1]+1;
           }
           else
           {
               Partions_plus_Extra=NPartions[i-1];
           }

          R2: for(R=0;R<Partions_plus_Extra;R++)
           {
               if(R==NPartions[i-1]-1)
               {
                 K2:  for(k=0;k<NPartions[i-1];k++)
                   {
				PE_input.N0_input[Counter1]= PE_output.PE0_output[k];
				PE_input.N1_input[Counter1]= PE_output.PE0_output[k];
				PE_input.N2_input[Counter1]= PE_output.PE0_output[k];
				PE_input.N3_input[Counter1]= PE_output.PE0_output[k];
				PE_input.N4_input[Counter1]= PE_output.PE0_output[k];
				PE_input.N5_input[Counter1]= PE_output.PE0_output[k];
				PE_input.N6_input[Counter1]= PE_output.PE0_output[k];
				PE_input.N7_input[Counter1]= PE_output.PE0_output[k];
				Counter1=Counter1+1;
				PE_input.N0_input[Counter1]= PE_output.PE1_output[k];
				PE_input.N1_input[Counter1]= PE_output.PE1_output[k];
				PE_input.N2_input[Counter1]= PE_output.PE1_output[k];
				PE_input.N3_input[Counter1]= PE_output.PE1_output[k];
				PE_input.N4_input[Counter1]= PE_output.PE1_output[k];
				PE_input.N5_input[Counter1]= PE_output.PE1_output[k];
				PE_input.N6_input[Counter1]= PE_output.PE1_output[k];
				PE_input.N7_input[Counter1]= PE_output.PE1_output[k];
				Counter1=Counter1+1;
				PE_input.N0_input[Counter1]= PE_output.PE2_output[k];
				PE_input.N1_input[Counter1]= PE_output.PE2_output[k];
				PE_input.N2_input[Counter1]= PE_output.PE2_output[k];
				PE_input.N3_input[Counter1]= PE_output.PE2_output[k];
				PE_input.N4_input[Counter1]= PE_output.PE2_output[k];
				PE_input.N5_input[Counter1]= PE_output.PE2_output[k];
				PE_input.N6_input[Counter1]= PE_output.PE2_output[k];
				PE_input.N7_input[Counter1]= PE_output.PE2_output[k];
				Counter1=Counter1+1;
				PE_input.N0_input[Counter1]= PE_output.PE3_output[k];
				PE_input.N1_input[Counter1]= PE_output.PE3_output[k];
				PE_input.N2_input[Counter1]= PE_output.PE3_output[k];
				PE_input.N3_input[Counter1]= PE_output.PE3_output[k];
				PE_input.N4_input[Counter1]= PE_output.PE3_output[k];
				PE_input.N5_input[Counter1]= PE_output.PE3_output[k];
				PE_input.N6_input[Counter1]= PE_output.PE3_output[k];
				PE_input.N7_input[Counter1]= PE_output.PE3_output[k];
				Counter1=Counter1+1;
				PE_input.N0_input[Counter1]= PE_output.PE4_output[k];
				PE_input.N1_input[Counter1]= PE_output.PE4_output[k];
				PE_input.N2_input[Counter1]= PE_output.PE4_output[k];
				PE_input.N3_input[Counter1]= PE_output.PE4_output[k];
				PE_input.N4_input[Counter1]= PE_output.PE4_output[k];
				PE_input.N5_input[Counter1]= PE_output.PE4_output[k];
				PE_input.N6_input[Counter1]= PE_output.PE4_output[k];
				PE_input.N7_input[Counter1]= PE_output.PE4_output[k];
				Counter1=Counter1+1;
				PE_input.N0_input[Counter1]= PE_output.PE5_output[k];
				PE_input.N1_input[Counter1]= PE_output.PE5_output[k];
				PE_input.N2_input[Counter1]= PE_output.PE5_output[k];
				PE_input.N3_input[Counter1]= PE_output.PE5_output[k];
				PE_input.N4_input[Counter1]= PE_output.PE5_output[k];
				PE_input.N5_input[Counter1]= PE_output.PE5_output[k];
				PE_input.N6_input[Counter1]= PE_output.PE5_output[k];
				PE_input.N7_input[Counter1]= PE_output.PE5_output[k];
				Counter1=Counter1+1;
				PE_input.N0_input[Counter1]= PE_output.PE6_output[k];
				PE_input.N1_input[Counter1]= PE_output.PE6_output[k];
				PE_input.N2_input[Counter1]= PE_output.PE6_output[k];
				PE_input.N3_input[Counter1]= PE_output.PE6_output[k];
				PE_input.N4_input[Counter1]= PE_output.PE6_output[k];
				PE_input.N5_input[Counter1]= PE_output.PE6_output[k];
				PE_input.N6_input[Counter1]= PE_output.PE6_output[k];
				PE_input.N7_input[Counter1]= PE_output.PE6_output[k];
				Counter1=Counter1+1;
				PE_input.N0_input[Counter1]= PE_output.PE7_output[k];
				PE_input.N1_input[Counter1]= PE_output.PE7_output[k];
				PE_input.N2_input[Counter1]= PE_output.PE7_output[k];
				PE_input.N3_input[Counter1]= PE_output.PE7_output[k];
				PE_input.N4_input[Counter1]= PE_output.PE7_output[k];
				PE_input.N5_input[Counter1]= PE_output.PE7_output[k];
				PE_input.N6_input[Counter1]= PE_output.PE7_output[k];
				PE_input.N7_input[Counter1]= PE_output.PE7_output[k];
				Counter1=Counter1+1;
		} //end for k
 	} // end if R=NPartions[i]-1

        else if(R==NPartions[i-1] && NExtraPE_Needed[i-1] !=0) //Handle the extra neurons
        {
	    if(NExtraPE_Needed[i-1]==1)
              {
			  PE_input.N0_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE0_output[Out_index];
			  Counter1=Counter1+1;
	  } //end else if NExtraPE_Needed[i+1]==1

	    else if(NExtraPE_Needed[i-1]==2)
              {
			  PE_input.N0_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE0_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE1_output[Out_index];
			  Counter1=Counter1+1;
	  } //end else if NExtraPE_Needed[i+1]==2

	    else if(NExtraPE_Needed[i-1]==3)
              {
			  PE_input.N0_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE0_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE1_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE2_output[Out_index];
			  Counter1=Counter1+1;
	  } //end else if NExtraPE_Needed[i+1]==3

	    else if(NExtraPE_Needed[i-1]==4)
              {
			  PE_input.N0_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE0_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE1_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE2_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE3_output[Out_index];
			  Counter1=Counter1+1;
	  } //end else if NExtraPE_Needed[i+1]==4

	    else if(NExtraPE_Needed[i-1]==5)
              {
			  PE_input.N0_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE0_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE1_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE2_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE3_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE4_output[Out_index];
			  Counter1=Counter1+1;
	  } //end else if NExtraPE_Needed[i+1]==5

	    else if(NExtraPE_Needed[i-1]==6)
              {
			  PE_input.N0_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE0_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE1_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE2_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE3_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE4_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE5_output[Out_index];
			  Counter1=Counter1+1;
	  } //end else if NExtraPE_Needed[i+1]==6

	    else if(NExtraPE_Needed[i-1]==7)
              {
			  PE_input.N0_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE0_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE0_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE1_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE1_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE2_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE2_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE3_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE3_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE4_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE4_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE5_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE5_output[Out_index];
			  Counter1=Counter1+1;
			  PE_input.N0_input[Counter1]= PE_output.PE6_output[Out_index];
			  PE_input.N1_input[Counter1]= PE_output.PE6_output[Out_index];
			  PE_input.N2_input[Counter1]= PE_output.PE6_output[Out_index];
			  PE_input.N3_input[Counter1]= PE_output.PE6_output[Out_index];
			  PE_input.N4_input[Counter1]= PE_output.PE6_output[Out_index];
			  PE_input.N5_input[Counter1]= PE_output.PE6_output[Out_index];
			  PE_input.N6_input[Counter1]= PE_output.PE6_output[Out_index];
			  PE_input.N7_input[Counter1]= PE_output.PE6_output[Out_index];
			  Counter1=Counter1+1;
	  } //end else if NExtraPE_Needed[i+1]==7

	} //end  else if(R==NPartions[i] && NExtraPE_Needed[i] !=0)
      } // end for(R=0;R<Partions_plus_Extra;R++)

	if(NExtraPE_Needed[i] !=0)
      {
           Partions_plus_Extra=NPartions[i]+1;
      }
      else
      {
           Partions_plus_Extra=NPartions[i];
      }
     R3: for(R=0;R<Partions_plus_Extra;R++)
      {
           if(R==NPartions[i]-1)
           {
             K3:  for(k=0;k<NPartions[i];k++)
               {
                 J3:  for(j=0;j<NInputs[i];j++)
                   {
				 PE_weight.N0_weight[j]=weights[wightcount];
				 wightcount=wightcount+1;
				 PE_weight.N1_weight[j]=weights[wightcount];
				 wightcount=wightcount+1;
				 PE_weight.N2_weight[j]=weights[wightcount];
				 wightcount=wightcount+1;
				 PE_weight.N3_weight[j]=weights[wightcount];
				 wightcount=wightcount+1;
				 PE_weight.N4_weight[j]=weights[wightcount];
				 wightcount=wightcount+1;
				 PE_weight.N5_weight[j]=weights[wightcount];
				 wightcount=wightcount+1;
				 PE_weight.N6_weight[j]=weights[wightcount];
				 wightcount=wightcount+1;
				 PE_weight.N7_weight[j]=weights[wightcount];
				 wightcount=wightcount+1;
				}
				 PE_output.PE0_output[k]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
				 PE_output.PE1_output[k]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
				 PE_output.PE2_output[k]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]);
				 PE_output.PE3_output[k]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]);
				 PE_output.PE4_output[k]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input,PE_weight.N4_weight,NInputs[i]);
				 PE_output.PE5_output[k]=PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input,PE_weight.N5_weight,NInputs[i]);
				 PE_output.PE6_output[k]=PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input,PE_weight.N6_weight,NInputs[i]);
				 PE_output.PE7_output[k]=PE7_Unit<fp6, fp1, fp2>(PE_input.N7_input,PE_weight.N7_weight,NInputs[i]);
			}//end k

    }//end if r==  ///if(R==NPartions[i]-1)

    else if(R==NPartions[i] && NExtraPE_Needed[i] !=0)
    {
		 if(NExtraPE_Needed[i]==1)
              {
			 Ex8:   for(j=0;j<NInputs[i];j++)
                      {
			PE_weight.N0_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
		  }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==1
		 else if(NExtraPE_Needed[i]==2)
              {
			 Ex9:     for(j=0;j<NInputs[i];j++)
                      {
			PE_weight.N0_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N1_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
		  }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
			PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==2
		 else if(NExtraPE_Needed[i]==3)
              {
			 Ex10:     for(j=0;j<NInputs[i];j++)
                      {
			PE_weight.N0_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N1_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N2_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
		  }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
			PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
			PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==3
		 else if(NExtraPE_Needed[i]==4)
              {
			 Ex11:    for(j=0;j<NInputs[i];j++)
                      {
			PE_weight.N0_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N1_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N2_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N3_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
		  }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
			PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
			PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]);
			PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==4
		 else if(NExtraPE_Needed[i]==5)
              {
			 Ex12:     for(j=0;j<NInputs[i];j++)
                      {
			PE_weight.N0_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N1_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N2_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N3_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N4_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
		  }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
			PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
			PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]);
			PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]);
			PE_output.PE4_output[NPartions[i]]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input,PE_weight.N4_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==5
		 else if(NExtraPE_Needed[i]==6)
              {
			 Ex13:    for(j=0;j<NInputs[i];j++)
                      {
			PE_weight.N0_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N1_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N2_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N3_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N4_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N5_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
		  }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
			PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
			PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]);
			PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]);
			PE_output.PE4_output[NPartions[i]]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input,PE_weight.N4_weight,NInputs[i]);
			PE_output.PE5_output[NPartions[i]]=PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input,PE_weight.N5_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==6
		 else if(NExtraPE_Needed[i]==7)
              {
			 Ex14:    for(j=0;j<NInputs[i];j++)
                      {
			PE_weight.N0_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N1_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N2_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N3_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N4_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N5_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
			PE_weight.N6_weight[j]=weights[wightcount];
			wightcount=wightcount+1;
		  }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
			PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
			PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]);
			PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]);
			PE_output.PE4_output[NPartions[i]]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input,PE_weight.N4_weight,NInputs[i]);
			PE_output.PE5_output[NPartions[i]]=PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input,PE_weight.N5_weight,NInputs[i]);
			PE_output.PE6_output[NPartions[i]]=PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input,PE_weight.N6_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==7
	} // end for else if R
   } //end of R
} //end i<nLayer

 //-----------------------------------------------------------------------------------------------------

 if(i==Lcounter-1){// push the outputs to the axi bus
      if(NExtraPE_Needed[i] !=0){// increase the number of partitions if there is an extra
           Partions_plus_Extra=NPartions[i]+1;
      }
      else{
          Partions_plus_Extra=NPartions[i];// if there is no extra the number of partitions stays same
      }
      Counter3=0;
      R4: for(R=0;R<Partions_plus_Extra;R++){
              if(R==NPartions[i]-1){// we stack the outputs of PEs in the)
                    K4: for(k=0;k<NPartions[i];k++)
					{
						 out.data = PE_output.PE0_output[k];
						 out.keep = 3;
						 out.last = (OutCounter+0==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE1_output[k];
						 out.keep = 3;
						 out.last = (OutCounter+1==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE2_output[k];
						 out.keep = 3;
						 out.last = (OutCounter+2==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE3_output[k];
						 out.keep = 3;
						 out.last = (OutCounter+3==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE4_output[k];
						 out.keep = 3;
						 out.last = (OutCounter+4==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE5_output[k];
						 out.keep = 3;
						 out.last = (OutCounter+5==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE6_output[k];
						 out.keep = 3;
						 out.last = (OutCounter+6==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE7_output[k];
						 out.keep = 3;
						 out.last = (OutCounter+7==NOutputs-1);
						 out_stream.write(out);


						 OutCounter=OutCounter+8;
				} //end for(k=0;k<NPartions[i];k++)
      		} // end if(R==NPartions[i]-1)
      else if(R==NPartions[i] && NExtraPE_Needed[i] !=0){
	 if(NExtraPE_Needed[i]==1)
          {
			out.data = PE_output.PE0_output[NPartions[i]];
			out.keep = 3;
            out.last = (OutCounter+0==NOutputs-1);
			out_stream.write(out);

			OutCounter=OutCounter+1;;
		  } //end else if NExtraPE_Needed[i]==1
	 else if(NExtraPE_Needed[i]==2)
          {
			out.data = PE_output.PE0_output[NPartions[i]];
			out.keep = 3;
            out.last = (OutCounter+0==NOutputs-1);
            out_stream.write(out);

			out.data = PE_output.PE1_output[NPartions[i]];
			out.keep = 3;
			out.last = (OutCounter+1==NOutputs-1);
			out_stream.write(out);

			OutCounter=OutCounter+2;
		  } //end else if NExtraPE_Needed[i]==2
	 else if(NExtraPE_Needed[i]==3)
          {
			out.data = PE_output.PE0_output[NPartions[i]];
			out.keep = 3;
				out.last = (OutCounter+0==NOutputs-1);
				out_stream.write(out);
			out.data = PE_output.PE1_output[NPartions[i]];
			out.keep = 3;
				out.last = (OutCounter+1==NOutputs-1);
				out_stream.write(out);
			out.data = PE_output.PE2_output[NPartions[i]];
			out.keep = 3;
				out.last = (OutCounter+2==NOutputs-1);
				out_stream.write(out);

				OutCounter=OutCounter+3;
		  } //end else if NExtraPE_Needed[i]==3
	 else if(NExtraPE_Needed[i]==4)
          {
			 out.data = PE_output.PE0_output[NPartions[i]];
			 out.keep = 3;
				out.last = (OutCounter+0==NOutputs-1);
				out_stream.write(out);

			 out.data = PE_output.PE1_output[NPartions[i]];
			 out.keep = 3;
				out.last = (OutCounter+1==NOutputs-1);
				out_stream.write(out);

			 out.data = PE_output.PE2_output[NPartions[i]];
			 out.keep = 3;
				out.last = (OutCounter+2==NOutputs-1);
				out_stream.write(out);

			 out.data = PE_output.PE3_output[NPartions[i]];
			 out.keep = 3;
				out.last = (OutCounter+3==NOutputs-1);
    			out_stream.write(out);

				OutCounter=OutCounter+4;
			} //end else if NExtraPE_Needed[i]==4
	 else if(NExtraPE_Needed[i]==5)
          {
			 out.data = PE_output.PE0_output[NPartions[i]];
			 out.keep = 3;
				out.last = (OutCounter+0==NOutputs-1);
				out_stream.write(out);

			 out.data = PE_output.PE1_output[NPartions[i]];
			 out.keep = 3;
				out.last = (OutCounter+1==NOutputs-1);
				out_stream.write(out);

			 out.data = PE_output.PE2_output[NPartions[i]];
			 out.keep = 3;
				out.last = (OutCounter+2==NOutputs-1);
				out_stream.write(out);

			 out.data = PE_output.PE3_output[NPartions[i]];
			 out.keep = 3;
				out.last = (OutCounter+3==NOutputs-1);
				out_stream.write(out);

			 out.data = PE_output.PE4_output[NPartions[i]];
			 out.keep = 3;
				out.last = (OutCounter+4==NOutputs-1);
				out_stream.write(out);

				OutCounter=OutCounter+5;
			} //end else if NExtraPE_Needed[i]==5
	 else if(NExtraPE_Needed[i]==6)
          {
				 out.data = PE_output.PE0_output[NPartions[i]];
				 out.keep = 3;
                 out.last = (OutCounter+0==NOutputs-1);
    			 out_stream.write(out);

				 out.data = PE_output.PE1_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);

				 out.data = PE_output.PE2_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);

				 out.data = PE_output.PE3_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);

				 out.data = PE_output.PE4_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+4==NOutputs-1);
    				out_stream.write(out);

				 out.data = PE_output.PE5_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+5==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+6;
		  } //end else if NExtraPE_Needed[i]==6
	 else if(NExtraPE_Needed[i]==7)
          {
				 out.data = PE_output.PE0_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);

				 out.data = PE_output.PE1_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);

				 out.data = PE_output.PE2_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);

				 out.data = PE_output.PE3_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);

				 out.data = PE_output.PE4_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+4==NOutputs-1);
    				out_stream.write(out);

				 out.data = PE_output.PE5_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+5==NOutputs-1);
    				out_stream.write(out);

				 out.data = PE_output.PE6_output[NPartions[i]];
				 out.keep = 3;
                    out.last = (OutCounter+6==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+7;
			} //end else if NExtraPE_Needed[i]==7
	} //end  else if(R==NPartions[i] && NExtraPE_Needed[i] !=0)
    } // end for(R=0;R<Partions_plus_Extra;R++)
   }//	else if(i==Lcounter-1)
 }//  end for(i=0;i<Lcounter;i++)
  return;

}
//---------------------------------------------------------------------------------------------------------------------------


 //--------------------- PE0_Unit--------------------
template<class T, class U, class V>
T PE0_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
   #pragma HLS INLINE off

   	  T sum=0;

	  int i;

   	  PE0_Unit:for(i=0;i<Num_operations;i++)
   	  	  	  	  	  {
   	   	   	   	   	   	   //#pragma HLS UNROLL factor=2
   		      	  	  	   sum=sum+(in_fifo[i]* weight_fifo[i]);

   	 #ifndef __SYNTHESIS__
 	     // cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
      #endif
   	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
   	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
   	  	  	  	  	  }

   	  #ifndef __SYNTHESIS__
  	   	     cout<< "PE0-----------:" << sum << "   " << '\n'<<'\n';
	  #endif

  	   	return RELU<T,T>(sum);
};
//---------------------------------------------------------------------------------------------------------------------------


 //--------------------- PE1_Unit--------------------
template<class T, class U, class V>
T PE1_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
   #pragma HLS INLINE off

   	  T sum=0;

	  int i;

   	  PE1_Unit:for(i=0;i<Num_operations;i++)
   	  	  	  	  	  {
   	   	   	   	   	   	   //#pragma HLS UNROLL factor=2
   		      	  	  	   sum=sum+(in_fifo[i]* weight_fifo[i]);

   	 #ifndef __SYNTHESIS__
 	     // cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
      #endif
   	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
   	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
   	  	  	  	  	  }

   	  #ifndef __SYNTHESIS__
  	   	     cout<< "PE1-----------:" << sum << "   " << '\n'<<'\n';
	  #endif

  	   	return RELU<T,T>(sum);
};
//---------------------------------------------------------------------------------------------------------------------------


 //--------------------- PE2_Unit--------------------
template<class T, class U, class V>
T PE2_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
   #pragma HLS INLINE off

   	  T sum=0;

	  int i;

   	  PE2_Unit:for(i=0;i<Num_operations;i++)
   	  	  	  	  	  {
   	   	   	   	   	   	   //#pragma HLS UNROLL factor=2
   		      	  	  	   sum=sum+(in_fifo[i]* weight_fifo[i]);

   	 #ifndef __SYNTHESIS__
 	     // cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
      #endif
   	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
   	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
   	  	  	  	  	  }

   	  #ifndef __SYNTHESIS__
  	   	     cout<< "PE2-----------:" << sum << "   " << '\n'<<'\n';
	  #endif

  	   	return RELU<T,T>(sum);
};
//---------------------------------------------------------------------------------------------------------------------------



 //--------------------- PE3_Unit--------------------
template<class T, class U, class V>
T PE3_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
   #pragma HLS INLINE off

   	  T sum=0;

	  int i;

   	  PE3_Unit:for(i=0;i<Num_operations;i++)
   	  	  	  	  	  {
   	   	   	   	   	   	   //#pragma HLS UNROLL factor=2
   		      	  	  	   sum=sum+(in_fifo[i]* weight_fifo[i]);

   	 #ifndef __SYNTHESIS__
 	     // cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
      #endif
   	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
   	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
   	  	  	  	  	  }

   	  #ifndef __SYNTHESIS__
  	   	     cout<< "PE3-----------:" << sum << "   " << '\n'<<'\n';
	  #endif

  	   	return RELU<T,T>(sum);
};
//---------------------------------------------------------------------------------------------------------------------------


 //--------------------- PE4_Unit--------------------
template<class T, class U, class V>
T PE4_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
   #pragma HLS INLINE off

   	  T sum=0;

	  int i;

   	  PE4_Unit:for(i=0;i<Num_operations;i++)
   	  	  	  	  	  {
   	   	   	   	   	   	   //#pragma HLS UNROLL factor=2
   		      	  	  	   sum=sum+(in_fifo[i]* weight_fifo[i]);

   	 #ifndef __SYNTHESIS__
 	     // cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
      #endif
   	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
   	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
   	  	  	  	  	  }

   	  #ifndef __SYNTHESIS__
  	   	     cout<< "PE4-----------:" << sum << "   " << '\n'<<'\n';
	  #endif

  	   	return RELU<T,T>(sum);
};
//---------------------------------------------------------------------------------------------------------------------------


 //--------------------- PE5_Unit--------------------
template<class T, class U, class V>
T PE5_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
   #pragma HLS INLINE off

   	  T sum=0;

	  int i;

   	  PE5_Unit:for(i=0;i<Num_operations;i++)
   	  	  	  	  	  {
   	   	   	   	   	   	  // #pragma HLS UNROLL factor=2
   		      	  	  	   sum=sum+(in_fifo[i]* weight_fifo[i]);

   	 #ifndef __SYNTHESIS__
 	     // cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
      #endif
   	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
   	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
   	  	  	  	  	  }

   	  #ifndef __SYNTHESIS__
  	   	     cout<< "PE5-----------:" << sum << "   " << '\n'<<'\n';
	  #endif

  	   	return RELU<T,T>(sum);
};
//---------------------------------------------------------------------------------------------------------------------------

//--------------------- PE6_Unit--------------------
template<class T, class U, class V>
T PE6_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
   #pragma HLS INLINE off

   	  T sum=0;

	  int i;

   	  PE6_Unit:for(i=0;i<Num_operations;i++)
   	  	  	  	  	  {
   	   	   	   	   	   	 //  #pragma HLS UNROLL factor=2
   		      	  	  	   sum=sum+(in_fifo[i]* weight_fifo[i]);

   	 #ifndef __SYNTHESIS__
 	     // cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
      #endif
   	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
   	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
   	  	  	  	  	  }

   	  #ifndef __SYNTHESIS__
  	   	     cout<< "PE6-----------:" << sum << "   " << '\n'<<'\n';
	  #endif

  	   	return RELU<T,T>(sum);
};
//---------------------------------------------------------------------------------------------------------------------------

//--------------------- PE7_Unit--------------------
template<class T, class U, class V>
T PE7_Unit(U in_fifo[PE_FIFOSIZE], V weight_fifo[PE_WEIGHT_SIZE], int Num_operations){
   #pragma HLS INLINE off

   	  T sum=0;

	  int i;

   	  PE7_Unit:for(i=0;i<Num_operations;i++)
   	  	  	  	  	  {
   	   	   	   	   	   	  // #pragma HLS UNROLL factor=2
   		      	  	  	   sum=sum+(in_fifo[i]* weight_fifo[i]);

   	 #ifndef __SYNTHESIS__
 	     // cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
      #endif
   	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
   	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
   	  	  	  	  	  }

   	  #ifndef __SYNTHESIS__
  	   	     cout<< "PE7-----------:" << sum << "   " << '\n'<<'\n';
	  #endif

  	   	return RELU<T,T>(sum);
};
//---------------------------------------------------------------------------------------------------------------------------



template<class T, class U>
T RELU(U x)
{
	if(x<0)
		return 0;
	else
		return x;
};

 //------------------------------------------------------------------------------------------

