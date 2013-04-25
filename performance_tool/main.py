# Test Tool for Domain Transform Project
# ======================================
#
#
#
# Needs PIL (execute pip install PIL as root)
import socket
import os
import ast
import sys
import git
import compile
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

TESTIMAGES = ['kyoto', 'bhudda']
TESTSIZES = ['640', '720p', '1024', '1080p', '2048', '4k']
GITHUB_URL = 'https://github.com/karhu/domain-transform.git'
BRANCHES = ['reference', 'master']
GIT_DIR = '/tmp/domain-transform'
SRC_DIR = GIT_DIR+"/src"
QMAKE_FILE = "DomainTransform.pro"

#binary = "../DomainTransform-build-GCC47-Default_build_Release/DomainTransform"


def main():
    git_repo = GIT_DIR
    git_origin = GITHUB_URL
    src_dir = SRC_DIR
    qmake_file = QMAKE_FILE
    git.clone_repo(git_origin, git_repo)

    for branch in BRANCHES:
        git.checkout_branch(git_repo, branch)
        git.clean(src_dir)
        compile.compile(qmake_file,cwd=src_dir)

if __name__ == "__main__":
    main()