#include <iostream>
#include <thread>
#include <future>
#include <chrono>

using namespace std;

void test_packaged_test( bool do_it ) {
    packaged_task<int()> task( [](){
        this_thread::sleep_for(chrono::seconds(1)); 
        return 7;
    } );
    auto f = task.get_future();
    if ( do_it ) {
        thread worker(move(task));
        worker.join();
        f.get(); // 7
    } else {
        // will not block
    }
}

auto test_async( bool do_it ) {
    auto f = async( launch::async, [](){
        this_thread::sleep_for(chrono::seconds(1)); 
        return 8;
    } );
    if ( do_it ) {
        f.get();
        return f; // 8
    } else {
        // future::~future() will block if f is not captured
        return f;
    }
}

void test_promise() {
    promise<int> p;
    auto f = p.get_future();
    thread( [&p]{ 
        this_thread::sleep_for(chrono::seconds(1)); 
        p.set_value_at_thread_exit(9);
    } ).detach(); // if not detached segfault could occur because p is out of scope
    f.get(); // 9
}

int main() {
    test_packaged_test( true );
    cout << "You can see this after a second\n";
    cout << "\n";

    test_packaged_test( false );
    cout << "You can see this immediately!\n";
    cout << "\n";

    test_async( true  );
    cout << "This will be shown after a second!\n";
    cout << "\n";

    test_async( false );
    cout << "This will be shown after a second!\n";
    cout << "\n";

    auto f = test_async( false );
    f.get();
    cout << "This will be shown after a second!\n";
    cout << "\n";

    test_promise();
    cout << "This will be shown after a second!\n";

    return 0;
}