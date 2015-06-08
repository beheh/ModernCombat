/*-- Klassenwahl --*/

#strict 2

#appendto MCSL


static const MCSL_Spawnprotection_Time = 108; //3 Sekunden

public func FxSpawnprotectionStart(object pTarget, int iNr)
{
  EffectVar(0, pTarget, iNr) = CreateContents(ROCK, pTarget);
  while(Contents(0, pTarget) != EffectVar(0, pTarget, iNr))
    ShiftContents(pTarget);

  //Effekte um den Spawnschutz zu kennzeichnen

  //Platzhaltereffekt: Leicht ausblenden
  SetClrModulation(RGBa(255, 255, 255, 50), pTarget);
}

public func FxSpawnprotectionTimer(object pTarget, int iNr, int iTime)
{
  if(!EffectVar(0, pTarget, iNr) || Contents(0, pTarget) != EffectVar(0, pTarget, iNr) || iTime >= MCSL_Spawnprotection_Time)
  {
    if(EffectVar(0, pTarget, iNr))
      RemoveObject(EffectVar(0, pTarget, iNr));

    return -1;
  }

  return true;
}

public func FxSpawnprotectionDamage(object pTarget, int iNr)
{
  return 0;
}

public func FxSpawnprotectionStop(object pTarget, int iNr)
{
  SetClrModulation(RGB(255, 255, 255), pTarget);
  return true;
}