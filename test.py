import pylsl
import numpy as np


N_SAMPLES = 200
streams = pylsl.resolve_stream('name', 'BlackrockTimestamps')
inlet = pylsl.StreamInlet(streams[0])

lsl_clocks = np.zeros((N_SAMPLES,), dtype=np.float)
pull_data = np.zeros((N_SAMPLES, inlet.channel_count), dtype=np.int32)
ts_data = pull_data.view(np.uint64)
for ix in range(N_SAMPLES):
    sample, timestamp = inlet.pull_sample()
    lsl_clocks[ix] = timestamp
    pull_data[ix] = sample
    print(lsl_clocks[ix], ts_data[ix])


import matplotlib.pyplot as plt
from scipy import stats
slope, intercept, r_value, p_value, std_err = stats.linregress(lsl_clocks, ts_data[:, 0])
print("slope: %f    intercept: %f" % (slope, intercept))
print("r-squared: %f" % r_value**2)
plt.plot(lsl_clocks, ts_data, 'o', label='timestamps')
plt.plot(lsl_clocks, intercept + slope * lsl_clocks, '-', label='fit')
plt.xlabel('LSL Times (s)')
plt.ylabel('Blackrock Timestamps (N)')
plt.legend()
plt.show()
print("Done")
