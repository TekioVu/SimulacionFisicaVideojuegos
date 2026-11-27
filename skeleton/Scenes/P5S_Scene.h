#pragma once
#include "Scene.h"
#include "../Forces/SpringForceGenerator.h"
#include "../Forces/AnchoredSpringForceGenerator.h"
#include "../Forces/GravityGenerator.h"
#include "../Particle/Particle.h"
#include "../RenderUtils.hpp"
#include <deque>
#include <vector>
#include "../Forces/BuoyancyForceGenerator.h"

constexpr float K_INICIAL = 10.0f; 
constexpr float K_PASO = 1.0f; 
constexpr float K_MIN = 0.5f;
constexpr float K_MAX = 2000.0f;
constexpr float REST_LENGTH = 10.0f;

constexpr int   SLINKY_N = 10;
constexpr float SLINKY_K = 100.0f;
constexpr float SLINKY_REST = 2.0f;
constexpr float SLINKY_ELONG = 1.0f;

class P5S_Scene : public Scene
{
public:
	explicit P5S_Scene(std::string name) : Scene(std::move(name)) {}
	void init() override;
	void cleanup() override;
	void update(double dt) override;
	void keyPress(unsigned char key, const physx::PxTransform& camera) override;

public:
	// Enumeración del orden de los objetos a renderizar, para poder acceder a ellos por índice
	enum OBJECTS {

	};

private:
	// Función para agregar un RenderItem a la escena, almacenando su transformada y color
	void addRenderItem(physx::PxShape* shape, const Vector3& pos, const Colors& color);
	void AddStaticSpring(ParticleModel springModel);
	void AddTwoParticleSpring(ParticleModel springModel);
	void AddRubberBand(ParticleModel springModel);
	void AddBuoyancy();


	std::deque<physx::PxTransform> m_transforms;
	std::vector<RenderItem*> m_renderItems;

	float impulseTimer = 0.0f;

	std::vector<ForceGenerator*> forces;
	std::vector<Particle*> particles;

	SpringForceGenerator* muelle = nullptr;
	Particle* staticParticle = nullptr;

	//Flotacion
	Particle* boxP; 
	BuoyancyForceGenerator* buoy; 
	float boxMass = 500.0f;

	std::vector<SpringForceGenerator*> slinkySprings;
	AnchoredSpringForceGenerator* slinkyAnchor = nullptr;
	float slinkyK = SLINKY_K;
	bool  slinkyHeld = true;

	void AddSlinky(ParticleModel m);
	void setSlinkyK(float k);

};

