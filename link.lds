OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

PHDRS {
    All PT_LOAD;
}

SECTIONS {
    . = 0xffffffff80000000;

    .bigos : {
        KEEP (*(.bigos))
    } : All

    .init : {
        KEEP (*(SORT_NONE(.init)))
    } : All

    .text : {
        *(.text.unlikely .text.*_unlikely .text.unlikely.*)
        *(.text.exit .text.exit.*)
        *(.text.startup .text.startup.*)
        *(.text.hot .text.hot.*)
        *(SORT(.text.sorted.*))
        *(.text .stub .text.* .gnu.linkonce.t.*)
        *(.gnu.warning)
    } : All

    .fini : {
        KEEP (*(SORT_NONE(.fini)))
    } : All

    .rodata : {
        *(.rodata .rodata.* .gnu.linkonce.r.*) 
    } : All

    .rodata1 : {
        *(.rodata1) 
    } : All

    .eh_frame_hdr : {
        *(.eh_frame_hdr)
        *(.eh_frame_entry 
        .eh_frame_entry.*)
    } : All

    .eh_frame : ONLY_IF_RO {
        KEEP (*(.eh_frame)) 
        *(.eh_frame.*) 
    } : All

    .eh_frame : ONLY_IF_RW {
        KEEP (*(.eh_frame)) 
        *(.eh_frame.*) 
    } : All

    .ctors : {
        KEEP (*crtbegin.o(.ctors))
        KEEP (*crtbegin?.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
    } : All

    .dtors : {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*crtbegin?.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
    } : All

    .data : {
        *(.data .data.* .gnu.linkonce.d.*)
        SORT(CONSTRUCTORS)
    } : All

    .4k_area ALIGN(0x1000) : {
        *(.4k_area)
    } : All

    .bss : {
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(. != 0 ? 64 / 8 : 1);
    } : All

    /DISCARD/ : {
        *(.note.GNU-stack) 
        *(.gnu_debuglink) 
        *(.gnu.lto_*) 
        *(.comment)
    }
}