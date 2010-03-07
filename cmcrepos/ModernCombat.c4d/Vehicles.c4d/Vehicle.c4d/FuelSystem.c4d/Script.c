/*-- Treibstoff-System --*/

#strict

static const FUEL_PETROL = 0;
static const FUEL_ENERGY = 1;

local fuel;

public func Initialize()
{
  //blah
  return(_inherited());
}

//public func GetFuelType(){return(VC4K_FUEL_PETROL);}

public func GetFuel(){return(fuel);}
public func SetFuel(int iAmount){fuel = iAmount;}

public func DoFuel(int iAmount)
{
  fuel += iAmount;
  return(fuel);
}

public func TransferFuel(object pSource, object pDestination)
{
  //blah
}