################################################################
#
# $Id:$
#
# $Log:$
#
CC=  wcc386  # gcc or g++

#CFLAGS=-g -Wall -DNORMALUNIX -DLINUX # -DUSEASM
#CFLAGS=-g -m80387 -DALLOCA
CFLAGS= #-I. -O
#LDFLAGS=-L/usr/X11R6/lib
#LIBS=-lXext -lX11 -lnsl -lm
LDFLAGS=
LIBS= #-lgcc -lgr -lpc

# subdirectory for objects
O=obj

# not too sophisticated dependency
OBJS=		&
	$(O)\doomdef.obj &
	$(O)\doomstat.obj &
	$(O)\dstrings.obj &
	$(O)\i_sound.obj &
	$(O)\i_net.obj &
	$(O)\tables.obj &
	$(O)\f_finale.obj &
	$(O)\f_wipe.obj &
	$(O)\d_main.obj &
	$(O)\d_net.obj &
	$(O)\d_items.obj &
	$(O)\g_game.obj &
	$(O)\m_menu.obj &
	$(O)\m_misc.obj &
	$(O)\m_argv.obj &
	$(O)\m_bbox.obj &
	$(O)\m_swap.obj &
	$(O)\m_cheat.obj &
	$(O)\m_random.obj &
	$(O)\am_map.obj &
	$(O)\p_ceilng.obj &
	$(O)\p_doors.obj &
	$(O)\p_enemy.obj &
	$(O)\p_floor.obj &
	$(O)\p_inter.obj &
	$(O)\p_lights.obj &
	$(O)\p_map.obj &
	$(O)\p_maputl.obj &
	$(O)\p_plats.obj &
	$(O)\p_pspr.obj &
	$(O)\p_setup.obj &
	$(O)\p_sight.obj &
	$(O)\p_spec.obj &
	$(O)\p_switch.obj &
	$(O)\p_mobj.obj &
	$(O)\p_telept.obj &
	$(O)\p_tick.obj &
	$(O)\p_saveg.obj &
	$(O)\p_user.obj &
	$(O)\r_bsp.obj &
	$(O)\r_data.obj &
	$(O)\r_draw.obj &
	$(O)\r_main.obj &
	$(O)\r_plane.obj &
	$(O)\r_segs.obj &
	$(O)\r_sky.obj &
	$(O)\r_things.obj &
	$(O)\w_wad.obj &
	$(O)\wi_stuff.obj &
	$(O)\v_video.obj &
	$(O)\st_lib.obj &
	$(O)\st_stuff.obj &
	$(O)\hu_stuff.obj &
	$(O)\hu_lib.obj &
	$(O)\s_sound.obj &
	$(O)\z_zone.obj &
	$(O)\info.obj &
	$(O)\sounds.obj &
	$(O)\strncas.obj &
	$(O)\strcas.obj	&
	$(O)\m_fixed.obj	

SYSOBJ	=	$(O)\xmalloc.obj &
		$(O)\i_system.obj &
		$(O)\i_video.obj 

all:	 $(O)\doom.exe

clean:
	-del $(O)\*.obj $(O)\doom.*


$(O)\doom.exe:	$(OBJS) $(SYSOBJ) $(O)\i_main.obj
	wlink @watcom\doom.lnk
	-copy $(O)\doom.exe test
#	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(SYSOBJ) $(O)\i_main.obj -o $(O)\doom.exe $(LIBS)
#	copy /Y $(O)\doom test
#	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(SYSOBJ) $(O)\i_main.obj -o $(O)\doom $(LIBS)
#	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(O)\i_main.obj -o $(O)\dosdoom.exe $(LIBS)


$(O)\doomdef.obj:
	$(CC) $(CFLAGS) doomdef.c /fo=$@
$(O)\i_main.obj:
	$(CC) $(CFLAGS) i_main.c /fo=$@
$(O)\doomstat.obj:
	$(CC) $(CFLAGS) doomstat.c /fo=$@
$(O)\dstrings.obj:
	$(CC) $(CFLAGS) dstrings.c /fo=$@
$(O)\i_sound.obj:
	$(CC) $(CFLAGS) i_sound.c /fo=$@
$(O)\i_net.obj:
	$(CC) $(CFLAGS) i_net.c /fo=$@
$(O)\tables.obj:
	$(CC) $(CFLAGS) tables.c /fo=$@
$(O)\f_finale.obj:
	$(CC) $(CFLAGS) f_finale.c /fo=$@
$(O)\f_wipe.obj:
	$(CC) $(CFLAGS) f_wipe.c /fo=$@
$(O)\d_main.obj:
	$(CC) $(CFLAGS) d_main.c /fo=$@
$(O)\d_net.obj:
	$(CC) $(CFLAGS) d_net.c /fo=$@
$(O)\d_items.obj:
	$(CC) $(CFLAGS) d_items.c /fo=$@
$(O)\g_game.obj:
	$(CC) $(CFLAGS) g_game.c /fo=$@
$(O)\m_menu.obj:
	$(CC) $(CFLAGS) m_menu.c /fo=$@
$(O)\m_misc.obj:
	$(CC) $(CFLAGS) m_misc.c /fo=$@
$(O)\m_argv.obj:
	$(CC) $(CFLAGS) m_argv.c /fo=$@
$(O)\m_bbox.obj:
	$(CC) $(CFLAGS) m_bbox.c /fo=$@
$(O)\m_fixed.obj:
	$(CC) $(CFLAGS) m_fixed.c /fo=$@
$(O)\m_swap.obj:
	$(CC) $(CFLAGS) m_swap.c /fo=$@
$(O)\m_cheat.obj:
	$(CC) $(CFLAGS) m_cheat.c /fo=$@
$(O)\m_random.obj:
	$(CC) $(CFLAGS) m_random.c /fo=$@
$(O)\am_map.obj:
	$(CC) $(CFLAGS) am_map.c /fo=$@
$(O)\p_ceilng.obj:
	$(CC) $(CFLAGS) p_ceilng.c /fo=$@
$(O)\p_doors.obj:
	$(CC) $(CFLAGS) p_doors.c /fo=$@
$(O)\p_enemy.obj:
	$(CC) $(CFLAGS) p_enemy.c /fo=$@
$(O)\p_floor.obj:
	$(CC) $(CFLAGS) p_floor.c /fo=$@
$(O)\p_inter.obj:
	$(CC) $(CFLAGS) p_inter.c /fo=$@
$(O)\p_lights.obj:
	$(CC) $(CFLAGS) p_lights.c /fo=$@
$(O)\p_map.obj:
	$(CC) $(CFLAGS) p_map.c /fo=$@
$(O)\p_maputl.obj:
	$(CC) $(CFLAGS) p_maputl.c /fo=$@
$(O)\p_plats.obj:
	$(CC) $(CFLAGS) p_plats.c /fo=$@
$(O)\p_pspr.obj:
	$(CC) $(CFLAGS) p_pspr.c /fo=$@
$(O)\p_setup.obj:
	$(CC) $(CFLAGS) p_setup.c /fo=$@
$(O)\p_sight.obj:
	$(CC) $(CFLAGS) p_sight.c /fo=$@
$(O)\p_spec.obj:
	$(CC) $(CFLAGS) p_spec.c /fo=$@
$(O)\p_switch.obj:
	$(CC) $(CFLAGS) p_switch.c /fo=$@
$(O)\p_mobj.obj:
	$(CC) $(CFLAGS) p_mobj.c /fo=$@
$(O)\p_telept.obj:
	$(CC) $(CFLAGS) p_telept.c /fo=$@
$(O)\p_tick.obj:
	$(CC) $(CFLAGS) p_tick.c /fo=$@
$(O)\p_saveg.obj:
	$(CC) -I. p_saveg.c /fo=$@
$(O)\p_user.obj:
	$(CC) $(CFLAGS) p_user.c /fo=$@
$(O)\r_bsp.obj:
	$(CC) $(CFLAGS) r_bsp.c /fo=$@
$(O)\r_data.obj:
	$(CC) $(CFLAGS) r_data.c /fo=$@
$(O)\r_draw.obj:
	$(CC) $(CFLAGS) r_draw.c /fo=$@
$(O)\r_main.obj:
	$(CC) $(CFLAGS) r_main.c /fo=$@
$(O)\r_plane.obj:
	$(CC) $(CFLAGS) r_plane.c /fo=$@
$(O)\r_segs.obj:
	$(CC) $(CFLAGS) r_segs.c /fo=$@
$(O)\r_sky.obj:
	$(CC) $(CFLAGS) r_sky.c /fo=$@
$(O)\r_things.obj:
	$(CC) $(CFLAGS) r_things.c /fo=$@
$(O)\w_wad.obj:
	$(CC) $(CFLAGS) w_wad.c /fo=$@
$(O)\wi_stuff.obj:
	$(CC) $(CFLAGS) wi_stuff.c /fo=$@
$(O)\v_video.obj:
	$(CC) $(CFLAGS) v_video.c /fo=$@
$(O)\st_lib.obj:
	$(CC) $(CFLAGS) st_lib.c /fo=$@
$(O)\st_stuff.obj:
	$(CC) $(CFLAGS) st_stuff.c /fo=$@
$(O)\hu_stuff.obj:
	$(CC) $(CFLAGS) hu_stuff.c /fo=$@
$(O)\hu_lib.obj:
	$(CC) $(CFLAGS) hu_lib.c /fo=$@
$(O)\s_sound.obj:
	$(CC) $(CFLAGS) s_sound.c /fo=$@
$(O)\z_zone.obj:
	$(CC) $(CFLAGS) z_zone.c /fo=$@
$(O)\info.obj:
	$(CC) $(CFLAGS) info.c /fo=$@
$(O)\sounds.obj:
	$(CC) $(CFLAGS) sounds.c /fo=$@
$(O)\strncas.obj:
	$(CC) $(CFLAGS) strnca~1.c /fo=$@
$(O)\strcas.obj:
	$(CC) $(CFLAGS) strcas~1.c /fo=$@
$(O)\muldi3.obj:
	$(CC) $(CFLAGS) muldi3.c /fo=$@
$(O)\ashrdi3.obj:
	$(CC) $(CFLAGS) ashrdi3.c /fo=$@

#Go32
#$(O)\i_video.obj:
#	$(CC) $(CFLAGS) go32/i_video.c /fo=$@
#$(O)\i_system.obj:
#	$(CC) $(CFLAGS) go32/i_system.c /fo=$@
#go32 DPMI
#$(O)\i_video.obj:
#	$(CC) $(CFLAGS) go32_dpmi/i_video.c /fo=$@
#$(O)\i_system.obj:
#	$(CC) $(CFLAGS) go32_dpmi/i_system.c /fo=$@
#$(O)\dpmi.obj:
#	$(CC) $(CFLAGS) go32_dpmi/dpmi.c /fo=$@
# NULL
#$(O)\fixed.obj:
#	tasm fixed.asm
#	-copy fixed.obj watcom
$(O)\i_video.obj:
	$(CC) $(CFLAGS) watcom\i_video.c /fo=$@
$(O)\i_system.obj:
	$(CC) $(CFLAGS) watcom\i_system.c /fo=$@

#GCC 1.39
$(O)\xmalloc.obj:
	$(CC) $(CFLAGS) go32\xmalloc.c /fo=$@


#############################################################
#
#############################################################