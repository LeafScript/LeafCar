## Update Fw

STM32F103ZET6:

RAM & Flash content:
```
RAM:
64KB
data  bss  _user_heap_stack

Flash:
512KB
isr_vector  text  rodata  ARM.extab  ARM  preinit_array  init_array  fini_array
```

Application Max Size (Planning):
```
RAM:
data + bss: 8192 bytes
stack: 1024 bytes
heap: 3072 bytes  (still unused)
total: 12288 bytes (0x3000)

Flash:
total: 128 KB
```

RAM Update interaction:
```
Peripheral

RAM (0x20000000)

Flash master (0x8000000 ~ 0x801FFFF)     Flash slave (0x8020000 ~ 0x803FFFF)
```