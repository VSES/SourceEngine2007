ValveSource Addon v1.0
XSI SDK - C++ version of AxisInterp operator 1.0
NOTE: this operator is obsolete and has been replaced by the
HelperBoneOp.
-----------------------------------------------------------------
Windows compilation
-----------------------------
You will find in the current directory the VC++ project file
cpp_simple constraint.dsp that has all the correct settings for compilation.

Make sure you setup msdev with the following settings:

include directories path:
[Install dir]\XSISDK\include

library directory path:
[Install dir]\XSISDK\lib\nt-x86

Linux and IRIX compilation
---------------------------------------
Here is how to compile this example on Linux and IRIX.
In the shell (tcsh), issue the following commands:

	source [Install dir]/.xsi_[version]
	gmake

To clean the build:
	rm -rf obj/ run/

To register the .so:
	xsi -i AxisInterpOp.spdl
