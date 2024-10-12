## How to build
1. Create build directory in project folder and move into it:
```
mkdir build
cd build
```
2. Preset cmake:
```
cmake ../
```
if Qt was installed in system and added to CMake, it shows version, otherwise it tells that qt wasn't install and create directory qt in project folder.
IMPORTANT! Qt version should be 6.6
3. Build project:
```
cmake --build .
```

