# Kernel_Experiments
This is a group project of Akinizer and Volkan Sevinç

1. General Information
In the experiments, we aimed to observe the behavior of virtual memory fields of a linux process. We dynamically allocated memory from heap and stack during our experiments. We chose 1K, 10K, 100K and 1M as our memory allocation sizes. We used a simple while loop for heap memory allocation and a basic recursive function for our stack memory allocation. Then we observed the difference in the virtual memory fields.
2. Environment and Setup
In the experiments we ran our codes on an Ubuntu 16.04.05 64-Bit machine that is working as dual boot to a Windows 10 Operating System. Computer we worked on has 32GB’s of RAM and 6 CPU core with Intel Hyper Threading technology each core running at 5.0 GHz. The kernel’s version is 4.16. During the experiments, we encountered some errors that we did not have during part A of the project. Because the said CPU(Intel i7 8700K)is a Coffee Lake CPU, Linux Kernel’s compiled successfully but could not load some modules such as nouveau and i915 module which are used by NVIDIA GPU’s and Intel GFX Drivers respectively. This caused kworker threads u23 and u24 to overload the CPU and freeze the system upon its first boot. This led us to try and use different Kernel versions. We modified GRUB files manually but weren’t able fix the problem that way. Kernel release 4.16 proved to be stable at last. Kernel versions 4.19.8 and 4.20.0-rc6 both had the same problem.
Our kernel module takes a process id(pid) and then prints out the vm_area_struct’s begin addresses, end addresses, their sizes, rss, total_vm, and top level page table entries.
We also have a C file named app.c which dynamically allocates memory for heap and stack to observe differences.
3. Experimenting and Analysis
Data, Code, main args, environment variables fields were constant during the test runs. Most top level page table entries were zero because they are unused virtual addresses. We also printed these one entry as a time as stated in the project description. Sample runs can be found below.
