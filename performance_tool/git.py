import os
import subprocess

GIT_COMMAND = "git"

def clone_repo(repo, directory=os.getcwd(), git_command=GIT_COMMAND):
    if (not os.path.exists(directory+"/.git")):
        subprocess.check_output([git_command, "clone", repo, directory])
    else:
        print "Git Repository {dir} already exists. Pulling instead.".format(dir=directory)
        pull(directory, git_command)

def checkout_branch(directory,branch, git_command=GIT_COMMAND):
    try:
        subprocess.check_output([git_command, "checkout", "-b", branch, "remotes/origin/"+branch], cwd=directory)
    except:
        subprocess.check_output([git_command, "checkout", branch], cwd=directory)

def pull(directory, git_command=GIT_COMMAND):
    subprocess.check_output([git_command, "pull"], cwd=directory)

def clean(directory, git_command=GIT_COMMAND):
    subprocess.check_output([git_command, "clean", "-f"], cwd=directory)

def current_branch_name(directory, git_command=GIT_COMMAND):
    o = subprocess.check_output([git_command, "rev-parse", "--abbrev-ref", "HEAD"])
    return o.rstrip('\n')