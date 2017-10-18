cd ..
rm -rf lib/*/
cd lib

echo "\n\nLib deleted\n\n"

# Refresh Catch
mkdir Catch
cd Catch
curl -s https://api.github.com/repos/philsquared/Catch/releases/latest | jq --raw-output '.assets[0] | .browser_download_url' | xargs wget
cd ..

echo "\n\nCatch refreshed\n\n"

# Refresh glad
mkdir glad
cd glad
pip install --user glad
glad --out-path=. --generator=c --local-files --spec=gl
cd ..

echo "\n\nglad Refreshed\n\n"

#Refresh glfw/glm/assimp
git clone --depth=1 https://github.com/glfw/glfw.git
git clone --depth=1 https://github.com/g-truc/glm.git
git clone --depth=1 https://github.com/assimp/assimp.git

echo "\n\n glfw, glm, assimp cloned \n\n"

#Refresh stb_image*
git clone --depth=1 https://github.com/nothings/stb.git
# find . -not -name 'stb_image.h' | xargs rm -rf
mv stb/stb_image.h .
# mv stb/stb_image_write.h .
rm -rf stb

echo "\n\n stb_image refreshed \n\n"

#Remove the git histories. (unneeded)
find . -name '.git' | xargs rm -rf #linux only ...

echo "\n\n git histories destroyed \n\n"
echo "\n\n done \n\n"

cd ..

