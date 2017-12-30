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
	MyShape() {
		dots.clear();
		color = RGB(0, 0, 0);
	}
	virtual void drawMe(CDC *) const = 0;
	virtual ~MyShape() {
		dots.clear();
	}

	virtual DWORD getColor() const{
		return color;
	}
protected:
	vector<CPoint> dots;
	COLORREF color;
};

class MyLine : public MyShape {
public:
	MyLine(CPoint startP, CPoint endP, COLORREF color) {
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
	}

	void drawMe(CDC *dc) const {
		COLORREF oldPen;
		dc->SelectObject(GetStockObject(DC_PEN));
		oldPen = dc->SetDCPenColor(color);

		dc->MoveTo(dots[0]);
		dc->LineTo(dots[1]);

		dc->SetDCPenColor(oldPen);
	}
};

class MyEllipse : public MyShape {
public:
	MyEllipse(CPoint startP, CPoint endP, COLORREF color) {
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
	}

	void drawMe(CDC *dc) const {
		COLORREF oldPen, oldBrush;

		dc->SelectObject(GetStockObject(DC_BRUSH));
		dc->SelectObject(GetStockObject(DC_PEN));

		oldBrush = dc->SetDCBrushColor(color);
		oldPen = dc->SetDCPenColor(color);

		dc->Ellipse(dots[0].x, dots[0].y, dots[1].x, dots[1].y);

		dc->SetDCBrushColor(oldBrush);
		dc->SetDCPenColor(oldPen);
	}
};

class MyTriangle : public MyShape {
public:
	MyTriangle(CPoint startP, CPoint endP, COLORREF color) {
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
	}

	void drawMe(CDC *dc) const {
		CPoint points[3]; //0 - top, 1 - left, 2 - right
		CPoint startP, endP;
		startP = dots[0];
		endP = dots[1];

		triangle_Vdots(startP, endP, points[0], points[1], points[2]);
		
		COLORREF oldPen, oldBrush;

		dc->SelectObject(GetStockObject(DC_BRUSH));
		dc->SelectObject(GetStockObject(DC_PEN));

		oldBrush = dc->SetDCBrushColor(color);
		oldPen = dc->SetDCPenColor(color);
		
		Polygon(*dc, points, 3);
		
		dc->SetDCBrushColor(oldBrush);
		dc->SetDCPenColor(oldPen);
	}
};

class MyRectangle : public MyShape {
public:
	MyRectangle(CPoint startP, CPoint endP, COLORREF color) {
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
	}

	void drawMe(CDC *dc) const {
		COLORREF oldPen, oldBrush;
		
		dc->SelectObject(GetStockObject(DC_BRUSH));
		dc->SelectObject(GetStockObject(DC_PEN));

		oldBrush = dc->SetDCBrushColor(color);
		oldPen = dc->SetDCPenColor(color);

		dc->Rectangle(dots[0].x, dots[0].y, dots[1].x, dots[1].y);

		dc->SetDCBrushColor(oldBrush);
		dc->SetDCPenColor(oldPen);
	}
};

class MyPolygon : public MyShape {
public:
	MyPolygon(vector<CPoint> dotsA, COLORREF color) {
		dots = dotsA;
		size = dots.size();
		this->color = color;
	}
	~MyPolygon() {
		dots.clear();
	}

	void drawMe(CDC *dc) const {
		CPoint *points;
		points = new CPoint[size];

		for (int i = 0;i < size;i++) {
			points[i] = dots[i];
		}

		COLORREF oldPen, oldBrush;

		dc->SelectObject(GetStockObject(DC_BRUSH));
		dc->SelectObject(GetStockObject(DC_PEN));

		oldBrush = dc->SetDCBrushColor(color);
		oldPen = dc->SetDCPenColor(color);

		Polygon(*dc, points, size);

		dc->SetDCBrushColor(oldBrush);
		dc->SetDCPenColor(oldPen);

		delete[] points;
	}
private:
	int size;
};


#endif
