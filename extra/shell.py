#! /usr/bin/python
import sys, os, stat
MAXLINE = 200
MAXARGS = 20
PIPE_READ = 0
PIPE_WRITE = 1
STDOUT_FILENO = sys.stdout.fileno()
STDIN_FILENO = sys.stdin.fileno()


def execute(argc,arg_list,file_input,file_output,pipe):
    pfd = os.pipe() if pipe else None
    child1 = os.fork()
    child2 = os.fork() if (child1 > 0 and pipe) else None
    if (child1 == -1): print("fork (failed to execute command)")
    if (child1 == 0):
        if (file_output):
            #os.close(STDOUT_FILENO)
            sys.stdout.flush()
            fd = os.open(file_output, os.O_WRONLY | os.O_CREAT, stat.S_IRUSR | stat.S_IWUSR)
            if (fd == -1): print("error open for writing")
            os.dup2(fd, STDOUT_FILENO)
            #sys.stdout = open(file_output, 'w')
        elif (file_input):
            sys.stdin.flush()
            os.close(STDIN_FILENO)
            fd = os.open(file_input, os.O_RDONLY)
            if (fd == -1): print("error open for reading")
        elif (pipe):
            if (-1 == os.close(STDOUT_FILENO)): print("error close")
            fd = os.dup(pfd[PIPE_WRITE])
            if (-1 == fd): print("error dup")
            if (fd != STDOUT_FILENO): sys.stderr.write("Pipe output not at STDOUT.\n")
            os.close(pfd[PIPE_READ])
            os.close(pfd[PIPE_WRITE])
            arg_list.pop()

        #print("%s"%arg_list[0])
        if (-1 == os.execvp(arg_list[0], arg_list)):
            print("execvp  (couldn't find command)")
        sys.exit(1)
    elif (child2 == 0 and pipe):
        if (-1 == os.close(STDIN_FILENO)): print("error close")
        fd = os.dup(pfd[PIPE_READ])
        if (-1 == fd): print("error dup")
        if (fd != STDIN_FILENO): sys.stderr.write("Pipe input not at STDIN.\n")
        os.close(pfd[PIPE_READ])
        os.close(pfd[PIPE_WRITE])
        arg_list[0] = arg_list[1]
        arg_list.pop()
        if (-1 == os.execvp(arg_list[0], arg_list)):
            print("error execvp (couldn't find command)")
    else:
        status = os.waitpid(child1, 0)
        if (-1 == status):
            print("error waitpid")
        if (pipe):
            os.close(pfd[PIPE_READ])
            os.close(pfd[PIPE_WRITE])

            #if (os.WIFEXITED(status) == 0):
            #    print("child1 returned w/ error code %d\n"%os.WEXITSTATUS(status))

            status = os.waitpid(child2, 0)
            if (-1 == status):
                print("error waitpid")

            #if (os.WIFEXITED(status) == 0):
            #    printf("child2 returned w/ error code %d\n", os.WEXITSTATUS(status))
    return



if __name__=="__main__":
    while True:
        try:
            print '%',
            sys.stdout.flush()
            arg_list_and_comment = raw_input().split(' ')
            sys.stdin.flush()
            arg_list = []
            for arg in arg_list_and_comment:
                if '#' in arg:
                    break
                elif arg:
                    arg_list.append(arg)
            argc = len(arg_list)

            if argc == 0: continue
            if arg_list[0] in ['exit','logout']: sys.exit(0)

            pipe = False
            output_file = None
            input_file = None
            background = False

            if '|' in arg_list:
                pipe = True
                arg_list.remove('|')
                argc -= 1
            if '<' in arg_list:
                file_idx = arg_list.index('<') + 1
                if file_idx < argc:
                    input_file = arg_list[file_idx]
                    arg_list.remove('<')
                    arg_list.remove(input_file)
                    argc -= 2
                else:
                    print "Bad command, No input file specified"
                    continue
            if '>' in arg_list:
                file_idx = arg_list.index('>') + 1
                if file_idx < argc:
                    output_file = arg_list[file_idx]
                    arg_list.remove('>')
                    arg_list.remove(output_file)
                    argc -= 2
                else:
                    print "Bad command, No output file specified"
                    continue
            if '&' in arg_list:
                background = True
                arg_list.remove('&')
                argc -= 1

            if background:
                bg_pid = os.fork()
                if bg_pid == 0:
                    execute(argc,arg_list,input_file,output_file,pipe)
                    break
            else:
                execute(argc,arg_list,input_file,output_file,pipe)
        except KeyboardInterrupt:
            print("interrupt caught, to exit please type 'exit'")
