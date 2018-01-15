#ifndef SHAEPS_H
#define SHAPES_H
#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class MyShape { //parent
public:
	MyShape() { //constructor
		dots.clear();
		color = RGB(0, 0, 0);
		isSelected = FALSE;
		type = 0;
	}

	MyShape(const MyShape *me) { //copy constructor
		dots = me->dots;
		color = me->color;
		isSelected = me->isSelected;
		type = me->type;
	}

	virtual MyShape *clone() { //return a pointer to self
		return this;
	}
	
	virtual ~MyShape() { //destructor
		dots.clear();
	}

	virtual void drawMe(CDC *) const = 0; //pure virtual function to draw the shape

	virtual bool isInside(CPoint point, CDC *dc) {
		return FALSE;
	}

	virtual void offsetShape(CPoint offset) = 0; //pure virtual function to move the shape

	virtual void unselectShape() { //clear unselect shape flag
		isSelected = FALSE;
	}

	virtual bool selectedState() const { //return selected flag state
		return isSelected;
	}

	virtual void updateColor(COLORREF newColor) { //update the color of the shape
		color = newColor;
	}

	virtual COLORREF getColor() const{ //return the color of the shape
		return color;
	}

	virtual void updateDots(CPoint point, int i) { //update a value of a vertex of shape
		dots[i].x = point.x;
		dots[i].y = point.y;
	}

	virtual void writeFile(fstream *myFile) { //write shape data to a file
		myFile->write((char*)&type, sizeof(int)); //shape type
		myFile->write((char*)&dots[0].x, sizeof(int)); //first vertex
		myFile->write((char*)&dots[0].y, sizeof(int));
		myFile->write((char*)&dots[1].x, sizeof(int)); //second vertex
		myFile->write((char*)&dots[1].y, sizeof(int));
		myFile->write((char*)&color, sizeof(COLORREF)); //shape color
	}
protected:
	vector<CPoint> dots; //vector of vertexes of retangle that lock the shape or vertexes of polygon
	COLORREF color; //color of shape
	bool isSelected; //flag to determine if shape is selected
	int type; //flag to determine type of shape (for saving and loading from file)
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UtilityFunctions.h" //include utility function class whitch uses the clase MyShape


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MyPolygon : public MyShape { //child
public:
	MyPolygon() { //default constructor
		dots.clear();
		size = 0;
		color = RGB(0, 0, 0);
		type = 5;
	}
	MyPolygon(vector<CPoint> dotsA, COLORREF color) { //constructor
		dots = dotsA;
		size = dots.size();
		this->color = color;
		type = 5;
	}

	MyPolygon(const MyPolygon *me) : MyShape(me) { // copy constructor
		size = me->size;
		type = 5;
	}

	virtual MyShape *clone() { //clone shape
		return new MyPolygon(*this); //return address of the copy of self
	}

	virtual ~MyPolygon() { // destructor
		dots.clear();
	}

	virtual void drawMe(CDC *dc) const { //draw a polygon
		CPoint *points; 
		points = new CPoint[size]; //array of polygon vertexes for drawing function

		for (int i = 0;i < size;i++) {
			points[i] = dots[i]; //copy the vertexes from vector to array
		}

		COLORREF oldPen, oldBrush; //save old colors

		dc->SelectObject(GetStockObject(DC_PEN)); //select standart pen
		dc->SelectObject(GetStockObject(DC_BRUSH)); //select standart brush
		if (isSelected) { //if shape is selected
			oldPen = dc->SetDCPenColor(color ^ RGB(255, 255, 255)); //outline color is invert color of the shape color
		}
		else {
			oldPen = dc->SetDCPenColor(color); //set color of current pen
		}

		oldBrush = dc->SetDCBrushColor(color); //set color of current brush

		dc->Polygon(points, size); //draw polygon

		dc->SetDCBrushColor(oldBrush); //return to old brush
		dc->SetDCPenColor(oldPen); //return to old pen

		delete[] points; //free memmory of array of vertexes
	}

	virtual bool isInside(CPoint point, CDC *dc) { //determine if a point is inside a polygon (close to border of the shape)
		double distance, dist0, dist1;
		//distance - between to vertexes
		//dist 0 - distance between point and first vertex
		//dist 1 - distance between point and second vertex
		for (int i = 1;i < size;i++) { //for each line of polygon
			distance = utilityFunctions::CPdist(dots[i-1], dots[i]);
			dist0 = utilityFunctions::CPdist(dots[i-1], point);
			dist1 = utilityFunctions::CPdist(dots[i], point); //claculate distances using utiliity function
			if (dist0 + dist1 < distance + 2.5) { //if point is in range of line
				isSelected = TRUE; //set selected flag
				drawMe(dc); //update shape border to invert color
				return TRUE; //return positive
			}
		}

		//do the same as above for line between last and first vertex in the vector
		distance = utilityFunctions::CPdist(dots[size - 1], dots[0]);
		dist0 = utilityFunctions::CPdist(dots[size - 1], point);
		dist1 = utilityFunctions::CPdist(dots[0], point);
		if (dist0 + dist1 < distance + 3.5) {
			isSelected = TRUE;
			drawMe(dc);
			return TRUE;
		}

		else { //if point is not in range from line
			isSelected = FALSE; //reset selcted flag
			drawMe(dc); //make sure boreder of shape is in original color
			return FALSE; //return negative
		}

	}

	virtual void offsetShape(CPoint offset) { //move shape
		for (int i = 0;i < size;i++) { //for each vertex
			dots[i] += offset; //move it by offset value
		}
	}

	virtual void writeFile(fstream *myFile) { //write to file
		myFile->write((char*)&type, sizeof(int)); //type - polygon
		myFile->write((char*)&size, sizeof(int)); //size - number of vertexes
		for (int i = 0;i < size;i++) { //write each vertex value
			myFile->write((char*)&dots[i].x, sizeof(int));
			myFile->write((char*)&dots[i].y, sizeof(int));
		}
		myFile->write((char*)&color, sizeof(COLORREF)); //color of polygon
	}
protected:
	int size; // size - number of vertexes in  polygon
};

class MyLine : public MyPolygon { //grand child
public:
	MyLine() : MyPolygon() { //default constructor
		type = 1;
	}

	MyLine(CPoint startP, CPoint endP, COLORREF color) { //constructor
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
		type = 1;
	}

	MyLine(const MyLine *me) : MyPolygon(me) { //copy constructor
		type = 1;
	}

	virtual MyShape *clone() { //copy of line
		return new MyLine(*this); //return address to a copy of self
	}

	void drawMe(CDC *dc) const { //documentation similar to parent
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

	virtual bool isInside(CPoint point, CDC *dc) { //documentation similar to parent
		double distance = utilityFunctions::CPdist(dots[0], dots[1]);
		double dist0 = utilityFunctions::CPdist(dots[0], point);
		double dist1 = utilityFunctions::CPdist(dots[1], point);

		if (dist0 + dist1 < distance + 2.5) {
			isSelected = TRUE;
			drawMe(dc);
			return TRUE;
		}
		else {
			isSelected = FALSE;
			drawMe(dc);
			return FALSE;
		}
	}

	virtual void offsetShape(CPoint offset) { //documentation similar to parent
		dots[0] += offset;
		dots[1] += offset;
	}

	virtual void writeFile(fstream *myFile) { //documentatioon similar to grand parent
		myFile->write((char*)&type, sizeof(int));
		myFile->write((char*)&dots[0].x, sizeof(int));
		myFile->write((char*)&dots[0].y, sizeof(int));
		myFile->write((char*)&dots[1].x, sizeof(int));
		myFile->write((char*)&dots[1].y, sizeof(int));
		myFile->write((char*)&color, sizeof(COLORREF));
	}
};



class MyTriangle : public MyPolygon { //granf child
public:
	MyTriangle() : MyPolygon() { //default constructor
		type = 3;
	}
	MyTriangle(CPoint startP, CPoint endP, COLORREF color) { //constructor
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
		type = 3;
	}

	MyTriangle(const MyTriangle *me) : MyPolygon(me) { //copy constructor
		type = 3;
	}

	virtual MyShape *clone() { //copy of triangle
		return new MyTriangle(*this); //return address to copy of self
	}

	void drawMe(CDC *dc) const {
		CPoint points[3]; //0 - top, 1 - left, 2 - right |-> vertexes if triangle

		utilityFunctions::triangle_Vdots(dots[0], dots[1], points[0], points[1], points[2]); //determive triangle vertexes using utility function
		

		//rest of method similar to parent
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

	virtual bool isInside(CPoint point, CDC *dc) {
		double area, area1, area2, area3;
		//area - of triangle
		//area1 - area of sub triangle betwin point and vertexes 1,2
		//area2 - area of sub triangle betwin point and vertexes 2,3
		//area3 - area of sub triangle betwin point and vertexes 3,1

		CPoint points[3]; //0 - top, 1 - left, 2 - right |-> vertexes of triangle

		utilityFunctions::triangle_Vdots(dots[0], dots[1], points[0], points[1], points[2]); //determive triangle vertexes using utility function
		area = utilityFunctions::triangleArea(points[0], points[1], points[2]);
		area1 = utilityFunctions::triangleArea(points[0], points[1], point);
		area2 = utilityFunctions::triangleArea(points[1], points[2], point);
		area3 = utilityFunctions::triangleArea(points[2], points[0], point); //calcualte area using utility function
		if (area == area1 + area2 + area3) { //if the sum of sub areas = to area of triangle
			isSelected = TRUE; //set selected flag
			drawMe(dc); //update border to invert color
			return TRUE; // return positive
		}
		else {
			isSelected = FALSE; //reset selected flag
			drawMe(dc); //make sure border is in original color
			return FALSE; //return negatice
		}
	}

	virtual void offsetShape(CPoint offset) { //documentation similar to parent
		dots[0] += offset;
		dots[1] += offset;
	}

	virtual void writeFile(fstream *myFile) { //documentation similar to parent
		myFile->write((char*)&type, sizeof(int));
		myFile->write((char*)&dots[0].x, sizeof(int));
		myFile->write((char*)&dots[0].y, sizeof(int));
		myFile->write((char*)&dots[1].x, sizeof(int));
		myFile->write((char*)&dots[1].y, sizeof(int));
		myFile->write((char*)&color, sizeof(COLORREF));
	}
};

class MyRectangle : public MyPolygon { //grand child
public:
	MyRectangle() : MyPolygon() { //defaul constructor
		type = 4;
	}

	MyRectangle(CPoint startP, CPoint endP, COLORREF color) { //constructor
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
		type = 4;
	}

	MyRectangle(const MyRectangle *me) : MyPolygon(me) { //copy constructor
		type = 4;
	}

	virtual MyShape *clone() { //copy rectangle
		return new MyRectangle(*this); //return address to copy of self
	}

	virtual void drawMe(CDC *dc) const { //documentation similar to parent
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

	virtual bool isInside(CPoint point, CDC *dc) { //determine if point is inside the rectangle
		int height, width; //height and width of rectangle
		CPoint center; //center point of rectangle

		height = dots[1].y - dots[0].y;
		width = dots[1].x - dots[0].x;
		
		center.x = dots[0].x + width / 2;
		center.y = dots[0].y + height / 2;

		height = height < 0 ? height * -1 : height; //absolute value of height
		width = width < 0 ? width * -1 : width; //absolut vlaue of width

		if (point.x < center.x+width/2 && point.x > center.x-width/2 && point.y < center.y+ height /2 && point.y > center.y-height/2) {
			//determine if point is in range of center of rectangle by height and width
			isSelected = TRUE; //set selected flag
			drawMe(dc); //invert color of border
			return TRUE; //return positive
		}
		else {
			isSelected = FALSE; //reset selected flag
			drawMe(dc); //set border to original color
			return FALSE; //return negative
		}
	}

	virtual void offsetShape(CPoint offset) { //documentation similar to parent
		dots[0] += offset;
		dots[1] += offset;
	}

	virtual void writeFile(fstream *myFile) { //documentation similar to parent
		myFile->write((char*)&type, sizeof(int));
		myFile->write((char*)&dots[0].x, sizeof(int));
		myFile->write((char*)&dots[0].y, sizeof(int));
		myFile->write((char*)&dots[1].x, sizeof(int));
		myFile->write((char*)&dots[1].y, sizeof(int));
		myFile->write((char*)&color, sizeof(COLORREF));
	}
};



class MyEllipse : public MyShape { //child
public:
	MyEllipse() : MyShape() { //default constructor
		type = 2;
	}

	MyEllipse(CPoint startP, CPoint endP, COLORREF color) { //constructor
		dots.push_back(startP);
		dots.push_back(endP);
		this->color = color;
		type = 2;
	}

	MyEllipse(const MyEllipse *me) : MyShape(me) { //copy constructor
		type = 2;
	}

	virtual MyShape *clone() { //copy circle
		return new MyEllipse(*this); //return address to a copy of self
	}

	void drawMe(CDC *dc) const { //documentation similar to parent
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

	virtual bool isInside(CPoint point, CDC *dc) {
		int height, width; //height - vertical radius, width - horizontal radius
		CPoint center, f1, f2; //center - of elipse, f1 - 1st focal, f2 - 2nd focal
		double fDist, dist1, dist2, bigRadius;
		//fdist - distance between focal and center - to be calculated
		//dist1 - distance between point and f1
		//dist2 - distance between point and f2
		//bigradius = height < width ? width : height

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
			fDist = sqrt(fDist); //calculate dist of focals from center
			bigRadius = sqrt(height);
			f1.x = center.x;
			f2.x = center.x;
			f1.y = center.y + fDist;
			f2.y = center.y - fDist;
			//set f1 and f2
		}
		else {
			fDist = width - height;
			fDist = sqrt(fDist);//calculate dist of focals from center
			bigRadius = sqrt(width);
			f1.y = center.y;
			f2.y = center.y;
			f1.x = center.x + fDist;
			f2.x = center.x - fDist;
			//set f1 and f2
		}

		dist1 = utilityFunctions::CPdist(f1, point);
		dist2 = utilityFunctions::CPdist(f2, point);
		//calculate distance of point from both foclas using utility function

		if (dist1 + dist2 < 2*bigRadius) { //determine if point is inside the ellipse
			isSelected = TRUE; //set selected flag
			drawMe(dc); //set border to invert color
			return TRUE; //return positive
		}
		else {
			isSelected = FALSE; //reset selected flag
			drawMe(dc); //set border to original color
			return FALSE; //return negative
		}
	}

	virtual void offsetShape(CPoint offset) { //documentation similar to MyPolygon
		dots[0] += offset;
		dots[1] += offset;
	}

	virtual void writeFile(fstream *myFile) { //documentation similar to MyPolygon
		myFile->write((char*)&type, sizeof(int));
		myFile->write((char*)&dots[0].x, sizeof(int));
		myFile->write((char*)&dots[0].y, sizeof(int));
		myFile->write((char*)&dots[1].x, sizeof(int));
		myFile->write((char*)&dots[1].y, sizeof(int));
		myFile->write((char*)&color, sizeof(COLORREF));
	}
};

class colorMap : public MyRectangle { //grand grand child
public:
	colorMap() { //constructor
		size = 250;
		offset = 130;
		CPoint a(offset - 1, 0);
		CPoint b(offset + 277, 258);
		dots.push_back(a);
		dots.push_back(b);
		color = RGB(255, 255, 255);
		type = 6;
	}
	void drawMe(CDC *dc) const {
		double h, l; //h - hue, l - luminance

		COLORREF rgb; //rgb color
		h = 0;
		l = 0;
		dc->MoveTo(0, 0);
		dc->Rectangle(offset - 1, 0, offset + 277, 280);
		dc->Rectangle(offset - 1, 0, offset + 277, 258); //draw background rectangle for color palet

		for (int i = offset;i <= size + offset;i++) { //paint rainbow pallet
			for (int j = 0;j <= size;j++) {
				rgb = CDrawingManager::HLStoRGB_ONE(h, l, 1); //full contrast
				dc->SetPixel(i, j, rgb);
				l += 0.004;
			}
			l = 0;
			h += 0.004;
		}

		for (int i = size + offset + 5;i <= size + offset + 25;i++) { //paint white->gray->black bar
			for (int j = 0;j <= size;j++) {
				rgb = CDrawingManager::HLStoRGB_ONE(h, l, 0); //zero contrast
				dc->SetPixel(i, j, rgb);
				l += 0.004;
			}
			l = 0;
			h += 0.05;
		}

		CString str("Selected Color:");
		CRect box(offset + 30, 260, offset + 150, 275);
		dc->DrawText(str, box, DT_CENTER); //print label
	}

	bool isInside(CPoint point) { //documentation similar to parent
		int height, width;
		CPoint center;

		height = dots[1].y - dots[0].y;
		width = dots[1].x - dots[0].x;

		center.x = dots[0].x + width / 2;
		center.y = dots[0].y + height / 2;

		height = height < 0 ? height * -1 : height;
		width = width < 0 ? width * -1 : width;

		if (point.x < center.x + width / 2 && point.x > center.x - width / 2 && point.y < center.y + height / 2 && point.y > center.y - height / 2) {
			isSelected = TRUE;
			return TRUE;
		}
		else {
			isSelected = FALSE;
			return FALSE;
		}
	}

	void selectedColor(CDC *dc, COLORREF color) { //paint small retangle with selected color
		CPoint a(offset + 185, 259);
		CPoint b(offset + 250, 278);


		COLORREF oldPen, oldBrush;

		dc->SelectObject(GetStockObject(DC_BRUSH));
		dc->SelectObject(GetStockObject(DC_PEN));


		oldPen = dc->SetDCPenColor(color);

		oldBrush = dc->SetDCBrushColor(color);

		dc->Rectangle(a.x, a.y, b.x, b.y);

		dc->SetDCBrushColor(oldBrush);
		dc->SetDCPenColor(oldPen);

		return;
	}

	void pallet_invldt(CRect &ret) const {
		ret.SetRect(offset - 1, 0, offset + 277, 280); //to be returned
	}
private:
	int size; //size of pallet
	int offset; //distance from top left corner
};

#endif
