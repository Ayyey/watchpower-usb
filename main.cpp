#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <libusb.h>
#include "httplib.h"
#include "src/timer.hpp"
#include "src/inverter_usb.hpp"
#include "src/watchpower.hpp"

#define VID 0x0665
#define PID 0x5161

/*
    uint8_t bmRequestType = LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE;
    uint8_t bRequest = LIBUSB_REQUEST_SET_CONFIGURATION;
    uint16_t wIndex;
    uint16_t wLenght;
*/
httplib::Client cli("https://127.0.0.1:8000");
mch::Timer timer;
void logMessage(std::string msg);
int main()
{
    try
    {
        cli.enable_server_certificate_verification(false);
        mch::InverterUSB inverter(VID, PID);
        mch::WatchPower wp(inverter);
        while (true)
        {

            wp.refreshData();
            logMessage("sucessfully updated invertor data!");
            auto parameters = wp.getInverterParameters();
            std::string data = parameters.dump();
            logMessage("successfuly got data! " + data);
            auto res = cli.Post("/data", data, "text/plain");
            if (res)
            {
                if (res->status == 202)
                {
                    logMessage("sucessfully send data!");
                }
            }
            else
            {
                auto err = res.error();
                logMessage("response error: " + httplib::to_string(err));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        }
    }
    catch (std::runtime_error err)
    {
        std::ofstream ofs;
        ofs.open("MCH_INVERTORUSB_ERRRORLOG.txt");
        ofs << err.what();
        ofs.close();
        return 1;
    }
}
void logMessage(std::string msg)
{
    std::cout << " [LOG]\t[" << timer.timeString() << "]\t" << msg << '\n';
}