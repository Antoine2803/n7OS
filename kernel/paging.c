#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <n7OS/mem.h>
#include <stddef.h> // nécessaire pour NULL

PageDirectory page_dir;

void initialise_paging()
{

    init_mem();

    page_dir = (PageDirectory)kmalloc_a(PAGE_SIZE_BITS);

    for (uint32_t index = 0; index < PAGE_SIZE_BITS / sizeof(PDE); index++)
    {
        page_dir[index].P = 1;
        page_dir[index].U = 0;
        page_dir[index].W = 1;
        page_dir[index].addr = kmalloc_a(PAGE_SIZE_BITS) >> 12;
    }

    for (uint32_t i = 0; i < get_stack_top(); i += PAGE_SIZE)
    {
        alloc_page_entry(i, 1, 1);
    }

    // Mise à jour du registre CR3
    __asm__ __volatile__("mov %0, %%cr3" ::"r"(&page_dir));

    uint32_t cr0;

    // Lancer le paging
    __asm__ __volatile__("mov %0, %%cr0" : "=r"(cr0));

    cr0 |= 0x80000001;

    __asm__ __volatile__("mov %%cr0, %0" :: "r"(cr0));
}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel)
{
    virtual_addr virtual_address;
    virtual_address.value = address;

    PageTable pgtab = (PageTable)page_dir[virtual_address.indice_rep].addr;

    pgtab[virtual_address.indice_table].P = 1;
    pgtab[virtual_address.indice_table].W = 1;
    pgtab[virtual_address.indice_table].U = 0;
    pgtab[virtual_address.indice_table].addr = address >> 12;

    setPage(address);

    return pgtab;
}
