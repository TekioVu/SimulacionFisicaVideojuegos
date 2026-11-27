#pragma once
#include "../Forces/ForceGenerator.h"
#include "../Utils/Vector3D.h"

class BuoyancyForceGenerator : public ForceGenerator
{
public:
	BuoyancyForceGenerator(float h, float v, float d, float surfaceY, float drag = 0.0f)
		: _height(h), _volume(v), _liquid_density(d), _surfaceY(surfaceY), _drag(drag) {
	}

	Vector3D Force(Vector3D pos, Vector3D vel, float mass) override
	{
		float h = pos.y;         
		float h0 = _surfaceY;     

		float immersed;          
		if (h - h0 > _height * 0.5f)       immersed = 0.0f; 
		else if (h0 - h > _height * 0.5f)  immersed = 1.0f;  
		else                               immersed = (h0 - h) / _height + 0.5f;

		float fy = _liquid_density * _volume * immersed * _gravity;  
		fy -= _drag * immersed * vel.y;                              

		return Vector3D{ 0.0f, fy, 0.0f };
	}

	void setVolume(float v) { _volume = v; _height = cbrtf(v); }  
	float getVolume() const { return _volume; }

private:
	float _height, _volume, _liquid_density, _surfaceY, _drag;
	float _gravity = 9.8f;
};