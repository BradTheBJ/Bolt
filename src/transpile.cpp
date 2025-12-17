#include <memory>
#include <any>
int main() {

std::cout << "Hello, World" << std::endl;
auto x = 5;
std::cout << x << std::endl;
auto y = 0;
y = x + 5;
std::cout << y << std::endl;
std::cout << "C++ superset test!" << std::endl;;
auto foo = std::make_unique<std::any>( 42);
}

