/*--- Werkzeugkasten ---*/

#strict 2

#include PACK

static EngiPlans, EngiBuildLimits, EngiStorage;

public func RefillTime() {return 30;}
public func Initialize()
{
	if(!EngiPlans)
	{
		EngiPlans = [BBWE, BNGT, HLSM];
		EngiBuildLimits = [10, 4, 5];
		EngiStorage = CreateArray(GetLength(EngiPlans));
	
		for(var i = 0; i < GetLength(EngiPlans); i++)
			EngiStorage[i] = CreateArray(GetTeamCount());
	}
	return _inherited(...);
}

public func Activate(pClonk)
{ 
  [$TxtCreateconstructionsi$]
    
  // Clonk anhalten
  SetComDir(COMD_Stop, pClonk);
  // Kann der Clonk überhaupt bauen?
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this(), 1, "$TxtNoConstructionPlans$");
  var idType; var i = 0;
  for(i = 0; i < GetLength(EngiPlans); i++)
  {
    idType = EngiPlans[i];
    AddMenuItem(Format("%d: $TxtConstructions$", idType->BuildCost()), "CreateConstructionSite", idType, pClonk);
  }
  return(1);
}

protected func CreateConstructionSite(idType)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (!WildcardMatch(GetAction(Contained()), "Walk*")) return;
  if (Contained(Contained())) return;
	if(GetPackPoints() < idType->BuildCost()) return PlayerMessage(GetOwner(), "Not enough points!", this);
	
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  var obj = CreateObject(idType, 0, 0, GetOwner());
  
  var index;
  for(var i = 0; i < GetLength(EngiPlans); i++)
  	if(EngiPlans[i] == idType)
  		index = i;

  var team = GetPlayerTeam(GetOwner());
  if(!EngiStorage[index][team])
 		EngiStorage[index][team] = CreateArray(EngiBuildLimits[index]);
  
  var best;
  var bestNumber = -1;
  for(var i = 0; i < EngiBuildLimits[index]; i++)
  {
  	if(!EngiStorage[index][team][i] || EngiStorage[index][team][i]->IsDestroyed())
  	{
  		best = i;
  		break;
  	}
  	
  	if(bestNumber == -1 || ObjectNumber(EngiStorage[index][team][i]) < bestNumber)
  	{
  		best = i;
  		bestNumber = ObjectNumber(EngiStorage[index][team][i]);
  	}
  }
  
  if(EngiStorage[index][team][best] && !EngiStorage[index][team][best]->IsDestroyed())
  	EngiStorage[index][team][best]->Replaced();
  	
  EngiStorage[index][team][best] = obj;
  
  if(idType == BNGT)
  {
  	if(GetDir(Contained()) == DIR_Left)
  		obj->SetRotation(-90);
  	else
  		obj->SetRotation(90);
  }
  
  // Bausatz verbrauchen
  DoPackPoints(-idType->BuildCost());

  return(1);
}

protected func Hit()
{
  Sound("RockHit*");
	return(1);
}
