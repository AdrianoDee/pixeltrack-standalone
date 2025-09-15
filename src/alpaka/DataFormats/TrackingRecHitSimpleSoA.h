#ifndef DataFormats_TrackingRecHitSimpleSoA_h
#define DataFormats_TrackingRecHitSimpleSoA_h

#include <cstdint>
#include <cstdlib>
#include <vector>

class TrackingRecHitSimpleSoA {
public:
  TrackingRecHitSimpleSoA() = default;
  explicit TrackingRecHitSimpleSoA(unsigned int nHits)
  {
    m_xl.resize(nHits);
    m_yl.resize(nHits);
    m_xerr.resize(nHits);
    m_yerr.resize(nHits);

    m_xg.resize(nHits);
    m_yg.resize(nHits);
    m_zg.resize(nHits);
    m_rg.resize(nHits);
    m_iphi.resize(nHits);

    m_charge.resize(nHits);
    m_xsize.resize(nHits);
    m_ysize.resize(nHits);
    m_detInd.resize(nHits);
  }

  explicit TrackingRecHitSimpleSoA(
      size_t nHits,
      const float* xl, const float* yl, const float* xerr, const float* yerr,
      const float* xg, const float* yg, const float* zg, const float* rg, const int16_t* iphi,
      const int32_t* charge, const int16_t* xsize, const int16_t* ysize, const int16_t* detInd)
      : m_xl(xl, xl + nHits),
        m_yl(yl, yl + nHits),
        m_xerr(xerr, xerr + nHits),
        m_yerr(yerr, yerr + nHits),
        m_xg(xg, xg + nHits),
        m_yg(yg, yg + nHits),
        m_zg(zg, zg + nHits),
        m_rg(rg, rg + nHits),
        m_iphi(iphi, iphi + nHits),
        m_charge(charge, charge + nHits),
        m_xsize(xsize, xsize + nHits),
        m_ysize(ysize, ysize + nHits),
        m_detInd(detInd, detInd + nHits) 
  {
    assert(m_xl.size() == nHits);
  }
  ~TrackingRecHitSimpleSoA() = default;

  auto nHits() const { return m_xl.size(); }

  float xl(size_t i) const { return m_xl[i]; }
  float yl(size_t i) const { return m_yl[i]; }
  float xerr(size_t i) const { return m_xerr[i]; }
  float yerr(size_t i) const { return m_yerr[i]; }

  float xg(size_t i) const { return m_xg[i]; }
  float yg(size_t i) const { return m_yg[i]; }
  float zg(size_t i) const { return m_zg[i]; }
  float rg(size_t i) const { return m_rg[i]; }
  int16_t iphi(size_t i) const { return m_iphi[i]; }

  int32_t charge(size_t i) const { return m_charge[i]; }
  int16_t xsize(size_t i) const { return m_xsize[i]; }
  int16_t ysize(size_t i) const { return m_ysize[i]; }
  int16_t detInd(size_t i) const { return m_detInd[i]; }

  std::vector<float>& xlVector() { return m_xl; }
  const std::vector<float>& xlVector() const { return m_xl; }

  std::vector<float>& ylVector() { return m_yl; }
  const std::vector<float>& ylVector() const { return m_yl; }

  std::vector<float>& xerrVector() { return m_xerr; }
  const std::vector<float>& xerrVector() const { return m_xerr; }

  std::vector<float>& yerrVector() { return m_yerr; }
  const std::vector<float>& yerrVector() const { return m_yerr; }

  std::vector<float>& xgVector() { return m_xg; }
  const std::vector<float>& xgVector() const { return m_xg; }

  std::vector<float>& ygVector() { return m_yg; }
  const std::vector<float>& ygVector() const { return m_yg; }

  std::vector<float>& zgVector() { return m_zg; }
  const std::vector<float>& zgVector() const { return m_zg; }

  std::vector<float>& rgVector() { return m_rg; }
  const std::vector<float>& rgVector() const { return m_rg; }

  std::vector<int16_t>& iphiVector() { return m_iphi; }
  const std::vector<int16_t>& iphiVector() const { return m_iphi; }

  std::vector<int32_t>& chargeVector() { return m_charge; }
  const std::vector<int32_t>& chargeVector() const { return m_charge; }

  std::vector<int16_t>& xsizeVector() { return m_xsize; }
  const std::vector<int16_t>& xsizeVector() const { return m_xsize; }

  std::vector<int16_t>& ysizeVector() { return m_ysize; }
  const std::vector<int16_t>& ysizeVector() const { return m_ysize; }

  std::vector<int16_t>& detIndVector() { return m_detInd; }
  const std::vector<int16_t>& detIndVector() const { return m_detInd; }

  const std::vector<std::vector<uint32_t>>& moduleStartVec() const { return m_moduleStart; }

  void readBinary(std::istream& is) {

    auto nHits = this->nHits();
    auto readVector = [&is, nHits](auto& vec) {
      if (nHits > 0) {
        is.read(reinterpret_cast<char*>(vec.data()),
                vec.size() * sizeof(typename std::decay<decltype(vec)>::type::value_type));
      }
    };

    readVector(m_xl);
    readVector(m_yl);
    readVector(m_xerr);
    readVector(m_yerr);

    readVector(m_xg);
    readVector(m_yg);
    readVector(m_zg);
    readVector(m_rg);
    readVector(m_iphi);

    readVector(m_charge);
    readVector(m_xsize);
    readVector(m_ysize);
    readVector(m_detInd);
  }

private:

    // local coord
  std::vector<float> m_xl;
  std::vector<float> m_yl;
  std::vector<float> m_xerr;
  std::vector<float> m_yerr;

  // global coord
  std::vector<float> m_xg;
  std::vector<float> m_yg;
  std::vector<float> m_zg;
  std::vector<float> m_rg;
  std::vector<int16_t> m_iphi;

  // cluster properties
  std::vector<int32_t> m_charge;
  std::vector<int16_t> m_xsize;
  std::vector<int16_t> m_ysize;
  std::vector<int16_t> m_detInd;

  std::vector<std::vector<uint32_t>> m_moduleStart;


};

#endif  // DataFormats_TrackingRecHitSimpleSoA_h
