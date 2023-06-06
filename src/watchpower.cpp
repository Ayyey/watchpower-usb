#include "watchpower.hpp"

namespace mch
{
    WatchPower::WatchPower(InverterUSB &inverter) : m_inverter(inverter) {}
    uint16_t WatchPower::calculateCRC(std::string str)
    {
        uint16_t crc;
        char i;
        crc = 0;
        for (uint16_t i = 0; i < str.length(); i++)
        {
            crc = crc ^ (int16_t)str[i] << 8;
            i = 8;
            do
            {
                if (crc & 0x8000)
                {
                    crc = crc << 1 ^ 0x1021;
                }
                else
                {
                    crc = crc << 1;
                }
            } while (--i);
        }
        return crc;
    }
    void WatchPower::appendCRC(std::string &str)
    {
        uint16_t crc = calculateCRC(str);
        str += char((crc >> 8) & 0xFF);
        str += char(crc & 0xFF);
        str += char(0);
    }
    /* Pivo, ne zabud kupit piva*/
    bool WatchPower::validateCRC(std::string str)
    {
        if (str.length() < 3)
            return false;
        uint16_t crc = calculateCRC(str);
        return char((crc >> 8) & 0xFF) == str[str.length() - 2] &&
               char(crc & 0xFF) == str[str.length() - 1];
    }
    bool WatchPower::isACK(std::string str)
    {
        return str.find("ACK") != -1;
    }
    void WatchPower::updateGeneralStatus()
    {
        m_buffer = m_inverter.sendInquiryMessage(INVERTER_MSG_GENERAL_STATUS_INQYIRY);
#define parseAndAdvance(dest, length)  \
    temp = m_buffer.substr(1, length); \
    dest = stof(temp);                 \
    m_buffer = m_buffer.replace(0, length + 1, "");

        std::string temp;
        parseAndAdvance(gridVoltage, 5);
        parseAndAdvance(gridFreq, 4);
        parseAndAdvance(outputVoltage, 5);
        parseAndAdvance(outputFreq, 4);
        parseAndAdvance(outputPowerApparent, 4);
        parseAndAdvance(outputPowerActive, 4);
        parseAndAdvance(loadPercent, 3);
        parseAndAdvance(busVoltage, 3);
        parseAndAdvance(batteryVoltage, 5);
        parseAndAdvance(batteryCurrent, 3); //documentation says its length is 2, but mine version has lenght of 3
        parseAndAdvance(batteryCapacity, 3);
        parseAndAdvance(temperature, 4);
        parseAndAdvance(solarCurrent, 4);
        parseAndAdvance(solarVoltage, 5);
        parseAndAdvance(batteryVoltageSCC, 5);
        parseAndAdvance(batteryDischargeCurrent, 5);
        temp = m_buffer.substr(1, 8);
        // TODO STATUS PARSING
#undef parseAndAdvance
        for (int i = 0; i < 8; i++)
        {
            status.set(i, temp[i] == '1');
        }
    }
    void WatchPower::updateMode()
    {
        m_buffer = m_inverter.sendInquiryMessage(INVERTER_MSG_MODE_INQUIRY);
        mode = m_buffer[1];
    }
    void WatchPower::updateWarningStatus()
    {
        m_buffer = m_inverter.sendInquiryMessage(INVERTER_MSG_WARNING_STATUS_INQUIRY);
        for (int i = 0; i < 32; i++)
        {
            warning.set(i, m_buffer[i] == '1');
        }
    }
    // void WatchPower::updateSerialNumber() {}
    // void WatchPower::updateFirmwareVerPrimary() {}
    // void WatchPower::updateFirmwareVerSecondary() {}
    // void WatchPower::updateRating(){}
    // void WatchPower::updateFlags() {}
    void WatchPower::refreshData()
    {
        updateMode();
        updateGeneralStatus();
        updateWarningStatus();
    }
    json WatchPower::getInverterParameters()
    {
        json result;
#define assignToJson(value) \
    result[#value] = value;
        assignToJson(gridVoltage);
        assignToJson(gridFreq);
        assignToJson(outputVoltage);
        assignToJson(outputFreq);
        assignToJson(outputPowerApparent);
        assignToJson(outputPowerActive);
        assignToJson(loadPercent);
        assignToJson(busVoltage);
        assignToJson(batteryVoltage);
        assignToJson(batteryCurrent);
        assignToJson(batteryCapacity);
        assignToJson(temperature);
        assignToJson(solarCurrent);
        assignToJson(solarVoltage);
        assignToJson(batteryVoltageSCC);
        assignToJson(batteryDischargeCurrent);
#undef assignToJson

#define mapBit(bit) \
    {#bit, warning[bit] == 1}

        result["warnings"] = {
            {"INVERTERFAULT", warning[1] == 1},
            {"BUSOVER", warning[2] == 1},
            {"BUSUNDER", warning[3] == 1},
            {"BUSSOFTFAIL", warning[4] == 1},
            {"LINEFAIL", warning[5] == 1},
            {"OPVSHORT", warning[6] == 1},
            {"INVETERVOLTAGETOOLOW", warning[7] == 1},
            {"INVERTERVOLTAGETOOHIGH", warning[8] == 1},
            {"OVERTEMPERATURE", warning[9] == 1},
            {"FANLOCKED", warning[10] == 1},
            {"BATTERYVOLTAGEHIGH", warning[11] == 1},
            {"BATTERYLOWALARM", warning[12] == 1},
            {"BATTERYUNDERSHUTDOWN", warning[14] == 1},
            {"OVERLOAD", warning[16] == 1},
            {"EEPROMFAULT", warning[17] == 1},
            {"INVERTEROVERCURRENT", warning[18] == 1},
            {"INVERTSOFTFAIL", warning[19] == 1},
            {"SELFTESTFAIL", warning[20] == 1},
            {"OPDCVOLTAGEOVER", warning[21] == 1},
            {"BATTERYOPEN", warning[22] == 1},
            {"CURRENTSENSORFAIL", warning[23] == 1},
            {"BATTERYSHORT", warning[24] == 1},
            {"POWERLIMIT", warning[25] == 1},
            {"PVVOLTAGEHIGH", warning[26] == 1},
            {"MPPTOVERLOADFAULT", warning[27] == 1},
            {"MPTOVERLOADWARNING", warning[28] == 1},
            {"BATTERYTOOLOWTOCHARGE", warning[29] == 1},
        };
        result["status"] = {
            {"ACCHARGING", status[0] == 1},
            {"SCCCHARGING", status[1] == 1},
            {"CHARGINGSTATUS", status[2] == 1},
            {"LOADSTATUS", status[4] == 1},
            {"SSCVERSION", status[5] == 1},
            {"CONFIG", status[6] == 1},
        };
        return result;
    }
} // namespace mch
