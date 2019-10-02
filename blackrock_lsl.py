from cerebus import cbpy
import pylsl
import time


STREAM_NAME = 'BlackrockTimestamps'
TYPE = 'sync'
CHANNEL_NAMES = ['Timestamps']
SYNC_RATE = 5  # Hz
CHAN_FORMAT = pylsl.cf_int32
SOURCE_ID = 'CEREBUS000'

print("Connecting to Blackrock device...")
conn_params = cbpy.defaultConParams()
cbpy.open(parameter=conn_params)

print("Creating LSL Outlet")
outlet_info = pylsl.StreamInfo(name=STREAM_NAME, type=TYPE, channel_count=len(CHANNEL_NAMES),
                               nominal_srate=SYNC_RATE, channel_format=CHAN_FORMAT, source_id=SOURCE_ID)
outlet_info.desc().append_child_value("manufacturer", "Blackrock")
channels = outlet_info.desc().append_child("channels")
for c in CHANNEL_NAMES:
    channels.append_child("channel").append_child_value("label", c)
outlet = pylsl.StreamOutlet(outlet_info)

print("Sending timestamps...")
try:
    start_time = pylsl.local_clock()
    n_samples = 0
    target_isi = 1 / SYNC_RATE
    while True:
        while (pylsl.local_clock() - start_time) < (n_samples * target_isi):
            time.sleep(0.1/SYNC_RATE)
        cbpy.trial_config(reset=True, noevent=True, nocontinuous=True, nocomment=True)
        res, ts = cbpy.time()
        lsl_stamp = pylsl.local_clock()
        outlet.push_sample([int(ts)], timestamp=lsl_stamp)
        n_samples += 1
except KeyboardInterrupt:
    print("Terminating...")

del outlet
cbpy.close()
