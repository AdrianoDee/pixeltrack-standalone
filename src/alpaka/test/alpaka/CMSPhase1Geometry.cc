#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>

#include "DataFormats/SOARotation.h"
#include "CondFormats/CAGeometrySoA.h"

using namespace caGeometry;

// --- Constants ---
constexpr uint16_t nModules = 1856;
constexpr uint16_t nLayers  = 10;
constexpr uint16_t nPairs   = 19;

// phi cut constants
constexpr int16_t phi0p05 = 522;
constexpr int16_t phi0p06 = 626;
constexpr int16_t phi0p07 = 730;

constexpr int16_t phicuts[nPairs]{
    phi0p05, phi0p07, phi0p07, phi0p05, phi0p06, phi0p06, phi0p05, phi0p05, phi0p06, phi0p06,
    phi0p06, phi0p05, phi0p05, phi0p05, phi0p05, phi0p05, phi0p05, phi0p05, phi0p05
};

constexpr float minz_vals[nPairs]{
    -20., 0., -30., -22., 10., -30., -70., -70., -22., 15., -30., -70., -70., -20., -22., 0, -30., -70., -70.
};
constexpr float maxz_vals[nPairs]{
    20., 30., 0., 22., 30., -10., 70., 70., 22., 30., -15., 70., 70., 20., 22., 30., 0., 70., 70.
};
constexpr float maxr_vals[nPairs]{
    20., 9., 9., 20., 7., 7., 5., 5., 20., 6., 6., 5., 5., 20., 20., 9., 9., 9., 9.
};

// Layer pairs
constexpr uint8_t layerPairs[2 * nPairs] = {
    0, 1, 0, 4, 0, 7,
    1, 2, 1, 4, 1, 7,
    4, 5, 7, 8,
    2, 3, 2, 4, 2, 7, 5, 6, 8, 9,
    0, 2, 1, 3,
    0, 5, 0, 8,
    4, 6, 7, 9
};

// startingPairs: first three true, rest false
constexpr uint8_t startingPairs_flags[nPairs] = {
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// layer start indices (nLayers+1 elements)
constexpr uint32_t layerStart[nLayers + 1] = {
    0,
    96,
    320,
    672,  // barrel
    1184,
    1296,
    1408, // positive endcap
    1520,
    1632,
    1744, // negative endcap
    nModules
};

// caThetaCuts and caDCACuts
constexpr float caDCACuts_vals[nLayers] = {
    0.15, 0.25, 0.25, 0.25, 0.25,
    0.25, 0.25, 0.25, 0.25, 0.25
};
constexpr float caThetaCuts_vals[nLayers] = {
    0.002, 0.002, 0.002, 0.002, 0.003,
    0.003, 0.003, 0.003, 0.003, 0.003
};

int write() {
    std::ifstream ifs("../../data/CMSPhase1Modules.bin", std::ios::binary);
    if (!ifs) {
        std::cerr << "Error: cannot open CMSPhase1Modules.bin for reading.\n";
        return 1;
    }

    std::vector<CAModule> modules(nModules);
    ifs.read(reinterpret_cast<char*>(modules.data()), modules.size() * sizeof(CAModule));
    if (!ifs) {
        std::cerr << "Error: failed to read all modules from file.\n";
        return 1;
    }
    ifs.close();

    std::vector<CALayer> layers(nLayers + 1);
    for (uint16_t i = 0; i < nLayers + 1; i++) {
        layers[i].layerStarts = static_cast<int32_t>(layerStart[i]);
        if (i < nLayers) {
            layers[i].caThetaCut = caThetaCuts_vals[i];
            layers[i].caDCACut   = caDCACuts_vals[i];
        } else {
            layers[i].caThetaCut = 0.0f;
            layers[i].caDCACut   = 0.0f;
        }
    }

    std::vector<CAPair> pairs(nPairs);
    for (uint16_t i = 0; i < nPairs; i++) {
        pairs[i].innerLayer   = layerPairs[2 * i];
        pairs[i].outerLayer   = layerPairs[2 * i + 1];
        pairs[i].phiCut       = phicuts[i];
        pairs[i].minz         = minz_vals[i];
        pairs[i].maxz         = maxz_vals[i];
        pairs[i].maxr         = maxr_vals[i];
        pairs[i].startingPair = startingPairs_flags[i];
    }

    CAGeometrySoA geo;
    geo.m_nModules = nModules;
    geo.m_nLayers  = nLayers;  // detector layers
    geo.m_nPairs   = nPairs;
    geo.m_modules  = modules.data();
    geo.m_layers   = layers.data();
    geo.m_pairs    = pairs.data();

    std::ofstream ofs("../../data/CMSPhase1Geometry.bin", std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(&geo.m_nModules), sizeof(geo.m_nModules));
    ofs.write(reinterpret_cast<const char*>(&geo.m_nLayers),  sizeof(geo.m_nLayers));
    ofs.write(reinterpret_cast<const char*>(&geo.m_nPairs),   sizeof(geo.m_nPairs));

    ofs.write(reinterpret_cast<const char*>(modules.data()), modules.size() * sizeof(CAModule));
    ofs.write(reinterpret_cast<const char*>(layers.data()),  layers.size()  * sizeof(CALayer));
    ofs.write(reinterpret_cast<const char*>(pairs.data()),   pairs.size()   * sizeof(CAPair));

    ofs.close();
    std::cout << "CMSPhase1Geometry.bin written.\n";

    return 0;
}

int verify() {
    std::ifstream ifs("../../data/CMSPhase1Geometry.bin", std::ios::binary);

    if (!ifs) {
        std::cerr << "Error opening CMSPhase1Geometry.bin\n";
        return 1;
    }

    uint16_t nMods, nLays, nP;
    ifs.read(reinterpret_cast<char*>(&nMods), sizeof(nMods));
    ifs.read(reinterpret_cast<char*>(&nLays), sizeof(nLays));
    ifs.read(reinterpret_cast<char*>(&nP), sizeof(nP));

    assert(nMods == nModules);
    assert(nLays == nLayers);
    assert(nP == nPairs);

    std::vector<CAModule> modules(nModules);
    std::vector<CALayer>  layers(nLayers + 1);
    std::vector<CAPair>   pairs(nPairs);

    ifs.read(reinterpret_cast<char*>(modules.data()), modules.size() * sizeof(CAModule));
    ifs.read(reinterpret_cast<char*>(layers.data()),  layers.size()  * sizeof(CALayer));
    ifs.read(reinterpret_cast<char*>(pairs.data()),   pairs.size()   * sizeof(CAPair));
    ifs.close();

    for (uint16_t i = 0; i < nLayers; ++i) {
        assert(layers[i].layerStarts == static_cast<int32_t>(layerStart[i]));
        assert(layers[i].caThetaCut == caThetaCuts_vals[i]);
        assert(layers[i].caDCACut   == caDCACuts_vals[i]);
    }

    for (uint16_t i = 0; i < nPairs; ++i) {
        assert(pairs[i].innerLayer   == layerPairs[2*i]);
        assert(pairs[i].outerLayer   == layerPairs[2*i+1]);
        assert(pairs[i].phiCut       == phicuts[i]);
        assert(pairs[i].minz         == minz_vals[i]);
        assert(pairs[i].maxz         == maxz_vals[i]);
        assert(pairs[i].maxr         == maxr_vals[i]);
        assert(pairs[i].startingPair == startingPairs_flags[i]);
    }

    std::cout << "All values verified successfully!\n";

    return 0;
}

int main() {
    write();
    verify();
    return 0;
}
