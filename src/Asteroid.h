#pragma  once

#include "cinder/Vector.h"
enum Asteroids_State { Asteroid_Large, Asteroid_Medium, Asteroid_Small};
class Asteroid
{
public:
	Asteroid();
	void Draw();
	void Init(int shapeType, float size);
	void Update();
	ci::Vec2f getCenter();
	double    getRadius();
	
public:

	ci::Vec2f m_Pos;
	ci::Vec2f m_Vel;
	float	  m_Radius;
	int		  m_iShapeType;
	float	  m_size;
	bool	  m_bHit;
	int		  m_state;
};