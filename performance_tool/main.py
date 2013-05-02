# Performance Tool for Domain Transform Project
# ======================================

import benchmark
import pickle
import getopt
from sys import argv
IMAGE_DIR = "testimages"

#TESTIMAGES = ['kyoto', 'bhudda']
TESTIMAGES = ['kyoto']
TESTSIZES = ['640', '720p', '1024', '1080p', '2048', '4k']
#GITHUB_URL = 'https://github.com/karhu/domain-transform.git'
GITHUB_URL = '.' # take current working directory as dir
BRANCHES = ['master']
GIT_DIR = '/tmp/domain-transform'
SRC_DIR = GIT_DIR+"/src"
QMAKE_FILE = "DomainTransform.pro"
BINARY = "/../build/DomainTransform"
IMAGE_DIR = "testimages"

#binary = "../DomainTransform-build-GCC47-Default_build_Release/DomainTransform"


def main(argv):
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

    try:
        opts, remainder = getopt.getopt(argv, "d:", ["dir="])
    except getopt.GetoptError:
        print "Error in passed parameters"
    else:
        if opts:
            opt, git_dir = opts[0]
            src_dir = git_dir + "/src/"
            images = map(lambda l: git_dir+"/"+IMAGE_DIR+"/"+l, images)
            r = benchmark.benchmark_local(src_dir=src_dir,
                                          images=images,
                                          qmake_file=qmake_file, binary=binary)
        else:
            print 'In case you want to specify an existing source directory, use "main.py -d <git_dir>" or "main.py dir=<git_dir>'
            images = map(lambda l: git_repo+"/"+IMAGE_DIR+"/"+l, images)
            r = benchmark.benchmark(git_repo=git_repo,
                                        src_dir=src_dir,
                                        branches=branches,
                                        git_origin=git_origin,
                                        images=images,
                                        qmake_file=qmake_file, binary=binary)
    print r

    from datetime import datetime
    f = open("run_"+benchmark.computer_name()+"_"+datetime.now().strftime('%y_%m_%d__%H_%M')+'.pickle', 'wb')
    pickle.dump(r, f)

if __name__ == "__main__":
    main(argv[1:])
