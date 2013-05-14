/*-- NUR OPEN BETA --*/

#strict 2

#appendto BBTP


/* Gegnererkennung */

private func Check()
{
  //Nicht aktiv, nicht suchen
  if(!bReady) return;

  //Kein Besitzer mehr: Verschwinden
  if(GetOwner() == NO_OWNER)
  {
    //Effekte
    Sparks(2,RGB(250,100));
    Sparks(2,RGB(0,200));
    if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",4, 10, 0, 0, 120, 140, RGBa(255,255,255,100), RGBa(255,255,255,130));
    Sound("Limitation.ogg");

    return RemoveObject();
  }

  var pVictim;
  for( var pVictim in FindObjects(Find_OnLine(0,0,Sin(iDir,80,1),-Cos(iDir,80,1)), 
				Find_Func("CheckEnemy",this,0,true),			//Gegner?
				Find_Not(Find_Distance(10)),				//Nah genug?
				Find_OCF(OCF_Alive),					//Lebewesen?
				Find_NoContainer()) )					//Im Freien?
    if(!pVictim->~HitExclude(this) && PathFree(GetX(),GetY(),GetX(pVictim),GetY(pVictim)))				//Freies Schussfeld?
      Detonate();
  return 1;
}
