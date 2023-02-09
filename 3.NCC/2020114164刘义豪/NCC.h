#pragma once
#include <iostream>
#include<iostream>
#include "gdal.h"
#include "gdal_priv.h"

bool CalNCC(GDALDataset* imgData, GDALDataset* tmpData, int& x, int& y);