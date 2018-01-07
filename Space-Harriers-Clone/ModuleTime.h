#ifndef _MODULE_TIME_H_
#define _MODULE_TIME_H_

#include "Module.h"

class ModuleTime :
	public Module
{
public:
	ModuleTime(bool enabled = true);
	~ModuleTime();

	bool Init() override;
	update_status PreUpdate() override;

	float GetDeltaTime() const;
private:
	float currentDeltaTime;
	Uint32 previousTicks;
};

#endif // !_MODULE_TIME_H_