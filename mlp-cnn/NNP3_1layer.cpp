#include"Controller_net_upgrade1.h"
//#include "hls_math.h"

	uint16  NInputs[MAXNOLAYERS];
	uint16  NNeurons[MAXNOLAYERS];
	uint16  NPartions[MAXNOLAYERS]; // How many partitions for each layer?
	uint16  NExtraPE_Needed[MAXNOLAYERS];

	uint8 	inChannels[MAXConvLAYERS];
	uint8 	filterNo[MAXConvLAYERS];
	uint8 	stride[MAXConvLAYERS];
	uint8	KSIZE[MAXConvLAYERS];
	uint8   convOut1D[MAXConvLAYERS];
	uint8 	NPartionsC[MAXConvLAYERS];
	uint8 	NExtraPE_NeededC[MAXConvLAYERS];
	uint8	kernelSize[MAXConvLAYERS];
	uint8 	imageSize[MAXConvLAYERS];
	int		filterSizeTotal[MAXConvLAYERS];

	void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL2> &out_stream, configuration *Config, int new_net, int conv){



	PE_weights PE_weight;

	PE_inputs<fp1> PE_input;

	PE_outputs<fp6> PE_output;

    fp2  weights[M_WEIGHT_SIZE];

    fp2  kernels[ksize];

    fp2  tempKernel[ksize];

    fp1 inputs[INPUT_SIZE];


	uint8   Lcounter=0;
	uint8   NOutputs=0;
	
	
	uint8	channels=0;
	uint8 	stride=0;
	uint8 	padding=0;
	uint8   KSIZE = 0;

	uint8	convLayer = 0;
	uint16 	convOut = 0;
	uint16	filter=0;

	int    	MaxW=0;
	int 	wightcount=0;
	int 	inCount=0;
	int 	outCount=0;
	int 	inputCount=0;
	int 	copyCount=0;
	int 	count=0;
	int 	count1=0;
	int 	count2=0;
	int    	c,f,i,j,k,l,m,n,o,p,q,R,s,w;



	AXI_VAL1 in1;
    AXI_VAL2 out;

	 ////threshold=Config->thresholdv;
	 Lcounter=Config->Nlayers;     // hidden +outputs
	 NOutputs=Config->Noutputs;

	 imageSize=Config->imageSize;
	 convOut = Config->convOut;
	 kernelSize = Config->KernelSize;
	 channels = Config->channels;
	 stride = Config->stride;
	 padding = Config->padding;
	 KSIZE = Config->KSIZE;
	 convOut1D = Config->convOut1D;
	 convLayer = Config->convLayer;
	 filter = Config->filter;

	 if(conv==1)
	 {
		 NInputs[0]=Config->convOut;
	 }
	 else
	 {
		 NInputs[0]=Config->Ninputs;
	 }

	 NInputs[1]=Config->NnuronsH1;
	 NInputs[2]=Config->NnuronsH2;
	 NInputs[3]=Config->NnuronsH3;
	 NInputs[4]=Config->NnuronsH4;

	 NNeurons[0]=Config->NnuronsH1;
	 NNeurons[1]=Config->NnuronsH2;
	 NNeurons[2]=Config->NnuronsH3;
	 NNeurons[3]=Config->NnuronsH4;
	 NNeurons[Lcounter-1]=NOutputs;

#ifndef __SYNTHESIS__
	 cout<< "ksize:"<< ksize <<'\n';
	 cout<< "NInputs[0]:"<< NInputs[0] <<'\n';
#endif
     INPUTS: for(i=0;i<INPUT_SIZE;i++)
     {
    	 if(i<Config->Ninputs)
    	 {
				 in_stream >> in1;
				 inputs[i] = in1.data;
    	 }
    	 else
    		 break;
				// cout<< "inputs:"<<i<<":"<< inputs[i] <<'\n';
     }


	 if(new_net==1)
	 {
		MAXW:for(j=0;j<MAXNOLAYERS;j++)
		{
			if(j<Lcounter)
			{
				MaxW=MaxW+(NInputs[j]*NNeurons[j]);
			}
			else
				break;
		}
#ifndef __SYNTHESIS__
		cout<< "MaxW:"<< MaxW <<'\n';
#endif
		WEIGHTS:for(i=0;i<M_WEIGHT_SIZE;i++)
		{
				if(i<MaxW)
				{
					in_stream>> in1;
					weights[i] = in1.data;
				}
				else
					break;
				// cout<< "weights"<<i<<":"<< weights[i] <<'\n';
		}
		PartitionL:for(i=0;i<MAXNOLAYERS;i++)
		{
			if(i<Lcounter)
			{
					NPartions[i]=(uint16)(NNeurons[i]/ NUM_PE);
					NExtraPE_Needed[i]=NNeurons[i]-(NPartions[i]*NUM_PE);
#ifndef __SYNTHESIS__
					cout<< "NPartions:"<< i << ":"<< NPartions[i] <<'\n';
					cout<< "NExtraPE_Needed:"<< i << ":" << NExtraPE_Needed[i] <<'\n';
#endif
			}
			else
				break;
		}

		 if(conv==1)
		 {

			KERNELS: for(i=0;i<ksize;i++)
					{
						if(i<KSIZE)
						{
							in_stream >> in1;
							kernels[i] = in1.data;
						}
						else
							break;
						// cout<< "kernels:"<<i<<":"<< kernels[i]<<'\n';
					}
	PartitionC:for(i=0;i<MAXConvLAYERS;i++)
		{
			NPartionsC[i]=(uint8)(convOut1D[i]/NUM_PE);
			NExtraPE_NeededC[i]=convOut1D[i]-(NPartionsC[i]*NUM_PE);
	#ifndef __SYNTHESIS__
			cout<< "NPartionsC:"<< NPartionsC[i] <<'\n';
			cout<< "NExtraPE_NeededC:"<< NExtraPE_NeededC[i] <<'\n';
	#endif
		}
	}
}

//-------------------------------end feeding weights-------------------------------------------------

//------------------------------- start feeding inputs-----------------------------------------------


if (conv==1)
{
	layer:for(l=0;l<convLayer;l++)
	{
		filter:for(f=0;f<filterNo[l];f++)
		{
			channels:for(c=0;c<inChannels[l];c++)
			{
				cacheKernel: for(s=0;s<kernelSize[l];s++)
				{
					tempKernel[s] = kernels[s+c*kernelSize[l]+f*filterSizeTotal[l]];
				}
				offset:for(n=0;n<MaxConvOut1D;n++)
				{
					if(n<convOut1D[l])
					{
						p1:for(p=0;p<NPc;p++)
						{
							if(p<NPartionsC[l])
							{
								inputCount=0;
								row: for(i=0;i<K;i++)
								{
									if(i<kernelSize[l])
									{
										col:for(j=0;j<K;j++)
										{
											if(j<kernelSize[l])
											{
												inCount = i*imageSize[l]+j+p*(kernelSize[l])+n*imageSize[l]*stride[l]+c*imageSize[l]*2;
												PE_input.N0_input[inputCount]= inputs[inCount+stride[l]*0];
												PE_input.N1_input[inputCount]= inputs[inCount+stride[l]*1];
												PE_input.N2_input[inputCount]= inputs[inCount+stride[l]*2];
												PE_input.N3_input[inputCount]= inputs[inCount+stride[l]*3];
												PE_input.N4_input[inputCount]= inputs[inCount+stride[l]*4];
												PE_input.N5_input[inputCount]= inputs[inCount+stride[l]*5];
												PE_input.N6_input[inputCount]= inputs[inCount+stride[l]*6];
												PE_input.N7_input[inputCount]= inputs[inCount+stride[l]*7];
												inputCount++;
											}
											else
												break;
										}
									}
									else
										break;

								}


									outCount = p+n+count+f*filterNo[l];
			#ifndef __SYNTHESIS__

								cout<< "PE_output.PE0_output:"<< PE_output.PE0_output[outCount] <<'\n';
			#endif
									PE_output.PE0_output[outCount]=PE_output.PE0_output[outCount]+PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, kernels, ksize);
									PE_output.PE1_output[outCount]=PE_output.PE1_output[outCount]+PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, kernels, ksize);
									PE_output.PE2_output[outCount]=PE_output.PE2_output[outCount]+PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, kernels, ksize);
									PE_output.PE3_output[outCount]=PE_output.PE3_output[outCount]+PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, kernels, ksize);
									PE_output.PE4_output[outCount]=PE_output.PE4_output[outCount]+PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, kernels, ksize);
									PE_output.PE5_output[outCount]=PE_output.PE5_output[outCount]+PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, kernels, ksize);
									PE_output.PE6_output[outCount]=PE_output.PE6_output[outCount]+PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input, kernels, ksize);
									PE_output.PE7_output[outCount]=PE_output.PE7_output[outCount]+PE7_Unit<fp6, fp1, fp2>(PE_input.N7_input, kernels, ksize);

							}
			
							else
								break;
							

						}

						inputCount=0;

						if(NExtraPE_NeededC!=0)
						{
						  if(NExtraPE_NeededC==1)
						  {
								ExR1:for(i=0;i<K;i++)
								{
									if(i<kernelSize)
									{
										ExC1:for(j=0;j<K;j++)
										{
											if(j<kernelSize)
											{
												inCount = i*imageSize+j+p*(kernelSize)+n*imageSize*stride+c*imageSize*2;
												PE_input.N0_input[inputCount]= inputs[inCount+stride*0];
												inputCount++;
											}
											else
												break;
										}
									}
									else
										break;
								}
								outCount = p+n+count+f*filter;
								PE_output.PE0_output[outCount]=PE_output.PE0_output[outCount]+PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, kernels, ksize);
						  }
						  else if(NExtraPE_NeededC==2)
						  {
							  ExR2:for(i=0;i<K;i++)
								{
								  if(i<kernelSize)
								  {
									ExC2:for(j=0;j<K;j++)
									{
									  if(i<kernelSize)
									  {
										inCount = i*imageSize+j+p*(kernelSize)+n*imageSize*stride+c*imageSize*2;
										PE_input.N0_input[inputCount]= inputs[inCount+stride*0];
										PE_input.N1_input[inputCount]= inputs[inCount+stride*1];
										inputCount++;
									  }
									  else
										  break;
									}
								  }
								  break;
								}
								outCount = p+n+count+f*filter;
								PE_output.PE0_output[outCount]=PE_output.PE0_output[outCount]+PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, kernels, ksize);
								PE_output.PE1_output[outCount]=PE_output.PE1_output[outCount]+PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, kernels, ksize);
						  }

						  else if(NExtraPE_NeededC==3)
						  {
							  ExR3:for(i=0;i<K;i++)
								{
								  if(i<kernelSize)
								  {
								  ExC3:for(j=0;j<K;j++)
									{
									  if(i<kernelSize)
									  {
											inCount = i*imageSize+j+p*(kernelSize)+n*imageSize*stride+c*imageSize*2;
											PE_input.N0_input[inputCount]= inputs[inCount+stride*0];
											PE_input.N1_input[inputCount]= inputs[inCount+stride*1];
											PE_input.N2_input[inputCount]= inputs[inCount+stride*2];
											inputCount++;
									  }
									  else
										   break;
									}
								  }
								  else
									   break;
								}
								outCount = p+n+count+f*filter;
								PE_output.PE0_output[outCount]=PE_output.PE0_output[outCount]+PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, kernels, ksize);
								PE_output.PE1_output[outCount]=PE_output.PE1_output[outCount]+PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, kernels, ksize);
								PE_output.PE2_output[outCount]=PE_output.PE2_output[outCount]+PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, kernels, ksize);
						  }
						  else if(NExtraPE_NeededC==4)
						  {
							  ExR4:for(i=0;i<K;i++)
								{
								  if(i<kernelSize)
								  {
								  ExC4:for(j=0;j<K;j++)
									{
									  if(i<kernelSize)
									  {
										inCount = i*imageSize+j+p*(kernelSize)+n*imageSize*stride+c*imageSize*2;
										PE_input.N0_input[inputCount]= inputs[inCount+stride*0];
										PE_input.N1_input[inputCount]= inputs[inCount+stride*1];
										PE_input.N2_input[inputCount]= inputs[inCount+stride*2];
										PE_input.N3_input[inputCount]= inputs[inCount+stride*3];
										inputCount++;
									  }
									  else
										  break;
									}
								  }
								  else
									  break;
								}
								outCount = p+n+count+f*filter;
								PE_output.PE0_output[outCount]=PE_output.PE0_output[outCount]+PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, kernels, ksize);
								PE_output.PE1_output[outCount]=PE_output.PE1_output[outCount]+PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, kernels, ksize);
								PE_output.PE2_output[outCount]=PE_output.PE2_output[outCount]+PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, kernels, ksize);
								PE_output.PE3_output[outCount]=PE_output.PE3_output[outCount]+PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, kernels, ksize);
						  }

						  else if(NExtraPE_NeededC==5)
						  {
							  ExR5:for(i=0;i<K;i++)
								{
								  if(i<kernelSize)
								  {
								  ExC5:for(j=0;j<K;j++)
									{
									  if(i<kernelSize)
									  {
										inCount = i*imageSize+j+p*(kernelSize)+n*imageSize*stride+c*imageSize*2;
										PE_input.N0_input[inputCount]= inputs[inCount+stride*0];
										PE_input.N1_input[inputCount]= inputs[inCount+stride*1];
										PE_input.N2_input[inputCount]= inputs[inCount+stride*2];
										PE_input.N3_input[inputCount]= inputs[inCount+stride*3];
										PE_input.N4_input[inputCount]= inputs[inCount+stride*4];
										inputCount++;
									  }
									  else
										  break;
									  
									}
								  }
								  else
									  break;
								}
								outCount = p+n+count+f*filter;
								PE_output.PE0_output[outCount]=PE_output.PE0_output[outCount]+PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, kernels, ksize);
								PE_output.PE1_output[outCount]=PE_output.PE1_output[outCount]+PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, kernels, ksize);
								PE_output.PE2_output[outCount]=PE_output.PE2_output[outCount]+PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, kernels, ksize);
								PE_output.PE3_output[outCount]=PE_output.PE3_output[outCount]+PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, kernels, ksize);
								PE_output.PE4_output[outCount]=PE_output.PE4_output[outCount]+PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, kernels, ksize);
	
						  }


						  else if(NExtraPE_NeededC==6)
						  {
							  ExR6:for(i=0;i<K;i++)
								{
								  if(i<kernelSize)
								  {
								  ExC6:for(j=0;j<K;j++)
									{
									  if(i<kernelSize)
									  {
										inCount = i*imageSize+j+p*(kernelSize)+n*imageSize*stride+c*imageSize*2;
										PE_input.N0_input[inputCount]= inputs[inCount+stride*0];
										PE_input.N1_input[inputCount]= inputs[inCount+stride*1];
										PE_input.N2_input[inputCount]= inputs[inCount+stride*2];
										PE_input.N3_input[inputCount]= inputs[inCount+stride*3];
										PE_input.N4_input[inputCount]= inputs[inCount+stride*4];
										PE_input.N5_input[inputCount]= inputs[inCount+stride*5];
										inputCount++;
									  }
									  else
										  break;
									}
								  }
								  else
									  break;
								}
								outCount = p+n+count+f*filter;
								PE_output.PE0_output[outCount]=PE_output.PE0_output[outCount]+PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, kernels, ksize);
								PE_output.PE1_output[outCount]=PE_output.PE1_output[outCount]+PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, kernels, ksize);
								PE_output.PE2_output[outCount]=PE_output.PE2_output[outCount]+PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, kernels, ksize);
								PE_output.PE3_output[outCount]=PE_output.PE3_output[outCount]+PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, kernels, ksize);
								PE_output.PE4_output[outCount]=PE_output.PE4_output[outCount]+PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, kernels, ksize);
								PE_output.PE5_output[outCount]=PE_output.PE5_output[outCount]+PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, kernels, ksize);

						  }

						  else if(NExtraPE_NeededC==7)
						  {
							  ExR7:for(i=0;i<K;i++)
								{
								  if(i<kernelSize)
								  {
								  ExC7:for(j=0;j<K;j++)
									{
									  if(i<kernelSize)
									  {
										inCount = i*imageSize+j+p*(kernelSize)+n*imageSize*stride+c*imageSize*2;
										PE_input.N0_input[inputCount]= inputs[inCount+stride*0];
										PE_input.N1_input[inputCount]= inputs[inCount+stride*1];
										PE_input.N2_input[inputCount]= inputs[inCount+stride*2];
										PE_input.N3_input[inputCount]= inputs[inCount+stride*3];
										PE_input.N4_input[inputCount]= inputs[inCount+stride*4];
										PE_input.N5_input[inputCount]= inputs[inCount+stride*5];
										PE_input.N6_input[inputCount]= inputs[inCount+stride*6];
									  }
									  else
										  break;
									}
								  }
								  else
									  break;
								}
								outCount = p+n+count+f*filter;
								PE_output.PE0_output[outCount]=PE_output.PE0_output[outCount]+PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, kernels, ksize);
								PE_output.PE1_output[outCount]=PE_output.PE1_output[outCount]+PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, kernels, ksize);
								PE_output.PE2_output[outCount]=PE_output.PE2_output[outCount]+PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, kernels, ksize);
								PE_output.PE3_output[outCount]=PE_output.PE3_output[outCount]+PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, kernels, ksize);
								PE_output.PE4_output[outCount]=PE_output.PE4_output[outCount]+PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, kernels, ksize);
								PE_output.PE5_output[outCount]=PE_output.PE5_output[outCount]+PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, kernels, ksize);
								PE_output.PE6_output[outCount]=PE_output.PE6_output[outCount]+PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input, kernels, ksize);

						  }

						}
						count=count+NPartionsC;
					  }
						else
							break;
					}//offset
			}//channel

//	cout<<"outIndex " << p+n+count <<'\n';
	copy1:for(m=0;m<MaxConvOut1D;m++)
	{
		if(m<convOut1D)
		{
		copy2:for(q=0;q<NPc;q++)
		{
			if(q<NPartionsC)
			{
				copyCount = m+q+count2+f*filter;
#ifndef __SYNTHESIS__

					cout<< "PE_output.PE0_output:"<< PE_output.PE0_output[copyCount] <<'\n';
#endif
				inputs[count1]   = PE_output.PE0_output[copyCount];
				inputs[count1+1] = PE_output.PE1_output[copyCount];
				inputs[count1+2] = PE_output.PE2_output[copyCount];
				inputs[count1+3] = PE_output.PE3_output[copyCount];
				inputs[count1+4] = PE_output.PE4_output[copyCount];
				inputs[count1+5] = PE_output.PE5_output[copyCount];
				inputs[count1+6] = PE_output.PE6_output[copyCount];
				inputs[count1+7] = PE_output.PE7_output[copyCount];
				count1 = count1+8;
			}
			else
				break;
		}
		if(NExtraPE_NeededC!=0)
		{
			if(NExtraPE_NeededC==1)
			{
				copyCount = m+q+count2+f*filter;
				inputs[count1]   = PE_output.PE0_output[copyCount];
				count1 = count1+1;
			}
			else if(NExtraPE_NeededC==2)
			{
				copyCount = m+q+count2+f*filter;
				inputs[count1]   = PE_output.PE0_output[copyCount];
				inputs[count1+1] = PE_output.PE1_output[copyCount];
				count1 = count1+2;
			}
			else if(NExtraPE_NeededC==3)
			{
				copyCount = m+q+count2+f*filter;
				inputs[count1]   = PE_output.PE0_output[copyCount];
				inputs[count1+1] = PE_output.PE1_output[copyCount];
				inputs[count1+2] = PE_output.PE2_output[copyCount];
				count1 = count1+3;
			}
			else if(NExtraPE_NeededC==4)
			{
				copyCount = m+q+count2+f*filter;
				inputs[count1]   = PE_output.PE0_output[copyCount];
				inputs[count1+1] = PE_output.PE1_output[copyCount];
				inputs[count1+2] = PE_output.PE2_output[copyCount];
				inputs[count1+3] = PE_output.PE3_output[copyCount];
				count1 = count1+4;
			}
			else if(NExtraPE_NeededC==5)
			{
				copyCount = m+q+count2+f*filter;
				inputs[count1]   = PE_output.PE0_output[copyCount];
				inputs[count1+1] = PE_output.PE1_output[copyCount];
				inputs[count1+2] = PE_output.PE2_output[copyCount];
				inputs[count1+3] = PE_output.PE3_output[copyCount];
				inputs[count1+4] = PE_output.PE4_output[copyCount];
				count1 = count1+5;
			}
			else if(NExtraPE_NeededC==6)
			{
				copyCount = m+q+count2+f*filter;
				inputs[count1]   = PE_output.PE0_output[copyCount];
				inputs[count1+1] = PE_output.PE1_output[copyCount];
				inputs[count1+2] = PE_output.PE2_output[copyCount];
				inputs[count1+3] = PE_output.PE3_output[copyCount];
				inputs[count1+4] = PE_output.PE4_output[copyCount];
				inputs[count1+5] = PE_output.PE5_output[copyCount];
				count1 = count1+6;
			}
			else if(NExtraPE_NeededC==7)
			{
				copyCount = m+q+count2+f*filter;
				inputs[count1]   = PE_output.PE0_output[copyCount];
				inputs[count1+1] = PE_output.PE1_output[copyCount];
				inputs[count1+2] = PE_output.PE2_output[copyCount];
				inputs[count1+3] = PE_output.PE3_output[copyCount];
				inputs[count1+4] = PE_output.PE4_output[copyCount];
				inputs[count1+5] = PE_output.PE5_output[copyCount];
				inputs[count1+6] = PE_output.PE6_output[copyCount];
				count1 = count1+7;
			}
		}
		count2=count2+NPartionsC;
	  }
		else
			break;
	}//copy
  }//filter
}//layer
#ifndef __SYNTHESIS__
	cout<<"count1: " << count1 <<'\n';
	cout<<"count2: " << count2 <<'\n';
#endif
}

LyrL: for(i=0;i<Lcounter;i++)
      {
		if(i<Lcounter)
		{
            int Partions_plus_Extra=0;
            int OutCounter=0;
            int Counter2=0;
            int Counter3=0;
            int Counter1=0;

        if(i==0)
        {
        	//INPUTS2: for(j=0;j<NInputs[0];j++)
        	INPUTS2: for(j=0;j<INPUT_SIZE;j++)
        	         {
        					if(j<NInputs[0])
        					{
								PE_input.N0_input[j]= inputs[j];
								#ifndef __SYNTHESIS__
								//				 cout<<"PE_input.N0_input[" << i << "]="<< PE_input.N0_input[i]<<'\n';
								#endif
								 PE_input.N1_input[j]= inputs[j];
								 PE_input.N2_input[j]= inputs[j];
								 PE_input.N3_input[j]= inputs[j];
								 PE_input.N4_input[j]= inputs[j];
								 PE_input.N5_input[j]= inputs[j];
								 PE_input.N6_input[j]= inputs[j];
								 PE_input.N7_input[j]= inputs[j];
        					}
        					else
        						break;
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
                    //J1: for(j=0;j<NInputs[i];j++)
                    J1: for(j=0;j<INPUT_SIZE;j++)
                    {
                    	if(j<NInputs[i])
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
                    	else
                    		break;

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
                 //Ex1: for(j=0;j<NInputs[i];j++)
                Ex1: for(j=0;j<INPUT_SIZE;j++)
                  {
                	 if(j<NInputs[i])
                	 {
						 PE_weight.N0_weight[j]=weights[wightcount];
						 wightcount=wightcount+1;
                	 }
                	 else
                		 break;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==1


		 else if(NExtraPE_Needed[i]==2)
          {
               //Ex2:   for(j=0;j<NInputs[i];j++)
            	   Ex2: for(j=0;j<INPUT_SIZE;j++)
                  {
            		 if(j<NInputs[i])
            		 {
						 PE_weight.N0_weight[j]=weights[wightcount];
						 wightcount=wightcount+1;
						 PE_weight.N1_weight[j]=weights[wightcount];
						 wightcount=wightcount+1;
            		 }
            		 else
            			 break;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==2


		 else if(NExtraPE_Needed[i]==3)
          {
			 //Ex3:  for(j=0;j<NInputs[i];j++)
				 Ex3: for(j=0;j<INPUT_SIZE;j++)
                  {
					 if(j<NInputs[i])
					 {
						 PE_weight.N0_weight[j]=weights[wightcount];
						 wightcount=wightcount+1;
						 PE_weight.N1_weight[j]=weights[wightcount];
						 wightcount=wightcount+1;
						 PE_weight.N2_weight[j]=weights[wightcount];
						 wightcount=wightcount+1;
					 }
					 else
						 break;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
				PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==3


		 else if(NExtraPE_Needed[i]==4)
          {
			 //Ex4:    for(j=0;j<NInputs[i];j++)
				 Ex4: for(j=0;j<INPUT_SIZE;j++)
                  {
					 if(j<NInputs[i])
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
					 else
						 break;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
				PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
				PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==4


		 else if(NExtraPE_Needed[i]==5)
          {
			 //Ex5:    for(j=0;j<NInputs[i];j++)
				 Ex5: for(j=0;j<INPUT_SIZE;j++)
                  {
					 if(j<NInputs[i])
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
					 else
						 break;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
				PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
				PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
				PE_output.PE4_output[NPartions[i]]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==5


		 else if(NExtraPE_Needed[i]==6)
          {
			 //Ex6:    for(j=0;j<NInputs[i];j++)
				 Ex6: for(j=0;j<INPUT_SIZE;j++)
                  {
					 if(j<NInputs[i])
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
					 else
						 break;

				 }
				PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]);
				PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]);
				PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]);
				PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]);
				PE_output.PE4_output[NPartions[i]]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]);
				PE_output.PE5_output[NPartions[i]]=PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]);
			  } //end else if NExtraPE_Needed[i+1]==6


		 else if(NExtraPE_Needed[i]==7)
          {
			 //Ex7:     for(j=0;j<NInputs[i];j++)
				 Ex7: for(j=0;j<INPUT_SIZE;j++)
                  {
					 if(j<NInputs[i])
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
					 else
						 break;

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
                 //J3:  for(j=0;j<NInputs[i];j++)
                J3:  for(j=0;j<INPUT_SIZE;j++)
                {
                	if(j<NInputs[i])
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
                	else
                		break;
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
			 //Ex8:   for(j=0;j<NInputs[i];j++)
				 Ex8: for(j=0;j<INPUT_SIZE;j++)
                      {
					 	 if(j<NInputs[i])
					 	 {
				 	 	 	 PE_weight.N0_weight[j]=weights[wightcount];
				 	 	 	 wightcount=wightcount+1;
					 	 }
					 	 else
					 		 break;
                      }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==1
		 else if(NExtraPE_Needed[i]==2)
              {
			 //Ex9:     for(j=0;j<NInputs[i];j++)
			 	 Ex9: for(j=0;j<INPUT_SIZE;j++)
                 {
			 		 if(j<NInputs[i])
			 		 {
						PE_weight.N0_weight[j]=weights[wightcount];
						wightcount=wightcount+1;
						PE_weight.N1_weight[j]=weights[wightcount];
						wightcount=wightcount+1;
			 		 }
			 		 else
			 			 break;
                 }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
			PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==2
		 else if(NExtraPE_Needed[i]==3)
              {
			 	 //Ex10:for(j=0;j<NInputs[i];j++)
				 Ex10: for(j=0;j<INPUT_SIZE;j++)
                      {
						 if(j<NInputs[i])
						 {
								PE_weight.N0_weight[j]=weights[wightcount];
								wightcount=wightcount+1;
								PE_weight.N1_weight[j]=weights[wightcount];
								wightcount=wightcount+1;
								PE_weight.N2_weight[j]=weights[wightcount];
								wightcount=wightcount+1;
						 }
						 else
							 break;
                      }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
			PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
			PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==3
		 else if(NExtraPE_Needed[i]==4)
              {
			 //Ex11:    for(j=0;j<NInputs[i];j++)
				 Ex11: for(j=0;j<INPUT_SIZE;j++)
                      {
					 if(j<NInputs[i])
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
					 else
						 break;
		  }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
			PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
			PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]);
			PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==4
		 else if(NExtraPE_Needed[i]==5)
              {
			 //Ex12:     for(j=0;j<NInputs[i];j++)
				 Ex12: for(j=0;j<INPUT_SIZE;j++)
                 {
					 if(j<NInputs[i])
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
					 else
						 break;
		  }
			PE_output.PE0_output[NPartions[i]]=PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]);
			PE_output.PE1_output[NPartions[i]]=PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]);
			PE_output.PE2_output[NPartions[i]]=PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]);
			PE_output.PE3_output[NPartions[i]]=PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]);
			PE_output.PE4_output[NPartions[i]]=PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input,PE_weight.N4_weight,NInputs[i]);
		  } //end else if NExtraPE_Needed[i+1]==5
		 else if(NExtraPE_Needed[i]==6)
              {
			 //Ex13:    for(j=0;j<NInputs[i];j++)
				 Ex13: for(j=0;j<INPUT_SIZE;j++)
                      {
					 if(j<NInputs[i])
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
					 else
						 break;
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
			 //Ex14:    for(j=0;j<NInputs[i];j++)
				 Ex14: for(j=0;j<INPUT_SIZE;j++)
                  {
					 if(j<NInputs[i])
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
					 else
						 break;
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
						 out.last = (OutCounter+0==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE1_output[k];
						 out.last = (OutCounter+1==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE2_output[k];
						 out.last = (OutCounter+2==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE3_output[k];
						 out.last = (OutCounter+3==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE4_output[k];
						 out.last = (OutCounter+4==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE5_output[k];
						 out.last = (OutCounter+5==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE6_output[k];
						 out.last = (OutCounter+6==NOutputs-1);
						 out_stream.write(out);

						 out.data = PE_output.PE7_output[k];
						 out.last = (OutCounter+7==NOutputs-1);
						 out_stream.write(out);


						 OutCounter=OutCounter+8;
				} //end for(k=0;k<NPartions[i];k++)
      		} // end if(R==NPartions[i]-1)
      else if(R==NPartions[i] && NExtraPE_Needed[i] !=0){
	 if(NExtraPE_Needed[i]==1)
          {
			out.data = PE_output.PE0_output[NPartions[i]];
            out.last = (OutCounter+0==NOutputs-1);
			out_stream.write(out);

			OutCounter=OutCounter+1;;
		  } //end else if NExtraPE_Needed[i]==1
	 else if(NExtraPE_Needed[i]==2)
          {
			out.data = PE_output.PE0_output[NPartions[i]];
            out.last = (OutCounter+0==NOutputs-1);
            out_stream.write(out);

			out.data = PE_output.PE1_output[NPartions[i]];
			out.last = (OutCounter+1==NOutputs-1);
			out_stream.write(out);

			OutCounter=OutCounter+2;
		  } //end else if NExtraPE_Needed[i]==2
	 else if(NExtraPE_Needed[i]==3)
          {
			out.data = PE_output.PE0_output[NPartions[i]];
				out.last = (OutCounter+0==NOutputs-1);
				out_stream.write(out);
			out.data = PE_output.PE1_output[NPartions[i]];
				out.last = (OutCounter+1==NOutputs-1);
				out_stream.write(out);
			out.data = PE_output.PE2_output[NPartions[i]];
				out.last = (OutCounter+2==NOutputs-1);
				out_stream.write(out);

				OutCounter=OutCounter+3;
		  } //end else if NExtraPE_Needed[i]==3
	 else if(NExtraPE_Needed[i]==4)
          {
			 out.data = PE_output.PE0_output[NPartions[i]];
				out.last = (OutCounter+0==NOutputs-1);
				out_stream.write(out);
			 out.data = PE_output.PE1_output[NPartions[i]];
				out.last = (OutCounter+1==NOutputs-1);
				out_stream.write(out);
			 out.data = PE_output.PE2_output[NPartions[i]];
				out.last = (OutCounter+2==NOutputs-1);
				out_stream.write(out);
			 out.data = PE_output.PE3_output[NPartions[i]];
				out.last = (OutCounter+3==NOutputs-1);
    			out_stream.write(out);

				OutCounter=OutCounter+4;
			} //end else if NExtraPE_Needed[i]==4
	 else if(NExtraPE_Needed[i]==5)
          {
			 out.data = PE_output.PE0_output[NPartions[i]];
				out.last = (OutCounter+0==NOutputs-1);
				out_stream.write(out);
			 out.data = PE_output.PE1_output[NPartions[i]];
				out.last = (OutCounter+1==NOutputs-1);
				out_stream.write(out);
			 out.data = PE_output.PE2_output[NPartions[i]];
				out.last = (OutCounter+2==NOutputs-1);
				out_stream.write(out);
			 out.data = PE_output.PE3_output[NPartions[i]];
				out.last = (OutCounter+3==NOutputs-1);
				out_stream.write(out);
			 out.data = PE_output.PE4_output[NPartions[i]];
				out.last = (OutCounter+4==NOutputs-1);
				out_stream.write(out);

				OutCounter=OutCounter+5;
			} //end else if NExtraPE_Needed[i]==5
	 else if(NExtraPE_Needed[i]==6)
          {
				 out.data = PE_output.PE0_output[NPartions[i]];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE1_output[NPartions[i]];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE2_output[NPartions[i]];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE3_output[NPartions[i]];
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE4_output[NPartions[i]];
                    out.last = (OutCounter+4==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE5_output[NPartions[i]];
                    out.last = (OutCounter+5==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+6;
		  } //end else if NExtraPE_Needed[i]==6
	 else if(NExtraPE_Needed[i]==7)
          {
				 out.data = PE_output.PE0_output[NPartions[i]];
                    out.last = (OutCounter+0==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE1_output[NPartions[i]];
                    out.last = (OutCounter+1==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE2_output[NPartions[i]];
                    out.last = (OutCounter+2==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE3_output[NPartions[i]];
                    out.last = (OutCounter+3==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE4_output[NPartions[i]];
                    out.last = (OutCounter+4==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE5_output[NPartions[i]];
                    out.last = (OutCounter+5==NOutputs-1);
    				out_stream.write(out);
				 out.data = PE_output.PE6_output[NPartions[i]];
                    out.last = (OutCounter+6==NOutputs-1);
    				out_stream.write(out);

				OutCounter=OutCounter+7;
			} //end else if NExtraPE_Needed[i]==7
      	  } //end  else if(R==NPartions[i] && NExtraPE_Needed[i] !=0)
      } // end for(R=0;R<Partions_plus_Extra;R++)
   }//	else if(i==Lcounter-1)
		}
		else
			break;
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

