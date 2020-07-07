#ifndef CONTROLLER_H
#define CONTROLLER_H
#include"pch.h"
#include"my_vector.h"
#include"my_list.h"
#include"diary.h"
#include"inquire.h"
#include <iostream>
#include<time.h>
#include<string>
#include<fstream>
#include<stdio.h>
#include <conio.h>
#include<Windows.h>

#define MAX_DANGER 49999//刚开始设一个足够大的风险值，方便计算
#define M_HOUR 1//现实多少秒等于模拟系统的1个小时
using namespace std;

extern int day,hour;//全局时间
extern string input;//全局的输入变量

typedef struct path {//路径点的结构
	string kind;//目前地是城市还是交通工具
	string location;//目前地
	int arrivetime;//到达该点是几点
	int arriveday;//到达该点是第几天
	int s_time;//在该点要花费多少时间进入下一点
}Path;

class City {
public:
	City() = default;//默认构造函数
	City(string, double);//构造函数
	string city_name;//城市名
	double danger;//城市风险等级	
};
class Timetable {
public:
	Timetable() = default;//默认构造函数
	Timetable(string, string, string, string, int, int, int,int);//构造函数
	string v_name;//班次
	string kind;//交通工具种类
	string start;//起点
	string destination;//终点
	int s_time;//交通工具出发时间
	int d_time;//交通工具到达时间
	int t_time;//总共时间
	int danger;//交通工具风险等级
};

class Passenger {
public:
	Passenger() = default;//默认构造函数
	Passenger(string, string, string,int,int,int,int);//构造函数
	void changestate();//改变当前状态
	void printstate();//输出当前状态
	void printstate_inquire();//动态查询的时候调用该函数
	void printroute();//输出每条路径
	string name;//乘客名字
	string start;//乘客最开始的地方
	string destination;//乘客目的地
	int go_day;//乘客在几日出发
	int go_hour;//乘客在几时出发
	int choice;//选择的风险策略，1是限时最少风险策略，0是最低风险策略
	int limit;//限制的时间，如果choice是0的话也为0.
	my_vector<my_list<Path>> plan;//乘客的旅行计划表,存放旅行线路
	int flag;//判断是否有路线，1有，0无
};

extern my_vector<City> city;//城市动态数组
extern my_vector<Timetable> timetable;//航班表动态数组
extern my_vector<Passenger> passenger;//乘客动态数组

extern void start();//模拟开始函数
extern void counter();//计时器函数
extern void readdata();//从文件中读数据
extern void readCity();//从文件中读取城市数据
extern void readPassenger();//从文件中读取乘客数据
extern void readTimetable();//从文件中读取航班表数据



#endif
