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

const float Ovni::PARTICLE_SPEED = 500;

Ovni::Ovni(const Vector3 startingPosition, float speed, const Texture& texture, const Animation& animation, const vector<Vector3>& path, const set<unsigned int>& particleSpawnsIndex, float scalingFactor) :
	Enemy(new ScreenBoundTransform(startingPosition),true),
	collider(nullptr),
	speed(speed),
	graphics(texture),
	animation(animation),
	scalingFactor(scalingFactor),
	currentTarget(0),
	path(path),
	particleSpawnsIndex(particleSpawnsIndex)
{
}

Ovni::~Ovni()
{
	collider->to_delete = true;
}

void Ovni::Init(map<string, void*> values)
{
	collider = App->collision->RegisterPrototypeInstance(*collider, *this);
}

void Ovni::Update()
{
	float deltaTime = App->time->GetDeltaTime();
	Vector3 newPosition = MoveTowards(GetTransform().GetScreenPositionAndDepth(), path[currentTarget], speed*deltaTime);
	static_cast<ScreenBoundTransform&>(GetTransform()).SetScreenPosition(newPosition);
	collider->UpdateValues(GetTransform());

	if (newPosition == path[currentTarget]) {
		if (particleSpawnsIndex.count(currentTarget)) {
			Vector3 velocity = App->player->GetChestPosition() - path[currentTarget];
			velocity = velocity.Normalized() * PARTICLE_SPEED;
			App->particles->AddParticleByName("ovni", path[currentTarget], velocity);
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
