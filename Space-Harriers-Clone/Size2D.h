#pragma once

class Size2D
{
public:
	Size2D() : width(0), height(0) {}
	Size2D(float width, float height) : width(width), height(height) {};
	
	float width;
	float height;
};