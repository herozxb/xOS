#include "stdio.h"
#include "elf.h"
uint32_t parse_elf(uintptr_t addr_exec) {

    uintptr_t addr = (uintptr_t)bin_img;

    // parse elf header
    elfhdr eh;
    memcpy((void*)&eh,(void*)addr,sizeof(elfhdr));
#ifdef DEBUG
    print_elfhdr(&eh);
    show_one_sector(addr);
#endif

    if (eh.e_magic != ELF_MAGIC){
        put_error("Bad elf file!");
        for(;;){}
    }

    // parse program header
    prghdr ph;
    for (int i = 0, offset = eh.e_phoff;
            i < eh.e_phnum; i++, offset += eh.e_phentsize){
        memcpy((void*)&ph,(void*)(addr+offset),eh.e_phentsize);
#ifdef DEBUG
        print_prghdr(&ph);
#endif
        if (i == 0)
            memcpy((void*)addr_exec,(void*)(addr+ph.p_offset),ph.p_memsz);
    }

    return eh.e_entry;
}

