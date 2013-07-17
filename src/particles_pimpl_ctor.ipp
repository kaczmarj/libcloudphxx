// vim:filetype=cpp
/** @file
  * @copyright University of Warsaw
  * @section LICENSE
  * GPLv3+ (see the COPYING file or http://www.gnu.org/licenses/)
  * @brief Thrust-based CPU/GPU particle-tracking logic for Lagrangian microphysics
  */

namespace libcloudphxx
{
  namespace lgrngn
  {
    // pimpl stuff
    template <typename real_t, int device>
    struct particles<real_t, device>::impl
    { 
      typedef unsigned long n_t;

      // member fields
      const int n_dims;
      const thrust_size_t n_part; 
      const real_t sd_conc_mean; //, dx, dy, dz;
      detail::u01<real_t, device> rng;

      // particle attributes
      thrust_device::vector<real_t> 
	rd3, // dry radii cubed 
	xi,  // wet radius proxy variable (e.g. xi = ln(rw))
        kpa, // kappa
	x,   // x spatial coordinate (for 2D and 3D)
	y,   // y spatial coordinate (for 3D)
	z;   // z spatial coordinate (for 1D, 2D and 3D)
      thrust_device::vector<n_t>
	n;  // multiplicity

      // helper vectors
      thrust_device::vector<real_t>
	u01; // uniform random numbers between 0 and 1

      // Eulerian-Lagrangian interface vers
      thrust_device::vector<thrust_size_t>
        ijk; // Eulerian grid cell index (always zero for 0D)
      thrust_device::vector<real_t> 
        rhod,    // dry air density
        rhod_th, // energy volume density divided by c_p
        rhod_rv, // water vapour density (=rhod * r_v)
        T, // temperature [K]
        p, // pressure [Pa]
        r; // water vapour mixing ratio [kg/kg]

      // fills u01[0:n] with random numbers
      void urand(thrust_size_t n) { rng.generate_n(u01, n); }

      // compile-time min(1, n) 
      int m1(int n) { return n == 0 ? 1 : n; }

      // ctor 
      impl(const opts_t<real_t> opts) : 
	sd_conc_mean(opts.sd_conc_mean),
	n_dims(opts.nx/m1(opts.nx) + opts.ny/m1(opts.ny) + opts.nz/m1(opts.nz)), // 0, 1, 2 or 3
	n_part(opts.sd_conc_mean * m1(opts.nx) * m1(opts.ny) * m1(opts.nz)) // sd_conc_mean * nx * ny * nz (with ni=min(ni,1))
      {
	u01.resize(n_part);
      }
    };

    // ctor
    template <typename real_t, int device>
    particles<real_t, device>::particles(const opts_t<real_t> opts) :
      pimpl(new impl(opts))
    {}
  };
};