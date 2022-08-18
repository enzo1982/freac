#ifndef H_GLIBC_VERSION_HEADER
#define H_GLIBC_VERSION_HEADER

#ifndef __ASSEMBLER__

#if defined __i386__
__asm__(".symver exp,exp@GLIBC_2.0");
__asm__(".symver exp2,exp2@GLIBC_2.1");
__asm__(".symver log,log@GLIBC_2.0");
__asm__(".symver log2,log2@GLIBC_2.1");
__asm__(".symver pow,pow@GLIBC_2.0");
#elif defined __x86_64__
__asm__(".symver exp,exp@GLIBC_2.2.5");
__asm__(".symver exp2,exp2@GLIBC_2.2.5");
__asm__(".symver log,log@GLIBC_2.2.5");
__asm__(".symver log2,log2@GLIBC_2.2.5");
__asm__(".symver pow,pow@GLIBC_2.2.5");
#elif defined __arm__
__asm__(".symver exp,exp@GLIBC_2.4");
__asm__(".symver exp2,exp2@GLIBC_2.4");
__asm__(".symver log,log@GLIBC_2.4");
__asm__(".symver log2,log2@GLIBC_2.4");
__asm__(".symver pow,pow@GLIBC_2.4");
#elif defined __aarch64__
__asm__(".symver exp,exp@GLIBC_2.17");
__asm__(".symver exp2,exp2@GLIBC_2.17");
__asm__(".symver log,log@GLIBC_2.17");
__asm__(".symver log2,log2@GLIBC_2.17");
__asm__(".symver pow,pow@GLIBC_2.17");
#endif

#endif

#endif
