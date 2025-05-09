#include <coroutine>
#include <iostream>

struct TestFutureType {
    struct promise_type;
    using CoHandle = std::coroutine_handle<promise_type>;

    struct promise_type {
        promise_type() {std::cout << "new promise\n";}
        promise_type(promise_type&&) = delete;
        
        auto initial_suspend() {
            std::cout << "init suspend\n";
            return std::suspend_always();
        }
        auto final_suspend() noexcept {
            std::cout << "final suspend\n";
            return std::suspend_always();
        }

        auto get_return_object() {
            std::cout << "get ret obj\n";
            return CoHandle::from_promise(*this);
        }
        void return_void() {std::cout << "ret void\n";}
        void unhandled_exception() {
            std::cout << "unhandled err\n";
            std::terminate();
        }
    };

    TestFutureType(CoHandle ch) {
        std::cout << "new future\n";
        coHandle_ = ch;
    }
    TestFutureType(const TestFutureType&) = delete;
    TestFutureType(TestFutureType&&) = delete;
    ~TestFutureType() {
        std::cout << "del future\n";
    }
    bool resume() {
        if (!coHandle_.done()) {
            coHandle_.resume();
        }
        return !coHandle_.done();
    }
private:
    CoHandle coHandle_;
};

TestFutureType coroTest() {
    std::cout << "start of coroTest\n";
    co_await std::suspend_always();
    std::cout << "end of coroTest\n";
}


int main() {
    TestFutureType fut = coroTest();
    std::cout << "calling resume\n";
    fut.resume();
    std::cout << "main end\n";

    return 0;
}