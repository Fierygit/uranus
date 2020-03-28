/*
 * @Author: Firefly
 * @Date: 2020-02-28 11:52:58
 * @Descripttion: 
 * @LastEditTime: 2020-02-28 11:54:15
 */

// https://blog.csdn.net/u012234115/article/details/71056957

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
 
// mostly need to read the linux config files to get system info
 
// ---- get os info ---- //
void getOsInfo()
{
    FILE *fp = fopen("/proc/version", "r");
    if(NULL == fp)
        printf("failed to open version\n");
    char szTest[1000] = {0};
    while(!feof(fp))
    {
        memset(szTest, 0, sizeof(szTest));
        fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n
        printf("%s", szTest);
    }
    fclose(fp);
}
 
// ---- get cpu info ---- //
void getCpuInfo()
{
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if(NULL == fp)
        printf("failed to open cpuinfo\n");
    char szTest[1000] = {0};
    // read file line by line
    while(!feof(fp))
    {
        memset(szTest, 0, sizeof(szTest));
        fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n
        printf("%s", szTest);
    }
    fclose(fp);
}
 
 
// ---- get memory info ---- //
void getMemoryInfo()
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if(NULL == fp)
        printf("failed to open meminfo\n");
    char szTest[1000] = {0};
    while(!feof(fp))
    {
        memset(szTest, 0, sizeof(szTest));
        fgets(szTest, sizeof(szTest) - 1, fp);
        printf("%s", szTest);
    }
    fclose(fp);
}

int main(int argc, const char** argv) {
    getCpuInfo();
    return 0;
}