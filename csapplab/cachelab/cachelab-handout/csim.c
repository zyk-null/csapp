#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void printUsage()
{
    
}
int main(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        //getopt()函数用于解析命令行参数,返回值为选项字符,如果没有选项字符,则返回-1,如果选项字符后面有冒号,则表示该选项需要参数
        switch (opt) {
        case 'h':
            printUsage();
            break;
        case 'v':
            break;
        case 's':
            break;
        case 'E':
            break;
        case 'b':
            break;
        case 't':
            break;
        default:
            break;
        }
    }
    return 0;
}
