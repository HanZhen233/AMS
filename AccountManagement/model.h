#ifndef MODEL_H
#define MODEL_H

#include<stdio.h>
#include<time.h>
#include<string.h>

typedef struct Card
{
char aName[18];         // 卡号
char aPwd[8];           // 密码
int  nStatus;           // 卡状态(0-未上机；1-正在上机；2-已注销；3-失效)
time_t tStart;          // 开卡时间
time_t tEnd;            // 卡的截止时间 
float fTotalUse;        // 累计金额
time_t tLast;           // 最后使用时间
int nUseCount;          // 使用次数
float fBalance;         // 余额
int nDel;               // 删除标识 0-未删除,1-删除
}Card;
typedef struct CardNode
{
	Card cardData;
    struct CardNode *next;
}CardNode,*lpCardNode;

typedef struct Billing {
	char aCardName[18];
	time_t tStart;
	time_t tEnd;
	int nStatus;
	float fAmount;
	int nDel;
}Billing;

typedef struct BillingNode{
  Billing BillingData;
  struct BillingNode *next;
}BillingNode,*lpBillingNode;

typedef struct LogonInfo{
	char aCardName[18];
	float fBalance;
	time_t tLogon;
}LogonInfo;

typedef struct SettleInfo{
    char aCardName[18];
    float fAmount;
	float fBalance;
	time_t tStart;
	time_t tEnd;
}SettleInfo;


#endif