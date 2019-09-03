#ifndef WAVETABLE_TWOOPFM1_H
#define WAVETABLE_TWOOPFM1_H
#define WAVETABLE_TWOOPFM1_NUM 128

#include "twoOpFM1_1.h"
#include "twoOpFM1_2.h"
#include "twoOpFM1_3.h"
#include "twoOpFM1_4.h"
#include "twoOpFM1_5.h"
#include "twoOpFM1_6.h"
#include "twoOpFM1_7.h"
#include "twoOpFM1_8.h"
#include "twoOpFM1_9.h"
#include "twoOpFM1_10.h"
#include "twoOpFM1_11.h"
#include "twoOpFM1_12.h"
#include "twoOpFM1_13.h"
#include "twoOpFM1_14.h"
#include "twoOpFM1_15.h"
#include "twoOpFM1_16.h"
#include "twoOpFM1_17.h"
#include "twoOpFM1_18.h"
#include "twoOpFM1_19.h"
#include "twoOpFM1_20.h"
#include "twoOpFM1_21.h"
#include "twoOpFM1_22.h"
#include "twoOpFM1_23.h"
#include "twoOpFM1_24.h"
#include "twoOpFM1_25.h"
#include "twoOpFM1_26.h"
#include "twoOpFM1_27.h"
#include "twoOpFM1_28.h"
#include "twoOpFM1_29.h"
#include "twoOpFM1_30.h"
#include "twoOpFM1_31.h"
#include "twoOpFM1_32.h"
#include "twoOpFM1_33.h"
#include "twoOpFM1_34.h"
#include "twoOpFM1_35.h"
#include "twoOpFM1_36.h"
#include "twoOpFM1_37.h"
#include "twoOpFM1_38.h"
#include "twoOpFM1_39.h"
#include "twoOpFM1_40.h"
#include "twoOpFM1_41.h"
#include "twoOpFM1_42.h"
#include "twoOpFM1_43.h"
#include "twoOpFM1_44.h"
#include "twoOpFM1_45.h"
#include "twoOpFM1_46.h"
#include "twoOpFM1_47.h"
#include "twoOpFM1_48.h"
#include "twoOpFM1_49.h"
#include "twoOpFM1_50.h"
#include "twoOpFM1_51.h"
#include "twoOpFM1_52.h"
#include "twoOpFM1_53.h"
#include "twoOpFM1_54.h"
#include "twoOpFM1_55.h"
#include "twoOpFM1_56.h"
#include "twoOpFM1_57.h"
#include "twoOpFM1_58.h"
#include "twoOpFM1_59.h"
#include "twoOpFM1_60.h"
#include "twoOpFM1_61.h"
#include "twoOpFM1_62.h"
#include "twoOpFM1_63.h"
#include "twoOpFM1_64.h"
#include "twoOpFM1_65.h"
#include "twoOpFM1_66.h"
#include "twoOpFM1_67.h"
#include "twoOpFM1_68.h"
#include "twoOpFM1_69.h"
#include "twoOpFM1_70.h"
#include "twoOpFM1_71.h"
#include "twoOpFM1_72.h"
#include "twoOpFM1_73.h"
#include "twoOpFM1_74.h"
#include "twoOpFM1_75.h"
#include "twoOpFM1_76.h"
#include "twoOpFM1_77.h"
#include "twoOpFM1_78.h"
#include "twoOpFM1_79.h"
#include "twoOpFM1_80.h"
#include "twoOpFM1_81.h"
#include "twoOpFM1_82.h"
#include "twoOpFM1_83.h"
#include "twoOpFM1_84.h"
#include "twoOpFM1_85.h"
#include "twoOpFM1_86.h"
#include "twoOpFM1_87.h"
#include "twoOpFM1_88.h"
#include "twoOpFM1_89.h"
#include "twoOpFM1_90.h"
#include "twoOpFM1_91.h"
#include "twoOpFM1_92.h"
#include "twoOpFM1_93.h"
#include "twoOpFM1_94.h"
#include "twoOpFM1_95.h"
#include "twoOpFM1_96.h"
#include "twoOpFM1_97.h"
#include "twoOpFM1_98.h"
#include "twoOpFM1_99.h"
#include "twoOpFM1_100.h"
#include "twoOpFM1_101.h"
#include "twoOpFM1_102.h"
#include "twoOpFM1_103.h"
#include "twoOpFM1_104.h"
#include "twoOpFM1_105.h"
#include "twoOpFM1_106.h"
#include "twoOpFM1_107.h"
#include "twoOpFM1_108.h"
#include "twoOpFM1_109.h"
#include "twoOpFM1_110.h"
#include "twoOpFM1_111.h"
#include "twoOpFM1_112.h"
#include "twoOpFM1_113.h"
#include "twoOpFM1_114.h"
#include "twoOpFM1_115.h"
#include "twoOpFM1_116.h"
#include "twoOpFM1_117.h"
#include "twoOpFM1_118.h"
#include "twoOpFM1_119.h"
#include "twoOpFM1_120.h"
#include "twoOpFM1_121.h"
#include "twoOpFM1_122.h"
#include "twoOpFM1_123.h"
#include "twoOpFM1_124.h"
#include "twoOpFM1_125.h"
#include "twoOpFM1_126.h"
#include "twoOpFM1_127.h"
#include "twoOpFM1_128.h"

static float* wavetable_twoOpFM1[WAVETABLE_TWOOPFM1_NUM] = {
    twoOpFM1_1_waveTable,
    twoOpFM1_2_waveTable,
    twoOpFM1_3_waveTable,
    twoOpFM1_4_waveTable,
    twoOpFM1_5_waveTable,
    twoOpFM1_6_waveTable,
    twoOpFM1_7_waveTable,
    twoOpFM1_8_waveTable,
    twoOpFM1_9_waveTable,
    twoOpFM1_10_waveTable,
    twoOpFM1_11_waveTable,
    twoOpFM1_12_waveTable,
    twoOpFM1_13_waveTable,
    twoOpFM1_14_waveTable,
    twoOpFM1_15_waveTable,
    twoOpFM1_16_waveTable,
    twoOpFM1_17_waveTable,
    twoOpFM1_18_waveTable,
    twoOpFM1_19_waveTable,
    twoOpFM1_20_waveTable,
    twoOpFM1_21_waveTable,
    twoOpFM1_22_waveTable,
    twoOpFM1_23_waveTable,
    twoOpFM1_24_waveTable,
    twoOpFM1_25_waveTable,
    twoOpFM1_26_waveTable,
    twoOpFM1_27_waveTable,
    twoOpFM1_28_waveTable,
    twoOpFM1_29_waveTable,
    twoOpFM1_30_waveTable,
    twoOpFM1_31_waveTable,
    twoOpFM1_32_waveTable,
    twoOpFM1_33_waveTable,
    twoOpFM1_34_waveTable,
    twoOpFM1_35_waveTable,
    twoOpFM1_36_waveTable,
    twoOpFM1_37_waveTable,
    twoOpFM1_38_waveTable,
    twoOpFM1_39_waveTable,
    twoOpFM1_40_waveTable,
    twoOpFM1_41_waveTable,
    twoOpFM1_42_waveTable,
    twoOpFM1_43_waveTable,
    twoOpFM1_44_waveTable,
    twoOpFM1_45_waveTable,
    twoOpFM1_46_waveTable,
    twoOpFM1_47_waveTable,
    twoOpFM1_48_waveTable,
    twoOpFM1_49_waveTable,
    twoOpFM1_50_waveTable,
    twoOpFM1_51_waveTable,
    twoOpFM1_52_waveTable,
    twoOpFM1_53_waveTable,
    twoOpFM1_54_waveTable,
    twoOpFM1_55_waveTable,
    twoOpFM1_56_waveTable,
    twoOpFM1_57_waveTable,
    twoOpFM1_58_waveTable,
    twoOpFM1_59_waveTable,
    twoOpFM1_60_waveTable,
    twoOpFM1_61_waveTable,
    twoOpFM1_62_waveTable,
    twoOpFM1_63_waveTable,
    twoOpFM1_64_waveTable,
    twoOpFM1_65_waveTable,
    twoOpFM1_66_waveTable,
    twoOpFM1_67_waveTable,
    twoOpFM1_68_waveTable,
    twoOpFM1_69_waveTable,
    twoOpFM1_70_waveTable,
    twoOpFM1_71_waveTable,
    twoOpFM1_72_waveTable,
    twoOpFM1_73_waveTable,
    twoOpFM1_74_waveTable,
    twoOpFM1_75_waveTable,
    twoOpFM1_76_waveTable,
    twoOpFM1_77_waveTable,
    twoOpFM1_78_waveTable,
    twoOpFM1_79_waveTable,
    twoOpFM1_80_waveTable,
    twoOpFM1_81_waveTable,
    twoOpFM1_82_waveTable,
    twoOpFM1_83_waveTable,
    twoOpFM1_84_waveTable,
    twoOpFM1_85_waveTable,
    twoOpFM1_86_waveTable,
    twoOpFM1_87_waveTable,
    twoOpFM1_88_waveTable,
    twoOpFM1_89_waveTable,
    twoOpFM1_90_waveTable,
    twoOpFM1_91_waveTable,
    twoOpFM1_92_waveTable,
    twoOpFM1_93_waveTable,
    twoOpFM1_94_waveTable,
    twoOpFM1_95_waveTable,
    twoOpFM1_96_waveTable,
    twoOpFM1_97_waveTable,
    twoOpFM1_98_waveTable,
    twoOpFM1_99_waveTable,
    twoOpFM1_100_waveTable,
    twoOpFM1_101_waveTable,
    twoOpFM1_102_waveTable,
    twoOpFM1_103_waveTable,
    twoOpFM1_104_waveTable,
    twoOpFM1_105_waveTable,
    twoOpFM1_106_waveTable,
    twoOpFM1_107_waveTable,
    twoOpFM1_108_waveTable,
    twoOpFM1_109_waveTable,
    twoOpFM1_110_waveTable,
    twoOpFM1_111_waveTable,
    twoOpFM1_112_waveTable,
    twoOpFM1_113_waveTable,
    twoOpFM1_114_waveTable,
    twoOpFM1_115_waveTable,
    twoOpFM1_116_waveTable,
    twoOpFM1_117_waveTable,
    twoOpFM1_118_waveTable,
    twoOpFM1_119_waveTable,
    twoOpFM1_120_waveTable,
    twoOpFM1_121_waveTable,
    twoOpFM1_122_waveTable,
    twoOpFM1_123_waveTable,
    twoOpFM1_124_waveTable,
    twoOpFM1_125_waveTable,
    twoOpFM1_126_waveTable,
    twoOpFM1_127_waveTable,
    twoOpFM1_128_waveTable
};

static long wavetable_twoOpFM1_lengths[WAVETABLE_TWOOPFM1_NUM] = {
    twoOpFM1_1_tableLength,
    twoOpFM1_2_tableLength,
    twoOpFM1_3_tableLength,
    twoOpFM1_4_tableLength,
    twoOpFM1_5_tableLength,
    twoOpFM1_6_tableLength,
    twoOpFM1_7_tableLength,
    twoOpFM1_8_tableLength,
    twoOpFM1_9_tableLength,
    twoOpFM1_10_tableLength,
    twoOpFM1_11_tableLength,
    twoOpFM1_12_tableLength,
    twoOpFM1_13_tableLength,
    twoOpFM1_14_tableLength,
    twoOpFM1_15_tableLength,
    twoOpFM1_16_tableLength,
    twoOpFM1_17_tableLength,
    twoOpFM1_18_tableLength,
    twoOpFM1_19_tableLength,
    twoOpFM1_20_tableLength,
    twoOpFM1_21_tableLength,
    twoOpFM1_22_tableLength,
    twoOpFM1_23_tableLength,
    twoOpFM1_24_tableLength,
    twoOpFM1_25_tableLength,
    twoOpFM1_26_tableLength,
    twoOpFM1_27_tableLength,
    twoOpFM1_28_tableLength,
    twoOpFM1_29_tableLength,
    twoOpFM1_30_tableLength,
    twoOpFM1_31_tableLength,
    twoOpFM1_32_tableLength,
    twoOpFM1_33_tableLength,
    twoOpFM1_34_tableLength,
    twoOpFM1_35_tableLength,
    twoOpFM1_36_tableLength,
    twoOpFM1_37_tableLength,
    twoOpFM1_38_tableLength,
    twoOpFM1_39_tableLength,
    twoOpFM1_40_tableLength,
    twoOpFM1_41_tableLength,
    twoOpFM1_42_tableLength,
    twoOpFM1_43_tableLength,
    twoOpFM1_44_tableLength,
    twoOpFM1_45_tableLength,
    twoOpFM1_46_tableLength,
    twoOpFM1_47_tableLength,
    twoOpFM1_48_tableLength,
    twoOpFM1_49_tableLength,
    twoOpFM1_50_tableLength,
    twoOpFM1_51_tableLength,
    twoOpFM1_52_tableLength,
    twoOpFM1_53_tableLength,
    twoOpFM1_54_tableLength,
    twoOpFM1_55_tableLength,
    twoOpFM1_56_tableLength,
    twoOpFM1_57_tableLength,
    twoOpFM1_58_tableLength,
    twoOpFM1_59_tableLength,
    twoOpFM1_60_tableLength,
    twoOpFM1_61_tableLength,
    twoOpFM1_62_tableLength,
    twoOpFM1_63_tableLength,
    twoOpFM1_64_tableLength,
    twoOpFM1_65_tableLength,
    twoOpFM1_66_tableLength,
    twoOpFM1_67_tableLength,
    twoOpFM1_68_tableLength,
    twoOpFM1_69_tableLength,
    twoOpFM1_70_tableLength,
    twoOpFM1_71_tableLength,
    twoOpFM1_72_tableLength,
    twoOpFM1_73_tableLength,
    twoOpFM1_74_tableLength,
    twoOpFM1_75_tableLength,
    twoOpFM1_76_tableLength,
    twoOpFM1_77_tableLength,
    twoOpFM1_78_tableLength,
    twoOpFM1_79_tableLength,
    twoOpFM1_80_tableLength,
    twoOpFM1_81_tableLength,
    twoOpFM1_82_tableLength,
    twoOpFM1_83_tableLength,
    twoOpFM1_84_tableLength,
    twoOpFM1_85_tableLength,
    twoOpFM1_86_tableLength,
    twoOpFM1_87_tableLength,
    twoOpFM1_88_tableLength,
    twoOpFM1_89_tableLength,
    twoOpFM1_90_tableLength,
    twoOpFM1_91_tableLength,
    twoOpFM1_92_tableLength,
    twoOpFM1_93_tableLength,
    twoOpFM1_94_tableLength,
    twoOpFM1_95_tableLength,
    twoOpFM1_96_tableLength,
    twoOpFM1_97_tableLength,
    twoOpFM1_98_tableLength,
    twoOpFM1_99_tableLength,
    twoOpFM1_100_tableLength,
    twoOpFM1_101_tableLength,
    twoOpFM1_102_tableLength,
    twoOpFM1_103_tableLength,
    twoOpFM1_104_tableLength,
    twoOpFM1_105_tableLength,
    twoOpFM1_106_tableLength,
    twoOpFM1_107_tableLength,
    twoOpFM1_108_tableLength,
    twoOpFM1_109_tableLength,
    twoOpFM1_110_tableLength,
    twoOpFM1_111_tableLength,
    twoOpFM1_112_tableLength,
    twoOpFM1_113_tableLength,
    twoOpFM1_114_tableLength,
    twoOpFM1_115_tableLength,
    twoOpFM1_116_tableLength,
    twoOpFM1_117_tableLength,
    twoOpFM1_118_tableLength,
    twoOpFM1_119_tableLength,
    twoOpFM1_120_tableLength,
    twoOpFM1_121_tableLength,
    twoOpFM1_122_tableLength,
    twoOpFM1_123_tableLength,
    twoOpFM1_124_tableLength,
    twoOpFM1_125_tableLength,
    twoOpFM1_126_tableLength,
    twoOpFM1_127_tableLength,
    twoOpFM1_128_tableLength
};

#endif // WAVETABLE_TWOOPFM1_H
