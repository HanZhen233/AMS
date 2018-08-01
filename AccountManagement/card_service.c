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

/* ��ʼ������*/
int initCardList()
{
	head=(lpCardNode)malloc(sizeof(CardNode));
	head->next=NULL;
	cardList=head;
	return TRUE;
}

/*�ͷŽ��*/
void releaseCardList()
{    p=cardList;

	while(p!=NULL)
	{
		q=p;
		p=p->next;
		free(q);
	}
}

/*��ӿ�*/
int addCard(Card card){
	
	return saveCard(&card,CARDPATH);
}

/*��ȷ��ѯ��*/
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


/* ��ʼ�����������ú���������Ϣ����������*/
//���ļ��еĿ���Ϣ��������������
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
	//��ȡ����Ϣ����
	nCount = getCardCount(CARDPATH);

	//��̬�����ڴ�
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



/*ģ����ѯ*/
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





/*���ݿ��ţ������ѯ���������� �������е�λ��*/
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



//��ֵ
int Recharge(const char* pName,const char* pPwd)
{
	lpCardNode cardNode = NULL;
	float charge = 0;

	int nIndex = 0;
	//��ȡ�ļ��еĿ���Ϣ
	if(FALSE == getCard())
	{
		return FALSE; 
	}
	cardNode = cardList->next;
	//���������ж��ܷ���г�ֵ
	while(cardNode != NULL)
	{

		if(strcmp(cardNode->cardData.aName,pName) == 0 && strcmp(cardNode->cardData.aPwd,pPwd) == 0&&(cardNode->cardData.nStatus !=2))
		{
			printf("�������ֵ�Ľ�");
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


//�˷�
int Refund(const char* pName,const char* pPwd)
{
	lpCardNode cardNode = NULL;
	float charge = 0;

	int nIndex = 0;
	//��ȡ�ļ��еĿ���Ϣ
	if(FALSE == getCard())
	{
		return FALSE; 
	}

	cardNode = cardList->next;
	//���������ж��ܷ�����˷�
	while(cardNode != NULL)
	{
		if(strcmp(cardNode->cardData.aName,pName) == 0 && strcmp(cardNode->cardData.aPwd,pPwd) == 0)
		{
			printf("�˷ѽ�%f\n",cardNode->cardData.fBalance);
			
			//���������е���Ϣ
			cardNode->cardData.fBalance = 0.0;
			//��������˷ѣ������ļ�����Ϣ

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


//ע��
int Cancel(const char* pName,const char* pPwd)
{
	lpCardNode cardNode = NULL;
	int nIndex = 0;
	//��ȡ�ļ��еĿ���Ϣ
	if(FALSE == getCard())
	{
		return FALSE; 
	}

	cardNode = cardList->next;
	//���������ж��ܷ����ע��
	while(cardNode != NULL)
	{
		if(strcmp(cardNode->cardData.aName,pName) == 0 && strcmp(cardNode->cardData.aPwd,pPwd) == 0)
		{
			//ֻ��״̬Ϊδʹ�õĿ�����ע����ע�������Ӧ����0
			if(cardNode->cardData.nStatus != 0)
			{
				return FALSE;
			}
			if(cardNode->cardData.fBalance < 0)
			{
				return FALSE;
			}
			//���������е���Ϣ
			printf("���ţ�\t�˷ѽ��\n");
			printf("%s\t%.1f\n",cardNode->cardData.aName,cardNode->cardData.fBalance);

			cardNode->cardData.nStatus = 2;
			printf("%d\n",cardNode->cardData.nStatus);
			cardNode->cardData.fBalance = 0.0;
			//�������ע���������ļ�����Ϣ

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