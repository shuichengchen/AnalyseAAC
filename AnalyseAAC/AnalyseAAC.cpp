// AnalyseAAC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "AACFrameBuffer.h"

int main()
{
	FILE*pTest = nullptr;
	fopen_s(&pTest, "Audioout.aac", "rb");
	AACFrameBuffer object;

	char szbuf[512] = { 0 };
	char szCache[1024] = { 0 };
	for (auto i = 0; i < 10000; i++)
	{
		memset(szbuf, 0, 512);
		//Read PCM
		if (fread(szbuf, 1, 512, pTest) <= 0) {
			printf("Failed to read raw data! \n");
			break;
		}
		else if (feof(pTest)) {
			break;
		}
		object.addStreamData(szbuf, 512);
		int ret = object.getNextFrame(szCache, sizeof(szCache));
		if (ret>0)
		{
			int a = 0;
		}
	}
	return 1;

}
