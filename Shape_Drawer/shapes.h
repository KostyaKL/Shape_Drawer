#ifndef SHAEPS_H
#define SHAPES_H
#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

void triangle_Vdots(CPoint start, CPoint end, CPoint &top, CPoint &left, CPoint &right);

class MyShape {
public:
	MyShape() { }
	virtual void drawMe(CDC *) = 0;
	virtual ~MyShape() { }
private:
	DWORD color;
};

class MyLine : public MyShape {
public:
	MyLine(int x1, int y1, int x2, int y2, DWORD color) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}

	void drawMe(CDC *dc) {
		dc->MoveTo(x1, y1);
		dc->LineTo(x2, y2);
	}
private:
	int x1, x2, y1, y2;
};

class MyEllipse : public MyShape {
public:
	MyEllipse(int x1, int y1, int x2, int y2, DWORD color) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}

	void drawMe(CDC *dc) {
		dc->Ellipse(x1,y1,x2,y2);
	}
private:
	int x1, x2, y1, y2;
};

class MyTriangle : public MyShape {
public:
	MyTriangle(int x1, int y1, int x2, int y2, DWORD color) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}

	void drawMe(CDC *dc) {
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
	MyRectangle(int x1, int y1, int x2, int y2, DWORD color) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}

	void drawMe(CDC *dc) {
		dc->Rectangle(x1, y1, x2, y2);
	}
private:
	int x1, x2, y1, y2;
};

class MyPolygon : public MyShape {
public:
	MyPolygon(vector<CPoint> dotsA, DWORD color) {
		dots = dotsA;
		size = dots.size();
	}
	~MyPolygon() {
		dots.clear();
	}

	void drawMe(CDC *dc) {
		dc->MoveTo(dots[0]);
		for (int i = 1;i < size;i++) {
			dc->LineTo(dots[i]);
		}
		dc->LineTo(dots[0]);
	}
private:
	vector<CPoint> dots;
	int size;
};


#endif
