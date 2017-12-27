#ifndef SHAEPS_H
#define SHAPES_H
#pragma once

#include "stdafx.h"
#include <vector>

class MyShape {
public:
	MyShape() { }
	virtual void drawMe() = 0;
	virtual ~MyShape() { }
};

class MyLine : public MyShape {
public:
	MyLine(int x1, int y1, int x2, int y2) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}

	void drewMe(CDC *dc) {
		dc->MoveTo(x1, y1);
		dc->LineTo(x2, y2);
	}
private:
	int x1, x2, y1, y2;
};

class MyEllipse : public MyShape {
public:
	MyEllipse(int x1, int y1, int x2, int y2) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}

	void drewMe(CDC *dc) {
		dc->Ellipse(x1,y1,x2,y2);
	}
private:
	int x1, x2, y1, y2;
};

class MyTriangle : public MyShape {
public:
	MyTriangle(int x1, int y1, int x2, int y2) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}

	void drewMe(CDC *dc) {
		CPoint top, left, right;
		if (x1 < x2) {
			top.x = x1 + (x2 - x1) / 2;
			left.x = x1;
			right.x = x2;
		}
		else {
			top.x = x2 + (x1 - x2) / 2;
			left.x = x2;
			right.x = x1;
		}

		if (y1 < y2) {
			top.y = y1;
			left.y = y2;
			right.y = y2;
		}
		else {
			top.y = y2;
			left.y = y1;
			right.y = y1;
		}

		dc->MoveTo(top);
		dc->LineTo(left);
		dc->LineTo(right);
		dc->LineTo(top);
	}
private:
	int x1, x2, y1, y2;
};

class MyRectangle : public MyShape {
public:
	MyRectangle(int x1, int y1, int x2, int y2) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}

	void drewMe(CDC *dc) {
		dc->Rectangle(x1, y1, x2, y2);
	}
private:
	int x1, x2, y1, y2;
};

class MyPolygon : public MyShape {
public:
	MyPolygon(int size, CPoint *dotsA) {
		for (int i = 0;i < size;i++)
			dots[i] = dotsA[i];
		this->size = size;
	}

	void drewMe(CDC *dc) {
		dc->MoveTo(dots[0]);
		for (int i = 1;i < size;i++) {
			dc->LineTo(dots[i]);
		}
		dc->LineTo(dots[0]);
	}
private:
	CPoint dots[50];
	int size;
};



void triangle_Vdots(CPoint start, CPoint end, CPoint &top, CPoint &left, CPoint &right) {
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


#endif
