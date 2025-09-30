#include <iostream>

#include "AlpakaCore/Product.h"
#include "AlpakaCore/ScopedContext.h"
#include "AlpakaCore/config.h"
#include "DataFormats/BeamSpotPOD.h"
#include "AlpakaDataFormats/alpaka/TrackingRecHit2DAlpaka.h"
#include "DataFormats/TrackingRecHitSimpleSoA.h"
#include "Framework/EDProducer.h"
#include "Framework/Event.h"
#include "Framework/EventSetup.h"
#include "Framework/PluginFactory.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE {

  class SiPixelRecHitFromSimple : public edm::EDProducer {
  public:
    explicit SiPixelRecHitFromSimple(edm::ProductRegistry& reg);
    ~SiPixelRecHitFromSimple() override = default;

  private:
    void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;
    
    cms::alpakatools::host_buffer<BeamSpotPOD> bsHost_;
    edm::EDGetTokenT<TrackingRecHitSimpleSoA> tSimpleHits_;
    edm::EDPutTokenT<cms::alpakatools::Product<Queue, TrackingRecHit2DAlpaka>> tHits_;

  };

  SiPixelRecHitFromSimple::SiPixelRecHitFromSimple(edm::ProductRegistry& reg)
      : bsHost_{cms::alpakatools::make_host_buffer<BeamSpotPOD, Platform>()},
        tSimpleHits_(reg.consumes<TrackingRecHitSimpleSoA>()),
        tHits_(reg.produces<cms::alpakatools::Product<Queue, TrackingRecHit2DAlpaka>>()) {}

  void SiPixelRecHitFromSimple::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    *bsHost_ = iSetup.get<BeamSpotPOD>();

    cms::alpakatools::ScopedContextProduce<Queue> ctx{iEvent.streamID()};

    auto const& simpleHits = iEvent.get(tSimpleHits_);
    TrackingRecHit2DAlpaka hits(simpleHits, ctx.stream());

    std::cout << __LINE__ << " -- " << __FILE__ << std::endl;
    std::cout << "sizeof(hits.hitsModuleStart()): " << sizeof(hits.hitsModuleStart()) << std::endl;
    std::cout << "hits.hitsModuleStart()[0]: " << hits.hitsModuleStart()[0] << std::endl;
    std::cout << "hits.hitsModuleStart()[1]: " << hits.hitsModuleStart()[1] << std::endl;
    std::cout << "hits.hitsModuleStart()[2]: " << hits.hitsModuleStart()[2] << std::endl;
    std::cout << "hits.hitsModuleStart()[3]: " << hits.hitsModuleStart()[3] << std::endl;
    std::cout << "hits.hitsModuleStart()[4]: " << hits.hitsModuleStart()[4] << std::endl;
    std::cout << "hits.hitsModuleStart()[5]: " << hits.hitsModuleStart()[5] << std::endl;
    std::cout << "hits.hitsModuleStart()[6]: " << hits.hitsModuleStart()[6] << std::endl;
    std::cout << "hits.hitsModuleStart()[7]: " << hits.hitsModuleStart()[7] << std::endl;
    std::cout << "hits.hitsModuleStart()[8]: " << hits.hitsModuleStart()[8] << std::endl;
    std::cout << __LINE__ << " -- " << __FILE__ << std::endl;

    ctx.emplace(iEvent, tHits_, std::move(hits));
    
  }

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE

DEFINE_FWK_ALPAKA_MODULE(SiPixelRecHitFromSimple);
