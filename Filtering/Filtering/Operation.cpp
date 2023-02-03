#include "Operation.h"
#include <iostream>
#include <gdal_priv.h>
#include <algorithm>
#include "stdio.h"
using namespace std;

void MedianFilter(GDALDataset* Img)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	// ��ȡӰ��Ĳ���
	int mXsize = Img->GetRasterXSize();
	int mYsize = Img->GetRasterYSize();
	int mBandNum = Img->GetRasterCount();//������
	GDALDataType mDataType = Img->GetRasterBand(1)->GetRasterDataType();
	
	// ���뻺����,OldBuf ���������Ӱ���ж�ȡ�����ݣ�NewBufд���˲��������
	unsigned char* OldBuf = new unsigned char[mXsize * mYsize * mDataType];
	unsigned char* NewBuf = new unsigned char[mXsize * mYsize * mDataType];

	// ��Ӱ���һ������ȡ�����˴�ʹ�õ�Ӱ��ֻ��һ������
	GDALRasterBand* mBand1 = Img->GetRasterBand(1);

	// �������е�����д��OldBuf��
	mBand1->RasterIO(GF_Read, 0, 0, mXsize, mYsize, OldBuf, mXsize, mYsize, mDataType, 0, 0);
	
	for (int i = 0; i < mYsize; i++)		//������㸳ֵ
	{
		for (int j = 0; j < mXsize; j++)
		{
			if (i == 0 || j == 0 || i == mXsize - 1 || j == mYsize - 1)
			{
				NewBuf[i * mXsize + j] = OldBuf[i * mXsize + j];
			}
		}
	}

	int temp[9] = { 0 };
	for (int i = 1; i < mYsize - 1; i++)		// �����һȦ�����ز�������
	{
		for (int j = 1; j < mXsize - 1; j++)
		{
			temp[0] = (int)OldBuf[(i - 1) * mXsize + j - 1];
			temp[1] = (int)OldBuf[(i - 1) * mXsize + j];
			temp[2] = (int)OldBuf[(i - 1) * mXsize + j + 1];

			temp[3] = (int)OldBuf[i * mXsize + j - 1];
			temp[4] = (int)OldBuf[i * mXsize + j];
			temp[5] = (int)OldBuf[i * mXsize + j + 1];

			temp[6] = (int)OldBuf[(i + 1) * mXsize + j - 1];
			temp[7] = (int)OldBuf[(i + 1) * mXsize + j];
			temp[8] = (int)OldBuf[(i + 1) * mXsize + j + 1];

			sort(temp, temp + 9);

			NewBuf[i * mXsize + j] = (unsigned char)temp[4];
		}
	}



	// �½�һ��������Ӱ���ʽΪGTiff
	GDALDriver* hDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

	// ����һ����������ݼ�
	char outpath[] = "D:\\Study\\����\\����ͼ����\\2020114164�����-ʵ��һ\\Nosie_Result_Median.tif";
	GDALDataset* outDstDS = hDriver->Create(outpath, mXsize, mYsize, 1, mDataType, NULL);

	// ��NewBufд��������ݼ���
	outDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, mXsize, mYsize, NewBuf, mXsize, mYsize, mDataType, 0, 0);

	// �ͷŽ�����ָ�룬�ͷ��ڴ�
	delete[] OldBuf, NewBuf;
	GDALClose(outDstDS);
}


void MeanFilter(GDALDataset* Img) {
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	//��ȡ�������������Լ���������
	int mXsize = Img->GetRasterXSize();
	int mYsize = Img->GetRasterYSize();
	int mBandNum = Img->GetRasterCount();
	GDALDataType mDataType = Img->GetRasterBand(1)->GetRasterDataType();

	//���������Դ����������
	unsigned char* OldBuf = new unsigned char[mXsize * mYsize * mDataType];
	unsigned char* NewBuf = new unsigned char[mXsize * mYsize * mDataType];

	//ȡ�����Σ��������GDALRasterBand�ķ���
	GDALRasterBand* mBand1 = Img->GetRasterBand(1);
	mBand1->RasterIO(GF_Read, 0, 0, mXsize, mYsize, OldBuf, mXsize, mYsize, mDataType, 0, 0);

	//����㸳ֵ
	for (int i = 0; i < mYsize; i++)		
	{
		for (int j = 0; j < mXsize; j++)
		{
			if (i == 0 || j == 0 || i == mXsize - 1 || j == mYsize - 1)
			{
				NewBuf[i * mXsize + j] = OldBuf[i * mXsize + j];
			}
		}
	}

	//��ֵ�˲�����
	int temp[9] = { 0 };
	for (int i = 1; i < mYsize - 1; i++)		
	{
		for (int j = 1; j < mXsize - 1; j++)
		{
			temp[0] = (int)OldBuf[(i - 1) * mXsize + j - 1];
			temp[1] = (int)OldBuf[(i - 1) * mXsize + j];
			temp[2] = (int)OldBuf[(i - 1) * mXsize + j + 1];

			temp[3] = (int)OldBuf[i * mXsize + j - 1];
			temp[4] = (int)OldBuf[i * mXsize + j];
			temp[5] = (int)OldBuf[i * mXsize + j + 1];

			temp[6] = (int)OldBuf[(i + 1) * mXsize + j - 1];
			temp[7] = (int)OldBuf[(i + 1) * mXsize + j];
			temp[8] = (int)OldBuf[(i + 1) * mXsize + j + 1];

			double sum = temp[0] + temp[1] + temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7] + temp[8];
			double ave = sum / 9;

			NewBuf[i * mXsize + j] = (unsigned char)ave;
		}
	}

	// �½�һ��������Ӱ���ʽΪGTiff
	GDALDriver* hDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

	// ����һ����������ݼ�
	char outpath[] = "D:\\Study\\����\\����ͼ����\\2020114164�����-ʵ��һ\\Nosie_Result_Mean.tif";
	GDALDataset* outDstDS = hDriver->Create(outpath, mXsize, mYsize, 1, mDataType, NULL);

	// ��NewBufд��������ݼ���
	outDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, mXsize, mYsize, NewBuf, mXsize, mYsize, mDataType, 0, 0);

	// �ͷŽ�����ָ�룬�ͷ��ڴ�
	delete[] OldBuf, NewBuf;
	GDALClose(outDstDS);
}