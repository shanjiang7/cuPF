#include "params.h"
#include "PhaseField.h"
#include <cmath>
#include <iostream>
#include <hdf5.h>
using namespace std;
#define LS -0.995
// constructor

PhaseField::PhaseField() {

    x = nullptr;
    phi = nullptr;
    x_device = nullptr;
    phi_new = nullptr;
}


void PhaseField::cpuSetup(params_MPI &pM){


    pM.nx_loc = params.nx/pM.nprocx;
    pM.ny_loc = params.ny/pM.nprocy;
    pM.nz_loc = params.nz;
    pM.nz_full_loc = params.nz_full;
    
    float dxd = params.dx*params.W0;
    float len_blockx = pM.nx_loc*dxd; //params.lxd/pM.nprocx;
    float len_blocky = pM.ny_loc*dxd; //params.lyd/pM.nprocy;
    
    float xmin_loc = params.xmin+ pM.px*len_blockx; 
    float ymin_loc = params.ymin+ pM.py*len_blocky; 
    float zmin_loc = params.zmin;

    if (pM.px==0){pM.nx_loc+=1;}
    else{xmin_loc+=dxd;} //params.lxd/params.nx;}
    
    if (pM.py==0){pM.ny_loc+=1;}
    else{ymin_loc+=dxd;}//params.lyd/params.ny;}

    pM.nz_loc+=1;
    pM.nz_full_loc+=1;

    params.fnx = pM.nx_loc+2*params.ha_wd;
    params.fny = pM.ny_loc+2*params.ha_wd;
    params.fnz = pM.nz_loc+2*params.ha_wd;
    params.fnz_f = pM.nz_full_loc+2*params.ha_wd;
    fnx = params.fnx, fny = params.fny, fnz = params.fnz, fnz_f = params.fnz_f;
    params.length=fnx*fny*fnz;
    params.full_length = fnx*fny*fnz_f;
    length = params.length,full_length = params.full_length, NUM_PF = params.NUM_PF;

    x = new float[fnx];
    y = new float[fny];
    z = new float[fnz];
    z_full = new float[fnz_f];

    for(int i=0; i<fnx; i++){
        x[i]=(i-params.ha_wd)*dxd + xmin_loc; 
    }

    for(int i=0; i<fny; i++){
        y[i]=(i-params.ha_wd)*dxd + ymin_loc;
    }

    for(int i=0; i<fnz; i++){
        z[i]=(i-params.ha_wd)*dxd + zmin_loc;
    }

    for(int i=0; i<fnz_f; i++){
        z_full[i]=(i-params.ha_wd)*dxd + zmin_loc;
    }


    psi = new float[length];
    phi = new float[length];
  //  Uc = new float[length];
    alpha = new int[length];
    alpha_i_full = new int[full_length];

  //  std::cout<<"x= ";
  //  for(int i=0; i<fnx; i++){
  //      std::cout<<x[i]<<" ";
  //  }
    cout<< "rank "<< pM.rank<< " xmin " << x[0] << " xmax "<<x[fnx-1]<<endl;
    cout<< "rank "<< pM.rank<< " ymin " << y[0] << " ymax "<<y[fny-1]<<endl;
    cout<< "rank "<< pM.rank<< " zmin " << z[0] << " zmax "<<z[fnz-1]<<endl;
    cout<<"x length of psi, phi, U="<<fnx<<endl;
    cout<<"y length of psi, phi, U="<<fny<<endl;
    cout<<"z length of psi, phi, U="<<fnz<<endl;   
    cout<<"length of psi, phi, U="<<length<<endl;
 

}


void PhaseField::initField(Mac_input mac){

    for (int i=0; i<2*NUM_PF+1; i++){
        //mac.theta_arr[i+1] = 1.0f*(rand()%10)/(10-1)*(-M_PI/2.0);
       // mac.theta_arr[i+1] = 1.0f*rand()/RAND_MAX*(-M_PI/2.0);
       // mac.theta_arr[i+1] = (i)*grain_gap- M_PI/2.0;
        mac.sint[i] = sinf(mac.theta_arr[i]);
        mac.cost[i] = cosf(mac.theta_arr[i]);
    }  
   

     
    float Dx = mac.X_mac[mac.Nx-1] - mac.X_mac[mac.Nx-2];
    float Dy = mac.Y_mac[mac.Ny-1] - mac.Y_mac[mac.Ny-2];
    float Dz = mac.Z_mac[mac.Nz-1] - mac.Z_mac[mac.Nz-2];    
    for(int id=0; id<length; id++){
      int k = id/(fnx*fny);
      int k_r = id - k*fnx*fny;
      int j = k_r/fnx;
      int i = k_r%fnx; 
      
      if ( (i>params.ha_wd-1) && (i<fnx-params.ha_wd) && (j>params.ha_wd-1) && (j<fny-params.ha_wd) && (k>params.ha_wd-1) && (k<fnz-params.ha_wd)){
      int kx = (int) (( x[i] - mac.X_mac[0] )/Dx);
      float delta_x = ( x[i] - mac.X_mac[0] )/Dx - kx;
         //printf("%f ",delta_x);
      int ky = (int) (( y[j] - mac.Y_mac[0] )/Dy);
      float delta_y = ( y[j] - mac.Y_mac[0] )/Dy - ky;

      int kz = (int) (( z[k] - mac.Z_mac[0] )/Dz);
      float delta_z = ( z[k] - mac.Z_mac[0] )/Dz - kz;

      if (kx==mac.Nx-1) {kx = mac.Nx-2; delta_x =1.0f;}
      if (ky==mac.Ny-1) {ky = mac.Ny-2; delta_y =1.0f;}
      if (kz==mac.Nz-1) {kz = mac.Nz-2; delta_z =1.0f;}

      int offset =  kx + ky*mac.Nx + kz*mac.Nx*mac.Ny;
      int offset_n =  kx + ky*mac.Nx + (kz+1)*mac.Nx*mac.Ny;
      //if (offset>mac.Nx*mac.Ny-1-1-mac.Nx) printf("%d, %d, %d, %d  ", i,j,kx,ky);
      psi[id] = ( (1.0f-delta_x)*(1.0f-delta_y)*mac.psi_mac[ offset ] + (1.0f-delta_x)*delta_y*mac.psi_mac[ offset+mac.Nx ] \
               +delta_x*(1.0f-delta_y)*mac.psi_mac[ offset+1 ] +   delta_x*delta_y*mac.psi_mac[ offset+mac.Nx+1 ] )*(1.0f-delta_z) + \
             ( (1.0f-delta_x)*(1.0f-delta_y)*mac.psi_mac[ offset_n ] + (1.0f-delta_x)*delta_y*mac.psi_mac[ offset_n+mac.Nx ] \
               +delta_x*(1.0f-delta_y)*mac.psi_mac[ offset_n+1 ] +   delta_x*delta_y*mac.psi_mac[ offset_n+mac.Nx+1 ] )*delta_z;

  
     //   psi[id]=0.0;
      psi[id] = psi[id]/params.W0;
      phi[id]=tanhf(psi[id]/params.sqrt2);
     //   Uc[id]=0.0;
      if (phi[id]>LS){

        alpha[id] = mac.alpha_mac[(j-1)*(fnx-2*params.ha_wd)+(i-1)];
       if (alpha[id]<1 || alpha[id]>NUM_PF) cout<<(j-1)*(fnx-2*params.ha_wd)+(i-1)<<alpha[id]<<endl;
       }

      else {alpha[id]=0;}
      }

    else{
       psi[id]=0.0f;
       phi[id]=0.0f;
     //  Uc[id]=0.0f;
       alpha[id]=0;
 
    }
    }



}


void PhaseField::output(params_MPI pM){

    string out_format = "ML3D_PF"+to_string(NUM_PF)+"_train"+to_string(q.num_case-q.valid_run)+"_test"+to_string(q.valid_run)+\
    "_Mt"+to_string(params.Mt)+"_grains"+to_string(params.num_theta)+"_frames"+to_string(params.nts)+\
    "_anis"+to_stringp(params.kin_delta,3)+"_G"+to_stringp(params.G,3)+"_Rmax"+to_stringp(params.R,3)+"_seed"+to_string(atoi(argv[3]));
    string out_file = out_format+ "_rank"+to_string(pM.rank)+".h5";
    out_file = "/scratch1/07428/ygqin/" + out_direc + "/" +out_file;
    cout<< "save dir" << out_file <<endl;

    hid_t  h5_file; 


    h5_file = H5Fcreate(out_file.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    h5write_1d(h5_file, "phi",      phi , length, "float");
    h5write_1d(h5_file, "alpha",  alpha_i_full, full_length, "int");
   // h5write_1d(h5_file, "alpha",    alpha_asse, valid_run*full_length, "int");
  //  h5write_1d(h5_file, "sequence", aseq_asse, num_case*params.num_theta, "int");

    h5write_1d(h5_file, "x_coordinates", x, fnx, "float");
    h5write_1d(h5_file, "y_coordinates", y, fny, "float");
    h5write_1d(h5_file, "z_coordinates", z_full, params.fnz_f, "float");

    h5write_1d(h5_file, "y_t",       q.tip_y_asse,   q.num_case*(params.nts+1), "float");
    h5write_1d(h5_file, "fractions", q.frac_asse,   q.num_case*(params.nts+1)*params.num_theta, "float");
    h5write_1d(h5_file, "angles",    q.angles_asse, q.num_case*(2*NUM_PF+1), "float");

    h5write_1d(h5_file, "extra_area", q.extra_area_asse,   q.num_case*(params.nts+1)*params.num_theta, "int");
    h5write_1d(h5_file, "total_area", q.total_area_asse,   q.num_case*(params.nts+1)*params.num_theta, "int");
    h5write_1d(h5_file, "tip_y_f", q.tip_final_asse,   q.num_case*(params.nts+1)*params.num_theta, "int");
    h5write_1d(h5_file, "cross_sec", q.cross_sec,  q.num_case*(params.nts+1)*fnx*fny, "int");

    H5Fclose(h5_file);
    H5Dclose(datasetT);
    H5Sclose(dataspaceT);
    H5Sclose(memspace);
    H5Fclose(h5in_file);

}




