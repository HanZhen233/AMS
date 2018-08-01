#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"model.h"
#include"global.h"
#include"tool.h"

int saveBilling(const Billing *pBilling,const char *pPath)
{
	FILE* fp = NULL;
	char startTime[20] = {0};
	char endTime[20] = {0};
	char lastTime[20] = {0};

	//打开文件
	
	if((fp = fopen(pPath,"a")) == NULL)
	{
		fp = fopen(pPath,"w");
		if(fp == NULL)
		{
			return FALSE;
		}

	}
	timeToString(pBilling->tStart,startTime);
	timeToString(pBilling->tEnd,endTime);
	
	//将数据写进文件
	fprintf(fp,"%s##%s##%s##%d##%.2f##%d\n",pBilling->aCardName,startTime,
		endTime,pBilling->nStatus,pBilling->fAmount,pBilling->nDel);
	//关闭文件
	fclose(fp);
	return TRUE;
}





int getBillingCount(const char* pPath)
{
	FILE* fp = NULL;
	int nCount = 0;
	char aBuf[BILLINGCHARNUM] = {0};
	//打开文件
	fp = fopen(pPath,"r");
	if(fp == NULL)
	{
		return FALSE;
	}
	//读取文件
	while(!feof(fp))
	{
		memset(aBuf,0,BILLINGCHARNUM);
		if((fgets(aBuf,BILLINGCHARNUM,fp)) != NULL)
		{
			if(strlen(aBuf) > 0)
			{
				nCount++;
			}
		}
	}		//关闭文件
	fclose(fp);
	return nCount;
}

Billing praseBilling(char* pBuf)
{
	Billing billing;
	const char *delims =  "##";
	char flag[6][20] = {0};
	int index = 0;
	char *buf = NULL;
	char *str = NULL;

	buf = pBuf;
	while((str = strtok(buf,delims)) != NULL)
	{
		strcpy(flag[index],str);
		buf = NULL;
		index++;
	}
	
	strcpy(billing.aCardName,flag[0]);
	billing.tStart=stringToTime(flag[1]);
	billing.tEnd=stringToTime(flag[2]);
	billing.nStatus = atoi(flag[3]);
	billing.fAmount=(float)atof(flag[4]);
	billing.nDel=atoi(flag[5]);
	
	return billing;
}



int readBilling(Billing* pBilling,const char* pPath)
{	
	FILE* fp = NULL;
	int i = 0;
	char aBuf[BILLINGCHARNUM] = {0};
	//打开文件

	fp = fopen(pPath,"r");
	if(fp == NULL)
	{
		return FALSE;
	}
	//读取文件
	
	while(!feof(fp))
	{
		memset(aBuf,0,BILLINGCHARNUM);
		if((fgets(aBuf,BILLINGCHARNUM,fp)) != NULL)
		{
			if(strlen(aBuf) > 0)
			{
				pBilling[i] = praseBilling(aBuf);
				i++;
			}
		}
	}
	
		
	//关闭文件
	fclose(fp);
	
	return TRUE;
}


int updateBilling(const Billing* pBilling,const char* pPath,int nIndex)
{
	FILE* fp = NULL;
	int nLine = 0;
	char aBuf[BILLINGCHARNUM] = {0};
	long lPosition = 0;
	char startTime[TIMELENGTH] = {0};
	char endTime[TIMELENGTH] = {0};
	fp = fopen(pPath,"rb+");
	if(fp == NULL)
	{
		return FALSE;
	}
	//遍历文件,找到该条记录，进行更新
	while(!feof(fp) && nLine < nIndex)
	{
		if(fgets(aBuf,BILLINGCHARNUM,fp) != NULL)
		{
			lPosition = ftell(fp);
			nLine++;
		}
	}

	fseek(fp,lPosition,0);

	timeToString(pBilling->tStart,startTime);
	timeToString(pBilling->tEnd,endTime);
	
	//将数据写进文件
	fprintf(fp,"\n%s##%s##%s##%d##%.2f##%d\n",pBilling->aCardName,startTime,
		endTime,pBilling->nStatus,pBilling->fAmount,pBilling->nDel);

	fclose(fp);
	return TRUE;
}
