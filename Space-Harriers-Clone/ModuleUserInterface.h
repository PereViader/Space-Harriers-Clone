#ifndef _MODULE_USER_INTERFACE_H_
#define _MODULE_USER_INTERFACE_H_

#include "Module.h"
#include "IRenderable.h"

#include <list>

class ModuleUserInterface :
	public Module, public IRenderable
{
public:
	ModuleUserInterface(bool enabled);
	virtual ~ModuleUserInterface();

	virtual bool Start() override;
	virtual update_status Update() override;

	virtual void Render() override;

private:
	std::list<IRenderable*> uiElements;
};

#endif // !_MODULE_USER_INTERFACE_H_