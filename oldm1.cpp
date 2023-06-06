#include <iostream>
#include <assert.h>
#include <string>
#include <thread>
#include <chrono>

#include <winusb.h>

#include "libusb.h"

#define VID 0x0665
#define PID 0x5161

#define ENDPOINT_IN 0x81
#define ENDPOINT_OUT 0x01

libusb_context *ctx;
libusb_device_handle *hUsb;


void disconnectFromDevice();
bool connectToDevice();
int main1()
{
    libusb_init(&ctx);
    libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
    libusb_set_option(ctx, LIBUSB_OPTION_USE_USBDK);
    hUsb = 0;
    connectToDevice();
    unsigned char buffer[9] = "QPIRI\xF8\x54\x0D";
    std::string result = "";
    std::cout << sizeof(buffer) << '\n';
    int actualLenght;
    uint8_t bmRequiestType = LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_STANDARD | LIBUSB_RECIPIENT_INTERFACE;
    int send_req = libusb_control_transfer(hUsb, 
        bmRequiestType,
        9, 0x200, 0x0, buffer, 8, 5000 
    );
    int repetition = 0;
    do{
        int resp = libusb_interrupt_transfer(hUsb, 0x81, (uint8_t *)buffer, 8, &actualLenght, 500);
        result += (char*)buffer;
        repetition++;
    }while(repetition < 12);
    std::cout << "___RESULT IS " << result << '\n';
    // if(resp != 0)
    // {
    //     std::cout << "____ERRORN" << send_req << "____" << libusb_error_name(send_req) << '\n';
    //     std::cout << "____ERRORN" << resp << "____" << libusb_error_name(resp) << '\n';
    // } else{
    //     std::cout << buffer << "\n";
    // }
    std::cout << "receiveced " << actualLenght << "bytes from device\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    disconnectFromDevice();
    libusb_exit(ctx);
    return 0;
}
bool connectToDevice()
{
    hUsb = libusb_open_device_with_vid_pid(ctx, VID, PID);
    if (hUsb == 0)
    {
        assert("Cannot open usb device" && false);
        return false;
    }
    //libusb_set_auto_detach_kernel_driver(hUsb, 1);
    int res = libusb_claim_interface(hUsb, 0);
    if (res == LIBUSB_ERROR_NOT_FOUND)
    {
        assert("Cannot claim usb device" && false);
        return false;
    }

    std::cout << "succesfully claimed usb device!\n";
    return true;
}
void disconnectFromDevice()
{
    if (hUsb != 0)
    {
        libusb_release_interface(hUsb, 0);
        libusb_close(hUsb);
        hUsb = 0;
    }
}