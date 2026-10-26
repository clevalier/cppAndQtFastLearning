
#include <iostream>
#include <memory>
#include <string>

// 1. 传值：修改的是副本
void byValue(int x) {
    x++;
}

// 2. 传引用：修改原始变量
void byReference(int& x) {
    x++;
}

// 3. const 引用：只读取，不修改
void byConstReference(const int& x) {
    std::cout << "只读：" << x << '\n';
}

// 4. 传指针：通过地址修改原始变量
void byPointer(int* x) {
    if (x != nullptr) {
        (*x)++;
    }
}

class Device {
private:
    std::string name_;

public:
    explicit Device(const std::string& name)
        : name_(name) {
        std::cout << "构造：" << name_ << '\n';
    }

    ~Device() {
        std::cout << "析构：" << name_ << '\n';
    }

    void show() const {
        std::cout << "设备：" << name_ << '\n';
    }
};

int main() {
    int value = 10;

    byValue(value);
    std::cout << "传值后：" << value << '\n';

    byReference(value);
    std::cout << "传引用后：" << value << '\n';

    byConstReference(value);

    byPointer(&value);
    std::cout << "传指针后：" << value << '\n';

    std::cout << "----- 普通局部对象 -----\n";

    {
        Device d("传感器A");
        d.show();
    }

    std::cout << "----- 智能指针 -----\n";

    {
        auto d = std::make_unique<Device>("传感器B");
        d->show();
    }

    std::cout << "程序结束\n";
    return 0;
}
