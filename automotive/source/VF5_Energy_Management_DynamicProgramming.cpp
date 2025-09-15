/*************************************************************************************************/
/*****************************************Header Flies********************************************/
/*************************************************************************************************/
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <unordered_map>

/*************************************************************************************************/
/*******************************Mode Driving impact Energy Consumer*******************************/
/*************************************************************************************************/

/*********************************Abstract Class Mode Driving*************************************/
class ModeEnergyConsumer {
public:
    virtual float GetEnergyConsumptionRate() const = 0;
    virtual std::string GetNodeName() const = 0;
    virtual ~ModeEnergyConsumer() {}
};

/*******************Derived Class is inherited from Abstract Class Mode Driving******************/
class EcoMode : public ModeEnergyConsumer {
public:
    float GetEnergyConsumptionRate() const override {
        return 0.09; /* consumption rate in kWh per hour */
    }

    std::string GetNodeName() const override {
        return "EcoMode";
    }
};

/*******************Derived Class is inherited from Abstract Class Mode Driving******************/
class normalMode : public ModeEnergyConsumer {
public:
    float GetEnergyConsumptionRate() const override {
        return 0.12; /* consumption rate in kWh per hour */
    }

    std::string GetNodeName() const override {
        return "NormalMode";
    }
};

/*******************Derived Class is inherited from Abstract Class Mode Driving******************/
class sportMode : public ModeEnergyConsumer {
public:
    float GetEnergyConsumptionRate() const override {
        return 0.15; /* consumption rate in kWh per hour */
    }

    std::string GetNodeName() const override {
        return "SportMode";
    }
};

/*************************************************************************************************/
/**************************Terrain and Temperature impact Energy Consumer*************************/
/*************************************************************************************************/

/*Scoped: The enumerators are scoped to TerrainType, so you access them like TerrainType::Grass.
Type-safe: Unlike traditional enums, enum class doesn't implicitly convert to int, reducing bugs.
Clearer namespace: Prevents name clashes since the enum values are not in the global scope.*/
enum class TerrainType {
    Flat,
    Uphill,
    Downhill
};

class Environment {
private:
    TerrainType terrain;
    float temperature; /*in Celsius*/
public:
    Environment(TerrainType terrain_type, float temp) : terrain(terrain_type), temperature(temp) {}

    float GetTerrainFactor() const {
        switch (terrain) {
            case TerrainType::Flat:
                return 1.0;
            case TerrainType::Uphill:
                return 1.2; /* Consumes more energy*/ 
            case TerrainType::Downhill:
                return 0.8; /* Consumes less energy */ 
            default:
                return 1.0;
        }
    }

    float GetTemperatureFactor() const {
        if (temperature < 4) {
            return 1.1; /* Cold weather increases consumption */
        } else if (temperature > 46) {
            return 1.05; /* Hot weather slightly increases consumption */ 
        } else {
            return 1.0; /* Optimal temperature */ 
        }
    }

    float TemperatureImpactPercent() const {
        return (GetTemperatureFactor() - 1.0) * 100;
    }

    float TerrainImpactPercent() const {
        return (GetTerrainFactor() - 1.0) * 100;
    }

    float GetOverallFactor() const {
        return GetTerrainFactor() * GetTemperatureFactor();
    }

    std::string GetTerrainName() const {
        switch (terrain) {
            case TerrainType::Flat:
                return "Flat";
            case TerrainType::Uphill:
                return "Uphill";
            case TerrainType::Downhill:
                return "Downhill";
            default:
                return "Unknown";
        }
    }

    float GetTemperature() const {
        return temperature;
    }

};

/*************************************************************************************************/
/**********************************Interface for Printing Table***********************************/
/*************************************************************************************************/
class IPrintTable {
public:
    virtual void PrintBatteryStatus(float CurrentBattery, float BatteryPercentage, float EstimateDistance) const = 0;
    virtual void PrintModeEnergyConsumerInfo(float distance, float speed, const ModeEnergyConsumer& mode, float consumption) const = 0;
    virtual void PrintlowBatteryWarning() const = 0;
    virtual void PrintEnvironent(std::string terr, float terrper, float temp, float tempper) const = 0;
    virtual ~IPrintTable() {}
};

/************************Print Status Energy Management System of VF5*****************************/ 
class StatusPrinter : public IPrintTable {
public:
    void PrintBatteryStatus(float CurrentBattery,float BatteryPercentage, float EstimateDistance) const override {
        std::cout << "🔋 Current Battery: " << CurrentBattery << " kWh :: " << BatteryPercentage << "%" << std::endl;
        std::cout << "🛣️  Estimated Distance: " << EstimateDistance << " km" << std::endl;
    }

    void PrintModeEnergyConsumerInfo(float distance,float speed, const ModeEnergyConsumer& mode, float consumption) const override {
        std::cout << "📏 Driving Distance: " << distance << " km" << " :: " << distance*1000 << " m" << std::endl;
        std::cout << "🚗 Driving Speed: " << speed << " km/h" << std::endl;
        std::cout << "🎯 Driving Mode: " << mode.GetNodeName() << std::endl;
        std::cout << "🔧 Driving Energy Consumption: " << consumption << " kWh" << std::endl;
    }

    void PrintEnvironent(std::string terr, float terrper, float temp, float tempper) const override {
        if(terrper < 0){
            std::cout << "🌍 Terrain: "<<  terr << " | Terrain Impact Decrease Consumption ⬇️: " << terrper << "%" << std::endl;
        }
        else if(terrper > 0){
            std::cout << "🌍 Terrain: "<<  terr << " | Terrain Impact Increase Consumption ⬆️: " << "+" <<  terrper << "%" << std::endl;
        }
        else{
            std::cout << "🌍 Terrain: "<<  terr << " | Terrain NO Impact Consumption" << std::endl;
        }

        if(tempper < 0){
            std::cout << "🌡️  Temperature: "<< temp << "°C | Temperature Impact Decrease Consumption ⬇️: " << tempper << "%" << std::endl;
        }
        else if(tempper > 0){
            std::cout << "🌡️  Temperature: "<< temp << "°C | Temperature Impact Increase Consumption ⬆️: " << "+" << tempper << "%" << std::endl;
        }
        else{
            std::cout << "🌡️  Temperature: "<< temp << "°C | Temperature NO Impact Consumption" << std::endl;
        }
    }

    void PrintlowBatteryWarning() const override {
        std::cout << "⚠️  Warning: Battery low!" << std::endl;
    }
};

/*************************************************************************************************/
/*******************************Battery Management System of VF5**********************************/
/*************************************************************************************************/
class Battery {
private:
    float capacity; /* in kWh */ 
    float currentCharge; /* in kWh */ 
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
        return currentCharge / consumptionRate; /* in km */ 
    }

    void Recharge(float amount) {
        currentCharge += amount;
        if (currentCharge > capacity) {
            currentCharge = capacity;
        }
    }

    bool IsBatteryLow() const {
        return GetRemainingBatteryPercentage() < 20.0; /* Low if below 20% */ 
    }
};

/*************************************************************************************************/
/*********************************Energy Management System of VF5*********************************/
/*************************************************************************************************/
class VF5_EnergyManagementSystem {
private:
    Battery VF5_Battery;
    std::unique_ptr<ModeEnergyConsumer> VF5_CurrentMode; /* Pointer to current energy consumption mode */ 
    std::unique_ptr<IPrintTable> VF5_Printer; /* Pointer to the printer interface */ 
    float TotalDistance =0;
public:
    VF5_EnergyManagementSystem(float VF5_Battery_capacity, std::unique_ptr<IPrintTable> printer)
        : VF5_Battery(VF5_Battery_capacity), VF5_CurrentMode(std::make_unique<normalMode>()), VF5_Printer(std::move(printer)) {};
    
    void VF5_SwitchMode(const std::string& mode) {
        if (mode == "EcoMode") {
            VF5_CurrentMode = std::make_unique<EcoMode>();
        } else if (mode == "NormalMode") {
            VF5_CurrentMode = std::make_unique<normalMode>();
        } else if (mode == "SportMode") {
            VF5_CurrentMode = std::make_unique<sportMode>();
        } else {
            std::cout << "Invalid mode. Staying in current mode." << std::endl;
        }
    }
    
    void VF5_Drive(float SpeedKmH, float SecondsDuration, Environment env = Environment(TerrainType::Flat,25.0)) {
        float Distance = (SpeedKmH / 3600) * SecondsDuration; /* distance in km */ 
        TotalDistance += Distance;
        float VF5_ConsumptionRate = VF5_CurrentMode->GetEnergyConsumptionRate();
        float VF5_EnergyNeeded = (VF5_ConsumptionRate * Distance)*env.GetOverallFactor(); /* kWh needed for the distance */ 
        if (VF5_EnergyNeeded > VF5_Battery.GetRemainingBattery()) {
            std::cout << "❌ Not enough battery to drive " << Distance << " km in " << VF5_CurrentMode->GetNodeName() << " mode." << std::endl;
            return;
        }
        VF5_Battery.ConsumedEnergy(VF5_EnergyNeeded);
        VF5_Printer->PrintEnvironent(env.GetTerrainName(), env.TerrainImpactPercent(), env.GetTemperature(), env.TemperatureImpactPercent());
        VF5_Printer->PrintModeEnergyConsumerInfo(TotalDistance, SpeedKmH, *VF5_CurrentMode, VF5_EnergyNeeded);
        VF5_Printer->PrintBatteryStatus(VF5_Battery.GetRemainingBattery(),VF5_Battery.GetRemainingBatteryPercentage(), VF5_Battery.GetRemainingDistance(VF5_ConsumptionRate));
        if (VF5_Battery.IsBatteryLow()) {
            VF5_Printer->PrintlowBatteryWarning();
        }
    }

    float VF5_GetRemainingBattery() const {
        return VF5_Battery.GetRemainingBattery();
    }
};

/*************************************************************************************************/
/************************************Genetic Algorithm of VF5*************************************/
/*************************************************************************************************/

/* StepBehavior represents a step in the movement or behavior of an individual.*/
struct StepBehavior {
    float Speed;     /* speed at that step, in km/h */
    std::string DrivingMode; /* driving mode: Eco, Normal, Sport */
};

class DynamicProgrammingAlgorithm {
private:
    float BatteryLimit;
    int ListStepLength;
public:
    DynamicProgrammingAlgorithm(float batterylimit, int liststeplength=20)
        : BatteryLimit(batterylimit), ListStepLength(liststeplength) {}

    using StepBehaviorInput = std::vector<StepBehavior>;
    using StateDistance = std::pair<float,std::vector<int>>; /*distance, list index step*/
    
    StateDistance TraceBestState(Environment env, float TimeDuration=60) {
        /*relationship between energy consumption and state distance*/
        std::unordered_map<float,StateDistance> RelationshipConsumptionAndState;
        RelationshipConsumptionAndState[0.0] = {0.0, {}};
        StepBehaviorInput ListStepInput = CreateRandomListStep();
        

    }

private:
    StepBehaviorInput CreateRandomListStep() {
        StepBehaviorInput ListStepInput;
        for(int i = 0; i< ListStepLength; i++) {
            StepBehavior Step;
            Step.Speed = static_cast<float>(rand() % 91 + 30); /* Random speed between 30 and 120 km/h */
            int mode = rand() % 3; /*Random 0 1 2 */
            if (mode == 0) {
                Step.DrivingMode = "EcoMode";
            } else if (mode == 1) {
                Step.DrivingMode = "NormalMode";
            } else {
                Step.DrivingMode = "SportMode";
            }
            ListStepInput.push_back(Step);
        }
        return ListStepInput;
    }

};
    


int main() {
    auto VF5_Display = std::make_unique<StatusPrinter>();
    float InitialBatteryCapacity = 38.4; /* in kWh */
    int CountStrategy = 0;
    VF5_EnergyManagementSystem VF5_CrimsonRed(InitialBatteryCapacity, std::move(VF5_Display)); /* 38.4 kWh battery capacity */ 
    for (int i=1;i<=3;i++){
        Environment env(TerrainType::Flat,25.0);
        if (i==1) {
            VF5_CrimsonRed.VF5_SwitchMode("EcoMode");
        } else if (i==2) {
            VF5_CrimsonRed.VF5_SwitchMode("NormalMode");
        } else {
            VF5_CrimsonRed.VF5_SwitchMode("SportMode");
        }
        VF5_CrimsonRed.VF5_Drive(25,60,env); /* Drive 25 km */ 
        std::cout << "-----------------------------------" << std::endl;
    }
    std::cout << "========================Genetic Algorithm=========================" << std::endl;
    std::cout << "🚀 Using Genetic Algorithm to find the optimal driving strategy..." << std::endl;
    Environment env(TerrainType::Flat,35.0);
    while (VF5_CrimsonRed.VF5_GetRemainingBattery() > (InitialBatteryCapacity * 0.1)) { /* Stop if battery below 10% */
        GeneticAlgorithm VF5_GA((InitialBatteryCapacity * 0.1), 20, 100, 100); /* battery limit, chromosome length, population size, generations */
        GeneticAlgorithm::Chromosome VF5_BestStrategy = VF5_GA.SelectionBestIndividual(env, 0.1); /* mutation rate */
        std::cout << "🎉 Optimal Driving Strategy Found:" << std::endl;
        std::cout << "🏆 Best Fitness Score (Estimated Distance): " << VF5_GA.GetLastBestFitnessScore() << " km" << std::endl;
        for (const auto& step : VF5_BestStrategy) {
            std::cout << "Speed: " << step.Speed << " km/h, Mode: " << step.DrivingMode << std::endl;
        }

        /* Execute the best strategy until battery is depleted */
        std::cout << "🚗 Executing Optimal Driving Strategy..." << std::endl;
        for (const auto& step : VF5_BestStrategy) {
            std::cout << "------------------New Step--------------------" << std::endl;
            VF5_CrimsonRed.VF5_SwitchMode(step.DrivingMode);
            VF5_CrimsonRed.VF5_Drive(step.Speed, 60, env); /* Drive for 1 minute at the given speed and mode */
            std::cout << "------------------End Step--------------------" << std::endl;
        }
        CountStrategy++;
        std::cout << "==================End Strategy " << CountStrategy << " ====================" << std::endl;
    }
    std::cout << "------------------End Genetic Algorithm--------------------" << std::endl;
    return 0;
}