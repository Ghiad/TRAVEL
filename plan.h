#ifndef PLAN_H
#define PLAN_H

#include"pch.h"
#include"controller.h"

extern void makepath();
extern void strategy(Passenger &, string, vector<string> &, list<Path> &,  double &, double &, vector<double> &, int, int, int);
#endif PLAN_H
