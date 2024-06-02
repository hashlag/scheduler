#ifndef WRAPPER_H_
#define WRAPPER_H_

#include <type_traits>

#include "callable.h"

namespace sched {

template<typename Fn, typename ReturnType = std::decay_t<decltype(std::declval<Fn>()())>>
class CallableWrapper : public Callable<ReturnType> {
public:
	CallableWrapper() = delete;
	explicit CallableWrapper(Fn&& fn) : fn_(std::forward<Fn>(fn)) {};

	ReturnType& operator()() override {
		execute();
		return result_;
	}

	void execute() override {
		if (!executed_) {
			result_ = fn_();
			executed_ = true;
		}
	}

private:
	Fn fn_;
	ReturnType result_;

	bool executed_ = false;
};

} //sched

#endif //WRAPPER_H_
