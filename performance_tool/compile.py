import subprocess

QMAKE_BINARY = "qmake"
MAKE_COMMAND = "make"
def compile(qmake_file, cwd, qmake_binary=QMAKE_BINARY, make_command=MAKE_COMMAND):
    print "Builiding "+qmake_file+" in "+cwd
    subprocess.check_call([qmake_binary, "-makefile", "-o", "Makefile", qmake_file], cwd=cwd)
    subprocess.check_call([make_command], cwd=cwd)