#!/bin/sh
#
MODULE=sally2
ASSEMBLER_PATH=zmac
ASSEMBLER_EXE="${ASSEMBLER_PATH}/zmac.exe"
ASM_EXTENSION=asm
SOURCE_DIR=../src
OUTPUT_DIR=../rel

if [ "--clean" = "${1}" ]; then
  rm -f ${OUTPUT_DIR}/*
  rmdir ${OUTPUT_DIR} 2> /dev/null
  cd ${ASSEMBLER_PATH}/src
  make clean
  cd ${OLDPWD}
  exit 1
fi

compile_code()
{
  ${ASSEMBLER_EXE} --oo lst,hex,cim --od ${OUTPUT_DIR} ${SOURCE_DIR}/${1}.${ASM_EXTENSION}
  mv ${OUTPUT_DIR}/${1}.cim ${OUTPUT_DIR}/${1}.com
}

if [ ! -d "${OUTPUT_DIR}" ]; then
  mkdir ${OUTPUT_DIR}
fi

if [ ! -e "${ASSEMBLER_EXE}" ]; then
  cd ${ASSEMBLER_PATH}/src
  make
  cd "${OLDPWD}"
fi

rm -f ${OUTPUT_DIR}/*
compile_code ${MODULE}
compile_code ddinit-0100
# Build ATR 8000 ROM
ASM_EXTENSION=MAC
compile_code ROM
