#include<stdio.h>
#include"model.h"
#include<string.h>
#include<stdlib.h>
#include"global.h"
#include"card_file.h"
Card ncard[50];
int i;
lpCardNode cardList;
lpCardNode head;
lpCardNode p,q;

/* 初始化链表*/
int initCardList()
{
	head=(lpCardNode)malloc(sizeof(CardNode));
	head->next=NULL;
	cardList=head;
	return TRUE;
}

/*释放结点*/
void releaseCardList()
{    p=cardList;

	while(p!=NULL)
	{
		q=p;
		p=p->next;
		free(q);
	}
}

/*添加卡*/
int addCard(Card card){
	
	return saveCard(&card,CARDPATH);
}

/*精确查询卡*/
Card *queryCard(const char name[])
{

	Card *pCard=NULL;
	p=head;
	while(p!=NULL){
		if(strcmp(p->cardData.aName,name)==0)
		{
			pCard=&p->cardData;
			break;
		}
		p=p->next;		
	}
	return pCard;
}


/* 初始化链表，并调用函数将卡信息存入链表中*/
//将文件中的卡信息保存在链表里面
int getCard()
{
	Card* pCard = NULL;
	int i = 0;
		int nCount = 0;
	 p = NULL;
	q = NULL;

	if(cardList != NULL)
	{
		releaseCardList();
	}
	initCardList();
	//获取卡信息数量
	nCount = getCardCount(CARDPATH);

	//动态分配内存
	pCard = (Card*)malloc(sizeof(Card)* nCount);

	if(pCard == NULL)
	{
		return FALSE;
	}
		
	if(readCard(pCard,CARDPATH) == FALSE)		
	{	
		free(pCard);
		pCard = NULL;
		return FALSE;
	}
	for(i = 0,p = cardList;i < nCount;i++)
	{
		q = (lpCardNode)malloc(sizeof(CardNode));
		if(q == NULL)
		{
			free(pCard);
			return FALSE;
		}
		memset(q, 0, sizeof(CardNode));
		q->cardData = pCard[i];
		q->next = NULL;

		p->next = q;
		p = q;
	}
/*	free(pCard);
	pCard =	NULL;*/
	return TRUE;

}



/*模糊查询*/
Card *queryCards(const char name[],int *pIndex)
{

	Card *pCard=NULL;
	if(getCard() == FALSE)
	{
		return NULL;
	}

	p=head;
	while(p!=NULL)
	{
		if(strstr(p->cardData.aName,name)!=NULL)
		{
			(*pIndex)++;
			pCard=(Card*)realloc(pCard,(*pIndex)*(sizeof(Card)));
			pCard[(*pIndex-1)]=p->cardData;
		}
		p=p->next;
	}

	return pCard;
}





/*根据卡号，密码查询，并返回其 在链表中的位置*/
Card *checkCard(const char *pName,const char* pPwd,int *pIndex)
{
	lpCardNode cardNode=NULL;
	int nIndex=0;
		
	if(getCard()==FALSE){
	return FALSE;

	}
	cardNode=cardList->next;
	while(cardNode!=NULL)
	{
		if(strcmp(cardNode->cardData.aName,pName)==0&&
			(strcmp(cardNode->cardData.aPwd,pPwd)==0)){
		*pIndex=nIndex;
		return &cardNode->cardData; 
		}
		cardNode=cardNode->next;
		nIndex++;
	}


	return NULL;
	
}



//充值
int Recharge(const char* pName,const char* pPwd)
{
	lpCardNode cardNode = NULL;
	float charge = 0;

	int nIndex = 0;
	//获取文件中的卡信息
	if(FALSE == getCard())
	{
		return FALSE; 
	}
	cardNode = cardList->next;
	//遍历链表，判断能否进行充值
	while(cardNode != NULL)
	{

		if(strcmp(cardNode->cardData.aName,pName) == 0 && strcmp(cardNode->cardData.aPwd,pPwd) == 0&&(cardNode->cardData.nStatus !=2))
		{
			printf("请输入充值的金额：");
			scanf("%f",&charge);
			cardNode->cardData.fBalance += charge;

			if(updateCard(&cardNode->cardData,CARDPATH,nIndex) == TRUE)
			{
				return TRUE;
			}                             
			
		}
		cardNode = cardNode->next;
		nIndex++;
	}
	return FALSE;
}


//退费
int Refund(const char* pName,const char* pPwd)
{
	lpCardNode cardNode = NULL;
	float charge = 0;

	int nIndex = 0;
	//获取文件中的卡信息
	if(FALSE == getCard())
	{
		return FALSE; 
	}

	cardNode = cardList->next;
	//遍历链表，判断能否进行退费
	while(cardNode != NULL)
	{
		if(strcmp(cardNode->cardData.aName,pName) == 0 && strcmp(cardNode->cardData.aPwd,pPwd) == 0)
		{
			printf("退费金额：%f\n",cardNode->cardData.fBalance);
			
			//更新链表中的信息
			cardNode->cardData.fBalance = 0.0;
			//如果可以退费，更新文件卡信息

			if(updateCard(&cardNode->cardData,CARDPATH,nIndex) == TRUE)
			{
				return TRUE;
			}                             
			
		}
		cardNode = cardNode->next;
		nIndex++;
	}
	return FALSE;
}


//注销
int Cancel(const char* pName,const char* pPwd)
{
	lpCardNode cardNode = NULL;
	int nIndex = 0;
	//获取文件中的卡信息
	if(FALSE == getCard())
	{
		return FALSE; 
	}

	cardNode = cardList->next;
	//遍历链表，判断能否进行注销
	while(cardNode != NULL)
	{
		if(strcmp(cardNode->cardData.aName,pName) == 0 && strcmp(cardNode->cardData.aPwd,pPwd) == 0)
		{
			//只有状态为未使用的卡才能注销，注销卡余额应大于0
			if(cardNode->cardData.nStatus != 0)
			{
				return FALSE;
			}
			if(cardNode->cardData.fBalance < 0)
			{
				return FALSE;
			}
			//更新链表中的信息
			printf("卡号：\t退费金额\n");
			printf("%s\t%.1f\n",cardNode->cardData.aName,cardNode->cardData.fBalance);

			cardNode->cardData.nStatus = 2;
			printf("%d\n",cardNode->cardData.nStatus);
			cardNode->cardData.fBalance = 0.0;
			//如果可以注销，更新文件卡信息

			if(updateCard(&cardNode->cardData,CARDPATH,nIndex) == TRUE)
			{
				return TRUE;
			}                             
			
		}
		cardNode = cardNode->next;
		nIndex++;
	}
	return FALSE;
}