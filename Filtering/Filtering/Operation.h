#pragma once
#include<iostream>
#include<gdal_priv.h>
#include<gdal.h>
//��ֵ�˲�
void MedianFilter(GDALDataset* Img);
//��ֵ�˲�
void MeanFilter(GDALDataset* Img);