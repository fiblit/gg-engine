#!/bin/bash

printf "%-45s" "Removing old 3rd-party libs..."
{
    if [ pwd == .*"gg-engine/lib/"]; then
        cd ..
    fi
    if [pwd != .*"gg-engine"]; then
        printf "ERROR: wrong dir\ngoto gg-engine/lib or gg-engine"
        exit
    fi
    rm -rf lib/*/
    cd lib
} &> /dev/null
printf "%15s\n" "Done."
printf "%-45s\n" "Downloading latest 3rd-party libs..."

# Refresh Catch
printf "%-45s" "Downloading latest catch.hpp..."
{
    mkdir Catch
    cd Catch
    curl -s https://api.github.com/repos/philsquared/Catch/releases/latest | jq --raw-output '.assets[0] | .browser_download_url' | xargs wget
    cd ..
} &> /dev/null
printf "%15s\n" "Done."

# Refresh glad
printf "%-45s" "Install glad and generate latest..."
{
    mkdir glad
    cd glad
    pip install --user glad
    glad --out-path=. --generator=c --local-files --spec=gl
    cd ..
} &> /dev/null
printf "%15s\n" "Done."

#Refresh glfw/glm/assimp
printf "%-45s" "Downloading latest version of glfw..."
git clone --depth=1 https://github.com/glfw/glfw.git &> /dev/null
printf "%15s\n" "Done."
printf "%-45s" "Downloading latest version of glm..."
git clone --depth=1 https://github.com/g-truc/glm.git &> /dev/null
printf "%15s\n" "Done."
printf "%-45s" "Downloading latest version of assimp..."
git clone --depth=1 https://github.com/assimp/assimp.git &> /dev/null
printf "%15s\n" "Done."

#Refresh stb_image*
printf "%-45s" "Downloading latest version of stb_image.h..."
{
    git clone --depth=1 https://github.com/nothings/stb.git
    # find . -not -name 'stb_image.h' | xargs rm -rf
    mv stb/stb_image.h .
    # mv stb/stb_image_write.h .
    rm -rf stb
} &> /dev/null
printf "%15s\n" "Done."

#Remove the git histories. (unneeded)
printf "%-45s" "Cleaning libs..."
{
    cd ..
    find lib -name '.git' | xargs rm -rf
} &> /dev/null
printf "%15s\n" "Done."

