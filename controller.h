#ifndef CONTROLLER_H
#define CONTROLLER_H
#include"pch.h"
#include <iostream>
#include<thread>
#include<time.h>
#include<string>
#include<vector>
#include<list>
#include<deque>
#include<fstream>
#include<stdio.h>
#include <conio.h>
#include<Windows.h>
#define REQUEST "request"
#define CONTINUE "continue\r"
#define MAX_DANGER 49999
using namespace std;
extern int day,hour;//全局时间
extern string input;
extern int stop;
typedef struct path {
	string kind;
	string location;//目前地
	int s_time;//花费时间
}Path;

class City {
public:
	City(string, double);
	string city_name;//城市名
	double danger;//城市风险等级	
};
class Timetable {
public:
	Timetable(string, string, string, string, int, int, int,int);
	string v_name;//班次
	string kind;//交通工具种类
	string start;//起点
	string destination;//终点
	int s_time;//出发时间
	int d_time;//到达时间
	int t_time;//总共时间
	int danger;//风险等级
};

class Passenger {
public:
	Passenger(string, string, string,int,int,int,int);
	void changestate();//改变当前状态
	void printstate();//输出当前状态
	string name;//乘客名字
	string start;//乘客最开始的地方
	string destination;//乘客目的地
	int go_day;//乘客出发日
	int go_hour;//乘客出发小时
	int choice;//选择的风险策略
	int limit;//限制的时间
	list<Path> plan;//乘客的旅行计划表
};
class Diary {
public:
	void write();
	void write(string,string,int);
private:
	vector<string> change;//输入的命令和乘客状态的变化
};
extern vector<City> city;//城市动态数组
extern vector<Timetable> timetable;
extern vector<Passenger> passenger;
extern Diary diary;//全局日志文件
extern void start();//模拟开始
extern void counter();//计时器函数
extern void readdata();//从文件中读数据
extern void readCity();
extern void readPassenger();
extern void readTimetable();
extern void inquire();//查询函数
extern void block();//遇到输入阻塞程序

#endif
