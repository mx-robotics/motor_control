#include <iostream>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;


struct SPWMDutyCycles{
    uint16_t inDutyCycleW;
    uint16_t inDutyCycleU;
    uint16_t inDutyCycleV;

};

struct ModulationIndexScalingParams{
    constexpr ModulationIndexScalingParams(float m, float c): offsetParam_m(m), offsetParam_c(c){};
    float offsetParam_m;
    float offsetParam_c;

};

constexpr float F_BUS = 60000000;

#define DEG_TO_RAD 0.017453292519943295769236907684886
class LUTGenerator{



public:
    static constexpr uint16_t maxSensorResolution = 16384;
    static constexpr uint8_t motorPoleCount = 11;
    static constexpr uint16_t LUTSize = maxSensorResolution / motorPoleCount;
    static constexpr uint16_t PWM_FREQ = 20000;
    static constexpr int MAX_DUTY_CYCLE = (F_BUS / PWM_FREQ) / 2;


     static constexpr  std::array<int, LUTSize> generate() {

        std::array <int, LUTSize> temp_table{};

        constexpr float modulation_index =1.15470053838;
        uint angle = 0;
        for (int i = 0; i < LUTSize; i++) {

            temp_table[i] = calculateDutyCycleW(angle,modulation_index,MAX_DUTY_CYCLE);

            angle++;
        }

        return temp_table;
    }


     static constexpr int calculateDutyCycleW(uint16_t rotor_position,float modulation_index,uint16_t MAX_DUTY_CYCLE){
         constexpr float angleScaler = 360.0f / LUTSize;
         constexpr float deg_to_radians = 0.017453f;
         uint16_t IN_W_duty_cycle=0;

         float r1 = 0;
         float r2 = 0;
         float t7 = 0;
         float t0 = 0;
         float t1 = 0;
         float t2 = 0;
         float angle = static_cast<float>(rotor_position) * angleScaler;

         if (angle >= 0 && angle < 60) {

             r1 = (sqrt(3) / 2) * modulation_index * sin((60 - angle) * deg_to_radians);
             r2 = (sqrt(3) / 2) * modulation_index * sin((angle) * deg_to_radians);

             t1 = r1 * (MAX_DUTY_CYCLE);
             t2 = r2 * (MAX_DUTY_CYCLE);
             t0 = (MAX_DUTY_CYCLE / 2) *
                  (1 - modulation_index * cos(angle * deg_to_radians) + (modulation_index / 6) * cos(3 * angle * deg_to_radians));
             t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

             IN_W_duty_cycle = (t2 + t7 + t1);
             //IN_U_duty_cycle = t2 + t7;
             //IN_V_duty_cycle = t7;

         } else if (angle >= 60 && angle < 120) {
             r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 60)) * deg_to_radians);
             r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 60) * deg_to_radians);

             t1 = r1 * (MAX_DUTY_CYCLE);
             t2 = r2 * (MAX_DUTY_CYCLE);
             t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 60) * deg_to_radians) +
                                          (modulation_index / 6) * cos(3 * (angle - 60) * deg_to_radians));
             t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

             IN_W_duty_cycle = t1 + t0;
             //IN_U_duty_cycle = t1 + t2 + t0;
             //IN_V_duty_cycle = t0;

         } else if (angle >= 120 && angle < 180) {

             r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 120)) * deg_to_radians);
             r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 120) * deg_to_radians);

             t1 = r1 * (MAX_DUTY_CYCLE);
             t2 = r2 * (MAX_DUTY_CYCLE);
             t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 120) * deg_to_radians) +
                                          (modulation_index / 6) * cos(3 * (angle - 120) * deg_to_radians));
             t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

             IN_W_duty_cycle = t7;
             //IN_U_duty_cycle = t1 + t2 + t7;
             //IN_V_duty_cycle = t2 + t7;


         } else if (angle >= 180 && angle < 240) {
             r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 180)) * deg_to_radians);
             r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 180) * deg_to_radians);

             t1 = r1 * (MAX_DUTY_CYCLE);
             t2 = r2 * (MAX_DUTY_CYCLE);
             t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 180) * deg_to_radians) +
                                          (modulation_index / 6) * cos(3 * (angle - 180) * deg_to_radians));
             t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

             IN_W_duty_cycle = t0;
             //IN_U_duty_cycle = t1 + t0;
             //IN_V_duty_cycle = t1 + t2 + t0;

         } else if (angle >= 240 && angle < 300) {
             r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 240)) * deg_to_radians);
             r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 240) * deg_to_radians);

             t1 = r1 * (MAX_DUTY_CYCLE);
             t2 = r2 * (MAX_DUTY_CYCLE);
             t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 240) * deg_to_radians) +
                                          (modulation_index / 6) * cos(3 * (angle - 240) * deg_to_radians));
             t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

             IN_W_duty_cycle = t2 + t7;
             //IN_U_duty_cycle = t7;
             //IN_V_duty_cycle = t1 + t2 + t7;


         } else if (angle >= 300 && angle < 360) {

             r1 = (sqrt(3) / 2) * modulation_index * sin((60 - (angle - 300)) * deg_to_radians);
             r2 = (sqrt(3) / 2) * modulation_index * sin((angle - 300) * deg_to_radians);

             t1 = r1 * (MAX_DUTY_CYCLE);
             t2 = r2 * (MAX_DUTY_CYCLE);
             t0 = (MAX_DUTY_CYCLE / 2) * (1 - modulation_index * cos((angle - 300) * deg_to_radians) +
                                          (modulation_index / 6) * cos(3 * (angle - 300) * deg_to_radians));
             t7 = MAX_DUTY_CYCLE - (t1 + t2 + t0);

             IN_W_duty_cycle = t1 + t2 + t0;
             //IN_U_duty_cycle = t0;
             //IN_V_duty_cycle = t1 + t0;


         }
         return IN_W_duty_cycle;




    }



    static constexpr  ModulationIndexScalingParams calculateModulationIndexScalingOffsetParameters() {

        uint16_t rotor_position = 200;
        float modulation_index_max = 1.1547005f;
        float IN_W_duty_cycle_max = calculateDutyCycleW(rotor_position, modulation_index_max, MAX_DUTY_CYCLE);

        float scalar1 = 0.4f;
        float modulationIndexNew = scalar1 * modulation_index_max;
        float dutyCycleScaledWithModuloIndex1 = IN_W_duty_cycle_max * scalar1;
        float dutyCycleCalculatedWithModuloIndex1 = calculateDutyCycleW(rotor_position, modulationIndexNew,
                                                                        MAX_DUTY_CYCLE);

        float scalar2 = 0.9f;
        modulationIndexNew = scalar2* modulation_index_max;
        float dutyCycleScaledWithModuloIndex2 = IN_W_duty_cycle_max * scalar2;
        float dutyCycleCalculatedWithModuloIndex2 = calculateDutyCycleW(rotor_position, modulationIndexNew,
                                                                        MAX_DUTY_CYCLE);


        float yy1 = dutyCycleCalculatedWithModuloIndex1 - dutyCycleScaledWithModuloIndex1;
        float yy2 = dutyCycleCalculatedWithModuloIndex2 - dutyCycleScaledWithModuloIndex2;

        float xx1 =  scalar1*100;
        float xx2 =  scalar2*100;

        float m = (yy2 - yy1) / (xx2 - xx1);

        float c = yy2 - (m * xx2);


        ModulationIndexScalingParams params{m, c};
        //ModulationIndexScalingParams params{-6.0f, 600.0f};

        /*Serial.print(", c = ");
        Serial.print(static_cast<int16_t >(params.offsetParam_c));
        Serial.print(", m = ");
        Serial.println(static_cast<int16_t >(floorf(params.offsetParam_m )));
*/


        return params;

    }




};

void calculateScaledLUTvsOnlineCalculatedDifference(int rotor_position, float modulation_index,const  std::array<int, 1489> &LUT){
    constexpr float modulation_indexFULL =1.15470053838;
    float scalar = modulation_index/modulation_indexFULL;
    constexpr auto params = LUTGenerator::calculateModulationIndexScalingOffsetParameters();
    int dutyCyleWCalculated = LUTGenerator::calculateDutyCycleW(rotor_position,modulation_index,LUTGenerator::MAX_DUTY_CYCLE);
    int dutyCyleScaled = LUT[rotor_position] * scalar + params.offsetParam_c + params.offsetParam_m * (scalar  *100);
    int diff = dutyCyleScaled - dutyCyleWCalculated;
    assert(abs(diff) < 3);


}

void testSVPWMScaling(const  std::array<int, 1489> &LUT){
    constexpr float modulation_indexFULL =1.15470053838;
    for (float modulation_index = 0; modulation_index <= modulation_indexFULL; modulation_index+=0.001) {
        for(int rotor_pos = 0; rotor_pos<1489; rotor_pos++){
            calculateScaledLUTvsOnlineCalculatedDifference(rotor_pos,modulation_index,LUT);

        }

    }
    std::cout << "SVPWM scaling tested, max. difference is under 3" << std::endl;
}

auto constexpr generateOtherPhasesFromLUT(const  std::array<int,1489> LUT, int phaseShift){
    std::array<int,1489> temp{};
    int LUTShift = static_cast<int>((phaseShift * 1489) /360.0f );
    for (int i = 0; i < 1489; ++i) {
        temp[i]=LUT[(i+LUTShift)%1489];
    }
    return temp;


}


int main() {
    std::array <int,1489> printArray1 {1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1473, 1473, 1473, 1473, 1473, 1473, 1474, 1474, 1474, 1474, 1474, 1475, 1475, 1475, 1475, 1475, 1476, 1476, 1476, 1476, 1477, 1477, 1477, 1478, 1478, 1478, 1478, 1479, 1479, 1479, 1480, 1480, 1480, 1481, 1481, 1481, 1482, 1482, 1482, 1483, 1483, 1483, 1484, 1484, 1484, 1485, 1485, 1485, 1486, 1486, 1486, 1487, 1487, 1487, 1488, 1488, 1488, 1489, 1489, 1490, 1490, 1490, 1491, 1491, 1491, 1492, 1492, 1492, 1493, 1493, 1493, 1493, 1494, 1494, 1494, 1495, 1495, 1495, 1495, 1496, 1496, 1496, 1496, 1497, 1497, 1497, 1497, 1498, 1498, 1498, 1498, 1498, 1498, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1500, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1498, 1498, 1498, 1498, 1498, 1497, 1497, 1497, 1497, 1496, 1496, 1495, 1495, 1495, 1494, 1494, 1493, 1493, 1492, 1492, 1491, 1491, 1490, 1490, 1489, 1488, 1488, 1487, 1486, 1485, 1485, 1484, 1483, 1482, 1481, 1480, 1480, 1479, 1478, 1477, 1476, 1475, 1474, 1472, 1471, 1470, 1469, 1468, 1467, 1465, 1464, 1463, 1461, 1460, 1459, 1457, 1456, 1454, 1453, 1451, 1450, 1448, 1447, 1445, 1443, 1442, 1440, 1438, 1436, 1435, 1433, 1431, 1429, 1427, 1425, 1423, 1421, 1419, 1417, 1415, 1413, 1410, 1408, 1406, 1404, 1401, 1399, 1397, 1394, 1392, 1389, 1387, 1384, 1382, 1379, 1377, 1374, 1371, 1369, 1366, 1363, 1360, 1357, 1355, 1352, 1349, 1346, 1343, 1340, 1337, 1334, 1331, 1327, 1324, 1321, 1318, 1314, 1311, 1308, 1304, 1301, 1298, 1294, 1291, 1287, 1284, 1280, 1277, 1273, 1269, 1266, 1262, 1258, 1254, 1251, 1247, 1243, 1239, 1235, 1231, 1227, 1223, 1219, 1215, 1211, 1207, 1203, 1199, 1194, 1190, 1186, 1182, 1177, 1173, 1169, 1164, 1160, 1155, 1151, 1147, 1142, 1138, 1133, 1128, 1124, 1119, 1115, 1110, 1105, 1101, 1096, 1091, 1086, 1082, 1077, 1072, 1067, 1062, 1057, 1052, 1047, 1042, 1038, 1033, 1028, 1023, 1017, 1012, 1007, 1002, 997, 992, 987, 982, 977, 971, 966, 961, 956, 951, 945, 940, 935, 930, 924, 919, 914, 908, 903, 898, 892, 887, 882, 876, 871, 865, 860, 855, 849, 844, 838, 833, 827, 822, 817, 811, 806, 800, 795, 789, 784, 778, 773, 767, 762, 756, 751, 745, 740, 734, 729, 723, 718, 713, 707, 702, 696, 691, 685, 680, 674, 669, 663, 658, 653, 647, 642, 636, 631, 625, 620, 615, 609, 604, 599, 593, 588, 583, 577, 572, 567, 561, 556, 551, 546, 541, 535, 530, 525, 520, 515, 509, 504, 499, 494, 489, 484, 479, 474, 469, 464, 459, 454, 449, 444, 439, 434, 430, 425, 420, 415, 410, 406, 401, 396, 391, 387, 382, 377, 373, 368, 364, 359, 355, 350, 346, 341, 337, 333, 328, 324, 320, 315, 311, 307, 303, 298, 294, 290, 286, 282, 278, 274, 270, 266, 262, 258, 254, 250, 247, 243, 239, 235, 232, 228, 224, 221, 217, 213, 210, 206, 203, 200, 196, 193, 189, 186, 183, 180, 176, 173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 140, 137, 135, 132, 129, 126, 124, 121, 119, 116, 113, 111, 108, 106, 104, 101, 99, 97, 94, 92, 90, 88, 85, 83, 81, 79, 77, 75, 73, 71, 69, 67, 65, 64, 62, 60, 58, 57, 55, 53, 52, 50, 48, 47, 45, 44, 42, 41, 40, 38, 37, 36, 34, 33, 32, 31, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 16, 15, 14, 13, 12, 12, 11, 10, 10, 9, 9, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 26, 26, 26, 26, 26, 26, 25, 25, 25, 25, 25, 24, 24, 24, 24, 23, 23, 23, 23, 22, 22, 22, 22, 21, 21, 21, 20, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 16, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 40, 41, 42, 44, 45, 47, 48, 50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 88, 90, 92, 94, 96, 99, 101, 104, 106, 108, 111, 113, 116, 118, 121, 124, 126, 129, 132, 135, 137, 140, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176, 180, 183, 186, 189, 193, 196, 200, 203, 207, 210, 214, 217, 221, 224, 228, 232, 235, 239, 243, 247, 250, 254, 258, 262, 266, 270, 274, 278, 282, 286, 290, 294, 298, 303, 307, 311, 315, 320, 324, 328, 333, 337, 341, 346, 350, 355, 359, 364, 368, 373, 377, 382, 387, 391, 396, 401, 406, 410, 415, 420, 425, 430, 434, 439, 444, 449, 454, 459, 464, 469, 474, 479, 484, 489, 494, 499, 504, 509, 515, 520, 525, 530, 535, 541, 546, 551, 556, 562, 567, 572, 577, 583, 588, 593, 599, 604, 609, 615, 620, 626, 631, 636, 642, 647, 653, 658, 663, 669, 674, 680, 685, 691, 696, 702, 707, 713, 718, 723, 729, 734, 740, 745, 751, 756, 762, 767, 773, 778, 784, 789, 795, 800, 806, 811, 817, 822, 827, 833, 838, 844, 849, 855, 860, 865, 871, 876, 882, 887, 892, 898, 903, 908, 914, 919, 924, 930, 935, 940, 945, 951, 956, 961, 966, 971, 977, 982, 987, 992, 997, 1002, 1007, 1012, 1018, 1023, 1028, 1033, 1038, 1043, 1047, 1052, 1057, 1062, 1067, 1072, 1077, 1082, 1086, 1091, 1096, 1101, 1105, 1110, 1115, 1119, 1124, 1128, 1133, 1138, 1142, 1147, 1151, 1155, 1160, 1164, 1169, 1173, 1177, 1182, 1186, 1190, 1194, 1199, 1203, 1207, 1211, 1215, 1219, 1223, 1227, 1231, 1235, 1239, 1243, 1247, 1251, 1254, 1258, 1262, 1266, 1269, 1273, 1277, 1280, 1284, 1287, 1291, 1294, 1298, 1301, 1305, 1308, 1311, 1315, 1318, 1321, 1324, 1327, 1331, 1334, 1337, 1340, 1343, 1346, 1349, 1352, 1355, 1357, 1360, 1363, 1366, 1369, 1371, 1374, 1377, 1379, 1382, 1384, 1387, 1389, 1392, 1394, 1397, 1399, 1401, 1404, 1406, 1408, 1410, 1413, 1415, 1417, 1419, 1421, 1423, 1425, 1427, 1429, 1431, 1433, 1435, 1436, 1438, 1440, 1442, 1443, 1445, 1447, 1448, 1450, 1451, 1453, 1454, 1456, 1457, 1459, 1460, 1461, 1463, 1464, 1465, 1467, 1468, 1469, 1470, 1471, 1472, 1474, 1475, 1476, 1477, 1478, 1479, 1480, 1480, 1481, 1482, 1483, 1484, 1485, 1485, 1486, 1487, 1488, 1488, 1489, 1490, 1490, 1491, 1491, 1492, 1492, 1493, 1493, 1494, 1494, 1495, 1495, 1495, 1496, 1496, 1496, 1497, 1497, 1497, 1498, 1498, 1498, 1498, 1498, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1498, 1498, 1498, 1498, 1498, 1498, 1497, 1497, 1497, 1497, 1496, 1496, 1496, 1496, 1495, 1495, 1495, 1495, 1494, 1494, 1494, 1493, 1493, 1493, 1492, 1492, 1492, 1492, 1491, 1491, 1491, 1490, 1490, 1490, 1489, 1489, 1488, 1488, 1488, 1487, 1487, 1487, 1486, 1486, 1486, 1485, 1485, 1485, 1484, 1484, 1484, 1483, 1483, 1483, 1482, 1482, 1482, 1481, 1481, 1481, 1480, 1480, 1480, 1479, 1479, 1479, 1478, 1478, 1478, 1478, 1477, 1477, 1477, 1476, 1476, 1476, 1476, 1475, 1475, 1475, 1475, 1475, 1474, 1474, 1474, 1474, 1474, 1473, 1473, 1473, 1473, 1473, 1473, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471};
    std::array <int,1489> printArray2 { 173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 140, 137, 135, 132, 129, 126, 124, 121, 119, 116, 113, 111, 108, 106, 104, 101, 99, 97, 94, 92, 90, 88, 85, 83, 81, 79, 77, 75, 73, 71, 69, 67, 65, 64, 62, 60, 58, 57, 55, 53, 52, 50, 48, 47, 45, 44, 42, 41, 40, 38, 37, 36, 34, 33, 32, 31, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 16, 15, 14, 13, 12, 12, 11, 10, 10, 9, 9, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 26, 26, 26, 26, 26, 26, 25, 25, 25, 25, 25, 24, 24, 24, 24, 23, 23, 23, 23, 22, 22, 22, 22, 21, 21, 21, 20, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 16, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 40, 41, 42, 44, 45, 47, 48, 50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 88, 90, 92, 94, 96, 99, 101, 104, 106, 108, 111, 113, 116, 118, 121, 124, 126, 129, 132, 135, 137, 140, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176, 180, 183, 186, 189, 193, 196, 200, 203, 207, 210, 214, 217, 221, 224, 228, 232, 235, 239, 243, 247, 250, 254, 258, 262, 266, 270, 274, 278, 282, 286, 290, 294, 298, 303, 307, 311, 315, 320, 324, 328, 333, 337, 341, 346, 350, 355, 359, 364, 368, 373, 377, 382, 387, 391, 396, 401, 406, 410, 415, 420, 425, 430, 434, 439, 444, 449, 454, 459, 464, 469, 474, 479, 484, 489, 494, 499, 504, 509, 515, 520, 525, 530, 535, 541, 546, 551, 556, 562, 567, 572, 577, 583, 588, 593, 599, 604, 609, 615, 620, 626, 631, 636, 642, 647, 653, 658, 663, 669, 674, 680, 685, 691, 696, 702, 707, 713, 718, 723, 729, 734, 740, 745, 751, 756, 762, 767, 773, 778, 784, 789, 795, 800, 806, 811, 817, 822, 827, 833, 838, 844, 849, 855, 860, 865, 871, 876, 882, 887, 892, 898, 903, 908, 914, 919, 924, 930, 935, 940, 945, 951, 956, 961, 966, 971, 977, 982, 987, 992, 997, 1002, 1007, 1012, 1018, 1023, 1028, 1033, 1038, 1043, 1047, 1052, 1057, 1062, 1067, 1072, 1077, 1082, 1086, 1091, 1096, 1101, 1105, 1110, 1115, 1119, 1124, 1128, 1133, 1138, 1142, 1147, 1151, 1155, 1160, 1164, 1169, 1173, 1177, 1182, 1186, 1190, 1194, 1199, 1203, 1207, 1211, 1215, 1219, 1223, 1227, 1231, 1235, 1239, 1243, 1247, 1251, 1254, 1258, 1262, 1266, 1269, 1273, 1277, 1280, 1284, 1287, 1291, 1294, 1298, 1301, 1305, 1308, 1311, 1315, 1318, 1321, 1324, 1327, 1331, 1334, 1337, 1340, 1343, 1346, 1349, 1352, 1355, 1357, 1360, 1363, 1366, 1369, 1371, 1374, 1377, 1379, 1382, 1384, 1387, 1389, 1392, 1394, 1397, 1399, 1401, 1404, 1406, 1408, 1410, 1413, 1415, 1417, 1419, 1421, 1423, 1425, 1427, 1429, 1431, 1433, 1435, 1436, 1438, 1440, 1442, 1443, 1445, 1447, 1448, 1450, 1451, 1453, 1454, 1456, 1457, 1459, 1460, 1461, 1463, 1464, 1465, 1467, 1468, 1469, 1470, 1471, 1472, 1474, 1475, 1476, 1477, 1478, 1479, 1480, 1480, 1481, 1482, 1483, 1484, 1485, 1485, 1486, 1487, 1488, 1488, 1489, 1490, 1490, 1491, 1491, 1492, 1492, 1493, 1493, 1494, 1494, 1495, 1495, 1495, 1496, 1496, 1496, 1497, 1497, 1497, 1498, 1498, 1498, 1498, 1498, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1498, 1498, 1498, 1498, 1498, 1498, 1497, 1497, 1497, 1497, 1496, 1496, 1496, 1496, 1495, 1495, 1495, 1495, 1494, 1494, 1494, 1493, 1493, 1493, 1492, 1492, 1492, 1492, 1491, 1491, 1491, 1490, 1490, 1490, 1489, 1489, 1488, 1488, 1488, 1487, 1487, 1487, 1486, 1486, 1486, 1485, 1485, 1485, 1484, 1484, 1484, 1483, 1483, 1483, 1482, 1482, 1482, 1481, 1481, 1481, 1480, 1480, 1480, 1479, 1479, 1479, 1478, 1478, 1478, 1478, 1477, 1477, 1477, 1476, 1476, 1476, 1476, 1475, 1475, 1475, 1475, 1475, 1474, 1474, 1474, 1474, 1474, 1473, 1473, 1473, 1473, 1473, 1473, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1473, 1473, 1473, 1473, 1473, 1473, 1474, 1474, 1474, 1474, 1474, 1475, 1475, 1475, 1475, 1475, 1476, 1476, 1476, 1476, 1477, 1477, 1477, 1478, 1478, 1478, 1478, 1479, 1479, 1479, 1480, 1480, 1480, 1481, 1481, 1481, 1482, 1482, 1482, 1483, 1483, 1483, 1484, 1484, 1484, 1485, 1485, 1485, 1486, 1486, 1486, 1487, 1487, 1487, 1488, 1488, 1488, 1489, 1489, 1490, 1490, 1490, 1491, 1491, 1491, 1492, 1492, 1492, 1493, 1493, 1493, 1493, 1494, 1494, 1494, 1495, 1495, 1495, 1495, 1496, 1496, 1496, 1496, 1497, 1497, 1497, 1497, 1498, 1498, 1498, 1498, 1498, 1498, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1500, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1498, 1498, 1498, 1498, 1498, 1497, 1497, 1497, 1497, 1496, 1496, 1495, 1495, 1495, 1494, 1494, 1493, 1493, 1492, 1492, 1491, 1491, 1490, 1490, 1489, 1488, 1488, 1487, 1486, 1485, 1485, 1484, 1483, 1482, 1481, 1480, 1480, 1479, 1478, 1477, 1476, 1475, 1474, 1472, 1471, 1470, 1469, 1468, 1467, 1465, 1464, 1463, 1461, 1460, 1459, 1457, 1456, 1454, 1453, 1451, 1450, 1448, 1447, 1445, 1443, 1442, 1440, 1438, 1436, 1435, 1433, 1431, 1429, 1427, 1425, 1423, 1421, 1419, 1417, 1415, 1413, 1410, 1408, 1406, 1404, 1401, 1399, 1397, 1394, 1392, 1389, 1387, 1384, 1382, 1379, 1377, 1374, 1371, 1369, 1366, 1363, 1360, 1357, 1355, 1352, 1349, 1346, 1343, 1340, 1337, 1334, 1331, 1327, 1324, 1321, 1318, 1314, 1311, 1308, 1304, 1301, 1298, 1294, 1291, 1287, 1284, 1280, 1277, 1273, 1269, 1266, 1262, 1258, 1254, 1251, 1247, 1243, 1239, 1235, 1231, 1227, 1223, 1219, 1215, 1211, 1207, 1203, 1199, 1194, 1190, 1186, 1182, 1177, 1173, 1169, 1164, 1160, 1155, 1151, 1147, 1142, 1138, 1133, 1128, 1124, 1119, 1115, 1110, 1105, 1101, 1096, 1091, 1086, 1082, 1077, 1072, 1067, 1062, 1057, 1052, 1047, 1042, 1038, 1033, 1028, 1023, 1017, 1012, 1007, 1002, 997, 992, 987, 982, 977, 971, 966, 961, 956, 951, 945, 940, 935, 930, 924, 919, 914, 908, 903, 898, 892, 887, 882, 876, 871, 865, 860, 855, 849, 844, 838, 833, 827, 822, 817, 811, 806, 800, 795, 789, 784, 778, 773, 767, 762, 756, 751, 745, 740, 734, 729, 723, 718, 713, 707, 702, 696, 691, 685, 680, 674, 669, 663, 658, 653, 647, 642, 636, 631, 625, 620, 615, 609, 604, 599, 593, 588, 583, 577, 572, 567, 561, 556, 551, 546, 541, 535, 530, 525, 520, 515, 509, 504, 499, 494, 489, 484, 479, 474, 469, 464, 459, 454, 449, 444, 439, 434, 430, 425, 420, 415, 410, 406, 401, 396, 391, 387, 382, 377, 373, 368, 364, 359, 355, 350, 346, 341, 337, 333, 328, 324, 320, 315, 311, 307, 303, 298, 294, 290, 286, 282, 278, 274, 270, 266, 262, 258, 254, 250, 247, 243, 239, 235, 232, 228, 224, 221, 217, 213, 210, 206, 203, 200, 196, 193, 189, 186, 183, 180, 176};
    std::array <int,1489> printArray3 {170, 173, 176, 180, 183, 186, 189, 193, 196, 200, 203, 207, 210, 214, 217, 221, 224, 228, 232, 235, 239, 243, 247, 250, 254, 258, 262, 266, 270, 274, 278, 282, 286, 290, 294, 298, 303, 307, 311, 315, 320, 324, 328, 333, 337, 341, 346, 350, 355, 359, 364, 368, 373, 377, 382, 387, 391, 396, 401, 406, 410, 415, 420, 425, 430, 434, 439, 444, 449, 454, 459, 464, 469, 474, 479, 484, 489, 494, 499, 504, 509, 515, 520, 525, 530, 535, 541, 546, 551, 556, 562, 567, 572, 577, 583, 588, 593, 599, 604, 609, 615, 620, 626, 631, 636, 642, 647, 653, 658, 663, 669, 674, 680, 685, 691, 696, 702, 707, 713, 718, 723, 729, 734, 740, 745, 751, 756, 762, 767, 773, 778, 784, 789, 795, 800, 806, 811, 817, 822, 827, 833, 838, 844, 849, 855, 860, 865, 871, 876, 882, 887, 892, 898, 903, 908, 914, 919, 924, 930, 935, 940, 945, 951, 956, 961, 966, 971, 977, 982, 987, 992, 997, 1002, 1007, 1012, 1018, 1023, 1028, 1033, 1038, 1043, 1047, 1052, 1057, 1062, 1067, 1072, 1077, 1082, 1086, 1091, 1096, 1101, 1105, 1110, 1115, 1119, 1124, 1128, 1133, 1138, 1142, 1147, 1151, 1155, 1160, 1164, 1169, 1173, 1177, 1182, 1186, 1190, 1194, 1199, 1203, 1207, 1211, 1215, 1219, 1223, 1227, 1231, 1235, 1239, 1243, 1247, 1251, 1254, 1258, 1262, 1266, 1269, 1273, 1277, 1280, 1284, 1287, 1291, 1294, 1298, 1301, 1305, 1308, 1311, 1315, 1318, 1321, 1324, 1327, 1331, 1334, 1337, 1340, 1343, 1346, 1349, 1352, 1355, 1357, 1360, 1363, 1366, 1369, 1371, 1374, 1377, 1379, 1382, 1384, 1387, 1389, 1392, 1394, 1397, 1399, 1401, 1404, 1406, 1408, 1410, 1413, 1415, 1417, 1419, 1421, 1423, 1425, 1427, 1429, 1431, 1433, 1435, 1436, 1438, 1440, 1442, 1443, 1445, 1447, 1448, 1450, 1451, 1453, 1454, 1456, 1457, 1459, 1460, 1461, 1463, 1464, 1465, 1467, 1468, 1469, 1470, 1471, 1472, 1474, 1475, 1476, 1477, 1478, 1479, 1480, 1480, 1481, 1482, 1483, 1484, 1485, 1485, 1486, 1487, 1488, 1488, 1489, 1490, 1490, 1491, 1491, 1492, 1492, 1493, 1493, 1494, 1494, 1495, 1495, 1495, 1496, 1496, 1496, 1497, 1497, 1497, 1498, 1498, 1498, 1498, 1498, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1498, 1498, 1498, 1498, 1498, 1498, 1497, 1497, 1497, 1497, 1496, 1496, 1496, 1496, 1495, 1495, 1495, 1495, 1494, 1494, 1494, 1493, 1493, 1493, 1492, 1492, 1492, 1492, 1491, 1491, 1491, 1490, 1490, 1490, 1489, 1489, 1488, 1488, 1488, 1487, 1487, 1487, 1486, 1486, 1486, 1485, 1485, 1485, 1484, 1484, 1484, 1483, 1483, 1483, 1482, 1482, 1482, 1481, 1481, 1481, 1480, 1480, 1480, 1479, 1479, 1479, 1478, 1478, 1478, 1478, 1477, 1477, 1477, 1476, 1476, 1476, 1476, 1475, 1475, 1475, 1475, 1475, 1474, 1474, 1474, 1474, 1474, 1473, 1473, 1473, 1473, 1473, 1473, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1471, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1472, 1473, 1473, 1473, 1473, 1473, 1473, 1474, 1474, 1474, 1474, 1474, 1475, 1475, 1475, 1475, 1475, 1476, 1476, 1476, 1476, 1477, 1477, 1477, 1478, 1478, 1478, 1478, 1479, 1479, 1479, 1480, 1480, 1480, 1481, 1481, 1481, 1482, 1482, 1482, 1483, 1483, 1483, 1484, 1484, 1484, 1485, 1485, 1485, 1486, 1486, 1486, 1487, 1487, 1487, 1488, 1488, 1488, 1489, 1489, 1490, 1490, 1490, 1491, 1491, 1491, 1492, 1492, 1492, 1493, 1493, 1493, 1493, 1494, 1494, 1494, 1495, 1495, 1495, 1495, 1496, 1496, 1496, 1496, 1497, 1497, 1497, 1497, 1498, 1498, 1498, 1498, 1498, 1498, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1500, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1499, 1498, 1498, 1498, 1498, 1498, 1497, 1497, 1497, 1497, 1496, 1496, 1495, 1495, 1495, 1494, 1494, 1493, 1493, 1492, 1492, 1491, 1491, 1490, 1490, 1489, 1488, 1488, 1487, 1486, 1485, 1485, 1484, 1483, 1482, 1481, 1480, 1480, 1479, 1478, 1477, 1476, 1475, 1474, 1472, 1471, 1470, 1469, 1468, 1467, 1465, 1464, 1463, 1461, 1460, 1459, 1457, 1456, 1454, 1453, 1451, 1450, 1448, 1447, 1445, 1443, 1442, 1440, 1438, 1436, 1435, 1433, 1431, 1429, 1427, 1425, 1423, 1421, 1419, 1417, 1415, 1413, 1410, 1408, 1406, 1404, 1401, 1399, 1397, 1394, 1392, 1389, 1387, 1384, 1382, 1379, 1377, 1374, 1371, 1369, 1366, 1363, 1360, 1357, 1355, 1352, 1349, 1346, 1343, 1340, 1337, 1334, 1331, 1327, 1324, 1321, 1318, 1314, 1311, 1308, 1304, 1301, 1298, 1294, 1291, 1287, 1284, 1280, 1277, 1273, 1269, 1266, 1262, 1258, 1254, 1251, 1247, 1243, 1239, 1235, 1231, 1227, 1223, 1219, 1215, 1211, 1207, 1203, 1199, 1194, 1190, 1186, 1182, 1177, 1173, 1169, 1164, 1160, 1155, 1151, 1147, 1142, 1138, 1133, 1128, 1124, 1119, 1115, 1110, 1105, 1101, 1096, 1091, 1086, 1082, 1077, 1072, 1067, 1062, 1057, 1052, 1047, 1042, 1038, 1033, 1028, 1023, 1017, 1012, 1007, 1002, 997, 992, 987, 982, 977, 971, 966, 961, 956, 951, 945, 940, 935, 930, 924, 919, 914, 908, 903, 898, 892, 887, 882, 876, 871, 865, 860, 855, 849, 844, 838, 833, 827, 822, 817, 811, 806, 800, 795, 789, 784, 778, 773, 767, 762, 756, 751, 745, 740, 734, 729, 723, 718, 713, 707, 702, 696, 691, 685, 680, 674, 669, 663, 658, 653, 647, 642, 636, 631, 625, 620, 615, 609, 604, 599, 593, 588, 583, 577, 572, 567, 561, 556, 551, 546, 541, 535, 530, 525, 520, 515, 509, 504, 499, 494, 489, 484, 479, 474, 469, 464, 459, 454, 449, 444, 439, 434, 430, 425, 420, 415, 410, 406, 401, 396, 391, 387, 382, 377, 373, 368, 364, 359, 355, 350, 346, 341, 337, 333, 328, 324, 320, 315, 311, 307, 303, 298, 294, 290, 286, 282, 278, 274, 270, 266, 262, 258, 254, 250, 247, 243, 239, 235, 232, 228, 224, 221, 217, 213, 210, 206, 203, 200, 196, 193, 189, 186, 183, 180, 176, 173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 140, 137, 135, 132, 129, 126, 124, 121, 119, 116, 113, 111, 108, 106, 104, 101, 99, 97, 94, 92, 90, 88, 85, 83, 81, 79, 77, 75, 73, 71, 69, 67, 65, 64, 62, 60, 58, 57, 55, 53, 52, 50, 48, 47, 45, 44, 42, 41, 40, 38, 37, 36, 34, 33, 32, 31, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 16, 15, 14, 13, 12, 12, 11, 10, 10, 9, 9, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 26, 26, 26, 26, 26, 26, 25, 25, 25, 25, 25, 24, 24, 24, 24, 23, 23, 23, 23, 22, 22, 22, 22, 21, 21, 21, 20, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 16, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 40, 41, 42, 44, 45, 47, 48, 50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 88, 90, 92, 94, 96, 99, 101, 104, 106, 108, 111, 113, 116, 118, 121, 124, 126, 129, 132, 135, 137, 140, 143, 146, 149, 152, 155, 158, 161, 164, 167 };

    constexpr auto LUT = LUTGenerator::generate();
    constexpr auto LUT1 = generateOtherPhasesFromLUT(LUT,0);
    constexpr auto LUT2 = generateOtherPhasesFromLUT(LUT1,120);
    constexpr auto LUT3 = generateOtherPhasesFromLUT(LUT1,240);
    constexpr auto params = LUTGenerator::calculateModulationIndexScalingOffsetParameters();
    std::cout << "params.m: " << params.offsetParam_m << " params.c: " << params.offsetParam_c << std::endl;
    int a = LUT1[50];
    int b = LUT2[50];
    int c = LUT3[50];

    std::cout << "a : " << a << std::endl;
    std::cout << "b : " << b << std::endl;
    std::cout << "c : " << c << std::endl;
    std::cout << "Sum of a+b+c : " << a+b+c << std::endl;

    testSVPWMScaling(LUT);
    std::vector<int> xAxis{};
    for (int i = 0; i < 1489; ++i) {
        xAxis.push_back(i);
    }
    std::vector<int > k(LUT1.begin(),LUT1.end());
    std::vector<int > k120(LUT2.begin(),LUT2.end());
    std::vector<int > k240(LUT3.begin(),LUT3.end());
    std::vector<int > printVector(printArray1.begin(),printArray1.end());
    std::vector<int > printVector2(printArray2.begin(),printArray2.end());
    std::vector<int > printVector3(printArray3.begin(),printArray3.end());
    plt::plot(xAxis,printVector);
    plt::plot(xAxis,printVector2);
    plt::plot(xAxis,printVector3);
    //plt::plot(xAxis,k120);
    //plt::plot(xAxis,k240);
    plt::show();

    return 0;
}
