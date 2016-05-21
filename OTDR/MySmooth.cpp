#include "StdAfx.h"
#include "MySmooth.h"
//WORD* data_from_file;
CMySmooth::CMySmooth(void)
{
	//data_from_file = new WORD[128000];
}

CMySmooth::~CMySmooth(void)
{
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//pSrcData:从文件或测试出来的原始数据(WORD)；nLen:数据点个数；pDestData:平滑处理后的数据，数据点个数与
//传入的一样.nPulseWidth:脉宽(如10)；nFrequency:频率，如25
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
void CMySmooth::Smooth(WORD* pSrcData, INT nLen, WORD* pDestData, INT nPulseWidth, INT nFrequency)
{
	Smooth(pSrcData, nLen, pDestData, GetRepeatNum(nPulseWidth, nFrequency));
}
void CMySmooth::Smooth(WORD* pSrcData, INT nLen, WORD* pDestData, INT nRepeatNum)
{
	INT nSum;
	INT nLenZero = nLen - 1;//以0为下标长度
	INT nHalfRepeatNum = nRepeatNum / 2;
	INT nHalfRepeatNumOne = nHalfRepeatNum - 1;
	INT nTemp, nCountTemp;
	//nRepeatNum --;
	for(int i=0; i<nLen; i++)
	{
		nSum = 0;
		if (nLenZero == i)
		{
			pDestData[i] = pSrcData[i];
		}
		else if (i < nHalfRepeatNum)
		{
			nTemp = i * 2 - 1;
			for (int j=0; j< nTemp; j++)
			{
				nSum += pSrcData[j];
			}//efor
			pDestData[i] = nSum / (nTemp);
		}
		else if (nLenZero - i < nHalfRepeatNumOne)
		{
			nTemp = (nLenZero - i) * 2 + 1;
			//for (int j=0; j< nTemp; j++)
			//{
			//	nSum += pSrcData[nLenZero - j];
			//}//efor
			nCountTemp = nLenZero - nTemp;
			for (int j=nLenZero; j> nCountTemp; j--)
			{
				nSum += pSrcData[j];
			}//efor
			pDestData[i] = nSum / (nTemp);
		}
		else
		{
			nTemp = i - (nRepeatNum + 1)/2;
			nCountTemp = nTemp + nRepeatNum;
			//for (int j=0; j<nRepeatNum; j++)
			for (int j=nTemp; j<nCountTemp; j++)
			{
				//nSum += pSrcData[i - (nRepeatNum + 1)/2 + j];
				nSum += pSrcData[j];
			}//efor
			pDestData[i] = nSum / nRepeatNum;
		}//eif
		//MYTRACE_INT(_T("i:%d\n"),i);
	}//efor
}
void CMySmooth::Smooth(int N,WORD input_data[],INT paramLen, WORD data_from_file[])//
{
	int i=0,j=0;
	float sum=0;
	unsigned int M=paramLen;
	CString temp;
	for (i = 0;i<M;i++)
	{
		sum = 0;
		if(i==M-1)
			data_from_file[i] = input_data[i];
		else if(i<N/2)
		{
			for (j = 0;j<(2*i-1);j++)
				sum = sum + input_data[j];
			data_from_file[i]=sum/(2*i-1);
		}
		else if((M-i)<(N/2-1))
		{
			for (j =0;j<(2*(M-i)+1);j++)
				sum = sum + input_data[M+1-j];
			data_from_file[i]=sum/(2*(M-i)+1);
		}
		else
		{
			for (j=0;j<N;j++)
				sum = sum + input_data[i-(N+1)/2+j]; 
			//for (j=0;j<N-1;j++)
			//	sum = sum + input_data[i-(N)/2+j]; 
			data_from_file[i] = sum/N;
		}
		//temp.Format(_T("i:%d %d\n"),i,data_from_file[i]);
		//MYTRACE(temp);
	}
}
//得到平均个数
INT CMySmooth::GetRepeatNum(INT nPulseWidth, INT nFrequency)
{
	INT nRepeatNum = 2;
	switch(nPulseWidth)
	{
	case 3://3ns
		{
			/*switch(nRange)
			{
			case 25:
				nRepeatNum = 2;
			}*/
			nRepeatNum = 2;
			break;
		}//ecase
	case 5://
		{
			/*switch(nRange)
			{
			case 25:
				nRepeatNum = 2;
			}*/
			nRepeatNum = 2;
			break;
		}//ecase
	case 10://
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 2;
				break;
			case 50:
				nRepeatNum = 2;
				break;
			case 100:
				nRepeatNum = 4;
				break;
			case 200:
				nRepeatNum = 4;
				break;
			case 400:
				nRepeatNum = 4;
				break;
			}//ecase
			break;
		}//ecase
	case 20://
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 2;
				break;
			case 50:
				nRepeatNum = 2;
				break;
			case 100:
				nRepeatNum = 4;
				break;
			case 200:
				nRepeatNum = 4;
				break;
			case 400:
				nRepeatNum = 4;
				break;
			}//ecase
			break;
		}//ecase
	case 50://ns
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 2;
				break;
			case 50:
				nRepeatNum = 2;
				break;
			case 100:
				nRepeatNum = 4;
				break;
			case 200:
				nRepeatNum = 4;
				break;
			case 400:
				nRepeatNum = 4;
				break;
			}//ecase
			break;
		}//ecase
	case 100://ns
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 4;
				break;
			case 50:
				nRepeatNum = 4;
				break;
			case 100:
				nRepeatNum = 6;
				break;
			case 200:
				nRepeatNum = 6;
				break;
			case 400:
				nRepeatNum = 8;
				break;
			}//ecase
			break;
		}//ecase
	case 200://ns
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 4;
				break;
			case 50:
				nRepeatNum = 4;
				break;
			case 100:
				nRepeatNum = 6;
				break;
			case 200:
				nRepeatNum = 6;
				break;
			case 400:
				nRepeatNum = 8;
				break;
			}//ecase
			break;
		}//ecase
	case 500://ns
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 8;
				break;
			case 50:
				nRepeatNum = 8;
				break;
			case 100:
				nRepeatNum = 10;
				break;
			case 200:
				nRepeatNum = 10;
				break;
			case 400:
				nRepeatNum = 20;
				break;
			}//ecase
			break;
		}//ecase
	case 1000://us
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 20;
				break;
			case 50:
				nRepeatNum = 20;
				break;
			case 100:
				nRepeatNum = 30;
				break;
			case 200:
				nRepeatNum = 30;
				break;
			case 400:
				nRepeatNum = 30;
				break;
			}//ecase
			break;
		}//ecase
	case 2000://us
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 30;
				break;
			case 50:
				nRepeatNum = 30;
				break;
			case 100:
				nRepeatNum = 40;
				break;
			case 200:
				nRepeatNum = 40;
				break;
			case 400:
				nRepeatNum = 40;
				break;
			}//ecase
			break;
		}//ecase
	case 5000://us
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 40;
				break;
			case 50:
				nRepeatNum = 40;
				break;
			case 100:
				nRepeatNum = 50;
				break;
			case 200:
				nRepeatNum = 50;
				break;
			case 400:
				nRepeatNum = 50;
				break;
			}//ecase
			break;
		}//ecase
	case 10000://us
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 50;
				break;
			case 50:
				nRepeatNum = 50;
				break;
			case 100:
				nRepeatNum = 60;
				break;
			case 200:
				nRepeatNum = 60;
				break;
			case 400:
				nRepeatNum = 60;
				break;
			}//ecase
			break;
		}//ecase
	case 20000://20us
		{
			switch(nFrequency)
			{
			case 25:
				nRepeatNum = 100;
				break;
			case 50:
				nRepeatNum = 100;
				break;
			case 100:
				nRepeatNum = 100;
				break;
			case 200:
				nRepeatNum = 100;
				break;
			case 400:
				nRepeatNum = 100;
				break;
			}//ecase
			break;
		}//ecase
	}//eswitch

	return nRepeatNum;
}