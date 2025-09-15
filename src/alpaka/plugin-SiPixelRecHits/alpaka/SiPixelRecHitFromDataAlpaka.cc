// #include <iostream>

// #include "AlpakaCore/Product.h"
// #include "AlpakaCore/ScopedContext.h"
// #include "AlpakaCore/config.h"
// #include "AlpakaDataFormats/alpaka/BeamSpotAlpaka.h"
// #include "AlpakaDataFormats/alpaka/SiPixelClustersAlpaka.h"
// #include "AlpakaDataFormats/alpaka/SiPixelDigisAlpaka.h"
// #include "AlpakaDataFormats/alpaka/TrackingRecHit2DAlpaka.h"
// #include "CondFormats/alpaka/PixelCPEFast.h"
// #include "Framework/EDProducer.h"
// #include "Framework/Event.h"
// #include "Framework/EventSetup.h"
// #include "Framework/PluginFactory.h"

// #include "PixelRecHits.h"  // TODO : split product from kernel

// namespace ALPAKA_ACCELERATOR_NAMESPACE {

//   class SiPixelRecHitFromSimple : public edm::EDProducer {
//   public:
//     explicit SiPixelRecHitFromSimple(edm::ProductRegistry& reg);
//     ~SiPixelRecHitFromSimple() override = default;

//   private:
//     void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;

//     edm::EDGetTokenT<TrackingRecHitSimpleSoA> tSimpleHits_;

//     edm::EDPutTokenT<cms::alpakatools::Product<Queue, TrackingRecHit2DAlpaka>> tokenHit_;
//     cms::alpakatools::host_buffer<BeamSpotPOD> bsHost_;

//   };

//   SiPixelRecHitFromSimple::SiPixelRecHitFromSimple(edm::ProductRegistry& reg)
//       : tBeamSpot(reg.consumes<cms::alpakatools::Product<Queue, BeamSpotAlpaka>>()),
//         tSimpleHits_(reg.consumes<cms::alpakatools::Product<Queue, SiPixelClustersAlpaka>>()),
//         tokenDigi_(reg.consumes<cms::alpakatools::Product<Queue, SiPixelDigisAlpaka>>()),
//         bsHost_{cms::alpakatools::make_host_buffer<BeamSpotPOD, Platform>()} {}

//   void SiPixelRecHitFromSimple::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

//     *bsHost_ = iSetup.get<BeamSpotPOD>();

//     auto const& pclusters = iEvent.get(token_);
//     cms::alpakatools::ScopedContextProduce<Queue> ctx{pclusters};

//     auto const& clusters = ctx.get(pclusters);
//     auto const& digis = ctx.get(iEvent, tokenDigi_);
//     auto const& bs = ctx.get(iEvent, tBeamSpot);

//     auto nHits = clusters.nClusters();
//     if (nHits >= TrackingRecHit2DSoAView::maxHits()) {
//       std::cout << "Clusters/Hits Overflow " << nHits << " >= " << TrackingRecHit2DSoAView::maxHits() << std::endl;
//     }
//     ctx.emplace(iEvent,
//                 tokenHit_,
//                 gpuAlgo_.makeHitsAsync(digis, clusters, bs, fcpe.getGPUProductAsync(ctx.stream()), ctx.stream()));
//   }

// }  // namespace ALPAKA_ACCELERATOR_NAMESPACE

// DEFINE_FWK_ALPAKA_MODULE(SiPixelRecHitFromSimple);
