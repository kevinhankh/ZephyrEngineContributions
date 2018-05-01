#pragma once
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <math.h>
#include <stdio.h>
#include <string>
#include <time.h>
#include <cstddef>
#include <mutex>

#define PI 3.14159265358979323846

enum Mast
{
	NONE,
	HALFMAST,
	FULLMAST
};
enum Rudder
{
	STRAIGHT,
	HALFSTARBOARD,
	FULLSTARBOARD,
	HALFPORT,
	FULLPORT
};