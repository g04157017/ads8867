#ifndef DECRYPT_H
#define DECRYPT_H
#include"encryption.h"
 
 
using namespace std;
 
// 1 逆行变换
void InvShiftRow(m_byte sta_matr[4*4]);
 
// 2 逆S盒变换
void InvSubBytes(m_byte sta_matr[4*4]);
 
// 3 逆列变换
void InvMixColumns(m_byte sta_matr[4*4]);
 
// 4 解密函数
void decrypt(m_byte in[4*4],word w[4*(Nr+1)]);  
 
#endif
