#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#pragma once

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class utilityFunctions { //utility functions class to be used as general methods for the program
public:
	static void triangle_Vdots(CPoint start, CPoint end, CPoint &top, CPoint &left, CPoint &right) { //calculate triangle vertexes locked in a rectangle
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


	static MyShape **shapeSrch(CPoint point, vector<MyShape*> &shapes, CDC *dc) { //determine if a point is inside one of the shapes
		int size;
		size = shapes.size();
		for (int i = size - 1;i >= 0;i--) {
			if (shapes[i]->isInside(point, dc)) {
				return &shapes[i];
			}
		}
		return NULL;
	}


	static void angleFix(double angle, CPoint startP, CPoint &point) { //set aspect ratio 1:1 when LSHIFT is pressed
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

	static double CPdist(CPoint a, CPoint b) { //claculate distance between two points
		int x, y;
		double c;
		x = a.x - b.x;
		y = a.y - b.y;
		c = x * x + y * y;
		c = sqrt(c);
		return c;
	}

	static double triangleArea(CPoint a, CPoint b, CPoint c) //calculate an area of a triangle
	{
		double area;
		area = ((a.x*(b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0);
		area = area < 0 ? area * -1 : area;
		return area;
	}

	static void savePrevState(vector<MyShape*> &shapes, vector<MyShape*> &lastAction) { //duplicate the data to a back up vector
		MyShape *temp;		
		int size;

		size = lastAction.size();
		for (int i = 0;i < size;i++) {
			delete lastAction[i];
		}
		lastAction.clear();

		size = shapes.size();
		for (int i = 0;i < size;i++) {
			temp = shapes[i]->clone();
			lastAction.push_back(temp);
		}
	}
};

#endif