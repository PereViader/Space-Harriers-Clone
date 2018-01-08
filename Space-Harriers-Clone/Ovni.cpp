#include "Ovni.h"

#include "ScreenBoundTransform.h"
#include "Application.h"
#include "ModuleTime.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "Pivot2D.h"
#include "ModuleShadow.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "Collider.h"
#include "Transform.h"
#include "ModulePlayer.h"
#include "FloorBasedTransform.h"
#include "Application.h"
#include "ModuleEnemy.h"
#include "Explosion.h"

Ovni::Ovni(float speed, float particleSpeed, const Texture& texture, const Animation& animation, const Size2D& size, const SFX& sfx, float scalingFactor) :
	Enemy(new FloorBasedTransform(),true),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::BOTTOM_CENTER, *this)),
	speed(speed),
	particleSpeed(particleSpeed),
	graphics(texture),
	animation(animation),
	scalingFactor(scalingFactor),
	currentTarget(0),
	sfx(sfx)
{
}

Ovni::Ovni(const Ovni & o) :
	Enemy(o),
	collider(App->collision->RegisterPrototypeInstance(*o.collider, *this)),
	speed(o.speed),
	particleSpeed(o.particleSpeed),
	graphics(o.graphics),
	animation(o.animation),
	scalingFactor(o.scalingFactor),
	currentTarget(o.currentTarget),
	path(o.path),
	particleSpawnsIndex(o.particleSpawnsIndex),
	sfx(o.sfx)
{
	App->audio->RegisterFxUsage(sfx);
}

Ovni::~Ovni()
{
	App->audio->UnloadFx(sfx);
	collider->MarkAsDeleted();
}

void Ovni::Init(const json& parameters)
{
	Vector3 position = parameters["position"];
	GetTransform().Move(position);

	vector<Vector3> path = parameters["path"];
	set<unsigned int> particleSpawns = parameters["particleSpawns"];
	SetPathAndBullets(path, particleSpawns);
}

void Ovni::Start()
{
	App->audio->PlayFx(sfx);
}

void Ovni::Update()
{
	FollowPath();
}

void Ovni::FollowPath()
{
	float deltaTime = App->time->GetDeltaTime();

	FloorBasedTransform& transform = static_cast<FloorBasedTransform&>(GetTransform());
	Vector3 newPosition = MoveTowards(transform.GetPosition(), path[currentTarget], speed*deltaTime);
	transform.SetPosition(newPosition);

	if (newPosition == path[currentTarget]) {
		if (particleSpawnsIndex.count(currentTarget)) {
			Vector3 velocity = App->player->GetPlayer().GetChestPosition() - transform.GetScreenPositionAndDepth();
			velocity = velocity.Normalized() * particleSpeed;
			App->particles->AddParticleByName("ovni", transform.GetScreenPositionAndDepth(), velocity);
		}
		currentTarget += 1;
		if (currentTarget == path.size()) {
			MarkAsDeleted();
		}
	}
}

void Ovni::OnCollision(const Collider& own, const Collider& other)
{
	if (other.colliderType == ColliderType::PlayerParticle) {
		OnOvniKilled();
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

void Ovni::SetPathAndBullets(const vector<Vector3>& path, const set<unsigned int>& particleSpawnsIndex)
{
	this->path = path;
	this->particleSpawnsIndex = particleSpawnsIndex;
}

void Ovni::OnOvniKilled()
{
	MarkAsDeleted();

	Explosion * explosion = static_cast<Explosion*>(App->enemies->InstantiateEnemyByName("explosion", json()));
	explosion->GetTransform().SetPosition(GetTransform());
}
