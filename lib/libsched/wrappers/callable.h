#ifndef CALLABLE_H_
#define CALLABLE_H_

#include "executable.h"

namespace sched {

template<typename ReturnType>
class Callable : public Executable {
public:
	virtual ReturnType& operator()() = 0;
	~Callable() override = default;
};

} //sched

#endif //CALLABLE_H_
