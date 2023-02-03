#include <gdal_priv.h>
#include <iostream>
#include "Operation.h"
#include "stdio.h"
using namespace std;

int main()
{
	//设置驱动以及路径中文设置
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	//输入路径
	char In_Path[] = "D:\\Study\\大三\\数字图像处理\\2020114164刘义豪-实验一\\nosieImg.tif";

	//只读打开影像
	GDALDataset* Data = (GDALDataset*)GDALOpen(In_Path, GA_ReadOnly);

	//无法打开影像
	if (Data == NULL) {
		cout << "无法打开影像" << endl;
		GDALDestroyDriverManager();
	}

	//均值滤波
	MeanFilter(Data);

	//中值滤波
	MedianFilter(Data);


	GDALClose(Data);
	GDALDestroyDriverManager();
	cout << "滤波成功！" << endl;
	system("pause");
}
