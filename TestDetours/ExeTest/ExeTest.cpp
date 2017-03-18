// ExeTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

//DWORD FindTarget( LPCTSTR lpszProcess )   
//{   
//	DWORD dwRet = 0;   
//	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );   
//	PROCESSENTRY32 pe32;   
//	pe32.dwSize = sizeof( PROCESSENTRY32 );   
//	Process32First( hSnapshot, &pe32 );   
//	do  
//	{   
//		if ( lstrcmpi( pe32.szExeFile, lpszProcess ) == 0 )   
//		{   
//			dwRet = pe32.th32ProcessID;   
//			break;   
//		}   
//	} while ( Process32Next( hSnapshot, &pe32 ) );   
//	CloseHandle( hSnapshot );   
//	return dwRet;   
//} 

int _tmain(int argc, _TCHAR* argv[])
{
	

	HMODULE hMod = LoadLibrary(_T("TestDetours.dll"));

	getchar();

	FreeLibrary(hMod);
	
	return 0;
}
//
//#include "nids.h"        
//char ascii_string[10000];        
//char *char_to_ascii(char ch)        
//{        
//    char *string;        
//    ascii_string[0] = 0;        
//    string = ascii_string;        
//    if (isgraph(ch))        
//    {        
//        *string++ = ch;        
//    }        
//    else if (ch == ' ')        
//    {        
//        *string++ = ch;        
//    }        
//    else if (ch == 'n' || ch == 'r')        
//    {        
//        *string++ = ch;        
//    }        
//    else    
//    {        
//        *string++ = '.';        
//    }        
//    *string = 0;        
//    return ascii_string;        
//}        
///*      
//=======================================================================================================================      
//下面的函数是对浏览器接收的数据进行分析      
//=======================================================================================================================      
// */    
//void parse_client_data(char content[], int number)        
//{        
//    char temp[1024];        
//    char str1[1024];        
//    char str2[1024];        
//    char str3[1024];        
//    int i;        
//    int k;        
//    int j;        
//    char entity_content[1024];        
//    if (content[0] != 'H' && content[1] != 'T' && content[2] != 'T' && content[3] != 'P')        
//    {        
//        printf("实体内容为（续）：n");        
//        for (i = 0; i < number; i++)        
//        {        
//            printf("%s", char_to_ascii(content[i]));        
//        }        
//        printf("n");        
//    }        
//    else    
//    {        
//        for (i = 0; i < strlen(content); i++)        
//        {        
//            if (content[i] != 'n')        
//            {        
//                k++;        
//                continue;        
//            }        
//            for (j = 0; j < k; j++)        
//                temp[j] = content[j + i - k];        
//            temp[j] = '';        
//            if (strstr(temp, "HTTP"))        
//            {        
//                printf("状态行为：");        
//                printf("%sn", temp);        
//                sscanf(temp, "%s %s %s", str1, str2);        
//                printf("HTTP协议为:%sn", str1);        
//                printf("状态代码为:%sn", str2);        
//            }        
//            if (strstr(temp, "Date"))        
//            {        
//                printf("当前的时间为（Date）:%sn", temp + strlen("Date:"));        
//                printf("%sn", temp);        
//            }        
//            if (strstr(temp, "Server"))        
//            {        
//                printf("服务器为（Server）:%sn", temp + strlen("Server:"));        
//                printf("%sn", temp);        
//            }        
//            if (strstr(temp, "Cache-Control"))        
//            {        
//                printf("缓存机制为（Cache-Control）:%sn", temp + strlen("Cache-Control:"));        
//                printf("%sn", temp);        
//            }        
//            if (strstr(temp, "Expires"))        
//            {        
//                printf("资源期限为（Expires）:%sn", temp + strlen("Expires:"));        
//                printf("%sn", temp);        
//            }        
//            if (strstr(temp, "Last-Modified"))        
//            {        
//                printf("最后一次修改的时间为（Last-Modified）:%sn", temp + strlen("Last-Modified:"));        
//                printf("%sn", temp);        
//            }        
//            if (strstr(temp, "ETag"))        
//            {        
//                printf("Etag为（ETag）:%sn", temp + strlen("Etag:"));        
//                printf("%sn", temp);        
//            }        
//            if (strstr(temp, "Accept-Ranges"))        
//            {        
//                printf("Accept-Ranges（Accept-Ranges）:%sn", temp + strlen("Accept-Ranges:"));        
//                printf("%sn", temp);        
//            }        
//            if (strstr(temp, "Content-Length"))        
//            {        
//                printf("内容长度为（Content-Length）:%sn", temp + strlen("Content-Length:"));        
//                printf("%sn", temp);        
//            }        
//            if (strstr(temp, "Connection"))        
//            {        
//                printf("连接状态为（Connection）:%sn", temp + strlen("Connection:"));        
//                printf("%sn", temp);        
//            }        
//            if (strstr(temp, "Content-Type"))        
//            {        
//                printf("内容类型为（Content-Type）:%sn", temp + strlen("Content-Type:"));        
//                printf("%sn", temp);        
//            }        
//            /* 获取实体内容 */    
//            if ((content[i] == 'n') && (content[i + 1] == 'r'))        
//            {        
//                if (i + 3 == strlen(content))        
//                {        
//                    printf("无实体内容n");        
//                    break;        
//                }        
//                for (j = 0; j < number - i - 3; j++)        
//                    entity_content[j] = content[i + 3+j];        
//                entity_content[j] = '';        
//                printf("实体内容为：n");        
//                for (i = 0; i < j; i++)        
//                {        
//                    printf("%s", char_to_ascii(entity_content[i]));        
//                }        
//                printf("n");        
//                break;        
//            }        
//            k = 0;        
//        }        
//    }        
//}        
///*      
//=======================================================================================================================      
//下面的函数是对WEB服务器接收到的数据进行分析      
//=======================================================================================================================      
// */    
//void parse_server_data(char content[], int number)        
//{        
//    char temp[1024];        
//    char str1[1024];        
//    char str2[1024];        
//    char str3[1024];        
//    int i;        
//    int k;        
//    int j;        
//    char entity_content[1024];        
//    for (i = 0; i < strlen(content); i++)        
//    {        
//        if (content[i] != 'n')        
//        {        
//            k++;        
//            continue;        
//        }        
//        for (j = 0; j < k; j++)        
//            temp[j] = content[j + i - k];        
//        temp[j] = '';        
//        if (strstr(temp, "GET"))        
//        {        
//            printf("请求行为：");        
//            printf("%sn", temp);        
//            sscanf(temp, "%s %s %s", str1, str2, str3);        
//            printf("使用的命令为:%sn", str1);        
//            printf("获得的资源为:%sn", str2);        
//            printf("HTTP协议类型为:%sn", str3);        
//        }        
//        if (strstr(temp, "Accept:"))        
//        {        
//            printf("接收的文件包括（Accept:）:%sn", temp + strlen("Accept:"));        
//            printf("%sn", temp);        
//        }        
//        if (strstr(temp, "Referer"))        
//        {        
//            printf("转移地址为（Referer）:%sn", temp + strlen("Referer:"));        
//            printf("%sn", temp);        
//        }        
//        if (strstr(temp, "Accept-Language"))        
//        {        
//            printf("使用的语言为（Accept-Language）:%sn", temp + strlen("Accept-Language:"));        
//            printf("%sn", temp);        
//        }        
//        if (strstr(temp, "Accept-Encoding"))        
//        {        
//            printf("接收的编码方式为（Accept-Encoding）:%sn", temp + strlen("Accept-Encoding:"));        
//            printf("%sn", temp);        
//        }        
//        if (strstr(temp, "If-Modified-Since"))        
//        {        
//            printf("上次修改时间为（If-Modified-Since）:%sn", temp + strlen("If-Modified-Since:"));        
//            printf("%sn", temp);        
//        }        
//        if (strstr(temp, "If-None-Match"))        
//        {        
//            printf("If-None-Match为（If-Modified-Since）:%sn", temp + strlen("If-None-Match:"));        
//            printf("%sn", temp);        
//        }        
//        if (strstr(temp, "User-Agent"))        
//        {        
//            printf("用户的浏览器信息为（User-Agent）:%sn", temp + strlen("User-Agent:"));        
//            printf("%sn", temp);        
//        }        
//        if (strstr(temp, "Host"))        
//        {        
//            printf("访问的主机为（Host）:%sn", temp + strlen("Host:"));        
//            printf("%sn", temp);        
//        }        
//        if (strstr(temp, "Connection"))        
//        {        
//            printf("连接状态为（Connection）:%sn", temp + strlen("Connection:"));        
//            printf("%sn", temp);        
//        }        
//        if (strstr(temp, "Cookie"))        
//        {        
//            printf("Cookie为（Cookie）:%sn", temp + strlen("Cookie:"));        
//            printf("%sn", temp);        
//        }        
//        /* 获取实体内容 */    
//        if ((content[i] == 'n') && (content[i + 1] == 'r') && (content[i + 2] == 'n'))        
//        {        
//            if (i + 3 == strlen(content))        
//            {        
//                printf("无实体内容n");        
//                break;        
//            }        
//            for (j = 0; j < strlen(content) - i - 3; j++)        
//                entity_content[j] = content[i + 3+j];        
//            entity_content[j] = '';        
//            printf("实体内容为：n");        
//            printf("%s", entity_content);        
//            printf("n");        
//            break;        
//        }        
//        k = 0;        
//    }        
//}        
///*      
//=======================================================================================================================      
//下面是回调函数，实现对HTTP协议的分析      
//=======================================================================================================================      
// */    
//void http_protocol_callback(struct tcp_stream *tcp_http_connection, void **param)        
//{        
//    char address_content[1024];        
//    char content[65535];        
//    char content_urgent[65535];        
//    struct tuple4 ip_and_port = tcp_http_connection->addr;        
//    strcpy(address_content, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));        
//    sprintf(address_content + strlen(address_content), " : %i", ip_and_port.source);        
//    strcat(address_content, " <----> ");        
//    strcat(address_content, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));        
//    sprintf(address_content + strlen(address_content), " : %i", ip_and_port.dest);        
//    strcat(address_content, "n");        
//    if (tcp_http_connection->nids_state == NIDS_JUST_EST)        
//    {        
//        if (tcp_http_connection->addr.dest != 80)        
//         /* 只捕获HTTP协议数据包 */    
//        {        
//            return ;        
//        }        
//        tcp_http_connection->client.collect++; /* 浏览器接收数据 */    
//        tcp_http_connection->server.collect++; /* WEB服务器端接收数据 */    
//        printf("nnn==============================================n");        
//        printf("%s 建立连接...n", address_content);        
//        return ;        
//    }        
//    if (tcp_http_connection->nids_state == NIDS_CLOSE)        
//    {        
//        printf("--------------------------------n");        
//        printf("%s连接正常关闭...n", address_content);        
//        /* 连接正常关闭 */    
//        return ;        
//    }        
//    if (tcp_http_connection->nids_state == NIDS_RESET)        
//    {        
//        printf("--------------------------------n");        
//        printf("%s连接被RST关闭...n", address_content);        
//        /* 连接被RST关闭 */    
//        return ;        
//    }        
//    if (tcp_http_connection->nids_state == NIDS_DATA)        
//    {        
//        struct half_stream *hlf;        
//        if (tcp_http_connection->client.count_new)        
//         /* 浏览器接收数据 */    
//        {        
//            hlf = &tcp_http_connection->client;        
//            /* hlft表示浏览器接收的数据 */    
//            strcpy(address_content, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));        
//            sprintf(address_content + strlen(address_content), ":%i", ip_and_port.source);        
//            strcat(address_content, " <---- ");        
//            strcat(address_content, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));        
//            sprintf(address_content + strlen(address_content), ":%i", ip_and_port.dest);        
//            strcat(address_content, "n");        
//            printf("n");        
//            printf("%s", address_content);        
//            printf("浏览器接收数据...n");        
//            printf("n");        
//            memcpy(content, hlf->data, hlf->count_new);        
//            content[hlf->count_new] = '';        
//            parse_client_data(content, hlf->count_new);        
//            /* 分析浏览器接收的数据 */    
//        }        
//        else    
//        {        
//            hlf = &tcp_http_connection->server;        
//            /* hlf表示Web服务器的TCP连接端 */    
//            strcpy(address_content, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));        
//            sprintf(address_content + strlen(address_content), " : %i", ip_and_port.source);        
//            strcat(address_content, " ----> ");        
//            strcat(address_content, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));        
//            sprintf(address_content + strlen(address_content), ":%i", ip_and_port.dest);        
//            strcat(address_content, "n");        
//            printf("n");        
//            printf("%s", address_content);        
//            printf("服务器接收数据...n");        
//            printf("n");        
//            memcpy(content, hlf->data, hlf->count_new);        
//            content[hlf->count_new] = '';        
//            parse_server_data(content, hlf->count_new);        
//            /* 分析WEB服务器接收的数据 */    
//        }        
//    }        
//    return ;        
//}        
///*      
//=======================================================================================================================      
//主函数      
//=======================================================================================================================      
// */    
//void main()        
//{        
//    if (!nids_init())        
//     /* Libnids初始化 */    
//    {        
//        printf("出现错误：%sn", nids_errbuf);        
//        exit(1);        
//    }        
//    nids_register_tcp(http_protocol_callback);        
//    /* 注册回调函数 */    
//    nids_run(); /* 进入循环捕获数据包状态 */    
//}
//
