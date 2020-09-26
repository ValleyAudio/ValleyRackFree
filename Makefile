# SLUG = Valley
# VERSION = 0.6.16

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS += -O3 -std=c99
CXXFLAGS += -O3

# Careful about linking to libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine.
LDFLAGS +=

# Add .cpp and .c files to the build
SOURCES = $(wildcard src/*.cpp src/*.c src/*/*.cpp src/*/*.c src/*/*/*.cpp src/*/*/*/c)
BINARIES += ADD_BANK1.bin
BINARIES += ADD_BANK2.bin
BINARIES += ADD_BANK3.bin
BINARIES += ADD_BANK4.bin
BINARIES += ADD_SAW.bin
BINARIES += ADD_SINE.bin
BINARIES += ADD_SQR.bin
BINARIES += ALTOSAX.bin
BINARIES += AM_HARM.bin
BINARIES += BASIC.bin
BINARIES += BI_PULSE.bin
BINARIES += BITCRUSH1.bin
BINARIES += BITCRUSH2.bin
BINARIES += CELLO_1.bin
BINARIES += CELLO_2.bin
BINARIES += CHIP_1.bin
BINARIES += CHIP_2.bin
BINARIES += CHIRP.bin
BINARIES += DISTORT.bin
BINARIES += EBASS.bin
BINARIES += FM1.bin
BINARIES += FM2.bin
BINARIES += FM3.bin
BINARIES += FM4.bin
BINARIES += FOLD_SINE.bin
BINARIES += GMTRY_1.bin
BINARIES += GMTRY_2.bin
BINARIES += GMTRY_3.bin
BINARIES += GRIT.bin
BINARIES += LINEAR.bin
BINARIES += OBOE.bin
BINARIES += OPAL.bin
BINARIES += OVERTONE1.bin
BINARIES += OVERTONE2.bin
BINARIES += PIANO.bin
BINARIES += PLAITS_1.bin
BINARIES += PLAITS_2.bin
BINARIES += PLAITS_3.bin
BINARIES += PLUCK.bin
BINARIES += PWM.bin
BINARIES += REED.bin
BINARIES += RESO_SAW.bin
BINARIES += RESO_SQR.bin
BINARIES += SAW_GAP1.bin
BINARIES += SAW_GAP2.bin
BINARIES += SAW_PHASE.bin
BINARIES += SINE_HARM.bin
BINARIES += SWEEPHARM.bin
BINARIES += SYMMETRY.bin
BINARIES += TEE_EKS.bin
BINARIES += THEREMIN.bin
BINARIES += TWO_OP_RAND.bin
BINARIES += TWO_OPFM1.bin
BINARIES += TWO_OPFM2.bin
BINARIES += VIDEOGAME.bin
BINARIES += VIOLIN.bin
BINARIES += VOICE_1.bin
BINARIES += VOICE_2.bin
BINARIES += VOICE_3.bin
BINARIES += VOICE_4.bin
BINARIES += VOICE_5.bin
BINARIES += VOICE_6.bin
BINARIES += VOX_MACH.bin
BINARIES += XFADE.bin

# Must include the VCV plugin Makefile framework
RACK_DIR ?= ../..

# Convenience target for including files in the distributable release
#.PHONY: dist
#dist: all

DISTRIBUTABLES += $(wildcard LICENSE* *.pdf README*) res
include $(RACK_DIR)/plugin.mk
