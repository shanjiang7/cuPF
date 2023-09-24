# cuPF
Phase field solvers written in C++, CUDA. Scaling: CUDA-aware MPI. 

## Cite

If you are using the codes in this repository, please cite the following paper
```
@article{qin2022dendrite,
  title={Dendrite-resolved, full-melt-pool phase-field simulations to reveal non-steady-state effects and to test an approximate model},
  author={Qin, Yigong and Bao, Yuanxun and DeWitt, Stephen and Radhakrishnan, Balasubramanian and Biros, George},
  journal={Computational Materials Science},
  volume={207},
  pages={111262},
  year={2022},
  publisher={Elsevier}
  url = {https://www.sciencedirect.com/science/article/pii/S0927025622000660}
}
```

## Usage

### 3D grain simulation
cd 3Dgrain  
module load cuda   
make  
./phase_field input_file -s SEED  

### Dendrites simulation  
./phase_field INPUT_FILE MACRO_INPUTS_DIR  

### CUDA-aware MPI
module load xl cuda spectrum_mpi  
export CUDA_PATH=$CUDA_DIR  
export MY_SPECTRUM_OPTIONS="--gpu --aff on" 
ibrun -n NUM_GPUS ./phase_field INPUT_FILE MACRO_INPUTS_DIR 
     
## Performance/scaling
 
<img width="556" alt="timing" src="https://user-images.githubusercontent.com/62076142/119079589-00022f00-b9be-11eb-837f-288778b5244c.png">

<img width="556" alt="timing" src="https://user-images.githubusercontent.com/62076142/119079655-23c57500-b9be-11eb-844f-21b30837c56c.png">

## Example
```
Dentrite-scale simulation
```
https://user-images.githubusercontent.com/62076142/189384626-9093423b-6516-4eb5-9464-cf358a0a4ce4.mp4
```
Grain-scale simulation
```
![3Dview](https://user-images.githubusercontent.com/62076142/189384211-b82a2127-dd0f-4581-9a7e-67f8576419e9.png)



## Reference
```
[1] Blas Echebarria, Roger Folch, Alain Karma, and Mathis Plapp. Quantitative phase-field model of alloy solidification. Physical Review E, 70(6):061604, 2004.
[2] D. Tourret and A. Karma. Three-dimensional dendritic needle network model for alloy solidifi- cation. Acta Materialia, 120:240–254, 2016.
```
## Author
This software was primarily written by Yigong Qin who is advised by Prof. George Biros.
