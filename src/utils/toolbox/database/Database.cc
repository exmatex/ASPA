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
// File:        Database.cc
// Package:     MPTCOUPLER toolbox
// 
// 
// 
// Description: An abstract base class for managing database objects in files
//

#include "toolbox/database/Database.h"

#ifdef DEBUG_NO_INLINE
#include "toolbox/database/Database.I"
#endif

namespace MPTCOUPLER {
   namespace toolbox {

Database::~Database()
{
}

/*  
 * Scalar/array functions for boolean type.
 */ 

void Database::getScalar(const string& key, bool& scalar)
{
   scalar = getBool(key);
}

void Database::putScalar(const string& key, bool scalar)
{
   putBool(key, scalar);
}

void Database::getArray(const string& key, vector<bool>& array)
{
   getBoolArray(key, array);
}

void Database::putArray(const string& key, const vector<bool>& array)
{
   putBoolArray(key, array);
}

/*  
 * Scalar/array functions for char type.
 */ 

void Database::getScalar(const string& key, char& scalar)
{
   scalar = getChar(key);
}

void Database::putScalar(const string& key, char scalar)
{
   putChar(key, scalar);
}

void Database::getArray(const string& key, vector<char>& array)
{
   getCharArray(key, array);
}

void Database::putArray(const string& key, const vector<char>& array)
{
   putCharArray(key, array);
}

/*  
 * Scalar/array functions for float type.
 */ 

void Database::getScalar(const string& key, float& scalar)
{
   scalar = getFloat(key);
}

void Database::putScalar(const string& key, float scalar)
{
   putFloat(key, scalar);
}

void Database::getArray(const string& key, vector<float>& array)
{
   getFloatArray(key, array);
}

void Database::putArray(const string& key, const vector<float>& array)
{
   putFloatArray(key, array);
}

/*  
 * Scalar/array functions for double type.
 */ 

void Database::getScalar(const string& key, double& scalar)
{
   scalar = getDouble(key);
}

void Database::putScalar(const string& key, double scalar)
{
   putDouble(key, scalar);
}

void Database::getArray(const string& key, vector<double>& array)
{
   getDoubleArray(key, array);
}

void Database::putArray(const string& key, const vector<double>& array)
{
   putDoubleArray(key, array);
}

/*  
 * Scalar/array functions for integer type.
 */ 

void Database::getScalar(const string& key, int& scalar)
{
   scalar = getInteger(key);
}

void Database::putScalar(const string& key, int scalar)
{
   putInteger(key, scalar);
}

void Database::getArray(const string& key, vector<int>& array)
{
   getIntegerArray(key, array);
}

void Database::putArray(const string& key, const vector<int>& array)
{
   putIntegerArray(key, array);
}

/* 
 * Scalar/array functions for string type.
 */
 
void Database::getScalar(const string& key, string& scalar)
{
   scalar = getString(key);
}
 
void Database::putScalar(const string& key, const string& scalar) 
{
   putString(key, scalar);
}
 
void Database::getArray(const string& key, vector<string>& array)
{
   getStringArray(key, array); 
}
 
void Database::putArray(const string& key, const vector<string>& array)
{
   putStringArray(key, array);
}


}
}




