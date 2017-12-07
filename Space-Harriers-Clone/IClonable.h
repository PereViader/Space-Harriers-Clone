#pragma once

template<typename T>
class IClonable {
public:
	virtual T Clone() const = 0;
};