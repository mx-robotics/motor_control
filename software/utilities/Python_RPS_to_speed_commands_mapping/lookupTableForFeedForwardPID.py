import random as rnd
import matplotlib.pyplot as plt

speed_command = 0

measured_rps = 0


class FFPIDLUTGenerator():
    def __init__(self):
        self.mockRPSValues = []
        self.populate_mock_rps_data_array()

    def _createMockRPSValue(self, speed_cmd):  # speed cmds are assumed to be between 0 and 100
        if 30 > speed_cmd >= -0:
            return speed_cmd * 0.045
        if 30 <= speed_cmd < 60:
            return speed_cmd * 0.055
        if 60 <= speed_cmd <= 75:
            return speed_cmd * 0.062
        if 75 < speed_cmd <= 100:
            return speed_cmd * 0.068

    def populate_mock_rps_data_array(self):
        for i in range(100):
            mock_rps_value = self._createMockRPSValue(i)
            self.mockRPSValues.append(mock_rps_value)

    def constructRPSToSpeedCmdMap(self, rps_list, max_rps_value, min_rps_value):
        granularity = (max_rps_value - min_rps_value) / len(rps_list)
        starting_rps = min_rps_value
        constant_time_rps_map = []
        for i in range(len(rps_list)):
            idx, nearest_value = min(enumerate(rps_list), key=lambda x: abs(x[1] - starting_rps))
            starting_rps += granularity
            constant_time_rps_map.append(idx)
        return constant_time_rps_map

    def get_LUT_index(self, rps, list, maxx, minn):
        res = int((rps -minn) * 100.0 / (maxx - minn))
        print(list[res])


FFPIDLUTGeneratorObj = FFPIDLUTGenerator()
cmd_rps_map = FFPIDLUTGeneratorObj.constructRPSToSpeedCmdMap(FFPIDLUTGeneratorObj.mockRPSValues, 7.00, 0.8)
print(cmd_rps_map)
FFPIDLUTGeneratorObj.get_LUT_index(2.42, cmd_rps_map, 7.00, 0.8)
fig, ax1 = plt.subplots()
ax1.plot(range(0, 100), FFPIDLUTGeneratorObj.mockRPSValues)
plt.show()
