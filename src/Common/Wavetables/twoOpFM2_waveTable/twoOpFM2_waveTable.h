#ifndef WAVETABLE_TWOOPFM2_H
#define WAVETABLE_TWOOPFM2_H
#define WAVETABLE_TWOOPFM2_NUM 128

#include "twoOpFM2_1.h"
#include "twoOpFM2_2.h"
#include "twoOpFM2_3.h"
#include "twoOpFM2_4.h"
#include "twoOpFM2_5.h"
#include "twoOpFM2_6.h"
#include "twoOpFM2_7.h"
#include "twoOpFM2_8.h"
#include "twoOpFM2_9.h"
#include "twoOpFM2_10.h"
#include "twoOpFM2_11.h"
#include "twoOpFM2_12.h"
#include "twoOpFM2_13.h"
#include "twoOpFM2_14.h"
#include "twoOpFM2_15.h"
#include "twoOpFM2_16.h"
#include "twoOpFM2_17.h"
#include "twoOpFM2_18.h"
#include "twoOpFM2_19.h"
#include "twoOpFM2_20.h"
#include "twoOpFM2_21.h"
#include "twoOpFM2_22.h"
#include "twoOpFM2_23.h"
#include "twoOpFM2_24.h"
#include "twoOpFM2_25.h"
#include "twoOpFM2_26.h"
#include "twoOpFM2_27.h"
#include "twoOpFM2_28.h"
#include "twoOpFM2_29.h"
#include "twoOpFM2_30.h"
#include "twoOpFM2_31.h"
#include "twoOpFM2_32.h"
#include "twoOpFM2_33.h"
#include "twoOpFM2_34.h"
#include "twoOpFM2_35.h"
#include "twoOpFM2_36.h"
#include "twoOpFM2_37.h"
#include "twoOpFM2_38.h"
#include "twoOpFM2_39.h"
#include "twoOpFM2_40.h"
#include "twoOpFM2_41.h"
#include "twoOpFM2_42.h"
#include "twoOpFM2_43.h"
#include "twoOpFM2_44.h"
#include "twoOpFM2_45.h"
#include "twoOpFM2_46.h"
#include "twoOpFM2_47.h"
#include "twoOpFM2_48.h"
#include "twoOpFM2_49.h"
#include "twoOpFM2_50.h"
#include "twoOpFM2_51.h"
#include "twoOpFM2_52.h"
#include "twoOpFM2_53.h"
#include "twoOpFM2_54.h"
#include "twoOpFM2_55.h"
#include "twoOpFM2_56.h"
#include "twoOpFM2_57.h"
#include "twoOpFM2_58.h"
#include "twoOpFM2_59.h"
#include "twoOpFM2_60.h"
#include "twoOpFM2_61.h"
#include "twoOpFM2_62.h"
#include "twoOpFM2_63.h"
#include "twoOpFM2_64.h"
#include "twoOpFM2_65.h"
#include "twoOpFM2_66.h"
#include "twoOpFM2_67.h"
#include "twoOpFM2_68.h"
#include "twoOpFM2_69.h"
#include "twoOpFM2_70.h"
#include "twoOpFM2_71.h"
#include "twoOpFM2_72.h"
#include "twoOpFM2_73.h"
#include "twoOpFM2_74.h"
#include "twoOpFM2_75.h"
#include "twoOpFM2_76.h"
#include "twoOpFM2_77.h"
#include "twoOpFM2_78.h"
#include "twoOpFM2_79.h"
#include "twoOpFM2_80.h"
#include "twoOpFM2_81.h"
#include "twoOpFM2_82.h"
#include "twoOpFM2_83.h"
#include "twoOpFM2_84.h"
#include "twoOpFM2_85.h"
#include "twoOpFM2_86.h"
#include "twoOpFM2_87.h"
#include "twoOpFM2_88.h"
#include "twoOpFM2_89.h"
#include "twoOpFM2_90.h"
#include "twoOpFM2_91.h"
#include "twoOpFM2_92.h"
#include "twoOpFM2_93.h"
#include "twoOpFM2_94.h"
#include "twoOpFM2_95.h"
#include "twoOpFM2_96.h"
#include "twoOpFM2_97.h"
#include "twoOpFM2_98.h"
#include "twoOpFM2_99.h"
#include "twoOpFM2_100.h"
#include "twoOpFM2_101.h"
#include "twoOpFM2_102.h"
#include "twoOpFM2_103.h"
#include "twoOpFM2_104.h"
#include "twoOpFM2_105.h"
#include "twoOpFM2_106.h"
#include "twoOpFM2_107.h"
#include "twoOpFM2_108.h"
#include "twoOpFM2_109.h"
#include "twoOpFM2_110.h"
#include "twoOpFM2_111.h"
#include "twoOpFM2_112.h"
#include "twoOpFM2_113.h"
#include "twoOpFM2_114.h"
#include "twoOpFM2_115.h"
#include "twoOpFM2_116.h"
#include "twoOpFM2_117.h"
#include "twoOpFM2_118.h"
#include "twoOpFM2_119.h"
#include "twoOpFM2_120.h"
#include "twoOpFM2_121.h"
#include "twoOpFM2_122.h"
#include "twoOpFM2_123.h"
#include "twoOpFM2_124.h"
#include "twoOpFM2_125.h"
#include "twoOpFM2_126.h"
#include "twoOpFM2_127.h"
#include "twoOpFM2_128.h"

static float* wavetable_twoOpFM2[WAVETABLE_TWOOPFM2_NUM] = {
    twoOpFM2_1_waveTable,
    twoOpFM2_2_waveTable,
    twoOpFM2_3_waveTable,
    twoOpFM2_4_waveTable,
    twoOpFM2_5_waveTable,
    twoOpFM2_6_waveTable,
    twoOpFM2_7_waveTable,
    twoOpFM2_8_waveTable,
    twoOpFM2_9_waveTable,
    twoOpFM2_10_waveTable,
    twoOpFM2_11_waveTable,
    twoOpFM2_12_waveTable,
    twoOpFM2_13_waveTable,
    twoOpFM2_14_waveTable,
    twoOpFM2_15_waveTable,
    twoOpFM2_16_waveTable,
    twoOpFM2_17_waveTable,
    twoOpFM2_18_waveTable,
    twoOpFM2_19_waveTable,
    twoOpFM2_20_waveTable,
    twoOpFM2_21_waveTable,
    twoOpFM2_22_waveTable,
    twoOpFM2_23_waveTable,
    twoOpFM2_24_waveTable,
    twoOpFM2_25_waveTable,
    twoOpFM2_26_waveTable,
    twoOpFM2_27_waveTable,
    twoOpFM2_28_waveTable,
    twoOpFM2_29_waveTable,
    twoOpFM2_30_waveTable,
    twoOpFM2_31_waveTable,
    twoOpFM2_32_waveTable,
    twoOpFM2_33_waveTable,
    twoOpFM2_34_waveTable,
    twoOpFM2_35_waveTable,
    twoOpFM2_36_waveTable,
    twoOpFM2_37_waveTable,
    twoOpFM2_38_waveTable,
    twoOpFM2_39_waveTable,
    twoOpFM2_40_waveTable,
    twoOpFM2_41_waveTable,
    twoOpFM2_42_waveTable,
    twoOpFM2_43_waveTable,
    twoOpFM2_44_waveTable,
    twoOpFM2_45_waveTable,
    twoOpFM2_46_waveTable,
    twoOpFM2_47_waveTable,
    twoOpFM2_48_waveTable,
    twoOpFM2_49_waveTable,
    twoOpFM2_50_waveTable,
    twoOpFM2_51_waveTable,
    twoOpFM2_52_waveTable,
    twoOpFM2_53_waveTable,
    twoOpFM2_54_waveTable,
    twoOpFM2_55_waveTable,
    twoOpFM2_56_waveTable,
    twoOpFM2_57_waveTable,
    twoOpFM2_58_waveTable,
    twoOpFM2_59_waveTable,
    twoOpFM2_60_waveTable,
    twoOpFM2_61_waveTable,
    twoOpFM2_62_waveTable,
    twoOpFM2_63_waveTable,
    twoOpFM2_64_waveTable,
    twoOpFM2_65_waveTable,
    twoOpFM2_66_waveTable,
    twoOpFM2_67_waveTable,
    twoOpFM2_68_waveTable,
    twoOpFM2_69_waveTable,
    twoOpFM2_70_waveTable,
    twoOpFM2_71_waveTable,
    twoOpFM2_72_waveTable,
    twoOpFM2_73_waveTable,
    twoOpFM2_74_waveTable,
    twoOpFM2_75_waveTable,
    twoOpFM2_76_waveTable,
    twoOpFM2_77_waveTable,
    twoOpFM2_78_waveTable,
    twoOpFM2_79_waveTable,
    twoOpFM2_80_waveTable,
    twoOpFM2_81_waveTable,
    twoOpFM2_82_waveTable,
    twoOpFM2_83_waveTable,
    twoOpFM2_84_waveTable,
    twoOpFM2_85_waveTable,
    twoOpFM2_86_waveTable,
    twoOpFM2_87_waveTable,
    twoOpFM2_88_waveTable,
    twoOpFM2_89_waveTable,
    twoOpFM2_90_waveTable,
    twoOpFM2_91_waveTable,
    twoOpFM2_92_waveTable,
    twoOpFM2_93_waveTable,
    twoOpFM2_94_waveTable,
    twoOpFM2_95_waveTable,
    twoOpFM2_96_waveTable,
    twoOpFM2_97_waveTable,
    twoOpFM2_98_waveTable,
    twoOpFM2_99_waveTable,
    twoOpFM2_100_waveTable,
    twoOpFM2_101_waveTable,
    twoOpFM2_102_waveTable,
    twoOpFM2_103_waveTable,
    twoOpFM2_104_waveTable,
    twoOpFM2_105_waveTable,
    twoOpFM2_106_waveTable,
    twoOpFM2_107_waveTable,
    twoOpFM2_108_waveTable,
    twoOpFM2_109_waveTable,
    twoOpFM2_110_waveTable,
    twoOpFM2_111_waveTable,
    twoOpFM2_112_waveTable,
    twoOpFM2_113_waveTable,
    twoOpFM2_114_waveTable,
    twoOpFM2_115_waveTable,
    twoOpFM2_116_waveTable,
    twoOpFM2_117_waveTable,
    twoOpFM2_118_waveTable,
    twoOpFM2_119_waveTable,
    twoOpFM2_120_waveTable,
    twoOpFM2_121_waveTable,
    twoOpFM2_122_waveTable,
    twoOpFM2_123_waveTable,
    twoOpFM2_124_waveTable,
    twoOpFM2_125_waveTable,
    twoOpFM2_126_waveTable,
    twoOpFM2_127_waveTable,
    twoOpFM2_128_waveTable
};

static long wavetable_twoOpFM2_lengths[WAVETABLE_TWOOPFM2_NUM] = {
    twoOpFM2_1_tableLength,
    twoOpFM2_2_tableLength,
    twoOpFM2_3_tableLength,
    twoOpFM2_4_tableLength,
    twoOpFM2_5_tableLength,
    twoOpFM2_6_tableLength,
    twoOpFM2_7_tableLength,
    twoOpFM2_8_tableLength,
    twoOpFM2_9_tableLength,
    twoOpFM2_10_tableLength,
    twoOpFM2_11_tableLength,
    twoOpFM2_12_tableLength,
    twoOpFM2_13_tableLength,
    twoOpFM2_14_tableLength,
    twoOpFM2_15_tableLength,
    twoOpFM2_16_tableLength,
    twoOpFM2_17_tableLength,
    twoOpFM2_18_tableLength,
    twoOpFM2_19_tableLength,
    twoOpFM2_20_tableLength,
    twoOpFM2_21_tableLength,
    twoOpFM2_22_tableLength,
    twoOpFM2_23_tableLength,
    twoOpFM2_24_tableLength,
    twoOpFM2_25_tableLength,
    twoOpFM2_26_tableLength,
    twoOpFM2_27_tableLength,
    twoOpFM2_28_tableLength,
    twoOpFM2_29_tableLength,
    twoOpFM2_30_tableLength,
    twoOpFM2_31_tableLength,
    twoOpFM2_32_tableLength,
    twoOpFM2_33_tableLength,
    twoOpFM2_34_tableLength,
    twoOpFM2_35_tableLength,
    twoOpFM2_36_tableLength,
    twoOpFM2_37_tableLength,
    twoOpFM2_38_tableLength,
    twoOpFM2_39_tableLength,
    twoOpFM2_40_tableLength,
    twoOpFM2_41_tableLength,
    twoOpFM2_42_tableLength,
    twoOpFM2_43_tableLength,
    twoOpFM2_44_tableLength,
    twoOpFM2_45_tableLength,
    twoOpFM2_46_tableLength,
    twoOpFM2_47_tableLength,
    twoOpFM2_48_tableLength,
    twoOpFM2_49_tableLength,
    twoOpFM2_50_tableLength,
    twoOpFM2_51_tableLength,
    twoOpFM2_52_tableLength,
    twoOpFM2_53_tableLength,
    twoOpFM2_54_tableLength,
    twoOpFM2_55_tableLength,
    twoOpFM2_56_tableLength,
    twoOpFM2_57_tableLength,
    twoOpFM2_58_tableLength,
    twoOpFM2_59_tableLength,
    twoOpFM2_60_tableLength,
    twoOpFM2_61_tableLength,
    twoOpFM2_62_tableLength,
    twoOpFM2_63_tableLength,
    twoOpFM2_64_tableLength,
    twoOpFM2_65_tableLength,
    twoOpFM2_66_tableLength,
    twoOpFM2_67_tableLength,
    twoOpFM2_68_tableLength,
    twoOpFM2_69_tableLength,
    twoOpFM2_70_tableLength,
    twoOpFM2_71_tableLength,
    twoOpFM2_72_tableLength,
    twoOpFM2_73_tableLength,
    twoOpFM2_74_tableLength,
    twoOpFM2_75_tableLength,
    twoOpFM2_76_tableLength,
    twoOpFM2_77_tableLength,
    twoOpFM2_78_tableLength,
    twoOpFM2_79_tableLength,
    twoOpFM2_80_tableLength,
    twoOpFM2_81_tableLength,
    twoOpFM2_82_tableLength,
    twoOpFM2_83_tableLength,
    twoOpFM2_84_tableLength,
    twoOpFM2_85_tableLength,
    twoOpFM2_86_tableLength,
    twoOpFM2_87_tableLength,
    twoOpFM2_88_tableLength,
    twoOpFM2_89_tableLength,
    twoOpFM2_90_tableLength,
    twoOpFM2_91_tableLength,
    twoOpFM2_92_tableLength,
    twoOpFM2_93_tableLength,
    twoOpFM2_94_tableLength,
    twoOpFM2_95_tableLength,
    twoOpFM2_96_tableLength,
    twoOpFM2_97_tableLength,
    twoOpFM2_98_tableLength,
    twoOpFM2_99_tableLength,
    twoOpFM2_100_tableLength,
    twoOpFM2_101_tableLength,
    twoOpFM2_102_tableLength,
    twoOpFM2_103_tableLength,
    twoOpFM2_104_tableLength,
    twoOpFM2_105_tableLength,
    twoOpFM2_106_tableLength,
    twoOpFM2_107_tableLength,
    twoOpFM2_108_tableLength,
    twoOpFM2_109_tableLength,
    twoOpFM2_110_tableLength,
    twoOpFM2_111_tableLength,
    twoOpFM2_112_tableLength,
    twoOpFM2_113_tableLength,
    twoOpFM2_114_tableLength,
    twoOpFM2_115_tableLength,
    twoOpFM2_116_tableLength,
    twoOpFM2_117_tableLength,
    twoOpFM2_118_tableLength,
    twoOpFM2_119_tableLength,
    twoOpFM2_120_tableLength,
    twoOpFM2_121_tableLength,
    twoOpFM2_122_tableLength,
    twoOpFM2_123_tableLength,
    twoOpFM2_124_tableLength,
    twoOpFM2_125_tableLength,
    twoOpFM2_126_tableLength,
    twoOpFM2_127_tableLength,
    twoOpFM2_128_tableLength
};

#endif // WAVETABLE_TWOOPFM2_H
