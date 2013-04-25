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
import benchmark
import compile
IMAGE_DIR = "testimages"

#TESTIMAGES = ['kyoto', 'bhudda']
TESTIMAGES = ['bhudda']
TESTSIZES = ['640', '720p', '1024', '1080p', '2048', '4k']
GITHUB_URL = 'https://github.com/karhu/domain-transform.git'
BRANCHES = ['reference', 'master']
GIT_DIR = '/tmp/domain-transform'
SRC_DIR = GIT_DIR+"/src"
QMAKE_FILE = "DomainTransform.pro"
BINARY = "DomainTransform"
IMAGE_DIR = "testimages"

#binary = "../DomainTransform-build-GCC47-Default_build_Release/DomainTransform"


def main():
    git_repo = GIT_DIR
    git_origin = GITHUB_URL
    src_dir = SRC_DIR
    qmake_file = QMAKE_FILE
    testimages = TESTIMAGES
    testsizes = TESTSIZES
    binary = BINARY
    branches = BRANCHES
    images = []
    for img_prefix in testimages:
        imgs = map(lambda l: img_prefix+"_"+l+".png", testsizes)
        images += imgs
    images = map(lambda l: git_repo+"/"+IMAGE_DIR+"/"+l, images)
    r = benchmark.benchmark(git_repo=git_repo,
                            src_dir=src_dir,
                            branches=branches,
                            git_origin=git_origin,
                            images=images,
                            qmake_file=qmake_file, binary=binary)
    print r

if __name__ == "__main__":
    main()