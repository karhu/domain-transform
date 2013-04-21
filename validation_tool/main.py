# Test Tool for Domain Transform Project
# ======================================
#
#
#
# Needs PIL (execute pip install PIL as root)
import tester
import os
default_image_dir = "../testimages"

test_arguments = {
    'nc': [
        {'n': 1, 's': 40.0, 'r': 0.5},
        {'n': 2, 's': 40.0, 'r': 0.5},
        {'n': 3, 's': 40.0, 'r': 0.5},
        {'n': 4, 's': 40.0, 'r': 0.5}
    ],
    'rf': [
        {'n': 1, 's': 40.0, 'r': 0.5},
        {'n': 2, 's': 40.0, 'r': 0.5},
        {'n': 3, 's': 40.0, 'r': 0.5},
        {'n': 4, 's': 40.0, 'r': 0.5}
    ]
}

reference_argument = 'ref'

test_binary = "../DomainTransform-build-GCC47-Default_build_Release/DomainTransform"
reference_binary = test_binary


def main():
    image_dir = default_image_dir
    files = [image_dir+"/"+f for f in os.listdir(image_dir) if os.path.isfile(os.path.join(image_dir, f)) and f.endswith(".png")]
    for method, arguments in test_arguments.iteritems():
        for args in arguments:
            # Prepend a "-" for every key in the arguments dict and then flatten the list
            command_arguments = [i for tuple in args.items() for i in ("-"+str(tuple[0]),str(tuple[1]))]
            for f in files:
                tester.run_test(test_binary, reference_binary, method, command_arguments, f)

if __name__ == "__main__":
    main()