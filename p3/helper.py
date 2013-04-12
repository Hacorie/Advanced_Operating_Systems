def piper(pid, r, w):
    if pid:
        if w != -1:
            os.close(w)
        os.wait(pid, 0)
    else:
        if r != -1:
            os.dup2(r, NEW_STDIN)
            new_stdin = r
            os.close(r)
        if w != -1:
            os.dup2(w, NEW_STDOUT)
            new_stdout = w
            os.close(w)
        execute(item)
