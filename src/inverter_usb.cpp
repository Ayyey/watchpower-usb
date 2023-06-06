#include "inverter_usb.hpp"
namespace mch
{
    InverterUSB::InverterUSB(int16_t VID, int16_t PID)
    {
        libusb_init(&m_ctx);
        libusb_set_option(m_ctx, LIBUSB_OPTION_USE_USBDK);
        m_handle = libusb_open_device_with_vid_pid(m_ctx, VID, PID);
        if (m_handle == 0)
        {
            throw std::runtime_error("Couldn't create device handle");
        }
    }
    InverterUSB::~InverterUSB()
    {
        if (m_handle != 0)
        {
            libusb_release_interface(m_handle, 0);
            libusb_close(m_handle);
            m_handle = 0;
        }
    }
    std::string InverterUSB::sendInquiryMessage(inverterUSBMessage msg)
    {
        int res = libusb_claim_interface(m_handle, 0);
        if (res != 0)
        {
            std::string errText = "Couldn't claim usb device, error_name: ";
            errText += libusb_error_name(res);
            throw std::runtime_error(errText);
        }

        uint8_t bmRequestType = LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE;
        uint8_t bRequest = LIBUSB_REQUEST_SET_CONFIGURATION;
        uint16_t wValue = 0x200;
        uint16_t wIndex = 0x0;
        uint16_t wLenght = 8;
        char buffer[9];
        std::strcpy(buffer, msg.msg);
        int resp = libusb_control_transfer(m_handle,
                                           bmRequestType, bRequest,
                                           wValue, wIndex, (uint8_t *)buffer, wLenght, 800);
        if (resp != 7)//idk what is this
            checkUSBTransferResult(resp);
        int repitition = 0;
        std::string resultString = "";
        do
        {
            int receivedLenght = 0;
            resp = libusb_interrupt_transfer(m_handle, 0x81, (uint8_t *)buffer, wLenght, &receivedLenght, 800);
            checkUSBTransferResult(resp);
            resultString += buffer;
            repitition++;
        } while (repitition < msg.rcvPacketsCount);
        libusb_release_interface(m_handle, 0);
        return resultString;
    }
    void InverterUSB::checkUSBTransferResult(int respCode)
    {
        if (respCode != 0)
        {
            std::string errText = "USB Transfer FAILED! error_name: ";
            errText += libusb_error_name(respCode);
            throw std::runtime_error(errText);
        }
    }
} // namespace mch
