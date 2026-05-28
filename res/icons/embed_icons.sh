#!/bin/bash
# icon src: https://icon-sets.iconify.design/?query=vault
for file in *.png; do
    echo "Processing $file"
    new_file="${file%.png}.hpp"
    xxd -i $file > $new_file
    printf "#pragma once\n\nnamespace Enigma::Textures {\n$(cat $new_file)" > $new_file
    printf "\n\n} // !Enigma::Textures" >> $new_file
done
