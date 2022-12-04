@echo on
mkdir build-web
cd build-web
emcmake cmake -G "MinGW Makefiles" .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html"  && ^
emmake make && ^
echo Starting server http://localhost:8000/raylib_jam_256_unexpected.html && ^
python -m http.server 8000
