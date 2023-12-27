#include<iostream>
#include<string>
#include"decrypt.h"
 
 
using namespace std;
 
// 1 逆行变换  循环右移     已测试没有问题 
void InvShiftRow(m_byte sta_matr[4*4]){
	for(int i=0;i<4;i++){
		m_byte temp[i];
		//存数 防止被覆盖
		for(int j=0;j<i;j++){
			temp[j]=sta_matr[i*4+3-j];
		} 
		// 将不会发生下标溢出的进行赋值 
		for(int j=0;j<4-i;j++){
			sta_matr[i*4+3-j]=sta_matr[i*4+3-j-i];	
		}
		// 将暂存的数放回状态数组 行中 
		for(int m=0;m<i;m++){
			sta_matr[i*4+m]=temp[i-m-1];
		}
	} 
 
}
// 2 逆S盒变换    没变化没问题 
void InvSubBytes(m_byte sta_matr[4*4]){
	// 将16个字节依次进行代换
	for(int i=0;i<16;i++){
		//bitset地址存放是低位在前,高位在后,与常规相反,计算需要谨慎 
		int row = sta_matr[i][7]*8+sta_matr[i][6]*4+sta_matr[i][5]*2+sta_matr[i][4];
		int col = sta_matr[i][3]*8+sta_matr[i][2]*4+sta_matr[i][1]*2+sta_matr[i][0];
		sta_matr[i] = inv_S[row*16+col]; 
	} 
}
 
// 3 逆列变换   没变化没问题 
void InvMixColumns(m_byte sta_matr[4*4],m_byte s[4*4]){
	m_byte matr[4];
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)
			matr[j] = sta_matr[i+j*4];
 
		sta_matr[i] = GFMul(s[0], matr[0]) ^ GFMul(s[1], matr[1]) ^ GFMul(s[2], matr[2]) ^ GFMul(s[3], matr[3]);
		sta_matr[i+4] = GFMul(s[4], matr[0]) ^ GFMul(s[5], matr[1]) ^ GFMul(s[6], matr[2]) ^ GFMul(s[7], matr[3]);
		sta_matr[i+8] = GFMul(s[8], matr[0]) ^ GFMul(s[9], matr[1]) ^ GFMul(s[10], matr[2]) ^ GFMul(s[11], matr[3]);
		sta_matr[i+12] = GFMul(s[12], matr[0]) ^ GFMul(s[13], matr[1]) ^ GFMul(s[14], matr[2]) ^ GFMul(s[15], matr[3]);
	}
}
 
// 4 解密函数 
void decrypt(m_byte sta_matr[4*4],word w[4*(Nr+1)]){
	word key[4];
	for(int i=0; i<4; i++)
		key[i] = w[4*Nr+i];
	//先进行一次轮密钥加 
	RKey_Add(sta_matr,key);
	//九轮操作  逆行移位   逆S盒 轮密钥加  逆列混合 
	for(int r=Nr-1; r>0; r--)
	{
		InvShiftRow(sta_matr);
		InvSubBytes(sta_matr);
		for(int i=0; i<4; i++)
			key[i] = w[4*r+i];
		RKey_Add(sta_matr, key);
		InvMixColumns(sta_matr,decry_s); 	
	}
 	//第十轮   逆行移位   逆S盒 轮密钥加 
	InvShiftRow(sta_matr);
		InvSubBytes(sta_matr);
		for(int i=0; i<4; i++)
			key[i] = w[i];
		RKey_Add(sta_matr, key);
} 
 

