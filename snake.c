#include "snake.h"

/*全局变量定义*/
Snake snake;	//定义蛇结构体变量
Food food;		//定义食物结构体变量
Block block = {1000,1000};    //定义障碍
char now_Dir = RIGHT;	//当前蛇头方向
char direction = RIGHT; //预期蛇头方向
int paused = 0; // 0: 运行状态, 1: 暂停状态
int c_index=0;
int score=0;
int foods=0;
int MAX=0;
int rannum;
/*主菜单实现*/
int Menu() {
	GotoXY(40, 12); 			     //定位光标位置
	printf("欢迎来到贪吃蛇小游戏");
	GotoXY(43, 14);
	printf("1.开始游戏");
	GotoXY(43, 16);
	printf("2.帮助");
	GotoXY(43, 18);
	printf("3.关于");
	GotoXY(43, 20);
	printf("4.排行榜");
	GotoXY(43, 22);
	printf("其他任意键退出游戏");
	Hide();	                       //隐藏光标
	char ch;
	int result = 0;
	ch = _getch();   			    //接收用户输入的菜单选项
	switch (ch) {				//根据选项设置返回结果值
	case '1': result = 1; break;
	case '2': result = 2; break;
	case '3': result = 3; break;
	case '4': result = 4; break;
	}
	system("cls");  				//调用系统命令cls完成清屏操作
	return result;
}

//光标定位函数，将光标定位到(x,y)坐标位置  
void GotoXY(int x, int y) {
	HANDLE hout;
	COORD cor;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	cor.X = x;
	cor.Y = y;
	SetConsoleCursorPosition(hout, cor);
}

/*隐藏光标*/
void Hide() {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);    //hout是一个句柄，用来指挥一个设备或者对象，这里是STD_OUTPUT_HANDLE，就是标准输出设备（控制台窗口）
	CONSOLE_CURSOR_INFO cor_info = { 1, 0 };         //windows api定义的结构体，用以管理光标属性，第一个是大小，第二个是是否可见
	SetConsoleCursorInfo(hout, &cor_info);          //告诉函数，设置该句柄所对应的光标
}


/*关于菜单实现*/
void About() {
	GotoXY(30, 12);
	printf("杭州电子科技大学--程序设计综合实践案例");
	GotoXY(43, 14);
	printf("贪吃蛇-控制台游戏");
	GotoXY(43, 16);
	printf("按任意键返回上级菜单");
	Hide();		//隐藏光标
	char ch = _getch();
	system("cls");
}

/*帮助菜单实现*/
void Help() {
	GotoXY(40, 12);
	printf("w 上");
	GotoXY(40, 14);
	printf("s 下");
	GotoXY(40, 16);
	printf("a 左");
	GotoXY(40, 18);
	printf("d 右");
	GotoXY(40, 20);
	printf("q 暂停");
	GotoXY(40, 22);
	printf("当蛇撞到自身或撞墙时游戏结束");
	GotoXY(45, 24);
	printf("按任意键返回上级菜单");
	Hide();		//隐藏光标
	char ch = _getch();
	system("cls");
}

void Rank() {
	GotoXY(40, 12);
	printf("现在的最高得分是%d",MAX);
	Hide();		//隐藏光标
	char ch = _getch();
	system("cls");
}

/*初始化地图函数*/
void InitMap() {
	Hide();	   				//隐藏光标
	//设置蛇头位置在地图中心
	snake.snakeNode[0].x = MAP_WIDTH / 2 - 1;
	snake.snakeNode[0].y = MAP_HEIGHT / 2 - 1;
	GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y); 	  //将光标移动到蛇头位置
	printf("@"); 				//打印蛇头
	snake.length = 3;   		//设置蛇长初始值为3节
	snake.speed = 250;  		//设置蛇初始移动速度为250
	now_Dir = RIGHT;            //当前蛇头方向
	//显示蛇身
	for (int i = 1; i < snake.length; i++) {
		//设置蛇身的纵坐标位置和蛇头位置相同
		snake.snakeNode[i].y = snake.snakeNode[i - 1].y;
		//设置蛇身的横坐标位置，蛇身在蛇头的左边,所以横坐标依次减1
		snake.snakeNode[i].x = snake.snakeNode[i - 1].x - 1;
		GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y); 	//移动光标到蛇身位置
		printf("o");   		//打印蛇身
	}
	//生成地图上下边界
	for (int i = 0; i < MAP_WIDTH; i++) {
		GotoXY(i, 0);
		printf("-");
		GotoXY(i, MAP_HEIGHT - 1);
		printf("-");
	}
	//生成地图左右边界
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		GotoXY(0, i);
		printf("|");
		GotoXY(MAP_WIDTH - 1, i);
		printf("|");
	}
	//生成食物
	PrintFood();
	//得分说明
	GotoXY(50, 5);
	printf("当前得分:0");
}

void Printblock() {
	int flag=1;
	while (flag) {
		flag=0;
		if (block.x != 1000)
		{
			GotoXY(block.x,block.y);
			printf(" ");
		}
		block.x = rand() % (MAP_WIDTH - 2) + 1;
		block.y = rand() % (MAP_HEIGHT - 2) + 1;
		for (int k = 0; k <= snake.length - 1; k++) {
			if (snake.snakeNode[k].x == block.x && snake.snakeNode[k].y == block.y) {
				flag = 1;  				//位置有重叠，需要继续循环
				break;
			}
		}
	}
	GotoXY(block.x,block.y);
	printf("X");
}

/*生成食物函数*/
void PrintFood() {
	int flag = 1;
	while (flag)
	{
		flag = 0;
		//设置随机的食物坐标位置
		food.x = rand() % (MAP_WIDTH - 2) + 1;
		food.y = rand() % (MAP_HEIGHT - 2) + 1;
		//循环判断食物位置是否和蛇的位置重叠，如果重叠则需要重新设置食物位置
		for (int k = 0; k <= snake.length - 1; k++) {
			if (snake.snakeNode[k].x == food.x && snake.snakeNode[k].y == food.y) {
				flag = 1;  				//位置有重叠，需要继续循环
				break;
			}
		}
	}
	GotoXY(food.x, food.y);
	rannum = rand() % 3 + 1;
	if (rannum == 1)
	{
		printf("$");
	}
	else if (rannum == 2)
	{
		printf("#");

	}
	else
	{
		printf("&");
		rannum = 3;
	}
	foods = rannum;
}

int MoveSnake() {
	Snakenode temp;
	int flag = 0;
	temp = snake.snakeNode[snake.length - 1];  // 记录蛇尾

	for (int i = snake.length - 1; i >= 1; i--) {
		snake.snakeNode[i] = snake.snakeNode[i - 1];  // 蛇身前移
	}

	GotoXY(snake.snakeNode[1].x, snake.snakeNode[1].y);
	printf("o");  // 打印新的蛇身

	// 处理用户输入
	if (_kbhit()) {
		char key = _getch();
		if (key == 'p') {  // 按下 'q' 进行暂停
			paused = 1;
			GotoXY(50, 10);
			printf("游戏暂停，按任意键继续...");

			while (paused) {
				if (_kbhit()) {
					_getch();  // 读取输入并继续游戏
					paused = 0;
					GotoXY(50, 10);
					printf("                         "); // 清除“游戏暂停”提示
				}
			}
		}
		else {  // 处理方向输入
			direction = key;
			switch (direction) {
			case UP: if (now_Dir != DOWN) now_Dir = direction; break;
			case DOWN: if (now_Dir != UP) now_Dir = direction; break;
			case LEFT: if (now_Dir != RIGHT) now_Dir = direction; break;
			case RIGHT: if (now_Dir != LEFT) now_Dir = direction; break;
			}
		}
	}

	// 移动蛇头
	switch (now_Dir) {
	case UP: snake.snakeNode[0].y--; break;
	case DOWN: snake.snakeNode[0].y++; break;
	case LEFT: snake.snakeNode[0].x--; break;
	case RIGHT: snake.snakeNode[0].x++; break;
	}

	GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
	printf("@");  // 打印新的蛇头

	// 判断是否吃到食物
	if (snake.snakeNode[0].x == food.x && snake.snakeNode[0].y == food.y) {
		snake.length++;
		flag = 1;  // 标记吃到食物
		snake.snakeNode[snake.length - 1] = temp;
		if (foods == 1)
			score++;
		if (foods == 2)
			score+=2;
		if (foods == 3)
			score+=3;
	}
	

	// 清除旧的蛇尾
	if (!flag) {
		GotoXY(temp.x, temp.y);
		printf(" ");
	}
	else {
		Printblock();
		PrintFood();  // 生成新的食物
		GotoXY(50, 5);
		printf("当前得分:%d",score);
		if(score>MAX)
			MAX=score;
	}
	//截断蛇身
	if (clashItself()) {
		for (int i = snake.length - 1; i > c_index; i--) {
			GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
			printf(" ");
			snake.length=c_index;
		}
	}
	// 判断是否游戏结束（撞墙）
	if (!IsCorrect()) {
		system("cls");
		GotoXY(45, 14);
		printf("最终得分：%d", score);
		GotoXY(45, 16);
		printf("你输了！");
		GotoXY(45, 18);
		printf("按任意键返回主菜单");
		_getch();
		system("cls");
		return 0;
	}

	SpeedControl();
	Sleep(snake.speed);  // 控制速度

	return 1;
}


/*判断是否自撞或撞墙,返回值为0表示自撞或撞墙，否则为1*/
int IsCorrect() {
	if (snake.snakeNode[0].x == 0 || snake.snakeNode[0].y == 0 || snake.snakeNode[0].x == MAP_WIDTH - 1 || snake.snakeNode[0].y == MAP_HEIGHT - 1|| snake.snakeNode[0].x == block.x && snake.snakeNode[0].y == block.y) //判断蛇头是否撞墙
		return 0;
	return 1;
}
int clashItself() {
	for (int i = 1; i < snake.length; i++) {   //判断蛇头是否和蛇身重叠，重叠表示自撞
		if (snake.snakeNode[0].x == snake.snakeNode[i].x && snake.snakeNode[0].y == snake.snakeNode[i].y) {
			c_index = i;
			return 1;
		}
	}
	return 0;
}

/*速度调整函数*/
void SpeedControl() {
	switch (snake.length) {  		//根据蛇长调整蛇的移动速度
	case 6:snake.speed = 200; break;
	case 9:snake.speed = 180; break;
	case 12:snake.speed = 160; break;
	case 15:snake.speed = 140; break;
	case 18:snake.speed = 120; break;
	case 21:snake.speed = 100; break;
	case 24:snake.speed = 80; break;
	case 27:snake.speed = 60; break;
	case 30:snake.speed = 40; break;
	default:break;
	}
}
