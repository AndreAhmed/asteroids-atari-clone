#pragma once
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "Bullet.h"
#include <list>
using namespace std;

class Ship
{
public:
	enum states{Ship_Alive, Ship_Dead};
public:
	Ship(void);
	~Ship(void);
	void Init(ci::Vec2f pos);
	void Draw();
	void Update();
	void FireBullet();
	ci::Vec2f getCenter();
	double	  getRadius();
private:
	
public:
	list<Bullet>   m_Bullets;
	ci::Vec2f	   m_Pos;
	ci::Vec2f	   m_Vel;
	float		   m_Angle;
	float		   m_fire;
	int			   m_state;
	int			   m_lifeSpan;
	bool		   m_Thrust;
	bool		   m_saveField;
	bool		   up,left,right,fire;
};

