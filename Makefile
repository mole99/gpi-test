
SRC = lib/gpi/GpiCbHdl.cpp lib/gpi/GpiCommon.cpp lib/vpi/VpiImpl.cpp lib/vpi/VpiCbHdl.cpp lib/gpi_log/gpi_logging.cpp fstdumper.cpp

OBJ = $(addsuffix .o, $(basename $(SRC)))

$(info $(OBJ))

DEFINES = -DCOCOTBVPI_EXPORTS

WARNS = -Wall \
	-Wextra \
	-Wcast-qual \
	-Wwrite-strings \
	-Wconversion \
	-Wno-missing-field-initializers

CFLAGS = $(WARNS) -fPIC -Iinclude

CXX_WARNS = $(WARNS) -Wnon-virtual-dtor -Woverloaded-virtual

CXXFLAGS =  $(CXX_WARNS) \
      -std=c++11 \
      -fvisibility=hidden \
      -fvisibility-inlines-hidden \
      -fPIC -Iinclude

LDFLAGS = -shared

TESTBENCH = $(wildcard testbench/*.sv)

$(OBJDIR):
	@mkdir $(OBJDIR)

%.o: %.c
	@echo Compiling $(<F)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

%.o: %.cpp
	@echo Compiling $(<F)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(DEFINES)

libgpi.so: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

libgpi.so.vpi : libgpi.so
	@cp $< $@

# Simulators

Vtop.vvp: $(TESTBENCH)
	iverilog -o $@ -s div_int_tb $(TESTBENCH) -g2012

simulation-iverilog: libgpi.so.vpi Vtop.vvp
	vvp -M . -mlibgpi.so Vtop.vvp

simulation-vcs: libgpi.so
	vcs -sverilog -full64 +vpi -load ./libgpi.so -debug_acc+all $(TESTBENCH)
	./simv

clean:
	rm $(OBJ) libgpi.so libgpi.so.vpi Vtop.vvp simv
