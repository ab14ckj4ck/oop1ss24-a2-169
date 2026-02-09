///---------------------------------------------------------------------------------------------------------------------
/// Random.hpp
///
/// Authors: Tutors
///---------------------------------------------------------------------------------------------------------------------

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <string>

namespace Oop
{
  ///-------------------------------------------------------------------------------------------------------------------
  /// Random class
  /// Generates random numbers.
  ///
  class Random final
  {
  public:
    ///---------------------------------------------------------------------------------------------------------------
    /// Returns an instance of the Random class (Singleton pattern).
    /// Example: Random::getInstance().getRandomNumber()
    ///
    static Random& getInstance();

    ///---------------------------------------------------------------------------------------------------------------
    /// Deleted copy constructor.
    ///
    Random(Random const&) = delete;

    ///-----------------------------------------------------------------------------------------------------------------
    /// Deleted assignment operator.
    ///
    Random& operator=(Random const&) = delete;

    ///-----------------------------------------------------------------------------------------------------------------
    /// Generates a random number in range in range [1, upper_bound].
    ///
    /// @param upper_bound The upper bound for the number to be generated.
    /// @return A random number in range [1, upper_bound].
    ///
    size_t getRandomNumber(size_t const upper_bound);

  private:
    ///---------------------------------------------------------------------------------------------------------------
    /// Mersenne Twister pseudo-random generators
    ///
    std::mt19937 mersenne_twister_;

    ///---------------------------------------------------------------------------------------------------------------
    /// Names of the environment seeds
    ///
    static std::string const SEED;

    ///---------------------------------------------------------------------------------------------------------------
    /// Private constructor. Access to class possible only through getInstance.
    ///
    Random();

    ///---------------------------------------------------------------------------------------------------------------
    /// Gets the seed from environment.
    /// If the environment variable does not exist it falls back to generating the seed.
    ///
    /// @param environment_variable Name of the environment variable holding the seed.
    /// @returns A number suitable to seed the random number generator.
    ///
    size_t getSeed(std::string const environment_variable = nullptr);

    ///---------------------------------------------------------------------------------------------------------------
    /// Gets the seed from environment.
    ///
    /// @param environment_variable Name of the environment variable holding the seed.
    /// @return A number from the environment_variable suitable to seed the random number generator.
    ///
    size_t getEnvironmentSeed(std::string const environment_variable);

    ///---------------------------------------------------------------------------------------------------------------
    /// Generates a random seed.
    ///
    /// @return A hardware generated number suitable to seed the random number generator.
    ///
    size_t getHardwareSeed();

    ///---------------------------------------------------------------------------------------------------------------
    /// Parses the seed to a numerical value.
    ///
    /// @param environment_seed Seed to parse.
    /// @return The numerical value of the seed parameter.
    ///
    size_t parseSeed(std::string const seed);
  };
} // namespace Oop

#endif
