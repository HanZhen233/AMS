#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"model.h"
#include"card_service.h"
#include"global.h"
#include"card_file.h"
#include"billing_file.h"
#include"billing_service.h"
/*上机*/
int doLogon(const char *pName,const char* pPwd,LogonInfo *pInfo)
{
	Card *pCard =NULL;
	int nIndex=0;
	Billing billing;
	pCard=checkCard(pName,pPwd,&nIndex);
	if(pCard==NULL){
	return FALSE;
	
	}
	if(pCard->nStatus!=0)
	{
	return UNUSE;
	}
	if(pCard->fBalance<=0)
	{
		return INSUFFICIENTBALANCE;
	}

	pCard->nStatus=1;
	pCard->nUseCount++;
	pCard->tLast=time(NULL);
	if(updateCard(pCard,CARDPATH,nIndex)==FALSE)
	{
		return FALSE;
	}
	strcpy(billing.aCardName,pName);
	billing.tStart=time(NULL);
	billing.tEnd=0;
	billing.nStatus=0;
	billing.fAmount=0;
	billing.nDel=0;
	if(saveBilling(&billing,BILLINGPATH)==TRUE)
	{
		strcpy(pInfo->aCardName,pName);
		pInfo->fBalance=pCard->fBalance;
		pInfo->tLogon=billing.tStart;
		
	}
	return TRUE;

}
/*下机*/
int doSettle(const char *pName,const char* pPwd,SettleInfo *pInfo)
{
	Card *pCard =NULL;
	Billing *pBilling=NULL;
	int nIndex=0;
	int nIndex1=0;
	float cost=0;
	int costTime=0;
	float costMoney=0;
	pCard=checkCard(pName,pPwd,&nIndex);
	if(pCard==NULL)
	return FALSE;
	if(pCard->nStatus!=1){
	return UNUSE;
	}

	


	/*更新卡信息和账单信息*/
	pCard->nStatus=0;
	
	pBilling=checkBilling(pName,&nIndex1);

	pBilling->nStatus=1;
	pBilling->tEnd=time(NULL);
	cost=(float)difftime(pBilling->tStart,pBilling->tStart);
	costTime=(int)(cost/60)+1;
	if(costTime%UNIT==0){
		costMoney=(int)(costTime/UNIT)*CHARGE;
	}
	else
	{
		costMoney=(int)(costTime/UNIT+1)*CHARGE;
	}
		pBilling->fAmount=costMoney;
		pCard->fBalance-=costMoney;
		pCard->fTotalUse+=costMoney;

				
	if(updateBilling(pBilling,BILLINGPATH,nIndex1)==TRUE&&(updateCard(pCard,CARDPATH,nIndex)==TRUE))
	{
		strcpy(pInfo->aCardName,pName);
		pInfo->fAmount=pBilling->fAmount;
		pInfo->fBalance=pCard->fBalance;
		pInfo->tStart=pBilling->tStart;
		pInfo->tEnd=pBilling->tEnd;
	}

 return TRUE;
	}



