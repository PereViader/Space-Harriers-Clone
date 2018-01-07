#ifndef _ICLONABLE_H__
#define _ICLONABLE_H__

template<typename T>
class IClonable {
public:
	virtual T Clone() const = 0;
};

#endif // !_ICLONABLE_H__