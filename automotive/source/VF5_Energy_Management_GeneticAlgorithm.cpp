/*************************************************************************************************/
/*****************************************Header Flies********************************************/
/*************************************************************************************************/
#include <iostream>
#include <string>
#include <memory>
#include <vector>

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
    float distance =0;
public:
    VF5_EnergyManagementSystem(float VF5_Battery_capacity, std::unique_ptr<IPrintTable> printer)
        : VF5_Battery(VF5_Battery_capacity), VF5_CurrentMode(std::make_unique<normalMode>()), VF5_Printer(std::move(printer)) {};
    
    void VF5_Drive(float SpeedKmH, float SecondsDuration, Environment env = Environment(TerrainType::Flat,25.0)) {
        distance += (SpeedKmH / 3600) * SecondsDuration; /* distance in km */ 
        float VF5_ConsumptionRate = VF5_CurrentMode->GetEnergyConsumptionRate();
        float VF5_EnergyNeeded = (VF5_ConsumptionRate * distance)*env.GetOverallFactor(); /* kWh needed for the distance */ 
        if (SpeedKmH > 80) {
            VF5_CurrentMode = std::make_unique<sportMode>();
        } else if (SpeedKmH < 30) {
            VF5_CurrentMode = std::make_unique<EcoMode>();
        } else {
            VF5_CurrentMode = std::make_unique<normalMode>();
        }
        VF5_Battery.ConsumedEnergy(VF5_EnergyNeeded);
        VF5_Printer->PrintEnvironent(env.GetTerrainName(), env.TerrainImpactPercent(), env.GetTemperature(), env.TemperatureImpactPercent());
        VF5_Printer->PrintModeEnergyConsumerInfo(distance, SpeedKmH, *VF5_CurrentMode, VF5_EnergyNeeded);
        VF5_Printer->PrintBatteryStatus(VF5_Battery.GetRemainingBattery(),VF5_Battery.GetRemainingBatteryPercentage(), VF5_Battery.GetRemainingDistance(VF5_ConsumptionRate));
        if (VF5_Battery.IsBatteryLow()) {
            VF5_Printer->PrintlowBatteryWarning();
        }

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

class GeneticAlgorithm {
private:
    float BatteryLimit;      /* energy limit, can be used to control the consumption */
    int ChromosomeLength;  /* chromosome length (number of genes in each individual) */
    int PopulationLength;  /* population size (number of individuals in the population) */
    int GenerationsLength; /* number of generations to evolve */
    std::unique_ptr<ModeEnergyConsumer> CurrentMode; /* Pointer to current energy consumption mode */ 

public:
    GeneticAlgorithm(float batterylimit, int chromosomelength=20, int populationlength=100, int generationslength=100)
        : BatteryLimit(batterylimit), ChromosomeLength(chromosomelength), PopulationLength(populationlength), GenerationsLength(generationslength) {}
    
    
    /* give a new name (alias) to the std::vector<Step> data type — call it Chromosome 
    A Chromosome is a sequence of steps, i.e. a vector containing multiple steps.
    This is how you represent individuals in a genetic algorithm 
    each individual is a sequence of behaviors or strategies.*/
    /* "using" Increased abstraction: you work with "Chromosome" instead of "vector<Step>" */
    using Chromosome = std::vector<StepBehavior>;

private:
    /* The randomChromosome() function is used to generate a random individual in a genetic algorithm */
    /* An individual might look like this:
    {
        {45, 0}, // 45 km/h in Eco mode
        {90, 1}, // 90 km/h in Sport mode
        {60, 0}, // 60 km/h in Eco mode
        ...
    }*/
    Chromosome CreateRandomIndividual() {
        Chromosome Individual;
        for(int i = 0; i< ChromosomeLength; i++) {
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
            Individual.push_back(Step);
        }
        return Individual;
    }

    /* EvaluateFitnessScore function returns the total distance
    an individual can travel before running out of battery. 
    Individuals that can travel further have higher fitness, i.e. are more fit.*/
    float EvaluateFitnessScore(const Chromosome& IndivitualNeedEvaluate, const Environment& env) {
        float TotalDistance   = 0.0;
        float Distance        = 0.0;
        float ConsumptionRate = 0.0;
        float EnergyNeeded    = 0.0;
        int   SportModeCount  = 0;
        float SportModeRation = 0.0;
        float Penalty         = 0.0;
        Battery BatteryNeedEvaluate(BatteryLimit);

        for (const auto& Step : IndivitualNeedEvaluate) {
            /* distance in km for 1 minute */
            Distance = (Step.Speed / 3600) * 60; 
            /* Get driving mode*/
            if (Step.DrivingMode == "EcoMode") {
                CurrentMode = std::make_unique<EcoMode>();
            } else if (Step.DrivingMode == "NormalMode") {
                CurrentMode = std::make_unique<normalMode>();
            } else {
                CurrentMode = std::make_unique<sportMode>();
                SportModeCount++;
            }
            /* Get comsunption rate */
            ConsumptionRate = CurrentMode->GetEnergyConsumptionRate();
            /* kWh needed for the distance */
            EnergyNeeded = (ConsumptionRate * Distance) * env.GetOverallFactor();

            /*Check remaining energy:
            If the battery is not enough to do this step → stop (break).
            If enough → consume energy and add distance.*/
            if ( EnergyNeeded <= BatteryNeedEvaluate.GetRemainingBattery()) {
                BatteryNeedEvaluate.ConsumedEnergy(EnergyNeeded);
                TotalDistance += Distance;
            } else {
                break; /* Stop if battery is depleted */
            } 
        }
        
        /* Additional penalty if individual uses Sport mode too much. */
        SportModeRation = static_cast<float>(SportModeCount) / static_cast<float>(IndivitualNeedEvaluate.size());
        /* 20% penalty if SportModeRation is greater than 50% */
        if (SportModeRation > 0.5) {
            Penalty = TotalDistance * 0.2; /* 20% penalty */
            TotalDistance -= Penalty;
        }
        /* Fitness score is total distance traveled */
        return TotalDistance;
    }



};


int main() {
    auto VF5_Display = std::make_unique<StatusPrinter>();
    VF5_EnergyManagementSystem VF5_CrimsonRed(38.4, std::move(VF5_Display)); /* 38.4 kWh battery capacity */ 
    for (int i=1;i<=3;i++){
        Environment env(TerrainType::Flat,25.0);
        VF5_CrimsonRed.VF5_Drive(25,60,env); /* Drive 25 km */ 
        std::cout << "-----------------------------------" << std::endl;
    }
    std::cout << "------------------another speed-----------------" << std::endl;
    for (int i=1;i<=3;i++){
        Environment env(TerrainType::Uphill,48.0);
        VF5_CrimsonRed.VF5_Drive(50,60,env); /* Drive 50 km */ 
        std::cout << "-----------------------------------" << std::endl;
    }
    std::cout << "------------------another speed-----------------" << std::endl;
    for (int i=1;i<=3;i++){
        Environment env(TerrainType::Downhill,3.0);
        VF5_CrimsonRed.VF5_Drive(85,300,env); /* Drive 85 km */ 
        std::cout << "-----------------------------------" << std::endl;
    }
    return 0;
}