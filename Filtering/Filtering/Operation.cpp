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

	// 获取影像的参数
	int mXsize = Img->GetRasterXSize();
	int mYsize = Img->GetRasterYSize();
	int mBandNum = Img->GetRasterCount();//波段数
	GDALDataType mDataType = Img->GetRasterBand(1)->GetRasterDataType();
	
	// 申请缓冲区,OldBuf 用来保存从影像中读取的数据，NewBuf写入滤波后的数据
	unsigned char* OldBuf = new unsigned char[mXsize * mYsize * mDataType];
	unsigned char* NewBuf = new unsigned char[mXsize * mYsize * mDataType];

	// 将影像的一个波段取出，此处使用的影像只有一个波段
	GDALRasterBand* mBand1 = Img->GetRasterBand(1);

	// 将波段中的数据写到OldBuf中
	mBand1->RasterIO(GF_Read, 0, 0, mXsize, mYsize, OldBuf, mXsize, mYsize, mDataType, 0, 0);
	
	for (int i = 0; i < mYsize; i++)		//给最外层赋值
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
	for (int i = 1; i < mYsize - 1; i++)		// 最外层一圈的像素不做处理
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



	// 新建一个驱动，影像格式为GTiff
	GDALDriver* hDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

	// 创建一个输出的数据集
	char outpath[] = "D:\\Study\\大三\\数字图像处理\\2020114164刘义豪-实验一\\Nosie_Result_Median.tif";
	GDALDataset* outDstDS = hDriver->Create(outpath, mXsize, mYsize, 1, mDataType, NULL);

	// 将NewBuf写到输出数据集中
	outDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, mXsize, mYsize, NewBuf, mXsize, mYsize, mDataType, 0, 0);

	// 释放建立的指针，释放内存
	delete[] OldBuf, NewBuf;
	GDALClose(outDstDS);
}


void MeanFilter(GDALDataset* Img) {
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	//获取长、宽、波段数以及数据类型
	int mXsize = Img->GetRasterXSize();
	int mYsize = Img->GetRasterYSize();
	int mBandNum = Img->GetRasterCount();
	GDALDataType mDataType = Img->GetRasterBand(1)->GetRasterDataType();

	//缓冲区用以储存计算内容
	unsigned char* OldBuf = new unsigned char[mXsize * mYsize * mDataType];
	unsigned char* NewBuf = new unsigned char[mXsize * mYsize * mDataType];

	//取出波段，另外调用GDALRasterBand的方法
	GDALRasterBand* mBand1 = Img->GetRasterBand(1);
	mBand1->RasterIO(GF_Read, 0, 0, mXsize, mYsize, OldBuf, mXsize, mYsize, mDataType, 0, 0);

	//最外层赋值
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

	//均值滤波处理
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

	// 新建一个驱动，影像格式为GTiff
	GDALDriver* hDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

	// 创建一个输出的数据集
	char outpath[] = "D:\\Study\\大三\\数字图像处理\\2020114164刘义豪-实验一\\Nosie_Result_Mean.tif";
	GDALDataset* outDstDS = hDriver->Create(outpath, mXsize, mYsize, 1, mDataType, NULL);

	// 将NewBuf写到输出数据集中
	outDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, mXsize, mYsize, NewBuf, mXsize, mYsize, mDataType, 0, 0);

	// 释放建立的指针，释放内存
	delete[] OldBuf, NewBuf;
	GDALClose(outDstDS);
}