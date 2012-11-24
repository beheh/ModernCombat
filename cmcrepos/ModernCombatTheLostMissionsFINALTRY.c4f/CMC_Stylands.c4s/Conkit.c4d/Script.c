/*--- Bausatz ---*/

#strict

//public func Activate(pClonk)
public func ControlDigDouble(pClonk)
{ 
  [$TxtCreateconstructionsi$]
  // Clonk anhalten
  SetComDir(COMD_Stop(), pClonk);
  // Kann der Clonk überhaupt bauen?
  if (pClonk && !GetPhysical("CanConstruct", PHYS_Current, pClonk))
    { PlayerMessage(GetController(pClonk), "$TxtCantConstruct$", this, GetName(pClonk)); return false; }
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this(), 1, "$TxtNoconstructionplansa$");
  var idType; var i = 0;
  while (idType = GetPlrKnowledge(GetOwner(pClonk), 0, i++, C4D_Structure()))
    // Keine Trapper/Indianer Sachen mit Bausatz bauen. (Außer für Wachtürme/Palisaden, die es ausdrücklich zulassen)
    if(idType->~IsConkitBuilding() || (!idType->~IsIndianHandcraft() && !idType->~IsTrapperHandcraft()))
      AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk);
  return(1);
}

protected func CreateConstructionSite(idType)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  //if (GetAction(Contained()) ne "Walk") return(0);
  if (Contained(Contained())) return(0);
  // Pruefen, ob das Gebaeude hier gebaut werden kann
  if (idType->~RejectConstruction(0, 10, Contained()) ) return(0);
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  // Baustelle erschaffen
  var x = GetX();
  var y = GetY()+10;
  var w = GetDefWidth(idType)+6;
  var h = GetDefHeight(idType);
  //FreeRect(x-w/2,y-h/2,w,h);
  if(!idType->~BasementID())
    DrawMaterialRect("Earth-Smooth",x-w/2,y+11,w,11);
  var pSite;
  if(!(pSite = CreateConstruction(idType, 0, 10, GetOwner(Contained()), 1, 1,0))) return(0);
  // Effekt
  AddEffect("IntBuild",pSite,10,1,0,GetID());
  // Meldung
  Message("$TxtConstructions$", Contained(), GetName(pSite));
  // Bausatz verbrauchen
  RemoveObject();
  return(1);
}

protected func Hit()
{
  Sound("RockHit*");
	return(1);
}

func DrawMaterialRect(string mat, int x, int y, int w, int h)
{
  DrawMaterialQuad(mat, x, y, x + w, y, x + w, y - h, x, y - h, false);
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }


/* Bau-Effekt */

public func FxIntBuildTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var con = GetCon(pTarget);
  if(con >= 100) return(-1);
  
  DoCon(+1,pTarget);
  if(iEffectTime % 10) return(0);
  con++;

  var x = 0;
  var y = -GetDefHeight(GetID(pTarget))*con/100/2;
  var rgb = RGB(255-(con*255/100),0,con*255/100);
  var w = GetObjWidth(pTarget);
  
  for(var i = 10; i > 0; i--)
  {
    x = Random(w)-w/2;
    pTarget->CreateParticle("NoGravSpark",x,y,0,0,30,rgb,pTarget);
  }
  
  if(!Random(3))
    Sound("Build*");
}