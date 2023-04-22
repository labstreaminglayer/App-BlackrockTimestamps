#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cbsdk.h>
#include <lsl_cpp.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <chrono>
#include <thread>
#include <cxxopts.hpp>
#include <signal.h>

using namespace std::chrono_literals;


static volatile sig_atomic_t keep_running = 1;
static void sig_handler(int _)
{
    (void)_;
    keep_running = 0;
}


static int32_t sample[2] = {0, 0};
void heartbeat_lsl_callback(uint32_t nInstance, const cbSdkPktType type, const void* pEventData, void* pCallbackData)
{
    auto* pOutlet = (lsl::stream_outlet*)pCallbackData;
    auto* pPkt = (cbPKT_SYSHEARTBEAT*)pEventData;
#ifdef CBSDK_PROTO_4
    memcpy(sample, &pPkt->cbpkt_header.time, sizeof(pPkt->cbpkt_header.time));
#else
    sample[1] = pPkt->time;
#endif  // CBSDK_PROTO_4
    pOutlet->push_sample(sample);
}


int main(int argc, char* argv[]) {
    // cxxopts - Define and parse commandline arguments.
    cxxopts::Options options("BlackrockTimestampsLSL", "Retrieve timestamps from Blackrock hardware and re-stream over LSL outlet.");

    options.add_options()
            ("i,inst_ip", "Instrument IP address", cxxopts::value<std::string>()->default_value(""))
            ("c,client_ip", "Client interface IP address", cxxopts::value<std::string>()->default_value(""))
            ("h,help", "Print usage")
            ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }


    // instantiate connection to device
    std::cout << "initializing cbsdk connection..." << std::endl;
    uint32_t nInstance = 0;
    cbSdkConnection con = cbSdkConnection();
    con.szInIP = result["client_ip"].as<std::string>().c_str();
    con.szOutIP = result["inst_ip"].as<std::string>().c_str();
    cbSdkResult res = cbSdkOpen(
            nInstance,
            CBSDKCONNECTION_DEFAULT,
            con
    );
    if (res != CBSDKRESULT_SUCCESS)
    {
        std::cerr << "cbSdkOpen failed!" << std::endl;
        if (res == CBSDKRESULT_ERROFFLINE)
            std::cerr << "CBSDK device offline!" << std::endl;
        return 1;
    }

    // Test version
    cbSdkVersion version;
    res = cbSdkGetVersion(nInstance, &version);
    if (res == CBSDKRESULT_SUCCESS) {
        std::cout << "cbSdkGetVersion returned " << version.major << "." << version.minor << "." << version.release << std::endl;
    }
    else {
        std::cerr << "cbSdkGetVersion failed! Possible protocol mismatch." << std::endl;
        return 1;
    }

    // Setup LSL Outlet
    std::cout << "opening outlet..." << std::endl;
    lsl::stream_info info(
            "BlackrockTimestamps",
            "sync",
            2,
            lsl::IRREGULAR_RATE,
            lsl::cf_int32,
            "CEREBUS000"
    );
    // Header
    lsl::xml_element channels = info.desc().append_child("channels");
    channels.append_child("channel").append_child_value("label", "TsHigh32");
    channels.append_child("channel").append_child_value("label", "TsLow32");
    info.desc().append_child("acquisition")
            .append_child_value("manufacturer","Blackrock Neurotech");
    // Instantiate outlet
    lsl::stream_outlet outlet(info);

    signal(SIGINT, sig_handler);

	try {
        // Register callback for heartbeat packets
        res = cbSdkRegisterCallback(nInstance, CBSDKCALLBACK_SYSHEARTBEAT, heartbeat_lsl_callback, &outlet);
        // TODO: handle res
        while (keep_running) {
            std::this_thread::sleep_for(500ms);
        }
        res = cbSdkUnRegisterCallback(nInstance, CBSDKCALLBACK_SYSHEARTBEAT);
        // TODO: handle res
        cbSdkClose(nInstance);
	}
	catch(std::exception &e) {
		std::cerr << "Error in BlackrockTimestampLSL: " << e.what() << std::endl;
		return 1;
	}
    return 0;
}
