
# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS += -O3 -std=c99
CXXFLAGS += -O3

# Careful about linking to libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine.
LDFLAGS +=

# Add .cpp and .c files to the build
SOURCES = $(wildcard src/*.cpp src/*.c src/*/*.cpp src/*/*.c src/*/*/*.cpp src/*/*/*/c)

# Must include the VCV plugin Makefile framework
RACK_DIR ?= ../..

BINARIES += src/ADD_BANK1.bin
BINARIES += src/ADD_BANK2.bin
BINARIES += src/ADD_BANK3.bin
BINARIES += src/ADD_BANK4.bin
BINARIES += src/ADD_SAW.bin
BINARIES += src/ADD_SINE.bin
BINARIES += src/ADD_SQR.bin
BINARIES += src/ALTOSAX.bin
BINARIES += src/AM_HARM.bin
BINARIES += src/BASIC.bin
BINARIES += src/BI_PULSE.bin
BINARIES += src/BITCRUSH1.bin
BINARIES += src/BITCRUSH2.bin
BINARIES += src/CELLO_1.bin
BINARIES += src/CELLO_2.bin
BINARIES += src/CHIP_1.bin
BINARIES += src/CHIP_2.bin
BINARIES += src/CHIRP.bin
BINARIES += src/DISTORT.bin
BINARIES += src/EBASS.bin
BINARIES += src/FM1.bin
BINARIES += src/FM2.bin
BINARIES += src/FM3.bin
BINARIES += src/FM4.bin
BINARIES += src/FOLD_SINE.bin
BINARIES += src/GMTRY_1.bin
BINARIES += src/GMTRY_2.bin
BINARIES += src/GMTRY_3.bin
BINARIES += src/GRIT.bin
BINARIES += src/LINEAR.bin
BINARIES += src/OBOE.bin
BINARIES += src/OPAL.bin
BINARIES += src/OVERTONE1.bin
BINARIES += src/OVERTONE2.bin
BINARIES += src/PIANO.bin
BINARIES += src/PLAITS_1.bin
BINARIES += src/PLAITS_2.bin
BINARIES += src/PLAITS_3.bin
BINARIES += src/PLUCK.bin
BINARIES += src/PWM.bin
BINARIES += src/REED.bin
BINARIES += src/RESO_SAW.bin
BINARIES += src/RESO_SQR.bin
BINARIES += src/SAW_GAP1.bin
BINARIES += src/SAW_GAP2.bin
BINARIES += src/SAW_PHASE.bin
BINARIES += src/SINE_HARM.bin
BINARIES += src/SWEEPHARM.bin
BINARIES += src/SYMMETRY.bin
BINARIES += src/TEE_EKS.bin
BINARIES += src/THEREMIN.bin
BINARIES += src/TWO_OP_RAND.bin
BINARIES += src/TWO_OPFM1.bin
BINARIES += src/TWO_OPFM2.bin
BINARIES += src/VIDEOGAME.bin
BINARIES += src/VIOLIN.bin
BINARIES += src/VOICE_1.bin
BINARIES += src/VOICE_2.bin
BINARIES += src/VOICE_3.bin
BINARIES += src/VOICE_4.bin
BINARIES += src/VOICE_5.bin
BINARIES += src/VOICE_6.bin
BINARIES += src/VOX_MACH.bin
BINARIES += src/XFADE.bin


# Convenience target for including files in the distributable release
#.PHONY: dist
#dist: all

DISTRIBUTABLES += $(wildcard LICENSE* *.pdf README*) res
include $(RACK_DIR)/plugin.mk
