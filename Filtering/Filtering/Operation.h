#pragma once
#include<iostream>
#include<gdal_priv.h>
#include<gdal.h>
//中值滤波
void MedianFilter(GDALDataset* Img);
//均值滤波
void MeanFilter(GDALDataset* Img);