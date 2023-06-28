#include <thread>
#include <atomic>
#include <cassert>

int main() {
    int data = 0;
    std::atomic<int> flag = {0};

    std::thread release( [&]() {
        assert( data == 0 );
        data = 1;
        flag.store( 1, std::memory_order_release );
    } );

    std::thread acqrel( [&]() {
        int expected = 1;
        while( !flag.compare_exchange_strong( expected, 2, std::memory_order_acq_rel ) ) {
            assert( expected == 0 );
            expected = 1;
        }
    } );

    std::thread acquire( [&]() {
        while( flag.load(std::memory_order_acquire) < 2 );
        assert( data == 1 );
    } );

    release.join();
    acqrel.join();
    acquire.join();

    return 0;
}