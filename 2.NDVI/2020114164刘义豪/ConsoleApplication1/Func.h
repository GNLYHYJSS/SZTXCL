#pragma once
#include <iostream>
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

using namespace std;

// ����NDVI,mDatasetΪ�������ݣ�mNDVIΪ������
void CalcuNDVI(GDALDataset* mDataset, GDALDataset*& mNDVI);

// ����NDVI��������ֵ�ָ�ķ�ʽ��ȡֲ��,mDatasetΪԭʼӰ��
// mNDVIsetΪNDVI���ݣ���ֵ��[-1,+1]֮�䣬VegetationSetΪ��ȡ����ֲ������,th1Ϊ����ֵ��th2Ϊ����ֵ
void ExtractVegetation(GDALDataset* mDataset, GDALDataset* mNDVIset, GDALDataset*& VegetationSet, float th1, float th2);
