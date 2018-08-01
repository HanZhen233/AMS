#include<stdio.h>
#include<stdlib.h>
#include"global.h"
#include"model.h"
#include"billing_file.h"

lpBillingNode BillingList;
lpBillingNode head;
lpBillingNode p,q;

/* 初始化链表*/
int initBillingList()
{
	head=(lpBillingNode)malloc(sizeof(BillingNode));
	head->next=NULL;
	BillingList=head;
	return TRUE;
}

/*释放结点*/
void releaseBillingList()
{    p=BillingList;

	while(p!=NULL)
	{
		q=p;
		p=p->next;
		free(q);
	}
}



int getBilling()
{
	Billing* pBilling = NULL;
	int i = 0;
	int nCount = 0;
	 p = NULL;
	 q = NULL;
	
	if(BillingList != NULL)
	{
		releaseBillingList();
	}
	initBillingList();
	//获取卡信息数量
	nCount = getBillingCount(BILLINGPATH);
			
	//动态分配内存
	pBilling = (Billing*)malloc(sizeof(Billing)* nCount);

	if(pBilling == NULL)
	{
		return FALSE;
	}
	
	if(readBilling(pBilling,BILLINGPATH) == FALSE)		
	{		
		free(pBilling);
		pBilling = NULL;
		return FALSE;
	}


	for(i = 0,p = BillingList;i < nCount;i++)
	{
		q = (lpBillingNode)malloc(sizeof(BillingNode));
		if(q == NULL)
		{
			/*free(pBilling);*/
			return FALSE;
		}
		memset(q, 0, sizeof(BillingNode));
		q->BillingData = pBilling[i];
		q->next = NULL;

		p->next = q;
		p = q;
	}
	/*free(pBilling);
	pBilling =	NULL;*/

	
	return TRUE;

}
Billing *checkBilling(const char *pName,int *pIndex)
{
	lpBillingNode BillingNode=NULL;
	int nIndex=0;

	if(getBilling()==FALSE){
		
	return FALSE;

	}


	BillingNode=BillingList->next;
	while(BillingNode!=NULL)
	{
		if((strcmp(BillingNode->BillingData.aCardName,pName)==0)&&
			(BillingNode->BillingData.nStatus==0)){
		*pIndex=nIndex;
		
		return &BillingNode->BillingData; 
		}
		BillingNode=BillingNode->next;
		nIndex++;
	}
	return NULL;
	
}





