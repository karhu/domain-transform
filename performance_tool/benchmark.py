import ast
import git
import socket
import subprocess
import compile

def computer_name():
    return socket.gethostname()

def run(binary, images, method='nc', args=['-s', '20', '-r', '0.8', '-n', '2']):
    print binary
    results = []
    for image in images:
        o = subprocess.check_output([binary,'-m', method,"-b", "-i", image]+args)
        r = ast.literal_eval(o)
        results.append(r)
    return results

def benchmark(git_repo, src_dir, branches, git_origin, images, qmake_file, binary):
    git.clone_repo(git_origin, git_repo)
    results = {"computer_name": computer_name()}
    for branch in branches:
        git.checkout_branch(git_repo, branch)
        git.clean(src_dir)
        compile.compile(qmake_file,cwd=src_dir)

        r = {"revision": git.current_revision(git_repo)}
        r["results"] = run(src_dir+"/"+binary, images)
        results[branch] = r

    return results
