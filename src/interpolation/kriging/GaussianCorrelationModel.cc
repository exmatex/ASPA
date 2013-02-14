// DO-NOT-DELETE revisionify.begin() 
/*
Copyright (c) 2007-2008 Lawrence Livermore National Security LLC

This file is part of the mdef package (version 0.1) and is free software: 
you can redistribute it and/or modify it under the terms of the GNU
Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program.  If not, see
<http://www.gnu.org/licenses/>.

                              DISCLAIMER

This work was prepared as an account of work sponsored by an agency of
the United States Government. Neither the United States Government nor
Lawrence Livermore National Security, LLC nor any of their employees,
makes any warranty, express or implied, or assumes any liability or
responsibility for the accuracy, completeness, or usefulness of any
information, apparatus, product, or process disclosed, or represents
that its use would not infringe privately-owned rights. Reference
herein to any specific commercial products, process, or service by
trade name, trademark, manufacturer or otherwise does not necessarily
constitute or imply its endorsement, recommendation, or favoring by
the United States Government or Lawrence Livermore National Security,
LLC. The views and opinions of authors expressed herein do not
necessarily state or reflect those of the United States Government or
Lawrence Livermore National Security, LLC, and shall not be used for
advertising or product endorsement purposes.
*/
// DO-NOT-DELETE revisionify.end() 
//
// File:        GaussianCorrelationModel.cc
// Package:     MPTCOUPLER kriging algorithm
// 
// 
// 
// Description: Class implementating gaussian correlation model.
//
// $Id: GaussianCorrelationModel.cc,v 1.1 2005/08/23 21:12:40 knap2 Exp $
//
// $Log: GaussianCorrelationModel.cc,v $
// Revision 1.1  2005/08/23 21:12:40  knap2
// Initial source.
//
//

#include "GaussianCorrelationModel.h"

#include <cassert>
#include <cmath>

#include <mtl/mtl.h>

//
//
//

namespace MPTCOUPLER {
    namespace krigalg {

  //
  // construction/destruction
  //
  
  GaussianCorrelationModel::GaussianCorrelationModel(const std::vector<double> & thetas)
    : CorrelationModel(thetas)
  {

    //
    // the current implementation envisions having a single value of theta
    //

    assert(_thetas.size() == 1);

    return;

  }

  GaussianCorrelationModel::~GaussianCorrelationModel()
  {

    return;

  }

  //
  // get value of the correlation function between two points
  //

  Matrix 
  GaussianCorrelationModel::getValue(const Point & firstPoint,
				     const Point & secondPoint) const
  {

    //
    // compute L2-norm of the firstPoint-secondPoint distance
    //

    const double distance = mtl::two_norm(firstPoint - secondPoint);
  
    //
    // prepare Matrix
    //

    Matrix matrix = identity(1);
    matrix[0][0] = exp(-_thetas.front()*distance*distance);

    //
    // return the value
    //

    return matrix;
    
  }

  //
  // get a string representation of the class name
  //

  std::string
  GaussianCorrelationModel::getClassName() const
  {

    return std::string("MPTCOUPLER::krigalg::GaussianCorrelationModel");

  }

  //
  // Database output
  //
  
  void
  GaussianCorrelationModel::putToDatabase(toolbox::Database & db) const
  {

    //
    // store base-class data
    //
    
    CorrelationModel::putToDatabase(db);

    //
    //
    //

    return;

  }

  //
  // Database input
  //

  void
  GaussianCorrelationModel::getFromDatabase(toolbox::Database & db)
  {

    //
    // get base-class data
    //

    CorrelationModel::getFromDatabase(db);

    //
    //
    //
    
    return;
    
  }

}
}


