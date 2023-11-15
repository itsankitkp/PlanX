define hook-stop
 # Translate the segment:offset into a physical address
 printf "[%4x:%4x] ", $cs, $eip
 x/i $cs*16+$eip
end
layout asm
layout reg
set architecture i8086
target remote localhost:1234

set history save on
set history size 1000
set debuginfod enabled off
