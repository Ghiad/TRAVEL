#include"pch.h"
#include"controller.h"
#include"plan.h"
using namespace std;

int hour = 0, day = 1;//全局时间
string input;//输入

my_vector<City> city;//城市动态数组
my_vector<Passenger> passenger;//乘客动态数组
my_vector<Timetable> timetable;//航班表动态数组


City::City(string t_city_name, double t_danger) {//City构造函数
	city_name = t_city_name;
	danger = t_danger;
}
Timetable::Timetable(string t_v_name, string t_kind, string t_start, string t_destination, int t_s_time, int t_d_time, int t_t_time, int t_danger) {//Timetable构造函数
	v_name = t_v_name;
	kind = t_kind;
	start = t_start;
	destination = t_destination;
	s_time = t_s_time;
	d_time = t_d_time;
	t_time = t_t_time;
	danger = t_danger;
}
Passenger::Passenger(string t_name, string t_start, string t_destination, int t_day, int t_hour, int t_choice, int t_limit) {//Passenger构造函数
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
		time_data >> kind >> v_name >> start >> s_time >> destination >> d_time >> t_time >> danger;
		if (!time_data) break;
		Timetable tmp_time(v_name, kind, start, destination, s_time, d_time, t_time, danger);
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
		pas_data >> name >> start >> destination >> goday >> gohour >> choice >> limit;
		if (!pas_data) break;//读到文件末尾跳出循环
		Passenger tmp_pas(name, start, destination, goday, gohour, choice, limit);
		passenger.push_back(tmp_pas);
	}
	pas_data.close();
}
void readdata() {
	readCity();
	readTimetable();
	readPassenger();
}


void Passenger::changestate() {//当到出发日子后每过一小时改变一下乘客状态
	if (go_day < day) {//乘客出发时间的第二天开始
		if (flag == 1) {
			if (plan.front().front().s_time != -1) {
				//没到达终点，就改变状态，到达后s_time为-1不改变状态,第一个front是从可能的多条线路里选第一个，第二个是一条线路里的第一个路径点
				plan.front().front().s_time--;
				if (plan.front().front().s_time == 0) {
					plan.front().pop_front();//到达后删除第一个路径点
					diary.write(plan.front().front().kind,name, plan.front().front().location, plan.front().front().s_time);//往日志文件里写入旅客的状态变化
				}
			}
		}
	}

}
void Passenger::printstate() {
	if (flag == 1) {
		auto next_plan = (*plan.begin()).begin();
		next_plan = next_plan->next;//第一条路径里的第二个路径点
		if (go_day >= day) {
			cout << "旅客 " << name << "\t还没到达出发时间，出发时间是第" << go_day + 1 << "天" << endl;
		}
		else if (plan.front().front().kind == "城市")//在城市
			cout << "旅客 " << name << "\t现在在" << plan.front().front().location << "\t\t还要等待" << plan.front().front().s_time << "小时乘坐\t" << (*next_plan).object.location << endl;
		else if (next_plan == plan.front().end())//到达终点
			cout << "旅客 " << name << "\t现在到达终点\t\t" << plan.front().front().location << endl;
		else//在车上
			cout << "旅客 " << name << "\t现在在乘坐" << plan.front().front().kind << plan.front().front().location << "\t还要" << plan.front().front().s_time << "小时到达\t\t" << (*next_plan).object.location << endl;
	}
	else
		cout << "旅客 " << name << "\t没有符合要求的旅行路线" << endl;
}
void Passenger::printstate_inquire() {
	if (flag == 1) {
		auto next_plan = (*plan.begin()).begin();
		next_plan = next_plan->next;//第一条路径里的第二个路径点
		if (go_day >= day) {
			cout << "旅客 " << name << "\t还没到达出发时间，出发时间是第" << go_day + 1 << "天" << endl;
		}
		else if (plan.front().front().kind == "城市")//在城市
			cout << "旅客 " << name << "\t现在在" << plan.front().front().location << "\t\t还要等待" << plan.front().front().s_time+1 << "小时乘坐\t" << (*next_plan).object.location << endl;
		else if (next_plan == plan.front().end())//到达终点
			cout << "旅客 " << name << "\t现在到达终点\t\t" << plan.front().front().location << endl;
		else//在车上
			cout << "旅客 " << name << "\t现在在乘坐" << plan.front().front().kind << plan.front().front().location << "\t还要" << plan.front().front().s_time+1 << "小时到达\t\t" << (*next_plan).object.location << endl;
	}
	else
		cout << "旅客 " << name << "\t没有符合要求的旅行路线" << endl;
}
void Passenger::printroute() {//输出乘客的每条路径
	auto begin_itr = plan.begin(), end_itr = plan.end();//这一部分是先计算好到达某点的天数和小时
	while (begin_itr != end_itr) {
		auto route_beg = (*begin_itr).begin(), route_end = (*begin_itr).end();
		(*route_beg).object.arriveday = go_day + 1;//默认出发时间是旅客要求的第二天0点
		(*route_beg).object.arrivetime = 0;
		while (route_beg != route_end) {
			auto tmp = route_beg;
			route_beg=route_beg->next;//这个时候一定是乘坐车辆,因为路径点是1个城市1个交通工具交替，直到最后一个城市后才没有交通工具的点
			if (route_beg != route_end) {
				int usetime = (*route_beg).object.s_time;//在车上花的时间
				route_beg=route_beg->next;//这个时候在城市里
				//到达下一个城市的天数=到达现在这个城市的天数+（到达现在这个城市的小时+等车的时间+车上花的时间）/24
				(*route_beg).object.arriveday = (*tmp).object.arriveday + ((*tmp).object.arrivetime  + (*tmp).object.s_time + usetime) / 24;
				(*route_beg).object.arrivetime = ((*tmp).object.arrivetime + usetime + (*tmp).object.s_time) % 24;
			}
		}
		begin_itr++;
	}
	//这是输出路线的模块
	begin_itr = plan.begin();
	while (begin_itr != end_itr) {
		auto route_beg = (*begin_itr).begin(), route_end = (*begin_itr).end();
		cout << "旅客 " << name << endl << "第" << go_day + 1 << "天" << (*route_beg).object.s_time << "点" << "从" << (*route_beg).object.location << "出发" << endl;
		while (route_beg != route_end) {
			route_beg=route_beg->next;
			if (route_beg != route_end) {//不加判断会访问空指针
				auto tmp = route_beg;
				tmp=tmp->next;
				if ((*route_beg).object.kind == "城市"&&tmp != route_end)
					cout << "第" << (*route_beg).object.arriveday << "天" << (*route_beg).object.arrivetime << "点到达" << (*route_beg).object.location << "再过" << (*route_beg).object.s_time << "小时出发" << endl;
				else if (tmp == route_end)
					cout << "第" << (*route_beg).object.arriveday << "天" << (*route_beg).object.arrivetime << "点到达终点" << (*route_beg).object.location << endl << endl;
			}
		}
		begin_itr++;
	}
	if (flag == 0)
		cout << "旅客 " << name << "\t没有符合要求的旅行路线" << endl << endl;
}

void counter() {//计时器
	clock_t now = 0, old = 0;
	while (true) {
		now = clock();
		if (((now - old) / CLOCKS_PER_SEC) >= M_HOUR) {//每过M_HOUR秒，系统推进1个小时
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
				if (input == CONTINUE) {//直到输入继续命令
					cout << endl;
					input.clear();//清空input，为下次输入命令做准备
					break;
				}
			}
			now = clock();
			old = now;
		}

	}
}

void start() {
	int tmp_hour = hour, tmp_day = day;
	auto begin_pass = passenger.begin(), end_pass = passenger.end();
	while (true) {
		if (tmp_hour != hour) {//每过1小时,输出所有乘客的状态
			inquireall();
			while (begin_pass != end_pass) {
				(*begin_pass).changestate();//改变乘客的状态
				begin_pass++;
			}
			tmp_hour = hour;
		}
		begin_pass = passenger.begin();//恢复迭代器到容器首位
		end_pass = passenger.end();
		int i = 0;
		while (begin_pass != end_pass) {
			if ((*begin_pass).flag == 1) {//有路线线路
				if ((*begin_pass).plan.front().front().s_time == -1) i++;//有一个到达终点就+1
				if (i == passenger.size()) {//所有的都到达终点
					hour++;
					if (hour % 24 == 0) {
						day++;
						hour = 0;
					}
					return;//模拟结束
				}
			}
			begin_pass++;
		}
		begin_pass = passenger.begin();//恢复迭代器到容器首位
		end_pass = passenger.end();
	}

}
