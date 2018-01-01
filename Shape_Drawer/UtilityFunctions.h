#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#pragma once

#include "stdafx.h"

class utilityFunctions {
public:
	static void triangle_Vdots(CPoint start, CPoint end, CPoint &top, CPoint &left, CPoint &right) {
		if (start.x < end.x) {
			top.x = start.x + (end.x - start.x) / 2;
			left.x = start.x;
			right.x = end.x;
		}
		else {
			top.x = end.x + (start.x - end.x) / 2;
			left.x = end.x;
			right.x = start.x;
		}

		top.y = end.y;
		left.y = start.y;
		right.y = start.y;
	}


	static MyShape *shapeSrch(CPoint point, vector<MyShape*> shapes, CDC *dc) {
		MyShape *ret;
		int size;
		ret = NULL;
		size = shapes.size();
		for (int i = size - 1;i >= 0;i--) {
			ret = shapes[i]->isInside(point, dc);
			if (ret != NULL) {
				return ret;
			}
		}
		return ret;
	}


	static void angleFix(double angle, CPoint startP, CPoint &point) {
		if (angle >= 337.5 && angle < 22.5) { //up
			point.x = startP.x;
		}
		else if (angle >= 22.5 && angle < 67.5) { //up right - x[pos] y[neg]
			if (point.x - startP.x < startP.y - point.y) {
				point.y = startP.y + point.x - startP.x;
			}
			else {
				point.x = startP.x + point.y - startP.y;
			}
		}
		else if (angle >= 67.5 && angle < 112.5) { //right
			point.y = startP.y;
		}
		else if (angle >= 112.5 && angle < 157.5) { //right down - x[pos] y[pos]
			if (point.x - startP.x < point.y - startP.y) {
				point.y = startP.y + point.x - startP.x;
			}
			else {
				point.x = startP.x + point.y - startP.y;
			}
		}
		else if (angle >= 157.5 && angle < 202.5) { //down
			point.x = startP.x;
		}
		else if (angle >= 202.5 && angle < 247.5) { //down left - x[neg] y[pos]
			if (startP.x - point.x < point.y - startP.y) {
				point.y = startP.y + point.x - startP.x;
			}
			else {
				point.x = startP.x + point.y - startP.y;
			}
		}
		else if (angle >= 247.5 && angle < 292.5) { //left
			point.y = startP.y;
		}
		else if (angle >= 292.5 && angle < 337.5) { //left up - x[neg] y[neg]
			if (startP.x - point.x < startP.y - point.x) {
				point.y = startP.y + point.x - startP.x;
			}
			else {
				point.x = startP.x + point.y - startP.y;
			}
		}
	}

	static double CPdist(CPoint a, CPoint b) {
		int x, y;
		double c;
		x = a.x - b.x;
		y = a.y - b.y;
		c = x * x + y * y;
		c = sqrt(c);
		return c;
	}

	static double triangleArea(CPoint a, CPoint b, CPoint c)
	{
		double area;
		area = ((a.x*(b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0);
		area = area < 0 ? area * -1 : area;
		return area;
	}
};

#endif