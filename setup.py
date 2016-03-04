import sys
from cx_Freeze import setup, Executable

build_exe_options = {"packages": ["os"]}
base="Win32GUI"

setup(	name="NPSerialOscilloscopeGUI",
		version="1.0",
		description="potato",
		options = {"build_exe": build_exe_options},
		executables = [Executable("oscilloscope_gui.py", base=base)])