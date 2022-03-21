#include "stdio.h" 
#include "unistd.h" 
#include "sys/types.h" 
#include "sys/stat.h" 
#include "fcntl.h" 
#include "stdlib.h" 
#include "string.h" 



#define ledon  1
#define ledoff 0
int main(int argc , char *argv[])
{
    int fd,ret;
    char *filename ;
    unsigned char databuf[1];

    if(argc != 3)
    {
        printf("error usage!!\r\n");
        return -1; 
    }
    filename = argv[1];
    /*打开LED驱动*/
    fd = open(filename,O_RDWR);
    if(fd < 0)
    {
        printf("file %s open failed!\r\n",argv[1]);
        return -1;
        /* code */
    }
    databuf[0] = atoi(argv[2]);
    ret = write(fd,databuf,sizeof(databuf));
    if (ret < 0 )
    {
        printf("led control failed!!\r\n");
        close(fd);
        return -1;
        /* code */
    }
    ret = close(fd);
    if(ret < 0)
    {
        printf("file %s close failed!!\r\n",argv[1]);
        return -1;

    }
    return 0;
    
}