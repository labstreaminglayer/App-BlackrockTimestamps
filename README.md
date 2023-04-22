# App-BlackrockTimestamps

This repository contains two different applications that do the same thing. Each pulls timestamps from Blackrock Cerebus / Neuroport system, then re-streams them over LSL as data samples. As each LSL data sample is timestamped with an LSL timestamp, this effectively creates LSL-Cerebus timestamp pairs. The user can use these pairs to learn a (rolling) linear transform between the two clock domains.

This approach of re-streaming timestamps only was chosen over re-streaming the data because (a) the data is on the PC anyway, (b) these data tend to be higher bandwidth than most LSL modalities so the cost incurred by re-streaming is great, and (c) XDF is inefficient at storing high-bandwidth data and spike events so the Blackrock native format is preferred for those data types.

See the `cbpy` folder for a Python script that is compatible with Cerebus / Neuroport firmware 7.5.x via CereLink's cbpy.

See the `cbsdk_Win` folder for a C++ application that is compatible with Cerebus / Neuroport firmware via cbsdk, for Windows only.
