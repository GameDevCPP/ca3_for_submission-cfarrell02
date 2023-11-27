# Drop-Pod-Game
C++ game for games engineering coursework.

## Installation

### Itch.io
If you are wanting to just play the game without having to get the repo visit our Itch.Io page and follow the instructions there. https://stoketheflame.itch.io/drop-pod

### Download Repo
If starting on a new computer or redownloading the repo from scratch and lib/sfml and/or lib/b2d is empty. Open git bash shell in drop-pod-game directory and run command:

git submodule update --init --recursive

This will download the submodules for the project.
To check if that has worked correctly run command:

git submodule status

Results should show:


Then compile with CMake. This will show an error for developers just ignore.

CMake Warning (dev) at C:/Program Files/CMake/share/cmake-3.22/Modules/FindPackageHandleStandardArgs.cmake:438 (message):
  The package name passed to `find_package_handle_standard_args` (VORBIS)
  does not match the name of the calling package (Vorbis).  This can lead to
  problems in calling code that expects `find_package` result variables
  (e.g., `_FOUND`) to follow a certain pattern.
Call Stack (most recent call first):
  lib/sfml/cmake/Modules/FindVorbis.cmake:25 (find_package_handle_standard_args)
  lib/sfml/cmake/Macros.cmake:279 (find_package)
  lib/sfml/src/SFML/Audio/CMakeLists.txt:70 (sfml_find_package)
This warning is for project developers.  Use -Wno-dev to suppress it.

IGNORE THIS IT SHOULD STILL COMPILE.
