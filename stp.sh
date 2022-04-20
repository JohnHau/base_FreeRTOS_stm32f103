#!/bin/bash
gcc -o read_stp_msg.exe  read_stp_msg.c ./small_text_protocol/stp.c  ./CRC16/CRC16.c
echo done
