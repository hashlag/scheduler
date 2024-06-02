#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <list>

#include "task/task.h"
#include "task_id/task_id.h"
#include "future/future_result.h"

namespace sched {

class Scheduler {
public:
	Scheduler() = default;

	Scheduler(const Scheduler&) = delete;
	Scheduler& operator=(const Scheduler&) = delete;

	Scheduler(Scheduler&&) = default;
	Scheduler& operator=(Scheduler&&) = default;

	template <typename Fn, typename... Args>
	TaskId add(Fn&& fn, Args&&... args) {
		tasks_.emplace_front(std::forward<Fn>(fn), std::forward<Args>(args)...);
		return TaskId(tasks_.begin());
	}

	void remove(TaskId id) {
		tasks_.erase(id.it_);
	}

	template <typename ReturnType>
	ReturnType& get_result(TaskId id) {
		return id.it_->template get_result<ReturnType>();
	}

	template <typename ReturnType>
	FutureResult<ReturnType> get_future_result(TaskId id) {
		return FutureResult<ReturnType>(id.it_);
	}

	void execute_all() {
		for (auto& task : tasks_) {
			task.execute();
		}
	}

private:
	std::list<Task> tasks_;
};

} //sched

#endif //SCHEDULER_H_
