#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"

class Application;
struct Collider;

class Module
{
public:

	Module(bool enabled = true) : enabled(enabled)
	{}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return enabled;
	}

	 bool Enable()
	{
		if(enabled == false)
			return enabled = Start();

		return true;
	}

	 bool Disable()
	 {
		 if(enabled == true)
			 return enabled = !CleanUp();

		 return true;
	 }

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

private:
	bool enabled = true;
};

#endif // __MODULE_H__