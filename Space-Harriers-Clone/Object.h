#pragma once
class Object
{
public:
	Object(bool enabled);
	~Object();

	bool IsEnabled();

private:
	bool _isDead;
	bool _enabled;
};