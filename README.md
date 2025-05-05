
## Massively parallel modular computations

This package offers a  massively parrallel implementation for the computation of free resolution. In the implementation, we separate the coordination and computations layers allowing the integration of different programming languages without the need to rewrite the entire code. It also facilitates easy editing and optimization of the implementation.  The application relies on the task-based workflow provided by [GPI-Space](http://www.gpi-space.de/) for task coordination, and uses the computer algebra  system [Singular](https://www.singular.uni-kl.de/) for computational tasks.

This application  uses the generic  framework template, which based on [modular](https://github.com/singular-gpispace/modular), implemented by Patrick Rakotoarisoa from  the repository
[template](https://github.com/singular-gpispace/template) with modifications.

To use the framework, installing Singular, GPI-Space, along with their dependencies, and the project code are required. 
We offer two distinct installation methods. The preferred approach involves using the supercomputing package manager Spack, 
which automates the handling of all dependencies. Alternatively, a manual installation of components is available,
serving as anoption if the installation via Spack fails on the target system.


## Example for computation of free resolutions

```bash

LIB "templategspc.lib";
LIB "random.lib";
LIB "modstd.lib"; 
// Configure GPI-Space	
configToken gc = configure_gspc();
gc.options.tmpdir = "/home/santosh/gspc-res/example_dir/temp";
// gc.options.tmpdir = "/dev/shm/gnawali/temp";

gc.options.nodefile = "nodefile";
gc.options.procspernode = 6;
gc.options.loghost = "schipp";
gc.options.logport = 9876;


ring R= 0, (w,x,y,z), (dp,c);
ideal J= w2-x*z, w*x- y*z, x2-w*y, x*y- z2, y2- w*z;

rtimer=0;
system("--ticks-per-sec",1000); // set timer resolution to ms
int t=rtimer;
def re =gspc_template_SchRes(J, gc);
re;

rtimer-t;
print("timer");
```