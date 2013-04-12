#!/usr/bin/env python

"""
Author: Nathan Perry
Assignment: 3
Class: Advanced OS
Professor: Butler

Purpose: Shell Implementation
"""

import sys
import getopt
import os
import string
import resource
import signal


#Global variables
ENV_VAR = {"$AOSPATH":"/bin:/usr/bin", "$AOSCWD":os.getcwd()}
SH_VAR = {}
rLimit_CPU = -1
rLimit_AS = -1
pids = []
NEW_STDIN = 0
NEW_STDOUT = 1
new_stdin = 0
new_stdout = 1
pipesNO = {}
fd_i = []
fd_o = []
def handler(signum, frame):
    for pid in pids:
        p, s = os.waitpid(pid, os.WNOHANG)
        if p == pid:
            print "A background Process Terminated, pid:  ", pid
    #raise IOError("Couldn't open device")

#Switch Statement Class
class switch(object):
    def __init__(self, value):
        self.value = value
        self.fall = False

    def __iter__(self):
        yield self.match
        raise StopIteration

    def match(self, *args):
        if self.fall or not args:
            return True
        elif self.value in args:
            self.fall = True
            return True
        else:
            return False

"""Function if done with program"""
def quit():
    return 0

"""Function takes a filename as argument -- open file and execute commands inside"""
def src(filename):
    #open file, read commands, and execute them
    f = open(filename, 'r')
    lines = f.readlines()

    parse(lines)

    return 1

"""Function sets shell variables"""
def set(var, val):
    #set var = val
    var = "$" + var
    SH_VAR[var] = val

    return 1

"""Function prints whatever is the arg"""
def echo(toPrint):
    #print out toPrint
    for item in toPrint:
        print item

    return 1

"""Function prints out environment variables"""
def envprt():
    #print out all environment variables
    for var in ENV_VAR:
        print "%s: \t%s" %(var, ENV_VAR[var])

    return 1

"""Function sets environment variables"""
def envset(var, val):
    #set var = val
    var = "$" + var
    ENV_VAR[var] = val

    return 1

"""Function unsets/deletes environment variables"""
def envunset(var):
    #get rid of var
    var = "$" + var
    if var in ENV_VAR:
        del ENV_VAR[var]
        return 1
    return -1

"""Function figures out what is being referenced"""
def witch(item):
    #figure out which item is being referenced
    paths = ENV_VAR["$AOSPATH"].split(':')
    for path in paths:
        for f in os.listdir(path):
            if f == item:
                print "%s/%s" %(path, f)
                return 1

    return 1

"""Function that sets limits"""
def lim(args):
    global rLimit_CPU, rLimit_AS
    #args can be of size 0 or 2
    if len(args) == 0:
        print rLimit_CPU
        print rLimit_AS
    else:
        rLimit_CPU = args[0] #resource.setrlimit(resource.RLIMIT_CPU, (int(args[0]), int(args[0])))
        rLimit_AS = args[1]  #resource.setrlimit(resource.RLIMIT_AS, (int(args[1])*1024*1024, int(args[1])*1024*1024))

    return 1

"""Function prints current working directory"""
def pwd():
    #print current working directory
    print ENV_VAR["$AOSCWD"]
    return 1

"""Function to change working directory"""
def cd(args):
    #may be 0 or 1
    if len(args) == 0:
        return 1
    else:
        os.chdir(args[0])
        ENV_VAR["$AOSCWD"] = os.getcwd()
        return 1

"""Function to parse list of commands"""
def parse(commands):
    for command in commands:
        interpolate(command)
    return 1

"""Function to interpolate a string"""
def interpolate(command):
    if '#' in command:
        pos = string.find(command, "#")
        command = command[0:pos].strip()
    for var in ENV_VAR:
        if var in command:
            command = string.replace(command, var, ENV_VAR[var])
    for var in SH_VAR:
        if var in command:
            command = string.replace(command, var, SH_VAR[var])
    execute(command)
    return 1

def findCommand(item):
    #figure out which item is being referenced
    interpolate( "envset AOSPATH .:$AOSPATH")
    if item[0:2] == "./":
        item = item[2:]
    paths = ENV_VAR["$AOSPATH"].split(':')
    for path in paths:
        for f in os.listdir(path):
            if f == item:
                ENV_VAR["$AOSPATH"] = ENV_VAR["$AOSPATH"][2:]
                return path + "/" + item

"""Function to execute a single command"""
def execute(command, fdi = [], fdo = [], hasPipes = False):
    global NEW_STDIN, NEW_STDOUT, new_stdin, new_stdout
    if ';' in command:
        commands = command.split(';')
        for item in commands:
            NEW_STDIN = 0
            NEW_STDOUT = 1
            new_stdin = 0
            new_stdout = 1
            execute(item.lstrip())
        return


    commands = command.split()

    for case in switch(commands[0]):
        if case("envprt"):
            #print command
            envprt()
            break
        if case("echo"):
            #print command
            commands.pop(0)
            echo(commands)
            break
        if case("envset"):
            #print command
            envset(commands[1], commands[2])
            break
        if case("set"):
            #print command
            set(commands[1], commands[2])
            break
        if case("envunset"):
            #print command
            envunset(commands[1])
            break
        if case("pwd"):
            #print command
            pwd()
            break
        if case("cd"):
            #print command
            commands.pop(0)
            cd(commands)
            break
        if case("src"):
            #print command
            src(commands[1])
            break
        if case("witch"):
            #print command
            witch(commands[1])
            break
        if case("lim"):
            #print command
            commands.pop(0)
            lim(commands)
            break
        if case():
            #print "Large Command: %s" %command
            numPipes = command.count('|')
            if numPipes != 0:
                coms = command.split('|')
                pipe_count = 0
                for item in coms:
                    NEW_STDIN = new_stdin
                    NEW_STDOUT = new_stdout
                    r,w = os.pipe()
                    pipesNO[pipe_count] = [r, w]
                    new_stdout = w
                    pid = os.fork()

                    if pid:
                        #we are parent
                        if pipe_count == numPipes:
                            new_stdout = NEW_STDOUT
                            pipe_count -= 1
                            keep_std_in = os.dup(NEW_STDIN)
                            os.dup2(new_stdin, NEW_STDIN)
                            os.close(NEW_STDIN)
                            os.dup2(keep_std_in, NEW_STDIN)
                            os.close(keep_std_in)

                            for thing in pipesNO:
                                try:
                                    os.close(pipesNO[thing][0])
                                    os.close(pipesNO[thing][1])
                                except OSError:
                                    pass
                        else:
                            new_stdin = r
                            os.close(w)
                            pipe_count += 1
                    else:
                        #we are child
                        if new_stdin != NEW_STDIN:
                            dup2(new_stdin, NEW_STDIN)
                            os.close(new_stdin)
                        os.dup2(w, sys.stdout.fileno())
                        os.close(new_stdout)
                        execute(coms[pipe_count], [], [], True)
                        os.close(NEW_STDOUT)
                        sys.exit(0)
            else:
                path = findCommand(commands[0])
                path = os.path.realpath(path)
                if os.path.isfile(path):
                    if len(commands) > 1:
                        args = commands[1:]
                    else:
                        args = ""
                    if ">" in command:
                        fd = os.open(commands[3], os.O_WRONLY | os.O_TRUNC | os.O_CREAT, 0666)
                        fd_o.append(fd)
                        comg = command.split(">")
                        execute(comg[0], fd_i, fd_o)
                        del fd_i[:]
                        del fd_o[:]
                    elif "<" in command:
                        fd = os.open(commands[2], os.O_RDONLY, 0666)
                        if fd < 0:
                            print "FILE NOT FOUND"
                            return 0
                        fd_i.append(fd)
                        coml = command.split("<")
                        execute(coml[0], fd_i, fd_o)
                        del fd_i[:]
                        del fd_o[:]
                    elif "&" in command:
                        rc = os.fork()
                        pids.append(rc)
                        if(rc == 0):
                            resource.setrlimit(resource.RLIMIT_CPU, (int(rLimit_CPU), int(rLimit_CPU)))
                            resource.setrlimit(resource.RLIMIT_AS, (int(rLimit_AS)*1024*1024, int(rLimit_AS)*1024*1024))
                            os.execve(path, (path,) + tuple(args), ENV_VAR)
                        else:
                            print "Child Process started with PID: ", rc
                    else:
                        if(!hasPipes)
                            rc = os.fork()
                        if(rc):
                            nom = os.waitpid(rc, 0);
                        else:
                            resource.setrlimit(resource.RLIMIT_CPU, (int(rLimit_CPU), int(rLimit_CPU)))
                            resource.setrlimit(resource.RLIMIT_AS, (int(rLimit_AS)*1024*1024, int(rLimit_AS)*1024*1024))
                            if(len(fdo)):
                                os.dup2(fdo[0], NEW_STDOUT)
                                os.close(fdo[0])
                            elif len(fdi):
                                os.dup2(fdi[0], NEW_STDIN)
                                os.close(fdi[0])
                            os.execve(path, (path,) + tuple(args), ENV_VAR) 

            break;
    #print
    return 1

"""Function for program with 0 args"""
def shell(p = True):
    #Setup infinite loop
    keepGoing = True
    while keepGoing:
        #print shell message
        if(p):
            print "nep2d_sh> " ,

        #try and get some user_input
        try:
            var = raw_input()
        except:
            var = "exit"
            if(p):
                print

        #Make sure not end of stream
        if var == "exit":
            keepGoing = False
        else:
            interpolate(var)
    return 0

"""main function"""
def main():
    signal.signal(signal.SIGCHLD, handler)
    signal.siginterrupt(signal.SIGCHLD, False)
    #Check for args
    if sys.stdin.isatty() and len(sys.argv) == 1:
        #call shell with prints 
        shell()
    else:
        #call shell without prints
        if(sys.stdin.isatty()):
            src(sys.argv[1])
        else:
            shell(False)
    return 0

if __name__ == "__main__":
    main()
