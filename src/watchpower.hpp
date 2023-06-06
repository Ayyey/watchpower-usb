
// STATUS BITS
#define STATUS_BIT_ACCHARGINGSTATUSBIT 1
#define STATUS_BIT_SCCCHARGINGSTATUS 2
#define STATUS_BIT_CHARGINGSTATUS 3
#define STATUS_BIT_RESERVED_B3 4
#define STATUS_BIT_LOADSTATUS 5
#define STATUS_BIT_SSCVERSION 6
#define STATUS_BIT_CONFIGSTATUS 7
#define STATUS_BIT_RESERVED_B7 8

// FLAG BITS
#define FLAG_BIT_BUZZER 1
#define FLAG_BIT_OVERLOADBYPASS 2
#define FLAG_BIT_POWERSAVING 3
#define FLAG_BIT_LCDTIMEOUT 4
#define FLAG_BIT_OVERLOADRESTART 5
#define FLAG_BIT_OVERTEMPERATURERESTART 6
#define FLAG_BIT_BACKLIGHT 7
#define FLAG_BIT_ALARM 8
#define FLAG_BIT_FAULTCODERECORD 9

// WARNING ENTRY
#define WARNING_ENTRY_BIT_RESERVED_A0 = 1
#define WARNING_ENTRY_BIT_INVERTERFAULT = 2
#define WARNING_ENTRY_BIT_BUSOVER = 3
#define WARNING_ENTRY_BIT_BUSUNDER = 4
#define WARNING_ENTRY_BIT_BUSSOFTFAIL = 5
#define WARNING_ENTRY_BIT_LINEFAIL = 6
#define WARNING_ENTRY_BIT_OPVSHORT = 7
#define WARNING_ENTRY_BIT_INVERTERVOLTAGETOOLOW = 8
#define WARNING_ENTRY_BIT_INTERVERVOLTAGETOOHIGH = 9
#define WARNING_ENTRY_BIT_OVERTEMPERATURE = 10
#define WARNING_ENTRY_BIT_FANLOCKED = 11
#define WARNING_ENTRY_BIT_BATTERYVOLTAGEHIGH = 12
#define WARNING_ENTRY_BIT_BATTERYLOWALARM = 13
#define WARNING_ENTRY_BIT_RESERVED_A13 = 14
#define WARNING_ENTRY_BIT_BATTERYUNDERSHUTDOWN = 15
#define WARNING_ENTRY_BIT_RESERVED_A15 = 16
#define WARNING_ENTRY_BIT_OVERLOAD = 17
#define WARNING_ENTRY_BIT_EEPROMFAULT = 18
#define WARNING_ENTRY_BIT_INVERTEROVERCURRENT = 19
#define WARNING_ENTRY_BIT_INVERTERSOFTFAIL = 20
#define WARNING_ENTRY_BIT_SELFTESTFAIL = 21
#define WARNING_ENTRY_BIT_OPDCVOLTAGEOVER = 22
#define WARNING_ENTRY_BIT_BATTERYOPEN = 23
#define WARNING_ENTRY_BIT_CURRENTSENSORFAIL = 24
#define WARNING_ENTRY_BIT_BATTERYSHORT = 25
#define WARNING_ENTRY_BIT_POWERLIMIT = 26
#define WARNING_ENTRY_BIT_PVVOLTAGEHIGH = 27
#define WARNING_ENTRY_BIT_MPPTOVERLOADFAULT = 28
#define WARNING_ENTRY_BIT_MPPTOVERLOADWARNING = 29
#define WARNING_ENTRY_BIT_BATTERYTOOLOWTOCHARGE = 30
#define WARNING_ENTRY_BIT_RESERVED_A30 = 31
#define WARNING_ENTRY_BIT_RESERVED_A31 = 32

#pragma once
#include <string>
#include <bitset>

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "inverter_usb.hpp"
namespace mch
{
    class WatchPower
    {
    private:
        InverterUSB &m_inverter;
        std::string m_buffer = "";

        uint16_t calculateCRC(std::string);

        void appendCRC(std::string &);
        bool validateCRC(std::string);

        bool querySolar(const char *);
        bool isACK(std::string);

        void updateGeneralStatus();
        void updateMode();
        void updateWarningStatus();
        // void updateSerialNumber();
        // void updateFirmwareVerPrimary();
        // void updateFirmwareVerSecondary();
        // void updateRating();
        // void updateFlags();

        char mode;

        std::bitset<8> status;
        std::bitset<32> warning;
        // std::bitset<9> flagEntry;
        // float batteryRechargeVoltage,
        //     maxACChargingCurrent,
        //     maxChargingCurrent;

        // BatteryTypes batteryType;
        // OutputSourcePriorities outputSourcePriority;
        // ChargePriorities ChargePriority;

        float gridVoltage,
            gridFreq,
            outputVoltage, outputFreq, outputPowerApparent, outputPowerActive,
            loadPercent,
            busVoltage,
            batteryVoltage, batteryCurrent, batteryCapacity,
            temperature,
            solarCurrent, solarVoltage,
            batteryVoltageSCC,
            batteryDischargeCurrent;

    public:
        WatchPower(InverterUSB &inverter);

        void refreshData();
        
        // void refreshDeviceInfo();
        // void refreshSettings();

        //  json getGeneralStatus();
        //  json getMode();
        //  json getWarningStatus();
        //  xjson getRating();
        //  json getFlags();

        json getInverterParameters();
    };
    enum class BatteryTypes
    {
        AGM,
        Flooded,
        User
    };
    enum class OutputSourcePriorities
    {
        UtilityFirst,
        SolarFirst,
        SBU
    };
    enum class ChargePriorities
    {
        UtilityFirst,
        SolarFirst,
        SolarAndUtility,
        SolarOnly
    };
} // namespace mch
