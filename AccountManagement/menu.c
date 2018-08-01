#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include"model.h"
#include"service.h"
#include"card_service.h"
#include"tool.h"
#include"global.h"

int checklength(char aa[]);
void outputMenu(){
	printf("\n\n----------菜单----------\n");
	printf("1.添加卡\n");
	printf("2.查询卡\n");
	printf("3.上机\n");
	printf("4.下机\n");
	printf("5.充值\n");
	printf("6.退费\n");
	printf("7.查询统计\n");
	printf("8.注销卡\n");
	printf("0.退出\n");
	printf("请输入菜单编号：");
}

/*添加卡*/
void add()
{ 
	char name[18]={'0'};
	char pname[1000];
	char password[8]={'0'};
	char ppassword[1000];
	float money=0.0;
	int nlength=0;
	int plength=0;
	Card card;
	struct tm* endTime;     // 截止时间
	struct tm* startTime;   // 开卡时间
	int success;
	printf("\n\n----------添加卡----------\n");
    printf("请输入卡号<1-18位>:");
    scanf("%s",&pname);
	
	if(checklength(pname)>18){
	printf("卡号超出18位，请重新输入\n");
	return;
	}
	else if(checklength(pname)<1){
		printf("卡号不能为空，请重新输入\n");
		return;
	}
	else
	{
		strcpy(name,pname);
	}
	printf("请输入密码<1-8位>：");
	scanf("%s",&ppassword);
	if(checklength(ppassword)>8){
	printf("密码超出8位，请重新输入\n");
    return ;
	}
	else if(checklength(ppassword)<1){
		printf("密码不能为空，请重新输入\n");
    return ;
}
	else
	{strcpy(password,ppassword);
	}
	printf("请输入开卡金额<RMB>：");
	scanf("%f",&money);
	strcpy(card.aName,name);
	strcpy(card.aPwd,password);
	card.fTotalUse=0;
	card.fBalance=money;
	card.nStatus=0;
	card.nDel=0;
	card.nUseCount=0;
	card.tStart = card.tEnd = card.tLast = time(NULL); 
	// 开卡时间，截止时间，最后使用时间都默认为当前时间。
	// 根据开卡时间，计算截止时间，每张卡的有效期为一年
	startTime = localtime(&card.tStart);
	endTime = localtime(&card.tEnd);
	endTime->tm_year = startTime->tm_year + 1;
	card.tEnd = mktime(endTime);
	success=addCard(card);
	if(success==FALSE){
	printf("\n\n添加失败!\n\n");
	}
	else{
	printf("\n添加成功!");
	printf("\n\n添加卡的信息如下\n");
	printf("卡号\t\t密码\t\t状态\t\t开卡金额\n");
	printf("%s\t\t%s\t\t%d\t\t%.2f\n",card.aName,card.aPwd,card.nStatus,money);
	}
}

/*查询卡*/
void query(){
	char name[18];
	char pname[1000];
	/*char password[8];
	char ppassword[1000];*/
	char nTime[20];
	Card *qCard;
	int pIndex=0;
	int i;
	//lpCardNode pCard;
	printf("\n\n----------查询卡---------\n");
	printf("请输入查询的卡号<长度为1-18>:");
	scanf("%s",&pname);
	if(checklength(pname)>18)
	{printf("卡号超出18位，请重新输入\n");
	return;
	}
	else{
	strcpy(name,pname);
	}
	/*查询密码*/
	/*printf("请输入查询的卡号<长度为1-8>:");
	scanf("%s",&ppassword);
	if(checklength(ppassword)>8)
	{
		printf("密码超出8位，请重新输入\n");
		return;
	}
	else
	{
		strcpy(password,ppassword);
	}
	*/

	/*     精确查询
	qCard=queryCard(name);
	if(qCard==NULL)
	{
		printf("未查询到此卡信息\n");
	}
	else
	{
		printf("\n\n查询到的卡信息如下：\n");
		timeToString(qCard->tLast,nTime);
		printf("卡号\t\t状态\t\t余额\t\t累计使用\t\t使用次数\t\t上次使用时间\n");
		printf("%s\t\t%d\t\t%.2f\t\t%.2f\t\t\t%d\t\t\t%s\n",qCard->aName,qCard->nStatus,qCard->fTotalUse,qCard->fBalance,qCard->nUseCount,nTime);
	}*/

	/*模糊查询*/
	
	qCard=queryCards(name,&pIndex);
	if(qCard==NULL)
	{
		printf("未查到相关卡信息\n");
	}
	else
	{
		printf("\n\n查询到的卡信息如下：\n");
		printf("卡号\t\t状态\t\t余额\t\t累计使用\t\t使用次数\t\t上次使用时间\n");
		for(i=0;i<pIndex;i++)
		{
		timeToString(qCard[i].tLast,nTime);
		printf("%s\t\t%d\t\t%.2f\t\t%.2f\t\t\t%d\t\t\t%s\n",qCard[i].aName,qCard[i].nStatus,qCard[i].fBalance,qCard[i].fTotalUse,qCard[i].nUseCount,nTime);
		}
	}

	/*根据卡号和密码查询卡信息，并返回其在链表中的位置*/

	/*pCard=checkCard(name,password);
	if(pCard==NULL)
	{
		printf("帐号或密码输入错误，请重试");
	}
	else
	{
		qCard=&(pCard->cardData);
		timeToString(qCard->tLast,nTime);
printf("卡号\t\t状态\t\t余额\t\t累计使用\t\t使用次数\t\t上次使用时间\n");
		printf("%s\t\t%d\t\t%.2f\t\t%.2f\t\t\t%d\t\t\t%s\n",qCard->aName,qCard->nStatus,qCard->fTotalUse,qCard->fBalance,qCard->nUseCount,nTime);
		printf("卡信息的地址：");
		printf("%d\n",pCard);
	}

	*/
}

/*上机函数*/
void logon()
{	
	char aName[18]={0};
	char aPwd[8]={0};
	int nResult=0;
	LogonInfo* pInfo=NULL;
	char aTime[TIMELENGTH]={0};
	printf("----------上机----------\n");
	printf("\n请输入上机卡号（长度为1-18）：");
	scanf("%s",&aName);
	printf("\n请输入上机密码（长度为1-8）：");
	scanf("%s",&aPwd);
	pInfo=(LogonInfo*)malloc(sizeof(LogonInfo));
	nResult=doLogon(aName,aPwd,pInfo);
	switch(nResult)
	{
	case 0:
		printf("上机失败\n");
		break;

    case 1:
		printf("卡号\t余额\t上机时间\n");
		timeToString(pInfo->tLogon,aTime);
		printf("%s\t%.2f\t%s\n",pInfo->aCardName,pInfo->fBalance,aTime);
		break;
	case 2:
		printf("该卡正在上机或者已经注销\n");
		break;
	case 3:
		printf("卡余额不足\n");
		break;
	default:
		{
			break;
		}
		
	}


	
}
/*下机*/
void settle()
{
	char aName[18]={0};
	char aPwd[8]={0};
	int nResult=0;
char aTime1[TIMELENGTH]={0};
char aTime2[TIMELENGTH]={0};
	SettleInfo *pInfo=NULL;
	printf("----------下机----------\n");
	printf("\n请输入下机卡号（长度为1-18）：");
	scanf("%s",&aName);
	printf("\n请输入下机密码（长度为1-8）：");
	scanf("%s",&aPwd);
	pInfo=(SettleInfo*)malloc(sizeof(SettleInfo));
	nResult=doSettle(aName,aPwd,pInfo);
	switch(nResult)
	{
	case 0:
		printf("未查到该卡上机信息，下机失败\n");
		break;

    case 1:
		timeToString(pInfo->tStart,aTime1);
		timeToString(pInfo->tEnd,aTime2);
		printf("----------下机信息如下----------\n");
		printf("卡号\t消费\t余额\t上机时间\t\t下机时间\n");
		printf("%s\t%.2f\t%.2f\t%s\t%s\n",pInfo->aCardName,pInfo->fAmount,pInfo->fBalance,aTime1,aTime2);
		break;
	case 2:
		printf("未查到该卡上机信息，下机失败\n");
		break;

	default:
		{
			break;
		}
		
	}

}


/*充值*/
void addMoney()
{
	char aName[18] = {0};
	char aPwd[8] = {0};
	printf("\n请输入账号：");
	scanf("%s",aName);
	printf("\n请输入密码：");
	scanf("%s",aPwd);
	if(TRUE == Recharge(aName,aPwd))
	{
		printf("充值成功\n");
	}
	else
	{
		printf("充值失败\n");
	}
}



void refundMoney(){
char aName[18] = {0};
	char aPwd[8] = {0};
	printf("\n请输入账号：");
	scanf("%s",aName);
	printf("\n请输入密码：");
	scanf("%s",aPwd);
	if(TRUE == Refund(aName,aPwd))
	{
		printf("退费成功\n");
	}
	else
	{
		printf("退费失败\n");
	}

}
void queryStatistics(){printf("\n\n查询统计\n");}
void annul(){char aName[32] = {0};
	char aPwd[20] = {0};
	printf("\n请输入账号：");
	scanf("%s",aName);
	printf("\n请输入密码：");
	scanf("%s",aPwd);
	if(TRUE == Cancel(aName,aPwd))
	{
		printf("注销成功\n");
	}
	else
	{
		printf("注销失败\n");
	}

}









void exitApp()
{
	releaseCardList();/*释放链表空间*/
	exit(0);
}








int checklength(char aa[])
{
	int count =0;
	while(aa[count]!='\0'){
	count++;
	}
	return count;
}
