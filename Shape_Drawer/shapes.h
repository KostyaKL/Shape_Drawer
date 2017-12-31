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
		isSelected = FALSE;
		index = -1;
	}
	
	virtual ~MyShape() {
		dots.clear();
	}

	virtual void drawMe(CDC *) const = 0;

	virtual MyShape *isInside(CPoint point, CDC *dc) = 0;

	virtual void offsetShape(CPoint offset) = 0;

	void unselectShape() {
		isSelected = FALSE;
	}

	void updateColor(COLORREF newColor) {
		color = newColor;
	}

	void setIndex(int index) {
		this->index = index;
	}

	int myIndex() const {
		return index;
	}

	virtual DWORD getColor() const{
		return color;
	}
protected:
	vector<CPoint> dots;
	COLORREF color;
	bool isSelected;
	int index;
};

class MyLine : public MyShape {
public:
	MyLine(CPoint startP, CPoint endP, COLORREF color, int index) {
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
		this->index = index;
	}

	void drawMe(CDC *dc) const {
		COLORREF oldPen;
		dc->SelectObject(GetStockObject(DC_PEN));
		if (isSelected) {
			oldPen = dc->SetDCPenColor(color + RGB(255, 255, 255));
		}
		else {
			oldPen = dc->SetDCPenColor(color);
		}

		dc->MoveTo(dots[0]);
		dc->LineTo(dots[1]);

		dc->SetDCPenColor(oldPen);
	}

	virtual MyShape *isInside(CPoint point, CDC *dc) {
		return NULL;
	}

	virtual void offsetShape(CPoint offset) {
		dots[0] += offset;
		dots[1] += offset;
	}
};

class MyEllipse : public MyShape {
public:
	MyEllipse(CPoint startP, CPoint endP, COLORREF color, int index) {
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
		this->index = index;
	}

	void drawMe(CDC *dc) const {
		COLORREF oldPen, oldBrush;

		dc->SelectObject(GetStockObject(DC_BRUSH));
		dc->SelectObject(GetStockObject(DC_PEN));
		if (isSelected) {
			oldPen = dc->SetDCPenColor(color ^ RGB(255, 255, 255));
		}
		else {
			oldPen = dc->SetDCPenColor(color);
		}
		oldBrush = dc->SetDCBrushColor(color);

		dc->Ellipse(dots[0].x, dots[0].y, dots[1].x, dots[1].y);

		dc->SetDCBrushColor(oldBrush);
		dc->SetDCPenColor(oldPen);
	}

	virtual MyShape *isInside(CPoint point, CDC *dc) {
		if (point.x > dots[0].x && point.y > dots[0].y && point.x < dots[1].x && point.y < dots[1].y) {
			isSelected = TRUE;
			drawMe(dc);
			return this;
		}
		else if (point.x < dots[0].x && point.y < dots[0].y && point.x > dots[1].x && point.y > dots[1].y) {
			isSelected = TRUE;
			drawMe(dc);
			return this;
		}
		else {
			isSelected = FALSE;
			drawMe(dc);
			return NULL;
		}
	}

	virtual void offsetShape(CPoint offset) {
		dots[0] += offset;
		dots[1] += offset;
	}
};

class MyTriangle : public MyShape {
public:
	MyTriangle(CPoint startP, CPoint endP, COLORREF color, int index) {
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
		this->index = index;
	}

	void drawMe(CDC *dc) const {
		CPoint points[3]; //0 - top, 1 - left, 2 - right
		CPoint startP, endP;
		startP = dots[0];
		endP = dots[1];

		triangle_Vdots(startP, endP, points[0], points[1], points[2]);
		
		COLORREF oldPen, oldBrush;

		dc->SelectObject(GetStockObject(DC_BRUSH));
		if (isSelected) {
			oldPen = dc->SetDCPenColor(color ^ RGB(255, 255, 255));
		}
		else {
			oldPen = dc->SetDCPenColor(color);
		}

		oldBrush = dc->SetDCBrushColor(color);
		
		Polygon(*dc, points, 3);
		
		dc->SetDCBrushColor(oldBrush);
		dc->SetDCPenColor(oldPen);
	}

	virtual MyShape *isInside(CPoint point, CDC *dc) {
		return NULL;
	}

	virtual void offsetShape(CPoint offset) {
		dots[0] += offset;
		dots[1] += offset;
	}
};

class MyRectangle : public MyShape {
public:
	MyRectangle(CPoint startP, CPoint endP, COLORREF color, int index) {
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
		this->index = index;
	}

	void drawMe(CDC *dc) const {
		COLORREF oldPen, oldBrush;
		
		dc->SelectObject(GetStockObject(DC_BRUSH));
		dc->SelectObject(GetStockObject(DC_PEN));

		if (isSelected) {
			oldPen = dc->SetDCPenColor(color ^ RGB(255, 255, 255));
		}
		else {
			oldPen = dc->SetDCPenColor(color);
		}
		oldBrush = dc->SetDCBrushColor(color);

		dc->Rectangle(dots[0].x, dots[0].y, dots[1].x, dots[1].y);

		dc->SetDCBrushColor(oldBrush);
		dc->SetDCPenColor(oldPen);
	}

	virtual MyShape *isInside(CPoint point, CDC *dc) {
		if (point.x > dots[0].x && point.y > dots[0].y && point.x < dots[1].x && point.y < dots[1].y) {
			isSelected = TRUE;
			drawMe(dc);
			return this;
		}
		else if (point.x < dots[0].x && point.y < dots[0].y && point.x > dots[1].x && point.y > dots[1].y) {
			isSelected = TRUE;
			drawMe(dc);
			return this;
		}
		else {
			isSelected = FALSE;
			drawMe(dc);
			return NULL;
		}
	}

	virtual void offsetShape(CPoint offset) {
		dots[0] += offset;
		dots[1] += offset;
	}
};

class MyPolygon : public MyShape {
public:
	MyPolygon(vector<CPoint> dotsA, COLORREF color, int index) {
		dots = dotsA;
		size = dots.size();
		this->color = color;
		this->index = index;
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
		if (isSelected) {
			oldPen = dc->SetDCPenColor(color ^ RGB(255, 255, 255));
		}
		else {
			oldPen = dc->SetDCPenColor(color);
		}

		oldBrush = dc->SetDCBrushColor(color);

		Polygon(*dc, points, size);

		dc->SetDCBrushColor(oldBrush);
		dc->SetDCPenColor(oldPen);

		delete[] points;
	}

	virtual MyShape *isInside(CPoint point, CDC *dc) {
		return NULL;
	}

	virtual void offsetShape(CPoint offset) {
		for (int i = 0;i < size;i++) {
			dots[i] += offset;
		}
	}
private:
	int size;
};


#endif
