-- 
-- File: xmake.lua
-- Created by Dizzrt on 2023/05/03.
-- 
-- Copyright (C) 2023 The BigOS Authors.
-- Licensed under the GNU General Public License v3.0 only.
-- 

set_xmakever("2.7.8")

set_project("bigos")
set_version("0.1.0")

includes("toolchains.lua")

set_languages("c17","cxx17")
set_toolchains("x86_64-elf-gcc")
add_rules("mode.debug", "mode.release")

add_includedirs("$(projectdir)/include")
add_includedirs("$(projectdir)/c++/include")
add_includedirs("$(projectdir)/c++/libsupc++/include")
add_includedirs("$(projectdir)/drivers/include")

add_cxxflags("-mno-sse","-mno-sse2", "-mno-mmx", "-mcmodel=kernel", "-ffreestanding", "-mno-red-zone", "-fno-rtti", "-fno-exceptions")

add_asflags("-mno-sse","-mno-sse2", "-mno-mmx", "-mcmodel=kernel", "-ffreestanding", "-mno-red-zone", "-fno-rtti", "-fno-exceptions")

target("kernel")
    set_kind("binary")
    -- O2 optimize
    set_optimize("faster")
    add_files("kernel/**.cc")
    add_files("ktl/**.cc")
    add_files("drivers/**.cc")
    add_files("c++/**.cc")
    add_files("mm/**.cc")
    add_files("irq/**.cc")
    add_files("irq/**.s")

    on_link(function (target) 
        local objs_table = target:objectfiles()
        table.insert(objs_table,1,path.translate("$(projectdir)/lib/crtbegin.o"))
        table.insert(objs_table,1,path.translate("$(projectdir)/lib/crti.o"))
        table.insert(objs_table,1,path.translate("$(projectdir)/lib/crt0.o"))
        table.insert(objs_table,path.translate("$(projectdir)/lib/crtend.o"))
        table.insert(objs_table,path.translate("$(projectdir)/lib/crtn.o"))

        local ld_flags = "-nostdlib -lgcc"
        local ld_script = path.translate("$(projectdir)/link.lds")

        local objs = table.concat(objs_table," ")
        local lib_dir = path.translate("$(projectdir)/lib")
        local output_path = path.translate("$(buildir)/kernel")

        os.exec("x86_64-elf-ld %s -L%s -T %s %s -o %s",ld_flags,lib_dir,ld_script,objs,output_path)
    end)
