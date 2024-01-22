# Operativsystem - Project

This repository is regarding the project on the course "Operativsystem". Since the project includes potential risks of damaging the kernel of the operating system this code should only be used inside a Virtual Machine (VM).

## Compilation

- Start the VM.
- Compile the required modules.
- Compile potential C-files that should use the modules.
- Run the wanted tests that is to be done.

## TODO

This sections contains what the next programming session should accomplish and generally what is needed to be done in order to succeed with the project. This is mostly to have a clear goal to strive for.

### 2023-01-22

- Get a clear structure of how the LKM, daemons, sockets and clients should communicate.
- Get a clear image of how the funcions in the LKM should be handled. Should they be implemented as Syscalls, use sockets or other implementations?

## Change log

This section contains information regarding what changes with each programming session. This is generally a good thing to have such that we see if the project goes forward or if help is needed.

### 2023-01-22

- Many examples have been created to understand how the LKM fkn works.
- Tried exporting functions to a header file but it seems that only modules can utilize exported functions.
- Have discussed regarding the structure of the LKMs, socket and client communication.

## Resources

All the resources that might come in usefull will be put in this section for easy access. This includes tutorials, guides on writing the kernel modules and the project specification.

#### Github containing the specification

    https://git.cs.umu.se/courses/5dv171vt24

#### Kernel Modules

    https://wiki.archlinux.org/index.php/kernel_modules

#### Writing a Linux Kernel Module - Part 1.

    http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/

#### The Linux Kernel Module Programming Guide

    https://linux.die.net/lkmpg/index.html
