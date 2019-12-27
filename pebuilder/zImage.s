.globl	zimage
.globl	end_zimage

.section	.zimage

zimage:

//.ascii "TODO: linux zImage gets .incbin'ed here\r\n\0"
.incbin	"zImage"

.balign	0x1000

end_zimage:
