#ifndef PLAN_H
#define PLAN_H

#include"pch.h"
#include"controller.h"
#include"my_vector.h"
extern void makepath();
extern void strategy(Passenger &, string, my_vector<string> &,list<Path> &,  double &, double &, my_vector<double> &, int, int, int);
#endif PLAN_H
