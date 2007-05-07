# MODIFY BELOW

#DISABLEGL = -DDISABLE_GL
DISABLEWGL = -DDISABLEWGL
DISABLEDX9 = -DDISABLE_DX9

#BUILD = DEBUG
BUILD = RELEASE

# MODIFY ABOVE
# LEAVE THE REST ALONE (unnless you know what you are doing

CC = g++

ALLBUILDS = -pedantic-errors -Wall -W -Werror $(DISABLEGL) $(DISABLEWGL) $(DISABLEDX9) -D_CRT_SECURE_NO_DEPRECATE -Iinclude
DEBUG = -ggdb $(ALLBUILDS)
RELEASE = -O2 $(ALLBUILDS)

LSDLFLAGS = -lSDL_image -lSDL_mixer -lSDL_gfx -lSDL_ttf -lSDL_net
LGLFLAGS = -lGL -lGLU
WSDLFLAGS = -lSDLmain -lSDL $(LSDLFLAGS)
WGLFLAGS = -lopengl32 -lglu32
WFLAGS = -mwindows -lmingw32 -lwinmm -lcomctl32 -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
DX9FLAGS = -ld3dx9 -ld3d9 -ld3dxof -ldxguid
3DSFLAGS = -l3ds
LOALFLAGS = -lalut -lopenal

CFLAGS = $($(BUILD))
XFLAGS = $(3DSFLAGS) $(WGLFLAGS) $(WFLAGS) $(WSDLFLAGS) -static-libgcc
LFLAGS = `sdl-config --cflags --static-libs` $(3DSFLAGS) $(LGLFLAGS) $(LSDLFLAGS) $(LOALFLAGS) --static-libgcc
LD = ld

SRCS =  src/Zeni/Camera.cpp \
	src/Zeni/Color.cpp \
	src/Zeni/Colors.cpp \
	src/Zeni/Coordinate.cpp \
	src/Zeni/Core.cpp \
	src/Zeni/Font.cpp \
	src/Zeni/Fonts.cpp \
	src/Zeni/Game.cpp \
	src/Zeni/Gamestate.cpp \
	src/Zeni/Light.cpp \
	src/Zeni/main.cpp \
	src/Zeni/Material.cpp \
	src/Zeni/Model.cpp \
	src/Zeni/Net.cpp \
	src/Zeni/Render_Wrapper.cpp \
	src/Zeni/Sound.cpp \
	src/Zeni/Sounds.cpp \
	src/Zeni/Texture.cpp \
	src/Zeni/Textures.cpp \
	src/Zeni/Timer.cpp \
	src/Zeni/Vector3f.cpp \
	src/Zeni/Vertex2f.cpp \
	src/Zeni/Vertex3f.cpp \
	src/Zeni/Vertex_Buffer.cpp \
	src/Zeni/Video.cpp \
	src/Zeni/Video_DX9.cpp \
	src/Zeni/Video_GL.cpp \
	Character.cpp \
	Enemy.cpp \
	Fight.cpp \
	Instructions.cpp \
	Level1.cpp \
	Level2.cpp \
	Loss.cpp \
	Title.cpp \
	Victory.cpp \
	zeniapp.cpp
OBJS = $(SRCS:.cpp=.o)

all: zeniapp.exe

linux: zeniapp

windows: zeniapp.exe

zeniapp: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o zeniapp $(LFLAGS)

zeniapp.exe: $(OBJS)
ifeq ($(DISABLEGL),-DDISABLE_GL)
ifeq ($(DISABLEDX9),-DDISABLE_DX9)
	echo "No rendering engine compiled.  Try again." > /dev/null
else
	$(CC) $(CFLAGS) $(OBJS) -o zeniapp $(XFLAGS) $(DX9FLAGS)
endif
else
ifeq ($(DISABLEDX9),-DDISABLE_DX9)
	$(CC) $(CFLAGS) $(OBJS) -o zeniapp $(XFLAGS) $(WGLFLAGS)
else
	$(CC) $(CFLAGS) $(OBJS) -o zeniapp $(XFLAGS) $(WGLFLAGS) $(DX9FLAGS)
endif
endif

%.o:
	$(CC) $(CFLAGS) -c $*.cpp -o $@

partclean:
	rm -f stdout.txt stderr.txt *~ */*~ */*/*~ */*/*/*~

moreclean: partclean
	rm -f *.o */*.o */*/*.o */*/*/*.o

clean: moreclean
	rm -f zeniapp zeniapp.exe

package: clean
	cd ../ && rm -f Zeniapp.7z && 7z a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=off zeniapp.7z zeniapp/