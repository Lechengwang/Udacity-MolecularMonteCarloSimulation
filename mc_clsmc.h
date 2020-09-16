#include <memory>

#ifndef _MC_CLSMC_H
#define _MC_CLSMC_H 1

class RNDGenerator;
class Potential;
class MCSettings;

class MCMover{
public:
  MCMover(std::shared_ptr<MCSettings>, std::shared_ptr<RNDGenerator>, std::shared_ptr<Potential>);
  ~MCMover();
  void MCMove();
  double MCPot(int,double **);
  double getAccRatio();
  double ** getMCCoords();
private:
  double _MCTotal;
  double _MCAccep;
  std::shared_ptr<MCSettings> _mcSettings;
  std::shared_ptr<RNDGenerator> _rnd;
  std::shared_ptr<Potential> _pot;
  double ** _newcoords;
  double ** _MCCoords;
};
#endif
