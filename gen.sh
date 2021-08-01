#!/usr/bin/bash
clang -E gglm_gen.h > TEMP.h
clang-format TEMP.h -style=file > gglm.h
python postfmt.py gglm.h
