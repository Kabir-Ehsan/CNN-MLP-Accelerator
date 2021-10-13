#include"Controller_net3.h"

void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL2> &out_stream, configuration *Config, int new_net)
{
	int w, i, j, k, l, m;
	int    Lcounter=0;
	int    NOutputs=0;
	int    MaxW=0;
	w = 0;
	
	fp2  weights[M_WEIGHT_SIZE];
	fp1 inputs[INPUT_SIZE];
	fp1 sum[INPUT_SIZE];

	int NInputs[MAXNOLAYERS];
	int NNeurons[MAXNOLAYERS];

	AXI_VAL1 in;
    AXI_VAL2 out;

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

/*	 assert(Lcounter<=5);

	 assert(NInputs[0]<=784);
	 assert(NInputs[1]<=784);
	 assert(NInputs[2]<=784);
	 assert(NInputs[3]<=784);
	 assert(NInputs[4]<=784);

	 assert(NNeurons[0]<=784);
	 assert(NNeurons[1]<=784);
	 assert(NNeurons[2]<=784);
	 assert(NNeurons[3]<=784);
	 assert(NNeurons[4]<=784);*/

//    INPUTS: for(i=0;i<Config->Ninputs;i++)
    INPUTS: for(i=0;i<INPUT_SIZE;i++)
    {
    	if(i<NInputs[0])
    	{
				 in_stream >> in;
				 inputs[i]= in.data;
#ifndef __SYNTHESIS__
//				 cout<<"PE_input.N0_input[" << i << "]="<< PE_input.N0_input[i]<<'\n';
#endif
    	}
    	else
    		break;
    }

    if(new_net==1)
    {

    		//MAXW:for(j=0;j<Lcounter;j++)
    		MAXW:for(j=0;j<MAXNOLAYERS;j++)
    		{
    			if(j<Lcounter)
    			{
    				  MaxW=MaxW+(NInputs[j]*NNeurons[j]);
    			}
    			else
    				break;
    		}
    		//assert(MaxW<=M_WEIGHT_SIZE);
    		//WEIGHTS:for(i=0;i<(MaxW);i++)
    		WEIGHTS:for(i=0;i<M_WEIGHT_SIZE;i++)
    		{
    			if(i<MaxW)
    			{
    				in_stream>> in;
    				weights[i] = in.data;
    			}
    			else
    				break;
    		}
    }


//	Layer: for(i=0;i<Lcounter;i++)
	Layer: for(i=0;i<MAXNOLAYERS;i++)
	{
		if(i<Lcounter)
		{
	//	Node:for(j=0;j<NNeurons[i];j++)
		Node:for(j=0;j<INPUT_SIZE;j++)
			{
				if(j<NNeurons[i])
				{
					sum[j] = 0;
		//		Mac:for(l=0;l<NInputs[i];l++)
					Mac:for(l=0;l<INPUT_SIZE;l++)
					{
						if(l<NInputs[i])
						{
							sum[j] = sum[j]+inputs[l]*weights[w];
							w++;
						}
						else
							break;
					}
				}
				else
					break;

			}
	//	Copy:for(k=0;k<NNeurons[i];k++)
		Copy:for(k=0;k<INPUT_SIZE;k++)
			 {
				if(k<NNeurons[i])
				{
					inputs[k] = RELU<fp1,fp1>(sum[k]);
				}
				else
					break;
			 }
		}
		else
			break;
	}

//	OUT:for(m=0;m<NNeurons[i-1];m++)
	OUT:for(m=0;m<OUTPUT_SIZE;m++)
			{
				if(m<NNeurons[i-1])
				{
					out.data = inputs[m];
					out.last = (m==NOutputs-1);
					out_stream.write(out);
				}
				else
					break;
			}

}

template<class T, class U>
T RELU(U x)
{
	if(x<0)
		return 0;
	else
		return x;
};
