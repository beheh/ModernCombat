/*--- Werkzeugkasten ---*/

#strict 2

#include PACK

local plans;

public func RefillTime() {return 30;}
public func Initialize() {plans = [BDWE, BNGT, HLSM]; return _inherited(...);}

public func Activate(pClonk)
{ 
  [$TxtCreateconstructionsi$]
    
  // Clonk anhalten
  SetComDir(COMD_Stop, pClonk);
  // Kann der Clonk überhaupt bauen?
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this(), 1, "$TxtNoconstructionplansa$");
  var idType; var i = 0;
  for(i = 0; i < GetLength(plans); i++)
  {
    idType = plans[i];
    AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk);
  }
  return(1);
}

protected func CreateConstructionSite(idType)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (!WildcardMatch(GetAction(Contained()), "Walk*")) return(0);
  if (Contained(Contained())) return(0);
	if(GetPackPoints() < idType->BuildCost()) return 0;
	
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  var obj = CreateObject(idType);
  if(idType == BNGT)
  {
  	if(GetDir(Contained()) == DIR_Left)
  		obj->SetRotation(-90);
  	else
  		obj->SetRotation(90);
  }
  
  DoPackPoints(-idType->BuildCost());

  // Bausatz verbrauchen
  

  return(1);
}

protected func Hit()
{
  Sound("RockHit*");
	return(1);
}
