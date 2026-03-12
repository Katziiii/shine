#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "inc/rom.h"

uint8_t rom_buffer[BUFFER];
uint8_t memory[BUFFER];

uint8_t rom_type_code;
uint8_t region_code;

char rom_type[40];
char title[10];
char region[10];
char publisher[40];

uint8_t version;

void load_rom(char *path)
{
    FILE *rom = fopen(path, "rb");

    fread(rom_buffer, 1, BUFFER, rom);
    fclose(rom);

    for(int i = 0; i < BUFFER; i++)
        memory[i] = rom_buffer[i];

    printf("Rom Loaded!\n");
}

void get_lic_code() {
    uint8_t old_lic_code = memory[0x014B];
    uint16_t lic_code;

    if (old_lic_code == 0x33) {
        uint8_t a = memory[0x0144];
        uint8_t b = memory[0x0145];
        lic_code = (a << 8) | b;

        switch (lic_code) {
            case 0x3030: strcpy(publisher, "None"); break;
            case 0x3031: strcpy(publisher, "Nintendo Research & Development 1"); break;
            case 0x3038: strcpy(publisher, "Capcom"); break;
            case 0x3133: strcpy(publisher, "EA (Electronic Arts)"); break;
            case 0x3138: strcpy(publisher, "Hudson Soft"); break;
            case 0x3139: strcpy(publisher, "B-AI"); break;
            case 0x3230: strcpy(publisher, "KSS"); break;
            case 0x3232: strcpy(publisher, "Planning Office WADA"); break;
            case 0x3234: strcpy(publisher, "PCM Complete"); break;
            case 0x3235: strcpy(publisher, "San-X"); break;
            case 0x3238: strcpy(publisher, "Kemco"); break;
            case 0x3239: strcpy(publisher, "SETA Corporation"); break;
            case 0x3330: strcpy(publisher, "Viacom"); break;
            case 0x3331: strcpy(publisher, "Nintendo"); break;
            case 0x3332: strcpy(publisher, "Bandai"); break;
            case 0x3333: strcpy(publisher, "Ocean Software/Acclaim Entertainment"); break;
            case 0x3334: strcpy(publisher, "Konami"); break;
            case 0x3335: strcpy(publisher, "HectorSoft"); break;
            case 0x3337: strcpy(publisher, "Taito"); break;
            case 0x3338: strcpy(publisher, "Hudson Soft"); break;
            case 0x3339: strcpy(publisher, "Banpresto"); break;
            case 0x3431: strcpy(publisher, "Ubi Soft1"); break;
            case 0x3432: strcpy(publisher, "Atlus"); break;
            case 0x3434: strcpy(publisher, "Malibu Interactive"); break;
            case 0x3436: strcpy(publisher, "Angel"); break;
            case 0x3437: strcpy(publisher, "Bullet-Proof Software2"); break;
            case 0x3439: strcpy(publisher, "Irem"); break;
            case 0x3530: strcpy(publisher, "Absolute"); break;
            case 0x3531: strcpy(publisher, "Acclaim Entertainment"); break;
            case 0x3532: strcpy(publisher, "Activision"); break;
            case 0x3533: strcpy(publisher, "Sammy USA Corporation"); break;
            case 0x3534: strcpy(publisher, "Konami"); break;
            case 0x3535: strcpy(publisher, "Hi Tech Expressions"); break;
            case 0x3536: strcpy(publisher, "LJN"); break;
            case 0x3537: strcpy(publisher, "Matchbox"); break;
            case 0x3538: strcpy(publisher, "Mattel"); break;
            case 0x3539: strcpy(publisher, "Milton Bradley Company"); break;
            case 0x3630: strcpy(publisher, "Titus Interactive"); break;
            case 0x3631: strcpy(publisher, "Virgin Games Ltd.3"); break;
            case 0x3634: strcpy(publisher, "Lucasfilm Games4"); break;
            case 0x3637: strcpy(publisher, "Ocean Software"); break;
            case 0x3639: strcpy(publisher, "EA (Electronic Arts)"); break;
            case 0x3730: strcpy(publisher, "Infogrames5"); break;
            case 0x3731: strcpy(publisher, "Interplay Entertainment"); break;
            case 0x3732: strcpy(publisher, "Broderbund"); break;
            case 0x3733: strcpy(publisher, "Sculptured Software6"); break;
            case 0x3735: strcpy(publisher, "The Sales Curve Limited7"); break;
            case 0x3738: strcpy(publisher, "THQ"); break;
            case 0x3739: strcpy(publisher, "Accolade8"); break;
            case 0x3830: strcpy(publisher, "Misawa Entertainment"); break;
            case 0x3833: strcpy(publisher, "LOZC G."); break;
            case 0x3836: strcpy(publisher, "Tokuma Shoten"); break;
            case 0x3837: strcpy(publisher, "Tsukuda Original"); break;
            case 0x3931: strcpy(publisher, "Chunsoft Co.9"); break;
            case 0x3932: strcpy(publisher, "Video System"); break;
            case 0x3933: strcpy(publisher, "Ocean Software/Acclaim Entertainment"); break;
            case 0x3935: strcpy(publisher, "Varie"); break;
            case 0x3936: strcpy(publisher, "Yonezawa10/S’Pal"); break;
            case 0x3937: strcpy(publisher, "Kaneko"); break;
            case 0x3939: strcpy(publisher, "Pack-In-Video"); break;
            case 0x3948: strcpy(publisher, "Bottom Up"); break;
            case 0x4134: strcpy(publisher, "Konami (Yu-Gi-Oh!)"); break;
            case 0x424C: strcpy(publisher, "MTO"); break;
            case 0x444B: strcpy(publisher, "Kodansha"); break;
            default: strcpy(publisher, "Unknown"); break;
        }

    } else {
        switch(old_lic_code) {
            case 0x00: strcpy(publisher, "None"); break;
            case 0x01: strcpy(publisher, "Nintendo"); break;
            case 0x08: strcpy(publisher, "Capcom"); break;
            case 0x09: strcpy(publisher, "HOT-B"); break;
            case 0x0A: strcpy(publisher, "Jaleco"); break;
            case 0x0B: strcpy(publisher, "Coconuts Japan"); break;
            case 0x0C: strcpy(publisher, "Elite Systems"); break;
            case 0x13: strcpy(publisher, "EA (Electronic Arts)"); break;
            case 0x18: strcpy(publisher, "Hudson Soft"); break;
            case 0x19: strcpy(publisher, "ITC Entertainment"); break;
            case 0x1A: strcpy(publisher, "Yanoman"); break;
            case 0x1D: strcpy(publisher, "Japan Clary"); break;
            case 0x1F: strcpy(publisher, "Virgin Games Ltd.3"); break;
            case 0x24: strcpy(publisher, "PCM Complete"); break;
            case 0x25: strcpy(publisher, "San-X"); break;
            case 0x28: strcpy(publisher, "Kemco"); break;
            case 0x29: strcpy(publisher, "SETA Corporation"); break;
            case 0x30: strcpy(publisher, "Infogrames5"); break;
            case 0x31: strcpy(publisher, "Nintendo"); break;
            case 0x32: strcpy(publisher, "Bandai"); break;
            case 0x33: strcpy(publisher, "Use new licensee code"); break;
            case 0x34: strcpy(publisher, "Konami"); break;
            case 0x35: strcpy(publisher, "HectorSoft"); break;
            case 0x38: strcpy(publisher, "Capcom"); break;
            case 0x39: strcpy(publisher, "Banpresto"); break;
            case 0x3C: strcpy(publisher, "Entertainment Interactive (stub)"); break;
            case 0x3E: strcpy(publisher, "Gremlin"); break;
            case 0x41: strcpy(publisher, "Ubi Soft1"); break;
            case 0x42: strcpy(publisher, "Atlus"); break;
            case 0x44: strcpy(publisher, "Malibu Interactive"); break;
            case 0x46: strcpy(publisher, "Angel"); break;
            case 0x47: strcpy(publisher, "Spectrum HoloByte"); break;
            case 0x49: strcpy(publisher, "Irem"); break;
            case 0x4A: strcpy(publisher, "Virgin Games Ltd.3"); break;
            case 0x4D: strcpy(publisher, "Malibu Interactive"); break;
            case 0x4F: strcpy(publisher, "U.S. Gold"); break;
            case 0x50: strcpy(publisher, "Absolute"); break;
            case 0x51: strcpy(publisher, "Acclaim Entertainment"); break;
            case 0x52: strcpy(publisher, "Activision"); break;
            case 0x53: strcpy(publisher, "Sammy USA Corporation"); break;
            case 0x54: strcpy(publisher, "GameTek"); break;
            case 0x55: strcpy(publisher, "Park Place15"); break;
            case 0x56: strcpy(publisher, "LJN"); break;
            case 0x57: strcpy(publisher, "Matchbox"); break;
            case 0x59: strcpy(publisher, "Milton Bradley Company"); break;
            case 0x5A: strcpy(publisher, "Mindscape"); break;
            case 0x5B: strcpy(publisher, "Romstar"); break;
            case 0x5C: strcpy(publisher, "Naxat Soft16"); break;
            case 0x5D: strcpy(publisher, "Tradewest"); break;
            case 0x60: strcpy(publisher, "Titus Interactive"); break;
            case 0x61: strcpy(publisher, "Virgin Games Ltd.3"); break;
            case 0x67: strcpy(publisher, "Ocean Software"); break;
            case 0x69: strcpy(publisher, "EA (Electronic Arts)"); break;
            case 0x6E: strcpy(publisher, "Elite Systems"); break;
            case 0x6F: strcpy(publisher, "Electro Brain"); break;
            case 0x70: strcpy(publisher, "Infogrames5"); break;
            case 0x71: strcpy(publisher, "Interplay Entertainment"); break;
            case 0x72: strcpy(publisher, "Broderbund"); break;
            case 0x73: strcpy(publisher, "Sculptured Software6"); break;
            case 0x75: strcpy(publisher, "The Sales Curve Limited7"); break;
            case 0x78: strcpy(publisher, "THQ"); break;
            case 0x79: strcpy(publisher, "Accolade8"); break;
            case 0x7A: strcpy(publisher, "Triffix Entertainment"); break;
            case 0x7C: strcpy(publisher, "MicroProse"); break;
            case 0x7F: strcpy(publisher, "Kemco"); break;
            case 0x80: strcpy(publisher, "Misawa Entertainment"); break;
            case 0x83: strcpy(publisher, "LOZC G."); break;
            case 0x86: strcpy(publisher, "Tokuma Shoten"); break;
            case 0x8B: strcpy(publisher, "Bullet-Proof Software2"); break;
            case 0x8C: strcpy(publisher, "Vic Tokai Corp.17"); break;
            case 0x8E: strcpy(publisher, "Ape Inc.18"); break;
            case 0x8F: strcpy(publisher, "I’Max19"); break;
            case 0x91: strcpy(publisher, "Chunsoft Co.9"); break;
            case 0x92: strcpy(publisher, "Video System"); break;
            case 0x93: strcpy(publisher, "Tsubaraya Productions"); break;
            case 0x95: strcpy(publisher, "Varie"); break;
            case 0x96: strcpy(publisher, "Yonezawa10/S’Pal"); break;
            case 0x97: strcpy(publisher, "Kemco"); break;
            case 0x99: strcpy(publisher, "Arc"); break;
            case 0x9A: strcpy(publisher, "Nihon Bussan"); break;
            case 0x9B: strcpy(publisher, "Tecmo"); break;
            case 0x9C: strcpy(publisher, "Imagineer"); break;
            case 0x9D: strcpy(publisher, "Banpresto"); break;
            case 0x9F: strcpy(publisher, "Nova"); break;
            case 0xA1: strcpy(publisher, "Hori Electric"); break;
            case 0xA2: strcpy(publisher, "Bandai"); break;
            case 0xA4: strcpy(publisher, "Konami"); break;
            case 0xA6: strcpy(publisher, "Kawada"); break;
            case 0xA7: strcpy(publisher, "Takara"); break;
            case 0xA9: strcpy(publisher, "Technos Japan"); break;
            case 0xAA: strcpy(publisher, "Broderbund"); break;
            case 0xAC: strcpy(publisher, "Toei Animation"); break;
            case 0xAD: strcpy(publisher, "Toho"); break;
            case 0xAF: strcpy(publisher, "Namco"); break;
            case 0xB0: strcpy(publisher, "Acclaim Entertainment"); break;
            case 0xB1: strcpy(publisher, "ASCII Corporation or Nexsoft"); break;
            case 0xB2: strcpy(publisher, "Bandai"); break;
            case 0xB4: strcpy(publisher, "Square Enix"); break;
            case 0xB6: strcpy(publisher, "HAL Laboratory"); break;
            case 0xB7: strcpy(publisher, "SNK"); break;
            case 0xB9: strcpy(publisher, "Pony Canyon"); break;
            case 0xBA: strcpy(publisher, "Culture Brain"); break;
            case 0xBB: strcpy(publisher, "Sunsoft"); break;
            case 0xBD: strcpy(publisher, "Sony Imagesoft"); break;
            case 0xBF: strcpy(publisher, "Sammy Corporation"); break;
            case 0xC0: strcpy(publisher, "Taito"); break;
            case 0xC2: strcpy(publisher, "Kemco"); break;
            case 0xC3: strcpy(publisher, "Square"); break;
            case 0xC4: strcpy(publisher, "Tokuma Shoten"); break;
            case 0xC5: strcpy(publisher, "Data East"); break;
            case 0xC6: strcpy(publisher, "Tonkin House"); break;
            case 0xC8: strcpy(publisher, "Koei"); break;
            case 0xC9: strcpy(publisher, "UFL"); break;
            case 0xCA: strcpy(publisher, "Ultra Games"); break;
            case 0xCB: strcpy(publisher, "VAP, Inc."); break;
            case 0xCC: strcpy(publisher, "Use Corporation"); break;
            case 0xCD: strcpy(publisher, "Meldac"); break;
            case 0xCE: strcpy(publisher, "Pony Canyon"); break;
            case 0xCF: strcpy(publisher, "Angel"); break;
            case 0xD0: strcpy(publisher, "Taito"); break;
            case 0xD1: strcpy(publisher, "SOFEL (Software Engineering Lab)"); break;
            case 0xD2: strcpy(publisher, "Quest"); break;
            case 0xD3: strcpy(publisher, "Sigma Enterprises"); break;
            case 0xD4: strcpy(publisher, "ASK Kodansha Co."); break;
            case 0xD6: strcpy(publisher, "Naxat Soft16"); break;
            case 0xD7: strcpy(publisher, "Copya System"); break;
            case 0xD9: strcpy(publisher, "Banpresto"); break;
            case 0xDA: strcpy(publisher, "Tomy"); break;
            case 0xDB: strcpy(publisher, "LJN"); break;
            case 0xDD: strcpy(publisher, "Nippon Computer Systems"); break;
            case 0xDE: strcpy(publisher, "Human Ent."); break;
            case 0xDF: strcpy(publisher, "Altron"); break;
            case 0xE0: strcpy(publisher, "Jaleco"); break;
            case 0xE1: strcpy(publisher, "Towa Chiki"); break;
            case 0xE2: strcpy(publisher, "Yutaka"); break;
            case 0xE3: strcpy(publisher, "Varie"); break;
            case 0xE5: strcpy(publisher, "Epoch"); break;
            case 0xE7: strcpy(publisher, "Athena"); break;
            case 0xE8: strcpy(publisher, "Asmik Ace Entertainment"); break;
            case 0xE9: strcpy(publisher, "Natsume"); break;
            case 0xEA: strcpy(publisher, "King Records"); break;
            case 0xEB: strcpy(publisher, "Atlus"); break;
            case 0xEC: strcpy(publisher, "Epic/Sony Records"); break;
            case 0xEE: strcpy(publisher, "IGS"); break;
            case 0xF0: strcpy(publisher, "A Wave"); break;
            case 0xF3: strcpy(publisher, "Extreme Entertainment"); break;
            case 0xFF: strcpy(publisher, "LJN"); break;
            default: strcpy(publisher, "Unknown"); break;
        }
    }
}

void get_rom_type () {
    rom_type_code = memory[0x147];

    switch(rom_type_code) {
        case 0x00: strcpy(rom_type, "ROM ONLY"); break;
        case 0x01: strcpy(rom_type, "MBC1"); break;
        case 0x02: strcpy(rom_type, "MBC1+RAM"); break;
        case 0x03: strcpy(rom_type, "MBC1+RAM+BATTERY"); break;

        case 0x05: strcpy(rom_type, "MBC2"); break;
        case 0x06: strcpy(rom_type, "MBC2+BATTERY"); break;

        case 0x08: strcpy(rom_type, "ROM+RAM"); break;
        case 0x09: strcpy(rom_type, "ROM+RAM+BATTERY"); break;

        case 0x0B: strcpy(rom_type, "MMM01"); break;
        case 0x0C: strcpy(rom_type, "MMM01+RAM"); break;
        case 0x0D: strcpy(rom_type, "MMM01+RAM+BATTERY"); break;

        case 0x0F: strcpy(rom_type, "MBC3+TIMER+BATTERY"); break;
        case 0x10: strcpy(rom_type, "MBC3+TIMER+RAM+BATTERY"); break;
        case 0x11: strcpy(rom_type, "MBC3"); break;
        case 0x12: strcpy(rom_type, "MBC3+RAM"); break;
        case 0x13: strcpy(rom_type, "MBC3+RAM+BATTERY"); break;

        case 0x19: strcpy(rom_type, "MBC5"); break;
        case 0x1A: strcpy(rom_type, "MBC5+RAM"); break;
        case 0x1B: strcpy(rom_type, "MBC5+RAM+BATTERY"); break;
        case 0x1C: strcpy(rom_type, "MBC5+RUMBLE"); break;
        case 0x1D: strcpy(rom_type, "MBC5+RUMBLE+RAM"); break;
        case 0x1E: strcpy(rom_type, "MBC5+RUMBLE+RAM+BATTERY"); break;

        case 0x20: strcpy(rom_type, "MBC6"); break;

        case 0x22: strcpy(rom_type, "MBC7+SENSOR+RUMBLE+RAM+BATTERY"); break;

        case 0xFC: strcpy(rom_type, "POCKET CAMERA"); break;
        case 0xFD: strcpy(rom_type, "BANDAI TAMA5"); break;
        case 0xFE: strcpy(rom_type, "HuC3"); break;
        case 0xFF: strcpy(rom_type, "HuC1+RAM+BATTERY"); break;

        default: strcpy(rom_type, "Unknown"); break;
    }
}

void get_rom_titel () {
    for (int i = 0x134, k = 0; i < 0x143 && k < 10; i++, k++) {
        title[k] = memory[i];
    }
}

void get_rom_region () {
    region_code = memory [0x14A] ;

    switch (region_code) {
        case 0 : strcpy(region, "Japan"); break;
        case 1 : strcpy(region, "Global"); break;
    }

} ;

void get_rom_version () {
    version = memory [0x14c] ;
}

void get_rom_info () {
    get_lic_code() ;
    get_rom_titel() ;
    get_rom_region() ;
    get_rom_type() ;
    get_rom_version () ;
    printf("Publisher: %s\n",publisher);
    printf("Titel: %s\n",title);
    printf("Region: %s\n",region);
    printf("Type: %s\n",rom_type);
    printf("Version: %02x\n",version);
} ;
