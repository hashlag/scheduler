#ifndef TASK_ID_H_
#define TASK_ID_H_

#include <list>

#include "task/task.h"

namespace sched {

class TaskId {
public:
	TaskId() = delete;

private:
	explicit TaskId(std::list<Task>::iterator it) : it_(it) {};

	std::list<Task>::iterator it_;

	friend class Scheduler;
};

} //sched

#endif //TASK_ID_H_
