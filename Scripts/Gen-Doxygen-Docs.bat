::Generates documentation using doxygen Doxyfile config::
::Note: doxygen.exe must be set in the environnement variables Path::
::Note: Doxyfile config file is generated with Doxywizard gui::
@echo off
cd ..
doxygen ./Doxyfile
PAUSE