// https://webserver2.tecgraf.puc-rio.br/~ismael/Cursos/YC++/apostilas/win32_xcoff_pe/tyne-example/Tiny%20PE.htm

.globl _start
.extern main
.extern end_zimage

.section .pehdr

.equ	sectalign, 0x1000
.equ	filealign, 0x200

.equ	baseaddr, 0x10000

mz_hdr:

	.ascii	"MZ"         // e_magic
	.short	0            // e_cblp
	.short	0            // e_cp
	.short	0            // e_crlc
	.short	0            // e_cparhdr
	.short	0            // e_minalloc
	.short	0            // e_maxalloc
	.short	0            // e_ss
	.short	0            // e_sp
	.short	0            // e_csum
	.short	0            // e_ip
	.short	0            // e_cs
	.short	0x40         // e_lsarlc
	.short	0            // e_ovno
	.rept	4
	.short	0            // e_res
	.endr
	.short	0            // e_oemid
	.short	0            // e_oeminfo
	.rept	10
	.short	0            // e_res2
	.endr
	.word pe_hdr-mz_hdr  // e_lfanew

pe_hdr:
	.ascii	"PE\0\0"             // PE signature
	.short	0x1c0                // Machine (arm)
	.short	1                    // NumberOfSections
	.word	0                    // TimeDateStamp
	.word	0                    // PointerToSymbolTable
	.word	0                    // NumberOfSymbols
	.short	end_opt_hdr-opt_hdr  // SizeOfOptionalHeader
	.short	0x10f                // Characteristics (stripped, executable, 32bit)


opt_hdr:
	.short	0x10B                      // Magic (PE32)
	.byte	0                          // MajorLinkerVersion
	.byte	0                          // MinorLinkerVersion
	.word	0                          // SizeOfCode
	.word	0                          // SizeOfInitializedData
	.word	0                          // SizeOfUninitializedData
	.word	0x1000                     // AddressOfEntryPoint
	.word	0                          // BaseOfCode
	.word	0                          // BaseOfData
	.word	baseaddr                   // ImageBase
	.word	sectalign                  // SectionAlignment
	.word	filealign                  // FileAlignment
	.short	0                          // MajorOperatingSystemVersion
	.short	0                          // MinorOperatingSystemVersion
	.short	0                          // MajorImageVersion
	.short	0                          // MinorImageVersion
	.short	0                          // MajorSubsystemVersion
	.short	0                          // MinorSubsystemVersion
	.word	0                          // Win32VersionValue
	.word	end_zimage-mz_hdr          // SizeOfImage
	.word	0x200                      // SizeOfHeaders
	.word	0                          // CheckSum
	.short	9                          // Subsystem (WinCE GUI)
	.short	0                          // DllCharacteristics
	.word	0x10000                    // SizeOfStackReserve
	.word	0x1000                     // SizeOfStackCommit
	.word	0x100000                   // SizeOfHeapReserve
	.word	0x1000                     // SizeOfHeapCommit
	.word	0                          // LoaderFlags
	.word	16                         // NumberOfRvaAndSizes

	.space	0x80                       // DataDirectory

end_opt_hdr:

sect_hdrs:
	.ascii	".text\0\0\0" // Name
	.word	end_zimage-_start   // VirtualSize
	.word	0x1000              // VirtualAddress
	.word	end_zimage-_start   // SizeOfRawData
	.word	0x1000              // PointerToRawData
	.word	0                   // PointerToRelocations
	.word	0                   // PointerToLinenumbers
	.short	0                   // NumberOfRelocations
	.short	0                   // NumberOfLinenumbers
	.word	0xe0000020          // Characteristics (rwx)


end_hdrs:

.balign 0x1000

_start:
//	ldr	sp, =0x84080000
	b	main
