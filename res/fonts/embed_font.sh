#!/bin/bash
xxd -i Ubuntu-Regular.ttf > Ubuntu-Regular.hpp

printf "#pragma once\n\nnamespace Enigma::Fonts {\n$(cat Ubuntu-Regular.hpp)" > Ubuntu-Regular.hpp
printf "\n\n} // !Enigma::Fonts" >> Ubuntu-Regular.hpp