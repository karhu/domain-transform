import subprocess
import os
import math
from PIL import Image

REQUIRED_ACCURACY = 1e-3
class ImageSizeDoesntMatchException(Exception):
    pass
class ImagesDontMatchException(Exception):
    pass

def validate(ref_filename, test_filename):
    ref_im = Image.open(ref_filename)
    test_im = Image.open(test_filename)

    ref_size = ref_im.size
    test_size = test_im.size
    if ref_size[0] != test_size[0] or ref_size[1] != test_size[1]:
        raise ImageSizeDoesntMatchException("Image sizes don't match for {ref} and {test}. Expected {ref_size} but got {test_size}".format(
            ref=ref_filename, test=test_filename, ref_size=ref_size, test_size=test_size))


    px_ref_im = ref_im.load()
    px_test_im = test_im.load()
    errorcounter = 0
    for x in range(0,ref_size[0]):
        for y in range(0, ref_size[1]):
            rval = (px_ref_im[x, y])
            tval = (px_test_im[x, y])
            diff = map(lambda l,r: math.fabs(float(l)-float(r)), rval, tval)
            if (diff[0] >= REQUIRED_ACCURACY or
                        diff[1] >= REQUIRED_ACCURACY or
                        diff[2] >= REQUIRED_ACCURACY):
                print("({x}, {y}): Difference: {diff}".format(x=x, y=y, diff=diff))
                errorcounter += 1
    if (errorcounter > 0):
        raise ImagesDontMatchException("Images don't match: Found {n} occurences".format(n=errorcounter))

def run_test(test_binary,  reference_binary, method, arguments, inputfile, test_dir="/tmp/validationtest",
             reference_prefix="ref_", test_prefix="test_", run_reference_program=True):

    file_prefix = os.path.basename(inputfile).replace(".png", "")
    filename = file_prefix+"_"+str(method)+"__"+"".join(arguments)
    filename = filename.replace(".","_")+".png"
    test_filename = test_dir+"/"+test_prefix+filename
    ref_filename = test_dir+"/"+reference_prefix+filename

    if (not os.path.exists(test_dir)):
        os.makedirs(test_dir)

    command_args = arguments+["-m",str(method)]+["-i",inputfile]
    test_command_args = [test_binary]+command_args+["-o",test_filename]
    ref_command_args = [reference_binary]+command_args+["-o",ref_filename]

    if (os.path.exists(test_filename)):
        os.remove(test_filename)

    print("Executing: ", str(test_command_args))
    subprocess.check_call(test_command_args)

    if (run_reference_program):
        if (os.path.exists(ref_filename)):
            os.remove(ref_filename)
        print("Executing: ", str(ref_command_args))
        subprocess.check_call(ref_command_args)

    print("Validating")
    validate(ref_filename, test_filename)
    print("Validated")