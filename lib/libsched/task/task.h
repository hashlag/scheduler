#ifndef TASK_H_
#define TASK_H_

#include <functional>
#include <memory>

#include "wrappers/callable_wrapper.h"

namespace sched {

class Task {
public:
	Task() = delete;

	template<typename Fn, typename... Args>
	explicit Task(Fn&& fn_, Args&&... args) {
		auto packed = [fn = std::forward<Fn>(fn_), ...args = std::forward<Args>(args)]() mutable {
			return fn(std::forward<Args>(args)...);
		};

		callable_ = std::make_shared<CallableWrapper<decltype(packed)>>(std::move(packed));
	}

	template<typename ReturnType>
	ReturnType& get_result() {
		return std::static_pointer_cast<Callable<ReturnType>>(callable_)->operator()();
	}

	void execute() {
		std::static_pointer_cast<Executable>(callable_)->execute();
	}

private:
	std::shared_ptr<void> callable_;
};

} //sched

#endif //TASK_H_
