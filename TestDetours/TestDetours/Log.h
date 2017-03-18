#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>		// linux��ͷ�ļ�
#endif

#define FILE_MAX_SIZE (1024*1024*32)

/*
��õ�ǰʱ���ַ���
@param buffer [out]: ʱ���ַ���
@return ��
*/
void get_local_time(char* buffer)
{
	time_t rawtime; 
	struct tm* timeinfo;

	time(&rawtime); 
	timeinfo = localtime(&rawtime); 
	sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d", 
		(timeinfo->tm_year+1900), timeinfo->tm_mon, timeinfo->tm_mday,
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

/*
����ļ���С
@param filename [in]: �ļ���
@return �ļ���С
*/
long get_file_size(char* filename)
{
	long length = 0;
	FILE *fp = NULL;

	fp = fopen(filename, "rb");
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
	}

	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}

	return length;
}

/*
д����־�ļ�
@param filename [in]: ��־�ļ���
@param max_size [in]: ��־�ļ���С����
@param buffer [in]: ��־����
@param buf_size [in]: ��־���ݴ�С
@return ��
*/
void write_log_file(char* filename, long max_size, const char* buffer, unsigned buf_size)
{
	if (filename != NULL && buffer != NULL)
	{
		// �ļ������������, ɾ��
		long length = get_file_size(filename);

		if (length > max_size)
		{
			unlink(filename); // ɾ���ļ�
		}

		// д��־
		{
			FILE *fp;
			fp = fopen(filename, "at+");
			if (fp != NULL)
			{
				/*char now[32];
				memset(now, 0, sizeof(now));
				get_local_time(now);
				fwrite(now, strlen(now)+1, 1, fp);*/
				fwrite(buffer, buf_size, 1, fp);

				fclose(fp);
				fp = NULL;
			}
		}
	}
}

//int main(int argc, char** argv)
//{
//	int i;
//	for (i=0; i<10; ++i)
//	{
//		char buffer[32];
//		memset(buffer, 0, sizeof(buffer));
//		sprintf(buffer, "====> %d\n", i);
//		write_log_file("log.txt", FILE_MAX_SIZE, buffer, strlen(buffer));
//#ifdef WIN32
//		Sleep(100); // ����
//#else
//		sleep(1);	// ��
//#endif
//
//	}
//	// system("pause");
//	return 0;
//}