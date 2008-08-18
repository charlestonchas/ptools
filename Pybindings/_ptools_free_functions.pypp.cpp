// This file has been generated by Py++.

#include "boost/python.hpp"
#include "ptools.h"
#include "_ptools_free_functions.pypp.hpp"

namespace bp = boost::python;

void register_free_functions(){

    { //::PTools::Angle
    
        typedef ::dbl ( *Angle_function_type )( ::PTools::Coord3D const &,::PTools::Coord3D const & );
        
        bp::def( 
            "Angle"
            , Angle_function_type( &::PTools::Angle )
            , ( bp::arg("vector1"), bp::arg("vector2") ) );
    
    }

    { //::PTools::Dihedral
    
        typedef ::dbl ( *Dihedral_function_type )( ::PTools::Coord3D const &,::PTools::Coord3D const &,::PTools::Coord3D const &,::PTools::Coord3D const & );
        
        bp::def( 
            "Dihedral"
            , Dihedral_function_type( &::PTools::Dihedral )
            , ( bp::arg("a"), bp::arg("b"), bp::arg("c"), bp::arg("d") ) );
    
    }

    { //::PTools::Dist
    
        typedef ::dbl ( *Dist_function_type )( ::PTools::Atom const &,::PTools::Atom const & );
        
        bp::def( 
            "Dist"
            , Dist_function_type( &::PTools::Dist )
            , ( bp::arg("at1"), bp::arg("at2") ) );
    
    }

    { //::PTools::Dist2
    
        typedef ::dbl ( *Dist2_function_type )( ::PTools::Atom const &,::PTools::Atom const & );
        
        bp::def( 
            "Dist2"
            , Dist2_function_type( &::PTools::Dist2 )
            , ( bp::arg("at1"), bp::arg("at2") ) );
    
    }

    { //::PTools::Norm
    
        typedef ::dbl ( *Norm_function_type )( ::PTools::Coord3D const & );
        
        bp::def( 
            "Norm"
            , Norm_function_type( &::PTools::Norm )
            , ( bp::arg("A") ) );
    
    }

    { //::PTools::Norm2
    
        typedef ::dbl ( *Norm2_function_type )( ::PTools::Coord3D const & );
        
        bp::def( 
            "Norm2"
            , Norm2_function_type( &::PTools::Norm2 )
            , ( bp::arg("A") ) );
    
    }

    { //::PTools::PrintCoord
    
        typedef ::std::string ( *PrintCoord_function_type )( ::PTools::Coord3D const & );
        
        bp::def( 
            "PrintCoord"
            , PrintCoord_function_type( &::PTools::PrintCoord )
            , ( bp::arg("A") ) );
    
    }

    { //::PTools::Rmsd
    
        typedef ::dbl ( *Rmsd_function_type )( ::PTools::AtomSelection const &,::PTools::AtomSelection const & );
        
        bp::def( 
            "Rmsd"
            , Rmsd_function_type( &::PTools::Rmsd )
            , ( bp::arg("atsel1"), bp::arg("atsel2") ) );
    
    }

    { //::PTools::WritePDB
    
        typedef void ( *WritePDB_function_type )( ::PTools::Rigidbody const &,::std::string );
        
        bp::def( 
            "WritePDB"
            , WritePDB_function_type( &::PTools::WritePDB )
            , ( bp::arg("rigid"), bp::arg("filename") ) );
    
    }

    { //::PTools::superimpose
    
        typedef ::PTools::Superimpose_t ( *superimpose_function_type )( ::PTools::AtomSelection,::PTools::AtomSelection,int );
        
        bp::def( 
            "superimpose"
            , superimpose_function_type( &::PTools::superimpose )
            , ( bp::arg("selref"), bp::arg("selmob"), bp::arg("verbosity")=(int)(0) ) );
    
    }

}
