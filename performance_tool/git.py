import os
import subprocess

GIT_COMMAND = "git"

def clone_repo(repo, directory=os.getcwd(), git_command=GIT_COMMAND):
    print "Cloning {repo} into {directory}".format(repo=repo, directory=directory)
    if (not os.path.exists(directory+"/.git")):
        subprocess.check_output([git_command, "clone", repo, directory])
    else:
        print "Git Repository {dir} already exists. Pulling instead.".format(dir=directory)
        pull(directory, git_command)

def add_origin(remote, origin="origin", directory=os.getcwd(), git_command=GIT_COMMAND):
    subprocess.check_output([git_command, "remote", "add", origin, remote], cwd=directory)

def checkout_branch(directory,branch, git_command=GIT_COMMAND):
    print "Checkout {branch} in {directory}".format(branch=branch, directory=directory)
    subprocess.check_output([git_command, "fetch", "origin"], cwd=directory)
    # try:
    #     subprocess.check_output([git_command, "checkout", "-t", "origin/"+branch], cwd=directory)
    # except:
    subprocess.check_output([git_command, "checkout", "origin/"+branch], cwd=directory)

def pull(directory, git_command=GIT_COMMAND):
    print "Executing a pull in {directory}".format(directory=directory)
    subprocess.check_output([git_command, "pull"], cwd=directory)

def clean(directory, git_command=GIT_COMMAND):
    print "Executing a clean in {directory}".format(directory=directory)
    subprocess.check_output([git_command, "clean", "-f", "-n", "-d", "-x"], cwd=directory)

def current_branch_name(directory, git_command=GIT_COMMAND):
    o = subprocess.check_output([git_command, "rev-parse", "--abbrev-ref", "HEAD"])
    return o.rstrip('\n')

def current_revision(directory, git_command=GIT_COMMAND):
    o = subprocess.check_output([git_command, "rev-parse", "HEAD"])
    return o.rstrip('\n')

def fetch_all(directory, git_command=GIT_COMMAND):
    print "Fetching all from {directory}".format(directory=directory)

    subprocess.check_call([git_command, "fetch", "-a"], cwd=directory)