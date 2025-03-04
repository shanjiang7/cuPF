//////////////////////
// params.h
// COPYRIGHT Yigong Qin, ALL RIGHTS RESERVED
/////////////////////

#pragma once

#include <string>

class GlobalConstants 
{
public:
  // processing parameters
  int thermalType;
  float G, R, delta, kin_delta, underCoolingRate, V;
  float underCoolingRate0, nuc_Nmax0;

  // physical parameters
  float k, c_infty, m_slope, c_infm, Dl, GT;
  float Dh, d0, W0, L_cp;
  float lamd, tau0, beta0, mu_k, lT;
  float R_tilde, Dl_tilde, lT_tilde, beta0_tilde;
  float alpha0, U0, eps, eta;  

  // geometry parameters
  float lx, lxd, lyd, lzd, tmax;
  float xmin, ymin, zmin; 
  float asp_ratio_yx, asp_ratio_zx, moving_ratio;
  float z0, r0, top, angle, min_angle;
  float mp_len;
  // grid parameters
  int nx, ny, nz, nz_full, fnx, fny, fnz, fnz_f, length, full_length, haloWidth;
  int Mt, preMt, nts, NUM_PF; 
  float dx, dt, hi, cfl, dt_sqrt;
  int Nx, Ny, Nz, Nt;

  // initial/boundary condition parameters
  int ictype;
  int bcX, bcY, bcZ;
  int num_theta, num_nodes;

  // anisotropy parameters
  bool includeInterfaceAnisotropy;
  float cosa, sina, sqrt2, a_s, epsilon, a_12;

  // noise parameters
  int noi_period, seed_val; 

  // nucleation parameters
  float Tmelt, Ti, Tliq, Tsol;
  float undcool_mean, undcool_std, nuc_Nmax, nuc_rad, pts_cell; 


  

};











