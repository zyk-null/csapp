#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
int h,v,s,E,b;//全局变量,用于存储命令行参数,其中h,v,s,E,b分别对应-h,-v,-s,-E,-b
int S;//S=1<<s
char t[100];//用于存储文件名
int hit=0,miss=0,eviction=0;//命中次数,未命中次数,淘汰次数
struct cacheLine{
    int valid;//是否有效
    int tag;//标记
    int lru;//最近最少使用
};
struct cacheLine **cache=NULL;//缓存
//print usage information
void printUsage()
{
    printf("Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
            "Options:\n"
            "  -h         Print this help message.\n"
            "  -v         Optional verbose flag.\n"
            "  -s <s>     Number of set index bits.\n"
            "  -E <E>     Number of lines per set.\n"
            "  -b <b>     Number of block offset bits.\n"
            "  -t <tracefile>     Name of the valgrind trace to replay.\n"
            "\n"
            "Examples:\n"
            "  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n"
            "  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n"
    );
}
void initCache()//初始化缓存
{
    int S=1<<s;
    int i,j;
    cache=(struct cacheLine**)malloc(sizeof(struct cacheLine*)*S);
    for(i=0;i<S;i++)
    {
        cache[i]=(struct cacheLine*)malloc(sizeof(struct cacheLine)*E);
        for(j=0;j<E;j++)
        {
            cache[i][j].valid=0;
            cache[i][j].tag=-1;
            cache[i][j].lru=0;
        }
    }
}
void updateCache(unsigned long long addr)
{
    int i;
    int maxlru=-1;
    int maxlruindex=-1;
    int flag=0;
    int set=(addr>>b)&((-1U)>>(64-s));//计算set
    int tag=addr>>(s+b);//计算tag
    for(i=0;i<E;i++)//遍历缓存
    {
        if(cache[set][i].valid==1&&cache[set][i].tag==tag)//如果命中
        {
            hit++;//命中次数加1
            cache[set][i].lru=0;//将lru置为0
            flag=1;
            break;
        }
    }
    if(flag==0)//如果未命中
    {
        miss++;//未命中次数加1
        for(i=0;i<E;i++)//遍历缓存
        {
            if(cache[set][i].valid==0)//如果有空行
            {
                cache[set][i].valid=1;//将空行置为有效
                cache[set][i].tag=tag;//将tag写入空行
                cache[set][i].lru=0;//将lru置为0
                flag=1;
                break;
            }
        }
        if(flag==0)//如果没有空行
        {
            eviction++;//淘汰次数加1
            maxlru=cache[set][0].lru;
            maxlruindex=0;
            for(i=1;i<E;i++)//遍历缓存
            {
                if(cache[set][i].lru>maxlru)//找到lru最big的行
                {
                    maxlru=cache[set][i].lru;
                    maxlruindex=i;
                }
            }
            cache[set][maxlruindex].tag=tag;//将tag写入lru最big的行
            cache[set][maxlruindex].lru=0;//将lru置为0
        }
    }
}
void updateLRU()
{
    for(int i=0;i<S;i++)//遍历缓存
    {
        for(int j=0;j<E;j++)
        {
            if(cache[i][j].valid==1)
                cache[i][j].lru++;//lru加1
        }
    }
}
void parseCache(FILE *fp)//解析文件
{
    char op;
    unsigned long long addr;
    int size;

    while(fscanf(fp," %c %llx,%d",&op,&addr,&size)!=EOF)//读取文件
    {
        switch (op) 
        {
        case 'I':continue;
        case 'L':
            updateCache(addr);
            break;
        case 'M':
            updateCache(addr);
        case 'S':
            updateCache(addr);       
        }
        updateLRU();

    }
    fclose(fp);
    for(int i=0;i<S;i++){
        free(cache[i]);
    }
    free(cache);
}


int main(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        //getopt()函数用于解析命令行参数,返回值为选项字符,如果没有选项字符,则返回-1,如果选项字符后面有冒号,则表示该选项需要参数
        switch (opt) {
        case 'h'://如果选项字符为h,则打印帮助信息
            h=1;
            printUsage();
            break;
        case 'v'://如果选项字符为v,则打印详细信息
            v=1;
            break;
        case 's'://
            s=atoi(optarg);
            break;
        case 'E':
            E=atoi(optarg);
            break;
        case 'b':
            b=atoi(optarg);
            break;
        case 't':
            strcpy(t,optarg);
            break;
        default:
            break;
        }
    }
    if(s<0||E<0||b<0||t[0]=='\0')//如果命令行参数不合法,则打印帮助信息
    {
        printUsage();
        return 0;
    }
    S=1<<s;
    FILE *fp=fopen(t,"r");//打开文件
    if(fp==NULL)//如果文件打开失败,则打印帮助信息
    {
        printUsage();
        return 0;
    }
    initCache();//初始化缓存
    parseCache(fp);//解析文件
    printSummary(hit,miss,eviction);//打印命中次数,未命中次数,淘汰次数
    return 0;
}
