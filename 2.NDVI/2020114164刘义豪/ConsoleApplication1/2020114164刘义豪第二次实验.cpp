/*
* 用于学习GDAL编程 实验二 计算植被指数，并通过阈值法提取植被
* 时间 20210705
* 本次实验采用的影像为四波段的影像
*/

#include<iostream>
#include "gdal.h"
#include "gdal_priv.h"
#include "Func.h"

using namespace std;

int main()
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	// 读取影像
	char openpath[] = "D:\\Study\\大三\\数字图像处理\\2020114164刘义豪实验二\\test.tif";

	GDALDataset* mDataset;


	// 只读方式打开
	mDataset = (GDALDataset*)GDALOpen(openpath, GA_ReadOnly);

	if (mDataset == NULL)
	{
		cout << "未能正确加载数据!!!" << endl;
		system("pause");
		GDALDestroyDriverManager();

	}
	// 获取影像数据
	int mX = mDataset->GetRasterXSize();
	int mY = mDataset->GetRasterYSize();
	GDALDataType mImgDataType = mDataset->GetRasterBand(1)->GetRasterDataType();

	// 获取影像的地理信息和投影信息
	double geoInformation[6];
	mDataset->GetGeoTransform(geoInformation);
	const char* gdalProjection = mDataset->GetProjectionRef();

	// 新建一个驱动，影像格式为GTiff
	GDALDriver* hDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

	// 调用CalcuNDVI 进行计算，需要先建立一个数据集保存NDVI结果,NDVI中的数据类型是float
	char savepath[] = "D:\\Study\\大三\\数字图像处理\\2020114164刘义豪实验二\\ndvi.tif";
	GDALDataset* mNDVIset = hDriver->Create(savepath, mX, mY, 1, GDT_Float32, NULL);

	cout << "正在计算NDVI" << endl;

	CalcuNDVI(mDataset, mNDVIset);

	// 将影像的地理信息写到NDVI数据集中
	mNDVIset->SetGeoTransform(geoInformation);
	mNDVIset->SetProjection(gdalProjection);


	// 调用ExtractVegetation进行提取，需要先建立一个数据集保存提取结果
	char savepath2[] = "D:\\Study\\大三\\数字图像处理\\2020114164刘义豪实验二\\vege1.tif";
	GDALDataset* mVegetationSet = hDriver->Create(savepath2, mX, mY, 4, mImgDataType, NULL);

	cout << "正在提取植被区域！！！" << endl;

	ExtractVegetation(mDataset, mNDVIset, mVegetationSet, 0.35, 0.7);

	// 将影像的地理信息写到植被数据集中
	mVegetationSet->SetGeoTransform(geoInformation);
	mVegetationSet->SetProjection(gdalProjection);


	// 关闭数据集，释放内存
	GDALClose(mDataset);
	GDALClose(mNDVIset);
	GDALClose(mVegetationSet);
	GDALDestroyDriverManager();

	cout << "处理结束！！！" << endl;
	getchar();

	return 0;
}
