#ifndef FUTURE_WRAPPER_H_
#define FUTURE_WRAPPER_H_

#include "task/task.h"

namespace sched {

template<typename ReturnType>
class FutureResult {
public:
	FutureResult() = delete;

private:
	operator ReturnType() const {
		return it_->template get_result<ReturnType>();
	}

	explicit FutureResult(std::list<Task>::iterator it) : it_(it) {};

	std::list<Task>::iterator it_;

	friend class Scheduler;
	friend class Task;
};

} //sched

#endif //FUTURE_WRAPPER_H_
