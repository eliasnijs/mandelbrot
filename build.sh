#!/bin/sh
#!/bin/sh

rm -r build
mkdir -p build

SOURCE="src/main.cpp"
NAME="mandelbrot.html"
VERSION="00"

SET_FRONTEND_TEMPLATE="--shell-file site/index.html"
FILES="${FILES} --preload-file src/shaders/mb.vs"
FILES="${FILES} --preload-file src/shaders/mb.fs"

# includes and libs
INCS="${INCS} -I libs/base -I libs/glad -I libs/imgui -I libs/linmath -I libs/stb"
LIBS="-L ./libs/archives -lm -lGL -lpthread -lglfw -limgui -lstb"

# flags
EMFLAGS="-s USE_GLFW=3 -s FULL_ES3=1 -s USE_WEBGL2=1 -DPLATFORM_WEB"
CPPFLAGS="${CPPFLAGS} -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=2 -DVERSION=\"${VERSION}\""
CFLAGS="${CFLAGS} -g -Wall -Wno-deprecated-declarations -Wno-write-strings -Wno-unused-function ${INCS} ${CPPFLAGS}"

# compiler
CC="em++"

# building
echo "[build.sh]# building $NAME"
echo ${CC} $CFLAGS ${EMFLAGS} ${FILES} ${CONFIG} -o "build/$NAME" $SOURCE $LIBS $SET_FRONTEND_TEMPLATE
${CC} $CFLAGS ${EMFLAGS} ${FILES} ${CONFIG} -o "build/$NAME" $SOURCE $LIBS $SET_FRONTEND_TEMPLATE
echo "[build.sh]# finished building"

