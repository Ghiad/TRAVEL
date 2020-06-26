#include"pch.h"
#include"controller.h"
#include"plan.h"
using namespace std;

int hour = 0, day = 0;//全局时间
int stop=0;
string input;//输入
Diary diary;
vector<City> city;//城市动态数组
vector<Timetable> timetable;
vector<Passenger> passenger;

City::City(string t_city_name, double t_danger) {//City构造函数
	city_name = t_city_name;
	danger = t_danger;
}
Timetable::Timetable(string t_v_name, string t_kind, string t_start, string t_destination, int t_s_time, int t_d_time, int t_t_time,int t_danger) {//Timetable构造函数
	v_name = t_v_name;
	kind = t_kind;
	start = t_start;
	destination = t_destination;
	s_time = t_s_time;
	d_time = t_d_time;
	t_time = t_t_time;
	danger = t_danger;
}
Passenger::Passenger(string t_name, string t_start, string t_destination,int t_day,int t_hour,int t_choice,int t_limit) {//Passenger构造函数
	name = t_name;
	start = t_start;
	destination = t_destination;
	go_day = t_day;
	go_hour = t_hour;
	choice = t_choice;
	limit = t_limit;
}
void readCity() {
	ifstream city_data("CITY.txt", ifstream::in);
	if (!city_data.is_open()) {
		cout << "CITY.txt isn't open" << endl;
		return;
	}
	while (!city_data.eof()) {
		string city_name;
		double city_danger;
		city_data >> city_name;
		city_data >> city_danger;
		if (!city_data) break;//读到文件末尾跳出循环
		City tmp_city(city_name, city_danger);
		city.push_back(tmp_city);
	}
	city_data.close();
}
void readTimetable() {
	ifstream time_data("TIMETABLE.txt", ifstream::in);
	if (!time_data.is_open()) {
		cout << "TIMETABLE.txt isn't open" << endl;
		return;
	}
	while (!time_data.eof()) {
		string v_name;//班次
		string kind;//交通工具种类
		string start;//起点
		string destination;//终点
		int s_time;//出发时间
		int d_time;//到达时间
		int t_time;//总共时间
		int danger;//风险等级
		time_data >> kind >> v_name >> start >> s_time >> destination >> d_time >>t_time >> danger;
		if (!time_data) break;
		Timetable tmp_time(v_name, kind, start, destination, s_time, d_time,t_time, danger);
		timetable.push_back(tmp_time);
	}
	time_data.close();
}
void readPassenger() {
	ifstream pas_data("PASSENGER.txt", ifstream::in);
	if (!pas_data.is_open()) {
		cout << "PASSENGER.txt isn't open" << endl;
		return;
	}
	while (!pas_data.eof()) {
		string name;
		string start;
		string destination;
		int goday;
		int gohour;
		int choice;
		int limit;
		pas_data >> name>>start>>destination>>goday>>gohour>>choice>>limit;
		if (!pas_data) break;//读到文件末尾跳出循环
		Passenger tmp_pas(name, start, destination,goday,gohour,choice,limit);
		passenger.push_back(tmp_pas);
	}
	pas_data.close();
}
void readdata() {
	readCity();
	readTimetable();
	readPassenger();
}

void Diary::write() {
	ofstream dia("DIARY.txt", ofstream::out | ofstream::app);
	dia << "第" << day << "天" << hour << "时 " << "输入查询命令" << endl<<endl;
}
void Diary::write(string name,string location,int w_time) {
	ofstream dia("DIARY.txt", ofstream::out|ofstream::app);
	//乘客状态变化
	if (w_time == -1)
		dia << "第" << day << "天" << hour << "时 " << "旅客" << name << "到达终点" << location << endl << endl;
	else
		dia << "第" << day << "天" << hour << "时 " << "旅客" << name << "到达" << location << "还要" << w_time << "小时到达下一地点" << endl<<endl;
}
void Passenger::changestate() {//每过一小时改变一下乘客状态
	if (go_day < day) {//出发时间的第二天开始
		if (plan.front().s_time != -1) {//没到达终点，就改变状态，到达后不改变状态
			plan.front().s_time--;
			if (plan.front().s_time == 0) {
				plan.pop_front();
				diary.write(name, plan.front().location, plan.front().s_time);
			}
		}
	}
	
}
void Passenger::printstate() {
	auto next_plan = plan.begin();
	next_plan++;
	if (go_day >= day) {
		cout << "旅客 " << name << "\t还没到达出发时间，出发时间是第" << go_day + 1 << "天" << endl;
	}
	else if (plan.front().kind == "城市")//在城市
		cout << "旅客 " << name << "\t现在在" << plan.front().location << "\t\t还要等待" << plan.front().s_time << "小时乘坐\t" << (*next_plan).location << endl;
	else if (next_plan == plan.end())//到达终点
		cout << "旅客 " << name << "\t现在到达终点\t\t" << plan.front().location << endl;
	else//在车上
		cout << "旅客 " << name << "\t现在在乘坐" << plan.front().kind<< plan.front().location << "\t还要" << plan.front().s_time << "小时到达\t\t" <<(*next_plan).location  << endl;
}

void inquire() {//查询操作
	auto begin_pass = passenger.begin(), end_pass = passenger.end();
	cout << "现在是第" << day << "天" << hour<<"点"<< endl;
	while (begin_pass != end_pass) {
		(*begin_pass).printstate() ;
		//diary.write();
		begin_pass++;
	}
	cout << endl;
}

void counter() {//计时器
	clock_t now=0, old = 0,stop=0;
	while (true) {
		now = clock();
		if (((now - old) / CLOCKS_PER_SEC) >= 1) {
			if (hour == 23) {
				hour = 0;
				day++;
			}
			else
				hour++;
			old = now;
		}
		if (!input.empty()) {//有输入时,暂停计时器
			while (true) {
				if (input == CONTINUE) {
					cout <<endl;
					input.clear();//清空input，为下次输入命令做准备
					break;
				}
			}
			now = clock();
			old = now;
		}
		
	}
}
void block() {
	while (true) {
		if (_kbhit()) {//有键盘输入
			int flag = 0;
			stop = 1;
			input.push_back(_getch());
			cout << input.back();
			while (true) {
				if (_kbhit()) {
					input.push_back(_getch());
					if (input.back() == 13)	break;//遇到回车终止循环
					else cout << input.back();

				}
			}
			cout << endl;
			input.pop_back();//删除“/r”
			if (input == REQUEST) {//如果输入等于request
				inquire();
				input.clear();
			}
			else {//如果是无意义符号，恢复计时
				input = "continue\r";
				flag = 1;
			}
			if (flag == 0) {
				while (true) {//没有遇到回车
					if (_kbhit()) {
						input.push_back(_getch());
						if (input.back() == 13)	break;
						else cout << input.back();

					}
				}
			}
			
		}
		stop = 0;
		
	}
	
	
}
void start() {
	int tmp_hour=hour, tmp_day=day;
	auto begin_pass = passenger.begin(), end_pass = passenger.end();
	while (true) {
		if (tmp_hour != hour) {//每过1小时
			while (begin_pass != end_pass) {
				(*begin_pass).changestate();
				begin_pass++;
			}
			tmp_hour = hour;
			inquire();
		}
		begin_pass = passenger.begin();//恢复迭代器到容器首位
		end_pass = passenger.end();
		int i = 0;
		while (begin_pass != end_pass) {
			if ((*begin_pass).plan.front().s_time == -1) i++;
			if (i == passenger.size()) return;//所有乘客到达终点
			begin_pass++;
		}
		begin_pass = passenger.begin();//恢复迭代器到容器首位
		end_pass = passenger.end();
	}

}
