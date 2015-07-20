namespace libcloudphxx
{
  namespace lgrngn
  {
    template <typename real_t, backend_t device>
    void particles_t<real_t, device>::impl::init_kernel()
    {
      //temporary vector to store kernel efficiencies before they are appended to user input parameters
      std::vector<real_t> tmp_kernel_eff;

      switch(opts_init.kernel)
      {
        case(golovin):
          if(n_user_params != 1)
          {
            throw std::runtime_error("Golovin kernel accepts exactly one parameter.");
          }
          // init device kernel parameters vector
          kernel_parameters.resize(n_user_params);
          thrust::copy(opts_init.kernel_parameters.begin(), opts_init.kernel_parameters.end(), kernel_parameters.begin());

          // init kernel
          k_golovin.resize(1, kernel_golovin<real_t, n_t> (kernel_parameters.data()));
          p_kernel = (&(k_golovin[0])).get();
          break;

        case(geometric):
          if(n_user_params != 0)
          {
            throw std::runtime_error("Geometric kernel doesn't accept parameters.");
          }

          // init kernel
          k_geometric.resize(1, kernel_geometric<real_t, n_t> ());
          p_kernel = (&(k_geometric[0])).get();
          break;
        
  
        //Hall kernel
        case(hall):
          if(n_user_params != 0)
          {
            throw std::runtime_error("Hall kernel doesn't accept parameters.");
          }
          //read in kernel efficiencies to a temporary container
          detail::hall_efficiencies<real_t> (tmp_kernel_eff);
         
          //reserve device memory for kernel parameters vector
          kernel_parameters.resize(opts_init.kernel_parameters.size() + tmp_kernel_eff.size());

          //copy user-defined parameters to device memory
          thrust::copy(opts_init.kernel_parameters.begin(), opts_init.kernel_parameters.end(), kernel_parameters.begin());

          //append efficiencies to device vector
          thrust::copy(tmp_kernel_eff.begin(), tmp_kernel_eff.end(), kernel_parameters.begin()+n_user_params);

          // init kernel
          k_geometric_with_efficiencies.resize(1, kernel_geometric_with_efficiencies<real_t, n_t> (kernel_parameters.data(), detail::hall_r_max<real_t>()));
          p_kernel = (&(k_geometric_with_efficiencies[0])).get();
          break;


        //Hall kernel with Davis and Jones (no van der Waals) efficiencies for small molecules (like Shima et al. 2009)
        case(hall_davis_no_waals):
          if(n_user_params != 0)
          {
            throw std::runtime_error("Hall + Davis kernel doesn't accept parameters.");
          }
          //read in kernel efficiencies to a temporary container
          detail::hall_davis_no_waals_efficiencies<real_t> (tmp_kernel_eff);
         
          //reserve device memory for kernel parameters vector
          kernel_parameters.resize(opts_init.kernel_parameters.size() + tmp_kernel_eff.size());

          //copy user-defined parameters to device memory
          thrust::copy(opts_init.kernel_parameters.begin(), opts_init.kernel_parameters.end(), kernel_parameters.begin());

          //append efficiencies to device vector
          thrust::copy(tmp_kernel_eff.begin(), tmp_kernel_eff.end(), kernel_parameters.begin()+n_user_params);

          // init kernel
          k_geometric_with_efficiencies.resize(1, kernel_geometric_with_efficiencies<real_t, n_t> (kernel_parameters.data(), detail::hall_davis_no_waals_r_max<real_t>()));
          p_kernel = (&(k_geometric_with_efficiencies[0])).get();
          break;

        //Hall kernel with Pinsky gravitational (stagnant) efficiencies for small molecules at p=1000mb
        case(hall_pinsky_1000mb_grav):
          if(n_user_params != 0)
          {
            throw std::runtime_error("Hall + Pinsky kernel doesn't accept parameters.");
          }
          //read in kernel efficiencies to a temporary container
          detail::hall_pinsky_1000mb_grav_efficiencies<real_t> (tmp_kernel_eff);
         
          //reserve device memory for kernel parameters vector
          kernel_parameters.resize(opts_init.kernel_parameters.size() + tmp_kernel_eff.size());

          //copy user-defined parameters to device memory
          thrust::copy(opts_init.kernel_parameters.begin(), opts_init.kernel_parameters.end(), kernel_parameters.begin());

          //append efficiencies to device vector
          thrust::copy(tmp_kernel_eff.begin(), tmp_kernel_eff.end(), kernel_parameters.begin()+n_user_params);

          // init kernel
          k_geometric_with_efficiencies.resize(1, kernel_geometric_with_efficiencies<real_t, n_t> (kernel_parameters.data(), detail::hall_pinsky_1000mb_grav_r_max<real_t>()));
          p_kernel = (&(k_geometric_with_efficiencies[0])).get();
          break;

        //Onishi turbulent kernel (Onishi 2015 JAS) with Hall, Davis and Jones (no van der Waals) efficiencies 
        case(onishi_hall_davis_no_waals):
          if(n_user_params != 2)
          {
            throw std::runtime_error("Please supply two kernel parameters: rate of dissipation epsilon [m^2/s^3] and Taylor microscale Reynolds number.");
          }
          //read in kernel efficiencies to a temporary container
          detail::hall_davis_no_waals_efficiencies<real_t> (tmp_kernel_eff);
         
          //reserve device memory for kernel parameters vector
          kernel_parameters.resize(opts_init.kernel_parameters.size() + tmp_kernel_eff.size());

          //copy user-defined parameters to device memory
          thrust::copy(opts_init.kernel_parameters.begin(), opts_init.kernel_parameters.end(), kernel_parameters.begin());

          //append efficiencies to device vector
          thrust::copy(tmp_kernel_eff.begin(), tmp_kernel_eff.end(), kernel_parameters.begin()+n_user_params);

          // init kernel
          k_onishi.resize(1, kernel_onishi<real_t, n_t> (kernel_parameters.data(), detail::hall_davis_no_waals_r_max<real_t>()));
          p_kernel = (&(k_onishi[0])).get();
          break;

        default:
          throw std::runtime_error("please supply a type of collision kernel to use"); //TODO: move to pimpl ctor sanity checks?
      }
    }
  }
}
