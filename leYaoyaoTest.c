/**
 * This module implements the integration tests for TCP��read and write file��uart and GPIO.
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

#define SERV_PORT (5050)//ip�˿ں�
#define SER_IP "127.0.0.1"//IP��ַ


static int server(void)
{
    /* 1.�����׽��� */
	int fd = -1, ret = -1, serv_fd = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd)
    {
        perror("server socket");
        return -1;
    }
    
    /* 2.�󶨶˿ں� */
    
    //����sockaddr_in�ṹ�����
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  //ʹ��IPv4��ַ
    serv_addr.sin_addr.s_addr = inet_addr(SER_IP);  //�ѵ�ָ�ʽ��IP��ַ����ת��
    serv_addr.sin_port = htons(SERV_PORT);  //�ѱ����ֽ���Ķ˿ں�ת��Ϊ�����ֽ���
    
    ret = bind(fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("server bind");
    }

    
    /* 3.���׽��ֽ��м���,�����������1���ͻ������� */
    ret = listen(fd, 1);
    if(-1 == ret)
    {
        perror("server listen");
    }
    printf("server listen success\n");
    
    /* 4.�����ȴ��ͻ������� */
    socklen_t socklen;
    serv_fd = accept(fd, (struct sockaddr *)&serv_addr,  &socklen);
    if(-1 == serv_fd)
    {
        perror("server accept");
    }
    printf("server accept success\n");

    /* 5.���տͻ��˷��͵����ݲ���ӡ���� */
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

    /* 6. ����ر��ļ��������������׽��� */

    return 0;
}


static int client(void)
{
    /* 1.�����׽��� */
	int fd = -1, ret = -1;
	fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd)
    {
        perror("client socket");
        return -1;
    }

    /* 2.���ӷ���� */
    
    //����sockaddr_in�ṹ�����
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  //ʹ��IPv4��ַ
    serv_addr.sin_addr.s_addr = inet_addr(SER_IP);  //�ѵ�ָ�ʽ��IP��ַ����ת��
    serv_addr.sin_port = htons(SERV_PORT);  //�ѱ����ֽ���Ķ˿ں�ת��Ϊ�����ֽ���
    ret = connect(fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("client connect");
        return -1;
    }
    
    /* 3.���ͻ��˷������� */
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

    /* 6. ����ر��ļ��������������׽��ּ��� */
    
    return 0;
}


static int file_ops(void)
{
    FILE *fp = NULL;
    int ret = 0;
    char buf[1024] = {0};
    fp = fopen("/home/leyaoyao.txt", "w+");//���ļ������Ȼ���ٽ��ж�д���ļ��������򴴽�
    if(NULL == fp)
    {
        perror("fopen");
    }

    ret = fwrite("file test123456789", 1, strlen("file test123456789"),fp);
    if(ret > 0)
    {
        //֤���ļ��Ѿ�д����
        printf("fwrite success,ret = %d\n", ret);
    }
    else
    {
        perror("fwrite error\n");
    }
    fflush(fp);//ˢ�»��������ں�����ж�����

    //�ļ�ָ���ƶ�����ͷ
    if(ftell(fp) > 0)
    {
        fseek(fp, 0L, SEEK_SET);
        printf("file point is %ld\n", ftell(fp));
    }
    ret = fread(buf, 1, sizeof(buf), fp);
    if(ret > 0)
    {
        //֤���ļ��Ѿ�������
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
    �����ĸ��������Բ���GPIO
    //1.ʹ��GPIO����������������
    int32_t fibo_gpio_cfg(uint8_t pinNum, gpio_PinMode_t mode)
    
    //2.����GPIO��ֵ
    int32_t fibo_gpio_set(uint8_t pinNum, gpio_Level_t level)
    
    //���߶�ȡGPIO��ֵ
    int32_t fibo_gpio_get(uint8_t pinNum, gpio_Level_t * level)

    //3.ȥʹ��GPIO��ÿ�ζ�GPIO������ɺ����ȥʹ��
    int32_t fibo_gpio_uncfg(uint8_t pinNum)
#endif
    return 0;
}


//uart,����һ�����ݣ����ȴ���������
static int uart_ops(void)
{
    int fd = -1, ret = -1;;
    char buf[1024] = {0};

    /* UART������ֱ�Ӵ��豸�ڵ���ж�д���� */
    fd = open("/dev/ttyS0", O_RDWR);//�Զ�д�ķ�ʽ���豸�ڵ�
    if(-1 == fd)
    {
        perror("open");
    }

    //��UARTд���ݣ�ʹ�ô��ڹ��߿��Կ�����ӡ
    write(fd , "le yao yao uart1 test\n", 22);

    while(1)
    {
        //��UART������
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
        printf("1.fork error��exit\n");
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


