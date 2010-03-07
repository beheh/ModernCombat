/*-- Fahrzeug-Kit --*/
//*bruum* *bruuuuuuum*

#strict
#include L_SE
#include FUEL

static const V_FUELTYPE = 0;
//static const V_... = 1;

public func Initialize()
{
  //blah
  return(_inherited());
}

public func GetVData(int iData)
{
  //Soll im Fahrzeug ausgefüllt werden.
  return(Default(iData));
}

public func Default(int iData)
{
  if(iData == V_FUELTYPE) return(FUEL_PETROL);
  //if(iData == ) return();
}