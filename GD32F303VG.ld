/* ============================================================================
 * GD32F303VG 链接脚本  (从 Keil 工程 A39X.sct 转写)
 *
 * 芯片: GD32F303VG  Cortex-M4   Flash 1MB / SRAM 96KB
 *
 * 注意: 本工程带 BootLoader, 应用从 0x08004000 开始:
 *       0x08000000 ~ 0x08003FFF   BootLoader (16KB)
 *       0x08004000 ~ 0x0803A000   应用区     (0x36000 = 216KB, 与 A39X.sct 一致)
 *       0x0803A000 ~ 0x080FFFFF   保留 (OTA 下载缓冲等)
 *   main.c 中 nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x4000) 即对应此偏移。
 *
 * 如需更大应用区, 可将 FLASH LENGTH 改为 0xFC000 (1MB - 16KB BootLoader)。
 * ========================================================================== */

ENTRY(Reset_Handler)

/* 堆 / 栈 (与 Keil 启动文件 Stack_Size/Heap_Size 一致, 各 1KB) */
_Min_Heap_Size  = 0x400;   /* required amount of heap  */
_Min_Stack_Size = 0x400;   /* required amount of stack */

/* ---------------------------------------------------------------------------
 * 存储器布局
 * -------------------------------------------------------------------------- */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08004000, LENGTH = 0x36000   /* 应用区 216KB */
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 0x18000   /* SRAM    96KB */
}

/* 栈顶 = RAM 末尾 */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* ---------------------------------------------------------------------------
 * 段定义
 * -------------------------------------------------------------------------- */
SECTIONS
{
    /* 中断向量表, 必须放在 Flash 最前面 (0x08004000) */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    /* 代码 */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.glue_7)          /* ARM->Thumb */
        *(.glue_7t)         /* Thumb->ARM */
        *(.eh_frame)
        KEEP (*(.init))
        KEEP (*(.fini))
        . = ALIGN(4);
        _etext = .;         /* 代码段结束 */
    } >FLASH

    /* 只读数据 */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH
    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH
    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

    /* 已初始化数据: 初值放在 Flash, 运行时拷贝到 RAM */
    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;          /* .data 在 RAM 中的起始 */
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;          /* .data 在 RAM 中的结束 */
    } >RAM AT> FLASH

    /* 未初始化数据 */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM

    /* 堆 + 栈 占位 (栈真正从 _estack 向下生长) */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } >RAM
}
