import os
import subprocess

GIT_COMMAND = "git"
class CouldntCloneGitRepositoryException(Exception):
    pass
class CouldntCheckoutGitBranchException(Exception):
    pass
class CouldntPullFromGitRepositoryException(Exception):
    pass

def clone_repo(repo, directory=os.getcwd(), git_command=GIT_COMMAND):
    try:
        response=subprocess.check_output([git_command, "clone", repo, directory], shell=True)
    except subprocess.CalledProcessError:
        raise CouldntCloneGitRepositoryException(response)

def checkout_branch(directory,branch, git_command=GIT_COMMAND):
    try:
        response=subprocess.check_output([git_command, "checkout", "-b", branch, "origin/"+branch], shell=True, cwd=directory)
    except:
        message = "Couldn't checkout repo {repo} in {dir}: {ex}".format(repo=directory, branch=str(branch), response)
        raise CouldntCheckoutGitBranchException(message)

def pull(directory, git_command=GIT_COMMAND):
    try:
        response = subprocess.check_output([git_command, "pull"])
    except:
        message = "Couldn't pull in {dir}: {ex}".format(dir=directory, ex=response)
        raise CouldntPullFromGitRepositoryException(message)