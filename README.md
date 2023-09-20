# GPI Test

This is a test of cocotb's GPI (Generic Procedural Interface) interface. It provides an interface to most common simulators.
Since it can only support what all simulators commonly support, it has less features than e.g. VPI. Nevertheless, it is capable to query signals, get and set their values, register callbacks and more.

To run the test on Icarus Verilog, execute:

	make simulation-iverilog

(Untested) To run the test on VCS, execute:

	make simulation-vcs