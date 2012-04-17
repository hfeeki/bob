/**
 * @file cxx/ip/ip/DCTFeatures.h
 * @date Thu Apr 7 19:52:29 2011 +0200
 * @author Laurent El Shafey <Laurent.El-Shafey@idiap.ch>
 *
 * @brief This file provides a class to extract DCT features as described in:
 *   "Polynomial Features for Robust Face Authentication",
 *   from C. Sanderson and K. Paliwal, in the proceedings of the
 *   IEEE International Conference on Image Processing 2002.
 *
 * Copyright (C) 2011-2012 Idiap Research Institute, Martigny, Switzerland
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOB_IP_DCT_FEATURES_H
#define BOB_IP_DCT_FEATURES_H

#include "core/cast.h"
#include "core/array_copy.h"
#include "ip/Exception.h"

#include <list>

#include "ip/block.h"
#include "sp/DCT2D.h"
#include "ip/zigzag.h"


namespace bob {
/**
 * \ingroup libip_api
 * @{
 *
 */
  namespace ip {

  /**
   * @brief This class can be used to extract DCT features. This algorithm 
   *   is described in the following article:
   *   "Polynomial Features for Robust Face Authentication", 
   *   from C. Sanderson and K. Paliwal, in the proceedings of the 
   *   IEEE International Conference on Image Processing 2002.
  */
	class DCTFeatures
	{
  	public:

	  	/**
        * @brief Constructor: generates the Difference of Gaussians filter
        */
	    DCTFeatures( const int block_h, const int block_w, const int overlap_h, 
        const int overlap_w, const int n_dct_coefs): m_dct2d(0),
          m_block_h(block_h), m_block_w(block_w), m_overlap_h(overlap_h), 
          m_overlap_w(overlap_w), m_n_dct_coefs(n_dct_coefs)
      {
        m_dct2d = new bob::sp::DCT2D(block_h, block_w);
      }

	  	/**
        * @brief Destructor
        */
	    virtual ~DCTFeatures() {
        if( m_dct2d!=0)
          delete m_dct2d;
      }

	  	/**
        * @brief Process a 2D blitz Array/Image by extracting DCT features.
        * @param src The 2D input blitz array
        * @param dst A container (with a push_back method such as an STL list)
        *   of 1D double blitz arrays.
        */
	    template <typename T, typename U> 
      void operator()(const blitz::Array<T,2>& src, U& dst);

      /**
       * @brief Process a list of blocks by extracting DCT features.
       * @param src 3D input blitz array (list of 2D blocks)
       * @param dst 2D output blitz array
       */
      template <typename T>
      void operator()(const blitz::Array<T,3>& src, blitz::Array<double, 2>& dst) const;
      
      /**
        * @brief Function which returns the number of blocks when applying 
        *   the DCTFeatures extractor on a 2D blitz::array/image.
        *   The first dimension is the height (y-axis), whereas the second
        *   one is the width (x-axis).
        * @param src The input blitz array
        */
      template<typename T>
      const int getNBlocks(const blitz::Array<T,2>& src);

	  private:
      /**
        * Attributes
        */
      bob::sp::DCT2D *m_dct2d;
      int m_block_h;
      int m_block_w;
      int m_overlap_h;
      int m_overlap_w;
      int m_n_dct_coefs;
	};

  template <typename T, typename U> 
  void DCTFeatures::operator()(const blitz::Array<T,2>& src, 
    U& dst) 
  { 
    // cast to double
    blitz::Array<double,2> double_version = bob::core::cast<double>(src);

    // get all the blocks
    std::list<blitz::Array<double,2> > blocks;
    blockReference(double_version, blocks, m_block_h, m_block_w, m_overlap_h, 
      m_overlap_w);
  
    /// dct extract each block
    for( std::list<blitz::Array<double,2> >::const_iterator it = blocks.begin(); 
      it != blocks.end(); ++it) 
    {
      // extract dct using operator()
      blitz::Array<double,2> dct_tmp_block(m_block_h, m_block_w);
      // TODO: avoid the copy if possible
      m_dct2d->operator()(bob::core::array::ccopy(*it), dct_tmp_block);

      // extract the required number of coefficients using the zigzag pattern
      blitz::Array<double,1> dct_block_zigzag(m_n_dct_coefs);
      zigzag(dct_tmp_block, dct_block_zigzag, m_n_dct_coefs);
      
      // Push the resulting processed block in the container
      dst.push_back(dct_block_zigzag);
    }
  }

  template <typename T> 
  void DCTFeatures::operator()(const blitz::Array<T,3>& src, blitz::Array<double, 2>& dst) const
  { 
    // Cast to double
    blitz::Array<double,3> double_version = bob::core::cast<double>(src);

    bob::core::array::assertSameShape(src, blitz::TinyVector<int, 3>(src.extent(0), m_block_h, m_block_w));
    dst.resize(src.extent(0), m_n_dct_coefs);
    
    // Dct extract each block
    for(int i = 0; i < double_version.extent(0); i++)
    {
      // Get the current block
      blitz::Array<double,2> dct_input = double_version(i, blitz::Range::all(), blitz::Range::all());

      // Extract dct using operator()
      blitz::Array<double,2> dct_tmp_block(m_block_h, m_block_w);
      m_dct2d->operator()(dct_input, dct_tmp_block);

      // Extract the required number of coefficients using the zigzag pattern
      blitz::Array<double,1> dct_block_zigzag(m_n_dct_coefs);
      zigzag(dct_tmp_block, dct_block_zigzag, m_n_dct_coefs);
      
      // Push the resulting processed block in the right dst row
      blitz::Array<double, 1> dst_row = dst(i, blitz::Range::all());
      dst_row = dct_block_zigzag;
    }
  }
  
  template<typename T>
  const int DCTFeatures::getNBlocks(const blitz::Array<T,2>& src)
  {
    const blitz::TinyVector<int,3> res = getBlock3DOutputShape(src, m_block_h, 
      m_block_w, m_overlap_h, m_overlap_w); 
    return res(0);
  }

}}

#endif /* BOB_IP_DCT_FEATURES_H */
