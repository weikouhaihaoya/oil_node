
#include <iostream>
#include <iomanip>
#include "serial_tool.h"

int main(int argc,char** argv) {
    std::string flag = argv[1];

    if(flag=="1")
    {
        WzSerialportPlus wzSerialportPlus;
        int ret = 0;
        if((ret = wzSerialportPlus.LoraReceiveMsg())<0)
        {
            return -1;
        }
        while (true){}
    }
   
    if(flag == "2")
    {
         int ret = 0;
         WzSerialportPlus wzSerialportPlus;

         if((ret = wzSerialportPlus.LoraSendMsg(argv[2], sizeof(argv[2])))<0)
         {
            return -1;
         }
    }

    return 0;
}

