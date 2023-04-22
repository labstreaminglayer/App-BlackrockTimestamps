# BlackrockTimestamps over cbsdk 6.05 Windows

See the top-level [README](#../README.md) for the motivation behind this application.

## Usage

The current version of this application is designed to work with Cerebus / Neuroport firmware 6.05 using cbsdk for Windows targeting that firmware version.

To keep this application's stream compatible with the sister Python script's streams targeting newer firmware with 64-bit timestamps, here we also create a 2-channel 32-bit integer stream. The main difference is that in this stream, because cbsdk 6.05 uses 32-bit timestamps, the first channel value is always 0.

Whether using this application or the Python script, the receiving inlet code or the XDF analysis code should reinterpret the `[n_sample, 2]` int32 chunks as `[n_sample, 1]` uint64 chunks. This is trivial to do in numpy:

`arru64 = arr32.view(np.uint64)`

## Build Instructions

### Prerequisites

* Blackrock Cerebus or NeuroPort Windows Suite 6.05 installed on your system
* An IDE with integrated CMake like Visual Studio or CLion.
  * If using CLion, MSYS2 mingw32 doesn't seem to work with the cbsdkx64.dll. Use MSVC or non msys2 mingw32.
