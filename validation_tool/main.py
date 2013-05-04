# Test Tool for Domain Transform Project
# ======================================
#
#
#
# Needs PIL (execute pip install PIL as root)
import tester
import os
import git

REFERENCE_IMAGES_GIT = "http://gitlab.rsp.li/domain-transform-project-reference-images.git"
TEST_DIR = '/tmp/validationtest'

default_image_dir = "../testimages"

test_arguments = {
    'nc': [
        {'n': 2, 's': 20.0, 'r': 0.3},
        {'n': 2, 's': 20.0, 'r': 0.8},
        {'n': 2, 's': 100.0, 'r': 0.8},
        {'n': 2, 's': 100.0, 'r': 1.4},
    ],
    # 'rf': [
    #     {'n': 1, 's': 20.0, 'r': 0.3},
    #     {'n': 2, 's': 20.0, 'r': 0.8},
    #     {'n': 3, 's': 100.0, 'r': 0.8},
    #     {'n': 4, 's': 100.0, 'r': 1.4}
    # ]
}

reference_argument = 'ref'

test_binary = "../build/DomainTransform"
reference_binary = test_binary


def main():
    image_dir = default_image_dir
    files = [image_dir+"/"+f for f in os.listdir(image_dir) if os.path.isfile(os.path.join(image_dir, f)) and f.endswith(".png")]
    git.clone_repo(REFERENCE_IMAGES_GIT, TEST_DIR)
    for method, arguments in test_arguments.iteritems():
        for args in arguments:
            # Prepend a "-" for every key in the arguments dict and then flatten the list
            command_arguments = [i for tuple in args.items() for i in ("-"+str(tuple[0]),str(tuple[1]))]
            for f in files:
                tester.run_test(test_binary, reference_binary, method, command_arguments, f, run_reference_program=False, test_dir=TEST_DIR)

if __name__ == "__main__":
    main()
