
#include <iostream>
#include <string>

class Device {
public:
    std::string name;

    Device(const std::string& n) : name(n) {}

    void show() const {
        std::cout << "设备：" << name << '\n';
    }
};

int main() {
    Device a("温度传感器");
    Device b("压力传感器");

    // 引用绑定 a
    Device& ref = a;

    // 指针指向 a
    Device* ptr = &a;

    std::cout << "第一次：\n";
    ref.show();
    ptr->show();

    // 修改指针，使其指向 b
    ptr = &b;

    std::cout << "第二次：\n";
    ref.show();
    ptr->show();

    return 0;
}
