
#include "serial_tool.h"

#include <stdio.h>        
#include <stdlib.h>        
#include <unistd.h>       
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>       
#include <termios.h>    
#include <errno.h>        
#include <getopt.h>        
#include <string.h>        
#include <time.h>       
#include <sys/select.h> 
#include <chrono>

WzSerialportPlus::WzSerialportPlus()
    :   serialportFd(-1),
        name(""),
        baudrate(9600),
        stopbit(1),
        databit(8),
        paritybit('n'),
        receivable(false),
        receiveMaxlength(2048),
        receiveTimeout(5000),
        receiveCallback(nullptr)
{
        mp3_info = {
        // {"2001", "mp3/倾尽天下-河图.320.mp3"},
        {"2001" ,"mp3/1hat.mp3"},
        {"2002", "mp3/2smoke_action.mp3"},
        {"2003", "mp3/3hand_smoke.mp3"},
        {"2004", "mp3/4work_dress.mp3"},
        {"2005", "mp3/5phone.mp3"},
        {"2006", "mp3/6flame.mp3"}}; 
}

WzSerialportPlus::WzSerialportPlus(const std::string& name,
                    const int& baudrate,
                    const int& stopbit,
                    const int& databit,
                    const int& paritybit):
            serialportFd(-1),
            name(name),
            baudrate(baudrate),
            stopbit(stopbit),
            databit(databit),
            paritybit(paritybit),
            receivable(false),
            receiveMaxlength(2048),
            receiveTimeout(5000),
            receiveCallback(nullptr)
{
        mp3_info = {
            // {"2001", "mp3/倾尽天下-河图.320.mp3"},
            {"2001" ,"mp3/1hat.mp3"},
            {"2002", "mp3/2smoke_action.mp3"},
            {"2003", "mp3/3hand_smoke.mp3"},
            {"2004", "mp3/4work_dress.mp3"},
            {"2005", "mp3/5phone.mp3"},
            {"2006", "mp3/6flame.mp3"}};
}

WzSerialportPlus::~WzSerialportPlus()
{
    close();

    while(receivable)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // printf("[WzSerialportPlus::~WzSerialportPlus()]: destructed...\n");
}

bool WzSerialportPlus::open()
{
    serialportFd = ::open(name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(serialportFd < 0)
    {
        // printf("[WzSerialportPlus::open()]: open failed with serialportFd is %d , maybe permission denied or this serialport is opened!\n",serialportFd);
        return false;
    }

    if (fcntl(serialportFd, F_SETFL, 0) < 0)  
    {
        // printf("[WzSerialportPlus::open()]: open failed with fcntl failed!\n");
        return false;
    }

    if (isatty(serialportFd) == 0)
    {
        // printf("[WzSerialportPlus::open()]: open failed with standard input is not a terminal device!\n");
        ::close(serialportFd);
        return false;
    }

    struct termios newtio, oldtio;
    if (tcgetattr(serialportFd, &oldtio) != 0)
    {
        // printf("[WzSerialportPlus::open()]: open failed with tcgetattr failed!\n");
        return false;
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
    /* databit init */
    switch (databit)
    {
        case 7:
            newtio.c_cflag |= CS7;
        break;
        case 8:
            newtio.c_cflag |= CS8;
        break;
        default:
            // printf("[WzSerialportPlus::open()]: open failed with invalid databit %d!\n",databit);
            return false;
    }
    /* paritybit init */
    switch (paritybit)
    {
        case 'o':
        case 'O':                     
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'e':
        case 'E':                     
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'n':
        case 'N':                    
            newtio.c_cflag &= ~PARENB;
            break;
        default:
            // printf("[WzSerialportPlus::open()]: open failed with invalid paritybit %d!\n",paritybit);
            return false;
    }
    /* stopbit init */
    switch (stopbit)
    {
        case 1:
            newtio.c_cflag &= ~CSTOPB;
        break;
        case 2:
            newtio.c_cflag |= CSTOPB;
        break;
        default:
            // printf("[WzSerialportPlus::open()]: open failed with invalid stopbit %d!\n",stopbit);
            return false;
    }
    /* baudrate init */
    switch (baudrate)
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
        case 19200:
            cfsetispeed(&newtio, B19200);
            cfsetospeed(&newtio, B19200);
            break;
        case 38400:
            cfsetispeed(&newtio, B38400);
            cfsetospeed(&newtio, B38400);
            break;
        case 57600:
            cfsetispeed(&newtio, B57600);
            cfsetospeed(&newtio, B57600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        case 230400:
            cfsetispeed(&newtio, B230400);
            cfsetospeed(&newtio, B230400);
            break;
        default:
            // printf("[WzSerialportPlus::open()]: open failed with invalid baudrate %d!\n",baudrate);
            return false;
    }

    newtio.c_cc[VTIME] = 1;
    newtio.c_cc[VMIN] = 1;

    tcflush(serialportFd,TCIFLUSH);
    if (tcsetattr(serialportFd, TCSANOW, &newtio) != 0)
    {
        // printf("[WzSerialportPlus::open()]: open failed with tcgetattr failed!\n");
        return false;
    }

    tcflush(serialportFd, TCIOFLUSH); 
    fcntl(serialportFd, F_SETFL, 0); 

    receivable = true;

    std::thread([&]{
        char* receiveData = new char[receiveMaxlength];
        int receivedLength = 0;
        int selectResult = -1;

        while (receivable)
        {
            memset(receiveData,0,receiveMaxlength);
            receivedLength = read(serialportFd, receiveData, receiveMaxlength); /* block util data received */
            if(receivedLength > 0)
            {
                // onReceive(receiveData,receivedLength);
                if(nullptr != receiveCallback)
                {
                    receiveCallback(receiveData,receivedLength);
                }
            }
            receivedLength = 0;
        }

        delete[] receiveData;
        receiveData = nullptr; 
    }).detach();

    // printf("[WzSerialportPlus::open()]: open success.\n");
    return true;
}

bool WzSerialportPlus::open(const std::string& name,
                            const int& baudrate,
                            const int& stopbit,
                            const int& databit,
                            const int& paritybit)
{
    this->name = name;
    this->baudrate = baudrate;
    this->stopbit = stopbit;
    this->databit = databit;
    this->paritybit = paritybit;
    return open();
}

void WzSerialportPlus::close()
{   
    if(receivable)
    {
        receivable = false;
    }

    if(serialportFd >= 0)
    {
        ::close(serialportFd);
        serialportFd = -1;
    }
}

int WzSerialportPlus::send(char* data,int length)
{
    // printf("data: %s\n",data);
    int lengthSent = 0;
    lengthSent = write(serialportFd, data, length);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return lengthSent;
}

void WzSerialportPlus::setReceiveCalback(ReceiveCallback receiveCallback)
{
    this->receiveCallback = receiveCallback;
}

void WzSerialportPlus::onReceive(char* data,int length)
{
    printf("onReceive: %s\n",data);
}

void WzSerialportPlus::playMusic(const std::string &musicFile)
{
    std::string command = "play \"" + musicFile + "\"";
    system(command.c_str());
}

int WzSerialportPlus::LoraSendMsg(char* data,int length)
{
    if(!(this->open("/dev/ttyS3",9600,1,8,'n')))
    {
        // std::cout<<"open uart error!"<<std::endl;
        return -1;
    }
    int len = 0;
    len = this->send(data, length);
    this->close();
    return 0;
}

int WzSerialportPlus::LoraReceiveMsg()
{
    if(!(this->open("/dev/ttyS3",9600,1,8,'n')))
    {
        std::cout<<"open uart error!"<<std::endl;
        return -1;
    }

    this->setReceiveCalback([&](char* data,int length){

    // 检查接收到的消息是否在字典中
    auto it = mp3_info.find(data);
    if (it != mp3_info.end())
    {
        // 打印匹配的值
        std::cout << "接收到的消息: " << it->second << std::endl;
        std::thread musicThread([this, it]{ playMusic(it->second); });
        musicThread.detach();
    }

    });
    return 0;
}