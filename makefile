#############################################################################
#
# WinAVR makefile written by Eric B. Weddington, J rg Wunsch, et al.
# Released to the Public Domain
# Please read the make user manual!
#
# Additional material for this makefile was submitted by:
# Tim Henigan
# Peter Fleury
# Reiner Patommel
# Sander Pool
# Frederik Rouleau
# Markus Pfaff
#
# This file version is a proposal to be used for the AVR gcc tutorial
# found at http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
# (tutorial in german only, sorry).
#
#############################################################################
#
# Usage
#######
#
# 1. Before compiling please tune all settings marked with ==> to
# match your requirements (up to the following <==).
# Settings not marked with ==> ... <== may be left unchanged usually.
#
# 2. On command line:
#
# make = Make software.
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make coff = Convert ELF to AVR COFF (for use with AVR Studio 3.x or
# VMLAB).
#
# make extcoff = Convert ELF to AVR Extended COFF (for use with AVR Studio
# 4.07 or greater).
#
# make program = Download the hex file to the device, using avrdude. Please
# customize the avrdude settings below first!
#
# make filename.s = Just compile filename.c into the assembler code only
#
# To rebuild your project do "make clean" then "make all".
#
# !!! Note: TO MODIFY THE FILE FOR SPECIFIC APPLICATION, CASES WITH "####" SHOULD BE MODIFIED
#
#############################################################################
# ==> define the name of your project and main source file (without extension)
#
# The gcc tutorial http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
# uses main.c as the main file, so you have to use the name main here.
# On unix like systems (linux etc.) all file names are case sensitive!
PROJECTNAME=ESP_Test

MCU = atmega32			#### Type of uC

F_OSC = 7372800UL		#### F_CPU

# Output format. (can be srec, ihex, binary)
FORMAT = ihex# Target file name (without extension).

TARGET = OUTPUT

CSRC = $(PROJECTNAME).c AuxLib.c LCD.c UART_4.c ESP8266.c	#### All External library (C files) should be added

ASRC =

OPT = 1				####Level of Optimization (0,1,2,3,s)

AVRDUDE_PROGRAMMER = usbasp 	#### Programmer

AVRDUDE_PORT = /dev/ttyUSB0	#### Linux: programmer like AVRISP mkII connected to USB

DEBUG = dwarf-2

EXTRAINCDIRS = .

CSTANDARD = -std=gnu99

EXTRA_CDEFS =

EXTRA_CUNDEFS =

EXTRA_COPTIONS = -Wstrict-prototypes

EXTRA_CPPOPTIONS =



CFLAGS=
CFLAGS= -g$(DEBUG)
CFLAGS+= $(foreach ICDEF,$(EXTRA_CDEFS),-D"$(ICDEF)")
CFLAGS+= $(foreach ICUNDEF,$(EXTRA_CUNDEFS),-U$(ICUNDEF))
CFLAGS+= -O$(OPT)
CFLAGS+= -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS+= -Wall
CFLAGS+= -Wa,-adhlns=$<.lst
CFLAGS+= $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS+= $(CSTANDARD)
CFLAGS+= $(EXTRA_COPTIONS)
CFLAGS+= -DF_OSC=$(F_OSC)
CFLAGS+= -DF_CPU=$(F_OSC)


CPPFLAGS=
CPPFLAGS= -g$(DEBUG)
CPPFLAGS+= $(foreach ICDEF,$(EXTRA_CDEFS),-D"$(ICDEF)")
CPPFLAGS+= $(foreach ICUNDEF,$(EXTRA_CUNDEFS),-U$(ICUNDEF))
CPPFLAGS+= -O$(OPT)
CPPFLAGS+= -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CPPFLAGS+= -Wall
CPPFLAGS+= -Wa,-adhlns=$<.lst
CPPFLAGS+= $(patsubst %,-I%,$(EXTRAINCDIRS))
CPPFLAGS+= $(EXTRA_CPPOPTIONS)
CPPFLAGS+= -DF_OSC=$(F_OSC)
CPPFLAGS+= -DF_CPU=$(F_OSC)

ASFLAGS = -Wa,-adhlns=$<.lst,-gstabsASFLAGS += -DF_OSC=$(F_OSC)

PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

PRINTF_LIB =	# full functionality except but no floating point

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

#
SCANF_LIB =	# full functionality except floating point, max. width 255


MATH_LIB = -lm

EXTMEMOPTS =

LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)


AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

AVRDUDE_FLAGS= -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS+= $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS+= $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS+= $(AVRDUDE_ERASE_COUNTER)
AVRDUDE_FLAGS+= -E noreset

# Define directories, if needed.
DIRAVR = c:/winavr
DIRAVRBIN = $(DIRAVR)/bin
DIRAVRUTILS = $(DIRAVR)/utils/bin
DIRINC = .
DIRLIB = $(DIRAVR)/avr/lib

# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
COPY = cp

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = -------- end --------
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:


# Define all object files.
OBJ = $(CSRC:.c=.o) $(ASRC:.S=.o)

# Compiler flags to generate dependency files.
### GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d
GENDEPFLAGS = -MD -MP -MF .deppp/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)


# Default target.
all: begin gccversion sizebefore build sizeafter finished end

build: elf hex eep lss sym

elf:$(TARGET).elf
hex:$(TARGET).hex
eep:$(TARGET).eep
lss:$(TARGET).lss
sym:$(TARGET).sym

# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on# the following magic strings to be generated by the compile job.
begin:
	@echo
	@echo $(MSG_BEGIN)
finished:
	@echo $(MSG_ERRORS_NONE)
end:
	@echo $(MSG_END)
	@echo

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf

sizebefore:
		@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
		@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi


# Display compiler version information.
gccversion :
		@$(CC) --version

# Program the device.
program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000

coff: $(TARGET).elf
	@echo
	@echo $(MSG_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof

extcoff: $(TARGET).elf
	@echo
	@echo $(MSG_EXTENDED_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@
%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

# Compile: create object files from C source files.
%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# Compile: create object files from C++ source files.
%.o : %.cpp
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CPPFLAGS) $< -o $@

# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Compile: create assembler files from C++ source files.
%.s : %.cpp
	$(CC) -S $(ALL_CPPFLAGS) $< -o $@

# Assemble: create object files from assembler source files.
%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Target: clean project.
clean: begin clean_list finished end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) .deppp/*
	$(REMOVE) *.bak *.BAK *~ *.o *.s *.lst

# Include the dependency files.
-include $(shell mkdir .deppp 2>/dev/null) $(wildcard .deppp/*)

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff \
clean clean_list program
