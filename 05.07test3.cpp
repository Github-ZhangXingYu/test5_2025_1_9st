//
// Created by 15411 on 2025/5/8.
//
//
// Created by 15411 on 2025/5/8.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#define LED_NUM 2// 定义LED数量
#define LED_C 2
#define  BUZZER_C 2

int r=0;//adc采样后存储的值
char *uart_out_space = "\r\n";// 定义回车换行符
void delay_ms(int ms);// 延时函数，用于实现指定毫秒数的延时
void adc_statistics(int fd);// ADC采集50次数据并返回统计结果的函数，fd是串口文件描述符
int uart_init();// 串口初始化函数，返回串口文件描述符
int read_uart(int fd);// 读取串口数据的函数，参数fd是串口文件描述符
int write_uart(int i,int fd);// 将ADC采样数据发送到串口的函数，参数i是采样数据，fd是串口文件描述符
void led (int argv1 , int argv2);// 控制LED的函数，参数argv1是LED状态（0或1），argv2是LED通道号
void buzzer (int argv1);// 控制蜂鸣器的函数，参数argv1是蜂鸣器状态（0或1）
int adc();// ADC采样函数，返回采样数据
int set_opt(int,int,int,char,int);// 设置串口参数的函数，参数依次为文件描述符、波特率、数据位、校验位、停止位
//"/dev/ttySAC3"是con2，靠近耳机接口的串口

//主函数
int main()
{
    int fd;   //串口是否加载
    fd = uart_init();
    read_uart(fd);
    return 0;
}

//串口初始化
int uart_init() {
    int fd;
    char *uart3 = "/dev/ttySAC3";
    char *uart_out = "Successfully initialized\r\n";//成功初始化
    // 打开串口设备
    if((fd = open(uart3, O_RDWR|O_NOCTTY))<0){
        printf("open %s is failed",uart3);
    }
    else{
        set_opt(fd, 115200, 8, 'N', 1); // 设置串口参数
        write(fd,uart_out, strlen(uart_out));
    }
    return fd;
}

int thread_wait=300;//默认等待300ms等线程结束
// LED闪烁线程函数
int led0_st=0,led1_st=0,led0_time=0,led1_time;
int led2_st=0,led2_time=0;
void* led_blink0(void* arg) {
    while (led0_st) {
        int time_rel=led0_time*100;//0.1s执行一次
        led(1,0);
        delay_ms(time_rel);
        led(0,0);
        delay_ms(time_rel);
    }
    led0_st=0;led0_time=0;
    printf("LED blinking stopped\n");
    return NULL;
}
void* led_blink1(void* arg) {
    while (led1_st) {
        int time_rel=led1_time*100;//0.1s执行一次
        led(1,1);
        delay_ms(time_rel);
        led(0,1);
        delay_ms(time_rel);
    }
    led1_st=0;led1_time=0;
    printf("LED blinking stopped\n");
    return NULL;
}
void* led_blink2(void* arg) {//交替闪烁
    led0_st=0;led0_time=0;
    led1_st=0;led1_time=0;
    while (led2_st) {
        int time_rel=led2_time*100;//0.1s执行一次
        led(1,0);
        led(0,1);
        delay_ms(time_rel);
        led(0,0);
        led(1,1);
        delay_ms(time_rel);
    }
    led2_st=0;led2_time=0;
    printf("LED blinking stopped\n");
    return NULL;
}
// BEEP闪烁线程函数
int beep_st=0,beep_time=0;
void* beep_blink(void* arg) {//交替闪烁
    while (beep_st) {
        int time_rel=beep_time*100;//0.1s执行一次
        buzzer(1);// 打开蜂鸣器
        delay_ms(time_rel);
        buzzer(0);// 关闭蜂鸣器
        delay_ms(time_rel);
    }
    beep_st=0;beep_time=0;
    printf("beep blinking stopped\n");
    return NULL;
}
int adc_st=0,adc_time=0;
// ADC闪烁采样线程函数
void* adc_blink(void* arg) {//交替闪烁
    int* FD = (int*)arg;
    int fd=*FD;
    while (adc_st) {
        int time_rel=adc_time*100;//0.1s执行一次
        adc();
        if(r!=0){
            char *uart_out_adc_out = "\r\nadc data is\r\n";
            write(fd,uart_out_adc_out,strlen(uart_out_adc_out));
            write_uart(r,fd);
            r = 0;
        }
        delay_ms(time_rel);
    }
    adc_st=0;adc_time=0;
    printf("adc blinking stopped\n");
    return NULL;
}
// ALL闪烁线程函数
int all_st=0;
void* all_blink(void* arg){
    int* FD = (int*)arg;
    int fd=*FD;
    int time_rel=500;//0.5s执行一次
    while(all_st){
        led(1,0);
        led(0,1);
        buzzer(1);
        adc();
        if(r!=0){
            char *uart_out_adc_out = "\r\nadc data is\r\n";
            write(fd,uart_out_adc_out,strlen(uart_out_adc_out));
            write_uart(r,fd);
            r = 0;
        }

        delay_ms(500);
        led(0,0);
        led(1,1);
        buzzer(0);
        delay_ms(time_rel);
    }
    all_st=0;
    printf("all blinking stopped\n");
    return NULL;
}
// TIMER线程函数
int timer_st=0,timer_seconds;
void* timer(void* arg){
    int* FD = (int*)arg;
    int fd=*FD;
    int i;
    char *uart_out_timer_start = "\r\ntimer_start";
    char *uart_out_timer_over = "\r\nThe timer has reached its set time.!";
    write(fd,uart_out_timer_start,strlen(uart_out_timer_start));
    // 定时器倒计时
    for (i = timer_seconds; i > 0; i--) {
        write(fd,uart_out_space,strlen(uart_out_space));//回车
        write_uart(i,fd);
        if(i>3){
            delay_ms(1000); // 延时1秒
        }
        else{
            buzzer(1);
            delay_ms(400);
            buzzer(0);
            delay_ms(600);
        }
        if(!timer_st)break;
    }
    write(fd,uart_out_timer_over,strlen(uart_out_timer_over));
    buzzer(1);
    delay_ms(1500);
    buzzer(0);

    timer_st=0;timer_seconds=0;
    printf("Timer stopped\n");
    return NULL;
}
//复位全部线程
void reset_thread(){
    led0_st=0;led1_st=0;led0_time=0;led1_time=0;
    led2_st=0;led2_time=0;
    beep_st=0;beep_time=0;
    adc_st=0;adc_time=0;
    all_st=0;
    timer_st=0;timer_seconds=0;
    delay_ms(thread_wait);//等待结束
}

//读串口数据并回传数据
int read_uart(int fd){
    char buffer[512];
    int nByte;
    memset(buffer, 0, sizeof(buffer));

    while(1){
        while((nByte = read(fd, buffer, 512))>0){
            buffer[nByte+1] = '\0';//转换成char
            if(buffer[0]=='0'){// 复位
                reset_thread();//重置线程
                led(0,0);
                led(0,1);
                buzzer(0);
                set_opt(fd, 115200, 8, 'N', 1); // 设置串口参数
            }
            if(buffer[0]=='1')// LED灯亮灭控制
            {
                int cmd = buffer[1]-'0';
                int io = buffer[2]-'0';
                if((cmd==0||cmd==1)&&(io==0||io==1)&&strlen(buffer)==3){
                    led(cmd,io);
                }
                else{
                    char *uart_out_led1 = "\r\nled_please_enter 1[0/1][0/1]";
                    write(fd,uart_out_led1,strlen(uart_out_led1));
                }
            }
            if(buffer[0]=='2')// LED灯闪烁控制,时间单位0.1s
            {
                if(buffer[1] >= '0' && buffer[1] <= '1' && strlen(buffer)>=3){
                    if(buffer[1]-'0'==0){
                        led0_time = 0;
                        int ii = 2;
                        for(;ii<strlen(buffer);ii++){
                            led0_time=led0_time*10+(buffer[ii]-'0');
                        }
                        if(led0_time==0){//0不用启动线程
                            led0_st = 0;led0_time = 0;
                            delay_ms(thread_wait);//等待结束
                        }
                        else if(!led0_st) {//需要启动线程
                            led0_st = 1;
                            pthread_t led_thread0;//led0
                            if (pthread_create(&led_thread0, NULL, led_blink0, NULL)!=0) {
                                perror("Failed to create LED blink0 thread");
                                exit(EXIT_FAILURE);
                            }
                        }
                    }
                    if(buffer[1]-'0'==1){
                        led1_time = 0;
                        int ii = 2;
                        for(;ii<strlen(buffer);ii++){
                            led1_time=led1_time*10+(buffer[ii]-'0');
                        }
                        if(led1_time==0){//0不用启动线程
                            led1_st = 0;led1_time = 0;
                            delay_ms(thread_wait);//等待结束
                        }
                        else if(!led1_st){//需要启动线程
                            led1_st = 1;
                            pthread_t led_thread1;//led1
                            if (pthread_create(&led_thread1, NULL, led_blink1, NULL) != 0) {
                                perror("Failed to create LED blink1 thread");
                                exit(EXIT_FAILURE);
                            }
                        }
                    }
                }
                else{
                    char *uart_out_led2 = "\r\nled_please_enter 2[0-1][1-100]";
                    write(fd,uart_out_led2,strlen(uart_out_led2));
                }
            }
            if(buffer[0]=='3')// 交替闪烁
            {
                if(buffer[1] >= '0' && buffer[1] <= '9'){
                    led2_time = 0;
                    int ii = 1;
                    for(;ii<strlen(buffer);ii++){
                        led2_time=led2_time*10+(buffer[ii]-'0');
                    }
                    if(led2_time==0){
                        led0_st=0;led0_time=0;
                        led1_st=0;led1_time=0;
                        led2_st = 0;led2_time = 0;
                        delay_ms(thread_wait);//等待结束
                    }
                    else if(!led2_st) {
                        led0_st = 0;led0_time = 0;
                        led1_st = 0;led1_time = 0;
                        delay_ms(thread_wait);//等待结束
                        led2_st = 1;
                        pthread_t led_thread2;//led0
                        if (pthread_create(&led_thread2, NULL, led_blink2, NULL) != 0) {
                            perror("Failed to create LED blink0 thread");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                else{
                    char *uart_out_led2 = "\r\nled_please_enter 3[1-100]";
                    write(fd,uart_out_led2,strlen(uart_out_led2));
                }
            }
            if(buffer[0]=='4')// 蜂鸣器鸣响控制
            {
                if((buffer[1]=='0'||buffer[1]=='1')&&strlen(buffer)==2){
                    int beep_en=buffer[1]-'0';
                    buzzer(beep_en);
                }
                else{
                    char *uart_out_beep1 = "\r\nbeep_please_enter 4[0/1]";
                    write(fd,uart_out_beep1,strlen(uart_out_beep1));
                }
            }
            if(buffer[0]=='5')// 蜂鸣器节拍控制
            {
                if(buffer[1] >= '0' && buffer[1] <= '9' && strlen(buffer)==2){
                    beep_time = buffer[1]-'0';
                    if(beep_time==0){
                        beep_st=0;beep_time=0;
                        delay_ms(thread_wait);//等待结束
                    }
                    else if(!beep_st) {
                        beep_st = 1;
                        pthread_t beep_thread;//led0
                        if (pthread_create(&beep_thread, NULL, beep_blink, NULL) != 0) {
                            perror("Failed to create beep blink thread");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                else{
                    char *uart_out_beep2 = "\r\nbeep_please_enter 5[0-9]";
                    write(fd,uart_out_beep2,strlen(uart_out_beep2));
                }
            }
            if(buffer[0]=='6') // ADC单次采样
            {
                if(strlen(buffer)==1){
                    adc();
                    if(r!=0){
                        char *uart_out_adc_out = "\r\nadc data is\r\n";
                        write(fd,uart_out_adc_out,strlen(uart_out_adc_out));
                        write_uart(r,fd);
                        r = 0;
                    }
                }
                else{
                    char *uart_out_adc1 = "\r\nadc_please_enter 6";
                    write(fd,uart_out_adc1,strlen(uart_out_adc1));
                }
            }
            if(buffer[0]=='7')// ADC多次采样
            {
                if(buffer[1]>='0' && buffer[1]<='9' && strlen(buffer)==2){
                    adc_time = buffer[1]-'0';
                    if(adc_time==0){
                        adc_st=0;adc_time=0;
                        delay_ms(thread_wait);//等待结束
                    }
                    else if(!adc_st){
                        adc_st = 1;
                        pthread_t adc_thread;//led0
                        if (pthread_create(&adc_thread, NULL, adc_blink, &fd) != 0) {
                            perror("Failed to create adc blink thread");
                            exit(EXIT_FAILURE);
                        }
                    }

                }
                else{
                    char *uart_out_adc = "\r\nadc_please_enter 7";
                    write(fd,uart_out_adc,strlen(uart_out_adc));
                }
            }
            if(buffer[0]=='8')// ADC数据分析
            {
                if(strlen(buffer)==1){
                    adc_statistics(fd);
                }
                else{
                    char *uart_out_statistics_1 = "\r\nadc_please_enter 8";
                    write(fd,uart_out_statistics_1,strlen(uart_out_statistics_1));
                }
            }
            if(buffer[0]=='9')// 整体功能运行控制
            {
                if(((buffer[1]-'0')==0||(buffer[1]-'0')==1) && strlen(buffer)==2){
                    if(buffer[1]=='0'){
                        all_st=0;
                        delay_ms(thread_wait);
                    }
                    else if(!all_st){
                        all_st=1;
                        pthread_t all_thread;//led0
                        if (pthread_create(&all_thread, NULL, all_blink, &fd) != 0) {
                            perror("Failed to create adc blink thread");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                else{
                    char *uart_out_all_1 = "\r\nall_please_enter 9[0/1]";
                    write(fd,uart_out_all_1,strlen(uart_out_all_1));
                }
            }
            if(buffer[0]=='a')// 定时器功能
            {
                if(buffer[1] >= '0' && buffer[1] <= '9'){
                    timer_seconds = 0;
                    int ii = 1;
                    for(;ii<strlen(buffer);ii++){
                        timer_seconds=timer_seconds*10+(buffer[ii]-'0');
                    }
                    if(timer_seconds==0){
                        timer_st=0;timer_seconds=0;
                        delay_ms(thread_wait);//等待结束
                    }
                    else if(!timer_st) {
                        timer_st = 1;
                        pthread_t timer_thread;//led0
                        if (pthread_create(&timer_thread, NULL, timer, &fd) != 0) {
                            perror("Failed to create adc blink thread");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                else{
                    char *uart_out_timer_1 = "\r\ntimer_please_enter a[1-100]";
                    write(fd,uart_out_timer_1,strlen(uart_out_timer_1));
                }
            }
            if(buffer[0]=='*')// 发送所有命令列表到串口
            {
                char *uart_out0 = "\r\n0 init\r\n";
                char *uart_out1 = "1 led_please_enter 1[0/1][0/1]\r\n";
                char *uart_out2 = "2 led_please_enter 2[0-1][1-100]\r\n";
                char *uart_out3 = "3 led_please_enter 3[1-100]\r\n";
                char *uart_out4 = "4 beep_please_enter 4[0/1]\r\n";
                char *uart_out5 = "5 beep_please_enter 5[0-9]\r\n";
                char *uart_out6 = "6 adc_please_enter 6\r\n";
                char *uart_out7 = "7 adc_please_enter 7\r\n";
                char *uart_out8 = "8 adc_please_enter 8\r\n";
                char *uart_out9 = "9 all_please_enter 9[0/1]\r\n";
                char *uart_out10 = "10 timer_please_enter a[1-100]\r\n";
                write(fd,uart_out0,strlen(uart_out0));
                write(fd,uart_out1,strlen(uart_out1));
                write(fd,uart_out2,strlen(uart_out2));
                write(fd,uart_out3,strlen(uart_out3));
                write(fd,uart_out4,strlen(uart_out4));
                write(fd,uart_out5,strlen(uart_out5));
                write(fd,uart_out6,strlen(uart_out6));
                write(fd,uart_out7,strlen(uart_out7));
                write(fd,uart_out8,strlen(uart_out8));
                write(fd,uart_out9,strlen(uart_out9));
                write(fd,uart_out10,strlen(uart_out10));
            }
            if(buffer[0] == '#') // 退出程序
            {
                reset_thread();//关闭线程

                led(0,0);
                led(0,1);
                buzzer(0);
                char *uart_out_over = "\r\nexit\r\n";
                write(fd,uart_out_over,strlen(uart_out_over));
                close(fd);
                return 0;
            }
            char *uart_out_uart_main = "\r\nsucceed";
            write(fd,uart_out_uart_main,strlen(uart_out_uart_main));
            write(fd, buffer, strlen(buffer));
            write(fd,uart_out_space,strlen(uart_out_space));
            memset(buffer, 0, strlen(buffer));
            nByte = 0;
        }
    }
}

//延时函数
void delay_ms(int ms) {
    usleep(ms * 1000); // usleep 的参数是微秒，所以要乘以 1000 转换为毫秒
}

//ADC 采集50次数据并返回统计结果
void adc_statistics(int fd) {
    int max_val = 0, min_val = 4095, sum = 0;
    int avg=0;
    int i,data;
    char *uart_out_adc_start1 = "\r\nadc data is";
    char *statistics_max = "\r\nmax ";
    char *statistics_min = "\r\nmin ";
    char *statistics_avg = "\r\naverage value ";
    write(fd,uart_out_adc_start1, strlen(uart_out_adc_start1));
    for(i=0;i<50;i++){
        data=adc();
        if(data != 0){
            if(i==0){
                max_val = data;
                min_val = data;
            }
            else{
                if (data > max_val) max_val = data;// 更新最大值
                if (data < min_val) min_val = data;// 更新最小值
            }
            sum += data;// 累加数据用于计算平均值
            delay_ms(20); // 延时 20 毫秒
            write(fd,uart_out_space,strlen(uart_out_space));//回车
            write_uart(data,fd);
        }
    }
    avg=sum / 50;
    write(fd,statistics_max,strlen(statistics_max));
    write_uart(max_val,fd);// 发送最大值数据到串口
    write(fd,statistics_min,strlen(statistics_min));
    write_uart(min_val,fd);// 发送最小值数据到串口
    write(fd,statistics_avg,strlen(statistics_avg));
    write_uart(avg,fd);// 发送平均值数据到串口
}

int write_uart(int i,int fd){
    char buffer[10], tmp[10];     //nuffbur  int转换为char   把电阻值在串口助手中打印出来
    int cnt = 0;
    while(i){
        tmp[cnt++] = i % 10 + '0';
        i /= 10;
    }
    int j;
    for(j = cnt - 1; j >= 0; j --){
        buffer[j] = tmp[cnt - j - 1];
    }
    buffer[cnt] = '\n';
    write(fd, buffer, cnt+1);
    return 0;
}

//LED灯 0靠近蜂鸣器，1靠近按键
void led (int argv1 , int argv2){//argv1 是状态   argv2 是位置
    int fd, led_num, led_c;
    char *leds = "/dev/leds";
    led_num = LED_NUM;
    led_c = LED_C;

    if(argv1 >= led_c) {
        printf("argv1 is 0 or 1\n");
        exit(1);
    }
    if(argv2 >= led_num) {
        printf("argv2 is 0 or 1\n");
        exit(1);
    }

    if ((fd = open(leds, O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        printf("open %s failed\n", leds);
    } else {
        ioctl(fd, argv1, argv2);
        printf("ioctl %s success\n", leds);
    }
    close(fd);//关闭这个设备节点
}

//蜂鸣器
void buzzer (int argv1) {
    char *buzzer_ctl = "/dev/buzzer_ctl";
    int fd, ret, buzzer_c;
    buzzer_c = BUZZER_C;

    if(argv1 >= buzzer_c) {
        printf("argv[1] is 0 or 1\n");
        exit(1);
    }
    if((fd = open(buzzer_ctl, O_RDWR | O_NOCTTY | O_NDELAY)) < 0){   //以可读可写非阻塞方式打开
        printf("open %s failed\n", buzzer_c);
        exit(1);
    }
    ret = ioctl(fd, argv1, 1);    //第二个参数固定
    close(fd);   //关闭这个设备节点
}

//ADC
int adc() {
    int fd;
    char *adc = "/dev/adc";
    char buffer[512];
    int len = 0;    //初始化

    memset(buffer, 0, sizeof(buffer));  //清空buffer
    printf("adc ready!\n");

    if((fd = open(adc, O_RDWR | O_NOCTTY | O_NDELAY)) < 0){  //以可读可写非阻塞方式打开
        printf("open %s err\n", adc);
    } else {
        printf("open adc success!\n");
        len = read(fd, buffer, 10);  //读取电阻值
        if(len == 0)
            printf("return null\n");
        else{
            r = atoi(buffer);    //把char变成int
            r = (int)(r * 10000 / 4095);   //转换成对应值
            printf("res value is %d\n", r);
            close(fd);
            return r;     //在主函数返回当前电阻值 int
        }
    }
}

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0) {
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    switch( nEvent )
    {
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':
            newtio.c_cflag &= ~PARENB;
            break;
    }

    switch( nSpeed )
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        case 460800:
            cfsetispeed(&newtio, B460800);
            cfsetospeed(&newtio, B460800);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }
    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
        newtio.c_cflag |=  CSTOPB;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }

    //	printf("set done!\n\r");
    return 0;
}

