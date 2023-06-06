#pragma once
#include "libusb.h"

#include <cstring>
#include <string>
#include <stdexcept>

namespace mch
{
    struct inverterUSBMessage
    {
        const char *msg;
        const int8_t rcvPacketsCount;
    };
    static const inverterUSBMessage INVERTER_MSG_RATING_INQUIRY{"QPIRI\xF8\x54\x0D", 13};
    // static const inverterUSBMessage INVERTER_MSG_FIRMWARE_PRIMARY_INQUIRY{"QVFW\x62\x99", 3};
    // static const inverterUSBMessage INVERTER_MSG_FIRMWARE_SECONDARY_INQUIRY{"QVFW2\xC3\xF5", 3};
    static const inverterUSBMessage INVERTER_MSG_WARNING_STATUS_INQUIRY{"QPIWS\xB4\xDA\x0D", 5};
    static const inverterUSBMessage INVERTER_MSG_GENERAL_STATUS_INQYIRY{"QPIGS\xB7\xA9\x0D", 14};
    static const inverterUSBMessage INVERTER_MSG_MODE_INQUIRY{"QMOD\x49\xC1\x0D", 1};
    // static const inverterUSBMessage INVERTER_MSG_SERIAL_INQUIRY{"QID\xD6\xEA", 3};
    // static const inverterUSBMessage INVERTER_MSG_FLAG_INQUIRY{"QFLAG\x98\x74", 2};

    static const inverterUSBMessage QSID{"QSID\xBB\x05", 1}; // IDK what this message is;
    // QDI 71 1B

    class InverterUSB
    {
    private:
        libusb_context *m_ctx;
        libusb_device_handle *m_handle;
        void checkUSBTransferResult(int code);

    public:
        InverterUSB(int16_t VID, int16_t PID);
        ~InverterUSB();
        std::string sendInquiryMessage(inverterUSBMessage msg);
    };
} // namespace mch
