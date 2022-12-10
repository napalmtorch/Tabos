void main(void* multiboot)
{
    char* vram = (char*)0xb8000;
    vram[0] = 'F';
    vram[2] = 'U';
    vram[4] = 'C';
    vram[6] = 'K';
}