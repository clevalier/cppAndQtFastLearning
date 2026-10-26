
#include <iostream>
#include <memory>
#include <string>
#include <utility>

class Device {
private:
    std::string name_;

public:
    explicit Device(std::string name)
        : name_(std::move(name)) {
        std::cout << "构造：" << name_ << '\n';
    }

    virtual ~Device() {
        std::cout << "析构：" << name_ << '\n';
    }

    virtual void showData() const {
        std::cout << "设备：" << name_ << '\n';
    }

    void start() {
        std::cout << "设备启动\n";
    }
};

class TemperatureSensor : public Device {
private:
    double temperature_;

public:
    TemperatureSensor(std::string name, double temp)
        : Device(std::move(name)),
          temperature_(temp) {}

    void showData() const override {
        std::cout << "温度：" << temperature_ << '\n';
    }
};

// 只读引用：不复制对象
void printDevice(const Device& device) {
    device.showData();
}

int main() {
    // 1. unique_ptr 拥有设备对象
    std::unique_ptr<Device> owner =
        std::make_unique<TemperatureSensor>("设备A", 26.5);

    // 2. 裸指针仅观察对象，不拥有它
    Device* currentDevice = owner.get();

    // 3. 通过 const 引用读取设备信息
    printDevice(*currentDevice);

    // 4. 转移所有权
    auto newOwner = std::move(owner);

    std::cout << "旧 owner 是否为空："
              << std::boolalpha
              << (owner == nullptr) << '\n';

    newOwner->showData();

    // 5. 清除观察指针，再销毁对象
    currentDevice = nullptr;
    newOwner.reset();

    std::cout << "程序结束\n";
    return 0;
}
