#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <stdio.h>
#include <n7OS/mem.h>
#include <stddef.h> // nécessaire pour NULL
#include <n7OS/processor_structs.h>

PageDirectory page_dir;

/**
 * Initialise le paging
 */
void initialise_paging()
{
    init_mem();

    page_dir = (PageDirectory)kmalloc_a(PAGE_SIZE);

    for (uint32_t index = 0; index < PAGE_SIZE / sizeof(PDE); index++)
    {
        page_dir[index].P = PRESENT;
        page_dir[index].U = 0;
        page_dir[index].W = 1;
        page_dir[index].addr = kmalloc_a(PAGE_SIZE) >> 12;
    }

    for (uint32_t i = 0x0; i < get_stack_top(); i += PAGE_SIZE)
    {
        alloc_page_entry(i, 1, 1);
    }

    // Mise à jour du registre CR3
    __asm__ __volatile__("mov %0, %%cr3" ::"r"(page_dir));

    uint64_t cr0;

    // Lancer le paging
    __asm__ __volatile__("mov %%cr0, %0 " : "=r"(cr0));

    cr0 |= 0x80000000;

    __asm__ __volatile__("mov %0, %%cr0" ::"r"(cr0));

    setup_base((uint32_t)page_dir);
}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel)
{
    virtual_addr virtual_address;
    virtual_address.value = address;

    // On récupère la page table correspondante à l'adresse
    PDE pde = page_dir[virtual_address.indice_rep];
    PageTable pt = (PageTable)(pde.addr << 12);
    PTE *pte = &pt[virtual_address.indice_table];

    // On repli la page table avec les 
    pte->P = PRESENT;
    pte->W = is_writeable;
    pte->U = ~is_kernel;
    pte->addr = findfreePage() >> 12;

    return pt;
}
