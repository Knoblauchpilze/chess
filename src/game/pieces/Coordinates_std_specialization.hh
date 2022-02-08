#ifndef    COORDINATES_STD_SPECIALIZATION_HH
# define   COORDINATES_STD_SPECIALIZATION_HH

# include "Coordinates.hh"

namespace std {

  /// @brief - Specialization of the hash for coordinates.
  template<>
  struct hash<chess::Coordinates> {
    inline
    std::size_t
    operator()(const chess::Coordinates& c) const noexcept {
      // As per this link, we can use this combining function
      // to hash the coordinates:
      // https://stackoverflow.com/questions/5889238/why-is-xor-the-default-way-to-combine-hashes
      std::size_t lhs = c.x();
      std::size_t rhs = c.y();

      lhs ^= rhs + 0x9e3779b97f4a7c16 + (lhs << 6u) + (lhs >> 2u);
      return lhs;
    }
  };

}

#endif    /* COORDINATES_STD_SPECIALIZATION_HH */
