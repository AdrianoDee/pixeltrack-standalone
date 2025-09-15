#ifdef GPU_DEBUG
#include <iostream>
#endif

#include "AlpakaCore/AllocatorPolicy.h"
#include "AlpakaCore/config.h"
#include "CondFormats/pixelCPEforGPU.h"

#include "PixelRecHits.h"
#include "gpuPixelRecHits.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  namespace {
    struct setHitsLayerStart {
      template <typename TAcc>
      ALPAKA_FN_ACC void operator()(const TAcc& acc,
                                    uint32_t const* __restrict__ hitsModuleStart,
                                    pixelCPEforGPU::ParamsOnGPU const* cpeParams,
                                    uint32_t* hitsLayerStart) const {
        ALPAKA_ASSERT_ACC(0 == hitsModuleStart[0]);

        cms::alpakatools::for_each_element_in_grid(acc, 11, [&](uint32_t i) {
          hitsLayerStart[i] = hitsModuleStart[cpeParams->layerGeometry().layerStart[i]];
#ifdef GPU_DEBUG
          printf("LayerStart %d %d: %d\n", i, cpeParams->layerGeometry().layerStart[i], hitsLayerStart[i]);
#endif
        });
      }
    };
  }  // namespace

  namespace pixelgpudetails {

    TrackingRecHit2DAlpaka PixelRecHitGPUKernel::makeHitsAsync(SiPixelDigisAlpaka const& digis_d,
                                                               SiPixelClustersAlpaka const& clusters_d,
                                                               BeamSpotAlpaka const& bs_d,
                                                               pixelCPEforGPU::ParamsOnGPU const* cpeParams,
                                                               Queue& queue) const {
      auto nHits = clusters_d.nClusters();
      TrackingRecHit2DAlpaka hits_d(nHits, clusters_d.clusModuleStart(), queue);

      const int threadsPerBlockOrElementsPerThread = 128;
      const int blocks = digis_d.nModules();  // active modules (with digis)
      const auto getHitsWorkDiv = cms::alpakatools::make_workdiv<Acc1D>(blocks, threadsPerBlockOrElementsPerThread);

#ifdef GPU_DEBUG
      std::cout << "launching getHits kernel for " << blocks << " blocks" << std::endl;
#endif
      if (blocks) {  // protect from empty events
        alpaka::enqueue(queue,
                        alpaka::createTaskKernel<Acc1D>(getHitsWorkDiv,
                                                        gpuPixelRecHits::getHits(),
                                                        cpeParams,
                                                        bs_d.data(),
                                                        digis_d.view(),
                                                        digis_d.nDigis(),
                                                        clusters_d.view(),
                                                        hits_d.view()));
      }

#ifdef GPU_DEBUG
      alpaka::wait(queue);
#endif

#if defined ALPAKA_ACC_CPU_B_TBB_T_SEQ_ASYNC_BACKEND && defined ALPAKA_DISABLE_CACHING_ALLOCATOR
      // FIXME this is required to keep the host buffer inside hits_d alive; it could be removed once the host buffers are also stream-ordered
      alpaka::wait(queue);
#endif

      return hits_d;
    }

  }  // namespace pixelgpudetails

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE
