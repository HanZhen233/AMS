#ifndef SERVICE_H
#define SERVICE_H
#include"model.h"
int doLogon(const char* pName,const char* pPwd,LogonInfo *pInfo);
int doSettle(const char *pName,const char* pPwd,SettleInfo *pInfo);
#endif