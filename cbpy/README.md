# BlackrockTimestamps over cbpy

See the top-level [README](#../README.md) for the motivation behind this script.

## Usage

The current version of this script is designed to work with Cerebus / Neuroport firmware 7.5.x. Please check the repo tags for older versions.

With firmware 7.5 came 64-bit uint timestamps. The `pylsl -> liblsl-outlet -> liblsl-inlet -> arbitrary language` path does not have good support for 64-bit integers because their implementation is not consistent on all platforms. That is somewhat moot anyway, because Blackrock timestamps are uint64, and LSL's nominal support for 64-bit integers is signed only.

Therefore, this application creates a 2-channel 32-bit signed integer stream. Whether loading the XDF or processing the stream in real-time, the data is initially received as a `[n_sample, 2]` int32 array and must be reinterpreted as a `[n_sample, 1]` uint64 array. This is trivial to do in numpy:

`arru64 = arr32.view(np.uint64)`

See the accompanying test.py script for a more detailed example.

## Requirements

* Python 3.9 environment with numpy.
    * `conda create -n br_lsl python=3.9 numpy`
    * `conda activate br_lsl`
    * If you need to modify cerebus.cbpy and build, you also need Qt6 and cython
        * `conda install pyqt cython`
    * If you want to run the test script then you also need a few more packages
        * `conda install scipy matplotlib`
* [pylsl](https://github.com/labstreaminglayer/pylsl)
    * `pip install pylsl`
    * On non-Windows platforms you will need to separately install [liblsl](https://github.com/sccn/liblsl/releases). For most users, this is easiest with [conda-forge](https://anaconda.org/conda-forge/liblsl) or [homebrew](https://github.com/labstreaminglayer/homebrew-tap).
* [cerebus.cbpy](https://github.com/CerebusOSS/CereLink)
    * Windows: `pip install https://github.com/CerebusOSS/CereLink/releases/download/v7.5.1b3/cerebus-0.0.5-cp39-cp39-win_amd64.whl`
    * MacOS ARM: `pip install https://github.com/CerebusOSS/CereLink/releases/download/v7.5.1b3/cerebus-0.0.5-cp39-cp39-macosx_11_0_arm64.whl`
    * Linux x64: `pip install https://github.com/CerebusOSS/CereLink/releases/download/v7.5.1b3/cerebus-0.0.5-cp39-cp39-linux_x86_64.whl`

## Notes

[cerebus API](https://github.com/dashesy/CereLink/blob/master/cerebus/cbpy.pyx)
