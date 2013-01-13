/**
 * @file bob/ap/Ceps.h
 * @date Wed Jan 11:10:20 2013 +0200
 * @author Elie Khoury <Elie.Khoury@idiap.ch>
 *
 * @brief Implement Linear and Mel Frequency Cepstral Coefficients
 * functions (MFCC and LFCC)
 *
 * Copyright (C) 2011-2013 Idiap Research Institute, Martigny, Switzerland
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


#ifndef BOB_AP_CEPS_H
#define BOB_AP_CEPS_H

#include <blitz/array.h>
#include <vector>
#include "bob/sp/FFT1D.h"

const double ENERGY_FLOOR = 1.0;
const double FBANK_OUT_FLOOR = 1.0;

namespace bob {
/**
 * \ingroup libap_api
 * @{
 *
 */
namespace ap {

/**
 * @brief This class is used to test the Ceps class (private methods)
 */
class CepsTest;

/**
 * @brief This class allows the extraction of features from raw audio data.
 * References:
 *  1. SPro tools (http://www.irisa.fr/metiss/guig/spro/spro-4.0.1/spro.html)
 *  2. Wikipedia (http://en.wikipedia.org/wiki/Mel-frequency_cepstrum).
 */
class Ceps
{
  public:
    /**
     * @brief Constructor: Initialize working arrays
     */
    Ceps(double sf, int win_length_ms, int win_shift_ms, size_t n_filters, 
      size_t n_ceps, double f_min, double f_max, double delta_win, 
      double pre_emphasis_coef);

    /**
     * @brief Get the Cepstral Shape
     */
    blitz::TinyVector<int,2> getCepsShape(const size_t input_length) const;
    blitz::TinyVector<int,2> getCepsShape(const blitz::Array<double,1>& input) const;

    /**
     * @brief Compute Cepstral features
     */
    void CepsAnalysis(const blitz::Array<double,1>& input, blitz::Array<double,2>& output);

    /**
     * @brief Destructor
     */
    virtual ~Ceps();

    /**
     * @brief Returns the sampling frequency/frequency rate
     */
    inline double getSamplingFrequency() const
    { return m_sf; }
    /**
     * @brief Returns the window length in miliseconds
     */
    inline int getWinLengthMs() const
    { return m_win_length_ms; }
    /**
     * @brief Returns the window length in number of samples
     */
    inline int getWinLength() const
    { return m_win_length; }
    /**
     * @brief Returns the window shift in miliseconds
     */
    inline int getWinShiftMs() const
    { return m_win_shift_ms; }
    /**
     * @brief Returns the window shift in number of samples
     */
    inline int getWinShift() const
    { return m_win_shift; }
    /**
     * @brief Returns the window size in number of samples. This is equal to
     * the next power of 2 integer larger or equal to the window length.
     */
    inline int getWinSize() const
    { return m_win_size; }
    /**
     * @brief Returns the number of filters used in the filter bank.
     */
    inline size_t getNFilters() const
    { return m_n_filters; }
    /**
     * @brief Returns the number of cepstral coefficient to keep
     */
    inline size_t getNCeps() const
    { return m_n_ceps; }
    /**
     * @brief Returns the frequency of the lowest triangular filter in the
     * filter bank
     */
    inline double getFMin() const
    { return m_f_min; }
    /**
     * @brief Returns the frequency of the highest triangular filter in the
     * filter bank
     */
    inline double getFMax() const
    { return m_f_max; }
    /**
     * @brief Tells whether the frequencies of the filters in the filter bank
     * are taken from the linear or the Mel scale
     */
    inline bool getFbLinear() const
    { return m_fb_linear; }
    /**
     * @brief Rerturns the size of the window used to compute first and second
     * order derivatives
     */
    inline size_t getDeltaWin() const
    { return m_delta_win; }
    /**
     * @brief Returns the pre-emphasis coefficient.
     */
    inline double getPreEmphasisCoeff() const
    { return m_pre_emphasis_coeff; }
    /**
     * @brief Tells whether the DCT coefficients are normalized or not
     */
    inline bool getDctNorm() const
    { return m_dct_norm; }
    /**
     * @brief Tells whether the energy is added to the cepstral coefficients 
     * or not
     */
    inline bool getWithEnergy() const
    { return m_with_energy; }
    /**
     * @brief Tells whether the first order derivatives are added to the 
     * cepstral coefficients or not
     */
    inline bool getWithDelta() const
    { return m_with_delta; }
    /**
     * @brief Tells whether the second order derivatives are added to the 
     * cepstral coefficients or not
     */
    inline bool getWithDeltaDelta() const
    { return m_with_delta_delta; }

    /**
     * @brief Sets the sampling frequency/frequency rate
     */
    void setSamplingFrequency(const double sf);
    /**
     * @brief Sets the window length in miliseconds
     */
    void setWinLengthMs(int win_length_ms);
    /**
     * @brief Sets the window shift in miliseconds
     */
    void setWinShiftMs(int win_shift_ms);
    /**
     * @brief Sets the number of filters used in the filter bank.
     */
    void setNFilters(size_t n_filters);
    /**
     * @brief Returns the number of cepstral coefficient to keep
     */
    void setNCeps(size_t n_ceps);
    /**
     * @brief Sets the size of the window used to compute first and second
     * order derivatives
     */
    inline void setDeltaWin(size_t delta_win)
    { m_delta_win = (int)delta_win; } 
    /**
     * @brief Sets the pre-emphasis coefficient. It should be a value in the 
     * range [0,1].
     */
    inline void setPreEmphasisCoeff(double pre_emphasis_coeff)
    { // TODO: check paramater value is in range [0,1]
      m_pre_emphasis_coeff = pre_emphasis_coeff; }
    /**
     * @brief Returns the frequency of the lowest triangular filter in the
     * filter bank
     */
    void setFMin(double f_min);
    /**
     * @brief Returns the frequency of the highest triangular filter in the
     * filter bank
     */
    void setFMax(double f_max);
    /**
     * @brief Sets whether the frequencies of the filters in the filter bank
     * are taken from the linear or the Mel scale
     */
    void setFbLinear(bool fb_linear);
    /**
     * @brief Sets whether the DCT coefficients are normalized or not
     */
    inline void setDctNorm(bool dct_norm)
    { m_dct_norm = dct_norm; }
    /**
     * @brief Sets whether the energy is added to the cepstral coefficients 
     * or not
     */
    inline void setWithEnergy(bool with_energy)
    { m_with_energy = with_energy; }
    /**
     * @brief Sets whether the first order derivatives are added to the 
     * cepstral coefficients or not
     */
    inline void setWithDelta(bool with_delta)
    { m_with_delta = with_delta; }
    /**
     * @brief Sets whether the first order derivatives are added to the 
     * cepstral coefficients or not. If enabled, first order derivatives are
     * automatically enabled as well.
     */
    inline void setWithDeltaDelta(bool with_delta_delta)
    { if(with_delta_delta) m_with_delta = true;
      m_with_delta_delta = with_delta_delta; }

  private:
    /**
     * @brief Compute the first order derivative from the given input
     */
    void addDerivative(const blitz::Array<double,2>& input, blitz::Array<double,2>& output);

    /**
     * @brief Mean Normalisation of the features
     */
    blitz::Array<double,2> dataZeroMean(blitz::Array<double,2>& frames, bool norm_energy, int n_frames, int frame_size);

    static double mel(double f);
    static double melInv(double f);
    void pre_emphasis(blitz::Array<double,1> &data);
    void hammingWindow(blitz::Array<double,1> &data);
    void logFilterBank(blitz::Array<double,1>& x);
    void logTriangularFBank(blitz::Array<double,1>& data);
    double logEnergy(blitz::Array<double,1> &data);
    void transformDCT(blitz::Array<double,1>& ceps_row);
    void initWinSize();
    void initWinLength();
    void initWinShift();
    void initCacheHammingKernel();
    void initCacheDctKernel();
    void initCacheFilterBank();
    void initCachePIndex();
    void initCacheFilters();

    double m_sf;
    int m_win_length_ms;
    int m_win_length;
    int m_win_shift_ms;
    int m_win_shift;
    int m_win_size;
    size_t m_n_filters;
    size_t m_n_ceps;
    double m_f_min;
    double m_f_max;
    int m_delta_win;
    double m_pre_emphasis_coeff;
    bool m_fb_linear;
    bool m_dct_norm;
    bool m_with_energy;
    bool m_with_delta;
    bool m_with_delta_delta;
    bool m_with_delta_energy;
    bool m_with_delta_delta_energy;
    blitz::Array<double,2> m_dct_kernel;
    blitz::Array<double,1> m_hamming_kernel;
    blitz::Array<double,1> m_filters;
    blitz::Array<int,1>  m_p_index;
    std::vector<blitz::Array<double,1> > m_filter_bank;
    bob::sp::FFT1D m_fft;

    mutable blitz::Array<double,1> m_cache_frame;
    mutable blitz::Array<std::complex<double>,1>  m_cache_complex1;
    mutable blitz::Array<std::complex<double>,1>  m_cache_complex2;

    friend class TestCeps;
};

class TestCeps
{
  public:
    TestCeps(Ceps& ceps);
    Ceps& m_ceps;

    // Methods to test
    double mel(double f) { return m_ceps.mel(f); }
    double melInv(double f) { return m_ceps.melInv(f); }
    blitz::TinyVector<int,2> getCepsShape(const size_t input_length) const
    { return m_ceps.getCepsShape(input_length); }
    blitz::TinyVector<int,2> getCepsShape(const blitz::Array<double,1>& input) const
    { return m_ceps.getCepsShape(input); }
    blitz::Array<double,1> getFilter(void) { return m_ceps.m_filters; }

    void CepsAnalysis(const blitz::Array<double,1>& input, blitz::Array<double,2>& ceps_2D)
    { m_ceps.CepsAnalysis(input, ceps_2D);}
    void hammingWindow(blitz::Array<double,1>& data){ m_ceps.hammingWindow(data); }
    void pre_emphasis(blitz::Array<double,1>& data){ m_ceps.pre_emphasis(data); }
    void logFilterBank(blitz::Array<double,1>& x){ m_ceps.logFilterBank(x); }
    void logTriangularFBank(blitz::Array<double,1>& data){ m_ceps.logTriangularFBank(data); }
    double logEnergy(blitz::Array<double,1> &data){ return m_ceps.logEnergy(data); }
    void transformDCT(blitz::Array<double,1>& ceps_row) { m_ceps.transformDCT(ceps_row); }
    blitz::Array<double,2> dataZeroMean(blitz::Array<double,2>& frames, bool norm_energy, int n_frames, int frame_size)
    { return m_ceps.dataZeroMean(frames,norm_energy, n_frames, frame_size); }
};

}
}

#endif /* BOB_AP_CEPS_H */