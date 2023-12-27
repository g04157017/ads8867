#include"encryption.h"
#include"decrypt.h"
#include<iostream>
#include<bitset>
 
int main(){
	//种子密钥 
	m_byte key[16] = {0x2b, 0x7e, 0x15, 0x16, 
					0x28, 0xae, 0xd2, 0xa6, 
					0xab, 0xf7, 0x15, 0x88, 
					0x09, 0xcf, 0x4f, 0x3c};
//	byte key[16] = {0x00, 0x01, 0x02, 0x03, 
//					0x04, 0x05, 0x06, 0x07, 
//					0x08, 0x09, 0x01, 0x02, 
//					0x03, 0x04, 0x05, 0x06};
//	byte key[16] = {0x00, 0x01, 0x02, 0x03, 
//						0x04, 0x05, 0x06, 0x07, 
//						0x08, 0x09, 0x0a, 0x0b, 
//						0x0c, 0x0d, 0x0e, 0x0f};
 
 
	//输入的明文 
	m_byte sta_matr[16]={0x32,0x88,0x31,0xe0,
						0x43,0x5a,0x31,0x37,
						0xf6,0x30,0x98,0x07,
						0xa8,0x8d,0xa2,0x34};
//    byte sta_matr[16]={0x61,0x62,0x63,0x64,
//    					0x65,0x66,0x67,0x68,
//    					0x69,0x6A,0x6B,0x6C,
//    					0x6D,0x6E,0x6F,0x70					
//	}; 
//	byte sta_matr[16] = {0x00, 0x11, 0x22, 0x33, 
//							0x44, 0x55, 0x66, 0x77, 
//							0x88, 0x99, 0xaa, 0xbb, 
//							0xcc, 0xdd, 0xee, 0xff};
 
						
	//进行密钥扩展
	word w[4*(Nr+1)];
	KeyExpansion(key,w); 
	
	//输出密钥
	cout<<"密钥是:"<<endl;
	for(int i=0;i<16;i++){
		cout<<hex<<key[i].to_ulong()<<"  ";
		if((i+1)%4==0)cout<<endl;
	}   
	cout<<endl;
	//输出明文
	cout<<"明文是:"<<endl;
	for(int i=0;i<16;i++){
		cout<<hex<<sta_matr[i].to_ulong()<<"  ";
		if((i+1)%4==0)cout<<endl;
	}  
	//输出密文
	cout<<endl;
	encrypt(sta_matr,w);
	cout<<"密文是:"<<endl;
	for(int i=0;i<16;i++){
		cout<<hex<<sta_matr[i].to_ulong()<<"  ";
		if((i+1)%4==0)cout<<endl;
	}  
	
	//再次进行解密
	cout<<endl;
	cout<<"密文解密是:"<<endl;
	decrypt(sta_matr,w);
	for(int i=0;i<16;i++){
		cout<<hex<<sta_matr[i].to_ulong()<<"  ";
		if((i+1)%4==0)cout<<endl;
	}  
	return 0;
	
 
 
	
/*
	以下是函数正确性验证部分
		1 密钥扩展验证
			for(int i=0;i<4*(Nr+1);i++){
			cout<<"w["<<dec<<i<<"]="<<hex<<w[i].to_ulong()<<endl;
			}
		
		2 验证有限域乘法
			byte temp=GFMul(0x57,0x83);
			for(int i=0;i<8;i++){
				cout<<temp[i]<<" ";}
				cout<<endl;
			cout<<"0x57*0x83:"<<hex<<temp.to_ulong(); 
		
		3 测试字节代换有无问题
			SubBytes(sta_matr);
			cout<<"字节代换后是:"<<endl;
			for(int i=0;i<16;i++){
				cout<<hex<<sta_matr[i].to_ulong()<<"  ";
				if((i+1)%4==0)cout<<endl;
			} 
			
		4 测试列混合函数是否正确
			byte s[16] = {0xc9,0xe5,0xfd,0x2b, 
					0x7a,0xf2,0x78,0x6e,
					0x63,0x9c,0x26,0x67,
					0xb0,0xa7,0x82,0xe5}; 
			MixColumns(s,encry_s);
			for(int i=0;i<16;i++){
				cout<<hex<<s[i].to_ulong()<<"  ";
				if((i+1)%4==0)cout<<endl;
			} 	
			
		
		
*/	
	
/*
	以下维验证bitset类型数据的存储以及计算方式
		1 小端模式：即常规思维的0011 0011在计算机存储变成1100 1100
		2 定义的两个byte(bitset<8>)数据进行异或也是按照两个小端形式的数据进行异或，存储的依旧是小端格式 
		3 如果按位输出 小端格式  按字节输出 大端格式 
		4 比如使用a则正常顺序(大端) 仅当按位的时候才是小端 
		5 a^b 按字节输出是正常大端顺序 
			byte temp0=0x57;
			cout<<temp<<endl;
			cout<<(temp>>1)<<endl;
			cout<<temp[5];
			byte temp2=0x83;
			cout<<(temp^temp2);
			cout<<"temp1:";
			for(int i=0;i<8;i++){
				cout<<temp[i]<<" ";
			} 
			cout<<endl; 
			cout<<"temp2:";
			for(int i=0;i<8;i++){
				cout<<temp2[i]<<" ";
			} 
			cout<<endl; 
			cout<<"temp1^temp2:";
			byte temp3=temp^temp2;
			for(int i=0;i<8;i++){
				cout<<temp3[i]<<" ";
			} 
			cout<<dec<<temp3.to_ulong();
*/		 
} 
