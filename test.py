import pylsl
import numpy as np


N_SAMPLES = 200
streams = pylsl.resolve_stream('name', 'BlackrockTimestamps')
inlet = pylsl.StreamInlet(streams[0])

data = np.ones((N_SAMPLES, 2), dtype=np.float)
for ix in range(N_SAMPLES):
    sample, timestamp = inlet.pull_sample()
    data[ix, 0] = timestamp
    data[ix, 1] = sample[0]
    print(data[ix])


import matplotlib.pyplot as plt
from scipy import stats
slope, intercept, r_value, p_value, std_err = stats.linregress(data[:, 0], data[:, 1])
print("slope: %f    intercept: %f" % (slope, intercept))
print("r-squared: %f" % r_value**2)
plt.plot(data[:, 0], data[:, 1], 'o', label='timestamps')
plt.plot(data[:, 0], intercept + slope * data[:, 0], '-', label='fit')
plt.xlabel('LSL Times (s)')
plt.ylabel('Blackrock Timestamps (N)')
plt.legend()
plt.show()
print("Done")
