import platform
import struct
import time


import numpy as np
import pylsl
from cerebus import cbpy


STREAM_NAME = 'BlackrockTimestamps'
TYPE = 'sync'
SYNC_RATE = 5  # Hz
SOURCE_ID = 'CEREBUS000'
CHANNEL_NAMES = ['TsHigh32', 'TsLow32']
CHAN_FORMAT = pylsl.cf_int32

print("Connecting to Blackrock device...")
conn_params = cbpy.defaultConParams()
cbpy.open(parameter=conn_params)

print("Creating LSL Outlet")
outlet_info = pylsl.StreamInfo(name=STREAM_NAME, type=TYPE, channel_count=len(CHANNEL_NAMES),
                               nominal_srate=pylsl.IRREGULAR_RATE, channel_format=CHAN_FORMAT, source_id=SOURCE_ID)
outlet_info.desc().append_child_value("manufacturer", "Blackrock")
channels = outlet_info.desc().append_child("channels")
for c in CHANNEL_NAMES:
    channels.append_child("channel").append_child_value("label", c)
outlet = pylsl.StreamOutlet(outlet_info)

print("Sending timestamps...")
start_time = pylsl.local_clock()
n_samples = 0
target_isi = 1 / SYNC_RATE
ts_arr = np.zeros((1,), dtype=np.uint64)
view_32 = ts_arr.view(np.int32)
try:
    while True:
        while (pylsl.local_clock() - start_time) < (n_samples * target_isi):
            time.sleep(0.1/SYNC_RATE)
        cbpy.trial_config(reset=True, noevent=True, nocontinuous=True, nocomment=True)
        res, ts = cbpy.time()
        ts_lsl = pylsl.local_clock()
        ts_arr[0] = ts
        outlet.push_sample(view_32, timestamp=ts_lsl)
        n_samples += 1
except KeyboardInterrupt:
    print("Terminating...")

del outlet
cbpy.close()
