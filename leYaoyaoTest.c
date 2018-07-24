/**
 * This module implements the integration tests for TCP、read and write file、uart and GPIO.
 *
 * Copyright (C) Shenzhen Fibocom Wireless Inc.
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SERV_PORT (5050)//ip端口号
#define SER_IP "127.0.0.1"//IP地址


static int server(void)
{
    /* 1.创建套接字 */
	int fd = -1, ret = -1, serv_fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd)
    {
        perror("server socket");
        return -1;
    }
    
    /* 2.绑定端口号 */
    
    //创建sockaddr_in结构体变量
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(SER_IP);  //把点分格式的IP地址进行转换
    serv_addr.sin_port = htons(SERV_PORT);  //把本地字节序的端口号转换为网络字节序
    
    ret = bind(fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("server bind");
    }

    
    /* 3.对套接字进行监听,设置最多允许1个客户端连接 */
    ret = listen(fd, 1);
    if(-1 == ret)
    {
        perror("server listen");
    }
    printf("server listen success\n");
    
    /* 4.阻塞等待客户端连接 */
    socklen_t socklen;
    serv_fd = accept(fd, (struct sockaddr *)&serv_addr,  &socklen);
    if(-1 == serv_fd)
    {
        perror("server accept");
    }
    printf("server accept success\n");

    /* 5.接收客户端发送的数据并打印出来 */
    char buf[1024];
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        ret = read(serv_fd, buf, sizeof(buf)-1);
        if(ret > 0)
        {
            printf("[server client say]:%s\n",buf);
        }
        sleep(1);

    }

    /* 6. 按需关闭文件描述符和网络套接字 */

    return 0;
}


static int client(void)
{
    /* 1.创建套接字 */
	int fd = -1, ret = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd)
    {
        perror("client socket");
        return -1;
    }

    /* 2.连接服务端 */
    
    //创建sockaddr_in结构体变量
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(SER_IP);  //把点分格式的IP地址进行转换
    serv_addr.sin_port = htons(SERV_PORT);  //把本地字节序的端口号转换为网络字节序
    ret = connect(fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("client connect");
        return -1;
    }
    
    /* 3.给客户端发送数据 */
    while(1)
    {
        do{
            ret = write(fd, " hello server...... ", sizeof(" hello server...... ")-1);
            if(ret > 0)
            {
                printf("client write success\n");
            }
            sleep(1);
        }while(ret < 0);

    }

    /* 6. 按需关闭文件描述符和网络套接字即可 */
    
    return 0;
}


static int file_ops(void)
{
    FILE *fp = NULL;
    int ret = 0;
    char buf[1024] = {0};
    fp = fopen("/home/leyaoyao.txt", "w+");//打开文件并清空然后再进行读写，文件不存在则创建
    if(NULL == fp)
    {
        perror("fopen");
    }

    ret = fwrite("file test123456789", 1, strlen("file test123456789"),fp);
    if(ret > 0)
    {
        //证明文件已经写入了
        printf("fwrite success,ret = %d\n", ret);
    }
    else
    {
        perror("fwrite error\n");
    }
    fflush(fp);//刷新缓冲区便于后面进行读操作

    //文件指针移动到开头
    if(ftell(fp) > 0)
    {
        fseek(fp, 0L, SEEK_SET);
        printf("file point is %ld\n", ftell(fp));
    }
    ret = fread(buf, 1, sizeof(buf), fp);
    if(ret > 0)
    {
        //证明文件已经读到了
        printf("[fread success]:%s\n",buf);
    }
    else
    {
        perror("fread error");
    }
    fclose(fp);
    fp = NULL;
    return 0;
}

static int gpio_ops(void)
{
#if 0
    以下四个函数可以操作GPIO
    //1.使能GPIO，并设置驱动方向
    int32_t fibo_gpio_cfg(uint8_t pinNum, gpio_PinMode_t mode)
    
    //2.设置GPIO的值
    int32_t fibo_gpio_set(uint8_t pinNum, gpio_Level_t level)
    
    //或者读取GPIO的值
    int32_t fibo_gpio_get(uint8_t pinNum, gpio_Level_t * level)

    //3.去使能GPIO，每次对GPIO操作完成后进行去使能
    int32_t fibo_gpio_uncfg(uint8_t pinNum)
#endif
    return 0;
}


//uart,发送一条数据，并等待接受数据
static int uart_ops(void)
{
    int fd = -1, ret = -1;;
    char buf[1024] = {0};

    /* UART操作是直接打开设备节点进行读写即可 */
    fd = open("/dev/ttyS0", O_RDWR);//以读写的方式打开设备节点
    if(-1 == fd)
    {
        perror("open");
    }

    //向UART写数据，使用串口工具可以看到打印
    write(fd , "le yao yao uart1 test\n", 22);

    while(1)
    {
        //从UART读数据
        ret = read(fd, buf, 1024);
        if(ret > 0)
        {
            printf("[read from uart]:%s\n",buf);
        }
        sleep(1);
    }
    
    close(fd);
    return 0;
}

int main(int argc, char **argv)
{
    pid_t fpid;
    fpid = fork();
    if(-1 == fpid)
    {
        printf("1.fork error，exit\n");
        exit(EXIT_FAILURE);
    }

    if(0 == fpid)
    {
        printf("1.fork success, pid=%d, ppid=%d, run server\n",getpid(), getppid());
        server();
    }
    else
    {
        fpid = fork();
        if(-1 == fpid)
        {
            printf("2.fork error\n");
        }
        
        if(0 == fpid)
        {
            printf("2.fork success, pid=%d, ppid=%d, run gpio_ops file_ops\n",getpid(), getppid());
            gpio_ops();
            file_ops();
        }
        else
        {
            fpid = fork();
            if(-1 == fpid)
            {
                printf("3.fork error\n");
            }
            
            if(0 == fpid)
            {
                printf("3.fork success, pid=%d, ppid=%d, run client\n",getpid(), getppid());
                sleep(1);
                client();
            }
            else
            {
                printf("parent is here, pid=%d, ppid=%d, run uart_ops\n",getpid(), getppid());
                uart_ops();
            }
            
        }
    }
    
    return 0;
}


