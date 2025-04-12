#pragma once
#ifndef ENIGMA_VERSION_H
#define ENIGMA_VERSION_H

/** major, minor, patch string(needed for Enigma.rc) */
#define ENIGMA_VERSION_STR "3.2.1"

/**  major, minor, patch divided (needed for Enigma.rc) */
#define ENIGMA_VERSION_DIV 3, 2, 1

#define ENIGMA_LICENSE                                                                                 \
  "The MIT License (MIT)\n"                                                                            \
  "Copyright (c) Bader-Eddine Ouaich and other contributors\n"                                         \
  "Permission is hereby granted, free of charge, to any person\n"                                      \
  "obtaining a copy of this software and associated documentation"                                     \
  " files (the Software), to deal in the Software without restriction,\n"                              \
  " including without limitation the rights to use, copy, modify, merge,\n"                            \
  " publish, distribute, sublicense, and/or sell copies of the Software,\n"                            \
  " and to permit persons to whom the Software is furnished to do so,\n"                               \
  " subject to the following conditions:\n"                                                            \
  "The above copyright notice and this permission notice shall be included\n"                          \
  " in all copies or substantial portions of the Software.\n"                                          \
  "THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,\n"                \
  " INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR\n"        \
  " PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE\n"       \
  " FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,\n" \
  " ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n"


/*
* Reminder: 
>> Enigma.rc will include this file to get versions as they change, no need to modify version in any other place than here.
>>> Also make sure you update version in doxyfile using doxywizard

* https://semver.org/
* # when minor version is incremented, patch must reset to 0
* # when major version is incremented, minor and patch must reset to 0
*
* Given a version number MAJOR.MINOR.PATCH, increment the:
* - 1. MAJOR version when you make incompatible API changes (e.g. installer-users have to modify their infrastructure (phone/tablet/PC/web-server/firewall config/etc) in some way), when its incremented, minor and patch are back to 0.
* - 2. MINOR version when you add functionality in a backwards compatible manner (e.g. passing additional data to an already-provisioned API or adding any end-user functionality that does not affect the installation-requirements)
* - 3. PATCH version when you make backwards compatible bug fixes (e.g. fixing any end-user bug that does not affect the installation requirements)
* Additional labels for pre-release and build metadata are available
* as extensions to the MAJOR.MINOR.PATCH format.
*/

#endif // !ENIGMA_VERSION_H
