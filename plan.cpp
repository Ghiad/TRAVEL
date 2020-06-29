#include"pch.h"
#include"plan.h"
#include"controller.h"
using namespace std;

void strategy(Passenger &pass, string start, vector<string> &passcity, list<Path> &tmp_route,  double &mindanger, double &sumdanger, vector<double> &increment, int arrivetime, int limit, int choice) {//最少风险策略
	auto begin_passcity = passcity.begin(), end_passcity = passcity.end();//passcity的迭代器
	Path tmp_path;//暂时存储路线
	if (sumdanger > mindanger) {//如果风险已经超出最小的，舍去
		tmp_route.pop_back();//把最新加的路径点删除
		tmp_route.pop_back();
		sumdanger -= increment.back();//把删除路径的的风险删去
		increment.pop_back();
		return;
	}
	if (choice && arrivetime > limit) {//如果超出限定时间,舍去
		tmp_route.pop_back();//把最新加的路径点删除
		tmp_route.pop_back();
		sumdanger -= increment.back();
		increment.pop_back();//把删除路径的的风险删去
		return;
	}
	while (begin_passcity != end_passcity) {
		if (start == (*begin_passcity)) {//环路出现,舍去,不应该在这舍去
			tmp_route.pop_back();
			tmp_route.pop_back();
			sumdanger -= increment.back();
			increment.pop_back();
			return;
		}
		begin_passcity++;
	}
	if (start == pass.destination) {//到达终点，且风险比旧的最小风险低
		tmp_path.location = start;
		tmp_path.s_time = -1;//已经到达终点change_state函数不再改变状态
		tmp_route.push_back(tmp_path);
		if(sumdanger==mindanger)//如果新的路径风险于旧的相同，就添加
			pass.plan.push_back(tmp_route) ;
		else if (pass.plan.begin() == pass.plan.end())//否则就是比旧的风险小，就不添加而是置换,如果还没有路径就新增一个
			pass.plan.push_back(tmp_route);
		else
			pass.plan[0] = tmp_route;
		mindanger = sumdanger;
		tmp_route.pop_back();//因为加了终点所以要多删一个点
		tmp_route.pop_back();
		tmp_route.pop_back();
		sumdanger -= increment.back();
		increment.pop_back();
		return;
	}
	else {
		passcity.push_back(start);//把当前城市加入到已经走过的城市中,防止环路
		auto begin_time = timetable.begin(), end_time = timetable.end();
		vector<Timetable> enabletime;//从该城市出发的班次
		while (begin_time != end_time) {//寻找所以出发点在目前地方且出发时机在到达时间之后的班次
			if ((*begin_time).start == start) {
				enabletime.push_back((*begin_time));
			}
			begin_time++;
		}

		auto begin_city = city.begin(), end_city = city.end();//寻找出发城市下标
		int i = 0;
		for (i = 0; begin_city != end_city; begin_city++, i++) {
			if ((*begin_city).city_name == start) break;
		}

		auto begin_enable = enabletime.begin(), end_enable = enabletime.end();//从所在点的班次开始
		string now_destion;
		while (begin_enable != end_enable) {
			now_destion = (*begin_enable).destination;//下一站点的出发点等于现在站点的目的地

			//交通工具的风险*乘坐时间*出发地的风险+在出发地等待的时间*出发地的风险,在出发地和目的地的时间不算

			if (city[i].city_name == pass.start) {//起始点在出发地，不算在出发地的等待时间
				sumdanger += (*begin_enable).danger*(*begin_enable).t_time*city[i].danger;
				increment.push_back((*begin_enable).danger*(*begin_enable).t_time*city[i].danger);
			}
			else if (arrivetime <= (*begin_enable).s_time) {
				sumdanger += (*begin_enable).danger*(*begin_enable).t_time*city[i].danger + ((*begin_enable).s_time - arrivetime)*city[i].danger;
				increment.push_back((*begin_enable).danger*(*begin_enable).t_time*city[i].danger + ((*begin_enable).s_time - arrivetime)*city[i].danger);
			}
			else if (arrivetime > (*begin_enable).s_time) {//出发时间在下一天
				sumdanger += (*begin_enable).danger*(*begin_enable).t_time*city[i].danger + (((*begin_enable).s_time + 24) - arrivetime)*city[i].danger;
				increment.push_back((*begin_enable).danger*(*begin_enable).t_time*city[i].danger + (((*begin_enable).s_time + 24) - arrivetime)*city[i].danger);
			}
			tmp_path.location = (*begin_enable).start;//路径点的输入,当前城市
			tmp_path.s_time = ((*begin_enable).s_time >= arrivetime) ? ((*begin_enable).s_time - arrivetime) : (((*begin_enable).s_time + 24) - arrivetime);//在该城市的等待时间
			tmp_path.kind = "城市";
			tmp_route.push_back(tmp_path);//把路径点加入到暂时路径中

			tmp_path.location = (*begin_enable).v_name;//即将乘坐的班次
			tmp_path.s_time = (*begin_enable).t_time;//在路上的时间
			tmp_path.kind = (*begin_enable).kind;//交通工具的种类
			tmp_route.push_back(tmp_path);
			
			strategy(pass, now_destion, passcity, tmp_route, mindanger, sumdanger, increment, (*begin_enable).d_time, limit, choice);//递归算法

			begin_enable++;
		}
		if (tmp_route.begin() != tmp_route.end()) {//路线不为空，该条线路不可用，要删去路径点
			tmp_route.pop_back();
			tmp_route.pop_back();
			if (passcity.begin() != passcity.end()) passcity.pop_back();
			sumdanger -= increment.back();
			increment.pop_back();
		}
		enabletime.clear();
	}
}
void makepath() {
	auto begin_pass = passenger.begin(), end_pass = passenger.end();
	while (begin_pass != end_pass) {//变量设在里面，这样生命周期等于规划完一个乘客的路线的时间
		list<Path> tmp_route;
		double mindanger = MAX_DANGER;//最小的风险
		double sumdanger = 0;//计算的风险
		vector<double> increment;//每次增加的风险
		vector<string> passcity;//走过的城市防止环路
		strategy((*begin_pass), (*begin_pass).start, passcity, tmp_route,  mindanger, sumdanger, increment, 0, (*begin_pass).limit, (*begin_pass).choice);
		(*begin_pass).changestate();
		begin_pass++;
	}
}
