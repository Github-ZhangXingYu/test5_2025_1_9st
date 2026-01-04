//
// Created by 15411 on 2025/5/7.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#define LED_NUM 2// 定义LED数量
#define LED_C 2
#define  BUZZER_C 2

int q=0;//串口输入检测到'#'，则退出循环标志
int r=0;//adc采样后存储的值
char *uart_out_space = "\r\n";// 定义回车换行符
void delay_ms(int ms);// 延时函数，用于实现指定毫秒数的延时
void led_control(int flicker1,int time);// 控制LED闪烁的函数，参数flicker1表示闪烁模式，time表示闪烁时间间隔
void beep_control(int beat);// 控制蜂鸣器鸣叫的函数，参数beat表示鸣叫节拍
void adc_control(int times,int fd);// 控制ADC采样的函数，参数times表示采样次数，fd是串口文件描述符
void all_control(int en_all,int fd);// 控制多模块联合运行的函数，en_all为1时启动联合运行，fd是串口文件描述符
void adc_statistics(int fd);// ADC采集50次数据并返回统计结果的函数，fd是串口文件描述符
void timer(int seconds, int fd);// 定时器函数，参数seconds表示定时时间（秒），fd是串口文件描述符
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
    while(1){
        read_uart(fd);
        if(q==1){
            return 0;
        }
    }
    return 0;
}

//延时函数
void delay_ms(int ms) {
    usleep(ms * 1000); // usleep 的参数是微秒，所以要乘以 1000 转换为毫秒
}

//led闪烁控制
void led_control(int flicker1,int time){
    int time_rel=time*200;//输入闪烁持续时间等级
    // 根据flicker1的值执行不同的LED闪烁模式
    switch (flicker1) {
        case 0:{
            break;
        }
        case 1:{
            led(1,0);
            delay_ms(time_rel);
            led(0,0);
            delay_ms(time_rel);
            break;
        }
        case 2:{
            led(1,1);
            delay_ms(time_rel);
            led(0,1);
            delay_ms(time_rel);
            break;
        }
        case 3:{
            led(1,0);
            led(1,1);
            delay_ms(time_rel);
            led(0,0);
            led(0,1);
            delay_ms(time_rel);
            break;
        }
        case 4:{
            led(1,0);
            led(0,1);
            delay_ms(time_rel);
            led(0,0);
            led(1,1);
            delay_ms(time_rel);
            break;
        }
    }
}

//蜂鸣器控制
void beep_control(int beat){
    int beat_time=beat*100;// 根据beat值计算蜂鸣器鸣叫时间
    if(beat>0){
        buzzer(1);// 打开蜂鸣器
        delay_ms(beat_time);
        buzzer(0);// 关闭蜂鸣器
        delay_ms(beat_time);
    }
}

//adc控制
void adc_control(int times,int fd){
    int i;
    char *uart_out_adc_start = "\r\nadc采样的数据为：";
    write(fd,uart_out_adc_start, strlen(uart_out_adc_start));
    for(i=0;i<times;i++){
        adc();
        delay_ms(20);
        write(fd,uart_out_space,strlen(uart_out_space));//回车
        write_uart(r,fd);
    }
}
//多模块联合运行
void all_control(int en_all,int fd){
    if(en_all==1){
        led(1,0);
        led(0,1);
        buzzer(1);
        adc_control(1,fd);
        delay_ms(500);
        led(0,0);
        led(1,1);
        buzzer(0);
        delay_ms(500);
    }
}

//ADC 采集50次数据并返回统计结果
void adc_statistics(int fd) {
    int max_val = 0, min_val = 4095, sum = 0;
    int avg=0;
    int i,data;
    char *uart_out_adc_start1 = "\r\nadc采样的数据为：";
    char *statistics_max = "\r\n最大值：";
    char *statistics_min = "\r\n最小值：";
    char *statistics_avg = "\r\n平均值：";
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

// 定时器函数
void timer(int seconds, int fd){
    int i;
    char *uart_out_timer_start = "\r\n定时器启动，时间：";
    char *uart_out_timer_over = "\r\n定时器时间到！";
    write(fd,uart_out_timer_start,strlen(uart_out_timer_start));
    // 定时器倒计时
    for (i = seconds; i > 0; i--) {
        write(fd,uart_out_space,strlen(uart_out_space));//回车
        write_uart(i,fd);
        delay_ms(1000); // 延时1秒
    }
    write(fd,uart_out_timer_over,strlen(uart_out_timer_over));
    buzzer(1);
    delay_ms(1000);
    buzzer(0);
}

//串口初始化
int uart_init() {
    int fd;
    char *uart3 = "/dev/ttySAC3";
    char buffer[512];
    char *uart_out = "please input\r\n";
    memset(buffer, 0, sizeof(buffer));
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

//读串口数据并回传数据
int read_uart(int fd){
    char buffer[512];
    int nByte;
    int flicker1=0,time_led=0;
    int time_beep=0;
    int all_en=0;
    memset(buffer, 0, sizeof(buffer));
    while(1){
        led_control(flicker1,time_led);// 控制LED闪烁
        beep_control(time_beep);// 控制蜂鸣器鸣叫
        all_control(all_en,fd);// 控制多模块联合运行
        while((nByte = read(fd, buffer, 512))>0){
            buffer[nByte+1] = '\0';//转换成char
            if(buffer[0]=='1')// LED灯亮灭控制
            {
                int cmd = buffer[1]-'0';
                int io = buffer[2]-'0';
                if((cmd==0||cmd==1)&&(io==0||io==1)&&strlen(buffer)==3){
                    led(cmd,io);
                }
                else{
                    char *uart_out_led1 = "\r\nled灯亮灭控制输入指令错误，请输入正确格式：1[0/1][0/1]";
                    write(fd,uart_out_led1,strlen(uart_out_led1));
                }
            }
            if(buffer[0]=='2')// LED灯闪烁控制
            {
                if(buffer[1] >= '0' && buffer[1] <= '4' && buffer[2] >= '1' && buffer[2] <= '9' && strlen(buffer)==3){
                    flicker1 = buffer[1]-'0';
                    time_led = buffer[2]-'0';
                    if(flicker1==0){
                        led(0,0);
                        led(0,1);
                    }
                }
                else{
                    char *uart_out_led2 = "\r\nled灯闪烁控制输入指令错误，请输入正确格式：2[0-4][1-9]";
                    write(fd,uart_out_led2,strlen(uart_out_led2));
                }
            }
            if(buffer[0]=='3')// 蜂鸣器鸣响控制
            {
                if((buffer[1]=='0'||buffer[1]=='1')&&strlen(buffer)==2){
                    int beep_en=buffer[1]-'0';
                    buzzer(beep_en);
                }
                else{
                    char *uart_out_beep1 = "\r\n蜂鸣器鸣响控制输入指令错误，请输入正确格式：3[0/1]";
                    write(fd,uart_out_beep1,strlen(uart_out_beep1));
                }
            }
            if(buffer[0]=='4')// 蜂鸣器节拍控制
            {
                if(buffer[1] >= '0' && buffer[1] <= '9' && strlen(buffer)==2){
                    time_beep=buffer[1]-'0';
                }
                else{
                    char *uart_out_beep2 = "\r\n蜂鸣器节拍控制输入指令错误，请输入正确格式：4[0-9]";
                    write(fd,uart_out_beep2,strlen(uart_out_beep2));
                }
            }
            if(buffer[0]=='5') // ADC单次采样
            {
                if(strlen(buffer)==1){
                    adc();
                    if(r!=0){
                        char *uart_out_adc_out = "\r\nadc单次采样数据：\r\n";
                        write(fd,uart_out_adc_out,strlen(uart_out_adc_out));
                        write_uart(r,fd);
                        printf("open %s failed!\n", buffer);
                        r = 0;
                    }
                }
                else{
                    char *uart_out_adc1 = "\r\nadc单次采样输入指令错误，请输入正确格式：5";
                    write(fd,uart_out_adc1,strlen(uart_out_adc1));
                }
            }
            if(buffer[0]=='6')// ADC多次采样
            {
                int adc_times=buffer[1]-'0';
                if(adc_times>=1 && adc_times<=9 && strlen(buffer)==2){
                    adc_control(adc_times,fd);
                }
                else{
                    char *uart_out_adc = "\r\nadc连续采样指令输入错误，请输入正确格式：6[1-9]";
                    write(fd,uart_out_adc,strlen(uart_out_adc));
                }
            }
            if(buffer[0]=='7')// ADC+蜂鸣器联合控制
            {
                if(strlen(buffer)==1){
                    adc();
                    if(r!=0){
                        char *uart_out_adc_out = "\r\nadc单次采样数据：\r\n";
                        write(fd,uart_out_adc_out,strlen(uart_out_adc_out));
                        write_uart(r,fd);
                        printf("open %s failed!\n", buffer);
                        r = 0;
                    }
                    buzzer(1);
                    delay_ms(100);
                    buzzer(0);
                }
                else{
                    char *uart_out_adc_beep = "\r\nadc+蜂鸣器指令输入错误，请输入正确格式：7";
                    write(fd,uart_out_adc_beep,strlen(uart_out_adc_beep));
                }
            }
            if(buffer[0]=='8') // 字母大小写转换
            {
                char *uart_out_case_result = "\r\n转换后的字母为:";
                char word[2];
                if(buffer[1] >= 'A' && buffer[1] <= 'Z' && strlen(buffer)==2){
                    word[0] = buffer[1]+32; // 大写字母转小写
                    word[1] = '\0';
                    write(fd, uart_out_case_result, strlen(uart_out_case_result));
                    write(fd, word, 1);
                }
                else if (buffer[1] >= 'a' && buffer[1] <= 'z' && strlen(buffer)==2){
                    word[0] = buffer[1]-32; // 大写字母转小写
                    word[1] = '\0';
                    write(fd, uart_out_case_result, strlen(uart_out_case_result));
                    write(fd, word, 1);
                }
                else{
                    char *uart_out_word = "\r\n字母大小写转换指令输入错误，请输入正确格式：8[字母]";
                    write(fd,uart_out_word,strlen(uart_out_word));
                }
            }
            if(buffer[0]=='9')// 整体功能运行控制
            {
                if(((buffer[1]-'0')==0||(buffer[1]-'0')==1) && strlen(buffer)==2){
                    all_en=buffer[1]-'0';
                    if((buffer[1]-'0')==0){
                        led(0,0);
                        led(0,1);
                    }
                }
                else{
                    char *uart_out_all_1 = "\r\n功能指令输入错误，请输入正确格式：9[0/1]";
                    write(fd,uart_out_all_1,strlen(uart_out_all_1));
                }
            }
            if(buffer[0]=='a')// ADC数据分析
            {
                if(strlen(buffer)==1){
                    adc_statistics(fd);
                }
                else{
                    char *uart_out_statistics_1 = "\r\nadc数据分析指令输入错误，请输入正确格式：a";
                    write(fd,uart_out_statistics_1,strlen(uart_out_statistics_1));
                }
            }
            if(buffer[0]=='b')// 定时器功能
            {
                if(buffer[1] >= '1' && buffer[1] <= '9' && strlen(buffer)==2){
                    int seconds=buffer[1]-'0';
                    timer(seconds,fd);
                }
                else{
                    char *uart_out_timer = "\r\n定时器功能指令输入错误，请输入正确格式：b[1-9]";
                    write(fd,uart_out_timer,strlen(uart_out_timer));
                }
            }
            if(buffer[0]=='*')// 发送所有命令列表到串口
            {
                char *uart_out1 = "命令1:led灯亮灭控制 注：命令格式：1[0/1][0/1]\r\n";
                char *uart_out2 = "命令2:led灯闪烁控制 注：命令格式：2[0-4][1-9]\r\nled时间间隔为*200 20[1-9]-暂停\r\n";
                char *uart_out3 = "命令3:蜂鸣器鸣响控制 注：命令格式：3[0/1]\r\n";
                char *uart_out4 = "命令4:蜂鸣器节拍控制 注：命令格式：4[0-9]\r\nbeep时间间隔为*100 40-关闭蜂鸣器\r\n";
                char *uart_out5 = "命令5:adc单次采样 注：命令格式：5\r\n";
                char *uart_out6 = "命令6:adc多次采样 注：命令格式：6[1-9]\r\n";
                char *uart_out7 = "命令7:adc+蜂鸣器 注：命令格式：7\r\n";
                char *uart_out8 = "命令8:字母大小写转换 注：命令格式：8[字母]\r\n";
                char *uart_out9 = "命令9:整体功能运行 注：命令格式：9[0/1]\r\n";
                char *uart_out10 = "命令10:adc数据分析 注：命令格式：a\r\n";
                char *uart_out11 = "命令11:定时器功能 注：命令格式：b[1-9]";
                set_opt(fd, 115200, 8, 'N', 1);
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
                write(fd,uart_out11,strlen(uart_out11));
            }
            if(buffer[0] == '#') // 退出程序
            {
                char *uart_out_over = "\r\n程序结束";
                write(fd,uart_out_over,strlen(uart_out_over));
                close(fd);
                q = 1;
                return 0;
            }
            char *uart_out_uart_main = "\r\n输入指令返回值：\r\n";
            write(fd,uart_out_uart_main,strlen(uart_out_uart_main));
            write(fd, buffer, strlen(buffer));
            write(fd,uart_out_space,strlen(uart_out_space));
            memset(buffer, 0, strlen(buffer));
            nByte = 0;
        }
    }
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

//LED灯
void led (int argv1 , int argv2) //argv1 是状态   argv2 是位置
{
    int fd, led_num, led_c;
    char *leds = "/dev/leds";
    led_num = LED_NUM;
    led_c = LED_C;

    printf("argvl is cmd; argv2 is io\n");   //判断输入指令是否正确
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

