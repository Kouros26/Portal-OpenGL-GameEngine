#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <corecrt_math_defines.h>
#include <vector>

namespace Asteroid
{
	class Point2D
	{
	public:
		Point2D();
		Point2D(float x, float y);
		float& X();
		float& Y();
		bool operator==(const Point2D& pt);
		~Point2D();

	private:
		float x;
		float y;
	};

	class Line2D
	{
	public:
		Line2D();
		Line2D(float x1, float y1, float x2, float y2);
		float lenght();
		float squareLenght();
		void translateX(float value);
		void translateY(float value);
		Point2D& getPoint(const char* pt);
		~Line2D();

	private:
		Point2D A;
		Point2D B;
	};

	class Circle
	{
	public:
		Circle();
		Circle(float x, float y, float rad);
		float& getRadius();
		Point2D& getCenter();
		void translateX(float value);
		void translateY(float value);
		void setPos(float x, float y);
		~Circle();

	private:
		Point2D Center;
		float radius;
	};

	class Rectangle
	{
	public:
		Rectangle();
		Rectangle(float x, float y, float w, float h);
		Point2D& getPoint(const char* pt);
		float& getWidth();
		float& getHeight();
		void translateX(float value);
		void translateY(float value);
		~Rectangle();

	protected:
		Point2D topLeft;
		Point2D topRight;
		Point2D botLeft;
		Point2D botRight;
		Point2D Center;
		float width;
		float height;
	};

	class OrientedRectangle : public Rectangle
	{
	public:
		OrientedRectangle();
		OrientedRectangle(float x, float y, float w, float h, float ang);
		float getRotation();
		void rotate(float angle);
		~OrientedRectangle();

	private:
		float angle;
	};

	class PolygonShip
	{
	public:
		PolygonShip();
		PolygonShip(float x, float y);
		void translateX(float value);
		void translateY(float value);
		std::vector<Line2D> lines;
		Point2D Center;

	};

	class PolygonAsteroid
	{
	public:
		PolygonAsteroid();
		PolygonAsteroid(float x, float y, float scale);
		void translateX(float value);
		void translateY(float value);
		void rotate(float value);
		std::vector<Line2D> lines;
		Point2D Center;
		float angle = 0;
	};
}