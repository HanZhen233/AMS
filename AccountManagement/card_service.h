#ifndef CARD_SERVICE_H
#define CARD_SERVICE_H
int initCardList();
void releaseCardList();
int addCard(Card ncard);
Card *queryCard(const char name[]);
Card *queryCards(const char name[],int *pIndex);
Card *checkCard(const char *pName,const char* pPwd,int *nIndex);
int  getCards();
int Recharge(const char* pName,const char* pPwd);
int Refund(const char* pName,const char* pPwd);
int Cancel(const char* pName,const char* pPwd);
#endif