################################################################
#
# $Id:$
#
# $Log:$
#
CC=gcc

#CFLAGS=-g -Wall -DNORMALUNIX -DLINUX # -DUSEASM
CFLAGS=-O0 -m68000 -DNORMALUNIX -DALLOCA -D__BIG_ENDIAN__
#CFLAGS=-DNORMALUNIX -O -m68000
#CFLAGS=-DNORMALUNIX -O -m68020 -fstrength-reduce -fomit-frame-pointer 
#GCC_OPTION=LFOAMP
#-fstrength-reduce -fomit-frame-pointer 
# crashes with w_wad -finline-functions
#LDFLAGS=-L/usr/X11R6/lib
#LIBS=-lXext -lX11 -lnsl -lm
LDFLAGS=
#LIBS=%LIB%\gnulib.a %LIB%\clib.L %LIB%\floateml.L %LIB%\IOCSLIB.L %LIB%\doslib.L
#LIBS=%LIB%\clib.L %LIB%\floateml.L %LIB%\IOCSLIB.L %LIB%\doslib.L
LIBS=\x68000\lib\libiocs.a \x68000\lib\libdos.a

# subdirectory for objects
O=obj

# not too sophisticated dependency
LIB1=				\
		$(O)/doomdef.o		\
		$(O)/doomstat.o		\
		$(O)/dstrings.o		\
		$(O)/i_system.o		\
		$(O)/i_sound.o		\
		$(O)/i_video.o		\
		$(O)/i_net.o		\
		$(O)/tables.o		\
		$(O)/f_finale.o		\
		$(O)/f_wipe.o 		\
		$(O)/d_main.o			\
		$(O)/d_net.o			\
		$(O)/d_items.o		\
		$(O)/g_game.o	\
		$(O)/m_menu.o			\
		$(O)/m_misc.o

LIB2=		$(O)/m_argv.o	\
		$(O)/m_bbox.o			\
		$(O)/m_cheat.o		\
		$(O)/m_random.o		\
		$(O)/am_map.o			\
		$(O)/p_doors.o		\
		$(O)/p_enemy.o	\
		$(O)/p_floor.o		\
		$(O)/p_inter.o		\
		$(O)/p_lights.o		\
		$(O)/p_map.o			\
		$(O)/p_maputl.o		\
		$(O)/p_plats.o		\
		$(O)/m_swap.o			\
		$(O)/p_ceilng.o
		

LIB3=		$(O)/p_pspr.o			\
		$(O)/p_setup.o		\
		$(O)/p_sight.o		\
		$(O)/p_spec.o			\
		$(O)/p_switch.o		\
		$(O)/p_mobj.o			\
		$(O)/p_telept.o		\
		$(O)/p_tick.o			\
		$(O)/p_saveg.o		\
		$(O)/p_user.o			\
		$(O)/r_bsp.o			\
		$(O)/r_data.o			\
		$(O)/r_draw.o			\
		$(O)/r_main.o			\
		$(O)/r_plane.o		\
		$(O)/r_segs.o

LIB4=		$(O)/r_sky.o			\
		$(O)/r_things.o		\
		$(O)/w_wad.o			\
		$(O)/wi_stuff.o	\
		$(O)/v_video.o		\
		$(O)/st_lib.o			\
		$(O)/st_stuff.o		\
		$(O)/hu_stuff.o		\
		$(O)/hu_lib.o			\
		$(O)/s_sound.o		\
		$(O)/z_zone.o			\
		$(O)/info.o				\
		$(O)/sounds.o	\
		$(O)/missing.o	\
		$(O)/m_swap.o \
		$(O)/mfixed020.o
#		$(O)/m_fixed-LFOAMP.o
#		$(O)/m_fixed.o
#		$(O)/mfixed020.o
#		$(O)/mfixed000.o

LIBOS=		$(O)/mfp.o	\
		$(O)/crtc.o	\
		$(O)/serial.o	\
		$(O)/vc.o	\
		$(O)/tvram.o

NATIVE=		$(O)/m_fixed.o
# this assembles incorrectly with the wrong byte oder... probalby an artifact somewhere
# with this being an i386 vs 68000.
# so instead, I have provided some pre-built assembly files 000 and 020 flavour
# and FYI the core difference.... as you can see from the diff
# the patterns are backwards.
#-       .dc.l $00000000,$40f00000
#+       .dc.l $40f00000,$00000000
#        .even
# ?C1:
#-       .dc.l $00000000,$41e00000
#+       .dc.l $41e00000,$00000000
#        .even
# ?C2:
#-       .dc.l $00000000,$c1e00000
#+       .dc.l $c1e00000,$00000000

all:	$(O)/dosdoom


purge-libs:
	del /F $(O)\lib1.a
	del /F $(O)\lib2.a


clean:
	del x68k\*.o
	del x68k\*.a
	del doom.x

$(O)/lib1.a:	$(LIB1)
	run68 ar /b $(O)/lib1.a $(LIB1)

$(O)/lib2.a:	$(LIB2)
	run68 ar /b $(O)/lib2.a $(LIB2)

$(O)/lib3.a:	$(LIB3)
	run68 ar /b $(O)/lib3.a $(LIB3)

$(O)/lib4.a:	$(LIB4)
	run68 ar /b $(O)/lib4.a $(LIB4)

$(O)/libOS.a:	$(LIBOS)
	run68 ar /b $(O)/libOS.a $(LIBOS)

$(O)/libnative.a:	$(NATIVE)
	run68 ar /b $(O)/libnative.a $(NATIVE)




$(O)/dosdoom:	$(O)/lib1.a $(O)/lib2.a $(O)/lib3.a $(O)/lib4.a $(O)/libos.a $(O)/i_main.o
	$(CC) $(O)/i_main.o $(O)/lib1.a $(O)/lib2.a $(O)/lib3.a $(O)/lib4.a $(O)/libos.a $(LIBS) -o doom.x
#	@echo "you need to manually link :("
#	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(O)/i_main.o \
#	-o $(O)/dosdoom $(LIBS)

$(O)/%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@


x68k/%.o:	%.s
	$(CC) $(CFLAGS) -c $< -o $@

$(O)/i_system.o:
	$(CC) $(CFLAGS) -I. -c x68k\i_system.c -o $@
$(O)/i_video.o:
	$(CC) $(CFLAGS) -I. -c x68k\i_video.c -o $@
$(O)/missing.o:
	$(CC) $(CFLAGS) -I. -c x68k\missing.c -o $@
$(O)/crtc.o:
	$(CC) $(CFLAGS) -I. -c x68k\crtc.c -o $@
$(O)/tvram.o:
	$(CC) $(CFLAGS) -I. -c x68k\tvram.c -o $@
$(O)/display.o:
	$(CC) $(CFLAGS) -I. -c x68k\display.c -o $@
$(O)/irq.o:
	$(CC) $(CFLAGS) -I. -c x68k\irq.c -o $@
$(O)/cynthia.o:
	$(CC) $(CFLAGS) -I. -c x68k\cynthia.c -o $@
$(O)/mfp.o:
	$(CC) $(CFLAGS) -I. -c x68k\mfp.c -o $@
$(O)/serial.o:
	$(CC) $(CFLAGS) -I. -c x68k\serial.c -o $@
$(O)/vc.o:
	$(CC) $(CFLAGS) -I. -c x68k\vc.c -o $@
$(O)/mfixed020.o:
        $(CC) $(CFLAGS) -I. -c x68k\mfixed020.s -o $@
$(O)/m_fixed.o:
	copy m_fixed.o $(O)

$(O)/m_swap.o:
        $(CC) $(CFLAGS) -I. -c m_swap.c -o $@

#$(O)/p_ceilng.o:
#	copy p_ceilng.o $(O)

#############################################################
#
#############################################################