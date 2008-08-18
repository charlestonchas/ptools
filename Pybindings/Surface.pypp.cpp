// This file has been generated by Py++.

#include "boost/python.hpp"
#include "ptools.h"
#include "Surface.pypp.hpp"

namespace bp = boost::python;

void register_Surface_class(){

    bp::class_< PTools::Surface >( "Surface", bp::init< int, int >(( bp::arg("nphi"), bp::arg("ncosth") )) )    
        .def( 
            "outergrid"
            , &::PTools::Surface::outergrid
            , ( bp::arg("rigid1"), bp::arg("rigid2"), bp::arg("srad") ) )    
        .def( 
            "removeclosest"
            , &::PTools::Surface::removeclosest
            , ( bp::arg("rigid1"), bp::arg("srad") ) )    
        .def( 
            "surfpoint"
            , &::PTools::Surface::surfpoint
            , ( bp::arg("rigid"), bp::arg("srad") ) )    
        .def( 
            "surfpointParams"
            , &::PTools::Surface::surfpointParams
            , ( bp::arg("max"), bp::arg("shift") ) );

}
