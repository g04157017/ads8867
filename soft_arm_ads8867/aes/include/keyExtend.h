#ifndef KEYEXTEND_H
#define KEYEXTEND_H
 
#include<bitset>
#include<def.h>
using namespace std;
typedef bitset<8> m_byte;
typedef bitset<32> word;
extern m_byte S[256];
extern m_byte inv_S[256];
extern word rcon[10];
extern m_byte encry_s[4*4];
extern m_byte decry_s[4*4];
 
//定义种子密钥长度以及扩展轮数
const int Nr = 10;//轮数 
const int Nk = 4; //种子密钥有四个字 
 
//密钥扩展 相关函数
// 1 四个字节转换成一个字 
word Word(m_byte b0,m_byte b1,m_byte b2,m_byte b3);
// 2 字移位 
word CycWord(word rw);
// 3 S盒替换 
word SubWord(word sw);
// 4 密钥扩展 
void KeyExpansion(m_byte key[4*Nk],word w[4*(Nr+1)]);
// 加密使用的列混合数组
 
 
#endif
