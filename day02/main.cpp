
#include <iostream>
#include <string>

// 设备基类
class Device {
private:
    std::string name_;
    bool running_ = false;

public:
    explicit Device(const std::string& name)
        : name_(name) {
        std::cout << "Device 构造函数\n";
    }

    virtual ~Device() {
        std::cout << "Device 析构函数\n";
    }

    void start() {
        running_ = true;
        std::cout << name_ << " 已启动\n";
    }

    const std::string& name() const {
        return name_;
    }

    virtual void showData() const {
        std::cout << "普通设备数据\n";
    }
};

// 温度传感器：继承 Device
class TemperatureSensor : public Device {
private:
    double temperature_;

public:
    TemperatureSensor(
        const std::string& name,
        double temperature
    ) : Device(name),
        temperature_(temperature) {
        std::cout << "Sensor 构造函数\n";
    }

    ~TemperatureSensor() override {
        std::cout << "Sensor 析构函数\n";
    }

    void showData() const override {
        std::cout << name()
                  << " 温度："
                  << temperature_
                  << " C\n";
    }
};

int main() {
    TemperatureSensor sensor("传感器A", 26.5);

    Device& device = sensor;

    device.start();
    device.showData();

    return 0;
}
