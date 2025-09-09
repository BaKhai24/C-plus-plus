#include <iostream>
#include <string>
#include <memory>
using namespace std;

class ModeEnergyConsumer {
public:
    virtual float GetEnergyConsumptionRate() const = 0;
    virtual std::string GetNodeName() const = 0;
    virtual ~ModeEnergyConsumer() {}
};

class EcoMode : public ModeEnergyConsumer {
public:
    float GetEnergyConsumptionRate() const override {
        return 0.09; // Example consumption rate in kWh per hour
    }

    std::string GetNodeName() const override {
        return "EcoMode";
    }
};

class normalMode : public ModeEnergyConsumer {
public:
    float GetEnergyConsumptionRate() const override {
        return 0.12; // Example consumption rate in kWh per hour
    }

    std::string GetNodeName() const override {
        return "NormalMode";
    }
};

class sportMode : public ModeEnergyConsumer {
public:
    float GetEnergyConsumptionRate() const override {
        return 0.15; // Example consumption rate in kWh per hour
    }

    std::string GetNodeName() const override {
        return "SportMode";
    }
};


class IPrintTable {
public:
    virtual void PrintBatteryStatus(float CurrentBattery, float BatteryPercentage, float EstimateDistance) const = 0;
    virtual void PrintModeEnergyConsumerInfo(float distance, const ModeEnergyConsumer& mode, float consumption) const = 0;
    virtual void PrintlowBatteryWarning() const = 0;
    virtual ~IPrintTable() {}
};

class StatusPrinter : public IPrintTable {
public:
    void PrintBatteryStatus(float CurrentBattery,float BatteryPercentage, float EstimateDistance) const override {
        std::cout << "Current Battery: " << CurrentBattery << " kWh :: " << BatteryPercentage << "%" << std::endl;
        std::cout << "Estimated Distance: " << EstimateDistance << " km" << std::endl;
    }

    void PrintModeEnergyConsumerInfo(float distance, const ModeEnergyConsumer& mode, float consumption) const override {
        std::cout << "Driving Distance: " << distance << " km" << std::endl;
        std::cout << "Mode: " << mode.GetNodeName() << std::endl;
        std::cout << "Energy Consumption: " << consumption << " kWh" << std::endl;
    }

    void PrintlowBatteryWarning() const override {
        std::cout << "Warning: Battery low!" << std::endl;
    }
};

class Battery {
private:
    float capacity; // in kWh
    float currentCharge; // in kWh
public:
    Battery(float capacity_value) : capacity(capacity_value), currentCharge(capacity_value) {}

    void ConsumedEnergy(float amount) {
        if (amount <= currentCharge) {
            currentCharge -= amount;
        } else {
            currentCharge = 0;
        }
    }

    float GetRemainingBattery() const {
        return currentCharge;
    }

    float GetRemainingBatteryPercentage() const {
        return (currentCharge / capacity) * 100;
    }

    float GetRemainingDistance(float consumptionRate) const {
        if (consumptionRate <= 0) return 0;
        return currentCharge / consumptionRate; // in km
    }

    void Recharge(float amount) {
        currentCharge += amount;
        if (currentCharge > capacity) {
            currentCharge = capacity;
        }
    }

    bool IsBatteryLow() const {
        return GetRemainingBatteryPercentage() < 20.0; // Low if below 20%
    }
};

class VF5_EnergyManagementSystem {
private:
    Battery VF5_Battery;
    std::unique_ptr<ModeEnergyConsumer> VF5_CurrentMode; // Pointer to current energy consumption mode
    std::unique_ptr<IPrintTable> VF5_Printer; // Pointer to printer
public:
    VF5_EnergyManagementSystem(float VF5_Battery_capacity, std::unique_ptr<IPrintTable> printer)
        : VF5_Battery(VF5_Battery_capacity), VF5_CurrentMode(std::make_unique<normalMode>()), VF5_Printer(std::move(printer)) {};
    
    void VF5_Drive(float distance) {
        float VF5_ConsumptionRate = VF5_CurrentMode->GetEnergyConsumptionRate();
        float VF5_EnergyNeeded = (VF5_ConsumptionRate * distance); // kWh needed for the distance
        VF5_Battery.ConsumedEnergy(VF5_EnergyNeeded);
        VF5_Printer->PrintModeEnergyConsumerInfo(distance, *VF5_CurrentMode, VF5_EnergyNeeded);
        VF5_Printer->PrintBatteryStatus(VF5_Battery.GetRemainingBattery(),VF5_Battery.GetRemainingBatteryPercentage(), VF5_Battery.GetRemainingDistance(VF5_ConsumptionRate));
        if (VF5_Battery.IsBatteryLow()) {
            VF5_Printer->PrintlowBatteryWarning();
        }

    }
};

int main() {
    auto VF5_Display = std::make_unique<StatusPrinter>();
    VF5_EnergyManagementSystem VF5_CrimsonRed(38.4, std::move(VF5_Display)); // 38.4 kWh battery capacity
    VF5_CrimsonRed.VF5_Drive(50); // Drive 50 km
    std::cout << "-----------------------------------" << std::endl;
    VF5_CrimsonRed.VF5_Drive(100); // Drive another 100 km
    return 0;
}