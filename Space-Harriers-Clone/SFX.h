#ifndef _SFX_H_
#define _SFX_H_

class SFX
{
public:
	SFX() : SFX(-1) {}
	SFX(unsigned int id) : sfxid(id) {}

	unsigned int GetId() const { return sfxid; }

private:
	unsigned int sfxid;
};

#endif // !_SFX_H_
