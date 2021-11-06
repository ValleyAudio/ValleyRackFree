
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

BINARIES += src/binaries/ADD_BANK1.bin
BINARIES += src/binaries/ADD_BANK2.bin
BINARIES += src/binaries/ADD_BANK3.bin
BINARIES += src/binaries/ADD_BANK4.bin
BINARIES += src/binaries/ADD_SAW.bin
BINARIES += src/binaries/ADD_SINE.bin
BINARIES += src/binaries/ADD_SQR.bin
BINARIES += src/binaries/ALTOSAX.bin
BINARIES += src/binaries/AM_HARM.bin
BINARIES += src/binaries/BASIC.bin
BINARIES += src/binaries/BI_PULSE.bin
BINARIES += src/binaries/BITCRUSH1.bin
BINARIES += src/binaries/BITCRUSH2.bin
BINARIES += src/binaries/CELLO_1.bin
BINARIES += src/binaries/CELLO_2.bin
BINARIES += src/binaries/CHIP_1.bin
BINARIES += src/binaries/CHIP_2.bin
BINARIES += src/binaries/CHIRP.bin
BINARIES += src/binaries/DISTORT.bin
BINARIES += src/binaries/EBASS.bin
BINARIES += src/binaries/FM1.bin
BINARIES += src/binaries/FM2.bin
BINARIES += src/binaries/FM3.bin
BINARIES += src/binaries/FM4.bin
BINARIES += src/binaries/FOLD_SINE.bin
BINARIES += src/binaries/GMTRY_1.bin
BINARIES += src/binaries/GMTRY_2.bin
BINARIES += src/binaries/GMTRY_3.bin
BINARIES += src/binaries/GRIT.bin
BINARIES += src/binaries/LINEAR.bin
BINARIES += src/binaries/OBOE.bin
BINARIES += src/binaries/OPAL.bin
BINARIES += src/binaries/OVERTONE1.bin
BINARIES += src/binaries/OVERTONE2.bin
BINARIES += src/binaries/PIANO.bin
BINARIES += src/binaries/PLAITS_1.bin
BINARIES += src/binaries/PLAITS_2.bin
BINARIES += src/binaries/PLAITS_3.bin
BINARIES += src/binaries/PLUCK.bin
BINARIES += src/binaries/PWM.bin
BINARIES += src/binaries/REED.bin
BINARIES += src/binaries/RESO_SAW.bin
BINARIES += src/binaries/RESO_SQR.bin
BINARIES += src/binaries/SAW_GAP1.bin
BINARIES += src/binaries/SAW_GAP2.bin
BINARIES += src/binaries/SAW_PHASE.bin
BINARIES += src/binaries/SINE_HARM.bin
BINARIES += src/binaries/SWEEPHARM.bin
BINARIES += src/binaries/SYMMETRY.bin
BINARIES += src/binaries/TEE_EKS.bin
BINARIES += src/binaries/THEREMIN.bin
BINARIES += src/binaries/TWO_OP_RAND.bin
BINARIES += src/binaries/TWO_OPFM1.bin
BINARIES += src/binaries/TWO_OPFM2.bin
BINARIES += src/binaries/VIDEOGAME.bin
BINARIES += src/binaries/VIOLIN.bin
BINARIES += src/binaries/VOICE_1.bin
BINARIES += src/binaries/VOICE_2.bin
BINARIES += src/binaries/VOICE_3.bin
BINARIES += src/binaries/VOICE_4.bin
BINARIES += src/binaries/VOICE_5.bin
BINARIES += src/binaries/VOICE_6.bin
BINARIES += src/binaries/VOX_MACH.bin
BINARIES += src/binaries/XFADE.bin


# Convenience target for including files in the distributable release
#.PHONY: dist
#dist: all

DISTRIBUTABLES += $(wildcard LICENSE* *.pdf README*) res
include $(RACK_DIR)/plugin.mk
