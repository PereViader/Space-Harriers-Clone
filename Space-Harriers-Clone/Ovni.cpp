#include "Ovni.h"

#include "ScreenBoundTransform.h"
#include "Application.h"
#include "ModuleTime.h"
#include "ModuleRender.h"
#include "Pivot2D.h"
#include "ModuleShadow.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Collider.h"
#include "Transform.h"
#include "ModulePlayer.h"
#include "FloorBasedTransform.h"

Ovni::Ovni(float speed, float particleSpeed, const Texture& texture, const Animation& animation, const Size2D& size, const vector<Vector3>& path, const set<unsigned int>& particleSpawnsIndex, float scalingFactor) :
	Enemy(new FloorBasedTransform(),true),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::BOTTOM_CENTER, *this)),
	speed(speed),
	particleSpeed(particleSpeed),
	graphics(texture),
	animation(animation),
	scalingFactor(scalingFactor),
	currentTarget(0),
	path(path),
	particleSpawnsIndex(particleSpawnsIndex)
{
}

//TODO copy constructor

Ovni::~Ovni()
{
	collider->MarkAsDeleted();
}

void Ovni::Init(map<string, void*> values)
{
	collider = App->collision->RegisterPrototypeInstance(*collider, *this);
}

void Ovni::Update()
{
	float deltaTime = App->time->GetDeltaTime();

	FloorBasedTransform& transform = static_cast<FloorBasedTransform&>(GetTransform());
	Vector3 newPosition = MoveTowards(transform.GetPosition(), path[currentTarget], speed*deltaTime);
	transform.SetPosition(newPosition);

	if (newPosition == path[currentTarget]) {
		if (particleSpawnsIndex.count(currentTarget)) {
			Vector3 velocity = App->player->GetChestPosition() - transform.GetScreenPositionAndDepth();
			velocity = velocity.Normalized() * particleSpeed;
			App->particles->AddParticleByName("ovni", transform.GetScreenPositionAndDepth(), velocity);
		}
		currentTarget += 1;
		if (currentTarget == path.size()) {
			//MarkAsDeleted();
			currentTarget = 0;
		}
	}
}

void Ovni::OnCollision(const Collider& own, const Collider& other)
{
	assert(&own == static_cast<const Collider*>(collider));
	if (other.colliderType == ColliderType::PlayerParticle) {
		MarkAsDeleted();
	}
}

Ovni* Ovni::Clone() const
{
	return new Ovni(*this);
}

void Ovni::Render()
{
	Enemy::Render();
	if (!ToDelete()) {
		Vector3 screen = GetTransform().GetScreenPositionAndDepth();
		float scale = scalingFactor * GetTransform().GetRenderingScale();
		animation.UpdateFrame();
		graphics.UpdateTexture(animation);
		App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::BOTTOM_CENTER, screen);
	}
}
