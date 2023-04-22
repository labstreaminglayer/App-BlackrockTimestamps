# BlackrockTimestamps over cbsdk 6.05 Windows

See the top-level [README](#../README.md) for the motivation behind this application.

## Usage

The current version of this application is designed to work with Cerebus / Neuroport using cbsdk for Windows that ships with Cerebus / NeuroPort Central Suite matching the hardware firmware version.

The application must be uniquely built for each firmware version. For example, if you build it using cbsdk for firmware 6.05, the application will then fail to work on a device running firmware 7.05.

To keep this application's stream compatible with the sister Python script's streams targeting newer firmware with 64-bit timestamps, here we also create a 2-channel 32-bit integer stream.

Whether using this application or the Python script, the receiving inlet code or the XDF analysis code should reinterpret the `[n_sample, 2]` int32 chunks as `[n_sample, 1]` uint64 chunks. This is trivial to do in numpy:

`arru64 = arr32.view(np.uint64)`

## Build Instructions

### Prerequisites

* Blackrock Cerebus or NeuroPort Windows Suite
* An IDE with integrated CMake like Visual Studio or CLion.
  * If using CLion, MSYS2 mingw32 doesn't seem to work with the cbsdkx64.dll. Use MSVC or non msys2 mingw32.
* An internet connection at project config time to download dependencies.

### Step-by-step

* Open this `cbsdk_Win` folder in your IDE. (Tested in CLion).
* If necessary, modify the CMake `CBSDK_ROOT` variable to the cbsdk path.
* Build the application.
* Copy QtCore*.dll and QtXml*.dll from your Central folder (parent to cbsdk) to the build directory.
* Open a Terminal window, cd to the build directory, and run `BlackrockTimestampsLSL.exe -h` to see the available arguments.
* Run `BlackrockTimestampsLSL.exe`, optionally passing any arguments.
