/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <string.h>


extern info_t *info;

void afficher(gdt_reg_t *gdtr){
    int i = 0;
    seg_desc_t *begin;
    begin = (seg_desc_t *)gdtr->desc;
    while (begin <= (seg_desc_t *)gdtr->desc + gdtr->limit / sizeof(seg_desc_t)) {
        uint32_t addr =
            (begin->base_1) | (begin->base_2 << 16) | (begin->base_3 << 24);
        uint32_t lim = (begin->limit_1) | (begin->limit_2 << 16);
        if (begin->g) {
        lim += 1;
        lim *= 4096;
        lim -= 1;
        }
        if (begin->p) {
        printf(
            "Segment %d : Adresse : [0x%x - 0x%x] : Type de descripteur: %d\n\n",
            i, addr, lim, begin->type);
        }
        begin++;
        i++;
    }
}

void tp(){
    // Déclaration de la table des descripteurs
    gdt_reg_t gdtr;
    get_gdtr(gdtr);
    afficher(&gdtr);
    
    // Création de notre propre table gdt
    seg_desc_t *new_table = 0x0;

    for (uint32_t i = 0; i < sizeof(seg_desc_t); i++) {
        *((uint8_t *)new_table + i) = 0;
    }

    // Code, 32 bits RX, flat, indice 1
    new_table->limit_1 = 0xffff;
    new_table->base_1 = 0x0;
    new_table->base_2 = 0x0;
    new_table->type = SEG_DESC_CODE_XR;
    new_table->s = 1;
    new_table->dpl = 0;
    new_table->p = 1;
    new_table->limit_2 = 0xf;
    new_table->avl = 0;
    new_table->l = 0;
    new_table->d = 1;
    new_table->g = 1;
    new_table->base_3 = 0x00;

    new_table++;

    // Données, 32 bits RW, flat, indice 2
    new_table->limit_1 = 0xffff;
    new_table->base_1 = 0x0000;
    new_table->base_2 = 0x0000;
    new_table->type = SEG_DESC_DATA_RW;
    new_table->s = 1;
    new_table->dpl = 0;
    new_table->p = 1;
    new_table->limit_2 = 0xf;
    new_table->avl = 0;
    new_table->l = 0;
    new_table->d = 1;
    new_table->g = 1;
    new_table->base_3 = 0x00;

    new_table -= 2;

    gdtr.limit = 3 * sizeof(seg_desc_t) - 1;
    gdtr.desc = new_table;

    set_gdtr(gdtr);

    printf("New GDT\n");
    get_gdtr(gdtr);
  
    afficher(&gdtr);

    // Data Selector
    seg_sel_t ds;
    ds.rpl = 0;
    ds.ti = 0;
    ds.index = 2;

    set_ss(ds);
    set_ds(ds);
    set_cs(0x0 | 0 << 2 | 1 << 3);

    //char src[64];
    //char *dst = 0;

    //memset(src, 0xff, 64);

    uint32_t base = 0x00600000;
    //seg_desc_t *new_desc;
    new_table->limit_1 = 0x1f;
    new_table->base_1 = base & 0x0000ffff;
    new_table->base_2 = (base & 0x000f0000) >> 16;
    new_table->type = SEG_DESC_DATA_RW;
    new_table->s = 1;
    new_table->dpl = 0;
    new_table->p = 1;
    new_table->limit_2 = 0x0;
    new_table->avl = 0;
    new_table->l = 0;
    new_table->d = 1;
    new_table->g = 1;
    new_table->base_3 = (base & 0x00f00000) >> 24;

    //set_es(new_table);

    //_memcpy8(dst, src, 32);
    
}
