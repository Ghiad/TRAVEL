#ifndef PLAN_H
#define PLAN_H

#include"pch.h"
#include"controller.h"

extern void makepath();
extern void strategy(Passenger &, string, vector<string> &, list<Path> &, list<Path> &, double &, double &, vector<double> &, int,int,vector<int> &);
extern void limit_strategy(Passenger &, string, vector<string> &, list<Path> &, list<Path> &, double &, double &, vector<double> &, int, int);
#endif PLAN_H
