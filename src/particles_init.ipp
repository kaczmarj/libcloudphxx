// vim:filetype=cpp
/** @file
  * @copyright University of Warsaw
  * @section LICENSE
  * GPLv3+ (see the COPYING file or http://www.gnu.org/licenses/)
  * @brief initialisation routine for super droplets
  */

namespace libcloudphxx
{
  namespace lgrngn
  {
    // init
    template <typename real_t, int device>
    void particles<real_t, device>::init(
      unary_function<real_t> *n_of_lnrd 
    )
    {
      init_dry(n_of_lnrd);
      init_xyz();
      init_Tpr();
      init_wet();
    }
  };
};