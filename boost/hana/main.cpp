// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/tuple.hpp>
#include <string>
namespace hana = boost::hana;
using namespace hana::literals;



template<uint64_t I>
struct intc { 
    enum e{ value = I }; 
    operator uint64_t()const{ return I; }  
};

int main() {


    auto tu = hana::make_tuple( intc<0>(), intc<1>(), intc<2>(), intc<3>(), intc<4>(), intc<5>(),
		                                   intc<6>(), intc<7>(), intc<8>(), intc<9>(), intc<10>(), intc<11>(),
		                                   intc<12>(), intc<13>(), intc<14>(), intc<15>() );
                                           
                                           
                                           
    hana::transform(tu,[](auto a){
        std::cout << a << std::endl;
        return a;
    });                                       
    return 0;
}