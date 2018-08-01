#include<stdio.h>
#include"menu.h"
#include"card_file.h"
#include"model.h"
#include"tool.h"
#include"card_service.h"

int main(void){
	int nSelection=-1;
	char rubbish[1000];
	int num;
	
	printf("欢迎来到计费管理系统！\n");
	printf("\n");
    initCardList();/*初始化链表*/
	do{

		outputMenu();
		nSelection=-1;
		num = scanf("%d",&nSelection);
		while(num==0)
{
        // 清空缓冲区
        scanf("%s", rubbish); 
        printf("\n\n请选择正确的菜单编号:\n\n ");
        num=scanf("%d",& nSelection);
    
   
}
	
		switch(nSelection){
		case 1:
			add();
			break;
		case 2:
			 query();
			break;
		case 3:
		    logon();
			break;
        case 4:
			settle();
			break;
		case 5:
		    addMoney();
			break;

		case 6:
		    refundMoney();
			break;
        case 7:
			queryStatistics();
			break;
        case 8:
			annul();
			break;
		case 0:
			exitApp();
			break;
		default:printf("\n\n请输入正确的菜单编号:");break;
		}
	}while(nSelection!=0);
	return 0;
}