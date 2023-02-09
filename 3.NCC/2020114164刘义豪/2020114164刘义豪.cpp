#include <iostream>
#include<iostream>
#include "gdal.h"
#include "gdal_priv.h"
#include "NCC.h"


int main()
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	char imgPath[] = "D:\\Study\\大三\\数字图像处理\\2020114164刘义豪实验三\\img.tif";
	char tmpPath[] = "D:\\Study\\大三\\数字图像处理\\2020114164刘义豪实验三\\template.tif";

	GDALDataset* imgData = (GDALDataset*)GDALOpen(imgPath, GA_ReadOnly);
	GDALDataset* tmpData = (GDALDataset*)GDALOpen(tmpPath, GA_ReadOnly);

	if (imgData == NULL || tmpData == NULL)
	{
		std::cout << "未能正确加载数据!!!" <<std:: endl;
		system("pause");
		GDALDestroyDriverManager();
	}

	int x = 0;
	int y = 0;
	bool flag;
	flag = CalNCC(imgData, tmpData, x, y);

	if (flag==false)
	{
		return 0;
	}
	std::cout << "模板中心像素与原图像在：" << std::endl;
	std::cout << "第" << x << "行，第" << y << "列" << std::endl;

	GDALClose(imgData);
	GDALClose(tmpData);
	GDALDestroyDriverManager();

	return 0;
}