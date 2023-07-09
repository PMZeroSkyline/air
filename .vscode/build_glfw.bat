cd ../third/glfw
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build
cmake --install build --prefix install