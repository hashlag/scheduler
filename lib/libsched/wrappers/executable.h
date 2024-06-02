#ifndef EXECUTABLE_H_
#define EXECUTABLE_H_

namespace sched {

class Executable {
public:
	virtual void execute() = 0;
	virtual ~Executable() = default;
};

} //sched

#endif //EXECUTABLE_H_
