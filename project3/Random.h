#ifndef RANDOM_H
#define RANDOM_H

#include <boost/version.hpp>

#if BOOST_VERSION == 103301

#include <boost/random/uniform_real.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

typedef boost::uniform_real<> real_distribution_type;
typedef boost::uniform_int<> int_distribution_type;
typedef boost::mt19937 mt_gen;
typedef boost::variate_generator variate_generator;

#else

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>

typedef boost::random::uniform_real_distribution<> real_distribution_type;
typedef boost::random::uniform_int_distribution<> int_distribution_type;
typedef boost::random::mt19937 mt_gen;
typedef boost::random::variate_generator<mt_gen, real_distribution_type> real_variate_generator;

#endif

#endif /*RANDOM_H*/
