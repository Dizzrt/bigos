OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

SECTIONS {
    . = 0xffffffff80000000;

    .bigos : {
        KEEP (*(.bigos))
    }

    .init : {
        KEEP (*(SORT_NONE(.init)))
    }

    .text : {
        *(.text.unlikely .text.*_unlikely .text.unlikely.*)
        *(.text.exit .text.exit.*)
        *(.text.startup .text.startup.*)
        *(.text.hot .text.hot.*)
        *(SORT(.text.sorted.*))
        *(.text .stub .text.* .gnu.linkonce.t.*)
        *(.gnu.warning)
    }

    .fini : {
        KEEP (*(SORT_NONE(.fini)))
    }

    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r.*) 
    }

    .rodata1 : {
        *(.rodata1) 
    }

    .eh_frame_hdr : {
        *(.eh_frame_hdr)
        *(.eh_frame_entry 
        .eh_frame_entry.*)
    }

    .eh_frame : ONLY_IF_RO {
        KEEP (*(.eh_frame)) 
        *(.eh_frame.*) 
    }

    .eh_frame : ONLY_IF_RW {
        KEEP (*(.eh_frame)) 
        *(.eh_frame.*) 
    }

    .init_array : {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    }

    .fini_array : {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN (__fini_array_end = .);
    }

    .ctors : {
        KEEP (*crtbegin.o(.ctors))
        KEEP (*crtbegin?.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
    }

    .dtors : {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*crtbegin?.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
    }

    .data : {
        *(.data .data.* .gnu.linkonce.d.*)
        SORT(CONSTRUCTORS)
    }

    .data1 : { 
        *(.data1) 
    }

    .bss : {
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(. != 0 ? 64 / 8 : 1);
    }

    /DISCARD/ : {
        *(.note.GNU-stack) 
        *(.gnu_debuglink) 
        *(.gnu.lto_*) 
        *(.comment)
    }
}