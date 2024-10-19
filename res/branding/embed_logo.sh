#!/bin/bash
xxd -i Logo.png > Logo.hpp
printf "#pragma once\n\nnamespace Enigma::Textures {\n$(cat Logo.hpp)" > Logo.hpp
printf "\n\n} // !Enigma::Textures" >> Logo.hpp