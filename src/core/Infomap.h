/*
 * Infomap.h
 *
 *  Created on: 4 mar 2015
 *      Author: Daniel
 */

#ifndef INFOMAP_H_
#define INFOMAP_H_

#include "InfomapBase.h"
#include "../io/Config.h"
#include <set>
#include "../utils/VectorMap.h"
#include "../utils/infomath.h"
#include "InfoNode.h"
#include "FlowData.h"
#include <utility>
#include <tuple>
#include "MapEquation.h"
#include "MemMapEquation.h"
#include <memory>
#include "InfomapOptimizer.h"

namespace infomap {

class Infomap : public InfomapBase {
	// using Base = InfomapBase;
	// using FlowDataType = FlowData;
	// using DeltaFlowDataType = MemDeltaFlow;
	// using FlowDataType = typename Objective::FlowDataType;
	// using DeltaFlowDataType = typename Objective::DeltaFlowDataType;
	// template<typename T>
	// using ptr = std::shared_ptr<T>;
	// using MapEquationPtr = std::unique_ptr<MapEquation>;
	using OptimizerPtr = std::unique_ptr<InfomapOptimizerBase>;

protected:
//	using Base::EdgeType;
//	using EdgeType = Base::EdgeType;
	// using EdgeType = Edge<InfoNode>;
public:
	// template<typename... Args>
	// Infomap(Args&&... args) : InfomapBase(std::forward<Args>(args)...) {}
	Infomap(bool forceNoMemory = false) : InfomapBase() { initOptimizer(forceNoMemory); }
	Infomap(const Config& conf) : InfomapBase(conf) { initOptimizer(); }
	Infomap(const std::string& flags) : InfomapBase(flags) { initOptimizer(); }
	virtual ~Infomap() {}

	// ===================================================
	// IO
	// ===================================================

	virtual std::ostream& toString(std::ostream& out) const {
    return m_optimizer->toString(out);
  }

	// ===================================================
	// Getters
	// ===================================================

	virtual double getCodelength() const {
    return m_optimizer->getCodelength();  
  }

	virtual double getIndexCodelength() const {
    return m_optimizer->getIndexCodelength();  
  }

	virtual double getModuleCodelength() const {
    return m_optimizer->getModuleCodelength();  
  }

	bool haveMemory() const {
    // return m_optimizer->haveMemory();
    return this->isMemoryInput();
  }

protected:
	Infomap& initOptimizer(bool forceNoMemory = false)
	{
		if (haveMemory() && !forceNoMemory) {
			m_optimizer = OptimizerPtr(new InfomapOptimizer<MemMapEquation>());
		} else {
			m_optimizer = OptimizerPtr(new InfomapOptimizer<MapEquation>());
		}
    m_optimizer->init(this);
		return *this;
	}

	// virtual InfomapBase& getInfomap(InfoNode& node);
	virtual InfomapBase* getNewInfomapInstance() const {
    return new Infomap();
  }
	virtual InfomapBase* getNewInfomapInstanceWithoutMemory() const {
    return new Infomap(true);
  }

	virtual unsigned int numActiveModules() const {
    return m_optimizer->numActiveModules();
  }

	// ===================================================
	// Run: Init: *
	// ===================================================

	// Init terms that is constant for the whole network
	virtual void initNetwork() {
    return m_optimizer->initNetwork();
  }

	virtual void initSuperNetwork() {
    return m_optimizer->initSuperNetwork();
  }

	virtual double calcCodelength(const InfoNode& parent) const {
    return m_optimizer->calcCodelength(parent);
  }

	// ===================================================
	// Run: Partition: *
	// ===================================================

	virtual void initPartition() {
    return m_optimizer->initPartition();
  }

	virtual void moveActiveNodesToPredifinedModules(std::vector<unsigned int>& modules) {
    return m_optimizer->moveActiveNodesToPredifinedModules(modules);
  }

	virtual unsigned int optimizeActiveNetwork() {
    return m_optimizer->optimizeActiveNetwork();
  }
	
	virtual unsigned int tryMoveEachNodeIntoBestModule() {
    return m_optimizer->tryMoveEachNodeIntoBestModule();
  }
	
	// unsigned int tryMoveEachNodeIntoBestModuleLocal() {
  // }

	virtual unsigned int tryMoveEachNodeIntoBestModuleInParallel() {
    return m_optimizer->tryMoveEachNodeIntoBestModuleInParallel();
  }

	virtual void consolidateModules(bool replaceExistingModules = true) {
    return m_optimizer->consolidateModules(replaceExistingModules);
  }

	virtual bool restoreConsolidatedOptimizationPointIfNoImprovement(bool forceRestore = false) {
    return m_optimizer->restoreConsolidatedOptimizationPointIfNoImprovement(forceRestore);
  }

	// ===================================================
	// Debug: *
	// ===================================================

	virtual void printDebug() {
    return m_optimizer->printDebug();
  }

	// ===================================================
	// Protected members
	// ===================================================

  OptimizerPtr m_optimizer;
};


} /* namespace infomap */

#endif /* INFOMAP_H_ */
