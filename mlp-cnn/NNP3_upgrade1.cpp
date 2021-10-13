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
	uint8	kernelSize[MAXConvLAYERS];//
	uint8 	imageSize[MAXConvLAYERS];//
	int		filterSizeTotal[MAXConvLAYERS];

	void Controller(hls::stream<AXI_VAL1> &in_stream, hls::stream<AXI_VAL2> &out_stream, configuration *Config, int new_net, int conv){



	PE_weights PE_weight;
	PE_inputs<fp1> PE_input;
	PE_outputs<fp6> PE_output;
	PE_outputs<fp6> PE_output1;

    fp2  weights[M_WEIGHT_SIZE];

    fp2  kernels[ksize];

    fp2  tempKernel[ksize];

    fp1  tempInputs[INPUT_SIZE];

    fp1 inputs[INPUT_SIZE];


	uint8   Lcounter=0;
	uint8   NOutputs=0;


//	uint8	channels=0;
//	uint8 	stride=0;
//	uint8 	padding=0;

	uint8	convLayer = 0;
	uint16 	convOut = 0;
	uint16	filter=0;
	uint16  KsizeTotal = 0;

	int MaxW=0;
	int	wightcount=0;
	int inCount;
	int outCount;
	int inputCount;
	int copyCount;
	int count;
	int count1;
	int count2;
	int kernelCount = 0;
	int a,b,c,f,i,j,k,l,m,n,o,p,q,R,s,w;

	AXI_VAL1 in1;
    AXI_VAL2 out;

	 ////threshold=Config->thresholdv;
	 Lcounter=Config->Nlayers;     // hidden +outputs
	 NOutputs=Config->Noutputs;

//	 imageSize=Config->imageSize;
	 convOut = Config->convOut;
//	 kernelSize = Config->KernelSize;
//	 channels = Config->channels;
//	 stride = Config->stride;
//	 padding = Config->padding;
	 KsizeTotal = Config->KsizeTotal;
//	 convOut1D = Config->convOut1D;
	 convLayer = Config->convLayer;
//	 filter = Config->filter;

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

	 imageSize[0]=Config->imageSize1;
	 imageSize[1]=Config->imageSize2;
	 imageSize[2]=Config->imageSize3;
	 imageSize[3]=Config->imageSize4;
	 imageSize[4]=Config->imageSize5;
	 imageSize[5]=Config->imageSize6;

	 kernelSize[0] = Config->KernelSize1;
	 kernelSize[1] = Config->KernelSize2;
	 kernelSize[2] = Config->KernelSize3;
	 kernelSize[3] = Config->KernelSize4;
	 kernelSize[4] = Config->KernelSize5;
	 kernelSize[5] = Config->KernelSize6;

	 inChannels[0] = Config->channels1;
	 inChannels[1] = Config->channels2;
	 inChannels[2] = Config->channels3;
	 inChannels[3] = Config->channels4;
	 inChannels[4] = Config->channels5;
	 inChannels[5] = Config->channels6;

	 stride[0] = Config->stride1;
	 stride[1] = Config->stride2;
	 stride[2] = Config->stride3;
	 stride[3] = Config->stride4;
	 stride[4] = Config->stride5;
	 stride[5] = Config->stride6;

	 KSIZE[0] = Config->KSIZE1;
	 KSIZE[1] = Config->KSIZE2;
	 KSIZE[2] = Config->KSIZE3;
	 KSIZE[3] = Config->KSIZE4;
	 KSIZE[4] = Config->KSIZE5;
	 KSIZE[5] = Config->KSIZE6;

	 convOut1D[0] = Config->convOut1D1;
	 convOut1D[1] = Config->convOut1D2;
	 convOut1D[2] = Config->convOut1D3;
	 convOut1D[3] = Config->convOut1D4;
	 convOut1D[4] = Config->convOut1D5;
	 convOut1D[5] = Config->convOut1D6;

	 filterNo[0] = Config->filter1;
	 filterNo[1] = Config->filter2;
	 filterNo[2] = Config->filter3;
	 filterNo[3] = Config->filter4;
	 filterNo[4] = Config->filter5;
	 filterNo[5] = Config->filter6;

	 filterSizeTotal[0] = Config->filterTotal1; //Config->filterTotal2, Config->filterTotal3, Config->filterTotal4, Config->filterTotal5, Config->filterTotal6};
	 filterSizeTotal[1] = Config->filterTotal2;
	 filterSizeTotal[2] = Config->filterTotal3;
	 filterSizeTotal[3] = Config->filterTotal4;
	 filterSizeTotal[4] = Config->filterTotal5;
	 filterSizeTotal[5] = Config->filterTotal6;

#ifndef __SYNTHESIS__
/*	 cout<< "ksize:"<< ksize <<'\n';
	 cout<< "NInputs[0]:"<< NInputs[0] <<'\n';
	 cout<< "filterSizeTotal[0]:"<< filterSizeTotal[0] <<'\n';
	 cout<< "filterNo[0]:"<< filterNo[0] <<'\n';
	 cout<< "convOut1D[0]:"<< convOut1D[0] <<'\n';
	 cout<< "KSIZE[0]:"<< KSIZE[0] <<'\n';
	 cout<< "inChannels[0]:"<< inChannels[0] <<'\n';
	 cout<< "kernelSize[0]:"<< kernelSize[0] <<'\n';
	 cout<< "imageSize[0]:"<< imageSize[0] <<'\n';
*/
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
				 //cout<< "inputs:"<<i<<":"<< inputs[i] <<'\n';
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
					//cout<< "weights"<<i<<":"<< weights[i] <<'\n';
				}
				else
					break;

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
						if(i<KsizeTotal)
						{
							in_stream >> in1;
							kernels[i] = in1.data;
						}
						else
							break;
						 //cout<< "kernels:"<<i<<":"<< kernels[i]<<'\n';
					}
			//cout<< "kernels:"<<i<<":"<< i<<'\n';
	PartitionC:for(i=0;i<MAXConvLAYERS;i++)
		{
			if(i<convLayer)
			{
				NPartionsC[i]=(uint8)(convOut1D[i]/NUM_PE);
				NExtraPE_NeededC[i]=convOut1D[i]-(NPartionsC[i]*NUM_PE);
	#ifndef __SYNTHESIS__
				cout<< "NPartionsC:"<< NPartionsC[i] <<'\n';
				cout<< "NExtraPE_NeededC:"<< NExtraPE_NeededC[i] <<'\n';
	#endif
			}
			else
				break;
		}
	}
}

//-------------------------------end feeding weights-------------------------------------------------

//------------------------------- start feeding inputs-----------------------------------------------


if (conv==1)
{
	layer:for(l=0;l<MAXConvLAYERS;l++)
	{

		inCount=0;
		inputCount=0;
		//copyCount=0;
		count1=0;

		if(l<convLayer)
		{
			cout<<"New layer"<<'\n';
/*		PE_outputs<fp6> PE_output;
#ifndef __SYNTHESIS__

		cout<< "PE_output.PE0_output0:"<< PE_output.PE0_output[0] <<'\n';
		cout<< "PE_output.PE0_output1:"<< PE_output.PE0_output[1] <<'\n';
		cout<< "PE_output.PE0_output2:"<< PE_output.PE0_output[2] <<'\n';
		cout<< "PE_output.PE0_output3:"<< PE_output.PE0_output[3] <<'\n';
		cout<< "PE_output.PE0_output4:"<< PE_output.PE0_output[4] <<'\n';
		cout<< "PE_output.PE0_output5:"<< PE_output.PE0_output[5] <<'\n';
		cout<< "PE_output.PE0_output6:"<< PE_output.PE0_output[6] <<'\n';
		cout<< "PE_output.PE0_output7:"<< PE_output.PE0_output[7] <<'\n';
		cout<< "PE_output.PE0_output8:"<< PE_output.PE0_output[8] <<'\n';
		cout<< "PE_output.PE0_output9:"<< PE_output.PE0_output[9] <<'\n';
		cout<< "PE_output.PE0_output10:"<< PE_output.PE0_output[10] <<'\n';
#endif*/

		filter:for(f=0;f<MAXFilter;f++)
		{

			if(f<filterNo[l])
			{
				copyCount=0;
				cout<<"New filter"<<'\n';
//				PE_inputs<fp1> PE_input1;

				initial:for(b=0;b<MAX_PE_OUT;b++)
				{
					PE_output1.PE0_output[b]=0;
					PE_output1.PE1_output[b]=0;
					PE_output1.PE2_output[b]=0;
					PE_output1.PE3_output[b]=0;
					PE_output1.PE4_output[b]=0;
					PE_output1.PE5_output[b]=0;
					PE_output1.PE6_output[b]=0;
					PE_output1.PE7_output[b]=0;
				}
			//count2=0;
			//copyCount= 0;

			channels:for(c=0;c<MAXChannel;c++)
			{
				outCount=0;

				if(c<inChannels[l])
				{
					cout<<"New Channel"<<'\n';
				//count=0;

				cacheKernel: for(s=0;s<MaxK;s++)
				{
					if(s<KSIZE[l])
					{
						//tempKernel[s] = kernels[s+c*KSIZE[l]+f*filterSizeTotal[l]];
						tempKernel[s] = kernels[kernelCount];
						cout<< "tempKernel[s]::: "<< s <<":" << tempKernel[s] <<'\n';
						//cout<< "kernelCount::: "<< kernelCount <<'\n';
						kernelCount++;
					}
					else
						break;
				}
				offset:for(n=0;n<MaxConvOut1D;n++)
				{
					if(n<convOut1D[l])
					{
						//inputCount=0;
						p1:for(p=0;p<NPc;p++)
						{
							//inputCount=0;
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
												//inCount = i*imageSize[l]+j+p*(kernelSize[l])+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
												inCount = i*imageSize[l]+j+p*stride[l]*NUM_PE+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
												//cout<<"inCount: "<<inCount << '\n'<<'\n';

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


									//outCount = p+n+count;//+f*filterNo[l];

			#ifndef __SYNTHESIS__
			/*					cout<< "Inside Partition loop" <<'\n';
								cout<< "PE_output.PE2_output:"<< PE_output.PE2_output[outCount] <<'\n';
								cout<< "PE_output.PE2_output:"<< PE_output.PE2_output[outCount+1] <<'\n';
								cout<< "PE_output.PE2_output:"<< PE_output.PE2_output[outCount+2] <<'\n';
								cout<< "PE_output.PE2_output:"<< PE_output.PE2_output[outCount+3] <<'\n';
								cout<< "PE_output.PE2_output:"<< PE_output.PE2_output[outCount+4] <<'\n';
								cout<< "PE_output.PE2_output:"<< PE_output.PE2_output[outCount+5] <<'\n';
								cout<< "PE_output.PE2_output:"<< PE_output.PE2_output[outCount+6] <<'\n';
								cout<< "PE_output.PE2_output:"<< PE_output.PE2_output[outCount+7] <<'\n';
								cout<< "PE_output.PE2_output:"<< PE_output.PE2_output[outCount+8] <<'\n';
								cout<< "PE_output.PE2_output:"<< PE_output.PE2_output[outCount+9] <<'\n';
*/
			#endif
//								cout<<"inputCount: "<<inputCount << '\n';


								PE_output1.PE0_output[outCount]=PE_output1.PE0_output[outCount]+(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, tempKernel, KSIZE[l]));
//								cout<< "PE_output1.PE0_output[outCount]:"<< outCount<< " "<<PE_output1.PE0_output[outCount] <<'\n';

								PE_output1.PE1_output[outCount]=PE_output1.PE1_output[outCount]+(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, tempKernel, KSIZE[l]));
//								cout<< "PE_output1.PE1_output[outCount]:"<< outCount<< " "<<PE_output1.PE1_output[outCount] <<'\n';

								PE_output1.PE2_output[outCount]=PE_output1.PE2_output[outCount]+(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, tempKernel, KSIZE[l]));
//								cout<< "PE_output1.PE2_output[outCount]:"<< outCount<< " "<<PE_output1.PE2_output[outCount] <<'\n';

								PE_output1.PE3_output[outCount]=PE_output1.PE3_output[outCount]+(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, tempKernel, KSIZE[l]));
//								cout<< "PE_output1.PE3_output[outCount]:"<< outCount<< " "<<PE_output1.PE3_output[outCount] <<'\n';

								PE_output1.PE4_output[outCount]=PE_output1.PE4_output[outCount]+(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, tempKernel, KSIZE[l]));
//								cout<< "PE_output1.PE4_output[outCount]:"<< outCount<< " "<<PE_output1.PE4_output[outCount] <<'\n';

								PE_output1.PE5_output[outCount]=PE_output1.PE5_output[outCount]+(PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, tempKernel, KSIZE[l]));
//								cout<< "PE_output1.PE5_output[outCount]:"<< outCount<< " "<<PE_output1.PE5_output[outCount] <<'\n';

								PE_output1.PE6_output[outCount]=PE_output1.PE6_output[outCount]+(PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input, tempKernel, KSIZE[l]));
//								cout<< "PE_output1.PE6_output[outCount]:"<< outCount<< " "<<PE_output1.PE6_output[outCount] <<'\n';

								PE_output1.PE7_output[outCount]=PE_output1.PE7_output[outCount]+(PE7_Unit<fp6, fp1, fp2>(PE_input.N7_input, tempKernel, KSIZE[l]));
//								cout<< "PE_output1.PE7_output[outCount]:"<< outCount<< " "<<PE_output1.PE7_output[outCount] <<'\n';
									outCount++;
							}
			
							else
								break;
							

						}

						cout<<"P:"<<p << '\n';

						inputCount=0;

						if(NExtraPE_NeededC[l]!=0)
						{
						  if(NExtraPE_NeededC[l]==1)
						  {
								ExR1:for(i=0;i<K;i++)
								{
									if(i<kernelSize[l])
									{
										ExC1:for(j=0;j<K;j++)
										{
											if(j<kernelSize[l])
											{
												//inCount = i*imageSize[l]+j+p*(kernelSize[l])+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
												inCount = i*imageSize[l]+j+p*stride[l]*NUM_PE+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
												//cout<<"inCount: "<<inCount << '\n'<<'\n';

												PE_input.N0_input[inputCount]= inputs[inCount+stride[l]*0];
												inputCount++;
											}
											else
												break;
										}
									}
									else
										break;
								}
								//outCount = p+n+count;//+f*filterNo[l];
								PE_output1.PE0_output[outCount]=PE_output1.PE0_output[outCount]+(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, tempKernel, KSIZE[l]));
								outCount++;
						  }
						  else if(NExtraPE_NeededC[l]==2)
						  {
							  ExR2:for(i=0;i<K;i++)
								{
								  if(i<kernelSize[l])
								  {
									ExC2:for(j=0;j<K;j++)
									{
									  if(j<kernelSize[l])
									  {
										//inCount = i*imageSize[l]+j+p*(kernelSize[l])+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										inCount = i*imageSize[l]+j+p*stride[l]*NUM_PE+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										//cout<<"inCount: "<<inCount << '\n'<<'\n';

										PE_input.N0_input[inputCount]= inputs[inCount+stride[l]*0];
										PE_input.N1_input[inputCount]= inputs[inCount+stride[l]*1];
										inputCount++;
									  }
									  else
										  break;
									}
								  }
								  else
									  break;
								}
								//outCount = p+n+count;//+f*filterNo[l];
							  PE_output1.PE0_output[outCount]=PE_output1.PE0_output[outCount]+(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, tempKernel, KSIZE[l]));
							  PE_output1.PE1_output[outCount]=PE_output1.PE1_output[outCount]+(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, tempKernel, KSIZE[l]));
								outCount++;
						  }

						  else if(NExtraPE_NeededC[l]==3)
						  {
							  ExR3:for(i=0;i<K;i++)
								{
								  if(i<kernelSize[l])
								  {
								  ExC3:for(j=0;j<K;j++)
									{
									  if(j<kernelSize[l])
									  {
											//inCount = i*imageSize[l]+j+p*(kernelSize[l])+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										    inCount = i*imageSize[l]+j+p*stride[l]*NUM_PE+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										    //cout<<"inCount: "<<inCount << '\n'<<'\n';

											PE_input.N0_input[inputCount]= inputs[inCount+stride[l]*0];
											PE_input.N1_input[inputCount]= inputs[inCount+stride[l]*1];
											PE_input.N2_input[inputCount]= inputs[inCount+stride[l]*2];
											inputCount++;
									  }
									  else
										   break;
									}
								  }
								  else
									   break;
								}
								//outCount = p+n+count;//+f*filterNo[l];
							  PE_output1.PE0_output[outCount]=PE_output1.PE0_output[outCount]+(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, tempKernel, KSIZE[l]));
							  PE_output1.PE1_output[outCount]=PE_output1.PE1_output[outCount]+(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, tempKernel, KSIZE[l]));
							  PE_output1.PE2_output[outCount]=PE_output1.PE2_output[outCount]+(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, tempKernel, KSIZE[l]));
								outCount++;
						  }
						  else if(NExtraPE_NeededC[l]==4)
						  {
							  ExR4:for(i=0;i<K;i++)
								{
								  if(i<kernelSize[l])
								  {
								  ExC4:for(j=0;j<K;j++)
									{
									  if(j<kernelSize[l])
									  {
										//inCount = i*imageSize[l]+j+p*(kernelSize[l])+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										inCount = i*imageSize[l]+j+p*stride[l]*NUM_PE+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										//cout<<"inCount: "<<inCount << '\n'<<'\n';

										PE_input.N0_input[inputCount]= inputs[inCount+stride[l]*0];
										PE_input.N1_input[inputCount]= inputs[inCount+stride[l]*1];
										PE_input.N2_input[inputCount]= inputs[inCount+stride[l]*2];
										PE_input.N3_input[inputCount]= inputs[inCount+stride[l]*3];
										inputCount++;
									  }
									  else
										  break;
									}
								  }
								  else
									  break;
								}
								//outCount = p+n+count;//+f*filterNo[l];
							  PE_output1.PE0_output[outCount]=PE_output1.PE0_output[outCount]+(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, tempKernel, KSIZE[l]));
							  PE_output1.PE1_output[outCount]=PE_output1.PE1_output[outCount]+(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, tempKernel, KSIZE[l]));
							  PE_output1.PE2_output[outCount]=PE_output1.PE2_output[outCount]+(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, tempKernel, KSIZE[l]));
							  PE_output1.PE3_output[outCount]=PE_output1.PE3_output[outCount]+(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, tempKernel, KSIZE[l]));

								outCount++;
						  }

						  else if(NExtraPE_NeededC[l]==5)
						  {
							  ExR5:for(i=0;i<K;i++)
								{
								  if(i<kernelSize[l])
								  {
								  ExC5:for(j=0;j<K;j++)
									{
									  if(j<kernelSize[l])
									  {
										//inCount = i*imageSize[l]+j+p*(kernelSize[l])+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										inCount = i*imageSize[l]+j+p*stride[l]*NUM_PE+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										//cout<<"inCount: "<<inCount << '\n'<<'\n';

										PE_input.N0_input[inputCount]= inputs[inCount+stride[l]*0];
										PE_input.N1_input[inputCount]= inputs[inCount+stride[l]*1];
										PE_input.N2_input[inputCount]= inputs[inCount+stride[l]*2];
										PE_input.N3_input[inputCount]= inputs[inCount+stride[l]*3];
										PE_input.N4_input[inputCount]= inputs[inCount+stride[l]*4];
										inputCount++;
									  }
									  else
										  break;
									  
									}
								  }
								  else
									  break;
								}
								//outCount = p+n+count;//+f*filterNo[l];
							  PE_output1.PE0_output[outCount]=PE_output1.PE0_output[outCount]+(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, tempKernel, KSIZE[l]));
							  PE_output1.PE1_output[outCount]=PE_output1.PE1_output[outCount]+(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, tempKernel, KSIZE[l]));
							  PE_output1.PE2_output[outCount]=PE_output1.PE2_output[outCount]+(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, tempKernel, KSIZE[l]));
							  PE_output1.PE3_output[outCount]=PE_output1.PE3_output[outCount]+(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, tempKernel, KSIZE[l]));
							  PE_output1.PE4_output[outCount]=PE_output1.PE4_output[outCount]+(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, tempKernel, KSIZE[l]));

								outCount++;
	
						  }


						  else if(NExtraPE_NeededC[l]==6)
						  {
							  ExR6:for(i=0;i<K;i++)
								{
								  if(i<kernelSize[l])
								  {
								  ExC6:for(j=0;j<K;j++)
									{
									  if(j<kernelSize[l])
									  {
										//inCount = i*imageSize[l]+j+p*(kernelSize[l])+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										inCount = i*imageSize[l]+j+p*stride[l]*NUM_PE+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										//cout<<"inCount: "<<inCount << '\n'<<'\n';

										PE_input.N0_input[inputCount]= inputs[inCount+stride[l]*0];
										PE_input.N1_input[inputCount]= inputs[inCount+stride[l]*1];
										PE_input.N2_input[inputCount]= inputs[inCount+stride[l]*2];
										PE_input.N3_input[inputCount]= inputs[inCount+stride[l]*3];
										PE_input.N4_input[inputCount]= inputs[inCount+stride[l]*4];
										PE_input.N5_input[inputCount]= inputs[inCount+stride[l]*5];
										inputCount++;
									  }
									  else
										  break;
									}
								  }
								  else
									  break;
								}
								//outCount = p+n+count;//+f*filterNo[l];
							  PE_output1.PE0_output[outCount]=PE_output1.PE0_output[outCount]+(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, tempKernel, KSIZE[l]));
							  PE_output1.PE1_output[outCount]=PE_output1.PE1_output[outCount]+(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, tempKernel, KSIZE[l]));
							  PE_output1.PE2_output[outCount]=PE_output1.PE2_output[outCount]+(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, tempKernel, KSIZE[l]));
							  PE_output1.PE3_output[outCount]=PE_output1.PE3_output[outCount]+(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, tempKernel, KSIZE[l]));
							  PE_output1.PE4_output[outCount]=PE_output1.PE4_output[outCount]+(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, tempKernel, KSIZE[l]));
							  PE_output1.PE5_output[outCount]=PE_output1.PE5_output[outCount]+(PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, tempKernel, KSIZE[l]));

								outCount++;

						  }

						  else if(NExtraPE_NeededC[l]==7)
						  {
							  ExR7:for(i=0;i<K;i++)
								{
								  if(i<kernelSize[l])
								  {
								  ExC7:for(j=0;j<K;j++)
									{
									  if(j<kernelSize[l])
									  {
										//inCount = i*imageSize[l]+j+p*(kernelSize[l])+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										inCount = i*imageSize[l]+j+p*stride[l]*NUM_PE+n*imageSize[l]*stride[l]+c*imageSize[l]*imageSize[l];
										//cout<<"inCount: "<<inCount << '\n'<<'\n';

										PE_input.N0_input[inputCount]= inputs[inCount+stride[l]*0];
										cout<< "PE_input.N0_input[inputCount]:"<< inputCount<< " "<<PE_input.N0_input[inputCount]<<'\n';
										PE_input.N1_input[inputCount]= inputs[inCount+stride[l]*1];
										PE_input.N2_input[inputCount]= inputs[inCount+stride[l]*2];
										PE_input.N3_input[inputCount]= inputs[inCount+stride[l]*3];
										PE_input.N4_input[inputCount]= inputs[inCount+stride[l]*4];
										PE_input.N5_input[inputCount]= inputs[inCount+stride[l]*5];
										PE_input.N6_input[inputCount]= inputs[inCount+stride[l]*6];
										inputCount++;
									  }
									  else
										  break;
									}
								  }
								  else
									  break;
								}
								//outCount = p+n+count;//+f*filterNo[l];
							  PE_output1.PE0_output[outCount]=PE_output1.PE0_output[outCount]+(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, tempKernel, KSIZE[l]));
							  PE_output1.PE1_output[outCount]=PE_output1.PE1_output[outCount]+(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, tempKernel, KSIZE[l]));
							  PE_output1.PE2_output[outCount]=PE_output1.PE2_output[outCount]+(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, tempKernel, KSIZE[l]));
							  PE_output1.PE3_output[outCount]=PE_output1.PE3_output[outCount]+(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, tempKernel, KSIZE[l]));
							  PE_output1.PE4_output[outCount]=PE_output1.PE4_output[outCount]+(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, tempKernel, KSIZE[l]));
							  PE_output1.PE5_output[outCount]=PE_output1.PE5_output[outCount]+(PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, tempKernel, KSIZE[l]));
							  PE_output1.PE6_output[outCount]=PE_output1.PE6_output[outCount]+(PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input, tempKernel, KSIZE[l]));

								outCount++;

						  }

						}
						//count=count+NPartionsC[l];
					  }
						else
							break;
					}//offset
				}
				else
					break;
			}//channel

//	cout<<"outIndex " << p+n+count <<'\n';
	copy1:for(m=0;m<MaxConvOut1D;m++)
	{
		if(m<convOut1D[l])
		{
		copy2:for(q=0;q<NPc;q++)
		{
			if(q<NPartionsC[l])
			{
				//copyCount = m+q+count2;//+f*filterNo[l];
//#ifndef __SYNTHESIS__
//					cout<< "PE_output.PE0_output:"<< PE_output.PE0_output[copyCount] <<'\n';
//#endif
				tempInputs[count1]   = PE_output1.PE0_output[copyCount];
				tempInputs[count1+1] = PE_output1.PE1_output[copyCount];
				tempInputs[count1+2] = PE_output1.PE2_output[copyCount];
				tempInputs[count1+3] = PE_output1.PE3_output[copyCount];
				tempInputs[count1+4] = PE_output1.PE4_output[copyCount];
				tempInputs[count1+5] = PE_output1.PE5_output[copyCount];
				tempInputs[count1+6] = PE_output1.PE6_output[copyCount];
				tempInputs[count1+7] = PE_output1.PE7_output[copyCount];
				count1 = count1+8;
				copyCount++;
			}
			else
				break;
		}//copy2
		if(NExtraPE_NeededC[l]!=0)
		{
			if(NExtraPE_NeededC[l]==1)
			{
				//copyCount = m+q+count2;//+f*filterNo[l];
				tempInputs[count1]   = PE_output1.PE0_output[copyCount];
				count1 = count1+1;
				copyCount++;
			}
			else if(NExtraPE_NeededC[l]==2)
			{
				//copyCount = m+q+count2;//+f*filterNo[l];
				tempInputs[count1]   = PE_output1.PE0_output[copyCount];
				tempInputs[count1+1] = PE_output1.PE1_output[copyCount];
				count1 = count1+2;
				copyCount++;
			}
			else if(NExtraPE_NeededC[l]==3)
			{
				//copyCount = m+q+count2;//+f*filterNo[l];
				tempInputs[count1]   = PE_output1.PE0_output[copyCount];
				tempInputs[count1+1] = PE_output1.PE1_output[copyCount];
				tempInputs[count1+2] = PE_output1.PE2_output[copyCount];
				count1 = count1+3;
				copyCount++;
			}
			else if(NExtraPE_NeededC[l]==4)
			{
				//copyCount = m+q+count2;//+f*filterNo[l];
				tempInputs[count1]   = PE_output1.PE0_output[copyCount];
				tempInputs[count1+1] = PE_output1.PE1_output[copyCount];
				tempInputs[count1+2] = PE_output1.PE2_output[copyCount];
				tempInputs[count1+3] = PE_output1.PE3_output[copyCount];
				count1 = count1+4;
				copyCount++;
			}
			else if(NExtraPE_NeededC[l]==5)
			{
				//copyCount = m+q+count2;//+f*filterNo[l];
				tempInputs[count1]   = PE_output1.PE0_output[copyCount];
				tempInputs[count1+1] = PE_output1.PE1_output[copyCount];
				tempInputs[count1+2] = PE_output1.PE2_output[copyCount];
				tempInputs[count1+3] = PE_output1.PE3_output[copyCount];
				tempInputs[count1+4] = PE_output1.PE4_output[copyCount];
				count1 = count1+5;
				copyCount++;
			}
			else if(NExtraPE_NeededC[l]==6)
			{
				//copyCount = m+q+count2;//+f*filterNo[l];
				tempInputs[count1]   = PE_output1.PE0_output[copyCount];
				tempInputs[count1+1] = PE_output1.PE1_output[copyCount];
				tempInputs[count1+2] = PE_output1.PE2_output[copyCount];
				tempInputs[count1+3] = PE_output1.PE3_output[copyCount];
				tempInputs[count1+4] = PE_output1.PE4_output[copyCount];
				tempInputs[count1+5] = PE_output1.PE5_output[copyCount];
				count1 = count1+6;
				copyCount++;
			}
			else if(NExtraPE_NeededC[l]==7)
			{
				//copyCount = m+q+count2;//+f*filterNo[l];
				tempInputs[count1]   = PE_output1.PE0_output[copyCount];
				tempInputs[count1+1] = PE_output1.PE1_output[copyCount];
				tempInputs[count1+2] = PE_output1.PE2_output[copyCount];
				tempInputs[count1+3] = PE_output1.PE3_output[copyCount];
				tempInputs[count1+4] = PE_output1.PE4_output[copyCount];
				tempInputs[count1+5] = PE_output1.PE5_output[copyCount];
				tempInputs[count1+6] = PE_output1.PE6_output[copyCount];
				count1 = count1+7;
				copyCount++;
			}
		}
		//count2=count2+NPartionsC[l];
	  }
		else
			break;
	}//copy
				/*initial:for(b=0;b<MAX_PE_OUT;b++)
				{
					PE_output1.PE0_output[b]=0;
					PE_output1.PE1_output[b]=0;
					PE_output1.PE2_output[b]=0;
					PE_output1.PE3_output[b]=0;
					PE_output1.PE4_output[b]=0;
					PE_output1.PE5_output[b]=0;
					PE_output1.PE6_output[b]=0;
					PE_output1.PE7_output[b]=0;
				}*/
		}
		else
			break;

  }//filter

  inputCopy: for(a=0;a<INPUT_SIZE;a++)
			 {
	  	  	  	  if(a<count1)
	  	  	  	  {
	  	  	  		  inputs[a] = RELU<fp6,fp6>(tempInputs[a]);
	  	  	  		  //inputs[a] = tempInputs[a];
	  	  	  		  cout<<"inputs:"<< a <<":"<< inputs[a] <<'\n';
	  	  	  	  }
	  	  	  	  else
	  	  	  		  break;
			 }

		}
		else
			break;
  }//layer
#ifndef __SYNTHESIS__
	cout<<"count1: " << count1 <<'\n';
	//cout<<"count2: " << count2 <<'\n';
#endif
}

//PE_inputs<fp1> PE_input;
//PE_outputs<fp6> PE_output;

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
										 cout<<"PE_input.N0_input[" << j << "]="<< PE_input.N0_input[j]<<'\n';
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
							 cout<<"PE_weight.N0_weight[" << j << "]="<< PE_weight.N0_weight[j]<<'\n';

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
                cout<<"wightcount:"<< wightcount <<'\n' ;
				 PE_output.PE0_output[k]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]));
				 PE_output.PE1_output[k]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]));
				 PE_output.PE2_output[k]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]));
				 PE_output.PE3_output[k]=RELU<fp6,fp6>(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]));
				 PE_output.PE4_output[k]=RELU<fp6,fp6>(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]));
				 PE_output.PE5_output[k]=RELU<fp6,fp6>(PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]));
				 PE_output.PE6_output[k]=RELU<fp6,fp6>(PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]));
				 PE_output.PE7_output[k]=RELU<fp6,fp6>(PE7_Unit<fp6, fp1, fp2>(PE_input.N7_input, PE_weight.N7_weight, NInputs[i]));

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
				PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]));
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
				PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]));
				PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]));
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
				PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]));
				PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]));
				PE_output.PE2_output[NPartions[i]]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]));
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
				PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]));
				PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]));
				PE_output.PE2_output[NPartions[i]]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]));
				PE_output.PE3_output[NPartions[i]]=RELU<fp6,fp6>(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]));
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
				PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]));
				PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]));
				PE_output.PE2_output[NPartions[i]]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]));
				PE_output.PE3_output[NPartions[i]]=RELU<fp6,fp6>(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]));
				PE_output.PE4_output[NPartions[i]]=RELU<fp6,fp6>(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]));
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
				PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]));
				PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]));
				PE_output.PE2_output[NPartions[i]]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]));
				PE_output.PE3_output[NPartions[i]]=RELU<fp6,fp6>(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]));
				PE_output.PE4_output[NPartions[i]]=RELU<fp6,fp6>(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]));
				PE_output.PE5_output[NPartions[i]]=RELU<fp6,fp6>(PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]));
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
				PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input, PE_weight.N0_weight, NInputs[i]));
				PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input, PE_weight.N1_weight, NInputs[i]));
				PE_output.PE2_output[NPartions[i]]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input, PE_weight.N2_weight, NInputs[i]));
				PE_output.PE3_output[NPartions[i]]=RELU<fp6,fp6>(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input, PE_weight.N3_weight, NInputs[i]));
				PE_output.PE4_output[NPartions[i]]=RELU<fp6,fp6>(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input, PE_weight.N4_weight, NInputs[i]));
				PE_output.PE5_output[NPartions[i]]=RELU<fp6,fp6>(PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input, PE_weight.N5_weight, NInputs[i]));
				PE_output.PE6_output[NPartions[i]]=RELU<fp6,fp6>(PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input, PE_weight.N6_weight, NInputs[i]));
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
				 PE_output.PE0_output[k]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]));
				 PE_output.PE1_output[k]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]));
				 PE_output.PE2_output[k]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]));
				 PE_output.PE3_output[k]=RELU<fp6,fp6>(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]));
				 PE_output.PE4_output[k]=RELU<fp6,fp6>(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input,PE_weight.N4_weight,NInputs[i]));
				 PE_output.PE5_output[k]=RELU<fp6,fp6>(PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input,PE_weight.N5_weight,NInputs[i]));
				 PE_output.PE6_output[k]=RELU<fp6,fp6>(PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input,PE_weight.N6_weight,NInputs[i]));
				 PE_output.PE7_output[k]=RELU<fp6,fp6>(PE7_Unit<fp6, fp1, fp2>(PE_input.N7_input,PE_weight.N7_weight,NInputs[i]));
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
			PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]));
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
			PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]));
			PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]));
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
			PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]));
			PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]));
			PE_output.PE2_output[NPartions[i]]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]));
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
			PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]));
			PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]));
			PE_output.PE2_output[NPartions[i]]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]));
			PE_output.PE3_output[NPartions[i]]=RELU<fp6,fp6>(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]));
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
			PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]));
			PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]));
			PE_output.PE2_output[NPartions[i]]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]));
			PE_output.PE3_output[NPartions[i]]=RELU<fp6,fp6>(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]));
			PE_output.PE4_output[NPartions[i]]=RELU<fp6,fp6>(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input,PE_weight.N4_weight,NInputs[i]));
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
			PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]));
			PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]));
			PE_output.PE2_output[NPartions[i]]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]));
			PE_output.PE3_output[NPartions[i]]=RELU<fp6,fp6>(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]));
			PE_output.PE4_output[NPartions[i]]=RELU<fp6,fp6>(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input,PE_weight.N4_weight,NInputs[i]));
			PE_output.PE5_output[NPartions[i]]=RELU<fp6,fp6>(PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input,PE_weight.N5_weight,NInputs[i]));
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
			PE_output.PE0_output[NPartions[i]]=RELU<fp6,fp6>(PE0_Unit<fp6, fp1, fp2>(PE_input.N0_input,PE_weight.N0_weight,NInputs[i]));
			PE_output.PE1_output[NPartions[i]]=RELU<fp6,fp6>(PE1_Unit<fp6, fp1, fp2>(PE_input.N1_input,PE_weight.N1_weight,NInputs[i]));
			PE_output.PE2_output[NPartions[i]]=RELU<fp6,fp6>(PE2_Unit<fp6, fp1, fp2>(PE_input.N2_input,PE_weight.N2_weight,NInputs[i]));
			PE_output.PE3_output[NPartions[i]]=RELU<fp6,fp6>(PE3_Unit<fp6, fp1, fp2>(PE_input.N3_input,PE_weight.N3_weight,NInputs[i]));
			PE_output.PE4_output[NPartions[i]]=RELU<fp6,fp6>(PE4_Unit<fp6, fp1, fp2>(PE_input.N4_input,PE_weight.N4_weight,NInputs[i]));
			PE_output.PE5_output[NPartions[i]]=RELU<fp6,fp6>(PE5_Unit<fp6, fp1, fp2>(PE_input.N5_input,PE_weight.N5_weight,NInputs[i]));
			PE_output.PE6_output[NPartions[i]]=RELU<fp6,fp6>(PE6_Unit<fp6, fp1, fp2>(PE_input.N6_input,PE_weight.N6_weight,NInputs[i]));
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
  	   	return sum;
  	   	//return RELU<T,T>(sum);
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
  	   	return sum;
  	   	//return RELU<T,T>(sum);
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
  	   	return sum;
  	   	//return RELU<T,T>(sum);
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
  	   	return sum;
  	   	//return RELU<T,T>(sum);
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
  	   	return sum;
  	   	//return RELU<T,T>(sum);
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
  	   	return sum;
  	   	//return RELU<T,T>(sum);
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
 	     //cout<< "in_fifo["<<i<<"]:" << in_fifo[i] <<"    "<< "weight_fifo["<<i<<"]:" << weight_fifo[i] << '\n';
      #endif
   	  		 // printf("in_fifo[%d]=%f            ",i,in_fifo[i]);
   	  		 // printf("      weight_fifo[%d]=%f \n",i,weight_fifo[i]);
   	  	  	  	  	  }

   	  #ifndef __SYNTHESIS__
  	   	     cout<< "PE6-----------:" << sum << "   " << '\n'<<'\n';
	  #endif
  	   	return sum;
  	   	//return RELU<T,T>(sum);
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
  	   	return sum;
  	   	//return RELU<T,T>(sum);
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

