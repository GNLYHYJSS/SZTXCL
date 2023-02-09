#include "NCC.h"
#include "stdio.h"
#include <cmath>

bool CalNCC(GDALDataset* imgData, GDALDataset* tmpData, int& x, int& y) {
	int imgX = imgData->GetRasterXSize();
	int imgY = imgData->GetRasterYSize();
	int imgBandNum = imgData->GetRasterCount();
	GDALDataType imgType = imgData->GetRasterBand(1)->GetRasterDataType();

	int tmpX = tmpData->GetRasterXSize();
	int tmpY = tmpData->GetRasterYSize();
	int tmpBandNum = tmpData->GetRasterCount();
	GDALDataType tmpType = tmpData->GetRasterBand(1)->GetRasterDataType();

	if (tmpX > imgX || tmpY > imgY || tmpX % 2 == 0 || tmpY % 2 == 0) {
		std::cout << "模板数据有误！" << std::endl;
		system("pause");
		return false;
	}

	GDALRasterBand* imgBand = imgData->GetRasterBand(1);
	GDALRasterBand* tmpBand = tmpData->GetRasterBand(1);

	unsigned char* imgBuff = (unsigned char*)CPLMalloc((double)imgX * imgY * imgType);
	unsigned char* tmpBuff = (unsigned char*)CPLMalloc((double)tmpX * tmpY * tmpType);

	imgBand->RasterIO(GF_Read, 0, 0, imgX, imgY, imgBuff, imgX, imgY, imgType, 0, 0);
	tmpBand->RasterIO(GF_Read, 0, 0, tmpX, tmpY, tmpBuff, tmpX, tmpY, tmpType, 0, 0);

	int maxX = 0;
	int maxY = 0;
	float maxNcc = 0;

	int mTmpX = tmpX / 2;
	int mTmpY = tmpY / 2;

	//遍历原图所有点位
	for (int i = 0; i < imgY - tmpY; i++) {
		for (int j = 0; j < imgX - tmpX; j++) {
			//每次到一个新点位，将像素和与平均值归零
			double sumTmp = 0, sumImg = 0, aveTmp = 0, aveImg = 0;
			//计算当前位置原图和模板像素平均值
			for (int t = 0; t < tmpY; t++) {
				for (int k = 0; k < tmpX; k++) {
					//模板当前像素值
					sumTmp += (double)tmpBuff[t * tmpX + k];
					//原图当前位置像素值
					sumImg += (double)imgBuff[(t + i) * imgX + (j + k)];
				}
			}
			//计算当前位置原图和模版的像素平均值
			aveImg = sumImg / ((double)imgX * imgY);
			aveTmp = sumTmp / ((double)tmpX * tmpY);

			//利用上式计算出当前位置的平均像素值，计算公式
			double sumImg2 = 0, sumTmp2 = 0, Mul = 0;
			//遍历当前位置所有位于模版位置内的模板元素和原图元素到达累和的效果
			for (int m = 0; m < imgY - tmpY; m++) {
				for (int n = 0; n < imgX - tmpX; n++) {
					//原图的i行j列元素减掉平均值，累和
					sumImg2 += pow(((double)imgBuff[(i + m) * imgX + (j + n)] - aveImg), 2);
					//模板的i行j列元素减掉平均值，累和
					sumTmp2 += pow(((double)tmpBuff[m * tmpX + n] - aveTmp), 2);
					//原图的i行j列元素减掉平均值，乘以，模板的i行j列元素减掉平均值
					Mul = ((double)imgBuff[(i + m) * imgX + (j + n)] - aveImg) * ((double)tmpBuff[m * tmpX + n] - aveTmp);
				}
			}
			//每一个位置计算一个NCC值
			double Ncc = Mul / sqrt(sumImg2 * sumTmp2);

			//判断是否为最大值NCC
			if (Ncc>maxNcc) {
				maxNcc = Ncc;
				maxX = j;
				maxY = i;
			}
			//该位置（I行J列）判断完毕，进行下一个位置
		}
	}
	//找到中心位置
	x = maxX + 1 + mTmpX;
	y = maxY + 1 + mTmpY;
	CPLFree(tmpBuff);
	CPLFree(imgBuff);

	return true;
}