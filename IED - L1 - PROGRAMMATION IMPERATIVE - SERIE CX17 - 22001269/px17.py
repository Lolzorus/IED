#!/usr/bin/env python3
import sys, os
# argv[1] = minor.bugfix version number
prog = 'cx17.' + sys.argv[1]
compile = 'gcc -Wall %s.c -o %s' % (prog, prog)
if os.system(compile) != 0 : exit
run = '%s indexe-moi.texte > %s.test' % (prog, prog)
os.system(run)
