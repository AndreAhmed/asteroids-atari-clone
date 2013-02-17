#pragma once

#include "cinder/Vector.h"

class Bullet
{

public:
	ci::Vec2f m_Pos;
	ci::Vec2f m_Vel;
	int		  m_life;
	bool	  m_Hit;
public:
	Bullet(void);
	~Bullet(void);
	ci::Vec2f getCenter();
	double	  getRadius();
	void Draw(int x, int y);
};

