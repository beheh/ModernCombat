/*-- Roter Phosphor --*/

#strict 2
#include PSPR

local light,last_size,life;


/* Initialisierung */

func Initialize()
{
  //Brennzeit setzen
  life = 550+Random(100);

  //Effekte
  light = AddLight(40,RGB());
  AddFireEffect(this,0,RGB(0,255,255),0,-5 - Random(15));

  //Besitzer setzen und Timer ausführen
  SetController(GetOwner());
  Timer();

  return 1;
}

/* Klebeeffekt */

public func FxPhosphoredStart(pTarget, iNo, iTemp, pPhosphor)
{
  if(iTemp)
    return -1;
  if(GetLength(FindObjects(Find_ID(GetID()), Find_ActionTarget(pTarget))) > 2)
    return -1;
  EffectVar(0, pTarget, iNo) = this;
  SetAction("AGlobbing", pTarget);
}
