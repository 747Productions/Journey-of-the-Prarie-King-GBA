
@{{BLOCK(bullet_bn_gfx)

@=======================================================================
@
@	bullet_bn_gfx, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ 1 tiles not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2026-07-10, 23:57:01
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global bullet_bn_gfxTiles		@ 32 unsigned chars
	.hidden bullet_bn_gfxTiles
bullet_bn_gfxTiles:
	.word 0x0EEEEEE0,0xEE8822EE,0xE888222E,0xE888882E,0xE888888E,0xE888888E,0xEE8888EE,0x0EEEEEE0

	.section .rodata
	.align	2
	.global bullet_bn_gfxPal		@ 32 unsigned chars
	.hidden bullet_bn_gfxPal
bullet_bn_gfxPal:
	.hword 0x0000,0x5D4B,0x1FD9,0x18EC,0x1D51,0x11DB,0x329B,0x4F1D
	.hword 0x03DF,0x2B93,0x1AED,0x3646,0x15A9,0x112A,0x0000,0x38E7

@}}BLOCK(bullet_bn_gfx)
