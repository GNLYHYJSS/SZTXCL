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
		std::cout << "ģ����������" << std::endl;
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

	//����ԭͼ���е�λ
	for (int i = 0; i < imgY - tmpY; i++) {
		for (int j = 0; j < imgX - tmpX; j++) {
			//ÿ�ε�һ���µ�λ�������غ���ƽ��ֵ����
			double sumTmp = 0, sumImg = 0, aveTmp = 0, aveImg = 0;
			//���㵱ǰλ��ԭͼ��ģ������ƽ��ֵ
			for (int t = 0; t < tmpY; t++) {
				for (int k = 0; k < tmpX; k++) {
					//ģ�嵱ǰ����ֵ
					sumTmp += (double)tmpBuff[t * tmpX + k];
					//ԭͼ��ǰλ������ֵ
					sumImg += (double)imgBuff[(t + i) * imgX + (j + k)];
				}
			}
			//���㵱ǰλ��ԭͼ��ģ�������ƽ��ֵ
			aveImg = sumImg / ((double)imgX * imgY);
			aveTmp = sumTmp / ((double)tmpX * tmpY);

			//������ʽ�������ǰλ�õ�ƽ������ֵ�����㹫ʽ
			double sumImg2 = 0, sumTmp2 = 0, Mul = 0;
			//������ǰλ������λ��ģ��λ���ڵ�ģ��Ԫ�غ�ԭͼԪ�ص����ۺ͵�Ч��
			for (int m = 0; m < imgY - tmpY; m++) {
				for (int n = 0; n < imgX - tmpX; n++) {
					//ԭͼ��i��j��Ԫ�ؼ���ƽ��ֵ���ۺ�
					sumImg2 += pow(((double)imgBuff[(i + m) * imgX + (j + n)] - aveImg), 2);
					//ģ���i��j��Ԫ�ؼ���ƽ��ֵ���ۺ�
					sumTmp2 += pow(((double)tmpBuff[m * tmpX + n] - aveTmp), 2);
					//ԭͼ��i��j��Ԫ�ؼ���ƽ��ֵ�����ԣ�ģ���i��j��Ԫ�ؼ���ƽ��ֵ
					Mul = ((double)imgBuff[(i + m) * imgX + (j + n)] - aveImg) * ((double)tmpBuff[m * tmpX + n] - aveTmp);
				}
			}
			//ÿһ��λ�ü���һ��NCCֵ
			double Ncc = Mul / sqrt(sumImg2 * sumTmp2);

			//�ж��Ƿ�Ϊ���ֵNCC
			if (Ncc>maxNcc) {
				maxNcc = Ncc;
				maxX = j;
				maxY = i;
			}
			//��λ�ã�I��J�У��ж���ϣ�������һ��λ��
		}
	}
	//�ҵ�����λ��
	x = maxX + 1 + mTmpX;
	y = maxY + 1 + mTmpY;
	CPLFree(tmpBuff);
	CPLFree(imgBuff);

	return true;
}