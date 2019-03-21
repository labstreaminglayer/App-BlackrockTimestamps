# App-BlackrockTimestamps

A Python-based application that streams Blackrock Cerebus/Neuroport timestamps.

# Requirements

* Python 3.5 environment with numpy, PyQt5
    * `conda create -n br_lsl python=3.5`
    * `conda activate br_lsl`
    * If you need to modify cerebus.cbpy and build, you also need Qt and cython
        * `conda install pyqt cython`
    * If you want to run the test then you also need a few more packages
        * `conda install numpy scipy matplotlib`
* [pylsl](https://github.com/labstreaminglayer/liblsl-Python)
    * `pip install pylsl`
* [cerebus.cbpy](https://github.com/dashesy/CereLink)
    * Windows: `pip install https://github.com/dashesy/CereLink/releases/download/7.0/cerebus-0.0.3-cp35-cp35m-win_amd64.whl`
    * MacOS: `pip install https://github.com/dashesy/CereLink/releases/download/7.0/cerebus-0.0.3-cp35-cp35m-macosx_10_6_x86_64.whl`

# Notes

[cerebus API](https://github.com/dashesy/CereLink/blob/master/cerebus/cbpy.pyx)
