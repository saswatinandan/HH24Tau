#ifndef ROO_CMS_SHAPE
#define ROO_CMS_SHAPE

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"
#include "TMath.h"
#include "RooMath.h"

class RooCMSShape : public RooAbsPdf {
 public:
  RooCMSShape() {};
  RooCMSShape(const char *name, const char *title,
              RooAbsReal& _x,
              RooAbsReal& _alpha,
              RooAbsReal& _beta,
              RooAbsReal& _gamma,
              RooAbsReal& _peak);

  RooCMSShape(const RooCMSShape& other, const char* name);
  inline TObject* clone(const char* newname) const override { return new RooCMSShape(*this,newname); }
  inline ~RooCMSShape() override {}
  Double_t evaluate() const override ;


  ClassDefOverride(RooCMSShape,1);

 protected:

  RooRealProxy x ;
  RooRealProxy alpha ;
  RooRealProxy beta ;
  RooRealProxy gamma ;
  RooRealProxy peak ;

};
#endif


