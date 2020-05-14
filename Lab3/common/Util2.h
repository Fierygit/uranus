//
// Created by Firefly on 2020/5/10.
//

#ifndef LAB3_UTIL_H
#define LAB3_UTIL_H

#include <string>
#include "public.h"
#include <stdio.h>


class Util {
public:
    /*
     * 字符串到 Command，解析不到返回空的
     */

    static Command strToCommand(std::string &buf) {
        return Command{};
    }

    /*
     * //!!!解析失败请返回 null
     */
    static void EncoderNum(std::string &RespBuf, int sum) {
        if(sum!=0){
            if(sum>=10) {
                int sumback=0;
                int tempSum=sum;
                LOG_F(INFO,"sum:%d",sum);
                for(; sum!=0; ) {
                    sumback=sumback*10+sum%10;
                    sum=sum/10;
                }
                for(; sumback!=0; ) {
                    RespBuf+=(sumback%10+48);
                    sumback=sumback/10;
                }
                while(tempSum!=0&&tempSum%10==0) {
                    RespBuf+=48;
                    tempSum=tempSum/10;
                }
            } else {
                RespBuf+=sum+48;
            }
        }
    }
    static std::string Encoder(std::string &buf) {
        std::string RespBuf;
        std::string OP;
        std::string key;
        std::string value;
        if(buf.empty()) {
            LOG_F(ERROR,"buf is empty");
            return "null";
        }
        for(int i=0; i<buf.length(); i++) {
            for(; i<buf.length(); i++) {
                if(buf[i]==' ')continue;
                else break;
            }
            if(i+3<buf.length()) {
                OP+=buf[i];
                OP+=buf[i+1];
                OP+=buf[i+2];
                break;
            }
            else return "null";
        }
        if(OP=="SET") {
            int i=0;
            for(; i<buf.length(); i++) {
                if(buf[i]=='T')break;
            }
            i++;
            if(buf[i]!=' ')return "null";
            for(; i<buf.length(); i++) {
                if(buf[i]==' ')continue;
                else break;
            }
            for(; i<buf.length(); i++) {
                if(buf[i]==' ')break;
                key+=buf[i];
            }
            if(key.empty()) {
                LOG_F(ERROR,"buf is empty");
                return "null";
            }
            for(; i<buf.length(); i++) {
                if(buf[i]==' ')continue;
                else break;
            }
            if(buf[i]!='\"') {
                LOG_F(ERROR,"not know what");
                return "null";
            }
            for(i++; i<buf.length(); i++) {
                if(buf[i]==' ')continue;
                else break;
            }
            for(; i<buf.length(); i++) {
                if(buf[i]=='\"')break;
                value+=buf[i];
            }
            if(value.empty()) {
                LOG_F(ERROR,"value is empty");
                return "null";
            }
            if(buf[i]!='\"') {
                LOG_F(ERROR,"don't have \" in the end");
                return "null";
            }
            for(i++; i<buf.length(); i++) {
                if(buf[i]==' ')continue;
                else {
                    LOG_F(ERROR,"wrong ending");
                    return "null";
                }
            }
            LOG_F(INFO,"OP:%s",OP.c_str());
            LOG_F(INFO,"key:%s",key.c_str());
            LOG_F(INFO,"value:%s",value.c_str());
            int encodeCount = 1;
            for(int i=0; i<value.length(); i++) {
                    if(value[i]==' ')encodeCount++;
                for(; i<value.length(); i++) {
                    if(value[i]==' ')continue;
                    else break;
                }
            }
            if(value[0]==' ')encodeCount--;
            if(value[value.length()-1]==' ')encodeCount--;
            LOG_F(INFO,"encodeCount:%d",encodeCount);
            RespBuf+='*';
            EncoderNum(RespBuf,encodeCount+2);
            RespBuf+="\r\n";
            RespBuf+='$';
            RespBuf+=OP.length()+48;
            RespBuf+="\r\n";
            RespBuf+=OP;
            RespBuf+="\r\n";
            RespBuf+='$';
            EncoderNum(RespBuf,key.length());
            RespBuf+="\r\n";
            RespBuf+=key;
            RespBuf+="\r\n";
            if(encodeCount==1) {
                RespBuf+='$';
                int sum=0;
                for(int i=0; i<value.length(); i++) {
                    if(value[i]==' ')break;
                    sum++;
                }
                EncoderNum(RespBuf,sum);
                RespBuf+="\r\n";
                for(int i=0; i<sum; i++) RespBuf+=value[i];
                RespBuf+="\r\n";
            } else if(encodeCount>1){
                for(int i=0,j=0,ij=0,sum=0; i<encodeCount; i++) {
                    if(j==0&&value[0]==' ') {
                        for(; j<value.length(); j++) {
                            if(value[j]==' ')continue;
                            else break;
                        }
                    }
                    RespBuf+='$';
                    sum=0;
                    ij=j;
                    for(; j<value.length(); j++) {
                        if(value[j]==' ')break;
                        sum++;
                    }
                    if(sum!=0){
                        EncoderNum(RespBuf,sum);
                        RespBuf+="\r\n";
                    }
                    for(;ij<j; ij++)RespBuf+=value[ij];
                    RespBuf+="\r\n";
                    for(; j<value.length(); j++) {
                        if(value[j]==' ')continue;
                        else break;
                    }
                }
            }

        }
        else if(OP=="GET") {
            int i=0;
            for(; i<buf.length(); i++) {
                if(buf[i]=='T')break;
            }
            i++;
            if(buf[i]!=' ')return "null";
            for(; i<buf.length(); i++) {
                if(buf[i]==' ')continue;
                else break;
            }
            for(; i<buf.length(); i++) {
                if(buf[i]==' ')break;
                key+=buf[i];
            }
            if(key.empty()) {
                LOG_F(ERROR,"buf is empty");
                return "null";
            }
            for(; i<buf.length(); i++) {
                if(buf[i]!=' ')return "null";
            }
            LOG_F(INFO,"OP:%s",OP.c_str());
            LOG_F(INFO,"key:%s",key.c_str());
            RespBuf+='*';
            RespBuf+=50;
            RespBuf+="\r\n";
            RespBuf+='$';
            RespBuf+=OP.length()+48;
            RespBuf+="\r\n";
            RespBuf+=OP;
            RespBuf+="\r\n";
            RespBuf+='$';
            EncoderNum(RespBuf,key.length());
            RespBuf+="\r\n";
            RespBuf+=key;
            RespBuf+="\r\n";
        }
        else if(OP=="DEL") {
            int encodeCount = 1;
            for(int i=0; i<buf.length(); i++) {
                if(buf[i]==' ')encodeCount++;
                for(; i<buf.length(); i++) {
                    if(buf[i]==' ')continue;
                    else break;
                }
            }
            if(buf[0]==' ')encodeCount--;
            if(buf[buf.length()-1]==' ')encodeCount--;
            LOG_F(INFO,"encodeCount:%d",encodeCount);
            if(encodeCount<=1)return "null";
            else if(encodeCount>1){
                RespBuf+='*';
                EncoderNum(RespBuf,encodeCount);
                RespBuf+="\r\n";
                for(int i=0,j=0,ij=0,sum=0; i<encodeCount; i++) {
                    if(j==0&&buf[0]==' ') {
                        for(; j<buf.length(); j++) {
                            if(buf[j]==' ')continue;
                            else break;
                        }
                    }
                    RespBuf+='$';
                    sum=0;
                    ij=j;
                    for(; j<buf.length(); j++) {
                        if(buf[j]==' ')break;
                        sum++;
                    }
                    EncoderNum(RespBuf,sum);
                    RespBuf+="\r\n";
                    for(;ij<j; ij++)RespBuf+=buf[ij];
                    RespBuf+="\r\n";
                    for(; j<buf.length(); j++) {
                        if(buf[j]==' ')continue;
                        else break;
                    }
                }
            }
        }
        else {
            return "null";
        }

        return RespBuf;
    }

    static Command Decoder(std::string &buf) {
        return Command();
    }

private:


};


#endif //LAB3_UTIL_H
