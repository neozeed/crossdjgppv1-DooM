@rem MS-DOS DJGPP v1 GO32 DPMI timer test
@IF NOT EXIST test\doom. (
echo.
echo Executable missing attempting to compile....
@make -f makefile.null
echo.
)
@cd test
@..\..\..\src\msdos-player\binary\i386_x86\msdos.exe ..\..\..\go32\GO32.EXE doom -episode 1
@cd ..