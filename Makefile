CC=gcc
CXX=g++
CFLAGS+=-D_FILE_OFFSET_BITS=64
CXXFLAGS+=-Wall -D_FILE_OFFSET_BITS=64
LDFLAGS+=
LIB_NAMES=crc32 support guid partnotes gptpartnotes gptpart basicmbr mbr gpt bsd parttypes attributes diskio diskio-unix
LIB_SRCS=$(NAMES:=.cc)
LIB_OBJS=$(LIB_NAMES:=.o)
LIB_HEADERS=$(LIB_NAMES:=.h)
DEPEND= makedepend $(CXXFLAGS)

all:	gdisk sgdisk

gdisk:	$(LIB_OBJS) gdisk.o gpttext.o
	$(CXX) $(LIB_OBJS) gdisk.o gpttext.o $(LDFLAGS) -luuid -o gdisk

sgdisk: $(LIB_OBJS) sgdisk.o
	$(CXX) $(LIB_OBJS) sgdisk.o $(LDFLAGS) -luuid -lpopt -o sgdisk

lint:	#no pre-reqs
	lint $(SRCS)

clean:	#no pre-reqs
	rm -f core *.o *~ gdisk sgdisk

# what are the source dependencies
depend: $(SRCS)
	$(DEPEND) $(SRCS)

$(OBJS):
	$(CRITICAL_CXX_FLAGS) 

# DO NOT DELETE

attributes.o: /usr/include/stdint.h /usr/include/features.h
attributes.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
attributes.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
attributes.o: /usr/include/bits/wchar.h /usr/include/stdio.h
attributes.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
attributes.o: /usr/include/libio.h /usr/include/_G_config.h
attributes.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
attributes.o: /usr/include/bits/sys_errlist.h attributes.h support.h
attributes.o: /usr/include/stdlib.h /usr/include/sys/types.h
attributes.o: /usr/include/time.h /usr/include/endian.h
attributes.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
attributes.o: /usr/include/sys/select.h /usr/include/bits/select.h
attributes.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
attributes.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
attributes.o: /usr/include/alloca.h
bsd.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
bsd.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
bsd.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
bsd.o: /usr/include/bits/typesizes.h /usr/include/libio.h
bsd.o: /usr/include/_G_config.h /usr/include/wchar.h
bsd.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
bsd.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
bsd.o: /usr/include/endian.h /usr/include/bits/endian.h
bsd.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
bsd.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
bsd.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
bsd.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
bsd.o: /usr/include/stdint.h /usr/include/bits/wchar.h /usr/include/fcntl.h
bsd.o: /usr/include/bits/fcntl.h /usr/include/sys/stat.h
bsd.o: /usr/include/bits/stat.h /usr/include/errno.h
bsd.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
bsd.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
bsd.o: /usr/include/asm-generic/errno-base.h support.h bsd.h gptpart.h
bsd.o: parttypes.h guid.h /usr/include/uuid/uuid.h /usr/include/sys/time.h
bsd.o: attributes.h diskio.h /usr/include/sys/ioctl.h
bsd.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
bsd.o: /usr/include/asm-generic/ioctls.h /usr/include/linux/ioctl.h
bsd.o: /usr/include/asm/ioctl.h /usr/include/asm-generic/ioctl.h
bsd.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
crc32.o: /usr/include/stdio.h /usr/include/features.h
crc32.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
crc32.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
crc32.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
crc32.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
crc32.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
crc32.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
crc32.o: /usr/include/endian.h /usr/include/bits/endian.h
crc32.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
crc32.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
crc32.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
crc32.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h crc32.h
crc32.o: /usr/include/stdint.h /usr/include/bits/wchar.h
diskio.o: /usr/include/sys/ioctl.h /usr/include/features.h
diskio.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
diskio.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
diskio.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
diskio.o: /usr/include/asm-generic/ioctls.h /usr/include/linux/ioctl.h
diskio.o: /usr/include/asm/ioctl.h /usr/include/asm-generic/ioctl.h
diskio.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
diskio.o: /usr/include/stdint.h /usr/include/bits/wchar.h
diskio.o: /usr/include/errno.h /usr/include/bits/errno.h
diskio.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
diskio.o: /usr/include/asm-generic/errno.h
diskio.o: /usr/include/asm-generic/errno-base.h /usr/include/fcntl.h
diskio.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
diskio.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
diskio.o: /usr/include/time.h /usr/include/endian.h
diskio.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
diskio.o: /usr/include/sys/select.h /usr/include/bits/select.h
diskio.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
diskio.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
diskio.o: /usr/include/sys/stat.h /usr/include/bits/stat.h support.h
diskio.o: /usr/include/stdlib.h /usr/include/alloca.h diskio.h parttypes.h
diskio.o: guid.h /usr/include/uuid/uuid.h /usr/include/sys/time.h gpt.h
diskio.o: gptpart.h attributes.h mbr.h partnotes.h bsd.h
diskio-unix.o: /usr/include/sys/ioctl.h /usr/include/features.h
diskio-unix.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
diskio-unix.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
diskio-unix.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
diskio-unix.o: /usr/include/asm-generic/ioctls.h /usr/include/linux/ioctl.h
diskio-unix.o: /usr/include/asm/ioctl.h /usr/include/asm-generic/ioctl.h
diskio-unix.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
diskio-unix.o: /usr/include/string.h /usr/include/xlocale.h
diskio-unix.o: /usr/include/stdint.h /usr/include/bits/wchar.h
diskio-unix.o: /usr/include/errno.h /usr/include/bits/errno.h
diskio-unix.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
diskio-unix.o: /usr/include/asm-generic/errno.h
diskio-unix.o: /usr/include/asm-generic/errno-base.h /usr/include/fcntl.h
diskio-unix.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
diskio-unix.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
diskio-unix.o: /usr/include/time.h /usr/include/endian.h
diskio-unix.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
diskio-unix.o: /usr/include/sys/select.h /usr/include/bits/select.h
diskio-unix.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
diskio-unix.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
diskio-unix.o: /usr/include/sys/stat.h /usr/include/bits/stat.h diskio.h
diskio-unix.o: support.h /usr/include/stdlib.h /usr/include/alloca.h
diskio-unix.o: parttypes.h guid.h /usr/include/uuid/uuid.h
diskio-unix.o: /usr/include/sys/time.h
diskio-windows.o: /usr/include/stdio.h /usr/include/features.h
diskio-windows.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
diskio-windows.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
diskio-windows.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
diskio-windows.o: /usr/include/libio.h /usr/include/_G_config.h
diskio-windows.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
diskio-windows.o: /usr/include/bits/sys_errlist.h /usr/include/stdint.h
diskio-windows.o: /usr/include/bits/wchar.h /usr/include/errno.h
diskio-windows.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
diskio-windows.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
diskio-windows.o: /usr/include/asm-generic/errno-base.h /usr/include/fcntl.h
diskio-windows.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
diskio-windows.o: /usr/include/time.h /usr/include/endian.h
diskio-windows.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
diskio-windows.o: /usr/include/sys/select.h /usr/include/bits/select.h
diskio-windows.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
diskio-windows.o: /usr/include/sys/sysmacros.h
diskio-windows.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/stat.h
diskio-windows.o: /usr/include/bits/stat.h support.h /usr/include/stdlib.h
diskio-windows.o: /usr/include/alloca.h diskio.h /usr/include/sys/ioctl.h
diskio-windows.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
diskio-windows.o: /usr/include/asm-generic/ioctls.h
diskio-windows.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
diskio-windows.o: /usr/include/asm-generic/ioctl.h
diskio-windows.o: /usr/include/bits/ioctl-types.h
diskio-windows.o: /usr/include/sys/ttydefaults.h parttypes.h guid.h
diskio-windows.o: /usr/include/uuid/uuid.h /usr/include/sys/time.h
gdisk.o: /usr/include/stdio.h /usr/include/features.h
gdisk.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
gdisk.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
gdisk.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
gdisk.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
gdisk.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
gdisk.o: /usr/include/string.h /usr/include/xlocale.h mbr.h
gdisk.o: /usr/include/stdint.h /usr/include/bits/wchar.h
gdisk.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
gdisk.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
gdisk.o: /usr/include/sys/select.h /usr/include/bits/select.h
gdisk.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
gdisk.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
gdisk.o: gptpart.h support.h /usr/include/stdlib.h /usr/include/alloca.h
gdisk.o: parttypes.h guid.h /usr/include/uuid/uuid.h /usr/include/sys/time.h
gdisk.o: attributes.h partnotes.h gpt.h bsd.h diskio.h
gdisk.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
gdisk.o: /usr/include/asm/ioctls.h /usr/include/asm-generic/ioctls.h
gdisk.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
gdisk.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
gdisk.o: /usr/include/sys/ttydefaults.h gpttext.h
gpt.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
gpt.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
gpt.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
gpt.o: /usr/include/bits/typesizes.h /usr/include/libio.h
gpt.o: /usr/include/_G_config.h /usr/include/wchar.h
gpt.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
gpt.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
gpt.o: /usr/include/endian.h /usr/include/bits/endian.h
gpt.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
gpt.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
gpt.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
gpt.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
gpt.o: /usr/include/stdint.h /usr/include/bits/wchar.h /usr/include/fcntl.h
gpt.o: /usr/include/bits/fcntl.h /usr/include/string.h /usr/include/xlocale.h
gpt.o: /usr/include/math.h /usr/include/bits/huge_val.h
gpt.o: /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h
gpt.o: /usr/include/bits/inf.h /usr/include/bits/nan.h
gpt.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
gpt.o: /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/errno.h
gpt.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
gpt.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
gpt.o: /usr/include/asm-generic/errno-base.h crc32.h gpt.h gptpart.h
gpt.o: support.h parttypes.h guid.h /usr/include/uuid/uuid.h
gpt.o: /usr/include/sys/time.h attributes.h mbr.h partnotes.h diskio.h
gpt.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
gpt.o: /usr/include/asm/ioctls.h /usr/include/asm-generic/ioctls.h
gpt.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
gpt.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
gpt.o: /usr/include/sys/ttydefaults.h bsd.h
gptpart.o: /usr/include/string.h /usr/include/features.h
gptpart.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
gptpart.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
gptpart.o: /usr/include/xlocale.h /usr/include/stdio.h
gptpart.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
gptpart.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
gptpart.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
gptpart.o: gptpart.h /usr/include/stdint.h /usr/include/bits/wchar.h
gptpart.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
gptpart.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
gptpart.o: /usr/include/sys/select.h /usr/include/bits/select.h
gptpart.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
gptpart.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
gptpart.o: support.h /usr/include/stdlib.h /usr/include/alloca.h parttypes.h
gptpart.o: guid.h /usr/include/uuid/uuid.h /usr/include/sys/time.h
gptpart.o: attributes.h
gpttext.o: /usr/include/string.h /usr/include/features.h
gpttext.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
gpttext.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
gpttext.o: /usr/include/xlocale.h /usr/include/errno.h
gpttext.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
gpttext.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
gpttext.o: /usr/include/asm-generic/errno-base.h /usr/include/stdint.h
gpttext.o: /usr/include/bits/wchar.h /usr/include/limits.h
gpttext.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gpttext.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gpttext.o: attributes.h gpttext.h gpt.h /usr/include/sys/types.h
gpttext.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
gpttext.o: /usr/include/time.h /usr/include/endian.h
gpttext.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
gpttext.o: /usr/include/sys/select.h /usr/include/bits/select.h
gpttext.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
gpttext.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
gpttext.o: gptpart.h support.h /usr/include/stdlib.h /usr/include/alloca.h
gpttext.o: parttypes.h guid.h /usr/include/uuid/uuid.h
gpttext.o: /usr/include/sys/time.h mbr.h partnotes.h diskio.h
gpttext.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
gpttext.o: /usr/include/asm/ioctls.h /usr/include/asm-generic/ioctls.h
gpttext.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
gpttext.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
gpttext.o: /usr/include/sys/ttydefaults.h bsd.h
guid.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
guid.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
guid.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
guid.o: /usr/include/bits/typesizes.h /usr/include/libio.h
guid.o: /usr/include/_G_config.h /usr/include/wchar.h
guid.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
guid.o: /usr/include/time.h guid.h /usr/include/stdint.h
guid.o: /usr/include/bits/wchar.h /usr/include/uuid/uuid.h
guid.o: /usr/include/sys/types.h /usr/include/endian.h
guid.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
guid.o: /usr/include/sys/select.h /usr/include/bits/select.h
guid.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
guid.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
guid.o: /usr/include/sys/time.h support.h /usr/include/stdlib.h
guid.o: /usr/include/alloca.h
mbr.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
mbr.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
mbr.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
mbr.o: /usr/include/bits/typesizes.h /usr/include/libio.h
mbr.o: /usr/include/_G_config.h /usr/include/wchar.h
mbr.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
mbr.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
mbr.o: /usr/include/endian.h /usr/include/bits/endian.h
mbr.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
mbr.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
mbr.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
mbr.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
mbr.o: /usr/include/stdint.h /usr/include/bits/wchar.h /usr/include/fcntl.h
mbr.o: /usr/include/bits/fcntl.h /usr/include/string.h /usr/include/xlocale.h
mbr.o: /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/errno.h
mbr.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
mbr.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
mbr.o: /usr/include/asm-generic/errno-base.h mbr.h gptpart.h support.h
mbr.o: parttypes.h guid.h /usr/include/uuid/uuid.h /usr/include/sys/time.h
mbr.o: attributes.h partnotes.h gpt.h bsd.h diskio.h /usr/include/sys/ioctl.h
mbr.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
mbr.o: /usr/include/asm-generic/ioctls.h /usr/include/linux/ioctl.h
mbr.o: /usr/include/asm/ioctl.h /usr/include/asm-generic/ioctl.h
mbr.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
partnotes.o: /usr/include/stdio.h /usr/include/features.h
partnotes.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
partnotes.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
partnotes.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
partnotes.o: /usr/include/libio.h /usr/include/_G_config.h
partnotes.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
partnotes.o: /usr/include/bits/sys_errlist.h partnotes.h gpt.h
partnotes.o: /usr/include/stdint.h /usr/include/bits/wchar.h
partnotes.o: /usr/include/sys/types.h /usr/include/time.h
partnotes.o: /usr/include/endian.h /usr/include/bits/endian.h
partnotes.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
partnotes.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
partnotes.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
partnotes.o: /usr/include/bits/pthreadtypes.h gptpart.h support.h
partnotes.o: /usr/include/stdlib.h /usr/include/alloca.h parttypes.h guid.h
partnotes.o: /usr/include/uuid/uuid.h /usr/include/sys/time.h attributes.h
partnotes.o: mbr.h diskio.h /usr/include/sys/ioctl.h
partnotes.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
partnotes.o: /usr/include/asm-generic/ioctls.h /usr/include/linux/ioctl.h
partnotes.o: /usr/include/asm/ioctl.h /usr/include/asm-generic/ioctl.h
partnotes.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
partnotes.o: bsd.h
parttypes.o: /usr/include/string.h /usr/include/features.h
parttypes.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
parttypes.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
parttypes.o: /usr/include/xlocale.h /usr/include/stdint.h
parttypes.o: /usr/include/bits/wchar.h /usr/include/stdio.h
parttypes.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
parttypes.o: /usr/include/libio.h /usr/include/_G_config.h
parttypes.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
parttypes.o: /usr/include/bits/sys_errlist.h parttypes.h
parttypes.o: /usr/include/stdlib.h /usr/include/sys/types.h
parttypes.o: /usr/include/time.h /usr/include/endian.h
parttypes.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
parttypes.o: /usr/include/sys/select.h /usr/include/bits/select.h
parttypes.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
parttypes.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
parttypes.o: /usr/include/alloca.h support.h guid.h /usr/include/uuid/uuid.h
parttypes.o: /usr/include/sys/time.h
sgdisk.o: /usr/include/stdio.h /usr/include/features.h
sgdisk.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
sgdisk.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
sgdisk.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
sgdisk.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
sgdisk.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
sgdisk.o: /usr/include/popt.h /usr/include/errno.h /usr/include/bits/errno.h
sgdisk.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
sgdisk.o: /usr/include/asm-generic/errno.h
sgdisk.o: /usr/include/asm-generic/errno-base.h /usr/include/stdint.h
sgdisk.o: /usr/include/bits/wchar.h mbr.h /usr/include/sys/types.h
sgdisk.o: /usr/include/time.h /usr/include/endian.h
sgdisk.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
sgdisk.o: /usr/include/sys/select.h /usr/include/bits/select.h
sgdisk.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
sgdisk.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
sgdisk.o: gptpart.h support.h /usr/include/stdlib.h /usr/include/alloca.h
sgdisk.o: parttypes.h guid.h /usr/include/uuid/uuid.h /usr/include/sys/time.h
sgdisk.o: attributes.h partnotes.h gpt.h bsd.h diskio.h
sgdisk.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
sgdisk.o: /usr/include/asm/ioctls.h /usr/include/asm-generic/ioctls.h
sgdisk.o: /usr/include/linux/ioctl.h /usr/include/asm/ioctl.h
sgdisk.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
sgdisk.o: /usr/include/sys/ttydefaults.h
support.o: /usr/include/stdio.h /usr/include/features.h
support.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
support.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
support.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
support.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
support.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
support.o: /usr/include/stdint.h /usr/include/bits/wchar.h
support.o: /usr/include/errno.h /usr/include/bits/errno.h
support.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
support.o: /usr/include/asm-generic/errno.h
support.o: /usr/include/asm-generic/errno-base.h /usr/include/fcntl.h
support.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
support.o: /usr/include/time.h /usr/include/endian.h
support.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
support.o: /usr/include/sys/select.h /usr/include/bits/select.h
support.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
support.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
support.o: /usr/include/string.h /usr/include/xlocale.h
support.o: /usr/include/sys/stat.h /usr/include/bits/stat.h support.h
support.o: /usr/include/stdlib.h /usr/include/alloca.h
testguid.o: guid.h /usr/include/stdint.h /usr/include/features.h
testguid.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
testguid.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
testguid.o: /usr/include/bits/wchar.h /usr/include/uuid/uuid.h
testguid.o: /usr/include/sys/types.h /usr/include/bits/types.h
testguid.o: /usr/include/bits/typesizes.h /usr/include/time.h
testguid.o: /usr/include/endian.h /usr/include/bits/endian.h
testguid.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
testguid.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
testguid.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
testguid.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/time.h
testguid.o: parttypes.h /usr/include/stdlib.h /usr/include/alloca.h support.h
