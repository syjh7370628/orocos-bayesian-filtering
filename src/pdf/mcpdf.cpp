// $Id$
// Copyright (C) 2003 Klaas Gadeyne <first dot last at gmail dot com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

// This file only contains template specialisation code
#include "mcpdf.h"

namespace BFL
{
  using namespace MatrixWrapper;


  // Template Specialisation for T =ColumnVector
  template <> inline
  ColumnVector MCPdf<ColumnVector>::ExpectedValueGet (  ) const
  {
    _CumSum = 0.0;
    _los = _listOfSamples;
    for ( _it_los = _los.begin() ; _it_los != _los.end() ; _it_los++ )
         _CumSum += ( _it_los->ValueGet() * _it_los->WeightGet() );
    return _CumSum/_SumWeights;
  }


  template <> inline
  SymmetricMatrix MCPdf<ColumnVector>::CovarianceGet (  ) const
  {
    _mean = (this->ExpectedValueGet());
    _los = _listOfSamples;
    _diffsum = 0.0;
    // Actual calculation
    for (_it_los = _los.begin(); _it_los != _los.end(); _it_los++)
      {
	_diff = ( _it_los->ValueGet() - _mean);
	_diffsum += _diff * (_diff.transpose() * _it_los->WeightGet());
      }
    // Biased estimator!! (unbiased possible with weighted samples??)
    (_diffsum/_SumWeights).convertToSymmetricMatrix(_covariance);
    return _covariance;
  }






  // Template Specialisation for T =unsigned int
  template <> inline
  unsigned int MCPdf<unsigned int>::ExpectedValueGet (  ) const
  {
    unsigned int result;
    double CumSum = 0;
    _los = _listOfSamples;
    double current_weight;
    for ( _it_los = _los.begin() ; _it_los != _los.end() ; _it_los++ )
      {
	current_weight = _it_los->WeightGet();
	CumSum += ( ((double)_it_los->ValueGet()) * current_weight );
      }
    result = (unsigned int)((CumSum/_SumWeights) + 0.5);
    return result;
  }




  template <> inline
  SymmetricMatrix MCPdf<unsigned int>::CovarianceGet (  ) const
  {
    unsigned int mean = this->ExpectedValueGet();
    unsigned int diff;
    double diffsum, current_weight;
    _los = _listOfSamples;
    diffsum = 0.0;
    // Actual calculation
    for (_it_los = _los.begin(); _it_los != _los.end(); _it_los++)
      {
	current_weight = _it_los->WeightGet();
	diff = (_it_los->ValueGet() - mean);
	diffsum += (((double)(diff * diff)) * current_weight);
      }

    // Biased estimator!! (unbiased possible with weighted samples??)
    _covariance(1,1) = (diffsum / _SumWeights);
    return _covariance;
  }

}
