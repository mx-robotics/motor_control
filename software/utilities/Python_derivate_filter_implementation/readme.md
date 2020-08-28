### Python Derivative Filter Implementation

This python script implements a derivative filter to filter out sensor data noise, uses it on (mock) noisy data and plots everything (noisy data, filtered data and expected data) for visualization.

The filter function takes in a value from a sensor reading and outputs modified/filtered result.  The **only** parameter for tuning the filtering is the `T_f` variable in the filtering class. Keep in mind that, increasing this value causes phase lag and makes the filter sluggish. 