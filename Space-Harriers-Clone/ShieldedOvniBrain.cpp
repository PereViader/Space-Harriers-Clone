#include "ShieldedOvniBrain.h"

#include <list>

#include "ShieldedOvni.h"
#include "FloorBasedTransform.h"
#include "ModuleFloor.h"

const float ShieldedOvniBrain::TIME_CLOSED = 5.0f;
const float ShieldedOvniBrain::TIME_OPEN = 6.0f;

const Vector3 ShieldedOvniBrain::HORIZON_START(SCREEN_WIDTH/2.0f, 500, Z_MAX);
const Vector3 ShieldedOvniBrain::HORIZON_CLOSER(SCREEN_WIDTH / 2.0f, 500, Z_MAX - 400);
const Vector3 ShieldedOvniBrain::OVNI_LEFT(300, 500, Z_MAX-400);
const Vector3 ShieldedOvniBrain::OVNI_RIGHT(SCREEN_WIDTH - 300, 500, Z_MAX - 400);
const Vector3 ShieldedOvniBrain::OVNI_TOP(SCREEN_WIDTH / 2.0f, 800, Z_MAX - 400);

ShieldedOvniBrain::ShieldedOvniBrain(float speed, const Texture& graphics, const Animation& animation, const Size2D& size, float scalingFactor) :
	Enemy(new FloorBasedTransform(),false),
	areOpen(false),
	currentTime(0)
{
	ovnis.push_back(new ShieldedOvni({ HORIZON_CLOSER, OVNI_LEFT  }, speed, graphics, animation, size, scalingFactor));
	ovnis.push_back(new ShieldedOvni({ HORIZON_CLOSER, OVNI_RIGHT }, speed, graphics, animation, size, scalingFactor));
	ovnis.push_back(new ShieldedOvni({ HORIZON_CLOSER, OVNI_TOP   }, speed, graphics, animation, size, scalingFactor));
}

ShieldedOvniBrain::ShieldedOvniBrain(const ShieldedOvniBrain & o) :
	Enemy(o),
	areOpen(o.areOpen),
	currentTime(o.currentTime)
{
	for (ShieldedOvni* ovni : o.ovnis) {
		ovnis.push_back(new ShieldedOvni(*ovni));
	}
}

ShieldedOvniBrain::~ShieldedOvniBrain()
{
	for (ShieldedOvni* ovni : ovnis)
		delete ovni;
}

void ShieldedOvniBrain::OnCollision(const Collider & own, const Collider & other)
{
	//Has no collision
}

void ShieldedOvniBrain::Init(map<string, void*> values)
{
	for (ShieldedOvni * ovni : ovnis) {
		ovni->GetTransform().SetPosition(HORIZON_START);
	}
}

void ShieldedOvniBrain::Update()
{
	currentTime += App->time->GetDeltaTime();

	float TIME_CHECK = areOpen ? TIME_OPEN : TIME_CLOSED;

	if (currentTime >= TIME_CHECK) {
		currentTime -= TIME_CHECK;
		areOpen = !areOpen;
		for (ShieldedOvni* ovni : ovnis) {
			ovni->SetOpen(areOpen);
		}
	}

	for (ShieldedOvni* ovni : ovnis)
		ovni->Update();

	for (ShieldedOvni* ovni : ovnis)
		ovni->Render();
}

Enemy * ShieldedOvniBrain::Clone() const
{
	return new ShieldedOvniBrain(*this);
}
