#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cbsdk.h>
#include <lsl_cpp.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <chrono>
#include <thread>
//#include <stdlib.h>

using namespace std::chrono_literals;

static int32_t sample[2] = {0, 0};

void heartbeat_lsl_callback(uint32_t nInstance, const cbSdkPktType type, const void* pEventData, void* pCallbackData)
{
    auto* pOutlet = (lsl::stream_outlet*)pCallbackData;
    auto* pPkt = (cbPKT_SYSHEARTBEAT*)pEventData;
    sample[1] = pPkt->time;
    pOutlet->push_sample(sample);
}

// connect to a Cerebus device and stream its timestamps to LSL
void stream_from_device() {
	try {
		// instantiate sensor
		std::cout << "initializing cbsdk connection..." << std::endl;
        uint32_t nInstance = 0;
        cbSdkResult res = cbSdkOpen(
                nInstance,
                CBSDKCONNECTION_DEFAULT
        );

        cbSdkVersion version;
        res = cbSdkGetVersion(nInstance, &version);

		std::cout << "opening outlet..." << std::endl;
		lsl::stream_info info(
            "BlackrockTimestamps",
            "sync",
            2,
            lsl::IRREGULAR_RATE,
            lsl::cf_int32,
            "CEREBUS000"
        );

		lsl::xml_element manu = info.desc().append_child_value("manufacturer", "Blackrock");
		lsl::xml_element channels = info.desc().append_child("channels");
        channels.append_child("channel").append_child_value("label", "TsHigh32");
        channels.append_child("channel").append_child_value("label", "TsLow32");

		// misc meta-data
		info.desc().append_child("acquisition")
			.append_child_value("manufacturer","Blackrock Neurotech");

		lsl::stream_outlet outlet(info);

		// acquisition loop
        res = cbSdkRegisterCallback(nInstance, CBSDKCALLBACK_SYSHEARTBEAT, heartbeat_lsl_callback, &outlet);

		while (true) {
            std::this_thread::sleep_for(2000ms);
		}

		// cbsdk shut down
	}
	catch(std::exception &e) {
		std::cerr << "Error trying to stream from cbsdk: " << e.what() << std::endl;
	}
}

int main(int argc, char* argv[]) {
	try {
		// for now stream from the 0'th sensor
		stream_from_device();
		return 0;
	}
	catch(std::exception &e) {
		std::cerr << "Error in BlackrockTimestampLSL: " << e.what() << std::endl;
		return 1;
	}
}
