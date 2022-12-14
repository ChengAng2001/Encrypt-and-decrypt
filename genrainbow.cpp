#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned int SHA1_tmp;
#define SHA1_ROTL(a,b) (SHA1_tmp=(a),((SHA1_tmp>>(32-b))&(0x7fffffff>>(31-b)))|(SHA1_tmp<<b))
#define SHA1_F(B,C,D,t) ((t<40)?((t<20)?((B&C)|((~B)&D)):(B^C^D)):((t<60)?((B&C)|(B&D)|(C&D)):(B^C^D)))

int UnitSHA1(const char* str, int length, unsigned sha1[5])
{
	/*
	计算字符串SHA-1
	参数说明：
	str         字符串指针
	length      字符串长度
	sha1         用于保存SHA-1的字符串指针
	返回值为参数sha1
	*/
	unsigned char* pp, * ppend;
	unsigned int l, i, K[80], W[80], TEMP, A, B, C, D, E, H0, H1, H2, H3, H4;
	H0 = 0x67452301, H1 = 0xEFCDAB89, H2 = 0x98BADCFE, H3 = 0x10325476, H4 = 0xC3D2E1F0;
	for (i = 0; i < 20; K[i++] = 0x5A827999);
	for (i = 20; i < 40; K[i++] = 0x6ED9EBA1);
	for (i = 40; i < 60; K[i++] = 0x8F1BBCDC);
	for (i = 60; i < 80; K[i++] = 0xCA62C1D6);
	l = length + ((length % 64 > 56) ? (128 - length % 64) : (64 - length % 64));
	if (!(pp = (unsigned char*)malloc((unsigned int)l))) return -1;
	for (i = 0; i < length; pp[i + 3 - 2 * (i % 4)] = str[i], i++);
	for (pp[i + 3 - 2 * (i % 4)] = 128, i++; i < l; pp[i + 3 - 2 * (i % 4)] = 0, i++);
	*((unsigned int*)(pp + l - 4)) = length << 3;
	*((unsigned int*)(pp + l - 8)) = length >> 29;
	for (ppend = pp + l; pp < ppend; pp += 64)
	{
		for (i = 0; i < 16; W[i] = ((unsigned int*)pp)[i], i++);
		for (i = 16; i < 80; W[i] = SHA1_ROTL((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1), i++);
		A = H0, B = H1, C = H2, D = H3, E = H4;
		for (i = 0; i < 80; i++)
		{
			TEMP = SHA1_ROTL(A, 5) + SHA1_F(B, C, D, i) + E + W[i] + K[i];
			E = D, D = C, C = SHA1_ROTL(B, 30), B = A, A = TEMP;
		}
		H0 += A, H1 += B, H2 += C, H3 += D, H4 += E;
	}
	free(pp - l);
	sha1[0] = H0, sha1[1] = H1, sha1[2] = H2, sha1[3] = H3, sha1[4] = H4;
	return 0;
}

void getstr(unsigned n, char str[8])
{
	str[0] = 'a'; str[1] = '0'; str[2] = '0'; str[3] = '0'; str[4] = '0'; str[5] = '0'; str[6] = '0'; str[7] = '0';
	int i = 2;
	while (n)
	{
		unsigned tmp = n % 36;
		if (tmp < 10)
			str[i++] = tmp + '0';
		else
		{
			str[i++] = tmp - 10 + 'a';
		}
		n = n / 36;
	}
}

void R(unsigned sha1[5], char str[8], int i)
{
	getstr((sha1[0] + sha1[1] * i) % 2176782336, str);
}


int main()
{
	int n;
	unsigned sha[5], _sha[5];
	char head[5000][9] = {}, tail[5000][9] = {};
	char str[50];
	scanf("%d", &n);
	getchar();
	for (int i = 0; i < n; i++)
	{
		head[i][0] = getchar();
		head[i][1] = getchar();
		head[i][2] = getchar();
		head[i][3] = getchar();
		head[i][4] = getchar();
		head[i][5] = getchar();
		head[i][6] = getchar();
		head[i][7] = getchar();
		getchar();
		tail[i][0] = getchar();
		tail[i][1] = getchar();
		tail[i][2] = getchar();
		tail[i][3] = getchar();
		tail[i][4] = getchar();
		tail[i][5] = getchar();
		tail[i][6] = getchar();
		tail[i][7] = getchar();
		getchar();
	}
	for (int i = 0; i < 5; i++)
	{
		scanf("%8x", &_sha[i]);
	}
	for (int i = 0; i < n; i++)
	{
		strcpy(str, head[i]);
		for (int j = 0; j < 1000; j++)
		{
			for (int k = 0; k < 100; k++)
			{
				UnitSHA1(str, strlen(str), sha);//求sha
				if (sha[0]==_sha[0]&& sha[1] == _sha[1] && sha[2] == _sha[2] && sha[3] == _sha[3] && sha[4] == _sha[4])
				{
					printf("%s\n", str);
					return 0;
				}
				R(sha, str, k + 1);//求str
			}
		}
	}
	printf("None\n");
	return 0;
}
