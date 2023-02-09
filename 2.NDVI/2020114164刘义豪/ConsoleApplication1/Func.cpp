#include "Func.h"
#include<iostream>
#include "gdal.h"
#include "gdal_priv.h"

void CalcuNDVI(GDALDataset* mDataset, GDALDataset*& mNDVIset)
{
	// 获取影像的参数
	int mX = mDataset->GetRasterXSize();
	int mY = mDataset->GetRasterYSize();
	int mDataType = mDataset->GetRasterBand(1)->GetRasterDataType();

	// 将影像中的四个波段取出,分别为NIR，R,G,B
	GDALRasterBand* mB = mDataset->GetRasterBand(1);
	GDALRasterBand* mG = mDataset->GetRasterBand(2);
	GDALRasterBand* mR = mDataset->GetRasterBand(3);
	GDALRasterBand* mNIR = mDataset->GetRasterBand(4);

	// 申请缓冲区
	float* RedBuf = (float*)CPLMalloc(sizeof(float) * mX * mY);
	float* NIRBuf = (float*)CPLMalloc(sizeof(float) * mX * mY);
	float* mNDVIband = (float*)CPLMalloc(sizeof(float) * mX * mY);



	// 从R和NIR波段中取出数据放到缓冲区
	mR->RasterIO(GF_Read, 0, 0, mX, mY, RedBuf, mX, mY, GDT_UInt32, 0, 0);
	mNIR->RasterIO(GF_Read, 0, 0, mX, mY, NIRBuf, mX, mY, GDT_UInt32, 0, 0);

	// 计算NDVI
	for (int i = 0; i < mY; i++)
	{
		for (int j = 0; j < mX; j++)
		{
			float r = RedBuf[i * mX + j];
			float nir = NIRBuf[i * mX + j];

			mNDVIband[i * mX + j] = (nir - r) / (nir + r);
		}
	}


	// 将计算结果写入NDVI的数据集中
	mNDVIset->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, mX, mY, mNDVIband, mX, mY, GDT_Float32, 0, 0);

	// 释放内存
	CPLFree(RedBuf);
	CPLFree(NIRBuf);
	CPLFree(mNDVIband);

}


void ExtractVegetation(GDALDataset* mDataset, GDALDataset* mNDVIset, GDALDataset*& VegetationSet, float th1, float th2)
{
	// 获取影像参数
	int mX = mDataset->GetRasterXSize();
	int mY = mDataset->GetRasterYSize();
	GDALDataType mImgDataType = mDataset->GetRasterBand(1)->GetRasterDataType();
	GDALDataType mNDVIDataType = mNDVIset->GetRasterBand(1)->GetRasterDataType();

	// 获取原始影像的四个波段
	GDALRasterBand* mBand1 = mDataset->GetRasterBand(1);
	GDALRasterBand* mBand2 = mDataset->GetRasterBand(2);
	GDALRasterBand* mBand3 = mDataset->GetRasterBand(3);
	GDALRasterBand* mBand4 = mDataset->GetRasterBand(4);

	// 获取NDVI的波段
	GDALRasterBand* mNDVIBand = mNDVIset->GetRasterBand(1);

	// 申请缓冲区，用来保存原始波段和NDVI的数据
	unsigned short* mBandBuf1 = (unsigned short*)CPLMalloc(mX * mY * mImgDataType);
	unsigned short* mBandBuf2 = (unsigned short*)CPLMalloc(mX * mY * mImgDataType);
	unsigned short* mBandBuf3 = (unsigned short*)CPLMalloc(mX * mY * mImgDataType);
	unsigned short* mBandBuf4 = (unsigned short*)CPLMalloc(mX * mY * mImgDataType);

	float* mNDVIBandBuf = (float*)CPLMalloc(mX * mY * mNDVIDataType);

	// 将各个波段的数据放入对应的缓冲区
	mBand1->RasterIO(GF_Read, 0, 0, mX, mY, mBandBuf1, mX, mY, mImgDataType, 0, 0);
	mBand2->RasterIO(GF_Read, 0, 0, mX, mY, mBandBuf2, mX, mY, mImgDataType, 0, 0);
	mBand3->RasterIO(GF_Read, 0, 0, mX, mY, mBandBuf3, mX, mY, mImgDataType, 0, 0);
	mBand4->RasterIO(GF_Read, 0, 0, mX, mY, mBandBuf4, mX, mY, mImgDataType, 0, 0);

	mNDVIBand->RasterIO(GF_Read, 0, 0, mX, mY, mNDVIBandBuf, mX, mY, mNDVIDataType, 0, 0);

	// 申请缓冲区，用来保存提取出的植被区域
	unsigned short* mVegBand1 = (unsigned short*)CPLMalloc(mX * mY * mImgDataType);
	unsigned short* mVegBand2 = (unsigned short*)CPLMalloc(mX * mY * mImgDataType);
	unsigned short* mVegBand3 = (unsigned short*)CPLMalloc(mX * mY * mImgDataType);
	unsigned short* mVegBand4 = (unsigned short*)CPLMalloc(mX * mY * mImgDataType);

	//  通过阈值来提取植被区域
	for (int i = 0; i < mX * mY; i++)
	{
		// 若在阈值范围内，赋对应的值，若不在，赋值为0
		if (mNDVIBandBuf[i] >= th1 && mNDVIBandBuf[i] <= th2)
		{
			mVegBand1[i] = mBandBuf1[i];
			mVegBand2[i] = mBandBuf2[i];
			mVegBand3[i] = mBandBuf3[i];
			mVegBand4[i] = mBandBuf4[i];
		}
		else
		{
			mVegBand1[i] = 0;
			mVegBand2[i] = 0;
			mVegBand3[i] = 0;
			mVegBand4[i] = 0;
		}
	}

	// 把提取结果放到数据集中
	VegetationSet->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, mX, mY, mVegBand1, mX, mY, mImgDataType, 0, 0);
	VegetationSet->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, mX, mY, mVegBand2, mX, mY, mImgDataType, 0, 0);
	VegetationSet->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, mX, mY, mVegBand3, mX, mY, mImgDataType, 0, 0);
	VegetationSet->GetRasterBand(4)->RasterIO(GF_Write, 0, 0, mX, mY, mVegBand4, mX, mY, mImgDataType, 0, 0);

	// 释放内存
	CPLFree(mBandBuf1);
	CPLFree(mBandBuf2);
	CPLFree(mBandBuf3);
	CPLFree(mBandBuf4);
	CPLFree(mNDVIBandBuf);
	CPLFree(mVegBand1);
	CPLFree(mVegBand2);
	CPLFree(mVegBand3);
	CPLFree(mVegBand4);
}
