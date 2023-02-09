#pragma once
#include <iostream>
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

using namespace std;

// 计算NDVI,mDataset为输入数据，mNDVI为计算结果
void CalcuNDVI(GDALDataset* mDataset, GDALDataset*& mNDVI);

// 利用NDVI，采用阈值分割的方式提取植被,mDataset为原始影像，
// mNDVIset为NDVI数据，其值在[-1,+1]之间，VegetationSet为提取出的植被数据,th1为低阈值，th2为高阈值
void ExtractVegetation(GDALDataset* mDataset, GDALDataset* mNDVIset, GDALDataset*& VegetationSet, float th1, float th2);
