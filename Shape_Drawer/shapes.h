#ifndef SHAEPS_H
#define SHAPES_H
#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

//void triangle_Vdots(CPoint start, CPoint end, CPoint &top, CPoint &left, CPoint &right);

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UtilityFunctions.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MyPolygon : public MyShape {
public:
	MyPolygon() {
		dots.clear();
		size = 0;
		color = RGB(0, 0, 0);
		index = -1;
	}
	MyPolygon(vector<CPoint> dotsA, COLORREF color, int index) {
		dots = dotsA;
		size = dots.size();
		this->color = color;
		this->index = index;
	}
	~MyPolygon() {
		dots.clear();
	}

	virtual void drawMe(CDC *dc) const {
		CPoint *points;
		points = new CPoint[size];

		for (int i = 0;i < size;i++) {
			points[i] = dots[i];
		}

		COLORREF oldPen, oldBrush;

		dc->SelectObject(GetStockObject(DC_PEN));
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

	virtual virtual MyShape *isInside(CPoint point, CDC *dc) {
		double distance, dist0, dist1;
		for (int i = 1;i < size;i++) {
			distance = utilityFunctions::CPdist(dots[i-1], dots[i]);
			dist0 = utilityFunctions::CPdist(dots[i-1], point);
			dist1 = utilityFunctions::CPdist(dots[i], point);
			if (dist0 + dist1 < distance + 2.5) {
				isSelected = TRUE;
				drawMe(dc);
				return this;
			}
		}
		distance = utilityFunctions::CPdist(dots[size - 1], dots[0]);
		dist0 = utilityFunctions::CPdist(dots[size - 1], point);
		dist1 = utilityFunctions::CPdist(dots[0], point);
		if (dist0 + dist1 < distance + 3.5) {
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
		for (int i = 0;i < size;i++) {
			dots[i] += offset;
		}
	}
protected:
	int size;
};



class MyLine : public MyPolygon {
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
			oldPen = dc->SetDCPenColor(color ^ RGB(255, 255, 255));
		}
		else {
			oldPen = dc->SetDCPenColor(color);
		}

		dc->MoveTo(dots[0]);
		dc->LineTo(dots[1]);

		dc->SetDCPenColor(oldPen);
	}

	virtual MyShape *isInside(CPoint point, CDC *dc) {
		double distance = utilityFunctions::CPdist(dots[0], dots[1]);
		double dist0 = utilityFunctions::CPdist(dots[0], point);
		double dist1 = utilityFunctions::CPdist(dots[1], point);

		if (dist0 + dist1 < distance + 2.5) {
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



class MyTriangle : public MyPolygon {
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

		utilityFunctions::triangle_Vdots(startP, endP, points[0], points[1], points[2]);
		
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
		
		Polygon(*dc, points, 3);
		
		dc->SetDCBrushColor(oldBrush);
		dc->SetDCPenColor(oldPen);
	}

	virtual MyShape *isInside(CPoint point, CDC *dc) {
		double area, area1, area2, area3;
		CPoint points[3]; //0 - top, 1 - left, 2 - right
		utilityFunctions::triangle_Vdots(dots[0], dots[1], points[0], points[1], points[2]);
		area = utilityFunctions::triangleArea(points[0], points[1], points[2]);
		area1 = utilityFunctions::triangleArea(points[0], points[1], point);
		area2 = utilityFunctions::triangleArea(points[1], points[2], point);
		area3 = utilityFunctions::triangleArea(points[2], points[0], point);
		if (area == area1 + area2 + area3) {
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

class MyRectangle : public MyPolygon {
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
		int height, width;
		CPoint center;

		height = dots[1].y - dots[0].y;
		width = dots[1].x - dots[0].x;
		
		center.x = dots[0].x + width / 2;
		center.y = dots[0].y + height / 2;

		height = height < 0 ? height * -1 : height;
		width = width < 0 ? width * -1 : width;

		if (point.x < center.x+width/2 && point.x > center.x-width/2 && point.y < center.y+ height /2 && point.y > center.y-height/2) {
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
		int height, width;
		CPoint center, f1, f2;
		double fDist, dist1, dist2, bigRadius;

		height = dots[1].y - dots[0].y;
		width = dots[1].x - dots[0].x;

		center.x = dots[0].x + width / 2;
		center.y = dots[0].y + height / 2;

		height /= 2;
		width /= 2;
		height *= height;
		width *= width;

		if (width < height) {
			fDist = height - width;
			fDist = sqrt(fDist);
			bigRadius = sqrt(height);
			f1.x = center.x;
			f2.x = center.x;
			f1.y = center.y + fDist;
			f2.y = center.y - fDist;
		}
		else {
			fDist = width - height;
			fDist = sqrt(fDist);
			bigRadius = sqrt(width);
			f1.y = center.y;
			f2.y = center.y;
			f1.x = center.x + fDist;
			f2.x = center.x - fDist;
		}

		dist1 = utilityFunctions::CPdist(f1, point);
		dist2 = utilityFunctions::CPdist(f2, point);

		if (dist1 + dist2 < 2*bigRadius) {
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



#endif
