# IMU_DESIGN


## SMART BOUY ENGINEERING DESIGN

Smartt Bouys are being used in Antartica to develop research on wave and ice dynamics. We are currently required to 
find algorithms the can decompress and encrypt on the STM32F051R8T6. 

The compression algorithms used are huffman and lz77. The encryption is Xor encryption.

The code used in the project was adapted from the following git repos/links:

Huffman - https://github.com/Bestoa/huffman-codec<br>
Huffman - https://www.codeproject.com/Questions/5320638/File-compression-and-decompression-and-C<br>
Lz77 - https://github.com/Favrito/LZ77<br>
Xor - https://github.com/agariy/LibXor-C-<br>
Icm-20948 - https://github.com/mokhwasomssi/stm32_hal_icm20948<br>

The stm code is in the ./STMCubeMix directory. The tests and pyscripts are in the ./Testing folder.
