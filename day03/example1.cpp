
#include <iostream>

class Device {
public:
    virtual void showData() const {
        std::cout << "普通设备数据\n";
    }

    virtual ~Device() = default;
};

class TemperatureSensor : public Device {
public:
    void showData() const override {
        std::cout << "温度：26.5 C\n";
    }
};

class PressureSensor : public Device {
public:
    void showData() const override {
        std::cout << "压力：0.6 MPa\n";
    }
};

// 使用引用传参
void printByRef(const Device& device) {
    device.showData();
}

// 使用指针传参
void printByPtr(const Device* device) {
    if (device == nullptr) {
        std::cout << "没有连接设备\n";
        return;
    }

    device->showData();
}

int main() {
    TemperatureSensor temp;
    PressureSensor pressure;

    printByRef(temp);
    printByRef(pressure);

    printByPtr(&temp);
    printByPtr(&pressure);

    printByPtr(nullptr);

    return 0;
}
